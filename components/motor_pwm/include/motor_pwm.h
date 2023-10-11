#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H

#define MO1_PIN 17
#define MO2_PIN 16
#define MO3_PIN 15

#define MOTOR_MCPWM_UNIT MCPWM_UNIT_0


/**
 * @brief 更新 PWM 占空比
 * 
 * 此函数用于更新 PWM 的占空比。
 * 
 * @param ch0 PWM 通道 0 的占空比
 * @param ch1 PWM 通道 1 的占空比
 * @param ch2 PWM 通道 2 的占空比
 */
void set_motorpwm_dutycycle(float ch0, float ch1, float ch2);

/**
 * @brief 电机pwm输出驱动初始化
 */
void motor_pwm_init(void);

/**
 * @brief 电机pwm输出驱动测试任务
 */
void motorPwmDriverTask(void *pvParameters);


#endif 
