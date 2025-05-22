/**
 ******************************************************************************
 * @file         hydra-svm-math.h
 * @author       komelyagin
 * @date         22 нояб. 2023 г.
 *
 * @brief
 * cTODO
 *                
 ******************************************************************************
 * @attention 
 * cTODO
 *
 ******************************************************************************
 */

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

/** @addtogroup cTODO hydra-svm-math
  * @{
  */
#include "hydra-types.h"
//#include "hydra-svm-measure.h"


//#define N_WINDOW_SIZE 21
#define CALIB_MAS 100     // размер массива для юстировки
// AVERAGE_MASS 31 для 2Гц, 65 для 1 Гц
#define AVERAGE_MASS 31

/*============================ Section Exported Start ===========================*/
/**
 * @addtogroup cTODO hydra-svm-math-exported
 * @{
 */
typedef struct  Hydra_Svm_Complex16_t
{
    int16_t Re;
    int16_t Im;
}Hydra_Svm_Complex16_t;

typedef struct  Hydra_Svm_Complex32_t
{
    int32_t Re;
    int32_t Im;
}Hydra_Svm_Complex32_t;

typedef struct  Hydra_Svm_Complex64_t
{
    int64_t Re;
    int64_t Im;
}Hydra_Svm_Complex64_t;

typedef struct
{
    uint16_t index_time_xcorr;
    uint16_t index_time_propagation;
    float    phase;
    float    abs;
}Hydra_out_xcorr_t;

typedef struct
{
  float         time1;          /* время измеренное до первого отражения, с */
  float         time2;          /* время измеренное до второго отражения, с */
  double        tost;           /* время (разница измеренных времен), с */
  float         L2;             /* расстояние между мишенями (загружается из паспотра при инициализации), мм */
  float         temp;           /* температура воды, градусы */
 // float         freq;           /* частота излучения (загружается из паспотра при инициализации), Гц */
  float         Vcurrent;       /* текущее значение измеренной (посчитанной) скорости, м/с */
  float         Vaverage;       /* усреднённое значение скорости звука, м/с */
  float         Vlastgood;      /* последнее хорошее значение скорости звука, м/с */
  uint16_t      otr1;           /* индекс отражения от первой мишени */
  uint16_t      otr2;           /* индекс отражения от второй мишени */
  int           nDataCount;     /* количество уже измеренных раз (для фильтра) */
  uint8_t       good_measure;   /* признак достоверности */
  uint8_t       water_exist;    /* признак наличия воды */
} hydra_svm_work_data_t;
//extern hydra_svm_work_data_t hydra_svm_work_data;

typedef struct
{
  uint16_t vkf_i_diff;
  uint16_t vkf_i_max_amp;
  int32_t  i32_1;
  uint16_t ui16_goodmeasure;
  int16_t  ui16_1;
  uint16_t otr1;
  uint16_t otr2;

  float         vcurrent;
  //float faza;
}hydra_svm_debug_data_t;

typedef struct
{
  float c; // скорость
  float t; // время между отражениями
  float t1;// время до первого отражения
}SpeedCalibrationMas_t;


//void hydra_svm_measure(uint8_t strob);

__declspec(dllexport)
uint16_t hydra_svm_speed_calibration(float* speed );

__declspec(dllexport)
float hydra_svm_vaverage_calc(float* work_mass, uint16_t work_mass_size);

//float hydra_sound_velocity_estimation(Hydra_out_xcorr_t* xcorr_1,
                          // float base_1,  Hydra_out_xcorr_t* xcorr_2, float base_2);

__declspec(dllexport)
void hydra_svm_vkf_my(int16_t* p_workconverted,  uint16_t otr1, 
                      uint16_t otr2, Hydra_out_xcorr_t* result,
                      Hydra_Svm_Complex32_t* data_complex);

__declspec(dllexport)
uint8_t hydra_svm_calc_v(int16_t* p_workconverted, hydra_svm_work_data_t* p_svm_work_data, Hydra_out_xcorr_t* xcorr_1);

__declspec(dllexport)
int16_t hydra_xcorr_real_v1(const Hydra_Svm_Complex32_t* s1,
                            const Hydra_Svm_Complex32_t* s2,
                            const float trh_1, const float trh_2, const uint16_t window_size);

__declspec(dllexport)
uint8_t hydra_svm_water_exist(int16_t* work_mass, const uint16_t work_mass_size);

__declspec(dllexport)
uint16_t hydra_svm_windows_finder(int16_t* p_workconverted,
                                  const uint16_t work_converted_size,
                                  uint16_t *p_otr1, uint16_t *p_otr2);

__declspec(dllexport)
uint8_t hydra_norm_and_trh(int16_t s_abs, int16_t trh, int16_t* min_delta_trh);

__declspec(dllexport)
float hydra_svm_lin_interp(float x1, float y1, float x2, float y2, float x);

__declspec(dllexport)
hydra_svm_work_data_t* hydra_svm_math_get_addr_hydra_svm_work_data(void);

__declspec(dllexport)
hydra_svm_debug_data_t* hydra_svm_math_get_addr_hydra_svm_debug_data(void);

__declspec(dllexport)
float hydra_time_propagation_calculation(Hydra_out_xcorr_t* xcorr);
     
/**
 * @}
 */
/*============================= Section Exported End ============================*/

/*============================ Section External Start ===========================*/
/**
 * @addtogroup cTODO hydra-svm-math-external
 * @{
 */

/**
  * @}
  */
/*============================= Section External End ============================*/


/**
  * @}
  */




__declspec(dllexport)
uint8_t hydra_svm_measure_normalization(int16_t* dest, const uint16_t* source, const uint16_t mas_size);






#endif /* HYDRA_SVM_MATH_H_ */
/****************************END OF FILE****************************/