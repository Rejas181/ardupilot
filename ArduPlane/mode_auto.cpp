#include "mode.h"
#include "Plane.h"

bool ModeAuto::_enter()
{
#if HAL_QUADPLANE_ENABLED
    // check if we should refuse auto mode due to a missing takeoff in
    // guided_wait_takeoff state
    if (plane.previous_mode == &plane.mode_guided &&
        quadplane.guided_wait_takeoff_on_mode_enter) {
        if (!plane.mission.starts_with_takeoff_cmd()) {
            gcs().send_text(MAV_SEVERITY_ERROR,"Takeoff waypoint required");
            quadplane.guided_wait_takeoff = true;
            return false;
        }
    }
    
    if (plane.quadplane.available() && plane.quadplane.enable == 2) {
        plane.auto_state.vtol_mode = true;
    } else {
        plane.auto_state.vtol_mode = false;
    }
#else
    plane.auto_state.vtol_mode = false;
#endif
    plane.next_WP_loc = plane.prev_WP_loc = plane.current_loc;
    // start or resume the mission, based on MIS_AUTORESET
    plane.mission.start_or_resume();

    if (hal.util->was_watchdog_armed()) {
        if (hal.util->persistent_data.waypoint_num != 0) {
            gcs().send_text(MAV_SEVERITY_INFO, "Watchdog: resume WP %u", hal.util->persistent_data.waypoint_num);
            plane.mission.set_current_cmd(hal.util->persistent_data.waypoint_num);
            hal.util->persistent_data.waypoint_num = 0;
        }
    }

#if HAL_SOARING_ENABLED
    plane.g2.soaring_controller.init_cruising();
#endif

    return true;
}

void ModeAuto::_exit()
{
    if (plane.mission.state() == AP_Mission::MISSION_RUNNING) {
        plane.mission.stop();

        bool restart = plane.mission.get_current_nav_cmd().id == MAV_CMD_NAV_LAND;
#if HAL_QUADPLANE_ENABLED
        if (plane.quadplane.is_vtol_land(plane.mission.get_current_nav_cmd().id)) {
            restart = false;
        }
#endif
        if (restart) {
            plane.landing.restart_landing_sequence();
        }
    }
    plane.auto_state.started_flying_in_auto_ms = 0;
}

void ModeAuto::update()
{

    // ===================================== datos a registrar ==================================================================================
    float pitch_input = plane.channel_pitch->norm_input();       //demanda de cabeceo normalizado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input(); //demanda de acelerador  normalizado de -1 a 1 de 1000 a 2000 PWM
    float gamma_d;                                               //valor de gamma deseado   

    if(pitch_input > 0) {gamma_d=pitch_input*16;}                 //ajuste del angulo deseado de cabeceo
    else {gamma_d=pitch_input*24;}
    float V_d = 26*throttle_input+10;

    //float temp = pitch_input+throttle_input;            //Variable temporal para que no me marque error el compilado
    //printf("variable temporal: %f \n",temp);            //Función temporal para que no me marque error el compilado 
   
    //--------------------Datos de los sensores--------------------
    Vector3f airspeed_vec_bf;                           //Vector de Velocidad aerodinámica en sistema cuerpo en m/s
    Vector3f vel_ang;                                   //Vector de velocidad angular en sistema cuerpo en lo que yo espero sean rad/s
    
    // display -- el if es para verificar que si hay un dato para mostrar
    AP::ahrs().airspeed_vector_true(airspeed_vec_bf);
    
    //velocidad angular
    vel_ang = ahrs.get_gyro();
    float theta=ahrs.get_pitch();

    plane.Log_Write_Manual(float(V_d),float(gamma_d),float(airspeed_vec_bf.x),float(airspeed_vec_bf.z),float(theta),float(vel_ang.y)); //Escritura del registro

    //======================================Control original=========================================================================

    if (plane.mission.state() != AP_Mission::MISSION_RUNNING) {
        // this could happen if AP_Landing::restart_landing_sequence() returns false which would only happen if:
        // restart_landing_sequence() is called when not executing a NAV_LAND or there is no previous nav point
        plane.set_mode(plane.mode_rtl, ModeReason::MISSION_END);
        gcs().send_text(MAV_SEVERITY_INFO, "Aircraft in auto without a running mission");
        return;
    }

    uint16_t nav_cmd_id = plane.mission.get_current_nav_cmd().id;

#if HAL_QUADPLANE_ENABLED
    if (plane.quadplane.in_vtol_auto()) {
        plane.quadplane.control_auto();
        return;
    }
#endif

#if AP_PLANE_GLIDER_PULLUP_ENABLED
    if (pullup.in_pullup()) {
        return;
    }
#endif

    if (nav_cmd_id == MAV_CMD_NAV_TAKEOFF ||
        (nav_cmd_id == MAV_CMD_NAV_LAND && plane.flight_stage == AP_FixedWing::FlightStage::ABORT_LANDING)) {
        plane.takeoff_calc_roll();
        plane.takeoff_calc_pitch();
        plane.takeoff_calc_throttle();
    } else if (nav_cmd_id == MAV_CMD_NAV_LAND) {
        plane.calc_nav_roll();
        plane.calc_nav_pitch();

        // allow landing to restrict the roll limits
        plane.nav_roll_cd = plane.landing.constrain_roll(plane.nav_roll_cd, plane.g.level_roll_limit*100UL);

        if (plane.landing.is_throttle_suppressed()) {
            // if landing is considered complete throttle is never allowed, regardless of landing type
            SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, 0.0);
        } else {
            plane.calc_throttle();
        }
#if AP_SCRIPTING_ENABLED
    } else if (nav_cmd_id == MAV_CMD_NAV_SCRIPT_TIME) {
        // NAV_SCRIPTING has a desired roll and pitch rate and desired throttle
        plane.nav_roll_cd = ahrs.roll_sensor;
        plane.nav_pitch_cd = ahrs.pitch_sensor;
#endif
    } else {
        // we are doing normal AUTO flight, the special cases
        // are for takeoff and landing
        if (nav_cmd_id != MAV_CMD_NAV_CONTINUE_AND_CHANGE_ALT) {
            plane.steer_state.hold_course_cd = -1;
        }
        plane.calc_nav_roll();
        plane.calc_nav_pitch();
        plane.calc_throttle();
    }
    /*
    //----------------------telemetria de prueba--------------------------
    Vector3f airspeed_vec_bf;
    Vector3f vel_ang;
    if (AP::ahrs().airspeed_vector_true(airspeed_vec_bf)) {
        // if we don't have an airspeed estimate then we don't have a
        // valid wind estimate on blimps
        //printf("velocidad aerodinamica esitmada: %f, %f, %f,\n",airspeed_vec_bf[0],airspeed_vec_bf[1],airspeed_vec_bf[2]);
        printf("velocidad aerodinamica esitmada: %f, %f, %f,\n",airspeed_vec_bf.x,airspeed_vec_bf.y,airspeed_vec_bf.z);
    }
    
    vel_ang = ahrs.get_gyro();
    printf("velocidad angular correjida: %f, %f, %f,\n",vel_ang.x,vel_ang.y,vel_ang.z);
    */
}

