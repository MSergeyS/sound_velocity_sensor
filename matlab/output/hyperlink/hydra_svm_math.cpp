#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "hydra_svm_math.h"

Hydra_out_xcorr_t
hydra_svm_vkf_my(int16_t* p_workconverted, uint16_t otr1, uint16_t otr2)
{
    //otr1 = 0;
    //otr2 = 0;
    //HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
    //float DATA_RATE = p_svm_prm->sADC.adc_freq;
    //float FREQUENCY_CENTRAL = p_svm_prm->sADC.gen_freq;
    uint16_t signal_lenght = 30 *(uint16_t)(DATA_RATE / FREQUENCY_CENTRAL);
    uint16_t window_size = (signal_lenght  + 57); // сигнал с хвостом
    uint16_t max_inx_window_1 = otr1 + window_size;
    bool otr_true = true;
    if(otr1 == 0)
    {
        otr1 = 336;              // минимальный индекс начала первого окна
        otr2 = 811;              // минимальный индекс начала первого окна
        window_size = 370 + 120; // максимальный размер окна
        max_inx_window_1 = 560;  // максимальный индекс первого окна
        otr_true = false;
    }

    uint16_t              data_size = HYDRA_SVM_ADC_OUT_BUFF_SIZE;
    Hydra_Svm_Complex32_t data_complex[HYDRA_SVM_ADC_OUT_BUFF_SIZE];

    Hydra_Svm_Complex32_t xcorr_max = {0,0};
    int64_t               xcorr_max_abs = 0;
    uint16_t              index_xcorr_max = 0;

    uint16_t reserve = 8; // 0
    float    phase = 0.0;
    float    abs = 0;
    int64_t xcorr_current_abs = 0;
    Hydra_Svm_Complex32_t xcorr_current;
    Hydra_Svm_Complex32_t signal_windows_1;
    Hydra_Svm_Complex32_t signal_windows_2;

    otr2 = otr2 - reserve;

    for (uint16_t i = otr1 - reserve; i < data_size - 4; i = i + 4)
    {
        data_complex[i].Re = p_workconverted[i];//1
        data_complex[i].Im = p_workconverted[i + 1];
        data_complex[i + 1].Re = -1 * p_workconverted[i + 2];//2
        data_complex[i + 1].Im = p_workconverted[i + 1];
        data_complex[i + 2].Re = -1 * p_workconverted[i + 2];//3
        data_complex[i + 2].Im = -1 * p_workconverted[i + 3];
        data_complex[i + 3].Re = p_workconverted[i + 4];//4
        data_complex[i + 3].Im = -1 * p_workconverted[i + 3];
    }

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
            xcorr_current.Re =  xcorr_current.Re +
                                    ( (signal_windows_1.Re * signal_windows_2.Re) +
                                      (signal_windows_1.Im * signal_windows_2.Im) );
            xcorr_current.Im =  xcorr_current.Im +
                                    ( (signal_windows_1.Re * signal_windows_2.Im) -
                                      (signal_windows_2.Re * signal_windows_1.Im) );

        }
        // вычисляем амплитуду (квадрат амплитуды) корреляционной функции
        xcorr_current_abs = (int64_t)xcorr_current.Re*xcorr_current.Re +
                            (int64_t)xcorr_current.Im*xcorr_current.Im;
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
    }

    if (otr_true)
    {
        Hydra_Svm_Complex32_t* s1 = &data_complex[otr1 - reserve];
        Hydra_Svm_Complex32_t* s2 = &data_complex[otr2 - reserve];
        uint16_t index_xcorr_max_f = hydra_xcorr_real(s1, s2, 2 * reserve);
        index_xcorr_max =
            ((index_xcorr_max - index_xcorr_max_f > -7) && (index_xcorr_max - index_xcorr_max_f < 7)) ?
            index_xcorr_max_f : index_xcorr_max;

    }

    //после того, как нашли максимальное a b, берем atan(b/a) в максимальном месте
    phase = atan2f((float)xcorr_max.Im, (float)xcorr_max.Re);
    abs = sqrtf(sqrtf((float)xcorr_max_abs) / (float)window_size);

    // индекс (измеренное время распространения в периодах частоты дискретизации)
    index_xcorr_max = (otr2 - otr1 + index_xcorr_max);

    Hydra_out_xcorr_t result = {index_xcorr_max, phase, abs};
    return result;
}

