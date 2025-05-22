/**
 ******************************************************************************
 * @file         hydra-svm-math.c
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

 /** @defgroup cTODO hydra-svm-math                Модуль <cTODO hydra-svm-math>
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

  /** @defgroup   cTODO hydra-svm-math-private  Функции и данные для внутреннего использования
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


   /** @defgroup   cTODO hydra-svm-math-exported  Функции и данные для внешнего использования
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
    *  Exported constants --------------------------------------------------------
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

    /** @defgroup   cTODO hydra-svm-math-external      Функции внешнего интерфеса
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
      * @addtogroup cTODO hydra-svm-math
      * @{
      */

#include <stdio.h>
#include "pch.h"
#include <stdint.h>
#define _USE_MATH_DEFINES

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "hydra-svm-math.h"
#include "hydra-svm.h"
//#include "hydra-filter.h"
//#include "hydra-ep-txt.h"


      /*============================ Section Private Start ============================*/

      /**
       * @addtogroup  cTODO hydra-svm-math-private
       * @{
       */

       //#define ADCBIT 12  //vTODO количество бит АЦП
       //#define ADCHALF 2000 // vTODO половина диапазона АЦП

#define INTERF_CORR_MAS 11
static const uint16_t INDEX_NULL = 10000;
static const uint16_t RESERVE = 17;

//Hydra_Svm_Complex_t debug_vkf_max;
//uint16_t good_measures = 0; // количество хороших замеров подряд

uint16_t hydra_svm_speed_calibration_mas_index = 0;

const float hydra_svm_interf_corr[INTERF_CORR_MAS] = 
              { 2.4, 2.55, 2.7, 2.85, 3.0, 3.15, 3.3, 3.45, 3.6, 3.75, 3.9 };
              // [м/с] TODO это табличные константы корректировки скорости (1300-1700)!
float Vaverage_mass[AVERAGE_MASS] = { 0 };
SpeedCalibrationMas_t hydra_svm_speed_calibration_mas[CALIB_MAS];


/**
 * @brief sgn - функция определяем знак числа
 *
 * @param a
 * @return      1 - число больше 0
 *             -1 - число меньше 0
 *              0 - число 0
 */
int
sgn(double a) {
    if (a > 0)return 1;
    else if (a < 0)return -1;
    else return 0;
}

///**
// * @brief функция возвращает минимальное из двух целых чисел
// *
// * @param a сравниваемое целое число
// * @param b сравниваемое целое число
// * @return
// */
//int
//min(int a, int b) {
//    if (a < b)return a;
//    else return b;
//}


unsigned int
sqrt_newton(long unsigned int L)
{
    unsigned int rslt = (unsigned int)L;
    unsigned long int div = L;
    //if (L <= 0) return 0;
    while (1)
    {
        div = (L / div + div) / 2;
        if (rslt > div)
            rslt = (unsigned)div;
        else
            return rslt;
    }
}

/**
 * @brief hydra_svm_array_XXX_add_val() - функция добавляет новое значение в массив, выбрасывая самое старое значение
 *
 * @param p_arr         - указатель на массив
 * @param n             - размер массива
 * @param new_val       - новое значение
 */
void hydra_svm_array_i32_add_val(int32_t* p_arr, uint16_t n, int32_t new_val)
{
    uint16_t i;
    for (i = n - 1; i > 0; i--)
    {
        p_arr[i] = p_arr[i - 1];
    }
    p_arr[0] = new_val;
}

uint8_t hydra_svm_array_float_add_cval(float* p_arr, uint16_t n, float new_val)
{
    uint16_t i;
    for (i = n - 1; i > 0; i--)
    {
        p_arr[i] = p_arr[i - 1];
    }
    p_arr[0] = new_val;
    return (p_arr[n - 1] > 1300);
}


/**
 * @}
 */
 /*============================= Section Private End =============================*/

 /*============================ Section Exported Start ===========================*/
 /**
  * @addtogroup  cTODO hydra-svm-math-exported
  * @{
  */
hydra_svm_work_data_t hydra_svm_work_data;
hydra_svm_work_data_t* p_hydra_svm_work_data = &hydra_svm_work_data;

hydra_svm_debug_data_t hydra_svm_debug_data;
hydra_svm_debug_data_t* p_hydra_svm_debug_data = &hydra_svm_debug_data;

/**
 * @brief       Возвращает адрес буфера с рабочими параметрами изделия
 * Пример использования:
 *
 * hydra_svm_work_data_t* p_hydra_svm_work_data = hydra_svm_math_get_addr_hydra_svm_work_data();
 *
 * @return      Адрес буфера
 */
hydra_svm_work_data_t*
hydra_svm_math_get_addr_hydra_svm_work_data(void)
{
    return &hydra_svm_work_data;
}

hydra_svm_debug_data_t*
hydra_svm_math_get_addr_hydra_svm_debug_data(void)
{
    return &hydra_svm_debug_data;
}

Hydra_Svm_Complex32_t data_complex[HYDRA_SVM_ADC_WORK_BUFF_SIZE];

#define ADC_WIDTH 12

//void hydra_svm_calc_v(int16_t* p_workconverted,hydra_svm_work_data_t* p_svm_work_data);
static uint16_t index_xcorr_max = 0;


