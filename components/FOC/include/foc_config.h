/**************************************************************************//**
  \file     config.h
  \brief    used to place important parameter configurations for users
  \author   Lao·Zhu
  \version  V1.0.3
  \date     1. August 2023
 ******************************************************************************/

#ifndef MINIFOC__CONFIG_H_
#define MINIFOC__CONFIG_H_

#include <string.h>
#include "esp_hw_link.h"


/*! \brief bus voltage is 5V */
#define VBUS            5.0f
/*! \brief set BLDC polar pairs to 7 */
#define POLAR_PAIRS     7
/*! \brief SC60228 resolution is 2^14 = 16384 */
#define ENCODER_RESO    16384
/*! \brief set calibrate torque to 0.5 */
#define CALI_TORQUE     1.5f
/*! \brief  using space vector pulse width modulator */
#define USE_SVPWM       1

/*! \brief set PWM frequency to 20kHz */
#define PWM_FREQUENCY   20
/*! \brief set FOC loop frequency to 8kHz */
#define FOC_LOOP_FREQ  8000
/*! \brief set speed/angle PID loop frequency to 100Hz */
#define PID_LOOP_FREQ 200
/*! \brief set speed update frequency to 200Hz */
#define SPEED_UP_FREQ   400

/*! \brief TIMER2 preemption priority set to 2 */
#define FOC_LOOP_PRIORITY   7
/*! \brief TIMER13 preemption priority set to 3 */
#define PID_LOOP_PRIORITY  6

#define TWO_PI 6.283185307179586476925286766559
#define _PI 3.14159265359f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _SQRT3 1.73205080757f
#define _SQRT3_2 0.86602540378f


#endif //MINIFOC__CONFIG_H_
