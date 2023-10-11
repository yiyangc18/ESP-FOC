/**************************************************************************//**
  \file     config.h
  \brief    used to link the hardware
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#include "esp_hw_link.h"

int micros(void)
{
    return esp_timer_get_time();
}

void delayms(uint32_t ms) {
    TickType_t xDelay = pdMS_TO_TICKS(ms);
    vTaskDelay(xDelay);
}

/*!
    \brief     convert floating point numbers to int32 type data
    \param[in] data0: floating point type data to be converted
    \retval    converted int32 type data
*/
unsigned int float_to_int32(float data0) {
    unsigned int uintp32 = (*((unsigned int *) (&data0)));
    return uintp32;
}

/*!
    \brief     convert int32 to floating point numbers type data
    \param[in] data0: int32 type data to be converted
    \retval    converted floating point type data
*/
float int32_to_float(unsigned int data0) {
    float fp32 = (*((float *) (&data0)));
    return fp32;
}

volatile unsigned char phase_sequence = 0;
void update_pwm_dutycycle(float ch0, float ch1, float ch2) {
    
    float duty0 = ch0 * 100.0 ;
    float duty1 = ch1 * 100.0 ;
    float duty2 = ch2 * 100.0 ;

    if (phase_sequence == 0)
        set_motorpwm_dutycycle(duty0, duty1, duty2);
    else
        set_motorpwm_dutycycle(duty1, duty0, duty2);

    //更新 PWM 占空比
    

}

float getSensorAngle(void){
    return hall_sensor_read_angle_in_radians();
}

#define _2PI 6.28318530718f
// normalizing radian angle to [0,2PI]
float _normalizeAngle(float angle){
  float a = fmodf(angle, _2PI);
  return a >= 0 ? a : (a + _2PI);
}
