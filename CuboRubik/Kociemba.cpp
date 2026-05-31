#include "Kociemba.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>

// Basic moves definition
// 0: U, 1: U2, 2: U', 3: R, 4: R2, 5: R', 6: F, 7: F2, 8: F', 9: D, 10: D2, 11: D', 12: L, 13: L2, 14: L', 15: B, 16: B2, 17: B'
CubieCube moveCube[6];

// Transition tables
short twistMove[2187][18];
short flipMove[2048][18];
short sliceMove[495][18];
unsigned short cornerPermMove[40320][18];
unsigned short edgePermMove[40320][18];
short sliceSortedMove[24][18];

// Pruning tables
char prunTwistSlice[2187 * 495];
char prunFlipSlice[2048 * 495];
char prunCornerPermSlice[40320 * 24];
char prunEdgePermSlice[40320 * 24];

CubieCube::CubieCube() {
    for (int i = 0; i < 8; i++) { cp[i] = i; co[i] = 0; }
    for (int i = 0; i < 12; i++) { ep[i] = i; eo[i] = 0; }
}

void CubieCube::cornerMultiply(const CubieCube& b) {
    CubieCube c;
    for (int i = 0; i < 8; i++) {
        c.cp[i] = cp[b.cp[i]];
        c.co[i] = (co[b.cp[i]] + b.co[i]) % 3;
    }
    for (int i = 0; i < 8; i++) {
        cp[i] = c.cp[i];
        co[i] = c.co[i];
    }
}

void CubieCube::edgeMultiply(const CubieCube& b) {
    CubieCube c;
    for (int i = 0; i < 12; i++) {
        c.ep[i] = ep[b.ep[i]];
        c.eo[i] = (eo[b.ep[i]] + b.eo[i]) % 2;
    }
    for (int i = 0; i < 12; i++) {
        ep[i] = c.ep[i];
        eo[i] = c.eo[i];
    }
}

void CubieCube::multiply(const CubieCube& b) {
    cornerMultiply(b);
    edgeMultiply(b);
}

int Cnk(int n, int k) {
    if (n < k || k < 0) return 0;
    if (k == 0) return 1;
    int s = 1;
    for (int i = 0; i < k; i++) {
        s *= (n - i);
        s /= (i + 1);
    }
    return s;
}

int KociembaSolver::getTwist(const CubieCube& c) {
    int ret = 0;
    for (int i = 0; i < 7; i++) ret = 3 * ret + c.co[i];
    return ret;
}

void KociembaSolver::setTwist(CubieCube& c, int twist) {
    int twistParity = 0;
    for (int i = 6; i >= 0; i--) {
        c.co[i] = twist % 3;
        twistParity += c.co[i];
        twist /= 3;
    }
    c.co[7] = (3 - twistParity % 3) % 3;
}

int KociembaSolver::getFlip(const CubieCube& c) {
    int ret = 0;
    for (int i = 0; i < 11; i++) ret = 2 * ret + c.eo[i];
    return ret;
}

void KociembaSolver::setFlip(CubieCube& c, int flip) {
    int flipParity = 0;
    for (int i = 10; i >= 0; i--) {
        c.eo[i] = flip % 2;
        flipParity += c.eo[i];
        flip /= 2;
    }
    c.eo[11] = (2 - flipParity % 2) % 2;
}

int KociembaSolver::getSlice(const CubieCube& c) {
    int a = 0;
    int x = 0;
    for (int j = 11; j >= 0; j--) {
        if (c.ep[j] >= 8) {
            a += Cnk(11 - j, x + 1);
            x++;
        }
    }
    return a;
}