void hydra_svm_vkf_my(int16_t* p_workconverted, uint16_t otr1, uint16_t otr2,
    Hydra_out_xcorr_t* result)
{   
    //uint16_t signal_lenght = 30 * (uint16_t)(DATA_RATE / FREQUENCY_CENTRAL);

    //HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
    //uint16_t signal_lenght = p_svm_prm->sADC.imp_cnt * (uint16_t)(p_svm_prm->sADC.adc_freq/ p_svm_prm->sADC.gen_freq);
    uint16_t signal_lenght = 27 * (uint16_t)(4);


    uint16_t window_size = (signal_lenght + 57); // сигнал с хвостом
    uint16_t max_inx_window_1 = otr1 + window_size;
    bool otr_true = 1;
    uint16_t reserve = RESERVE; // 0
    if (otr1 == 0)
    {
        otr1 = 300;              // минимальный индекс начала первого окна
        otr2 = 730;              // минимальный индекс начала первого окна
        window_size = 433;       // максимальный размер окна
        max_inx_window_1 = 560;  // максимальный индекс первого окна
        otr_true = 0;
        reserve = 0;
    }

    uint16_t              data_size = 1320;
    //Hydra_Svm_Complex32_t data_complex[1320] = { 0 };

    Hydra_Svm_Complex32_t xcorr_max = { 0, 0 };
    int64_t               xcorr_max_abs = 0;
    int16_t               index_xcorr_max = 0;

    float    phase = 0.0;
    float    abss = 0.0;
    int64_t xcorr_current_abs = 0;
    Hydra_Svm_Complex32_t xcorr_current = { 0, 0 };
    Hydra_Svm_Complex32_t signal_windows_1;
    Hydra_Svm_Complex32_t signal_windows_2;

    // проверки на всякий случай (если otr1 и otr2 корректные проверка не нужна)
    //if (otr1 < 12 || otr2 < 24)
    //{
    //    return -1; // отражения не найдены!
    //}
    //if (otr2 + window_size + 12 > data_size)
    //{
    //    return -2; // слишком маленькая скорость
    //}
    //if (otr1 > otr2)
    //{
    //    return -3; // были проверки на всякий случай
    //}

    otr2 = otr2 - reserve;
    FILE* pFile = fopen("myfile.txt", "w");
    for (uint16_t i = otr1 - reserve - 2; i < data_size - 4; i = i + 4)
    {
        data_complex[i].Re = p_workconverted[i];//1
        data_complex[i].Im = p_workconverted[i + 1];
        data_complex[i + 1].Re = -1 * p_workconverted[i + 2];//2
        data_complex[i + 1].Im = p_workconverted[i + 1];
        data_complex[i + 2].Re = -1 * p_workconverted[i + 2];//3
        data_complex[i + 2].Im = -1 * p_workconverted[i + 3];
        data_complex[i + 3].Re = p_workconverted[i + 4];//4
        data_complex[i + 3].Im = -1 * p_workconverted[i + 3];

        //data_complex[i].Re = p_workconverted[i];//1
        //data_complex[i].Im = -p_workconverted[i + 1];
        //data_complex[i + 1].Re = -1 * p_workconverted[i + 2];//2
        //data_complex[i + 1].Im = -p_workconverted[i + 1];
        //data_complex[i + 2].Re = -1 * p_workconverted[i + 2];//3
        //data_complex[i + 2].Im = 1 * p_workconverted[i + 3];
        //data_complex[i + 3].Re = p_workconverted[i + 4];//4
        //data_complex[i + 3].Im = 1 * p_workconverted[i + 3];

        //fprintf(pFile, "%d + 1i * %d\n", data_complex[i].Re, data_complex[i].Im);
        //fprintf(pFile, "%d + 1i * %d\n", data_complex[i + 1].Re, data_complex[i + 1].Im);
        //fprintf(pFile, "%d + 1i * %d\n", data_complex[i + 2].Re, data_complex[i + 2].Im);
        //fprintf(pFile, "%d + 1i * %d\n", data_complex[i + 3].Re, data_complex[i + 3].Im);
    }
    //fclose(pFile);

    int16_t index_xcorr_max_f = INDEX_NULL;
    Hydra_Svm_Complex64_t* s1;
    Hydra_Svm_Complex64_t* s2;

    // вычисленеие корреляционной функции
    for (uint16_t i = 0; i < window_size; i++)
    {
        xcorr_current.Im = 0;
        xcorr_current.Re = 0;
        //memset(xcorr_current,0,sizeof(Hydra_Svm_Complex64_t));
        xcorr_current_abs = 0;
        for (uint16_t j = 0; j < (window_size - i); j++)
        {
            // выборка сигнала в первом окне
            uint16_t inx_window_1 = otr1 - reserve + j;
            if (inx_window_1 > max_inx_window_1)
            {
                break;
            }
            signal_windows_1 = data_complex[inx_window_1];
            // выборка сигнала во втором окне
            signal_windows_2 = data_complex[otr2 - reserve + i + j];
            // накапливаем и перемножение signal_windows_1
            // с комплексносопряжённым signal_windows_2
            xcorr_current.Re = xcorr_current.Re +
                ((signal_windows_1.Re * signal_windows_2.Re) +
                    (signal_windows_1.Im * signal_windows_2.Im));
            xcorr_current.Im = xcorr_current.Im +
                ((signal_windows_1.Re * signal_windows_2.Im) -
                    (signal_windows_2.Re * signal_windows_1.Im));

        }
        // вычисляем амплитуду (квадрат амплитуды) корреляционной функции
        xcorr_current_abs = (int64_t)xcorr_current.Re * (int64_t)xcorr_current.Re +
                            (int64_t)xcorr_current.Im * (int64_t)xcorr_current.Im;
        // квадратный корень не берём - нам нужно только найти максимум
        // ищем максимум
        if (xcorr_current_abs > xcorr_max_abs)
        {
            // сохраняем данные для максимума
            index_xcorr_max = i;
            xcorr_max.Re = xcorr_current.Re;
            xcorr_max.Im = xcorr_current.Im;
            xcorr_max_abs = xcorr_current_abs;
        }
        //if (i == index_xcorr_max)
        //{
        //    // сохраняем данные для максимума
        //    xcorr_max.Re = xcorr_current.Re;
        //    xcorr_max.Im = xcorr_current.Im;
        //    xcorr_max_abs = xcorr_current_abs;
        //}

        fprintf(pFile, "%i : %i      %i     %i     %i    %li + 1i * %li       %lli  <=  %lli\n", i, index_xcorr_max, otr1, otr2, window_size, xcorr_current.Re, xcorr_current.Im, xcorr_current_abs, xcorr_max_abs);
    }
    fclose(pFile);

    if (otr_true)
    {
        //s1 = &data_complex[otr1 - reserve];
        //s2 = &data_complex[otr2 - reserve];

        //index_xcorr_max_f = hydra_xcorr_real_v0(s1, s2, 2 * reserve);

        otr2 = otr2 + reserve;
        index_xcorr_max = index_xcorr_max - (int16_t)reserve;

        s1 = &data_complex[otr1 + reserve - 1];
        s2 = &data_complex[otr2 + reserve - 1];

        index_xcorr_max_f = hydra_xcorr_real_v1(s1, s2, (float)0.2, (float)0.6, 3 * reserve + 1);
        //std::cout << index_xcorr_max << " " << index_xcorr_max_f << std::endl;
        index_xcorr_max_f =
            ((((int16_t)index_xcorr_max - (int16_t)index_xcorr_max_f) > -7) &&
                (((int16_t)index_xcorr_max - (int16_t)index_xcorr_max_f) < 7)) ?
            index_xcorr_max_f : INDEX_NULL;
    }
    // индекс (измеренное время распространения в периодах частоты дискретизации)
    index_xcorr_max = (otr2 - otr1 + index_xcorr_max - 1);
    // индекс (измеренное время распространения в периодах частоты дискретизации)
    index_xcorr_max_f = (index_xcorr_max_f == INDEX_NULL) ?
        INDEX_NULL : (otr2 - otr1 + index_xcorr_max_f - 1);

    //после того, как нашли максимальное a b, берем atan(b/a) в максимальном месте
    phase = atan2f((float)xcorr_max.Im, (float)xcorr_max.Re);
    abss = sqrtf(sqrtf((float)xcorr_max_abs) / (float)window_size);

    result->index_time_xcorr = index_xcorr_max;
    result->index_time_propagation = index_xcorr_max_f;
    result->phase = phase;
    result->abs = abss;

    return;
}

