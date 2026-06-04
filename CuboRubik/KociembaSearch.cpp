#include "Kociemba.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

extern short twistMove[2187][18];
extern short flipMove[2048][18];
extern short sliceMove[495][18];
extern unsigned short cornerPermMove[40320][18];
extern unsigned short edgePermMove[40320][18];
extern short sliceSortedMove[24][18];

extern char prunTwistSlice[2187 * 495];
extern char prunFlipSlice[2048 * 495];
extern char prunCornerPermSlice[40320 * 24];
extern char prunEdgePermSlice[40320 * 24];

CoordCube::CoordCube(const CubieCube& c) {
    twist = KociembaSolver::getTwist(c);
    flip = KociembaSolver::getFlip(c);
    slice = KociembaSolver::getSlice(c);
    prun1 = std::max(prunTwistSlice[twist * 495 + slice], prunFlipSlice[flip * 495 + slice]);

    cornerPerm = KociembaSolver::getCornerPerm(c);
    edgePerm = KociembaSolver::getEdgePerm(c);
    sliceSorted = KociembaSolver::getSliceSorted(c);
    prun2 = std::max(prunCornerPermSlice[cornerPerm * 24 + sliceSorted], prunEdgePermSlice[edgePerm * 24 + sliceSorted]);
}

void CoordCube::move(int m) {
    twist = twistMove[twist][m];
    flip = flipMove[flip][m];
    slice = sliceMove[slice][m];
    prun1 = std::max(prunTwistSlice[twist * 495 + slice], prunFlipSlice[flip * 495 + slice]);

    cornerPerm = cornerPermMove[cornerPerm][m];
    edgePerm = edgePermMove[edgePerm][m];
    sliceSorted = sliceSortedMove[sliceSorted][m];
    prun2 = std::max(prunCornerPermSlice[cornerPerm * 24 + sliceSorted], prunEdgePermSlice[edgePerm * 24 + sliceSorted]);
}

std::string KociembaSolver::moveToString(int m) {
    std::string faces[] = {"U", "R", "F", "D", "L", "B"};
    std::string suffixes[] = {"", "2", "'"};
    return faces[m / 3] + suffixes[m % 3];
}

bool KociembaSolver::searchPhase1(CoordCube node, int depth, int maxDepth, int lastMove, std::vector<int>& path) {
    if (depth == maxDepth) {
        if (node.twist == 0 && node.flip == 0 && node.slice == 0) {
            // Found Phase 1! Now start Phase 2
            int maxP2 = 30 - maxDepth; // Example limit
            for (int p2Limit = 0; p2Limit <= maxP2; p2Limit++) {
                if (searchPhase2(node, 0, p2Limit, lastMove, path)) {
                    return true;
                }
            }
        }
        return false;
    }

    if (depth + node.prun1 > maxDepth) return false;

    for (int i = 0; i < 18; i++) {
        // Skip same face or opposite face commutativity
        if (lastMove != -1 && i / 3 == lastMove / 3) continue;
        if (lastMove != -1 && i / 3 == (lastMove / 3) + 3) continue; // Basic opposite face skip
        
        CoordCube nextNode = node;
        nextNode.move(i);
        path.push_back(i);
        if (searchPhase1(nextNode, depth + 1, maxDepth, i, path)) return true;
        path.pop_back();
    }
    return false;
}

bool KociembaSolver::searchPhase2(CoordCube node, int depth, int maxDepth, int lastMove, std::vector<int>& path) {
    if (depth == maxDepth) {
        if (node.cornerPerm == 0 && node.edgePerm == 0 && node.sliceSorted == 0) {
            return true;
        }
        return false;
    }

    if (depth + node.prun2 > maxDepth) return false;

    int phase2Moves[] = {0, 1, 2, 4, 7, 9, 10, 11, 13, 16};
    for (int i : phase2Moves) {
        if (lastMove != -1 && i / 3 == lastMove / 3) continue;
        if (lastMove != -1 && i / 3 == (lastMove / 3) + 3) continue;
        
        CoordCube nextNode = node;
        nextNode.move(i);
        path.push_back(i);
        if (searchPhase2(nextNode, depth + 1, maxDepth, i, path)) return true;
        path.pop_back();
    }
    return false;
}

