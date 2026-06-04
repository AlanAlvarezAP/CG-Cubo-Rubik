#pragma once
#include "cube.hpp"
#include "tables.hpp"
#include "search_state.hpp"
#include "constants.hpp"
#include <string>
#include <vector>
#include <optional>
#include <sstream>
#include <cstdlib>
#include <ctime>

// ── Singleton table store ─────────────────────────────────────────
namespace detail {
    inline MoveTables*   g_mt      = nullptr;
    inline PruningTables* g_pt     = nullptr;
    inline bool           g_ready  = false;
}

/**
 * Pre-computes all move and pruning tables.
 * Safe to call multiple times; subsequent calls are no-ops.
 */
inline void initSolver() {
    if(detail::g_ready) return;

    // 1. Populate base moves
    auto baseData = makeBaseMoveData();
    Cube::moves.clear();
    for(auto& raw: baseData) {
        Cube c; c.fromRaw(raw);
        Cube::moves.push_back(c);
    }

    // 2. Compound moves (indices 9..17)
    for(int ci=0;ci<9;ci++) {
        const char* recipe = compoundMoves(ci);
        Cube c;
        std::istringstream ss(recipe);
        std::string tok;
        while(ss>>tok) {
            if(tok.empty()) continue;
            int face=faceNum(tok[0]);
            int power;
            if(tok.size()==1)       power=1;
            else if(tok[1]=='2')    power=2;
            else if(tok[1]=='\'')   power=3;
            else continue;
            for(int i=0;i<power;i++) c.multiply(Cube::moves[face]);
        }
        Cube::moves.push_back(c);
    }

    // 3. Build tables
    detail::g_mt = new MoveTables(buildMoveTables());
    detail::g_pt = new PruningTables(buildPruningTables(*detail::g_mt));
    detail::g_ready = true;
}

// ── Internal search ───────────────────────────────────────────────

namespace detail {

static const std::vector<int> allMoves1 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
static const std::vector<int> allMoves2 = {0,1,2,4,7,9,10,11,13,16};

static std::vector<std::vector<int>> nextMoves1Cache;
static std::vector<std::vector<int>> nextMoves2Cache;

inline void buildNextMovesCache() {
    if(!nextMoves1Cache.empty()) return;
    nextMoves1Cache.resize(6);
    nextMoves2Cache.resize(6);
    // Exact match of TS: if (face !== lastFace && face !== lastFace - 3)
    // Face order: U=0, R=1, F=2, D=3, L=4, B=5
    // lastFace-3 gives the "opposite" for faces 3,4,5 → 0,1,2
    // For faces 0,1,2: lastFace-3 is negative, so never matches → 4 faces allowed
    // Phase 2: U(0) and D(3) get all 3 powers; R,F,L,B get only half-turn (p=1)
    for(int lastFace=0;lastFace<=5;lastFace++) {
        std::vector<int> n1, n2;
        for(int face=0;face<=5;face++) {
            if(face!=lastFace && face!=lastFace-3) {
                for(int p=0;p<3;p++) n1.push_back(face*3+p);
                if(face==0||face==3) {
                    for(int p=0;p<3;p++) n2.push_back(face*3+p);
                } else {
                    n2.push_back(face*3+1);
                }
            }
        }
        nextMoves1Cache[lastFace]=n1;
        nextMoves2Cache[lastFace]=n2;
    }
}

struct SolveContext {
    const MoveTables*    mt;
    const PruningTables* pt;
    int maxDepth;
    std::string solution;
    bool found;

    std::vector<SearchState> statePool;
    int poolTop;

