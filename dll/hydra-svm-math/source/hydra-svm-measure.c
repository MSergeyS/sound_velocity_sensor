/*
 * hydra-svm-measure.c
 *
 *  Created on: Nov 29, 2023
 *      Author: horev
 */

/** @defgroup cTODO hydra-svm-measure                Модуль <cTODO hydra-svm-measure>
 *
 *  @verbatim
 * ==============================================================================
 *                     ##### Как использовать данный модуль #####
 *  =============================================================================
 * cTODO
 * @endverbatim

 * @brief cTODO
 *                
 ******************************************************************************
 * @attention Для работы модуля должны быть реализованы функции внешнего 
 * интерфеса модуля
 ******************************************************************************
 */

/** @defgroup   cTODO hydra-svm-measure-private  Функции и данные для внутреннего использования
 *
 *  @brief      Данный раздел содержит описание функций,типов, констант, макросов,
 *  переменных и используемых внутри модуля
 *  @verbatim
 *  ==============================================================================
 *                       ##### Section Private description  #####
 *  ==============================================================================
 *  Sequence of data arrangement within a section:
 *
 *  Private define ------------------------------------------------------------
 *  Private macros ------------------------------------------------------------
 *  Private constants ---------------------------------------------------------
 *  Private typedef -----------------------------------------------------------
 *  Private variables ---------------------------------------------------------
 *  Private function prototypes -----------------------------------------------
 *  Private function realisation ----------------------------------------------
 *
 *  Section Private boundaries:
 *
 *  ============================ Section Private Start ===========================
 *
 *                        ...Private Section realisation...
 *
 *  ============================= Section Private End ============================
 *  @endverbatim
 */

/** @defgroup   cTODO hydra-svm-measure-exported  Функции и данные для внешнего использования
 *
 *  @brief      Данный раздел содержит описание функций,типов и данных доступных
 *  для использования за пределами модуля
 *  @verbatim
 *  ==============================================================================
 *                       ##### Section Exported Labels #####
 *  ==============================================================================
 *  Sequence of data arrangement within a section:
 *
 *  Exported define -----------------------------------------------------------
 *  Exported macros -----------------------------------------------------------
 * Exported constants --------------------------------------------------------
 *  Exported typedef ----------------------------------------------------------
 *  Exported variables --------------------------------------------------------
 *  Exported function realisation ---------------------------------------------
 *  Exported function prototypes ----------------------------------------------
 *
 *  Section Private boundaries:
 *
 *  ============================ Section Exported Start ===========================
 *
 *                       ...Exported Section realisation...
 *
 *  ============================= Section Exported End ============================
 *  @endverbatim
 */

/** @defgroup   cTODO hydra-svm-measure-external      Функции внешнего интерфеса
 *
 *  @brief      В этом разделе представлены функции внешнего интерфейса. Данные
 *  функции должны быть реализованы на стороне пользователя в соотвествии с
 *  указанным описанием.
 *  @verbatim
 *  ==============================================================================
 *                       ##### Section External Labels #####
 *  ==============================================================================
 *  Sequence of data arrangement within a section:
 *
 *  External function prototypes ----------------------------------------------
 *
 *  Section Private boundaries:
 *
 *  ============================ Section External Start ===========================
 *
 *                        ...External Section realisation...
 *
 *  ============================= Section External End ============================
 *  @endverbatim
 */
/**
 * @addtogroup cTODO hydra-svm-measure
 * @{
 */

//#include "hydra-complexcfg.h"
#include "hydra-svm-measure.h"
#include "hydra-svm-math.h"
#include "hydra-svm.h"
#include "hydra-svm-math.h"
#include <string.h>

/*============================ Section Private Start ============================*/
/**
 * @addtogroup  cTODO hydra-svm-measure-private
 * @{
 */
//uint8_t svm_adc_tripleconverted[HYDRA_SVM_ADC_IZM][HYDRA_SVM_ADC_OUTPUT_BUFF_SIZE];

SVM_Measure_prm_t svm_measure_data;
SVM_Measure_prm_t* p_svm_measure_data = &svm_measure_data;


/**
 * @brief функция ищет среднее (полку) значения оцифрованных данных
 *              ВАЖНО: первые okno ячеек выходного массива равны среднему по окну.
 *
 * @param p_tripleconverted     - входной массив оцифрованных данных (приведённый к 12 МГц)
 * @param p_sredneeconverted    - выходной массив усреднённый по окну
 * @param mas_size              - размер массива данных
 * @return                      - TRUE - удачное создание массива
 *                              - FALSE - ошибка
 */

uint8_t
hydra_svm_measure_normalization(int16_t* p_normalizedconverted, const uint16_t* p_tripleconverted,  const uint16_t mas_size)
{
  float srednee_okno1, srednee_okno2;
  const uint16_t okno = 4; // размер окна (количество тактов тактирования АЦП на один период частоты излучающего сигнала)
  int16_t imas, iokno;
  int16_t normalized_fullsize[HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE]={0};


  if(mas_size>HYDRA_SVM_ADC_COMPLITE_BUFF_SIZE)
    return FALSE;
  if(mas_size<HYDRA_SVM_ADC_WORK_BUFF_SIZE)
    return FALSE;
  //memset(p_zeroconverted,0,mas_size*sizeof(uint16_t));
  if(mas_size<okno*2)
    return FALSE;

  // считаем среднее окном по всему массиву
  for(imas = okno*2; imas < mas_size-1; imas+=2)
  {
    srednee_okno1 = 0.;
    srednee_okno2 = 0.;
    for(iokno=0; iokno < okno; ++iokno)
    {
      srednee_okno1 = srednee_okno1 + (float)p_tripleconverted[iokno*2 + (imas-okno)];
      srednee_okno2 = srednee_okno2 + (float)p_tripleconverted[iokno*2 + (imas+1-okno)];
    }
    srednee_okno1 = srednee_okno1/(float)okno;
    srednee_okno2 = srednee_okno2/(float)okno;

    normalized_fullsize[imas]= ((int16_t)p_tripleconverted[imas]-(int16_t)(srednee_okno1));
    normalized_fullsize[imas+1]= ((int16_t)p_tripleconverted[imas+1]-(int16_t)(srednee_okno2));
  }
  memcpy(&p_normalizedconverted[150],normalized_fullsize,(HYDRA_SVM_ADC_WORK_BUFF_SIZE-150)*sizeof(int16_t)); // отрезаем только "нужный" отрезок массива
  return TRUE; // возвращаем среднее по всему массиву
}

/**
  * @}
  */
/*============================= Section Exported End ============================*/


/**
  * @}
  */
/****************************END OF FILE****************************/
