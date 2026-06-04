#pragma once
#include "types.hpp"
#include "constants.hpp"
#include "math.hpp"
#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

class Cube {
public:
    std::array<int8_t,6>  center = {0,1,2,3,4,5};
    std::array<int8_t,8>  cp     = {0,1,2,3,4,5,6,7};
    std::array<int8_t,8>  co     = {0,0,0,0,0,0,0,0};
    std::array<int8_t,12> ep     = {0,1,2,3,4,5,6,7,8,9,10,11};
    std::array<int8_t,12> eo     = {0,0,0,0,0,0,0,0,0,0,0,0};

    static std::vector<Cube> moves;

    // ── Construction ──────────────────────────────────────────────

    Cube() = default;

    Cube clone() const {
        Cube c; c.center=center; c.cp=cp; c.co=co; c.ep=ep; c.eo=eo;
        return c;
    }

    Cube& fromRaw(const CubeState& raw) {
        for(int i=0;i<6;i++)  center[i]= raw.center[i];
        for(int i=0;i<8;i++)  { cp[i]=raw.cp[i]; co[i]=raw.co[i]; }
        for(int i=0;i<12;i++) { ep[i]=raw.ep[i]; eo[i]=raw.eo[i]; }
        return *this;
    }

    CubeState toState() const {
        CubeState s;
        for(int i=0;i<6;i++)  s.center[i]=center[i];
        for(int i=0;i<8;i++)  { s.cp[i]=cp[i]; s.co[i]=co[i]; }
        for(int i=0;i<12;i++) { s.ep[i]=ep[i]; s.eo[i]=eo[i]; }
        return s;
    }

    // ── String serialization ──────────────────────────────────────

    std::string asString() const {
        std::array<char,54> result;
        for(int i=0;i<6;i++) result[9*i+4] = centerColor[center[i]][0];
        for(int i=0;i<8;i++) {
            int corner=cp[i], ori=co[i];
            for(int n=0;n<3;n++)
                result[cornerFacelet[i][(n+ori)%3]] = cornerColor[corner][n][0];
        }
        for(int i=0;i<12;i++) {
            int edge=ep[i], ori=eo[i];
            for(int n=0;n<2;n++)
                result[edgeFacelet[i][(n+ori)%2]] = edgeColor[edge][n][0];
        }
        return std::string(result.begin(),result.end());
    }

    static Cube fromString(const std::string& str) {
        Cube cube;
        for(int i=0;i<6;i++)
            for(int j=0;j<6;j++)
                if(str[9*i+4] == centerColor[j][0]) cube.center[i]=j;

        for(int i=0;i<8;i++) {
            int ori=0;
            for(;ori<3;ori++)
                if(str[cornerFacelet[i][ori]]=='U'||str[cornerFacelet[i][ori]]=='D') break;
            char col1=str[cornerFacelet[i][(ori+1)%3]];
            char col2=str[cornerFacelet[i][(ori+2)%3]];
            for(int j=0;j<8;j++)
                if(col1==cornerColor[j][1][0]&&col2==cornerColor[j][2][0]) {
                    cube.cp[i]=j; cube.co[i]=ori%3;
                }
        }

        for(int i=0;i<12;i++) {
            for(int j=0;j<12;j++) {
                if(str[edgeFacelet[i][0]]==edgeColor[j][0][0]&&str[edgeFacelet[i][1]]==edgeColor[j][1][0]) {
                    cube.ep[i]=j; cube.eo[i]=0; break;
                }
                if(str[edgeFacelet[i][0]]==edgeColor[j][1][0]&&str[edgeFacelet[i][1]]==edgeColor[j][0][0]) {
                    cube.ep[i]=j; cube.eo[i]=1; break;
                }
            }
        }
        return cube;
    }

    // ── Multiplication ────────────────────────────────────────────

    void centerMultiply(const Cube& other) {
        std::array<int8_t,6> tmp;
        for(int t=0;t<6;t++) tmp[t]=center[other.center[t]];
        center=tmp;
    }

    void cornerMultiply(const Cube& other) {
        std::array<int8_t,8> newCp, newCo;
        for(int t=0;t<8;t++) {
            int from=other.cp[t];
            newCp[t]=cp[from];
            newCo[t]=(co[from]+other.co[t])%3;
        }
        cp=newCp; co=newCo;
    }

    void edgeMultiply(const Cube& other) {
        std::array<int8_t,12> newEp, newEo;
        for(int t=0;t<12;t++) {
            int from=other.ep[t];
            newEp[t]=ep[from];
            newEo[t]=(eo[from]+other.eo[t])%2;
        }
        ep=newEp; eo=newEo;
    }

    void multiply(const Cube& other) {
        centerMultiply(other);
        cornerMultiply(other);
        edgeMultiply(other);
    }

