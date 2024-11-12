//#include <cstdint>

#define uint8_t unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t  unsigned long int


#define int8_t  char
#define int16_t short
#define int32_t int
#define int64_t long int

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

typedef struct  Hydra_out_xcorr_t 
{
    uint16_t index_time_propagation;
    float    phase;
    float    abs;
}Hydra_out_xcorr_t;

#define bool unsigned char
