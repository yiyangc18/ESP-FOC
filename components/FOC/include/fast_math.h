/**************************************************************************//**
  \file     fast_math.c
  \brief    fast math functions, including sine, cosine, _normalizeAngle
  \author   Lao·Zhu & Chery
  \date     October 2023
 ******************************************************************************/

#ifndef ESPFOC_COMP_FAST_MATH_H_
#define ESPFOC_COMP_FAST_MATH_H_

/*!
    \brief     fast clipping algorithm
    \param[in] x: number of to be clipped
    \param[in] low: minimum number
    \param[in] high: maximum number
    \retval    input value after clipping
*/
#define fast_constrain(x, low, high)    ((x)<(low)?(low):((x) >(high)?(high):(x)))

/*!
    \brief     fast calculation of cosine
    \param[in] x: angle to be calculated
    \retval    cosine value of angle theta
*/
#define fast_cos(x)                     fast_sin(1.5707963f - x);

float fast_sin(float theta);

float _normalizeAngle(float angle);

/*!
    \brief     convert floating point numbers to int32 type data
    \param[in] data0: floating point type data to be converted
    \retval    converted int32 type data
*/
unsigned int float_to_int32(float data0) ;

/*!
    \brief     convert int32 to floating point numbers type data
    \param[in] data0: int32 type data to be converted
    \retval    converted floating point type data
*/
float int32_to_float(unsigned int data0) ;


#endif //ESPFOC_COMP_FAST_MATH_H_
