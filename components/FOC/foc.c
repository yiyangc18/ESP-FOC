/**************************************************************************//**
  \file     foc.c
  \brief    this file contains the code implementation of FOC algorithm.
  \author   Chery
  \date     October 2023
 ******************************************************************************/

#include "foc.h"

static const char* TAG = "foc";

/*!
    \brief FOC handler
*/
volatile FOC_Structure_t FOC_Struct;

/*!
    \brief flag variable for FOC parameter availability
*/
unsigned char foc_parameter_available_flag = 1;

/*!
    \brief automatic phase sequence detection and correction
*/
void foc_calibrate_phase(void) {
    float last_angle = 0;
    int positive_counter = 0;
    float angle = 0;

    /* set that there is only a magnetic field on the straight axis */
    foc_set_phase_dutycycle(0, CALI_TORQUE, 0);
    log_report(TAG, "phase sequence calibration is starting");
    delayms(50);

    for (unsigned char counter = 0; counter < 100; ++counter) {
        /* measure current motor angle */
        float current_angle = encoder_get_mechanical_angle();

        /* judge whether the current angle increases */
        if (current_angle > last_angle)
            positive_counter++;

        /* update last angle as current angle */
        last_angle = current_angle;
        // log_report(TAG, "psc, current_angle:%f, last_angle:%f",current_angle, last_angle);

        /* set that there is only a magnetic field on the straight axis */
        foc_set_phase_dutycycle(angle, CALI_TORQUE, 0);
        angle += 0.1f;
        delayms(20);
    }

    log_report(TAG, "angle increases, positive_counter:%d",positive_counter);

    for (unsigned char counter = 0; counter < 100; ++counter) {
        /* measure current motor angle */
        float current_angle = encoder_get_mechanical_angle();

        /* judge whether the current angle decreases */
        if (current_angle < last_angle)
            positive_counter++;

        /* update last angle as current angle */
        last_angle = current_angle;
        // log_report(TAG, "psc, current_angle:%f, last_angle:%f",current_angle, last_angle);

        /* set that there is only a magnetic field on the straight axis */
        foc_set_phase_dutycycle(angle, CALI_TORQUE, 0);
        angle -= 0.1f;
        delayms(20);
    }

    log_report(TAG, "angle decreases, positive_counter:%d",positive_counter);

    /* zero the torque in all directions to release the motor */
    foc_set_phase_dutycycle(0, 0, 0);
    delayms(300);

    if (positive_counter >= 150)
        {
            phase_sequence = 0;
            log_report(TAG, "phase sequence calibration succeeded, phase sequence is u v w");
        }
    else if (positive_counter < 50)
        {
            phase_sequence = 1;
            log_report(TAG, "phase sequence calibration succeeded, phase sequence is u w v");
        }
    else
        log_report(TAG, "phase sequence calibration failed, please try again");
}

