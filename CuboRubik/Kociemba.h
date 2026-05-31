#ifndef KOCIEMBA_H
#define KOCIEMBA_H

#include <vector>
#include <string>
#include <array>
#include <stdexcept>

// Cubie representation
struct CubieCube {
    std::array<int, 8> cp; // Corner permutation
    std::array<int, 8> co; // Corner orientation
    std::array<int, 12> ep; // Edge permutation
    std::array<int, 12> eo; // Edge orientation

    CubieCube();
    void multiply(const CubieCube& b);
    void edgeMultiply(const CubieCube& b);
    void cornerMultiply(const CubieCube& b);
};

// Coordinate representation
struct CoordCube {
    int twist;
    int flip;
    int slice;
    int prun1;
    
    int cornerPerm;
    int edgePerm;
    int sliceSorted;
    int prun2;

    CoordCube(const CubieCube& c);
    void move(int m);
};

// Main solver class
class KociembaSolver {
public:
    static void initTables();
    static std::vector<std::string> solve(const std::string& facelets);

    static CubieCube faceletsToCubie(const std::string& f);
    
    static int getTwist(const CubieCube& c);
    static void setTwist(CubieCube& c, int twist);
    static int getFlip(const CubieCube& c);
    static void setFlip(CubieCube& c, int flip);
    static int getSlice(const CubieCube& c);
    static void setSlice(CubieCube& c, int slice);
    
    static int getCornerPerm(const CubieCube& c);
    static void setCornerPerm(CubieCube& c, int idx);
    static int getEdgePerm(const CubieCube& c);
    static void setEdgePerm(CubieCube& c, int idx);
    static int getSliceSorted(const CubieCube& c);
    static void setSliceSorted(CubieCube& c, int idx);

    static std::string moveToString(int m);

private:

    // Search
    static bool searchPhase1(CoordCube node, int depth, int maxDepth, int lastMove, std::vector<int>& path);
    static bool searchPhase2(CoordCube node, int depth, int maxDepth, int lastMove, std::vector<int>& path);
};

class InvalidCubeStateException : public std::runtime_error {
public:
    InvalidCubeStateException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
