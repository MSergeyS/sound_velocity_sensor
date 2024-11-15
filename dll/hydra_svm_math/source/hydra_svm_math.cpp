#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

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
    Hydra_Svm_Complex32_t data_complex[HYDRA_SVM_ADC_OUT_BUFF_SIZE] = {0};

    Hydra_Svm_Complex32_t xcorr_max = {0,0};
    int64_t               xcorr_max_abs = 0;
    int16_t               index_xcorr_max = 0;

    uint16_t reserve = 17; // 0
    float    phase = 0.0;
    float    abs = 0;
    int64_t xcorr_current_abs = 0;
    Hydra_Svm_Complex32_t xcorr_current;
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

        // data_complex[i].Re = -1 * p_workconverted[i];//1
        // data_complex[i].Im = -1 * p_workconverted[i + 1];
        // data_complex[i + 1].Re = p_workconverted[i + 2];//2
        // data_complex[i + 1].Im = -1 *p_workconverted[i + 1];
        // data_complex[i + 2].Re = p_workconverted[i + 2];//3
        // data_complex[i + 2].Im = p_workconverted[i + 3];
        // data_complex[i + 3].Re = -1 * p_workconverted[i + 4];//4
        // data_complex[i + 3].Im = p_workconverted[i + 3];

        // data_complex[i].Re = p_workconverted[i + 1];//1
        // data_complex[i].Im = p_workconverted[i + 2];
        // data_complex[i + 1].Re = -1 * p_workconverted[i + 3];//2
        // data_complex[i + 1].Im = p_workconverted[i + 2];
        // data_complex[i + 2].Re = -1 * p_workconverted[i + 3];//3
        // data_complex[i + 2].Im = -1 * p_workconverted[i + 4];
        // data_complex[i + 3].Re = p_workconverted[i + 5];//4
        // data_complex[i + 3].Im = -1 * p_workconverted[i + 4];

        // data_complex[i].Re = -1 * p_workconverted[i + 1];//1
        // data_complex[i].Im = -1 * p_workconverted[i + 2];
        // data_complex[i + 1].Re = p_workconverted[i + 3];//2
        // data_complex[i + 1].Im = -1 * p_workconverted[i + 2];
        // data_complex[i + 2].Re = p_workconverted[i + 3];//3
        // data_complex[i + 2].Im = p_workconverted[i + 4];
        // data_complex[i + 3].Re = -1 * p_workconverted[i + 5];//4
        // data_complex[i + 3].Im = p_workconverted[i + 4];
    }

    int16_t index_xcorr_max_f = 0;
    Hydra_Svm_Complex32_t* s1;
    Hydra_Svm_Complex32_t* s2;

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
        //if (i == index_xcorr_max)
        //{
        //    // сохраняем данные для максимума
        //    xcorr_max.Re = xcorr_current.Re;
        //    xcorr_max.Im = xcorr_current.Im;
        //    xcorr_max_abs = xcorr_current_abs;
        //}
    }

    if (otr_true)
    {
        //s1 = &data_complex[otr1 - reserve];
        //s2 = &data_complex[otr2 - reserve];
        
        //index_xcorr_max_f = hydra_xcorr_real_v0(s1, s2, 2 * reserve);

        otr2 = otr2 + reserve;
        index_xcorr_max = index_xcorr_max - (int16_t)reserve;

        s1 = &data_complex[otr1 - 8];
        s2 = &data_complex[otr2 - 8];

        index_xcorr_max_f = hydra_xcorr_real_v1(s1, s2, (float)0.2, (float)0.6, 2*reserve + 1);

        std::cout << index_xcorr_max_f << " " << index_xcorr_max << std::endl;
  
        index_xcorr_max =
               ((((int16_t)index_xcorr_max - (int16_t)index_xcorr_max_f) > -7) &&
                (((int16_t)index_xcorr_max - (int16_t)index_xcorr_max_f) < 7)) ?
               index_xcorr_max_f : index_xcorr_max;
    }

    //после того, как нашли максимальное a b, берем atan(b/a) в максимальном месте
    phase = atan2f((float)xcorr_max.Im, (float)xcorr_max.Re);
    abs = sqrtf(sqrtf((float)xcorr_max_abs) / (float)window_size);

    // индекс (измеренное время распространения в периодах частоты дискретизации)
    index_xcorr_max = (otr2 - otr1 + index_xcorr_max - 1);

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
hydra_sound_velocity_estimation(Hydra_out_xcorr_t xcorr_1, float base_1,
                                Hydra_out_xcorr_t xcorr_2, float base_2)
{
    float time_propagation_1 = 0.0;  //время распространения звука для первой базы
    float time_propagation_2 = 0.0;  //время распространения звука для второй базы
    float sound_velocity_array_1[3]; // возможные скорости звука (по первой базе)
    float sound_velocity_array_2[3]; // возможные скорости звука (по второй базе)
    float sound_velocity = 0.0;      // результирующая оценка скорости распространения звука
    float min_dsv = 1000.0;

    int8_t number_steps = 1;

    // время распространения звука
    time_propagation_1 = hydra_time_propagation_calculation(xcorr_1);
    time_propagation_2 = hydra_time_propagation_calculation(xcorr_2);

    // заполняем массивы возможных скоростей звука
    for (int8_t i = -1*number_steps; i < (number_steps + 1); i++)
    {
        sound_velocity_array_1[i+1] = 
                  2 * base_1 / (time_propagation_1 + i/FREQUENCY_CENTRAL);
        sound_velocity_array_2[i+1] = 
                  2 * base_2 / (time_propagation_2 + i/FREQUENCY_CENTRAL);
    }
    
    // ищем наибольшее совпадение в значениях двух массивов
    // (наиболее близкие значения скорости расчитанные по двум базам)
    float min_dsv_current = 0.0;
    for (uint8_t i = 0; i < 2*number_steps+1; i++)
    {
        for (uint8_t j = 0; j < 2*number_steps+1; j++)
        {
            min_dsv_current = fabsf(sound_velocity_array_1[i] - sound_velocity_array_2[j]);
            if (min_dsv_current < min_dsv) // наиболее близкие значения
            {
                min_dsv = min_dsv_current;
                sound_velocity = sound_velocity_array_1[i];
            }
        }
    }

    return sound_velocity;
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
hydra_xcorr_real_v0(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, uint16_t window_size)
{
    uint16_t index_xcorr_min = 0;

    int64_t  xcorr_current_abs = 0;
    int64_t  xcorr_min_abs = LLONG_MAX;

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
    float coeff_norm = (float)s2_abs_max / (float)s1_abs_max;

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

uint16_t
hydra_xcorr_real_abs(int32_t* s1_abs, int32_t* s2_abs, uint16_t window_size)
{
    uint16_t index_xcorr_min = 0;

    int64_t  xcorr_current_abs = 0;
    int64_t  xcorr_min_abs = LLONG_MAX;

    uint16_t inx_s2;
    // вычисленеие корреляционной функции
    for (uint16_t i = 0; i < (window_size - 1); i++)
    {
        xcorr_current_abs = 0;
        for (uint16_t j = 0; j < (window_size - 1); j++)
        {
            inx_s2 = i + j;
            xcorr_current_abs = xcorr_current_abs +
                ((s1_abs[j] > s2_abs[inx_s2]) ?
                  s1_abs[j] - s2_abs[inx_s2] : s2_abs[inx_s2] - s1_abs[j]);
        }
        if (xcorr_current_abs < xcorr_min_abs)
        {
            // сохраняем данные для максимума
            index_xcorr_min = i;
            xcorr_min_abs = xcorr_current_abs;
        }
    }

    return index_xcorr_min;
}

int16_t
hydra_xcorr_real_v1(const Hydra_Svm_Complex32_t* s1, const Hydra_Svm_Complex32_t* s2, 
                    const float trh_1, const float trh_2, const uint16_t window_size)
{
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

    for (uint16_t i = 1; i < window_size-1; i++)
    {
        s1_abs[i] = (s1_abs_t[i - 1] + s1_abs_t[i] + s1_abs_t[i + 1]) / 3;
        s2_abs[i] = (s2_abs_t[i - 1] + s2_abs_t[i] + s2_abs_t[i + 1]) / 3;
    }
    s1_abs[0] = s1_abs_t[0];
    s1_abs[window_size-1] = s1_abs_t[window_size-1];
    s2_abs[0] = s2_abs_t[0];
    s2_abs[window_size-1] = s2_abs_t[window_size-1];

    std::cout << "s1_abs = [";
    for (int16_t i = 0; i < window_size; i++)
    {
        std::cout << s1_abs[i] << " ";
    }
    std::cout << "]" << std::endl;
    std::cout << "s2_abs = [";
    for (int16_t i = 0; i < window_size; i++)
    {
        std::cout << s2_abs[i] << " ";
    }
    std::cout << "]" << std::endl;

    // значение порогов
    int16_t trh_1_s1 = (int16_t)(trh_1 * (float)s1_abs_max);
    int16_t trh_2_s1 = (int16_t)(trh_2 * (float)s1_abs_max);
    int16_t trh_1_s2 = (int16_t)(trh_1 * (float)s2_abs_max);
    int16_t trh_2_s2 = (int16_t)(trh_2 * (float)s2_abs_max);

    // ищем индекс и значение для порога trh_2
    int16_t inx_s1_trh2 = 0;
    int16_t inx_s2_trh2 = 0;
    int16_t min_delta_trh2_s1 = INT16_MAX;
    int16_t min_delta_trh2_s2 = INT16_MAX;
    for (int16_t i = 0; i < window_size; i++)
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
            (float)( (int16_t)inx_s1_trh1 * abs_s1_trh1) / (float)(abs_s1_trh2 - abs_s1_trh1);
    float inx_s2_0 = (float)inx_s2_trh1 +
            (float)((int16_t)inx_s2_trh1 * abs_s2_trh1) / (float)(abs_s2_trh2 - abs_s2_trh1);

    std::cout << inx_s1_trh1 << " " << inx_s1_trh2 << " " <<
                 abs_s1_trh1 << " " << abs_s1_trh2 << " " << inx_s1_0 << std::endl;
    std::cout << inx_s2_trh1 << " " << inx_s2_trh2 << " " << 
                 abs_s2_trh1 << " " << abs_s2_trh2 << " " << inx_s2_0 << std::endl;

    int16_t index = int16_t(round(inx_s2_0 - inx_s1_0)) + (inx_s2_trh2 - inx_s1_trh2);
    std::cout << index << std::endl;
    return index;
}

bool
hydra_norm_and_trh(int16_t s_abs, int16_t trh, int16_t* min_delta_trh)
{
    // ищем ближайшее к trh значение
    int16_t delta = (trh > s_abs) ? (trh - s_abs) : (s_abs - trh);
    if (delta < *min_delta_trh) {
        *min_delta_trh = delta; // запоминаем новую min delta
        return true; // признак изменения значений
    }
    return false;
}