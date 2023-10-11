/**************************************************************************//**
  \file     config.h
  \brief    used to link the hardware
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
    \brief     convert floating point numbers to int32 type data
    \param[in] data0: floating point type data to be converted
    \retval    converted int32 type data
*/
unsigned int float_to_int32(float data0) ;

/*!
    \brief     convert int32 to floating point numbers type data
    \param[in] data0: int32 type data to be converted
    \retval    converted floating point type data
*/
float int32_to_float(unsigned int data0) ;

/*!
    \brief motor phase sequence flag variable
*/
extern volatile unsigned char phase_sequence ;
void update_pwm_dutycycle(float ch0, float ch1, float ch2) ;

float getSensorAngle(void);

#define log_report(tag, format, ...) ESP_LOGI(tag, format, ##__VA_ARGS__)


float _normalizeAngle(float angle);


#endif //ESPFOC__HW_LINK_H_
