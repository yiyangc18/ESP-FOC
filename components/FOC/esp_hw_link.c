/**************************************************************************//**
  \file     esp_hw_link.h
  \brief    this file contains the code implementation of hardware link.
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


