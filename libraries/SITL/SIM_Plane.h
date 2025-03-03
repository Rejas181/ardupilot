/*
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
  simple plane simulator class
*/

#pragma once

#include "SIM_Aircraft.h"
#include "SIM_ICEngine.h"
#include <Filter/LowPassFilter.h>

namespace SITL {

/*
  a very simple plane simulator
 */
class Plane : public Aircraft {
public:
    Plane(const char *frame_str);

    /* update model by one time step */
    virtual void update(const struct sitl_input &input) override;

    /* static object creator */
    static Aircraft *create(const char *frame_str) {
        return NEW_NOTHROW Plane(frame_str);
    }

protected:
    const float hover_throttle = 0.7f;
    float angle_of_attack;
    float beta;

    struct {
        // from last_letter skywalker_2013/aerodynamics.yaml
        // thanks to Georacer!
        float s = 0.276;             //0.45
        float b = 1;             //1.88
        float c = 0.245;             //0.24
        float c_lift_0 = 0.20644;      //0.56
        float c_lift_deltae = 0.03558;    //0
        float c_lift_a = 4.1622;       //6.9
        float c_lift_q = 0.03907;         //0
        float mcoeff = 50;          //50
        float oswald = 0.9;         //0.9
        float alpha_stall = 0.4712; //0.4712
        float c_drag_q = 0;         //0
        float c_drag_deltae = 0.009;  //0.0
        float c_drag_p = 0.055925;       //0.1
        float c_y_0 = 0;            //0
        float c_y_b = -0.98;        //-0.98
        float c_y_p = 0;            //0
        float c_y_r = 0;            //0
        float c_y_deltaa = 0;       //0  
        float c_y_deltar = -0.2;    //-0.2
        float c_l_0 = 0;            //0
        float c_l_p = -1.0;         //-1.0
        float c_l_b = -0.12;        //-0.12
        float c_l_r = 0.14;         //0.14
        float c_l_deltaa = 0.25;    //0.25
        float c_l_deltar = -0.037;  //-0.037
        float c_m_0 = 0.045;        //0.045
        float c_m_a = -1.0849;         //-0.7
        float c_m_q = -0.02576;          //-20
        float c_m_deltae = -0.58015;     //1.0
        float c_n_0 = 0;            //0
        float c_n_b = 0.25;         //0.25
        float c_n_p = 0.022;        //0.022
        float c_n_r = -1;           //-1
        float c_n_deltaa = 0.00;    //0.00
        float c_n_deltar = 0.1;     //0.1
        float deltaa_max = 0.3491;  //0.3491
        float deltae_max = 0.5236;  //0.3491
        float deltar_max = 0.3491;  //0.3491
        // the X CoG offset should be -0.02, but that makes the plane too tail heavy
        // in manual flight. Adjusted to -0.15 gives reasonable flight
        Vector3f CGOffset{-0.15, 0, -0.05};
    } coefficient;

    float thrust_scale;
    bool reverse_thrust;
    bool elevons;
    bool vtail;
    bool dspoilers;
    bool reverse_elevator_rudder;
    bool ice_engine;
    bool tailsitter;
    bool aerobatic;
    bool copter_tailsitter;
    bool have_launcher;
    bool have_steering;
    float launch_accel;
    float launch_time;
    uint64_t launch_start_ms;

    const uint8_t throttle_servo = 2;
    const int8_t choke_servo = 14;
    const int8_t ignition_servo = 12;
    const int8_t starter_servo = 13;
    const float slewrate = 100;
    ICEngine icengine{
        throttle_servo,
        choke_servo,
        ignition_servo,
        starter_servo,
        slewrate,
        true
    };

    float liftCoeff(float alpha) const;
    float dragCoeff(float alpha) const;
    Vector3f getForce(float inputAileron, float inputElevator, float inputRudder) const;
    Vector3f getTorque(float inputAileron, float inputElevator, float inputRudder, float inputThrust, const Vector3f &force) const;
    void calculate_forces(const struct sitl_input &input, Vector3f &rot_accel);
};

} // namespace SITL
