#include "mode.h"
#include "Plane.h"

void ModeGS::update()
{
    float pitch_input = plane.channel_pitch->norm_input();  //demanda de cabeceo nomrmailzado de -1 a 1 de 1000 a 2000 PWM
    float throttle_input = plane.channel_throttle->norm_input();

    float temp = pitch_input+throttle_input;
    printf("variable temporal: %f \n",temp);
    //plane.get_throttle_input(true) //obtener la entrada de   
    Vector3f airspeed_vec_bf;
    if (AP::ahrs().airspeed_vector_true(airspeed_vec_bf)) {
        // if we don't have an airspeed estimate then we don't have a
        // valid wind estimate on blimps
        printf("velocidad aerodinamica esitmada: %f, %f, %f,\n",airspeed_vec_bf[1],airspeed_vec_bf[2]),airspeed_vec_bf[3];
    }
    
   
}

void ModeGS::run()
{
    // Run base class function and then output throttle
    Mode::run();

    output_pilot_throttle();
}