    // ── Move application ──────────────────────────────────────────

    Cube& move(const std::string& alg) {
        std::istringstream ss(alg);
        std::string tok;
        while(ss>>tok) {
            if(tok.empty()) continue;
            int face = faceNum(tok[0]);
            if(face<0) throw std::runtime_error("Invalid move: "+tok);
            int power;
            if(tok.size()==1) power=0;
            else if(tok[1]=='2') power=1;
            else if(tok[1]=='\'') power=2;
            else throw std::runtime_error("Invalid move: "+tok);
            for(int i=0;i<=power;i++) multiply(moves[face]);
        }
        return *this;
    }

    // ── Upright rotation ──────────────────────────────────────────

    std::string upright() const {
        Cube c=clone();
        std::string result;

        int i=0;
        for(;i<6;i++) if(c.center[i]==(int)Center::F) break;
        std::string rot1;
        if(i==(int)Center::D)      rot1="x";
        else if(i==(int)Center::U) rot1="x'";
        else if(i==(int)Center::B) rot1="x2";
        else if(i==(int)Center::R) rot1="y";
        else if(i==(int)Center::L) rot1="y'";
        if(!rot1.empty()) { result+=rot1; c.move(rot1); }

        int j=0;
        for(;j<6;j++) if(c.center[j]==(int)Center::U) break;
        std::string rot2;
        if(j==(int)Center::L)      rot2="z";
        else if(j==(int)Center::R) rot2="z'";
        else if(j==(int)Center::D) rot2="z2";
        if(!rot2.empty()) {
            if(!result.empty()) result+=" ";
            result+=rot2;
        }
        return result;
    }

    // ── Solved check ──────────────────────────────────────────────

    std::string verify() const {
        std::array<int,8> cc={};
        for(int i=0;i<8;i++) {
            if(cp[i]==-1) return "Invalid cube: Unrecognized or missing corners.";
            cc[cp[i]]++;
        }
        for(int i=0;i<8;i++)
            if(cc[i]!=1) return "Invalid cube: Duplicate corner detected.";

        std::array<int,12> ec={};
        for(int i=0;i<12;i++) {
            if(ep[i]==-1) return "Invalid cube: Unrecognized or missing edges.";
            ec[ep[i]]++;
        }
        for(int i=0;i<12;i++)
            if(ec[i]!=1) return "Invalid cube: Duplicate edge detected.";

        int twistSum=0;
        for(int i=0;i<8;i++) twistSum+=co[i];
        if(twistSum%3!=0) return "Invalid cube: Corner twist error.";

        int flipSum=0;
        for(int i=0;i<12;i++) flipSum+=eo[i];
        if(flipSum%2!=0) return "Invalid cube: Edge flip error.";

        if(cornerParity()!=edgeParity())
            return "Invalid cube: Parity error.";
        return "";
    }

    bool isSolved() const {
        Cube c=clone();
        std::string r=c.upright();
        if(!r.empty()) c.move(r);
        for(int i=0;i<6;i++)  if(c.center[i]!=i) return false;
        for(int i=0;i<8;i++)  if(c.cp[i]!=i||c.co[i]!=0) return false;
        for(int i=0;i<12;i++) if(c.ep[i]!=i||c.eo[i]!=0) return false;
        return true;
    }

    // ── Randomize ─────────────────────────────────────────────────

    Cube& randomize() {
        auto randint=[](int lo,int hi){ return lo+(int)(rand()%(hi-lo+1)); };
        auto shuffle=[&](auto& arr){
            int n=arr.size();
            for(int i=n-1;i>0;i--){int r=randint(0,i);std::swap(arr[i],arr[r]);}
        };
        auto swaps=[](auto& arr){
            int n=arr.size(),cnt=0;
            std::vector<bool> seen(n,false);
            while(true){
                int cur=-1;
                for(int i=0;i<n;i++) if(!seen[i]){cur=i;break;}
                if(cur<0) break;
                int len=0;
                while(!seen[cur]){seen[cur]=true;len++;cur=arr[cur];}
                cnt+=len+1;
            }
            return cnt;
        };

        do { shuffle(ep); shuffle(cp); } while((swaps(ep)+swaps(cp))%2!=0);

        auto randOri=[&](auto& arr,int mod){
            for(auto& x:arr) x=randint(0,mod-1);
        };
        auto validOri=[](auto& arr,int mod){
            int s=0;for(auto x:arr) s+=x;return s%mod==0;
        };

        do randOri(co,3); while(!validOri(co,3));
        do randOri(eo,2); while(!validOri(eo,2));
        return *this;
    }

    static Cube random() { return Cube().randomize(); }

    // ── Algorithm helpers ─────────────────────────────────────────

