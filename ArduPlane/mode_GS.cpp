#include "mode.h"
#include "Plane.h"

void ModeGS::update()
{
    float pitch_input = plane.channel_pitch->norm_input();       //demanda de cabeceo normalizado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input(); //demanda de acelerador  normalizado de -1 a 1 de 1000 a 2000 PWM
    float pitch_out;                                             //salida del elevador en PWM
    float throttle_out;                                          //salida del acelerador en PWM

    float temp = pitch_input+throttle_input;            //Variable temporal para que no me marque error el compilado
    printf("variable temporal: %f \n",temp);            //Función temporal para que no me marque error el compilado   
    Vector3f airspeed_vec_bf;                           //Vector de Velocidad aerodinámica en sistema cuerpo en m/s
    Vector3f vel_ang;                                   //Vector de velocidad angular en sistema cuerpo en lo que yo espero sean rad/s
    
    // display -- el if es para verificar que si hay un dato para mostrar
    if (AP::ahrs().airspeed_vector_true(airspeed_vec_bf)) {
        printf("velocidad aerodinamica esitmada: %f, %f, %f,\n",airspeed_vec_bf.x,airspeed_vec_bf.y,airspeed_vec_bf.z);
    }
    // display de velocidad angular
    vel_ang = ahrs.get_gyro();
    printf("velocidad angular correjida: %f, %f, %f,\n",vel_ang.x,vel_ang.y,vel_ang.z);

    //controlador de ganancias programadas obtenido mediante MATLAB
    

    //Salidas PWM
    SRV_Channels::set_output_scaled(SRV_Channel::k_aileron, plane.roll_in_expo(false));     //Salida manual del alerón (obtenida de: mode manual)
    SRV_Channels::set_output_scaled(SRV_Channel::k_elevator, pitch_out);                    //Salida del elevador
    output_rudder_and_steering(plane.rudder_in_expo(false));                                //Salida manual del Timón (obtenida de: mode manual)
    SRV_Channels::set_output_scaled(SRV_Channel::k_throttle, throttle_out);                     //Salida del acelerador
   
   
}

void ModeGS::run()
{
    // Run base class function and then output throttle
    Mode::run();

    output_pilot_throttle();
}