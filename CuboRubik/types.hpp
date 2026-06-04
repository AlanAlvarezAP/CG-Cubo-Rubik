#pragma once
#include <array>
#include <vector>
#include <string>

enum class Center { U=0, R=1, F=2, D=3, L=4, B=5 };
enum class Corner { URF=0, UFL=1, ULB=2, UBR=3, DFR=4, DLF=5, DBL=6, DRB=7 };
enum class Edge   { UR=0, UF=1, UL=2, UB=3, DR=4, DF=5, DL=6, DB=7, FR=8, FL=9, BL=10, BR=11 };

struct CubeState {
    std::array<int,6>  center;
    std::array<int,8>  cp;
    std::array<int,8>  co;
    std::array<int,12> ep;
    std::array<int,12> eo;
};