void KociembaSolver::setSlice(CubieCube& c, int slice) {
    int sliceEdge[4] = {8, 9, 10, 11};
    int otherEdge[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int a = slice;
    int x = 4;
    for (int j = 0; j < 12; j++) { c.ep[j] = -1; }
    for (int j = 0; j < 12; j++) {
        if (a - Cnk(11 - j, x) >= 0) {
            c.ep[j] = sliceEdge[4 - x];
            a -= Cnk(11 - j, x--);
        }
    }
    int x2 = 0;
    for (int j = 0; j < 12; j++) {
        if (c.ep[j] == -1) c.ep[j] = otherEdge[x2++];
    }
}

int KociembaSolver::getCornerPerm(const CubieCube& c) {
    int b = 0;
    for (int j = 7; j > 0; j--) {
        int k = 0;
        for (int i = 0; i < j; i++) {
            if (c.cp[i] > c.cp[j]) k++;
        }
        b = (b + k) * j;
    }
    return b;
}

void KociembaSolver::setCornerPerm(CubieCube& c, int idx) {
    int b = idx;
    std::vector<int> cp(8);
    for (int i = 0; i < 8; i++) cp[i] = i;
    for (int j = 1; j < 8; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            std::swap(cp[j], cp[j-k-1]);
        }
    }
    for (int i=0; i<8; i++) c.cp[i] = cp[i];
}

int KociembaSolver::getEdgePerm(const CubieCube& c) {
    int b = 0;
    for (int j = 7; j > 0; j--) {
        int k = 0;
        for (int i = 0; i < j; i++) {
            if (c.ep[i] > c.ep[j]) k++;
        }
        b = (b + k) * j;
    }
    return b;
}

void KociembaSolver::setEdgePerm(CubieCube& c, int idx) {
    int b = idx;
    std::vector<int> ep(8);
    for (int i = 0; i < 8; i++) ep[i] = i;
    for (int j = 1; j < 8; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            std::swap(ep[j], ep[j-k-1]);
        }
    }
    for (int i=0; i<8; i++) c.ep[i] = ep[i];
}

int KociembaSolver::getSliceSorted(const CubieCube& c) {
    int b = 0;
    for (int j = 3; j > 0; j--) {
        int k = 0;
        for (int i = 0; i < j; i++) {
            if (c.ep[i + 8] > c.ep[j + 8]) k++;
        }
        b = (b + k) * j;
    }
    return b;
}

void KociembaSolver::setSliceSorted(CubieCube& c, int idx) {
    int b = idx;
    std::vector<int> ep(4);
    for (int i = 0; i < 4; i++) ep[i] = i + 8;
    for (int j = 1; j < 4; j++) {
        int k = b % (j + 1);
        b /= j + 1;
        while (k-- > 0) {
            std::swap(ep[j], ep[j-k-1]);
        }
    }
    for(int i=0; i<12; i++) c.ep[i] = -1;
    for(int i=8; i<12; i++) c.ep[i] = ep[i-8];
}

