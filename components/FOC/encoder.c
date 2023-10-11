/**************************************************************************//**
  \file     encoder.c
  \brief    this file contains the code implementation of angle acquisition
            and initialization functions of sc60228 and sc60224.
  \author   Lao·Zhu
  \version  V1.0.2
  \date     29. October 2021
 ******************************************************************************/

#include "encoder.h"

const static char *TAG = "foc_encoder";

/*!
    \brief mechanical angle offset, which is used to align the mechanical
           angle with the zero point of the electrical angle.
*/
volatile float machine_angle_offset = 0.0;
/*!
    \brief the mechanical angle at the last moment is used to calculate
           the rotation speed of the motor rotor.
*/
volatile static float mechanical_angle_last = 0.0;
/*!
    \brief the mechanical angle of the motor rotor 
*/
volatile static float machine_angle_now = 0.0;

/*!
    \brief delay function for magnetic encoder
*/
void encoder_delay(void) {
    /* use loop functions to delay time */
    unsigned char delay_counter = 0xff;
    while (delay_counter)
        delay_counter--;
}

/*!
    \brief  read mechanical angle directly from encoder
    \retval the mechanical angle in radians, which is the raw data read back
            from the encoder.
*/
float encoder_get_mechanical_angle(void) {

    machine_angle_now = getSensorAngle() - machine_angle_offset;

    return machine_angle_now;
}

/*!
    \brief  according to the electrical angle calculated from the mechanical angle,
            this function will call encoder_get_mechanical_angle() function.
    \retval register raw data reading back
*/
float encoder_get_electronic_angle(void) {
    /* read back the mechanical angle directly from the magnetic encoder */
    FOC_Struct.mechanical_angle = encoder_get_mechanical_angle();

    /* calculate and update the mechanical angle and electric angle */
    float electric_angle = FOC_Struct.mechanical_angle * POLAR_PAIRS;  
    // electric_angle = fmodf(electric_angle, TWO_PI);  // 对 2π 取模，得到 0~2π 的电角度
    return electric_angle;
}

// #define FILTER_DATA_NUM 100

// float filter_data[FILTER_DATA_NUM];
// float filter_data_after[FILTER_DATA_NUM];
// int filter_data_index = 0;
/*!
    \brief called every 2 milliseconds to calculate the speed.
*/
void encoder_update_speed(void) {
    /* calculate the difference between this angle and the last angle */
    float angle_diff = machine_angle_now - mechanical_angle_last;
    //将机械角度之间的差值限制在-π到π之间，从而避免了跳变的情况
    if (angle_diff > _PI) angle_diff -= TWO_PI;
    else if (angle_diff < -_PI) angle_diff += TWO_PI;

    //得到的速度单位为 弧度/T_s，T_s = 1/SPEED_UP_FREQ 
    //快速换算 Omega_real = SPEED_UP_FREQ * tmp_mechanical_angle_velocity rad/s
    float tmp_mechanical_angle_velocity = angle_diff * SPEED_UP_FREQ;

    /* send it to low-pass filter for filtering to prevent PID high-frequency oscillation */
    FOC_Struct.rotate_speed =
        filter_update_value((Filter_Structure_t *) &velocity_filter, tmp_mechanical_angle_velocity);
    mechanical_angle_last = machine_angle_now;

    // log_report(TAG, "speed before filter: %f, after filter:%f", tmp_mechanical_angle_velocity, FOC_Struct.rotate_speed);

    // 将滤波前后的数据存入数组中 存满后打印分析
    // filter_data[filter_data_index] = tmp_mechanical_angle_velocity;
    // filter_data_after[filter_data_index] = FOC_Struct.rotate_speed;
    // filter_data_index++;
    // if (filter_data_index >= FILTER_DATA_NUM) {
    //     // 打印出存储的数据
    //     for (int i = 0; i < FILTER_DATA_NUM; i++) {
    //         printf("%d %f %f \r\n", i, filter_data[i], filter_data_after[i]);
    //     }
    //     filter_data_index = 0;
    // }

}

/*!
    \brief correct the mechanical angle zero deviation between the magnetic encoder and FOC.
*/
void encoder_zeroing(void) {
    /* delay to wait for the power supply voltage to be normal */
    delayms(500);

    /* set that there is only a magnetic field on the straight axis. */
    foc_set_phase_dutycycle(0, CALI_TORQUE, 0);
    delayms(300);
    machine_angle_offset = 0.0;
    log_report(TAG, "zeroing machine_angle_offset is initialized to 0.0");

    /* read the angle at this time as the offset angle */
    machine_angle_offset = getSensorAngle();
    log_report(TAG, "zeroing machine_angle_offset = %f", machine_angle_offset);

    /* zero the torque in all directions to release the motor */
    foc_set_phase_dutycycle(0, 0, 0);
    log_report(TAG, "zeroing finished");
    delayms(300);
}