float
hydra_time_propagation_calculation(Hydra_out_xcorr_t* xcorr)
{
    float fix_num_periods = 0;
    float time_propagation = 0.0;
    float index_time_propagation = 0.0;
    HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();

    // вермя распространения сигнала от мишени 1 до мишени 2 time_propagation
    index_time_propagation = ((int16_t)(xcorr->index_time_propagation) == INDEX_NULL) ?
        (float)(xcorr->index_time_xcorr) : (float)(xcorr->index_time_propagation);

    // сколько целых периодов "умещается" в time_xcorr
    //fix_num_periods = roundf(time_xcorr*p_svm_prm->sADC.gen_freq);
    fix_num_periods = floorf(index_time_propagation * p_svm_prm->sADC.gen_freq / p_svm_prm->sADC.adc_freq);

    // vTODO ?
    if (((xcorr->phase < 0) && (index_time_propagation - 4 * fix_num_periods) > 1) ||
        ((xcorr->phase < -M_PI / 2.0) && (index_time_propagation - fix_num_periods * 4 == 1)) ||
        ((xcorr->phase >= 0.0) && (xcorr->phase < M_PI / 2.0) && (index_time_propagation - fix_num_periods * 4 == 3))
        )
    {
        fix_num_periods = fix_num_periods + 1;
    }

    if (((float)(xcorr->index_time_xcorr) - 4 * (fix_num_periods + xcorr->phase / (2.0 * M_PI))) > 2) // > 2
    {
        fix_num_periods = fix_num_periods + 1;
    }

    if (((float)(xcorr->index_time_xcorr) - 4 * (fix_num_periods + xcorr->phase / (2.0 * M_PI))) < -2) // < -2
    {
        fix_num_periods = fix_num_periods - 1;
    }

    // время распространения звука
    time_propagation = (fix_num_periods + xcorr->phase / (2.0 * M_PI)) / p_svm_prm->sADC.gen_freq;

    return time_propagation;
}

