#include<bits/stdc++.h>             // placeholder, import each library later

#include "physics_h/physics.h"      // all purpose APP library, by Kirito Feng

#include "debug.h"                  // debugging library

// some typedefs to make my coding nicer
typedef long double llf;
typedef vector<llf> vllf;

// Simulation constants
const llf DELTA_T = 1e-4;           // how much each increment of time is
const llf END = 1.00;               // when simulation ends, in seconds

// Ball constants
const llf RADIUS = 0.03;            // radius of ball, in metres
const llf MASS = 0.1;               // mass of ball, in kg

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
            return x * -1500;
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
/**
    Helper method to check if two balls are in contact

    @param b1 the first ball
    @param b2 the second ball
    @return true if the two balls are in contact (distance between centres is 2 x RADIUS
             and false otherwise
 */
inline bool in_contact(const ball &b1, const ball &b2) {
    return (b1.get_s() - b2.get_s()).magnitude() <= 4*RADIUS*RADIUS;
}

/**
    Helper method that return the displacement between the centres of the balls

    @param b1 the first ball
    @param b2 the second ball
    @return the displacement between the two balls, based on the difference of `get_s()`
*/
inline vllf displacement(ball b1, ball b2) {
    return b1.get_s() - b2.get_s();
}

int main(){
    // initialize the two balls
    ball b1 = ball(vllf(0, 0.02), vllf(5, 0));
    ball b2 = ball(vllf(0.2, 0), vllf(2, 0.2));
    // run the simulation, recomputing the values every DELTA_T
    for(llf t = 0; t < END; t += DELTA_T) {
        // print the displacements, separated by commas
        // this is the output format for .csv files
        // because screw Microsoft software formats *cough* xlsx *cough*
        std::cout << b1.get_s().x << "," << b1.get_s().y << ","
                  << b2.get_s().x << "," << b2.get_s().y << std::endl;

        // boolean to store if the balls are in contact
        bool contact = in_contact(b1, b2);
        if(contact) {
            // get the change in displacement
            vllf delta_s = displacement(b1,b2).normalize() * (RADIUS + RADIUS) - displacement(b1, b2);
            // get the spring force
            vllf f_s_1 = b1.spring_force(-delta_s * 0.5);
            vllf f_s_2 = b2.spring_force(delta_s * 0.5);
            // apply Newton's 2nd law to get the acceleration
            // and update the balls
            b1.update(f_s_1 * (1 / b1.get_mass()));
            b2.update(f_s_2 * (1 / b2.get_mass()));
        } else {
            // acceleration does not change, just advance the simulation
            b1.advance();
            b2.advance();
        }
    }
}
