/**************************************************************************//**
  \file     filter.c
  \brief    控制器的低通滤波器传递函数为:
            \[G_f = \frac{1}{1+sT_f}\]
            离散化为:
            \[v_f(k)=\frac{T_f}{T_f+T_s}v_f(k-1)+\frac{T_S}{T_f+T_s}v(k)\]
            其中vf(k)为k时刻的滤波值， v(k)为k时刻的速度测量值， 
            Tf是滤波时间常数，Ts是采样时间（或上述式子的时间间隔）。 
            这个低通滤波器也可以写成这样的形式: \(v_f(k)=\alpha v_f(k-1)+(1-\alpha)v(k)\) 
            其中：\[\alpha=\frac{T_f}{T_f+T_s}\]
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#include "filter.h"
#include "foc_config.h"

#include <stdint.h>

Filter_Structure_t velocity_filter;

void init_lpf(Filter_Structure_t* lpf, float time_constant) {
    lpf->time_constant = time_constant;
    lpf->y_prev = 0.0f;
    lpf->timestamp_prev = 0;
}

float filter_update_value(Filter_Structure_t* lpf, float x) {
    uint32_t timestamp = micros();
    float dt = (timestamp - lpf->timestamp_prev) * 1e-6f;

    if (dt < 0.0f) {
        dt = 1e-3f;
    } else if (dt > 0.3f) {
        lpf->y_prev = x;
        lpf->timestamp_prev = timestamp;
        return x;
    }

    float alpha = lpf->time_constant / (lpf->time_constant + dt);
    float y = alpha * lpf->y_prev + (1.0f - alpha) * x;
    lpf->y_prev = y;
    lpf->timestamp_prev = timestamp;
    return y;
}