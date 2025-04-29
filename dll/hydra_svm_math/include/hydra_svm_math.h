#pragma once

// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the HYDRASVMMATH_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// HYDRASVMMATH_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifndef HYDRA_SVM_MATH_H_
#define HYDRA_SVM_MATH_H_

#define HYDRA_SVM_MATH_EXPORTS

#if defined (_WIN32)
#if defined (HYDRA_SVM_MATH_EXPORTS)
#define HYDRA_SVM_MATH_API __declspec(dllexport)
#else
#define HYDRA_SVM_MATH_API __declspec(dllimport)
#endif
#else
#define HYDRA_SVM_MATH_API
#endif
#define HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE 2222
#define HYDRA_SVM_ADC_WORK_BUFF_SIZE 1600

/** @addtogroup cTODO hydra-svm-math
  * @{
  */
#include "types.h"
#include "defines.h"

extern "C" HYDRA_SVM_MATH_API
Hydra_out_xcorr_t hydra_svm_vkf_my(int16_t* p_workconverted, uint16_t otr1, uint16_t otr2);

extern "C" HYDRA_SVM_MATH_API
float hydra_time_propagation_calculation(Hydra_out_xcorr_t xcorr);

extern "C" HYDRA_SVM_MATH_API
float hydra_sound_velocity_estimation(Hydra_out_xcorr_t xcorr_1, float base_1,
                          Hydra_out_xcorr_t xcorr_2, float base_2);

extern "C" HYDRA_SVM_MATH_API
float hydra_svm(int16_t* p_workconverted, float distance_1, float distance_2,
                          uint16_t* p_otr1, uint16_t* p_otr2,
                          Hydra_out_xcorr_t* xcorr);

extern "C" HYDRA_SVM_MATH_API
uint16_t hydra_xcorr_real_v0(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, 
                          uint16_t window_size);

extern "C" HYDRA_SVM_MATH_API
int16_t hydra_xcorr_real_v1(const Hydra_Svm_Complex32_t* s1, const Hydra_Svm_Complex32_t* s2,
                          const float trh_1, const float trh_2, const uint16_t window_size);

extern "C" HYDRA_SVM_MATH_API
uint16_t hydra_xcorr_real_abs(int32_t* s1_abs, int32_t* s2_abs,
                          uint16_t window_size);

extern "C" HYDRA_SVM_MATH_API
bool hydra_norm_and_trh(int16_t s_abs, int16_t trh, int16_t* min_delta_trh);

extern "C" HYDRA_SVM_MATH_API
uint8_t hydra_svm_measure_normalization(const uint16_t* p_tripleconverted,
                          int16_t* p_normalizedconverted, const uint16_t mas_size);


#endif // HYDRA_SVM_MATH_H_