float
hydra_time_propagation_calculation(Hydra_out_xcorr_t xcorr)
{
    float time_xcorr = 0.0;
    float fix_num_periods = 0;
    float time_propagation = 0.0;
    
    // измеренное время распространения в периодах частоты дискретизации
    time_xcorr = ((float)(xcorr.index_time_propagation))/DATA_RATE;

    // сколько целых периодов "умещается" в time_xcorr
    fix_num_periods = floorf(time_xcorr*FREQUENCY_CENTRAL);
    if ( ( ( xcorr.phase < 0 ) && (xcorr.index_time_propagation - 4*fix_num_periods > 1) ) ||
         ( (xcorr.phase < -M_PI / 2.0) && (xcorr.index_time_propagation - fix_num_periods * 4 == 1) ) ||
         ( (xcorr.phase >= 0.0) && (xcorr.phase < M_PI / 2.0) && (xcorr.index_time_propagation - fix_num_periods * 4 == 3) ) )
    {
        fix_num_periods = fix_num_periods + 1;
    }

    // время распространения звука
    time_propagation =
         (fix_num_periods + xcorr.phase / (float)(2.0 * M_PI))/FREQUENCY_CENTRAL;

    

    return time_propagation; // time_xcorr;//
}

float
hydra_svm(int16_t* p_workconverted, float distance_1, float distance_2,
          uint16_t* p_otr1, uint16_t* p_otr2,
          Hydra_out_xcorr_t* xcorr)
{
    float time_propagation = 0.0;
    float sound_velocity = 0.0;
    
    // ВКФ для первой базы (основной)
    *xcorr = hydra_svm_vkf_my(p_workconverted, *p_otr1, *p_otr2);
    // время распространения звука
    time_propagation = hydra_time_propagation_calculation(*xcorr);
    // примерная оценка скорости звука
    sound_velocity = 2 * (distance_2 - distance_1) / time_propagation;

    *p_otr1 = (uint16_t)roundf((2*distance_1/sound_velocity)*DATA_RATE);
    //*p_otr2 = *p_otr1 + xcorr->index_time_propagation-1;
    *p_otr2 = (uint16_t)roundf((2*distance_2/sound_velocity)*DATA_RATE);

    return sound_velocity;
}

uint16_t
hydra_xcorr_real(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, uint16_t window_size)
{
    uint16_t index_xcorr_min = 0;

    int64_t  xcorr_current_abs = 0;
    int64_t  xcorr_min_abs = 10000000000000000;

    int64_t  s1_abs[100] = {0};
    int64_t  s2_abs[100] = {0};

    int64_t  s1_abs_max = 0;
    int64_t  s2_abs_max = 0;

    for (uint16_t i = 0; i < window_size - 1; i++)
    {
        s1_abs[i] = (int64_t) sqrt(s1[i].Re * s1[i].Re + s1[i].Im * s1[i].Im);
        s2_abs[i] = (int64_t) sqrt(s2[i].Re * s2[i].Re + s2[i].Im * s2[i].Im);
        s2_abs[window_size+i] = 
                    (int64_t) sqrt(s2[window_size+i].Re * s2[window_size+i].Re +
                                   s2[window_size+i].Im * s2[window_size+i].Im);
        s1_abs_max = (s1_abs_max > s1_abs[i]) ? s1_abs_max : s1_abs[i];
        s2_abs_max = (s2_abs_max > s2_abs[i]) ? s2_abs_max : s2_abs[i];
        s2_abs_max = (s2_abs_max > s2_abs[window_size + i]) ? s2_abs_max : s2_abs[window_size + i];
    }
    float coeff_norm = (float) s2_abs_max / (float)s1_abs_max;

    for (uint16_t i = 0; i < window_size - 1; i++)
    {
        s1_abs[i] = (int64_t) (coeff_norm * (float) s1_abs[i]);
    }

    uint16_t inx_s2;
    // вычисленеие корреляционной функции
    for (uint16_t i = 0; i < (window_size - 1); i++)
    {
        xcorr_current_abs = 0;
        for (uint16_t j = 0; j < (window_size - 1); j++)
        {
            inx_s2 = i + j;
            xcorr_current_abs = xcorr_current_abs + 
                ( (s1_abs[j] > s2_abs[inx_s2]) ? 
                   s1_abs[j] - s2_abs[inx_s2] : s2_abs[inx_s2] - s1_abs[j] );
        }
        if (xcorr_current_abs < xcorr_min_abs)
        {
            // сохраняем данные для максимума
            index_xcorr_min = i;
            xcorr_min_abs = xcorr_current_abs;
        }
    }

    return index_xcorr_min - 3;
}