    static std::string inverse(const std::string& alg) {
        std::istringstream ss(alg);
        std::vector<std::string> tokens;
        std::string tok;
        while(ss>>tok) if(!tok.empty()) tokens.push_back(tok);
        std::string result;
        for(int i=(int)tokens.size()-1;i>=0;i--) {
            auto& m=tokens[i];
            if(!result.empty()) result+=" ";
            if(m.size()==1)       result+=m+"'";
            else if(m[1]=='2')    result+=m;
            else if(m[1]=='\'')   result+=m.substr(0,1);
            else throw std::runtime_error("Invalid move: "+m);
        }
        return result;
    }

    // ── Parities ──────────────────────────────────────────────────

    int cornerParity() const {
        int s=0;
        for(int i=7;i>=1;i--)
            for(int j=i-1;j>=0;j--)
                if(cp[j]>cp[i]) s++;
        return s%2;
    }

    int edgeParity() const {
        int s=0;
        for(int i=11;i>=1;i--)
            for(int j=i-1;j>=0;j--)
                if(ep[j]>ep[i]) s++;
        return s%2;
    }

    // ── Coordinates ───────────────────────────────────────────────

    int twist_get() const {
        int v=0;
        for(int i=0;i<=6;i++) v=3*v+co[i];
        return v;
    }
    void twist_set(int val) {
        int parity=0;
        for(int i=6;i>=0;i--) { int ori=val%3; val/=3; co[i]=ori; parity+=ori; }
        co[7]=(3-parity%3)%3;
    }

    int flip_get() const {
        int v=0;
        for(int i=0;i<=10;i++) v=2*v+eo[i];
        return v;
    }
    void flip_set(int val) {
        int parity=0;
        for(int i=10;i>=0;i--) { int ori=val%2; val/=2; eo[i]=ori; parity+=ori; }
        eo[11]=(2-parity%2)%2;
    }

    // permutationIndex: context=0 corners, 1 edges; fromEnd=true for FRtoBR
    int permutationIndex(int context, int start, int end, bool fromEnd, int index=-1) {
        int maxOur=end-start;
        int maxB=factorial(maxOur+1);
        int maxAll=(context==0)?7:11;
        

        if(index>=0) {
            std::array<int8_t,12> our={};
            for(int i=0;i<=maxOur;i++) our[i]=i+start;
            int b=index%maxB, a=index/maxB;
            if(context==0) cp.fill(-1); else ep.fill(-1);

            for(int j=1;j<=maxOur;j++) {
                int k=b%(j+1); b/=(j+1);
                while(k-->0) rotateRight(our.data(),0,j);
            }
            int x=maxOur;
            if(fromEnd) {
                for(int j=0;j<=maxAll;j++) {
                    int c=Cnk(maxAll-j,x+1);
                    if(a-c>=0) {
                        if(context==0) cp[j]=our[maxOur-x];
                        else ep[j]=our[maxOur-x];
                        a-=c; x--;
                    }
                }
            } else {
                for(int j=maxAll;j>=0;j--) {
                    int c=Cnk(j,x+1);
                    if(a-c>=0) {
                        if(context==0) cp[j]=our[x];
                        else ep[j]=our[x];
                        a-=c; x--;
                    }
                }
            }
            return index;
        } else {
            std::array<int8_t,12> our; our.fill(-1);
            int a=0,b=0,x=0;
            if(fromEnd) {
                for(int j=maxAll;j>=0;j--) {
                    int v=(context==0)?cp[j]:ep[j];
                    if(start<=v&&v<=end) { a+=Cnk(maxAll-j,x+1); our[maxOur-x]=v; x++; }
                }
            } else {
                for(int j=0;j<=maxAll;j++) {
                    int v=(context==0)?cp[j]:ep[j];
                    if(start<=v&&v<=end) { a+=Cnk(j,x+1); our[x]=v; x++; }
                }
            }
            for(int j=maxOur;j>=0;j--) {
                int k=0;
                while(our[j]!=start+j) { rotateLeft(our.data(),0,j); k++; }
                b=(j+1)*b+k;
            }
            return a*maxB+b;
        }
    }

    int URFtoDLF(int idx=-1) { return permutationIndex(0,(int)Corner::URF,(int)Corner::DLF,false,idx); }
    int URtoUL(int idx=-1)   { return permutationIndex(1,(int)Edge::UR,  (int)Edge::UL,   false,idx); }
    int UBtoDF(int idx=-1)   { return permutationIndex(1,(int)Edge::UB,  (int)Edge::DF,   false,idx); }
    int URtoDF(int idx=-1)   { return permutationIndex(1,(int)Edge::UR,  (int)Edge::DF,   false,idx); }
    int FRtoBR(int idx=-1)   { return permutationIndex(1,(int)Edge::FR,  (int)Edge::BR,   true, idx); }
};

inline std::vector<Cube> Cube::moves;