CubieCube KociembaSolver::faceletsToCubie(const std::string& f) {
    // Colors: U=0, R=1, F=2, D=3, L=4, B=5
    // Corners: URF(0), UFL(1), ULB(2), UBR(3), DFR(4), DFL(5), DLB(6), DBR(7)
    int cornerFacelet[8][3] = {
        {8, 9, 20}, {6, 18, 38}, {0, 36, 47}, {2, 45, 11},
        {29, 26, 15}, {27, 44, 24}, {33, 53, 42}, {35, 17, 51}
    };
    int cornerColor[8][3] = {
        {0, 1, 2}, {0, 2, 4}, {0, 4, 5}, {0, 5, 1},
        {3, 2, 1}, {3, 4, 2}, {3, 5, 4}, {3, 1, 5}
    };

    // Edges: UR(0), UF(1), UL(2), UB(3), DR(4), DF(5), DL(6), DB(7), FR(8), FL(9), BL(10), BR(11)
    int edgeFacelet[12][2] = {
        {5, 10}, {7, 19}, {3, 37}, {1, 46},
        {32, 16}, {28, 25}, {30, 43}, {34, 52},
        {23, 12}, {21, 41}, {50, 39}, {48, 14}
    };
    int edgeColor[12][2] = {
        {0, 1}, {0, 2}, {0, 4}, {0, 5},
        {3, 1}, {3, 2}, {3, 4}, {3, 5},
        {2, 1}, {2, 4}, {5, 4}, {5, 1}
    };

    CubieCube cc;
    for (int i = 0; i < 8; i++) {
        int ori = 0;
        int col1 = f[cornerFacelet[i][0]] - '0';
        int col2 = f[cornerFacelet[i][1]] - '0';
        int col3 = f[cornerFacelet[i][2]] - '0';
        
        if (col2 == 0 || col2 == 3) { ori = 1; std::swap(col1, col2); std::swap(col2, col3); }
        else if (col3 == 0 || col3 == 3) { ori = 2; std::swap(col1, col3); std::swap(col3, col2); }
        
        int perm = -1;
        for (int j = 0; j < 8; j++) {
            if (col1 == cornerColor[j][0] && col2 == cornerColor[j][1] && col3 == cornerColor[j][2]) {
                perm = j; break;
            }
        }
        if (perm == -1) {
            std::cout << "Invalid corner at index " << i << ". Colors: " << col1 << " " << col2 << " " << col3 << std::endl;
            throw InvalidCubeStateException("Invalid corner");
        }
        cc.cp[i] = perm;
        cc.co[i] = ori;
    }

    for (int i = 0; i < 12; i++) {
        int col1 = f[edgeFacelet[i][0]] - '0';
        int col2 = f[edgeFacelet[i][1]] - '0';
        int ori = 0;
        int perm = -1;
        
        // Edge orientation check (simple way: U/D colors are always dominant)
        // For E slice (FR, FL, BL, BR), F/B colors are dominant over R/L
        //bool c1Dominant = (col1 == 0 || col1 == 3 || (i >= 8 && (col1 == 2 || col1 == 5)));
        //bool c2Dominant = (col2 == 0 || col2 == 3 || (i >= 8 && (col2 == 2 || col2 == 5)));
        
        //if (c2Dominant) {
        //    ori = 1;
        //    std::swap(col1, col2);
        //}
        
        //for (int j = 0; j < 12; j++) {
        //    if (col1 == edgeColor[j][0] && col2 == edgeColor[j][1]) {
        //        perm = j; break;
        //    }
        //}
        for (int j = 0; j < 12; j++) {
            if (col1 == edgeColor[j][0] && col2 == edgeColor[j][1]) {
                perm = j;
                ori = 0; // normal
                break;
            } else if (col1 == edgeColor[j][1] && col2 == edgeColor[j][0]) {
                perm = j;
                ori = 1; // flipped
                break;
            }
        }

        if (perm == -1) throw InvalidCubeStateException("Invalid edge");
        cc.ep[i] = perm;
        cc.eo[i] = ori;
    }
    return cc;
}

std::vector<std::string> KociembaSolver::solve(const std::string& facelets) {
    if (facelets.length() != 54) {
        throw InvalidCubeStateException("Facelet string must be exactly 54 characters long.");
    }
    
    CubieCube cc = faceletsToCubie(facelets);
    CoordCube start(cc);
    std::vector<int> path;

    if (start.twist == 0 && start.flip == 0 && start.slice == 0 && start.cornerPerm == 0 && start.edgePerm == 0 && start.sliceSorted == 0) {
        return {};
    }

    for (int length = 0; length <= 30; length++) {
        if (searchPhase1(start, 0, length, -1, path)) {
            std::vector<std::string> result;
            for (int m : path) {
                result.push_back(moveToString(m));
            }
            return result;
        }
    }
    throw InvalidCubeStateException("Could not find a solution.");
}
