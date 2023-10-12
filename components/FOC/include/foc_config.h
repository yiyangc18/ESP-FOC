/**************************************************************************//**
  \file     foc_config.h
  \brief    this file contains the basic configuration of FOC algorithm.
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#ifndef ESPFOC_COMP_CONFIG_H_
#define ESPFOC_COMP_CONFIG_H_

#include <string.h>
#include "esp_hw_link.h"


/*! \brief bus voltage is 5V */
#define VBUS            5.0f
/*! \brief set BLDC polar pairs to 7 */
#define POLAR_PAIRS     7
/*! \brief set calibrate torque to 0.5 */
#define CALI_TORQUE     1.5f
/*! \brief  using space vector pulse width modulator */
#define USE_SVPWM       1

/*! \brief set PWM frequency to 20kHz */
#define PWM_FREQUENCY   20
/*! \brief set FOC loop frequency to 10kHz */
#define FOC_LOOP_FREQ  10000
/*! \brief set speed/angle PID loop frequency to 200Hz */
#define PID_LOOP_FREQ 200
/*! \brief set speed update frequency to 400Hz */
#define SPEED_UP_FREQ   400


#define TWO_PI 6.283185307179586476925286766559
#define _PI 3.14159265359f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _SQRT3 1.73205080757f
#define _SQRT3_2 0.86602540378f


#endif //ESPFOC_COMP_CONFIG_H_
