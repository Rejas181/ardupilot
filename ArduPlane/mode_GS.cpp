#include "mode.h"
#include "Plane.h"

void ModeGS::update()
{
    float pitch_input = plane.channel_pitch->norm_input();  //demanda de cabeceo nomrmailzado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input();

    //plane.get_throttle_input(true) //obtener la entrada de 
   
}

void ModeGS::run()
{
    // Run base class function and then output throttle
    Mode::run();

    output_pilot_throttle();
}