//float
//hydra_sound_velocity_estimation(Hydra_out_xcorr_t* xcorr_1, float base_1,
//                                Hydra_out_xcorr_t* xcorr_2, float base_2)
//{
//  uint8_t i,j;
//  float min_dsv_current = 0.0;
//  HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
//    float time_propagation_1 = 0.0;  //время распространения звука для первой базы
//    float time_propagation_2 = 0.0;  //время распространения звука для второй базы
//    float sound_velocity_array_1[5]; // возможные скорости звука (по первой базе)
//    float sound_velocity_array_2[5]; // возможные скорости звука (по второй базе)
//    float sound_velocity = 0.0;      // результирующая оценка скорости распространения звука
//    float min_dsv = 1000.0;
//
//    // время распространения звука
//    time_propagation_1 = hydra_time_propagation_calculation(xcorr_1);
//    time_propagation_2 = hydra_time_propagation_calculation(xcorr_2);
//
//    // заполняем массивы возможных скоростей звука
//    for (int8_t i = -2; i < 3; i++)
//    {
//        sound_velocity_array_1[i+2] =
//                  2 * base_1 / (time_propagation_1 + i/p_svm_prm->sADC.gen_freq);
//        sound_velocity_array_2[i+2] =
//                  2 * base_2 / (time_propagation_2 + i/p_svm_prm->sADC.gen_freq);
//    }
//
//    // ищем наибольшее совпадение в значениях двух массивов
//    // (наиболее близкие значения скорости расчитанные по двум базам)
//
//
//    for (i = 0; i < 5; i++)
//    {
//        for (j = 0; j < 5; j++)
//        {
//            min_dsv_current = fabsf(sound_velocity_array_1[i] - sound_velocity_array_2[j]);
//            if (min_dsv_current < min_dsv) // наиболее близкие значения
//            {
//                min_dsv = min_dsv_current;
//                sound_velocity = sound_velocity_array_1[i];
//            }
//        }
//    }
//
//    return sound_velocity;
//}

uint8_t
hydra_svm_calc_v(int16_t* p_workconverted, hydra_svm_work_data_t* p_svm_work_data, Hydra_out_xcorr_t* xcorr_1)
{
    HydraEpTxt_t* p_hydra_ep_txt = hydra_ep_txt_get_addr();
    HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();

    float tost = 1.;
    float interf_corr_speed, v1;
    int8_t ready = 0;
    uint16_t indx = 0;

 //   Hydra_out_xcorr_t xcorr_1;
    float sound_velocity = 0.0;

    float distance_1 = p_hydra_ep_txt->sAJST.base1 / 1000; // расстояние до первой мишени
    float distance   = p_hydra_ep_txt->sAJST.base / 1000; // расстояние между мишенями
    float distance_2 = (p_hydra_ep_txt->sAJST.base + p_hydra_ep_txt->sAJST.base1) / 1000; // расстояние до второй мишени

    //определяем наличие воды (достаточный уровень сигнала)
    p_svm_work_data->water_exist = hydra_svm_water_exist(p_workconverted, HYDRA_SVM_ADC_WORK_BUFF_SIZE);

    if (p_svm_work_data->water_exist)
    {
        // ВКФ для первой базы (основной)
        hydra_svm_vkf_my(p_workconverted, p_svm_work_data->otr1, p_svm_work_data->otr2, xcorr_1);
        // время распространения звука
        tost = hydra_time_propagation_calculation(xcorr_1);

        // примерная оценка скорости звука
        sound_velocity = 2 * (distance) / tost;
        p_svm_work_data->otr1 = (uint16_t)floorf((2 * distance_1 / sound_velocity) * p_svm_prm->sADC.adc_freq);// -(uint16_t)(RESERVE);
        p_svm_work_data->otr2 = (uint16_t)floorf((2 * distance_2 / sound_velocity) * p_svm_prm->sADC.adc_freq);// -(uint16_t)(RESERVE);
    }
    else
    {
        memset(Vaverage_mass, 0, sizeof(Vaverage_mass)); // зачищаем массив усреднения если не в воде
        p_svm_work_data->Vlastgood = 0.0;
    }

    //p_svm_work_data->good_measure = 0;

    //indx = (int)(sound_velocity - 1300) / 40; // оправка от интерференции
    //if (indx >= INTERF_CORR_MAS)
    //{
    //    indx = INTERF_CORR_MAS - 1;
    //}
    //if (indx < 0)
    //{
    //    indx = 0;
    //}
    //v1 = (float)(1300.0 + (float)(indx) * 40.0);
    //interf_corr_speed = hydra_svm_lin_interp(v1, hydra_svm_interf_corr[indx], v1 + 40, hydra_svm_interf_corr[indx + 1], sound_velocity);

    //p_svm_work_data->Vcurrent = sound_velocity + interf_corr_speed;
    //p_hydra_svm_debug_data->vcurrent = sound_velocity;
    //ready = hydra_svm_array_float_add_cval(Vaverage_mass, AVERAGE_MASS, p_svm_work_data->Vcurrent); // добавили посчитанную скорость в массив для фильтрации

    //if ((sound_velocity < 1300) || (sound_velocity > 1900))
    //{
    //    p_svm_work_data->otr1 = 0;
    //    ready = 0;
    //}


    //if (ready > 0)
    //{
    //    p_svm_work_data->Vaverage = hydra_svm_vaverage_calc(Vaverage_mass, AVERAGE_MASS);
    //    if (fabs((p_svm_work_data->Vaverage - p_svm_work_data->Vcurrent)) < 5.)       // допустим скачёк скорости звука не более 5м/с
    //    {
    //        p_svm_work_data->Vlastgood = p_svm_work_data->Vaverage;
    //        p_svm_work_data->tost = tost;
    //        p_svm_work_data->good_measure = (p_svm_work_data->water_exist > 0); // TODO надо больше 4 (500 - нормальный уровень, но, фактически сейчас имеем только 200 )
    //    }

    //    p_hydra_svm_debug_data->ui16_goodmeasure = (p_svm_work_data->good_measure);
    //}

    p_svm_work_data->tost = tost;
    p_svm_work_data->Vcurrent = sound_velocity;

    return p_svm_work_data->water_exist;
}

