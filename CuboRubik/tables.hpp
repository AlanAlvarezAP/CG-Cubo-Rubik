#pragma once
#include "cube.hpp"
#include "constants.hpp"
#include <vector>
#include <array>
#include <cstring>
#include <functional>

// ── Pruning table helpers ─────────────────────────────────────────

inline int pruning_get(const std::vector<uint32_t>& table, int index) {
    int pos   = index % 8;
    int slot  = index >> 3;
    int shift = pos << 2;
    return (table[slot] >> shift) & 0xF;
}

inline void pruning_set(std::vector<uint32_t>& table, int index, int value) {
    int pos   = index % 8;
    int slot  = index >> 3;
    int shift = pos << 2;
    table[slot] &= ~(0xFu << shift);
    table[slot] |= ((uint32_t)value << shift);
}

// ── Move tables ───────────────────────────────────────────────────

// context: 0=corners, 1=edges
inline std::vector<std::array<int,18>> computeMoveTable(int context, const std::string& coordMethod, int size) {
    std::vector<std::array<int,18>> table(size);
    Cube c;
    for(int i=0;i<size;i++) {
        // set coordinate
        if(coordMethod=="twist")    c.twist_set(i);
        else if(coordMethod=="flip"){ c.ep={0,1,2,3,4,5,6,7,8,9,10,11}; c.flip_set(i); }
        else if(coordMethod=="FRtoBR")   c.FRtoBR(i);
        else if(coordMethod=="URFtoDLF") c.URFtoDLF(i);
        else if(coordMethod=="URtoDF")   c.URtoDF(i);
        else if(coordMethod=="URtoUL")   c.URtoUL(i);
        else if(coordMethod=="UBtoDF")   c.UBtoDF(i);

        std::array<int,18> inner;
        int idx=0;
        for(int j=0;j<6;j++) {
            for(int k=0;k<3;k++) {
                if(context==0) c.cornerMultiply(Cube::moves[j]);
                else c.edgeMultiply(Cube::moves[j]);
                // get coordinate
                int val=0;
                if(coordMethod=="twist")     val=c.twist_get();
                else if(coordMethod=="flip") val=c.flip_get();
                else if(coordMethod=="FRtoBR")   val=c.FRtoBR();
                else if(coordMethod=="URFtoDLF") val=c.URFtoDLF();
                else if(coordMethod=="URtoDF")   val=c.URtoDF();
                else if(coordMethod=="URtoUL")   val=c.URtoUL();
                else if(coordMethod=="UBtoDF")   val=c.UBtoDF();
                inner[idx++]=val;
            }
            if(context==0) c.cornerMultiply(Cube::moves[j]);
            else c.edgeMultiply(Cube::moves[j]);
        }
        table[i]=inner;
    }
    return table;
}

inline std::vector<std::vector<int>> computeMergeURtoDF() {
    std::vector<std::vector<int>> table(336,std::vector<int>(336));
    Cube a,b;
    for(int i=0;i<336;i++) {
        for(int j=0;j<336;j++) {
            Cube a2,b2; a2.URtoUL(i); b2.UBtoDF(j);
            bool collision=false;
            for(int k=0;k<8;k++) {
                if(a2.ep[k]!=-1) {
                    if(b2.ep[k]!=-1) { collision=true; break; }
                    else b2.ep[k]=a2.ep[k];
                }
            }
            table[i][j]= collision? -1 : b2.URtoDF();
        }
    }
    return table;
}

struct MoveTables {
    std::array<std::array<int,18>,2>         parity;
    std::vector<std::array<int,18>>          twist;
    std::vector<std::array<int,18>>          flip;
    std::vector<std::array<int,18>>          FRtoBR;
    std::vector<std::array<int,18>>          URFtoDLF;
    std::vector<std::array<int,18>>          URtoDF;
    std::vector<std::array<int,18>>          URtoUL;
    std::vector<std::array<int,18>>          UBtoDF;
    std::vector<std::vector<int>>            mergeURtoDF;
};

