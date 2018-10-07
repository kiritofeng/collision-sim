#include<bits/stdc++.h>             // placeholder, import each library later

#include "physics_h/physics.h"      // all purpose APP library, by Kirito Feng

#include "debug.h"                  // debugging library

// some typedefs to make my coding nicer
typedef long double llf;
typedef vector<llf> vllf;

// Simulation constants
const llf DELTA_T = 1e-6;           // how much each increment of time is
const llf END = 30.0;               // when simulation ends, in seconds

// Ball constants
const llf RADIUS = 0.05;            // radius of ball, in metres
const llf MASS = 0.05;              // mass of ball, in kg

class ball {
    private:
        // displacement, velocity, and acceleration vectors
        vllf s, v, a;

        // radius of ball
        llf radius;

        // mass of ball
        llf mass;

    public:
        ball(vllf _s, vllf _v) : s(_s), v(_v), a(0, 0), radius(RADIUS), mass(MASS) {}

        // the spring force
        inline static vllf spring_force (vllf x) {
            return x * -1.0;
        }

        // return displacement
        inline vllf get_s() const {
            return s;
        }

        // update acceleration of ball
        inline void update(vllf _a) {
            a = _a;
            advance();
        }

        // the most important part
        inline void advance() {
            s = s + v * DELTA_T + a * (0.5 * DELTA_T * DELTA_T);
            v = v + a * DELTA_T;
        }

        // return radius of ball
        inline llf get_radius() const {
            return radius;
        }

        // return mass of ball
        inline llf get_mass() const {
            return mass;
        }

};
inline bool in_contact(const ball &b1, const ball &b2) {
    return (b1.get_s() - b2.get_s()).magnitude() <= 4*RADIUS*RADIUS;
}

inline vllf displacement(ball b1, ball b2) {
    return b1.get_s() - b2.get_s();
}

int main(){
    ball b1 = ball(vllf(0, 0), vllf(0, 0));
    ball b2 = ball(vllf(11, 11), vllf(0, 0));
    vllf s = displacement(b1, b2);
    for(llf t = 0; t < END; t += DELTA_T) {
        std::cout << b1.get_s().x << "\t" << b1.get_s().y << "\t"
                  << b2.get_s().y << "\t" << b2.get_s().y << std::endl;
        s = displacement(b1, b2);

        bool contact = in_contact(b1, b2);
        if(contact) {
            // get the change in displacement
            vllf delta_s = s - displacement(b1, b2);
            // get the spring force
            vllf f_s_1 = b1.spring_force(delta_s * 0.5);
            vllf f_s_2 = b2.spring_force(-delta_s * 0.5);
            b1.update(f_s_1 * (1 / b1.get_mass()));
            b2.update(f_s_2 * (1 / b2.get_mass()));
        } else {
            b1.advance();
            b2.advance();
        }
    }
}