void KociembaSolver::initTables() {
    // Initialize base moves U, R, F, D, L, B
    CubieCube U, R, F, D, L, B;
    U.cp = {3, 0, 1, 2, 4, 5, 6, 7};
    U.ep = {3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11};
    
    R.cp = {0, 1, 2, 3, 4, 5, 6, 7}; // Corrected below
    // R: URF(0)->UBR(3)->DBR(7)->DFR(4)
    R.cp[0]=4; R.cp[3]=0; R.cp[7]=3; R.cp[4]=7;
    R.co[0]=2; R.co[3]=1; R.co[7]=2; R.co[4]=1;
    // R edges: UR(0)->BR(11)->DR(4)->FR(8)
    R.ep[0]=8; R.ep[11]=0; R.ep[4]=11; R.ep[8]=4;

    F.cp = {0, 1, 2, 3, 4, 5, 6, 7};
    // F: UFL(1)->URF(0)->DFR(4)->DFL(5)
    F.cp[1]=5; F.cp[0]=1; F.cp[4]=0; F.cp[5]=4;
    F.co[1]=1; F.co[0]=2; F.co[4]=1; F.co[5]=2;
    // F edges: UF(1)->FR(8)->DF(5)->FL(9)
    F.ep[1]=9; F.ep[8]=1; F.ep[5]=8; F.ep[9]=5;
    F.eo[1]=1; F.eo[8]=1; F.eo[5]=1; F.eo[9]=1; // F/B flip edges

    D.cp = {0, 1, 2, 3, 5, 6, 7, 4};
    D.ep = {0, 1, 2, 3, 5, 6, 7, 4, 8, 9, 10, 11};

    L.cp = {0, 1, 2, 3, 4, 5, 6, 7};
    // L: ULB(2)->UFL(1)->DFL(5)->DLB(6)
    L.cp[2]=6; L.cp[1]=2; L.cp[5]=1; L.cp[6]=5;
    L.co[2]=2; L.co[1]=1; L.co[5]=2; L.co[6]=1;
    // L edges: UL(2)->FL(9)->DL(6)->BL(10)
    L.ep[2]=10; L.ep[9]=2; L.ep[6]=9; L.ep[10]=6;

    B.cp = {0, 1, 2, 3, 4, 5, 6, 7};
    // B: UBR(3)->ULB(2)->DLB(6)->DBR(7)
    B.cp[3]=7; B.cp[2]=3; B.cp[6]=2; B.cp[7]=6;
    B.co[3]=1; B.co[2]=2; B.co[6]=1; B.co[7]=2;
    // B edges: UB(3)->BL(10)->DB(7)->BR(11)
    B.ep[3]=11; B.ep[10]=3; B.ep[7]=10; B.ep[11]=7;
    B.eo[3]=1; B.eo[10]=1; B.eo[7]=1; B.eo[11]=1;

    moveCube[0] = U; moveCube[1] = R; moveCube[2] = F;
    moveCube[3] = D; moveCube[4] = L; moveCube[5] = B;

    std::cout << "  - TwistMove table..." << std::endl;
    // Generate Twist Move Table
    for (int i = 0; i < 2187; i++) {
        CubieCube a;
        setTwist(a, i);
        for (int j = 0; j < 6; j++) {
            CubieCube b = a;
            for (int k = 0; k < 3; k++) {
                b.cornerMultiply(moveCube[j]);
                twistMove[i][j * 3 + k] = getTwist(b);
            }
        }
    }

    std::cout << "  - FlipMove table..." << std::endl;
    // Generate Flip Move Table
    for (int i = 0; i < 2048; i++) {
        CubieCube a;
        setFlip(a, i);
        for (int j = 0; j < 6; j++) {
            CubieCube b = a;
            for (int k = 0; k < 3; k++) {
                b.edgeMultiply(moveCube[j]);
                flipMove[i][j * 3 + k] = getFlip(b);
            }
        }
    }

    std::cout << "  - SliceMove table..." << std::endl;
    // Generate Slice Move Table
    for (int i = 0; i < 495; i++) {
        CubieCube a;
        setSlice(a, i);
        for (int j = 0; j < 6; j++) {
            CubieCube b = a;
            for (int k = 0; k < 3; k++) {
                b.edgeMultiply(moveCube[j]);
                sliceMove[i][j * 3 + k] = getSlice(b);
            }
        }
    }

    std::cout << "  - Phase 1 Pruning Tables..." << std::endl;
    // Initialize Phase 1 Pruning Tables
    std::fill(std::begin(prunTwistSlice), std::end(prunTwistSlice), -1);
    prunTwistSlice[0] = 0;
    int depth = 0;
    int done = 1;
    int oldDone = 0;
    while (done > oldDone) {
        oldDone = done;
        for (int i = 0; i < 2187 * 495; i++) {
            if (prunTwistSlice[i] == depth) {
                int t = i / 495;
                int s = i % 495;
                for (int j = 0; j < 18; j++) {
                    int nextT = twistMove[t][j];
                    int nextS = sliceMove[s][j];
                    if (prunTwistSlice[nextT * 495 + nextS] == -1) {
                        prunTwistSlice[nextT * 495 + nextS] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    std::fill(std::begin(prunFlipSlice), std::end(prunFlipSlice), -1);
    prunFlipSlice[0] = 0;
    depth = 0;
    done = 1;
    oldDone = 0;
    while (done > oldDone) {
        oldDone = done;
        for (int i = 0; i < 2048 * 495; i++) {
            if (prunFlipSlice[i] == depth) {
                int f = i / 495;
                int s = i % 495;
                for (int j = 0; j < 18; j++) {
                    int nextF = flipMove[f][j];
                    int nextS = sliceMove[s][j];
                    if (prunFlipSlice[nextF * 495 + nextS] == -1) {
                        prunFlipSlice[nextF * 495 + nextS] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    std::cout << "  - Phase 2 Move tables..." << std::endl;
    // Phase 2 requires: U, D, R2, L2, F2, B2 (moves 0..2, 9..11, 4, 13, 7, 16)
    int phase2Moves[] = {0, 1, 2, 4, 7, 9, 10, 11, 13, 16};
    
    // Generate Phase 2 move tables
    for (int i = 0; i < 40320; i++) {
        CubieCube a;
        setCornerPerm(a, i);
        for (int m : phase2Moves) {
            CubieCube b = a;
            int baseMove = m / 3;
            int times = (m % 3) + 1;
            for(int k=0; k<times; k++) b.cornerMultiply(moveCube[baseMove]);
            cornerPermMove[i][m] = getCornerPerm(b);
        }
    }

    for (int i = 0; i < 40320; i++) {
        CubieCube a;
        setEdgePerm(a, i);
        for (int m : phase2Moves) {
            CubieCube b = a;
            int baseMove = m / 3;
            int times = (m % 3) + 1;
            for(int k=0; k<times; k++) b.edgeMultiply(moveCube[baseMove]);
            edgePermMove[i][m] = getEdgePerm(b);
        }
    }

    for (int i = 0; i < 24; i++) {
        CubieCube a;
        setSliceSorted(a, i);
        for (int m : phase2Moves) {
            CubieCube b = a;
            int baseMove = m / 3;
            int times = (m % 3) + 1;
            for(int k=0; k<times; k++) b.edgeMultiply(moveCube[baseMove]);
            sliceSortedMove[i][m] = getSliceSorted(b);
        }
    }

    std::cout << "  - Phase 2 Pruning tables (Corner)..." << std::endl;
    // Phase 2 Pruning Tables
    std::fill(std::begin(prunCornerPermSlice), std::end(prunCornerPermSlice), -1);
    prunCornerPermSlice[0] = 0;
    depth = 0; done = 1; oldDone = 0;
    while (done > oldDone) {
        oldDone = done;
        for (int i = 0; i < 40320 * 24; i++) {
            if (prunCornerPermSlice[i] == depth) {
                int cp = i / 24;
                int s = i % 24;
                for (int j : phase2Moves) {
                    int nextCP = cornerPermMove[cp][j];
                    int nextS = sliceSortedMove[s][j];
                    if (prunCornerPermSlice[nextCP * 24 + nextS] == -1) {
                        prunCornerPermSlice[nextCP * 24 + nextS] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }

    std::cout << "  - Phase 2 Pruning tables (Edge)..." << std::endl;
    std::fill(std::begin(prunEdgePermSlice), std::end(prunEdgePermSlice), -1);
    prunEdgePermSlice[0] = 0;
    depth = 0; done = 1; oldDone = 0;
    while (done > oldDone) {
        oldDone = done;
        for (int i = 0; i < 40320 * 24; i++) {
            if (prunEdgePermSlice[i] == depth) {
                int ep = i / 24;
                int s = i % 24;
                for (int j : phase2Moves) {
                    int nextEP = edgePermMove[ep][j];
                    int nextS = sliceSortedMove[s][j];
                    if (nextEP < 0 || nextEP >= 40320 || nextS < 0 || nextS >= 24) {
                        std::cout << "CRASH! nextEP=" << nextEP << " nextS=" << nextS << " ep=" << ep << " j=" << j << std::endl;
                        exit(1);
                    }
                    if (prunEdgePermSlice[nextEP * 24 + nextS] == -1) {
                        prunEdgePermSlice[nextEP * 24 + nextS] = depth + 1;
                        done++;
                    }
                }
            }
        }
        depth++;
    }
    std::cout << "  - initTables finished!" << std::endl;
}
