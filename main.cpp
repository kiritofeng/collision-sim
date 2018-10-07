#include<bits/stdc++.h>             // placeholder, import each library later

#include "physics_h/physics.h"      // all purpose APP library, by Kirito Feng

#include "debug.h"                  // debugging library

// some typedefs to make my coding nicer
typedef long double llf;
typedef vector<llf> vllf;

const llf DELTA_T = 1e-6;           // how much each increment of time is
const llf RADIUS = 0.05;            // radius of balls, in metres
const llf END = 30.0;               // when simulation ends, in seconds

class ball {
    private:
        // the spring constant
        inline static llf k (llf compression) {
            return 1.0;
        }
        // displacement, velocity, and acceleration vectors
        vllf s, v, a;
        // radius of ball
        llf radius;
        bool in_contact
    public:
        ball(vllf _s, vllf _v) {
            s = _s;
            v = _v;
            radius = RADIUS;
        }
        // return displacement
        inline vllf get_s() const {
            return s;
        }
        // the most important part
        inline void advance() {
            // write later uwah~~
        }
        inline void toggle_contact() {
            in_contact ^= 1;
        }
        inline llf get_radius(){
            return radius;
        }

};
inline bool in_contact(const ball &b1, const ball &b2) {
    return (b1.get_s() - b2.get_s()).magnitude() <= 4*RADIUS*RADIUS;
}
int main(){
}
