/**
 * @file timer_loop.h
 * @brief 定时器循环控制模块头文件
 * 
 * 该模块提供了定时器循环控制的功能，可以通过配置定时器参数来实现定时循环执行指定的任务。
 * @author   Chery
 * @date     October 2023
 */

#ifndef TIMER_LOOP_H_
#define TIMER_LOOP_H_

#include <stdio.h>
#include "esp_log.h"
#include "esp_timer.h"


typedef void (*loop_timer_cb)(void);

/**
 * @brief 定时器循环控制配置结构体
 *  该结构体用于配置定时器循环控制的参数，包括定时器编号、定时器分频系数、定时器周期等。
 */
typedef struct {
    loop_timer_cb callback;        //!< Function to call when timer expires
    const char* name;               //!< Timer name, used in esp_timer_dump function
    int period;                //!< Period of timer (in microseconds)
    int timer_index;         //!< Timer index
} loop_timer_config_t;

/**
 * @brief 初始化定时器循环控制模块
 * 
 * 该函数用于初始化定时器循环控制模块，根据传入的配置参数初始化定时器，并启动定时器循环控制。
 * 
 * @param config 定时器循环控制配置结构体指针
 * 
 */
void loop_timer_init(loop_timer_config_t *config);


#endif //TIMER_LOOP_H_
