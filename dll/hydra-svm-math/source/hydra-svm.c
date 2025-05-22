#include "hydra-svm.h"

HydraSvmParam_t    svm_prm;
HydraSvmParam_t*   p_svm_prm = &svm_prm;
HydraEpTxt_t       hydra_ep_txt;

/* HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
*/
HydraSvmParam_t*
hydra_svm_prm_get_addr(void)
{
  p_svm_prm->sADC.imp_cnt = 27;
  p_svm_prm->sADC.gen_freq = 3000000;
  p_svm_prm->sADC.adc_freq = 12000000;
  return p_svm_prm;
}

/**
 * @brief Функция возврящает адрес структуры текстового паспорта
 *
 * Пример:
   HydraEpTxt_t* p_hydra_ep_txt = hydra_ep_txt_get_addr();
 *
 * @return      адрес структуры текстового паспорта
 */
HydraEpTxt_t*
hydra_ep_txt_get_addr(void)
{
    hydra_ep_txt.sAJST.base1 = 24; // расстояние до первой мишени
    hydra_ep_txt.sAJST.base = 34;  // расстояние между мишенями
    return &hydra_ep_txt;
}