/**************************************************************************//**
  \file     esp_hw_link.h
  \brief    this file contains the code implementation of hardware link.
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#ifndef ESPFOC__HW_LINK_H_
#define ESPFOC__HW_LINK_H_

#include <math.h>
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "driver/timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "motor_pwm.h"
#include "sensor.h"


int micros(void);

void delayms(uint32_t ms);

/*!
    \brief motor phase sequence flag variable
*/
extern volatile unsigned char phase_sequence ;
void update_pwm_dutycycle(float ch0, float ch1, float ch2) ;

float getSensorAngle(void);

#define log_report(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)


#endif //ESPFOC__HW_LINK_H_
