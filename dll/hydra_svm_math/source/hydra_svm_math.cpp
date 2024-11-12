#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "hydra_svm_math.h"

uint16_t test_mass[1800] = { 2199,2205,2327,2310,2199,2185,2327,2322,2188,2188,2289,2334,2131,2179,2340,2338,2147,2130,2347,2316,2155,2120,2379,2361,2142,2135,2375,2384,2140,2146,2377,2349,2157,2176,2364,2338,2157,2182,2349,2355,2172,2176,439,577,2184,2196,2334,2348,1005,983,2356,2323,2248,2260,116,112,2238,2234,2284,2280,1610,1783,2280,2275,2256,2256,146,136,2255,2252,2272,2264,1088,1045,2276,2265,2256,2247,120,116,2252,2254,2276,2270,2376,2299,2266,2262,2230,2253,238,236,2231,2265,2240,2235,1069,1070,2258,2246,2232,2258,290,289,2304,2260,2219,2253,1547,1581,1104,1034,2281,2266,2112,2104,2672,2700,2239,2246,2203,2209,2579,2561,2287,2289,2352,2340,2109,2121,2186,2187,2140,2122,2058,2034,2329,2329,2387,2388,2355,2363,2183,2186,2255,2251,2263,2261,2315,2313,2180,2182,2182,2186,2201,2200,2226,2243,2294,2304,2339,2343,2391,2389,2370,2373,2307,2313,2229,2242,2141,2146,2095,2090,2219,2215,2283,2301,2290,2301,2267,2258,2233,2206,2198,2216,2218,2212,2180,2172,2163,2153,2161,2153,2190,2189,2198,2195,2252,2253,2297,2303,2353,2354,2307,2314,2274,2275,2220,2215,2212,2208,2227,2230,2284,2285,2293,2299,2300,2301,2275,2273,2304,2305,2281,2287,2264,2273,2207,2208,2204,2204,2157,2163,2185,2184,2194,2199,2242,2244,2263,2265,2269,2269,2242,2242,2207,2204,2182,2190,2193,2201,2213,2225,2243,2244,2248,2251,2263,2265,2284,2284,2314,2309,2299,2304,2288,2286,2252,2245,2239,2235,2214,2224,2244,2240,2262,2247,2287,2274,2267,2276,2265,2265,2247,2237,2220,2216,2201,2199,2210,2203,2204,2203,2207,2204,2214,2216,2232,2237,2252,2258,2266,2271,2268,2266,2246,2249,2233,2243,2230,2229,2235,2246,2252,2253,2269,2275,2284,2283,2287,2290,2277,2279,2264,2278,2290,2260,2267,2254,2247,2226,2226,2240,2215,2183,2224,2273,2240,2244,2254,2253,2253,2256,2238,2250,2223,2235,2215,2213,2218,2230,2221,2222,2238,2258,2249,2265,2266,2245,2233,2238,2243,2262,2226,2269,2237,2257,2234,2250,2240,2243,2252,2246,2259,2255,2265,2260,2274,2282,2272,2296,2315,2361,2018,2026,2740,2735,1531,1558,2945,2965,1409,1406,2974,3000,1434,1439,2918,2932,1486,1490,2898,2908,1568,1533,2831,2867,1541,1519,2842,2859,1542,1511,2904,2911,1483,1458,2969,2975,1468,1459,2986,2990,1476,1473,3010,3007,1430,1437,2989,2984,1405,1422,3041,3010,1414,1428,3036,3022,1418,1432,3020,3004,1403,1422,2983,2983,1393,1417,2983,2970,1395,1404,2971,2959,1410,1420,2970,2969,1450,1454,2987,2989,1446,1444,2965,2972,1427,1426,2968,2968,1461,1458,2993,2991,1454,1446,2974,2989,1462,1455,3003,3007,1451,1455,3004,3002,1423,1433,2941,2931,1580,1598,2659,2639,2042,2058,2333,2311,2206,2221,2270,2263,2178,2198,2314,2304,2146,2153,2341,2334,2103,2108,2385,2373,2093,2093,2397,2392,2134,2129,2339,2333,2195,2202,2285,2291,2230,2238,2264,2264,2236,2237,2269,2254,2229,2248,2238,2251,2257,2262,2246,2240,2260,2256,2238,2232,2245,2249,2244,2241,2242,2242,2236,2234,2253,2245,2224,2238,2249,2244,2257,2244,2247,2231,2262,2245,2222,2214,2252,2252,2220,2222,2269,2268,2221,2223,2274,2278,2226,2221,2301,2279,2226,2222,2267,2276,2221,2226,2277,2275,2226,2219,2276,2266,2248,2231,2277,2273,2251,2247,2255,2260,2236,2240,2255,2257,2236,2260,2253,2291,2233,2290,2247,2293,2237,2262,2240,2248,2251,2252,2215,2242,2232,2261,2236,2245,2254,2258,2236,2220,2242,2269,2240,2204,2246,2293,2253,2251,2251,2250,2251,2250,2244,2244,2252,2245,2242,2228,2251,2245,2253,2245,2249,2264,2253,2256,2248,2231,2242,2231,2251,2247,2247,2241,2244,2239,2246,2243,2227,2229,2257,2252,2238,2232,2256,2256,2235,2230,2252,2254,2236,2234,2253,2263,2258,2238,2247,2256,2235,2234,2258,2256,2237,2238,2244,2257,2246,2235,2249,2260,2248,2258,2231,2273,2253,2245,2283,2261,2244,2231,2247,2259,2216,2233,2247,2258,2234,2231,2254,2256,2232,2237,2249,2253,2238,2235,2249,2254,2236,2238,2248,2252,2234,2234,2252,2251,2231,2234,2267,2273,2232,2233,2269,2261,2229,2228,2276,2270,2231,2227,2272,2274,2221,2224,2279,2285,2220,2217,2282,2280,2217,2213,2282,2274,2210,2206,2280,2282,2214,2211,2253,2250,2243,2244,2233,2234,2251,2242,2265,2263,2195,2198,2296,2299,2208,2202,2278,2278,2194,2211,2256,2294,2194,2211,2289,2291,2184,2192,2299,2309,2189,2198,2308,2308,2194,2190,2311,2308,2186,2185,2313,2312,2188,2186,2319,2319,2185,2184,2304,2308,2186,2187,2325,2324,2191,2177,2328,2307,2185,2171,2330,2328,2167,2164,2317,2321,2163,2135,2333,2283,2179,2129,2322,2298,2169,2158,2326,2328,2182,2181,2316,2315,2182,2176,2320,2318,2185,2185,2309,2316,2189,2192,2310,2309,2196,2191,2294,2294,2209,2201,2295,2299,2202,2210,2288,2280,2199,2190,2314,2299,2176,2170,2338,2347,2170,2173,2296,2300,2206,2197,2260,2261,2245,2237,2271,2277,2196,2201,2278,2283,2229,2235,2265,2278,2233,2244,2250,2264,2236,2241,2263,2271,2236,2236,2257,2254,2235,2245,2253,2256,2246,2247,2243,2246,2255,2257,2222,2220,2319,2315,2009,2001,2794,2762,1323,1380,3482,3459,930,930,3683,3671,818,817,3755,3745,776,782,3786,3765,770,772,3787,3769,768,772,3779,3778,770,778,3774,3769,780,782,3763,3762,778,785,3767,3759,786,796,3757,3749,786,797,3750,3742,791,807,3766,3740,776,807,3773,3741,797,802,3746,3743,788,798,3757,3754,789,799,3767,3750,780,790,3762,3748,766,793,3775,3757,796,785,3761,3754,781,789,3766,3754,785,787,3763,3756,789,798,3761,3753,792,791,3772,3756,807,797,3770,3751,802,797,3758,3751,797,802,3738,3735,828,832,3645,3643,1069,1067,3047,3046,1756,1752,2584,2586,2066,2072,2270,2289,2298,2305,2156,2158,2379,2378,2104,2110,2311,2351,2128,2163,2302,2304,2201,2152,2305,2265,2215,2186,2294,2300,2206,2213,2285,2291,2209,2210,2279,2280,2231,2213,2283,2280,2209,2223,2278,2280,2218,2209,2289,2283,2210,2201,2290,2297,2192,2183,2308,2302,2188,2174,2307,2286,2194,2178,2302,2302,2201,2203,2296,2306,2204,2202,2294,2297,2200,2199,2291,2301,2207,2205,2299,2297,2210,2204,2302,2287,2208,2206,2301,2301,2198,2196,2302,2296,2197,2198,2304,2308,2199,2193,2299,2298,2186,2205,2281,2288,2203,2211,2292,2288,2213,2213,2280,2279,2215,2214,2288,2290,2212,2214,2298,2291,2208,2204,2298,2304,2201,2199,2298,2299,2205,2201,2308,2302,2213,2210,2278,2280,2225,2216,2288,2274,2230,2219,2262,2265,2233,2239,2261,2265,2242,2243,2257,2260,2239,2240,2269,2267,2240,2241,2266,2265,2244,2239,2258,2262,2240,2239,2255,2262,2242,2244,2259,2267,2234,2236,2260,2274,2231,2231,2272,2268,2230,2236,2266,2272,2235,2201,2263,2311,2247,2226,2244,2250,2235,2241,2255,2259,2246,2250,2251,2252,2257,2249,2264,2257,2251,2250,2250,2262,2240,2234,2234,2247,2213,2237,2242,2262,2230,2238,2264,2264,2229,2228,2269,2264,2240,2238,2250,2247,2246,2241,2252,2253,2259,2260,2254,2245,2253,2251,2214,2237,2262,2266,2226,2231,2259,2265,2229,2218,2304,2283,2232,2234,2279,2302,2259,2251,2215,2287,2265,2234,2290,2271,2242,2229,2262,2268,2211,2226,2283,2290,2194,2188,2377,2373,2023,2030,2546,2556,1850,1866,2664,2680,1750,1754,2807,2795,1645,1627,2875,2884,1580,1572,2926,2942,1547,1531,2932,2951,1541,1520,2960,2978,1531,1516,2955,2975,1512,1502,2975,2984,1515,1495,2978,2989,1513,1501,2983,2996,1490,1483,2984,2991,1502,1487,3004,3003,1484,1477,2990,2998,1482,1481,3013,3003,1498,1484,2997,2993,1483,1486,3000,3006,1478,1475,3005,3006,1485,1488,3006,3004,1478,1480,2999,2999,1485,1486,3001,3008,1495,1501,2993,3000,1497,1501,2997,2986,1482,1493,2971,2979,1503,1504,2985,2987,1499,1502,2970,2972,1529,1526,2896,2899,1665,1656,2738,2741,1870,1866,2542,2547,1979,1978,2461,2463,2106,2109,2359,2351,2168,2170,2303,2302,2213,2223,2289,2282,2226,2218,2278,2260,2237,2236,2255,2258,2248,2251,2244,2237,2265,2268,2235,2228,2273,2277,2226,2222,2273,2277,2223,2221,2280,2278,2214,2218,2282,2292,2218,2213,2300,2290,2208,2208,2303,2289,2205,2208,2280,2289,2200,2212,2284,2298,2225,2216,2266,2280,2221,2222,2287,2291,2218,2216,2278,2277,2246,2248,2226,2218,2295,2307,2199,2182,2295,2327,2176,2150,2336,2356,2176,2120,2356,2346,2197,2135,2339,2273,2182,2161,2310,2332,2184,2170,2318,2329,2188,2164,2302,2326,2190,2163,2308,2338,2203,2179,2322,2314,2192,2163,2325,2345,2172,2153,2322,2317,2185,2176,2327,2339,2176,2173,2312,2315,2184,2160,2332,2335,2167,2160,2338,2346,2173,2153,2340,2346,2157,2157,2331,2347,2177,2156,2356,2354,2139,2140,2318,2358,2118,2125,2372,2363,2137,2122,2372,2382,2136,2117,2360,2387,2142,2102,2391,2376,2123,2113,2374,2396,2093,2097,2399,2390,2110,2105,2368,2370,2144,2157,2350,2341,2179,2181,2318,2310,2190,2198,2282,2284,2243,2244,2260,2255,2241,2241,2262,2263,2235,2234,2275,2274,2233,2227,2273,2265,2230,2235,2272,2272,2231,2235,2274,2276,2232,2226,2278,2276,2223,2231,2280,2279,2215,2213,2289,2287,2212,2216,2295,2293,2202,2206,2311,2308,2170,2200,2295,2304,2207,2201,2304,2298,2192,2206,2286,2297,2221,2217,2291,2286,2220,2223,2278,2282,2225,2226,2277,2274,2229,2222,2280,2276,2222,2223,2285,2282,2219,2219,2285,2289,2214,2215,2291,2285,2224,2222,2285,2282,2203,2212,2253,2285,2166,2222,2237,2286,2193,2217,2273,2289,2229,2219,2280,2281,2227,2210,2284,2282,2224,2210,2286,2294,2219,2209,2285,2293,2213,2208,2294,2299,2211,2192,2311,2307,2194,2220,2324,2379,2180,2222,2317,2356,2158,2186,2312,2327,2167,2171,2314,2323,2187,2189,2314,2319,2185,2189,2311,2312,2203,2198,2311,2305,2193,2197,2292,2300,2203,2198,2313,2302,2203,2224,2299,2328 };
int16_t  test_workmass[1800];

