#include "mode.h"
#include "Plane.h"
#include <stdio.h>              // This example main program uses printf/fflush
#include "AP_gain_scheduling/controlador.h"               // Model header file

static controlador Control;              // Instance of model class

void ModeGS::update()
{

    //-------------------------Entradas RC------------------------
    float pitch_input = plane.channel_pitch->norm_input();       //demanda de cabeceo normalizado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input(); //demanda de acelerador  normalizado de -1 a 1 de 1000 a 2000 PWM
    float pitch_out;                                             //salida del elevador en PWM
    float throttle_out;                                          //salida del acelerador en PWM
    float gamma_d;                                               //valor de gamma deseado   

    if(pitch_input > 0) {gamma_d=pitch_input*16;}                 //ajuste del angulo deseado de cabeceo
    else {gamma_d=pitch_input*24;}
    float V_d=0.0260*throttle_out-16;

    //float temp = pitch_input+throttle_input;            //Variable temporal para que no me marque error el compilado
    //printf("variable temporal: %f \n",temp);            //Función temporal para que no me marque error el compilado 

    //display valores deseados
    printf("valores deseados: gamma: %f, V: %f,\n",gamma_d,V_d);
    
    //--------------------Datos de los sensores--------------------
    Vector3f airspeed_vec_bf;                           //Vector de Velocidad aerodinámica en sistema cuerpo en m/s
    Vector3f vel_ang;                                   //Vector de velocidad angular en sistema cuerpo en lo que yo espero sean rad/s
    
    // display -- el if es para verificar que si hay un dato para mostrar
    if (AP::ahrs().airspeed_vector_true(airspeed_vec_bf)) {
        printf("velocidad aerodinámica estimada: %f, %f, %f,\n",airspeed_vec_bf.x,airspeed_vec_bf.y,airspeed_vec_bf.z);
    }
    printf("angulo de cabeceo: %f,\n",ahrs.get_pitch());
    
    // display de velocidad angular
    vel_ang = ahrs.get_gyro();
    printf("velocidad angular corregida: %f, %f, %f,\n",vel_ang.x,vel_ang.y,vel_ang.z);

    //-------------controlador de ganancias programadas obtenido mediante MATLAB------------
    //Escritura de Variables de entrada
    Control.rtU.u=airspeed_vec_bf.x;
    Control.rtU.w=airspeed_vec_bf.z;
    Control.rtU.theta=ahrs.get_pitch();
    Control.rtU.q=vel_ang.y;
    Control.rtU.gammad=gamma_d;
    Control.rtU.Vd=V_d;                                //ajuste a velocidad maxima de 36 m/s
    //paso del controlador   
    Control.step();                                                  //step
    //Escritura de variables de salida                      
    pitch_out=66.175*Control.rtY.de+1417.6;                          //conversion de variable de salida dT a periodo en s
    throttle_out=1145.91*Control.rtY.dT+1500;                        //conversion de variable de salida de a periodo en s (+30�-20�)
    
    //variables en la estructura para monitoreo (en simulink y en el programa anterior)
    //wd=Control.rtY.wd;
    //Vreal=Control.rtY.V;
    //gammareal=Control.rtY.gamma;


    //display de las señales de control
    printf("señales de control: %f, %f,\n",pitch_out,throttle_out);
    
    //------------------------------------Salidas PWM-------------------------------------
    //Envio de señales PWM
    SRV_Channels::set_output_scaled(SRV_Channel::k_aileron, plane.roll_in_expo(false));         //Salida manual del alerón (obtenida de: mode manual)
    SRV_Channels::set_output_pwm_chan(SRV_Channel::k_elevator, pitch_out);                      //Salida del elevador
    output_rudder_and_steering(plane.rudder_in_expo(false));                                    //Salida manual del Timón (obtenida de: mode manual)
    SRV_Channels::set_output_pwm_chan(SRV_Channel::k_throttle, throttle_out);                   //Salida del acelerador
   
   
}

void ModeGS::run()
{
    // Run base class function and then output throttle
    Mode::run();
    Control.initialize();                    // Initialize model
    //output_pilot_throttle();
}