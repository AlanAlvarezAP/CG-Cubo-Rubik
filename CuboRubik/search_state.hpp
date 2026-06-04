#pragma once
#include "cube.hpp"
#include "tables.hpp"
#include "constants.hpp"
#include <string>

static const char* moveNames[] = {
    "U","U2","U'","R","R2","R'","F","F2","F'",
    "D","D2","D'","L","L2","L'","B","B2","B'",
};

struct SearchState {
    SearchState* parent   = nullptr;
    int          lastMove = -1;
    int          depth    = 0;

    int flip     = 0;
    int twist    = 0;
    int slice    = 0;
    int parity   = 0;
    int URFtoDLF = 0;
    int FRtoBR   = 0;
    int URtoUL   = 0;
    int UBtoDF   = 0;
    int URtoDF   = 0;

    const MoveTables*   mt = nullptr;
    const PruningTables* pt = nullptr;

    void setTables(const MoveTables* mt_, const PruningTables* pt_) { mt=mt_; pt=pt_; }

    void initFromCube(const Cube& c) {
        Cube tmp=c.clone();
        flip      = tmp.flip_get();
        twist     = tmp.twist_get();
        slice     = tmp.FRtoBR()/24;
        parity    = tmp.cornerParity();
        URFtoDLF  = tmp.URFtoDLF();
        FRtoBR    = tmp.FRtoBR();
        URtoUL    = tmp.URtoUL();
        UBtoDF    = tmp.UBtoDF();
    }

    std::string solution() const {
        if(!parent) return "";
        return parent->solution() + moveNames[lastMove] + " ";
    }

    int minDist1() const {
        int a = pruning_get(pt->sliceFlip,  N_SLICE1*flip  + slice);
        int b = pruning_get(pt->sliceTwist, N_SLICE1*twist + slice);
        return (a>b)?a:b;
    }

    int minDist2() const {
        int idx1 = (N_SLICE2*URtoDF   + FRtoBR)*N_PARITY + parity;
        int idx2 = (N_SLICE2*URFtoDLF + FRtoBR)*N_PARITY + parity;
        int a = pruning_get(pt->sliceURtoDFParity,   idx1);
        int b = pruning_get(pt->sliceURFtoDLFParity, idx2);
        return (a>b)?a:b;
    }

    void init2(SearchState* top=nullptr) {
        if(!parent) return;
        parent->init2(top?top:this);
        int m=lastMove;
        URFtoDLF = mt->URFtoDLF[parent->URFtoDLF][m];
        FRtoBR   = mt->FRtoBR  [parent->FRtoBR  ][m];
        parity   = mt->parity  [parent->parity  ][m];
        URtoUL   = mt->URtoUL  [parent->URtoUL  ][m];
        UBtoDF   = mt->UBtoDF  [parent->UBtoDF  ][m];
        if(this == (top?top:this))
            URtoDF = mt->mergeURtoDF[URtoUL][UBtoDF];
    }
};