inline MoveTables buildMoveTables() {
    MoveTables mt;
    mt.parity    = PARITY_TABLE;
    mt.twist     = computeMoveTable(0,"twist",    N_TWIST);
    mt.flip      = computeMoveTable(1,"flip",     N_FLIP);
    mt.FRtoBR    = computeMoveTable(1,"FRtoBR",   N_FRtoBR);
    mt.URFtoDLF  = computeMoveTable(0,"URFtoDLF", N_URFtoDLF);
    mt.URtoDF    = computeMoveTable(1,"URtoDF",   N_URtoDF);
    mt.URtoUL    = computeMoveTable(1,"URtoUL",   N_URtoUL);
    mt.UBtoDF    = computeMoveTable(1,"UBtoDF",   N_UBtoDF);
    mt.mergeURtoDF = computeMergeURtoDF();
    return mt;
}

// ── Pruning tables ────────────────────────────────────────────────

struct PruningTables {
    std::vector<uint32_t> sliceTwist;
    std::vector<uint32_t> sliceFlip;
    std::vector<uint32_t> sliceURFtoDLFParity;
    std::vector<uint32_t> sliceURtoDFParity;
};

inline std::vector<uint32_t> computePruningTable(
    int phase, int size, const MoveTables& mt,
    std::function<std::array<int,3>(int)> currentCoords,
    std::function<int(std::array<int,3>, int)> nextIndex)
{
    std::vector<uint32_t> table((size+7)/8, 0xFFFFFFFF);
    std::vector<int> moves1 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    std::vector<int> moves2 = {0,1,2,4,7,9,10,11,13,16};
    auto& moves = (phase==1)?moves1:moves2;

    int depth=0;
    pruning_set(table,0,depth);
    int done=1;

    while(done!=size) {
        for(int index=0;index<size;index++) {
            if(pruning_get(table,index)==depth) {
                auto cur=currentCoords(index);
                for(int m:moves) {
                    int next=nextIndex(cur,m);
                    if(pruning_get(table,next)==0xF) {
                        pruning_set(table,next,depth+1);
                        done++;
                    }
                }
            }
        }
        depth++;
    }
    return table;
}

inline PruningTables buildPruningTables(const MoveTables& mt) {
    PruningTables pt;

    pt.sliceTwist = computePruningTable(1, N_SLICE1*N_TWIST, mt,
        [](int i)->std::array<int,3>{ return {i%N_SLICE1, i/N_SLICE1, 0}; },
        [&](std::array<int,3> cur, int m)->int {
            return mt.twist[cur[1]][m]*N_SLICE1 + mt.FRtoBR[cur[0]*24][m]/24;
        });

    pt.sliceFlip = computePruningTable(1, N_SLICE1*N_FLIP, mt,
        [](int i)->std::array<int,3>{ return {i%N_SLICE1, i/N_SLICE1, 0}; },
        [&](std::array<int,3> cur, int m)->int {
            return mt.flip[cur[1]][m]*N_SLICE1 + mt.FRtoBR[cur[0]*24][m]/24;
        });

    pt.sliceURFtoDLFParity = computePruningTable(2, N_SLICE2*N_URFtoDLF*N_PARITY, mt,
        [](int i)->std::array<int,3>{ return {i%2, (i/2)%N_SLICE2, i/(2*N_SLICE2)}; },
        [&](std::array<int,3> cur, int m)->int {
            return (mt.URFtoDLF[cur[2]][m]*N_SLICE2 + mt.FRtoBR[cur[1]][m])*2 + mt.parity[cur[0]][m];
        });

    pt.sliceURtoDFParity = computePruningTable(2, N_SLICE2*N_URtoDF*N_PARITY, mt,
        [](int i)->std::array<int,3>{ return {i%2, (i/2)%N_SLICE2, i/(2*N_SLICE2)}; },
        [&](std::array<int,3> cur, int m)->int {
            return (mt.URtoDF[cur[2]][m]*N_SLICE2 + mt.FRtoBR[cur[1]][m])*2 + mt.parity[cur[0]][m];
        });

    return pt;
}