void ModeAuto::navigate()
{
    if (AP::ahrs().home_is_set()) {
        plane.mission.update();
    }
}


bool ModeAuto::does_auto_navigation() const
{
#if AP_SCRIPTING_ENABLED
   return (!plane.nav_scripting_active());
#endif
   return true;
}

bool ModeAuto::does_auto_throttle() const
{
#if AP_SCRIPTING_ENABLED
   return (!plane.nav_scripting_active());
#endif
   return true;
}

// returns true if the vehicle can be armed in this mode
bool ModeAuto::_pre_arm_checks(size_t buflen, char *buffer) const
{
#if HAL_QUADPLANE_ENABLED
    if (plane.quadplane.enabled()) {
        if (plane.quadplane.option_is_set(QuadPlane::OPTION::ONLY_ARM_IN_QMODE_OR_AUTO) &&
                !plane.quadplane.is_vtol_takeoff(plane.mission.get_current_nav_cmd().id)) {
            hal.util->snprintf(buffer, buflen, "not in VTOL takeoff");
            return false;
        }
        if (!plane.mission.starts_with_takeoff_cmd()) {
            hal.util->snprintf(buffer, buflen, "missing takeoff waypoint");
            return false;
        }
    }
#endif
    // Note that this bypasses the base class checks
    return true;
}

bool ModeAuto::is_landing() const
{
    return (plane.flight_stage == AP_FixedWing::FlightStage::LAND);
}

void ModeAuto::run()
{
#if AP_PLANE_GLIDER_PULLUP_ENABLED
    if (pullup.in_pullup()) {
        pullup.stabilize_pullup();
        return;
    }
#endif
    
    if (plane.mission.get_current_nav_cmd().id == MAV_CMD_NAV_ALTITUDE_WAIT) {

        wiggle_servos();

        SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, 0.0);
        SRV_Channels::set_output_scaled(SRV_Channel::k_throttleLeft, 0.0);
        SRV_Channels::set_output_scaled(SRV_Channel::k_throttleRight, 0.0);

        SRV_Channels::set_output_to_trim(SRV_Channel::k_throttle);
        SRV_Channels::set_output_to_trim(SRV_Channel::k_throttleLeft);
        SRV_Channels::set_output_to_trim(SRV_Channel::k_throttleRight);

        // Relax attitude control
        reset_controllers();

    } else {
        // Normal flight, run base class
        Mode::run();

    }
}
