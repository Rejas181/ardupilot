#include "mode.h"
#include "Plane.h"

void ModeManual::update()
{
// ===================================== datos a registrar ==================================================================================
    float pitch_input = plane.channel_pitch->norm_input();       //demanda de cabeceo normalizado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input(); //demanda de acelerador  normalizado de -1 a 1 de 1000 a 2000 PWM
    float pitch_out;                                             //salida del elevador en PWM
    float throttle_out;                                          //salida del acelerador en PWM
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


//===================================================== controlador original ================================================================================

    SRV_Channels::set_output_scaled(SRV_Channel::k_aileron, plane.roll_in_expo(false));
    SRV_Channels::set_output_scaled(SRV_Channel::k_elevator, plane.pitch_in_expo(false));
    //printf("valor en expo: %f\n", plane.pitch_in_expo(false));
    output_rudder_and_steering(plane.rudder_in_expo(false));

    const float throttle = plane.get_throttle_input(true);
    SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, throttle);

    plane.nav_roll_cd = ahrs.roll_sensor;
    plane.nav_pitch_cd = ahrs.pitch_sensor;
}

void ModeManual::run()
{
    reset_controllers();
}

// true if throttle min/max limits should be applied
bool ModeManual::use_throttle_limits() const
{
#if HAL_QUADPLANE_ENABLED
    if (quadplane.available() && quadplane.option_is_set(QuadPlane::OPTION::IDLE_GOV_MANUAL)) {
        return true;
    }
#endif
    return false;

}
