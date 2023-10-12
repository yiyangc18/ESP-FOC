/**
 * @file    sensor.h 
 * @brief   传感器驱动头文件
 * @author   Chery
 * @date     October 2023
 */

#ifndef ESPFOC_HALLSENSOR_H_
#define ESPFOC_HALLSENSOR_H_

#include <stdio.h>
#include <string.h>
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define MT6701_MISO 1
#define MT6701_SCLK 2
#define MT6701_SS 42

#define SPI_CLK 1000000 
#define SPI_MODE0 0
#define HSPI_HOST   2

#define PI 3.1415926535897932384626433832795
#define TWO_PI 6.283185307179586476925286766559



/**
 * @brief 初始化传感器回调函数
 * 
 * 此函数用于初始化传感器回调函数，以便在应用程序中使用。
 * 
 */
void hall_sensor_init(void);

/**
 * @brief 读取传感器回调函数
 * 
 * 此函数用于读取传感器的位置信息，并将其转换为弧度。
 * 
 * @return 传感器位置的弧度值
 */
float hall_sensor_read_angle_in_radians(void);


/**
 * @brief 测试任务
 */
void readSensorTask(void *pvParameters);



#endif //ESPFOC_HALLSENSOR_H_
