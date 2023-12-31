/**************************************************************************//**
  \file     foc.h
  \brief    this file contains the code implementation of FOC algorithm.
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#ifndef ESPFOC_COMP_FOC_H_
#define ESPFOC_COMP_FOC_H_

#include "pid.h"
#include "encoder.h"
#include "filter.h"
#include "foc_config.h"
#include "fast_math.h"
#include "encoder.h"


/*!
  \struct FOC_Structure_t
  \brief  structure of FOC algorithm
 */
typedef struct {
    float mechanical_angle;     ///< mechanical angle read form encoder
    float rotate_speed;         ///< motor rotate speed calculate from timer
    float current_q;          ///< user expect value of Iq
} FOC_Structure_t;

extern unsigned char foc_parameter_available_flag;
extern volatile FOC_Structure_t FOC_Struct;
void foc_set_phase_dutycycle(float elect_angle, float d, float q);
void foc_calibrate_phase(void);

void foc_loop(void);

void controller_loop(void);

#endif //ESPFOC_COMP_FOC_H_