/**
 * @brief Функция уточняет начало отражённых сигналов
 *
 * @param s1
 * @param s2
 * @param window_size
 * @param index_start_1
 * @param index_start_2
 * @return
 */
int16_t
hydra_xcorr_real_v1(const Hydra_Svm_Complex32_t* s1, const Hydra_Svm_Complex32_t* s2,
    const float trh_1, const float trh_2, const uint16_t window_size)
{
    int16_t  i;
    if (trh_1 > trh_2)
    {
        return 0;
    }
    //if (window_size > 100)
    //{
    //    return 0;
    //}

    int16_t  s1_abs[100] = { 0 };
    int16_t  s2_abs[100] = { 0 };
    int16_t  s1_abs_t[100] = { 0 };
    int16_t  s2_abs_t[100] = { 0 };

    // ищем абсолютное значение
    int16_t  s1_abs_max = 0;
    int16_t  s2_abs_max = 0;
    for (uint16_t i = 0; i < window_size; i++)
    {
        s1_abs_t[i] = (int16_t)sqrt(s1[i].Re * s1[i].Re + s1[i].Im * s1[i].Im);
        s2_abs_t[i] = (int16_t)sqrt(s2[i].Re * s2[i].Re + s2[i].Im * s2[i].Im);
        s1_abs_max = (s1_abs_max > s1_abs_t[i]) ? s1_abs_max : s1_abs_t[i];
        s2_abs_max = (s2_abs_max > s2_abs_t[i]) ? s2_abs_max : s2_abs_t[i];
    }

    for (uint16_t i = 1; i < window_size - 1; i++)
    {
        s1_abs[i] = (s1_abs_t[i - 1] + s1_abs_t[i] + s1_abs_t[i + 1]) / 3;
        s2_abs[i] = (s2_abs_t[i - 1] + s2_abs_t[i] + s2_abs_t[i + 1]) / 3;
    }
    s1_abs[0] = s1_abs_t[0];
    s1_abs[window_size - 1] = s1_abs_t[window_size - 1];
    s2_abs[0] = s2_abs_t[0];
    s2_abs[window_size - 1] = s2_abs_t[window_size - 1];



    // значение порогов
//    uint16_t trh_05_s1 = (uint16_t)(0.5 * (float)s1_abs_max);
//    uint16_t trh_05_s2 = (uint16_t)(0.5 * (float)s2_abs_max);
    int16_t trh_1_s1 = (int16_t)(trh_1 * (float)s1_abs_max);
    int16_t trh_2_s1 = (int16_t)(trh_2 * (float)s1_abs_max);
    int16_t trh_1_s2 = (int16_t)(trh_1 * (float)s2_abs_max);
    int16_t trh_2_s2 = (int16_t)(trh_2 * (float)s2_abs_max);

    // ищем индекс и значение для порога trh_2
    int16_t inx_s1_trh2 = 0;
    int16_t inx_s2_trh2 = 0;
    //    uint16_t inx_05_s1_trh = 0; // наиближайшее к 0.5 от максимума значение на фронте на первом отражении
    //    uint16_t inx_05_s2_trh = 0; // наиближайшее к 0.5 от максимума значение на фронте на втором отражении
    //    int16_t min_delta_trh05_s1 = INT16_MAX;
    //    int16_t min_delta_trh05_s2 = INT16_MAX;
    int16_t min_delta_trh2_s1 = INT16_MAX;
    int16_t min_delta_trh2_s2 = INT16_MAX;
    //    for (i = 0; i < window_size; i++)
    //    {
    //      // сначала ищем 0.5
    //      if (hydra_norm_and_trh(s1_abs[i], trh_05_s1, &min_delta_trh05_s1))
    //      {
    //        inx_05_s1_trh = i;
    //      }
    //      if (hydra_norm_and_trh(s2_abs[i], trh_05_s2, &min_delta_trh05_s2))
    //      {
    //        inx_05_s2_trh = i;
    //      }
    //    } // for i 0.5
    //    for(i = inx_05_s1_trh; i < window_size; ++i) // vTODO уменьшить окно!
    for (i = 0; i < window_size; i++)
    {
        // для массива s1
        if (hydra_norm_and_trh(s1_abs[i], trh_2_s1, &min_delta_trh2_s1))
        {
            inx_s1_trh2 = i;
        }
        // для массива s2
        if (hydra_norm_and_trh(s2_abs[i], trh_2_s2, &min_delta_trh2_s2))
        {
            inx_s2_trh2 = i;
        }
    }


    //    int64_t abs_s1_trh2 = s1_abs[inx_05_s1_trh];
    //    int64_t abs_s2_trh2 = s2_abs[inx_05_s2_trh];
    int64_t abs_s1_trh2 = s1_abs[inx_s1_trh2];
    int64_t abs_s2_trh2 = s2_abs[inx_s2_trh2];

    // ищем индекс и значение для порога trh_1
    int64_t abs_s1_trh1 = abs_s1_trh2;
    int64_t abs_s2_trh1 = abs_s2_trh2;
    int16_t inx_s1_trh1 = 0;
    int16_t inx_s2_trh1 = 0;
    int16_t inx_cur = 0;
    while ((abs_s1_trh1 > trh_1_s1))
    {
        --inx_s1_trh1;
        inx_cur = inx_s1_trh1 + inx_s1_trh2;
        if (inx_cur < 0)
        {
            break;
        }
        abs_s1_trh1 = s1_abs[inx_cur];
    }
    while ((abs_s2_trh1 > trh_1_s2))
    {
        --inx_s2_trh1;
        inx_cur = inx_s2_trh1 + inx_s2_trh2;
        if (inx_cur < 0)
        {
            break;
        }
        abs_s2_trh1 = s2_abs[inx_cur];
    }

    float inx_s1_0 = (float)inx_s1_trh1 +
        (float)((int16_t)inx_s1_trh1 * abs_s1_trh1) / (float)(abs_s1_trh2 - abs_s1_trh1);
    float inx_s2_0 = (float)inx_s2_trh1 +
        (float)((int16_t)inx_s2_trh1 * abs_s2_trh1) / (float)(abs_s2_trh2 - abs_s2_trh1);

    int16_t index = (int16_t)(round(inx_s2_0 - inx_s1_0)) + (inx_s2_trh2 - inx_s1_trh2);
    //std::cout << index << std::endl;
    return index;
}

