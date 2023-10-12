/**************************************************************************//**
  \file     pid.c
  \brief    this is the header file of pid.c, which defines the structure
            of PID algorithm and closed-loop state macro.
  \author   Lao·Zhu & Chery
  \version  V1.0.2
  \date     October 2023
 ******************************************************************************/

#include "pid.h"
#include "fast_math.h"
#include "foc_config.h"

const static char *TAG = "foc_pid";

/*!
    \brief flag variable for PID parameter availability
*/
unsigned char pid_parameter_available_flag = 1;
/*!
    \brief flag variable of PID closed loop mode
*/
volatile unsigned char pid_control_mode_flag = 0;
/*!
    \brief algorithm handler of PID speed loop
*/
volatile PID_Structure_t speed_pid_handler;
/*!
    \brief algorithm handler of PID angle loop
*/
volatile PID_Structure_t angle_pid_handler;

/*!
    \brief configure pid loop parameters
*/
void pid_config(unsigned char mode) {
    /* clear the value of the PID handler */
    memset((void *) &speed_pid_handler, 0x00, sizeof(PID_Structure_t));
    memset((void *) &angle_pid_handler, 0x00, sizeof(PID_Structure_t));

    /* update the PID closed loop flag byte */
    pid_control_mode_flag = mode;

    /* set maximum and minimum output torque */
    speed_pid_handler.maximum = SPEED_CTR_MAXOPUT ;
    speed_pid_handler.minimum = SPEED_CTR_MINOPUT;
    log_report(TAG, "speed_pid_handler.maximum = %f", speed_pid_handler.maximum);
    log_report(TAG, "speed_pid_handler.minimum = %f", speed_pid_handler.minimum);

    speed_pid_handler.kp = SPEED_CTR_KP;
    speed_pid_handler.ki = SPEED_CTR_KI;
    speed_pid_handler.kd = SPEED_CTR_KD;
    speed_pid_handler.sum_maximum = SPEED_SUM_MAXIMUM;
    log_report(TAG, "speed_pid kp = %f ki = %f kd = %f", speed_pid_handler.kp, speed_pid_handler.ki, speed_pid_handler.kd);

    /* set maximum and minimum output angle */
    angle_pid_handler.maximum = ANGLE_CTR_MAXOPUT;
    angle_pid_handler.minimum = ANGLE_CTR_MINOPUT;
    log_report(TAG, "angle_pid_handler.maximum = %f", angle_pid_handler.maximum);
    log_report(TAG, "angle_pid_handler.minimum = %f", angle_pid_handler.minimum);

    angle_pid_handler.kp = ANGLE_CTR_KP;
    angle_pid_handler.ki = ANGLE_CTR_KI;
    angle_pid_handler.kd = ANGLE_CTR_KD;
    angle_pid_handler.sum_maximum = ANGLE_SUM_MAXIMUM;
    log_report(TAG, "angle_pid kp = %f ki = %f kd = %f", angle_pid_handler.kp, angle_pid_handler.ki, angle_pid_handler.kd);

}

/*!
    \brief     calculate result using sampling value
    \param[in] pid_handler: PID data handler
    \param[in] collect: sampled data
    \retval    calculated output value of PID controller
*/
float pid_calculate_result(PID_Structure_t *pid_handler, float collect) {
    /* calculate PID error value */
    float current_result, error = pid_handler->expect - collect;
    log_report(TAG, "pid_calculate: error = %f", error);

    /* calculate the integral and realize anti integral saturation */
    pid_handler->summary = pid_handler->summary + error;
    pid_handler->summary = fast_constrain(pid_handler->summary, -pid_handler->sum_maximum, pid_handler->sum_maximum);
    log_report(TAG, "pid_handler->summary = %f", pid_handler->summary);

    /* calculate PID output value */
    current_result = pid_handler->kp * error + pid_handler->ki * pid_handler->summary
        + pid_handler->kd * (error - pid_handler->last_error);

    /* Update last time error of PID to calculate the differential term */
    pid_handler->last_error = error;

    /* Implementation of output limiting algorithm */
    current_result = fast_constrain(current_result, pid_handler->minimum, pid_handler->maximum);
    return current_result;
}

