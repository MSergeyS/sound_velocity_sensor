//#include <cstdint>
#include "hydra-types.h"
#include "defines.h"

typedef struct Hydra_Result_t
{

  uint32_t     session_tmr;
  uint16_t     session_number;
  uint16_t     reserv;

  struct
  {
    uint32_t time;           /**< Значение таймера на момент запуска ? */
    uint8_t stage;        /** флаг готовности оцифрованных значений. Устанавливается в ф. оцифровки, снимается в calc*/
    uint8_t previous_mode_id;
    uint8_t reserv[2];
    float ampl;              /**< Уровань сигнала (амплитуда) */
  }sCalc; /**промежуточные расчётные данные*/

  struct
  {
       float   vs; /** расчитаное значение скорости звука, м/с, действительно если не none */
       float   vs_raw; /** расчитаное значение скорости звука (сырые данные), м/с, действительно если не none */
       uint8_t fl_water_exist;/** признак нахождения в среде измерения, 0-не в среде, 1- в среде (в воде) */
       uint8_t reserv[3];

  }sSvmOutput; /** Параметры для выдачи на ведущую систему */


//  struct /// флаги текущего состояния работы
//  {
//    uint8_t stat_dataprocess;// текущая выполняемая задача измерения скорости звука
//    uint8_t mes_time; // время измерения/оцифровки данных (для фильтров) vTODO HydraSvmParam_t?
//
//  }sCurrentStatus;

} Hydra_Result_t;            /**< Структура с результатами работы SVM */

typedef struct HydraSvmParam_t
{
  Hydra_Result_t result;

  struct
  {
    uint16_t  imp_cnt;                /** количество излучаемых импульсов в пачке*/
    uint16_t  signal_len;             /** длина сигнала в замерах*/
    float     gen_freq;               /** частота излучения в МГц */
    float     adc_freq;               /** частота оцифровки в МГц */
  }sADC;
} HydraSvmParam_t;


// EP
typedef struct
{
  struct
    {
      float      f0;                    /** < Центральная частота, кГц*/
      float      T;                     /** < Период, мс*/
      uint32_t   n0;                    /** < Количество импульсов */
      uint8_t    f_kvadro;              /** < флаг использования квадратурной оцифровки*/ // TODO добавить в .с
      uint8_t    reserv[3];
    } sART;                      /** < ART - Параметры аналогового тракта*/

  struct
    {
      uint8_t      use_temp;           /** < Включение логики работы температурной защиты*/
      uint8_t      reserv[3];
      float        operating_time;      /** < наработка времени */ // может это в control?
      float        max_temp;            /** < Максимальная рабочая температура*/
      float        max_sound_velocity;  /** максимальная скорость звука в воде */
      float        min_sound_velocity;  /** минимальная скорость (или время?)*/
    } sProtect;

  struct
    {
      float        base1;           /** < расстояние между излучателем и первой мишенью, мм*/
      float        base;           /** < база между мишенями, мм*/
      float        d_otv;          /** < диаметр отверстия в первой мишени, мм*/
      float        level_comp;     /** < соотношение уровней отражённого сигнала (зависит от параллельности мишеней и диаметра отверстия первой мишени)*/

      uint16_t     d_max;          /** TODO hydra-control.c*/ //?
      uint8_t      f_quadro_dig;   /** < признак квадратурной оцифровки (не реализовано)*/
      uint8_t      reserv;
      uint16_t     adc_zero;       /** < среднее положение сигнала (соответствует напряжению на усилителе)*/
      uint16_t     align2;          /** < Выравнивание данных, =0*/

    } sAJST;                       /** < Юстировочные данные*/

} HydraEpTxt_t;

HydraSvmParam_t*  hydra_svm_prm_get_addr(void);

HydraEpTxt_t* hydra_ep_txt_get_addr(void);