uint8_t
hydra_norm_and_trh(int16_t s_abs, int16_t trh, int16_t* min_delta_trh)
{
    // ищем ближайшее к trh значение
    int16_t delta = (trh > s_abs) ? (trh - s_abs) : (s_abs - trh);
    if (delta < *min_delta_trh) {
        *min_delta_trh = delta; // запоминаем новую min delta
        return TRUE; // признак изменения значений
    }
    return FALSE;
}


/**
 * @brief функция расчитывает паспортные параметры base и base1
 *
 * @param speed - значение скорости
 * @return индекс последнего значения в массиве
 */
uint16_t
hydra_svm_speed_calibration(float* speed)
{
    uint16_t i, i2;
    float v1c = 2.4, v2c = 4, ve;
    float interf_corr = 3;
    const uint16_t otbros = 22;
    SpeedCalibrationMas_t speed_current;
    HydraEpTxt_t* p_ep_txt = hydra_ep_txt_get_addr();
    hydra_svm_work_data_t* p_svm_work_data = hydra_svm_math_get_addr_hydra_svm_work_data();
    float time1 = p_svm_work_data->time1;
    float tost = p_svm_work_data->tost;
    float base1 = 0;
    float base = 0;
    // сначала собираем массив на CALIB_MAS=100 измерений скорости и времени
    // сортируем его и отбрасываем по четыре первых и последних измерений чтобы отсеять выбросы
    // оставшиеся измерения усредняем
    // вычисляем расстояния до мишени

    if (hydra_svm_speed_calibration_mas_index < CALIB_MAS) // набираем массив
    {
        hydra_svm_speed_calibration_mas[hydra_svm_speed_calibration_mas_index].c = *speed;
        hydra_svm_speed_calibration_mas[hydra_svm_speed_calibration_mas_index].t = tost;//*time;
        hydra_svm_speed_calibration_mas[hydra_svm_speed_calibration_mas_index].t1 = time1;//*time1;
        hydra_svm_speed_calibration_mas_index++;
        return hydra_svm_speed_calibration_mas_index;
    }
    else
    {
        for (i = 0; i < CALIB_MAS; ++i)// сортируем
        {
            for (i2 = i; i2 < CALIB_MAS; ++i2)
            {
                if (hydra_svm_speed_calibration_mas[i].t < hydra_svm_speed_calibration_mas[i2].t)
                {
                    memcpy(&speed_current, &hydra_svm_speed_calibration_mas[i], sizeof(SpeedCalibrationMas_t));
                    memcpy(&hydra_svm_speed_calibration_mas[i], &hydra_svm_speed_calibration_mas[i2], sizeof(SpeedCalibrationMas_t));
                    memcpy(&hydra_svm_speed_calibration_mas[i2], &speed_current, sizeof(SpeedCalibrationMas_t));

                }
            }//for i2
        }//for i

        // считаем длину базы и усредняем
        for (i = otbros; i < CALIB_MAS - otbros; ++i)
        {
            // определяем интерфереционную поправку
            i2 = (int)((hydra_svm_speed_calibration_mas[i].c - 1300) / 40);
            if (i2 < 10)
            {
                v1c = hydra_svm_interf_corr[i2];//1300 + 40*i;
                v2c = hydra_svm_interf_corr[i2 + 1];//1300 + 40*(i+1);
                ve = hydra_svm_speed_calibration_mas[i2].c;
                //interf_corr =  (v2c-v1c)*(ve-(40*i2 + 1300))/40 + v1c; // линейно интерполируем поправку
                interf_corr = hydra_svm_lin_interp(1300 + 40 * i2, v1c, 1300 + 40 * (i2 + 1), v2c, ve);
            }
            base = base + (hydra_svm_speed_calibration_mas[i].c - interf_corr) * hydra_svm_speed_calibration_mas[i].t * 1000 / 2; // мм
            base1 = base1 + (hydra_svm_speed_calibration_mas[i].c - interf_corr) * hydra_svm_speed_calibration_mas[i].t1 * 1000 / 2; // мм
        }
        p_ep_txt->sAJST.base = base / (CALIB_MAS - otbros * 2);
        //p_ep_txt->sAJST.base1 = base1 /(CALIB_MAS-otbros*2);

        hydra_svm_speed_calibration_mas_index = 0;
        return 0;
    }// else index<CALIT_MAS
    return 0;
}