    SearchState* pop() { return (poolTop>0)?&statePool[--poolTop]:nullptr; }
    void push()        { poolTop++; }
};

void phase2(SolveContext& ctx, SearchState* ss, int depth);

void phase2search(SolveContext& ctx, SearchState* ss) {
    // init2 propagates phase-2 coords
    ss->init2();
    for(int depth=1;depth<=ctx.maxDepth-ss->depth;depth++) {
        phase2(ctx,ss,depth);
        if(ctx.found) return;
    }
}

void phase2(SolveContext& ctx, SearchState* ss, int depth) {
    if(ctx.found) return;
    if(depth==0) {
        if(ss->minDist2()==0) { ctx.solution=ss->solution(); ctx.found=true; }
        return;
    }
    if(ss->minDist2()>depth) return;

    auto& candidates = (ss->lastMove>=0)?detail::nextMoves2Cache[ss->lastMove/3]:detail::allMoves2;
    for(int move:candidates) {
        SearchState* next=ctx.pop();
        next->parent   = ss;
        next->lastMove = move;
        next->depth    = ss->depth+1;
        next->URFtoDLF = ctx.mt->URFtoDLF[ss->URFtoDLF][move];
        next->FRtoBR   = ctx.mt->FRtoBR  [ss->FRtoBR  ][move];
        next->parity   = ctx.mt->parity  [ss->parity  ][move];
        next->URtoDF   = ctx.mt->URtoDF  [ss->URtoDF  ][move];
        next->mt = ctx.mt; next->pt = ctx.pt;
        phase2(ctx,next,depth-1);
        ctx.push();
        if(ctx.found) return;
    }
}

void phase1(SolveContext& ctx, SearchState* ss, int depth);

void _phase1(SolveContext& ctx, SearchState* ss, int depth) {
    if(ctx.found) return;
    if(depth==0) {
        if(ss->minDist1()==0) {
            bool inM2 = false;
            if(ss->lastMove>=0) {
                for(int m:allMoves2) if(m==ss->lastMove){inM2=true;break;}
            }
            if(ss->lastMove<0 || !inM2) phase2search(ctx,ss);
        }
        return;
    }
    if(ss->minDist1()>depth) return;

    auto& candidates = (ss->lastMove>=0)?nextMoves1Cache[ss->lastMove/3]:allMoves1;
    for(int move:candidates) {
        SearchState* next=ctx.pop();
        next->parent   = ss;
        next->lastMove = move;
        next->depth    = ss->depth+1;
        next->flip     = ctx.mt->flip [ss->flip ][move];
        next->twist    = ctx.mt->twist[ss->twist][move];
        next->slice    = ctx.mt->FRtoBR[ss->slice*24][move]/24;
        next->mt = ctx.mt; next->pt = ctx.pt;
        _phase1(ctx,next,depth-1);
        ctx.push();
        if(ctx.found) return;
    }
}

inline std::string solveUpright(const Cube& cube, int maxDepth) {
    buildNextMovesCache();

    SolveContext ctx;
    ctx.mt       = detail::g_mt;
    ctx.pt       = detail::g_pt;
    ctx.maxDepth = maxDepth;
    ctx.found    = false;

    ctx.statePool.resize(maxDepth+1);
    for(auto& s:ctx.statePool) { s.setTables(ctx.mt,ctx.pt); }
    ctx.poolTop  = maxDepth+1;

    SearchState* root = ctx.pop();
    root->parent=nullptr; root->lastMove=-1; root->depth=0;
    root->initFromCube(cube);
    root->mt=ctx.mt; root->pt=ctx.pt;

    for(int depth=1;depth<=maxDepth;depth++) {
        _phase1(ctx,root,depth);
        if(ctx.found) break;
    }
    return ctx.found ? ctx.solution : "";
}

} // namespace detail

/**
 * Solves the given cube using Kociemba's two-phase algorithm.
 * Returns the solution move sequence, or empty string if not found.
 */
inline std::string solve(const Cube& cube, int maxDepth=22) {
    initSolver();

    Cube clone = cube.clone();
    std::string uprightAlg = clone.upright();
    if(!uprightAlg.empty()) clone.move(uprightAlg);

    std::array<int8_t,6> rotation = clone.center;
    std::string uprightSolution = detail::solveUpright(clone, maxDepth);
    if(uprightSolution.empty()) return "";

    // Translate face names back through rotation
    std::istringstream ss(uprightSolution);
    std::string tok, result;
    while(ss>>tok) {
        if(tok.empty()) continue;
        int origFace = rotation[faceNum(tok[0])];
        if(!result.empty()) result+=" ";
        result += faceName(origFace);
        if(tok.size()>1) result += tok[1];
    }
    return result;
}

/** Generates a random scramble sequence. */
inline std::string scramble(int length=25) {
    initSolver();
    static bool seeded=false;
    if(!seeded) { srand((unsigned)time(nullptr)); seeded=true; }

    static const char* names[] = {"U","D","R","L","F","B"};
    static const char* mods[]  = {"","'","2"};
    static const int   opp[]   = {1,0,3,2,5,4};

    std::vector<std::string> result;
    int last=-1, secondLast=-1;

    while((int)result.size()<length) {
        std::vector<int> avail;
        for(int f=0;f<6;f++) {
            if(f==last) continue;
            if(last>=0 && f==opp[last]) continue;
            if(secondLast>=0 && f==secondLast && opp[secondLast]==last) continue;
            avail.push_back(f);
        }
        int face = avail[rand()%avail.size()];
        int mod  = rand()%3;
        result.push_back(std::string(names[face])+mods[mod]);
        secondLast=last; last=face;
    }

    std::string out;
    for(auto& s:result) { if(!out.empty()) out+=" "; out+=s; }
    return out;
}