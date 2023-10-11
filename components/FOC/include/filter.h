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

#ifndef ESPFOC_ALGORITHM_FILTER_H_
#define ESPFOC_ALGORITHM_FILTER_H_

typedef struct {
    float time_constant;
    float y_prev;
    int timestamp_prev;
} Filter_Structure_t;

extern Filter_Structure_t velocity_filter;

/**
 * @brief 初始化低通滤波器
 * 
 * @param lpf 指向低通滤波器的指针
 * @param time_constant 滤波时间常数
 */
void init_lpf(Filter_Structure_t* lpf, float time_constant);

/**
 * @brief 低通滤波器
 * 
 * @param lpf 指向低通滤波器的指针
 * @param x 输入值
 * @return float 输出值
 */
float filter_update_value(Filter_Structure_t* lpf, float x);

#endif //ESPFOC_ALGORITHM_FILTER_H_