/**
 * @brief функция усредняет скорость звука в воде
 *
 * @param work_mass
 * @param work_mass_size
 * @param Vcurrent
 * @return усреднённая скорость
 */
float
hydra_svm_vaverage_calc(float* work_mass, const uint16_t work_mass_size)
{
    float V;
    int i, isort;
    int dead_zone_bot = (int)(work_mass_size * 3 / 8 - 1);
    int dead_zone_top = (int)(work_mass_size * 3 / 8 - 1);
    int isre = 0;

    float work_mass_copy[AVERAGE_MASS];
    memcpy(work_mass_copy, work_mass, work_mass_size * sizeof(float));

    for (i = 0; i < work_mass_size - 1; ++i)
    {
        // vTODO
        for (isort = 0; isort < work_mass_size - i - 1; ++isort)
        {
            if (work_mass_copy[isort] > work_mass_copy[isort + 1])
            {
                V = work_mass_copy[isort + 1];
                work_mass_copy[isort + 1] = work_mass_copy[isort];
                work_mass_copy[isort] = V;
            }
        }// for isort
    }// for i
    V = 0.;
    for (i = dead_zone_bot; i < (work_mass_size - dead_zone_top); ++i)
    {
        V = V + work_mass_copy[i];
        isre++;
    }
    V = V / isre;

    return V;
}

float
hydra_svm_vaverage_calc_v2(float* work_mass, const uint16_t work_mass_size)
{
    float a = 0.5;
    float b = 1. - a;
    float V = 0., Vsr;
    uint16_t i, isort;
    uint16_t isre = 0;
    uint16_t test_start = 0, test_end = 1;
    uint16_t istart = 0, iend = 1;

    float work_mass_copy[AVERAGE_MASS];
    memcpy(work_mass_copy, work_mass, work_mass_size * sizeof(float));

    for (i = 0; i < work_mass_size - 1; ++i)
    {
        // vTODO
        for (isort = 0; isort < work_mass_size - i - 1; ++isort)
        {
            if (work_mass_copy[isort] > work_mass_copy[isort + 1])
            {
                V = work_mass_copy[isort + 1];
                work_mass_copy[isort + 1] = work_mass_copy[isort];
                work_mass_copy[isort] = V;
            }
        }// for isort
    }// for i
    Vsr = work_mass_copy[0];
    for (i = 1; i < work_mass_size - 1; ++i)
    {
        if (fabs(work_mass_copy[i] - Vsr) < 5.)
        {
            Vsr = a * work_mass_copy[i] + b * Vsr;
            test_end = i;
        }
        else
        {
            Vsr = work_mass_copy[i];
            test_start = i;
        }//esle abs
        if (test_end - test_start > iend - istart)
        {
            istart = test_start;
            iend = test_start;
        }
    }//for i

    for (i = istart; i < (iend); ++i)
    {
        V = V + work_mass_copy[i];
        isre++;
    }
    V = V / isre;
    return V;
}

uint8_t
hydra_svm_water_exist(int16_t* work_mass, const uint16_t work_mass_size)
{
    uint16_t i;
    for (i = 300; i < work_mass_size - 20; i++)    // 330 - в начале массива излучённый сигнал (overdrive)
    {                                     // 20  - в конце массива какой-то артефакт мешает определять отсутствие воды!
        if (work_mass[i] > 500.) // vTODO
            return 5;
        // 20  - в конце массива какой-то артефакт мешает определять отсутствие воды!
        if (work_mass[i] > 100.) // vTODO
            return 1;
    }
    return 0;
}

/**
 * @brief
 *
 * @param p_workconverted
 * @param work_converted_size
 * @param p_otr1
 * @param p_otr2
 * @return
 */
