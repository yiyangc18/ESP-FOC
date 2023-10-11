/**************************************************************************//**
  \file     pid.h
  \brief    this is the header file of pid.c, which defines the structure
            of PID algorithm and closed-loop state macro.
  \author   Lao·Zhu
  \version  V1.0.1
  \date     9. October 2021
 ******************************************************************************/

#ifndef MINIFOC_ALGORITHM_PID_H_
#define MINIFOC_ALGORITHM_PID_H_

/*! \brief torque loop control mode */
#define TORQUE_LOOP_CONTROL     1
/*! \brief speed loop control mode */
#define SPEED_LOOP_CONTROL      2
/*! \brief angle loop control mode */
#define ANGLE_LOOP_CONTROL      3

#define SPEED_CTR_MAXOPUT        5.0f   //速度环的输出是Uq 最大电压为5V
#define SPEED_CTR_MINOPUT        -2.0f
#define SPEED_CTR_KP            0.9f
#define SPEED_CTR_KI            0.15f
#define SPEED_CTR_KD            0.02f
#define SPEED_SUM_MINIMUM       0.0f
#define SPEED_SUM_MAXIMUM       50.0f

#define ANGLE_CTR_MAXOPUT        100.0f
#define ANGLE_CTR_MINOPUT        -100.0f
#define ANGLE_CTR_KP            0.6f
#define ANGLE_CTR_KI            0.08f
#define ANGLE_CTR_KD            0.01f
#define ANGLE_SUM_MINIMUM       0.0f
#define ANGLE_SUM_MAXIMUM       100.0f

/*!
  \struct PID_Structure_t
  \brief  structure of PID algorithm
 */
typedef struct {
    float kp;           ///< proportional term coefficient in PID
    float ki;           ///< integral term coefficient in PID
    float kd;           ///< differential term coefficient in PID
    float summary;      ///< value of integral term in PID
    float expect;       ///< user expectations in PID
    float maximum;      ///< maximum output in PID
    float minimum;      ///< minimum output in PID
    float sum_maximum;  ///< maximum of anti saturation integral in PID
    float last_error;   ///< error value of previous calculation in PID
} PID_Structure_t;

extern unsigned char pid_parameter_available_flag;
extern volatile unsigned char pid_control_mode_flag;
extern volatile PID_Structure_t speed_pid_handler;
extern volatile PID_Structure_t angle_pid_handler;
void pid_config(unsigned char mode);
float pid_calculate_result(PID_Structure_t *pid_handler, float collect);

#endif //MINIFOC_ALGORITHM_PID_H_