//void hydra_svm_measure_data_prepare(const uint16_t* p_tripleconverted, uint16_t mas_size, int16_t* p_sredneeconverted);
//uint16_t hydra_svm_windows_finder(int16_t* p_workconverted, uint16_t* p_otr1, uint16_t* p_otr2);
// hydra_svm_vkf4(int16_t* p_workconverted, const uint16_t otr1, const uint16_t otr2);

int main(int argc, char* argv[])
{
    double vtime, vspeed;
    uint16_t otr1, otr2;
    hydra_svm_measure_data_prepare(test_mass, HYDRA_SVM_ADC_OUT_BUFF_SIZE, test_workmass);
    hydra_svm_windows_finder(test_workmass, &otr1, &otr2);
    vtime = hydra_svm_vkf4(test_workmass, otr1, otr2);
    vspeed = 0.001 * 2 * 34 / vtime;
    printf("time=%f, speed=%f\n", vtime * 1000000, vspeed);
    //system("PAUSE");	
    return 0;
}

/**
 * @brief ‘ункция подготавливает данные к расчЄту
 *
 * @param p_tripleconverted  - исходный массив измерений (с ј÷ѕ)
 * @param p_zeroconverted    - массив с измерениями без излучения (полка)
 * @param p_sredneeconverted - выходной массив с приведЄнными к 0 измерениями (с убранной полкой)
 */
