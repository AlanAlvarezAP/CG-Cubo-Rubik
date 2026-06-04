#pragma once
#include "types.hpp"
#include <array>
#include <string>
#include <unordered_map>

// ── Coordinate sizes ──────────────────────────────────────────────
constexpr int N_TWIST     = 2187;
constexpr int N_FLIP      = 2048;
constexpr int N_PARITY    = 2;
constexpr int N_FRtoBR    = 11880;
constexpr int N_SLICE1    = 495;
constexpr int N_SLICE2    = 24;
constexpr int N_URFtoDLF  = 20160;
constexpr int N_URtoDF    = 20160;
constexpr int N_URtoUL    = 1320;
constexpr int N_UBtoDF    = 1320;

// ── Facelet helpers ────────────────────────────────────────────────
constexpr int _U(int x){ return x-1; }
constexpr int _R(int x){ return _U(9)+x; }
constexpr int _F(int x){ return _R(9)+x; }
constexpr int _D(int x){ return _F(9)+x; }
constexpr int _L(int x){ return _D(9)+x; }
constexpr int _B(int x){ return _L(9)+x; }

constexpr std::array<int,6> centerFacelet = {4, 13, 22, 31, 40, 49};

constexpr std::array<std::array<int,3>,8> cornerFacelet = {{
    {_U(9),_R(1),_F(3)}, {_U(7),_F(1),_L(3)}, {_U(1),_L(1),_B(3)}, {_U(3),_B(1),_R(3)},
    {_D(3),_F(9),_R(7)}, {_D(1),_L(9),_F(7)}, {_D(7),_B(9),_L(7)}, {_D(9),_R(9),_B(7)},
}};

constexpr std::array<std::array<int,2>,12> edgeFacelet = {{
    {_U(6),_R(2)}, {_U(8),_F(2)}, {_U(4),_L(2)}, {_U(2),_B(2)},
    {_D(6),_R(8)}, {_D(2),_F(8)}, {_D(4),_L(8)}, {_D(8),_B(8)},
    {_F(6),_R(4)}, {_F(4),_L(6)}, {_B(6),_L(4)}, {_B(4),_R(6)},
}};

constexpr std::array<const char*,6> centerColor = {"U","R","F","D","L","B"};

constexpr std::array<std::array<const char*,3>,8> cornerColor = {{
    {"U","R","F"},{"U","F","L"},{"U","L","B"},{"U","B","R"},
    {"D","F","R"},{"D","L","F"},{"D","B","L"},{"D","R","B"},
}};

constexpr std::array<std::array<const char*,2>,12> edgeColor = {{
    {"U","R"},{"U","F"},{"U","L"},{"U","B"},
    {"D","R"},{"D","F"},{"D","L"},{"D","B"},
    {"F","R"},{"F","L"},{"B","L"},{"B","R"},
}};

inline const char* faceName(int f) {
    static const char* names[] = {"U","R","F","D","L","B"};
    return names[f];
}

inline int faceNum(char c) {
    switch(c) {
        case 'U': return 0; case 'R': return 1; case 'F': return 2;
        case 'D': return 3; case 'L': return 4; case 'B': return 5;
        case 'E': return 6; case 'M': return 7; case 'S': return 8;
        case 'x': return 9; case 'y': return 10; case 'z': return 11;
        case 'u': return 12; case 'r': return 13; case 'f': return 14;
        case 'd': return 15; case 'l': return 16; case 'b': return 17;
    }
    return -1;
}

// ── Base move data ─────────────────────────────────────────────────
inline std::array<CubeState,9> makeBaseMoveData() {
    return {{
        // U
        {{0,1,2,3,4,5},{3,0,1,2,4,5,6,7},{0,0,0,0,0,0,0,0},{3,0,1,2,4,5,6,7,8,9,10,11},{0,0,0,0,0,0,0,0,0,0,0,0}},
        // R
        {{0,1,2,3,4,5},{4,1,2,0,7,5,6,3},{2,0,0,1,1,0,0,2},{8,1,2,3,11,5,6,7,4,9,10,0},{0,0,0,0,0,0,0,0,0,0,0,0}},
        // F
        {{0,1,2,3,4,5},{1,5,2,3,0,4,6,7},{1,2,0,0,2,1,0,0},{0,9,2,3,4,8,6,7,1,5,10,11},{0,1,0,0,0,1,0,0,1,1,0,0}},
        // D
        {{0,1,2,3,4,5},{0,1,2,3,5,6,7,4},{0,0,0,0,0,0,0,0},{0,1,2,3,5,6,7,4,8,9,10,11},{0,0,0,0,0,0,0,0,0,0,0,0}},
        // L
        {{0,1,2,3,4,5},{0,2,6,3,4,1,5,7},{0,1,2,0,0,2,1,0},{0,1,10,3,4,5,9,7,8,2,6,11},{0,0,0,0,0,0,0,0,0,0,0,0}},
        // B
        {{0,1,2,3,4,5},{0,1,3,7,4,5,2,6},{0,0,1,2,0,0,2,1},{0,1,2,11,4,5,6,10,8,9,3,7},{0,0,0,1,0,0,0,1,0,0,1,1}},
        // E
        {{0,2,4,3,5,1},{0,1,2,3,4,5,6,7},{0,0,0,0,0,0,0,0},{0,1,2,3,4,5,6,7,9,10,11,8},{0,0,0,0,0,0,0,0,1,1,1,1}},
        // M
        {{5,1,0,2,4,3},{0,1,2,3,4,5,6,7},{0,0,0,0,0,0,0,0},{0,3,2,7,4,1,6,5,8,9,10,11},{0,1,0,1,0,1,0,1,0,0,0,0}},
        // S
        {{4,0,2,1,3,5},{0,1,2,3,4,5,6,7},{0,0,0,0,0,0,0,0},{4,1,6,3,0,5,2,7,8,9,10,11},{1,0,1,0,1,0,1,0,0,0,0,0}},
    }};
}

// x y z u r f d l b
inline const char* compoundMoves(int idx) {
    static const char* recipes[] = {
        "R M' L'",  // x  (9)
        "U E' D'",  // y  (10)
        "F S B'",   // z  (11)
        "U E'",     // u  (12)
        "R M'",     // r  (13)
        "F S",      // f  (14)
        "D E",      // d  (15)
        "L M",      // l  (16)
        "B S'",     // b  (17)
    };
    return recipes[idx];
}

constexpr std::array<std::array<int,18>,2> PARITY_TABLE = {{
    {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
    {0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0},
}};
