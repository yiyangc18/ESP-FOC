/**************************************************************************//**
  \file     encoder.h
  \brief    this file contains the code implementation of encoder, including
            zeroing, angle calculation, speed calculation.
  \author   Lao·Zhu, Modified by Chery
  \date     October 2023
 ******************************************************************************/

#ifndef ESPFOC_COMP_ENCODER_H_
#define ESPFOC_COMP_ENCODER_H_

#include "foc.h"
#include "foc_config.h"
#include "filter.h"

extern volatile float machine_angle_offset;
void encoder_zeroing(void);
float encoder_get_mechanical_angle(void);
float encoder_get_electronic_angle(void);
void encoder_update_speed(void);

#endif //ESPFOC_COMP_ENCODER_H_