/*!
    \brief      calculate the corresponding three-phase PWM duty cycle under the current electrical angle
    \param[in]  elect_angle: electrical angle of motor rotor, in radian system
    \param[in]  d: expected field strength in the direct axis direction of the motor
    \param[in]  q: expected field strength in the quadrature axis direction of the motor
*/
void foc_set_phase_dutycycle(float elect_angle, float Ud, float Uq) {

    float Ua,Ub,Uc;

#if USE_SVPWM == 0 //FOCModulationType::SinePWM

    float center;
    float Ualpha, Ubeta;
    /* fast calculation of cosine and sine value of electric angle */
    float _ca = fast_cos(elect_angle);
    float _sa = fast_sin(elect_angle);

    // Sinusoidal PWM modulation
    // Inverse Park + Clarke transformation

    // Inverse park transform
    Ualpha =  _ca * Ud - _sa * Uq;  // -sin(angle) * Uq;
    Ubeta =  _sa * Ud + _ca * Uq;    //  cos(angle) * Uq;

    // center = modulation_centered ? (VBUS)/2 : Uq;
    center = 0.5f * (float)VBUS;
    // Clarke transform
    Ua = Ualpha + center;
    Ub = -0.5f * Ualpha  + _SQRT3_2 * Ubeta + center;
    Uc = -0.5f * Ualpha - _SQRT3_2 * Ubeta + center;

    Ua = Ua / VBUS;
    Ub = Ub / VBUS;
    Uc = Uc / VBUS;

#else  //FOCModulationType::SpaceVectorPWM 

    // Nice video explaining the SpaceVectorModulation (SVPWM) algorithm
    // https://www.youtube.com/watch?v=QMSWUMEAejg

    // the algorithm goes
    // 1) Ualpha, Ubeta
    // 2) Uout = sqrt(Ualpha^2 + Ubeta^2)
    // 3) angle_el = atan2(Ubeta, Ualpha)
    //
    // equivalent to 2)  because the magnitude does not change is:
    // Uout = sqrt(Ud^2 + Uq^2)
    // equivalent to 3) is
    // angle_el = angle_el + atan2(Uq,Ud)


    // 如果负电压的变化与相位相反
    // 角度+180度
    if(Uq < 0) elect_angle += _PI;
        Uq = abs(Uq);


    float Uout,angle_el;
    int sector;
    // a bit of optitmisation
    if(Ud>0.000001f){ // only if Ud and Uq set
        // _sqrt is an approx of sqrt (3-4% error)
        Uout = sqrt(Ud*Ud + Uq*Uq) / VBUS;
        // angle normalisation in between 0 and 2pi
        // only necessary if using _sin and _cos - approximation functions
        angle_el = _normalizeAngle(elect_angle + atan2(Uq, Ud));
    }else{// only Uq available - no need for atan2 and sqrt
        Uout = Uq / VBUS;
        // angle normalisation in between 0 and 2pi
        // only necessary if using _sin and _cos - approximation functions
        angle_el = _normalizeAngle(elect_angle + _PI_2);
    }
    // find the sector we are in currently
    sector = floor(angle_el / _PI_3) + 1; // every sector is 60 degrees
    // calculate the duty cycles
    float T1 = _SQRT3*fast_sin(sector*_PI_3 - angle_el) * Uout; 
    float T2 = _SQRT3*fast_sin(angle_el - (sector-1.0f)*_PI_3) * Uout;
    // two versions possible
    float T0 = 0; // pulled to 0 - better for low power supply voltage 
    // float T0 = 1 - T1 - T2; // 以电压电源为中心/2

    // calculate the duty cycles(times)
    float Ta,Tb,Tc;
    switch(sector){
    case 1:
        Ta = T1 + T2 + T0/2;
        Tb = T2 + T0/2;
        Tc = T0/2;
        break;
    case 2:
        Ta = T1 +  T0/2;
        Tb = T1 + T2 + T0/2;
        Tc = T0/2;
        break;
    case 3:
        Ta = T0/2;
        Tb = T1 + T2 + T0/2;
        Tc = T2 + T0/2;
        break;
    case 4:
        Ta = T0/2;
        Tb = T1+ T0/2;
        Tc = T1 + T2 + T0/2;
        break;
    case 5:
        Ta = T2 + T0/2;
        Tb = T0/2;
        Tc = T1 + T2 + T0/2;
        break;
    case 6:
        Ta = T1 + T2 + T0/2;
        Tb = T0/2;
        Tc = T1 + T0/2;
        break;
    default:
        // possible error state
        Ta = 0;
        Tb = 0;
        Tc = 0;
    }

    Ua = Ta;
    Ub = Tb;
    Uc = Tc;

#endif

    // log_report(TAG, "FOC algorithm input: elect_angle:%f, Ud:%f, Uq:%f",elect_angle,Ud,Uq);
    // log_report(TAG, "FOC algorithm output: Ua:%f, Ub:%f, Uc:%f",Ua,Ub,Uc);

    update_pwm_dutycycle(Ua,Ub,Uc);

}

void foc_loop(void){

    /* obtain the electric angle at the current time */
    float angle = encoder_get_electronic_angle();

    /* set the PWM duty cycle according to the electric angle and Id=0 principle */
    foc_set_phase_dutycycle(angle, 0, FOC_Struct.current_q);

}


void controller_loop(void){

    /* judge whether angle closed-loop control is required */
    if (pid_control_mode_flag == ANGLE_LOOP_CONTROL){
        /* the calculated value of the angle loop is taken as the expected value of the speed loop */
        speed_pid_handler.expect =
            pid_calculate_result((PID_Structure_t *) &angle_pid_handler, FOC_Struct.mechanical_angle);
        
        FOC_Struct.current_q = pid_calculate_result((PID_Structure_t *) &speed_pid_handler, FOC_Struct.rotate_speed);
    }
    if(pid_control_mode_flag == SPEED_LOOP_CONTROL){
        /* calculate the speed loop PID and obtain the calculated value */
        FOC_Struct.current_q = pid_calculate_result((PID_Structure_t *) &speed_pid_handler, FOC_Struct.rotate_speed);

        // log_report(TAG, "controller_loop: expect = %f, ratate_speed = %f, current_q = %f", speed_pid_handler.expect, FOC_Struct.rotate_speed,FOC_Struct.current_q);
    }

}

