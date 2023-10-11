/**************************************************************************//**
  \file     encoder.h
  \brief    this is the header file of encoder.c.
  \author   Lao·Zhu, Modified by Chery
  \date     October 2023
  \note     Use function pointers, specific encoder support is implemented externally.
 ******************************************************************************/

#ifndef MINIFOC_ALGORITHM_ENCODER_H_
#define MINIFOC_ALGORITHM_ENCODER_H_

#include "foc.h"
#include "foc_config.h"
#include "filter.h"

extern volatile float machine_angle_offset;
void encoder_zeroing(void);
float encoder_get_mechanical_angle(void);
float encoder_get_electronic_angle(void);
void encoder_update_speed(void);

#endif //MINIFOC_ALGORITHM_ENCODER_H_
