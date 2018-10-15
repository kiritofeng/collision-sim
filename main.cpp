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

        // angular angular velocity, and angular acceleration vectors
        vllf omega, alpha;

        // radius of ball
        llf radius;

        // mass of ball
        llf mass;

    public:
        ball(vllf _s, vllf _v) : s(_s), v(_v), a(0, 0), omega(0, 0), alpha(0, 0), radius(RADIUS), mass(MASS) {}

        // the spring force
        inline static vllf spring_force (vllf x) {
            return x * -1500;
        }

        // return displacement
        inline vllf get_s() const {
            return s;
        }

        // return velocity
        inline vllf get_v() const {
            return v;
        }

        // update acceleration of ball
        inline void update1(vllf _a) {
            a = _a;
        }

        // update angular acceleration of ball
        inline void update2( vllf _alpha) {
            alpha = _alpha;
        }

        // advance velocity and displacement
        inline void advance1() {
            s = s + v * DELTA_T + a * (0.5 * DELTA_T * DELTA_T);
            v = v + a * DELTA_T;
        }

        // advance rotational values
        inline void advance2() {
            omega = omega + alpha * DELTA_T;
        }

        // return radius of ball
        inline llf get_radius() const {
            return radius;
        }

        // return mass of ball
        inline llf get_mass() const {
            return mass;
        }

        inline llf get_omega() const {
            return sqrt(omega.magnitude());
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
    Helper method that returns the displacement between the centres of the balls

    @param b1 the first ball
    @param b2 the second ball
    @return the displacement between the two balls, based on the difference of `get_s()`
*/
inline vllf displacement(ball b1, ball b2) {
    return b1.get_s() - b2.get_s();
}


/**
    Helper method that returns the kinetic energy of a ball

    @param b the ball
    @return the kinetic energy of the ball b
*/
inline llf KE(ball b) {
    return (0.5 * b.get_mass() * b.get_v().magnitude());
}

/**
    Helper method that returns the moment of inertia of a ball

    @param b the ball
    @return the moment of inertia of the ball
*/
inline llf moment(ball b){
    return 0.4 * b.get_mass() * b.get_radius() * b.get_radius();
}
int main(){
    // initialize the two balls
    ball b1 = ball(vllf(0, 0.02), vllf(5, 0));
    ball b2 = ball(vllf(0.2, 0), vllf(2, 0.2));
    // displacement between balls
    vllf s = displacement(b1, b2);
    // kinetic energy of balls
    llf k1 = KE(b1);
    llf k2 = KE(b2);
    // run the simulation, recomputing the values every DELTA_T
    for(llf t = 0; t < END; t += DELTA_T) {
        // print the displacements, separated by commas
        // this is the output format for .csv files
        // because screw Microsoft software formats *cough* xlsx *cough*
        std::cout << b1.get_s().x << "," << b1.get_s().y << ","
                  << b2.get_s().x << "," << b2.get_s().y << ","
                  << b1.get_omega() << "," << b2.get_omega() << std::endl;

        // boolean to store if the balls are in contact
        bool contact = in_contact(b1, b2);
        if(contact) {
            // get the change in compression
            vllf compression = displacement(b1,b2).normalize() * (RADIUS + RADIUS) - displacement(b1, b2);
            // get the spring force
            vllf f_s_1 = b1.spring_force(-compression * 0.5);
            vllf f_s_2 = b2.spring_force(compression * 0.5);
            // apply Newton's 2nd law to get the acceleration
            // update the balls' acceleration
            b1.update1(f_s_1 * (1 / b1.get_mass()));
            b2.update2(f_s_2 * (1 / b2.get_mass()));
            b1.advance1();
            b2.advance2();
            // now for the rotational part
            // first, get the change in displacement
            vllf delta_s = displacement(b1, b2) - s;
            // get the change in kinetic energy, divided by the change in displacement
            // this is the magnitude of the force
            llf delta_force_1 = (KE(b1) - k1) * (1 / sqrt(delta_s.magnitude()));
            llf delta_force_2 = (KE(b2) - k2) * (1 / sqrt(delta_s.magnitude()));
            // get the compressed radii
            // this is the center of collision
            vllf centre = (b1.get_s() + b2.get_s()) * 0.5;
            // get radii
            vllf r1 = centre - b1.get_s();
            vllf r2 = centre - b2.get_s();
            // get the force vector
            vllf f1 = centre + b1.get_v().normalize() * delta_force_1;
            vllf f2 = centre + b2.get_v().normalize() * delta_force_2;
            // torque = r cross f
            vllf t1 = r1 ^ f1;
            vllf t2 = r2 ^ f2;
            // divide by the moment to get the angular acceleration
            vllf alpha1 = t1 * (1 / moment(b1));
            vllf alpha2 = t2 * (1 / moment(b2));
            // update
            b1.update2(alpha1);
            b2.update2(alpha2);
            b1.advance2();
            b2.advance2();
        } else {
            // acceleration does not change, just advance the simulation
            b1.advance1();
            b1.advance2();
            b2.advance1();
            b2.advance2();
        }
        // update displacement vector
        s = displacement(b1, b2);
        // update kinetic energy
        k1 = KE(b1);
        k2 = KE(b2);
    }
}