uint16_t
hydra_svm_windows_finder(int16_t* p_workconverted, const uint16_t work_converted_size, uint16_t* p_otr1, uint16_t* p_otr2)
{
    uint16_t i, iwindow;
    uint16_t schetchik = 0;
    uint16_t otr1 = 0, otr2 = 0, otr3 = 0;

    double A = 0; // сумма амплитуд исследуемого сигнала
    double A1 = 0, A2 = 0; // суммарные самые большие амплитуды

    HydraEpTxt_t* p_ep_txt = hydra_ep_txt_get_addr();
    HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
    uint16_t massiv_size = HYDRA_SVM_ADC_WORK_BUFF_SIZE;
    double cnt2i = p_svm_prm->sADC.adc_freq / p_svm_prm->sADC.gen_freq;
    uint16_t window_size = (uint16_t)(p_svm_prm->sADC.imp_cnt * (cnt2i));
    //float kbase = (p_hydra_ep_txt->sAJST.base1 + p_hydra_ep_txt->sAJST.base)/p_hydra_ep_txt->sAJST.base1;
    uint16_t detect_level2 = 500; // уровень сигнала (определяется делителем на положительном входе операционника) TODO из паспорта!
    uint16_t detect_level1 = 200; // уровень сигнала

    if (window_size > massiv_size)
        return 1;

    // считаем, что оцифровывается весь сигнал (с излучением)
    for (i = 200; i < massiv_size * 0.6; i++) // ищем первое отражение
    {
        if ((p_workconverted[i] > detect_level1)) // если уровень больше среднего на 100 (для 12бит) (или 20 для 8бит)
        {
            A = 0;
            schetchik = 0;
            for (iwindow = 0; iwindow <= window_size; iwindow = iwindow + 4)
            {
                if (p_workconverted[iwindow + i] + p_workconverted[iwindow + i + 2] < 80)
                {
                    schetchik++;
                    //A = A + sqrt(p_workconverted[iwindow+i]*p_workconverted[iwindow+i] + p_workconverted[iwindow+i+1]*p_workconverted[iwindow+i+1]);
                }// if iwindow
            }// for iwindow
            if (schetchik >= p_svm_prm->sADC.imp_cnt - 2)
            {
                if (p_workconverted[i + 9] > 0 && p_workconverted[i + 10] > 0)
                    detect_level2 = (int16_t)(p_workconverted[i + 9] + p_workconverted[i + 10]) / 3;
                else if (p_workconverted[i + 9] > 0 && p_workconverted[i + 8] > 0)
                    detect_level2 = (int16_t)(p_workconverted[i + 9] + p_workconverted[i + 8]) / 3;
                //        for(iwindow=0;iwindow<schetchik*cnt2i;iwindow = iwindow+4)
                //        {
                //          A = A + sqrt(p_workconverted[i+iwindow]*p_workconverted[i+iwindow] + p_workconverted[i+iwindow+1]*p_workconverted[i+iwindow+1] );
                //        }
                //        A = A/schetchik; //средняя амплитуда
                //        if(A1 < A)
                {
                    //          A1=A;
                    otr1 = i;
                    if (detect_level2 > 2000)
                    {
                        return 1;
                    }
                    break;
                }
            }
        }//if detect_level1
    }
    for (i = (uint16_t)(otr1 * 2.4); i < massiv_size; i++) // ищем второе отражение
    {
        if ((p_workconverted[i] > detect_level2)) // если уровень больше среднего на 100 (для 12бит) (или 20 для 8бит)
        {
            A = 0;
            schetchik = 0;
            for (iwindow = 0; iwindow <= window_size; iwindow = iwindow + 4)
            {
                if (p_workconverted[iwindow + i] + p_workconverted[iwindow + i + 2] < 80)
                {
                    schetchik++;
                    //A = A + sqrt(p_workconverted[iwindow+i]*p_workconverted[iwindow+i] + p_workconverted[iwindow+i+1]*p_workconverted[iwindow+i+1]);
                }// if iwindow
            }// for iwindow
            if (schetchik >= p_svm_prm->sADC.imp_cnt - 2)
            {
                //        for(iwindow=0;iwindow<schetchik*cnt2i;iwindow = iwindow+4)
                //        {
                //          A = A + sqrt(p_workconverted[i+iwindow]*p_workconverted[i+iwindow] + p_workconverted[i+iwindow+1]*p_workconverted[i+iwindow+1] );
                //        }
                //        A = A/schetchik; //средняя амплитуда
                //        if(A2 < A)
                //        {
                //          A2=A;
                otr2 = i;
                break;
                //        }
            }
        }//if detect_level1
    }


    *p_otr1 = otr1 - 4;
    *p_otr2 = otr2 - 4;


    if (*p_otr1 > 0 && *p_otr2 > 0)
        return 0;

    return 1; /* ошибка */
}

/**
 * @brief функция линейной интерполяции
 *
 * @param x1 значение аргумента 1
 * @param y1 значение функции 1
 * @param x2 значение аргумента 2
 * @param y2 значение функции 2
 * @param x  значение аргумента искомого значения
 * @return значение функции в искомом месте
 */
float
hydra_svm_lin_interp(float x1, float y1, float x2, float y2, float x)
{
    float y;
    float dx = x2 - x1;
    float dy = y2 - y1;
    y = (x - x1) * dy / dx + y1;

    return y;
}

/**
  * @}
  */
  /*============================= Section Exported End ============================*/


  /**
    * @}
    */
    /****************************END OF FILE****************************/