void hydra_svm_measure_data_prepare(const uint16_t* p_tripleconverted, uint16_t mas_size, int16_t* p_sredneeconverted)
{
    short i1;

    for (i1 = 0; i1 < mas_size; i1++)
    {
        p_sredneeconverted[i1] = (p_tripleconverted[i1] - 2260);//-p_zeroconverted[i1]; TODO заполнить массив без излучения!!!!!

        // p_sredneeconverted[i1] = p_sredneeconverted[i1]>>2; // урезаем "шумный" бит, остаЄтся ещЄ 11 бит
    }
}

/**
 * @brief функция ищет отражения в оцифрованном сигнале
 *
 * @param p_workconverted       - оцифрованный сигнал
 * @param p_otr1                - индекс первого отражения
 * @param p_otr2                - индекс второго отражения
 * @return
 */
uint16_t
hydra_svm_windows_finder(int16_t* p_workconverted, uint16_t* p_otr1, uint16_t* p_otr2)
{
    int i, iwindow;
    uint16_t schetchik = 0;
    uint16_t otr1 = 0, otr2 = 0;

    // double A=0; // сумма амплитуд исследуемого сигнала
    uint16_t A1 = 0, A2 = 0; // суммарные самые большие амплитуды

    //HydraEpTxt_t* p_ep_txt = hydra_ep_txt_get_addr();
    //HydraSvmParam_t* p_svm_prm = hydra_svm_prm_get_addr();
    uint16_t massiv_size = HYDRA_SVM_ADC_OUT_BUFF_SIZE;
    double cnt2i = 12 / 3;
    uint16_t window_size = (uint16_t)(27 * (cnt2i));
    //float kbase = (p_hydra_ep_txt->sAJST.base1 + p_hydra_ep_txt->sAJST.base)/p_hydra_ep_txt->sAJST.base1;
    uint16_t detect_level2 = 500; // уровень сигнала (определяется делителем на положительном входе операционника) TODO из паспорта!
    uint16_t detect_level1 = 50; // уровень определения сигнала

    // сначала определяем уровень детектирования
    for (i = 200; i < massiv_size * 0.6; i++)
    {
        if (A1 < p_workconverted[i])
        {
            A1 = p_workconverted[i]; //max
            A2 = p_workconverted[i] + p_workconverted[i + 2] + A1 / 4;
        }
        detect_level1 = A1 / 3;
    }

    if (window_size > massiv_size)
        return 1;

    // считаем, что оцифровывается весь сигнал (с излучением)
    for (i = 200; i < massiv_size * 0.6; i++) // ищем первое отражение
    {
        if ((p_workconverted[i] > detect_level1)) // если уровень больше среднего на 100 (для 12бит) (или 20 для 8бит)
        {
            // A = 0;
            schetchik = 0;
            for (iwindow = 0; iwindow <= window_size; iwindow = iwindow + 4)
            {
                if (p_workconverted[iwindow + i] + p_workconverted[iwindow + i + 2] < A2)
                {
                    schetchik++;
                    //A = A + sqrt(p_workconverted[iwindow+i]*p_workconverted[iwindow+i] + p_workconverted[iwindow+i+1]*p_workconverted[iwindow+i+1]);
                }// if iwindow
            }// for iwindow
            if (schetchik >= 27 - 2)
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
    for (i = otr1 * 2.4; i < massiv_size; i++) // ищем второе отражение
    {
        if ((p_workconverted[i] > detect_level2)) // если уровень больше среднего на 100 (для 12бит) (или 20 для 8бит)
        {
            // A = 0;
            schetchik = 0;
            for (iwindow = 0; iwindow <= window_size; iwindow = iwindow + 4)
            {
                if (p_workconverted[iwindow + i] + p_workconverted[iwindow + i + 2] < 80)
                {
                    schetchik++;
                    //A = A + sqrt(p_workconverted[iwindow+i]*p_workconverted[iwindow+i] + p_workconverted[iwindow+i+1]*p_workconverted[iwindow+i+1]);
                }// if iwindow
            }// for iwindow
            if (schetchik >= 27 - 2)
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

    *p_otr1 = otr1;
    *p_otr2 = otr2;


    if (*p_otr1 > 0 && *p_otr2 > 0)
        return 0;

    return 1; /* ошибка */
}

double
hydra_svm_vkf4(int16_t* p_workconverted, const uint16_t otr1, const uint16_t otr2)
{
  uint16_t periodi = (uint16_t)(12/3);
  uint16_t window_size = (27*periodi + 24); // сигнал с хвостом
  uint16_t massiv_size = HYDRA_SVM_ADC_OUT_BUFF_SIZE;
  Hydra_Svm_Complex16_t vkf_mass[HYDRA_SVM_ADC_OUT_BUFF_SIZE];
//  double        vkf_amplitude_mass[HYDRA_SVM_ADC_OUT_BUFF_SIZE];
//  double        vkf_phase_mass[HYDRA_SVM_ADC_OUT_BUFF_SIZE];
  uint64_t        vkf_current_abs[HYDRA_SVM_ADC_OUT_BUFF_SIZE];
  uint64_t        vkf_max=0;

  Hydra_Svm_Complex32_t vkf_current;
  Hydra_Svm_Complex32_t vkf_current_sum[HYDRA_SVM_ADC_OUT_BUFF_SIZE];
  Hydra_Svm_Complex32_t vkf_current_max = { 0, 0 };

  uint16_t i,i2;
  int16_t i1, otr1_zapas = otr1 - 12;
  uint16_t i_diff;
  uint16_t i_diff_periodi = 0;
  double        faza;

  // проверки на всякий случай
  if(otr1 < 12 || otr2 < 24)
  {
    return -1;//отражения не найдены!
  }
  if(otr2+window_size+12 > massiv_size)
  {
    return -2;// слишком маленькая скорость
  }
  if(otr1 > otr2)
  {
    return -3;
  }
  // были проверки на всякий случай
  // создаём временный массив комплексных чисел
  for(i = 0; i<massiv_size-5;i=i+4)
  {
    vkf_mass[i].Re   =    p_workconverted[i  ];//1
    vkf_mass[i].Im   =    p_workconverted[i+1];
    vkf_mass[i+1].Re = -1*p_workconverted[i+2];//2
    vkf_mass[i+1].Im =    p_workconverted[i+1];
    vkf_mass[i+2].Re = -1*p_workconverted[i+2];//3
    vkf_mass[i+2].Im = -1*p_workconverted[i+3];
    vkf_mass[i+3].Re =    p_workconverted[i+4];//4
    vkf_mass[i+3].Im = -1*p_workconverted[i+3];
  }


  memset(vkf_current_abs,0,HYDRA_SVM_ADC_OUT_BUFF_SIZE*sizeof(uint64_t));
  memset(vkf_current_sum,0,HYDRA_SVM_ADC_OUT_BUFF_SIZE*sizeof(Hydra_Svm_Complex32_t));
  // сама автокорреляционная функция
  for(i2=otr2-24; i2<massiv_size-window_size-4; ++i2)
  {
    memset(&vkf_current,0,sizeof(Hydra_Svm_Complex32_t));
    for(i1=0;i1<window_size;++i1)
    {
      vkf_current.Re = vkf_current.Re + ((vkf_mass[i1+otr1_zapas].Re*vkf_mass[i2+i1].Re)/pow(2,ADC_WIDTH-1) + (vkf_mass[i1+otr1_zapas].Im*vkf_mass[i2+i1].Im)/pow(2,ADC_WIDTH-1));
      vkf_current.Im = vkf_current.Im + ((vkf_mass[i1+otr1_zapas].Re*vkf_mass[i2+i1].Im)/pow(2,ADC_WIDTH-1) - (vkf_mass[i1+otr1_zapas].Im*vkf_mass[i2+i1].Re)/pow(2,ADC_WIDTH-1));

    } // for i1

    vkf_current_sum[i2].Re = vkf_current.Re;
    vkf_current_sum[i2].Im = vkf_current.Im;
    //vkf_current_abs[i2] = (uint32_t)sqrt( (double)(vkf_current_sum[i2].Re)*(double)(vkf_current_sum[i2].Re )+ (double)(vkf_current_sum[i2].Im)*(double)(vkf_current_sum[i2].Im) );
    //vkf_current_abs[i2] = sqrt_newton( vkf_current_sum[i2].Re*vkf_current_sum[i2].Re + vkf_current_sum[i2].Im*vkf_current_sum[i2].Im );
    //vkf_current_abs[i2] = (uint32_t)hypot( (double)vkf_current_sum[i2].Re,(double)vkf_current_sum[i2].Im );
    vkf_current_abs[i2] = ((int64_t)vkf_current.Re*(int64_t)vkf_current.Re +
                           (int64_t)vkf_current.Im*(int64_t)vkf_current.Im) ;

  }// for i2


  // ищем максимум
  for(i=otr2-24;i<massiv_size-window_size;++i)
  {
    if(vkf_max<vkf_current_abs[i])
    {
      vkf_max = vkf_current_abs[i];
      memcpy(&vkf_current_max,&vkf_current_sum[i],sizeof(Hydra_Svm_Complex32_t));
      i_diff = (i-otr1_zapas);
      i_diff_periodi = (uint16_t)(i_diff/periodi);

                //printf("%d max%d\n",i,vkf_max);
    }// if vkf_max
    //printf("  %d %d  \n",i,vkf_current_abs[i]);
  }// for i

  //после того, как нашли максимальное a b, берем atan(b/a) в максимальном месте
  faza = atan2(((double)vkf_current_max.Im),((double)vkf_current_max.Re));
  if (faza < 0)
  {
    i_diff_periodi++;
  }

  return (i_diff_periodi + faza/(2.0*M_PI))*(1.0/(double)FREQUENCY_CENTRAL);
}

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
    uint16_t              index_xcorr_max = 0;

    uint16_t reserve = 8; // 0
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

    uint16_t index_xcorr_max_f = 0;
    Hydra_Svm_Complex32_t* s1;
    Hydra_Svm_Complex32_t* s2;
    float s1_abs_f[100] = { 0 };
    float s2_abs_f[100] = { 0 };

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
        s1 = &data_complex[otr1 - reserve];
        s2 = &data_complex[otr2 - reserve];

        //uint16_t index_xcorr_max_f = hydra_xcorr_real_v0(s1, s2, 2 * reserve);
        index_xcorr_max_f = hydra_xcorr_real_v1(s1, s2, &s1_abs_f[0], &s2_abs_f[0], (float)0.2, (float)0.6, 2 * reserve);
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

//uint16_t
//hydra_xcorr_real(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, uint16_t window_size)
//{
//    uint16_t index_xcorr_max = 0;
//
//    int64_t xcorr_current_abs = 0;
//    Hydra_Svm_Complex32_t xcorr_current;
//    Hydra_Svm_Complex32_t signal_windows_1;
//    Hydra_Svm_Complex32_t signal_windows_2;
//    uint64_t xcorr_max_abs = 0;
//
//    // вычисленеие корреляционной функции
//    for (uint16_t i = 0; i < window_size - 1; i++)
//    {
//        xcorr_current.Im = 0;
//        xcorr_current.Re = 0;
//        //memset(xcorr_current,0,sizeof(Hydra_Svm_Complex64_t));
//        xcorr_current_abs = 0;
//        for (uint16_t j = 0; j < (window_size - i); j++)
//        {
//            if ((i + j) < (window_size - 1))
//            {
//                // выборка сигнала в первом окне
//                signal_windows_1 = s1[j];
//                // выборка сигнала во втором окне
//                signal_windows_2 = s2[i + j];
//                // накапливаем и перемножение signal_windows_1
//                // с комплексносопряжённым signal_windows_2
//                xcorr_current.Re = xcorr_current.Re +
//                    ((signal_windows_1.Re * signal_windows_2.Re) +
//                        (signal_windows_1.Im * signal_windows_2.Im));
//                xcorr_current.Im = xcorr_current.Im +
//                    ((signal_windows_1.Re * signal_windows_2.Im) -
//                        (signal_windows_2.Re * signal_windows_1.Im));
//            }
//        }
//        // вычисляем амплитуду (квадрат амплитуды) корреляционной функции
//        xcorr_current_abs = (int64_t)xcorr_current.Re * xcorr_current.Re +
//            (int64_t)xcorr_current.Im * xcorr_current.Im;
//        // квадратный корень не берём - нам нужно только найти максимум
//        // ищем максимум
//        if (xcorr_current_abs > xcorr_max_abs)
//        {
//            // сохраняем данные для максимума
//            index_xcorr_max = i;
//            xcorr_max_abs = xcorr_current_abs;
//        }
//    }
//
//    return index_xcorr_max;
//}

//uint16_t
//hydra_xcorr_real(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, uint16_t window_size)
//{
//
//    uint16_t index_start_1 = 0;
//    uint16_t index_start_2 = 0;
//    window_size = window_size / 2;
//
//    Hydra_Svm_Complex32_t diff_s1[(370 + 120) / 2] = { 0 };
//    Hydra_Svm_Complex32_t diff_s2[(370 + 120) / 2] = { 0 };
//
//    for (uint16_t i = 0; i < window_size - 1; i++)
//    {
//        uint16_t inx_1_1 = index_start_1 + 2 * (i + 1);
//        uint16_t inx_1_2 = index_start_1 + 2 * i;
//        uint16_t inx_2_1 = index_start_2 + 2 * (i + 1);
//        uint16_t inx_2_2 = index_start_2 + 2 * i;
//
//        diff_s1[i].Re = s1[inx_1_1].Re - s1[inx_1_2].Re;
//        diff_s1[i].Im = s1[inx_1_1].Im - s1[inx_1_2].Im;
//        diff_s2[i].Re = s2[inx_2_1].Re - s2[inx_2_2].Re;
//        diff_s2[i].Im = s2[inx_2_1].Im - s2[inx_2_2].Im;
//    }
//
//    Hydra_Svm_Complex32_t xcorr_max;
//    uint16_t              index_xcorr_max = 0;
//
//    int64_t xcorr_current_abs = 0;
//    Hydra_Svm_Complex32_t xcorr_current;
//    Hydra_Svm_Complex32_t signal_windows_1;
//    Hydra_Svm_Complex32_t signal_windows_2;
//    uint64_t xcorr_max_abs = 0;
//
//    // вычисленеие корреляционной функции
//    for (uint16_t i = 0; i < window_size - 1; i++)
//    {
//        xcorr_current.Im = 0;
//        xcorr_current.Re = 0;
//        //memset(xcorr_current,0,sizeof(Hydra_Svm_Complex64_t));
//        xcorr_current_abs = 0;
//        for (uint16_t j = 0; j < (window_size - i); j++)
//        {
//            if ((i + j) < (window_size - 1))
//            {
//                // выборка сигнала в первом окне
//                signal_windows_1 = diff_s1[j];
//                // выборка сигнала во втором окне
//                signal_windows_2 = diff_s2[i + j];
//                // накапливаем и перемножение signal_windows_1
//                // с комплексносопряжённым signal_windows_2
//                xcorr_current.Re = xcorr_current.Re +
//                    ((signal_windows_1.Re * signal_windows_2.Re) +
//                        (signal_windows_1.Im * signal_windows_2.Im));
//                xcorr_current.Im = xcorr_current.Im +
//                    ((signal_windows_1.Re * signal_windows_2.Im) -
//                        (signal_windows_2.Re * signal_windows_1.Im));
//            }
//        }
//        // вычисляем амплитуду (квадрат амплитуды) корреляционной функции
//        xcorr_current_abs = (int64_t)xcorr_current.Re * xcorr_current.Re +
//            (int64_t)xcorr_current.Im * xcorr_current.Im;
//        // квадратный корень не берём - нам нужно только найти максимум
//        // ищем максимум
//        if (xcorr_current_abs > xcorr_max_abs)
//        {
//            // сохраняем данные для максимума
//            index_xcorr_max = 2 * i;
//            xcorr_max.Re = xcorr_current.Re;
//            xcorr_max.Im = xcorr_current.Im;
//            xcorr_max_abs = xcorr_current_abs;
//        }
//    }
//
//    return index_xcorr_max;
//}


uint16_t
hydra_xcorr_real_abs(int32_t* s1_abs, int32_t* s2_abs, uint16_t window_size)
{
    uint16_t index_xcorr_min = 0;

    int64_t  xcorr_current_abs = 0;
    int64_t  xcorr_min_abs = 10000000000000000;

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

uint16_t
hydra_xcorr_real_v1(Hydra_Svm_Complex32_t* s1, Hydra_Svm_Complex32_t* s2, 
                    float* s1_abs_f, float* s2_abs_f,
                    float trh_1, float trh_2, uint16_t window_size)
{
    if (trh_1 > trh_2)
    {
        return 0;
    }

    int64_t  s1_abs[100] = { 0 };
    int64_t  s2_abs[100] = { 0 };

    int64_t  s1_abs_max = 0;
    int64_t  s2_abs_max = 0;

    // коэффициенты для нормирования сигналов (приводим в диапахон [0 1])
    float coeff_norm_s1 = (float)1.0 / (float)s1_abs_max;
    float coeff_norm_s2 = (float)1.0 / (float)s2_abs_max;

    // ищем индекс и значение для порога trh_2
    int16_t inx_s1_trh2 = 0;
    int16_t inx_s2_trh2 = 0;
    float min_delta_trh2_s1 = 1.0;
    float min_delta_trh2_s2 = 1.0;

    for (uint16_t i = 0; i < window_size - 1; i++)
    {
        s1_abs[i] = (int64_t)sqrt(s1[i].Re * s1[i].Re + s1[i].Im * s1[i].Im);
        s2_abs[i] = (int64_t)sqrt(s2[i].Re * s2[i].Re + s2[i].Im * s2[i].Im);
        s2_abs[window_size + i] =
            (int64_t)sqrt(s2[window_size + i].Re * s2[window_size + i].Re +
                s2[window_size + i].Im * s2[window_size + i].Im);
        s1_abs_max = (s1_abs_max > s1_abs[i]) ? s1_abs_max : s1_abs[i];
        s2_abs_max = (s2_abs_max > s2_abs[i]) ? s2_abs_max : s2_abs[i];
        s2_abs_max = (s2_abs_max > s2_abs[window_size + i]) ? s2_abs_max : s2_abs[window_size + i];

        // для массива s1
        if (hydra_norm_and_trh(&s1_abs[i], &s1_abs_f[i], trh_2,
            &min_delta_trh2_s1, coeff_norm_s1))
        {
            inx_s1_trh2 = i;
        }

        // для массива s2
        if (hydra_norm_and_trh(&s2_abs[i], &s2_abs_f[i], trh_2,
            &min_delta_trh2_s2, coeff_norm_s2))
        {
            inx_s2_trh2 = i;
        }
        if (hydra_norm_and_trh(&s2_abs[window_size + i], &s2_abs_f[window_size + i], trh_2,
            &min_delta_trh2_s2, coeff_norm_s2))
        {
            inx_s2_trh2 = window_size + i;
        }
    }

    // ищем индекс и значение для порога trh_1
    int64_t abs_s1_trh2 = s1_abs[inx_s1_trh2];
    int64_t abs_s2_trh2 = s2_abs[inx_s2_trh2];
    int64_t abs_s1_trh1 = abs_s1_trh2;
    int64_t abs_s2_trh1 = abs_s2_trh2;
    int16_t inx_s1_trh1 = 0;
    int16_t inx_s2_trh1 = 0;
    while ((abs_s1_trh1 > trh_1) && (inx_s1_trh1 >= 0))
    {
        if (inx_s1_trh1 == 0)
        {
            break;
        }
        inx_s1_trh1 = inx_s1_trh1 - 1;
        abs_s1_trh1 = s1_abs[inx_s1_trh2 + inx_s1_trh1];
    }
    while ((abs_s2_trh1 > trh_1) && (inx_s2_trh1 >= 0))
    {
        if (inx_s2_trh1 == 0)
        {
            break;
        }
        inx_s2_trh1 = inx_s2_trh1 - 1;
        abs_s2_trh1 = s2_abs[inx_s2_trh2 + inx_s2_trh1];
    }

    float inx_s1_0 = (float)inx_s1_trh1 +
            (float)( inx_s1_trh1 * abs_s1_trh1) / (float)(abs_s1_trh2 - abs_s1_trh1);
    float inx_s2_0 = (float)inx_s2_trh1 +
            (float)( inx_s2_trh1 * abs_s1_trh1) / (float)(abs_s2_trh2 - abs_s2_trh1);

    return uint16_t(round(inx_s2_0 - inx_s1_0)) + (inx_s2_trh2 - inx_s1_trh2);
}

bool
hydra_norm_and_trh(int64_t*  s_abs, float* s_abs_f, float trh,
                    float* min_delta_trh, float coeff_norm)
{
    // нормируем
    *s_abs_f = coeff_norm * (float)*s_abs;
    // ищем ближайшее к trh2 значение
    float delta = (trh > *s_abs_f) ? (trh - *s_abs_f) : (*s_abs_f - trh);
    if (delta < *min_delta_trh) {
        *min_delta_trh = delta; // запоминаем новую min delta
        return true; // признак изменения значений
    }
    return false;
}