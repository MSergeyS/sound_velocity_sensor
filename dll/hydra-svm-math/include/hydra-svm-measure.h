/*
 * hydra-svm-measure.h
 *
 *  Created on: Nov 29, 2023
 *      Author: horev
 */

#ifndef HYDRA_SVM_MEASURE_H_
#define HYDRA_SVM_MEASURE_H_

/** @addtogroup cTODO hydra-svm-measure
  * @{
  */
#include "hydra-types.h"

/*============================ Section Exported Start ===========================*/
/**
 * @addtogroup cTODO hydra-svm-measure-exported
 * @{
 */

#define HYDRA_SVM_ADC_BUFF_SIZE 1945  // размер приёмного буфера для каждого АЦП (всего 3 АЦП, два замера) 1801 похоже достаточно
#define HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE (HYDRA_SVM_ADC_BUFF_SIZE * 3)  // размер собранного из трех АЦП массива
#define HYDRA_SVM_ADC_WORK_BUFF_SIZE (800*2 + 150) // размер используемых данных собранного массива vTODO


typedef struct
{
     struct
     {
       uint16_t adc_1_chanel_value[HYDRA_SVM_ADC_BUFF_SIZE];
       uint16_t adc_2_chanel_value[HYDRA_SVM_ADC_BUFF_SIZE];
       uint16_t adc_3_chanel_value[HYDRA_SVM_ADC_BUFF_SIZE];
     }sTempData;

     uint16_t svm_adc_data[HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE];  // итоговый массив измерения, собранный из 3 АЦП
     uint16_t adc_tripleconverted[HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE];  // собранный из стробов массив

     int16_t  adc_workconverted[HYDRA_SVM_ADC_WORK_BUFF_SIZE];  // рабочий массив с измерениями
     uint8_t id_adc_data;   // идентификатор замера (циклический счётчик)
}SVM_Measure_prm_t;

//uint8_t hydra_svm_measure_normalization(int16_t* dest, const uint16_t* source,  const uint16_t mas_size);

/**
  * @}
  */
/*============================= Section External End ============================*/


/**
  * @}
  */
#endif /* HYDRA_SVM_MEASURE_H_ */
/****************************END OF FILE****************************/
