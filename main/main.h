#ifndef ESPFOC_MAIN_H_
#define ESPFOC_MAIN_H_

#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "sdkconfig.h"


#include "sensor.h"
#include "motor_pwm.h"
#include "timer_loop.h"

#include "foc.h"

void foc_init(void);

void print_hello_Wrold(void);


#endif //ESPFOC_MAIN_H_
