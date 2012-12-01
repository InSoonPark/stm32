/* ----------------------------------------------------------------------
* Copyright (C) 2010 ARM Limited. All rights reserved.
*
* $Date:        15. July 2011
* $Revision: 	V1.0.10
*
* Project: 	    CMSIS DSP Library
* Title:	    arm_cfft_radix4_init_q15.c
*
* Description:	Radix-4 Decimation in Frequency Q15 FFT & IFFT initialization function
*
* Target Processor: Cortex-M4/Cortex-M3/Cortex-M0
*
* Version 1.0.10 2011/7/15
*    Big Endian support added and Merged M0 and M3/M4 Source code.
*
* Version 1.0.3 2010/11/29
*    Re-organized the CMSIS folders and updated documentation.
*
* Version 1.0.2 2010/11/11
*    Documentation updated.
*
* Version 1.0.1 2010/10/05
*    Production release and review comments incorporated.
*
* Version 1.0.0 2010/09/20
*    Production release and review comments incorporated.
*
* Version 0.0.5  2010/04/26
* 	 incorporated review comments and updated with latest CMSIS layer
*
* Version 0.0.3  2010/03/10
*    Initial version
* -------------------------------------------------------------------- */

#include "arm_math.h"
#include "arm_common_tables.h"

/**
 * @ingroup groupTransforms
 */


/**
 * @addtogroup CFFT_CIFFT
 * @{
 */

/*
* @brief  Twiddle factors Table
*/

/**
* \par
* Example code for Q15 Twiddle factors Generation::
* \par
* <pre>for(i = 0; i< N; i++)
* {
*	twiddleCoefQ15[2*i]= cos(i * 2*PI/(float)N);
*	twiddleCoefQ15[2*i+1]= sin(i * 2*PI/(float)N);
* } </pre>
* \par
* where N = 1024	and PI = 3.14159265358979
* \par
* Cos and Sin values are interleaved fashion
* \par
* Convert Floating point to Q15(Fixed point 1.15):
*	round(twiddleCoefQ15(i) * pow(2, 15))
*
*/

static const q15_t twiddleCoefQ15[2048] = {
  0x7fff, 0x0, 0x7fff, 0xc9, 0x7ffe, 0x192, 0x7ffa, 0x25b,
  0x7ff6, 0x324, 0x7ff1, 0x3ed, 0x7fea, 0x4b6, 0x7fe2, 0x57f,
  0x7fd9, 0x648, 0x7fce, 0x711, 0x7fc2, 0x7d9, 0x7fb5, 0x8a2,
  0x7fa7, 0x96b, 0x7f98, 0xa33, 0x7f87, 0xafb, 0x7f75, 0xbc4,
  0x7f62, 0xc8c, 0x7f4e, 0xd54, 0x7f38, 0xe1c, 0x7f22, 0xee4,
  0x7f0a, 0xfab, 0x7ef0, 0x1073, 0x7ed6, 0x113a, 0x7eba, 0x1201,
  0x7e9d, 0x12c8, 0x7e7f, 0x138f, 0x7e60, 0x1455, 0x7e3f, 0x151c,
  0x7e1e, 0x15e2, 0x7dfb, 0x16a8, 0x7dd6, 0x176e, 0x7db1, 0x1833,
  0x7d8a, 0x18f9, 0x7d63, 0x19be, 0x7d3a, 0x1a83, 0x7d0f, 0x1b47,
  0x7ce4, 0x1c0c, 0x7cb7, 0x1cd0, 0x7c89, 0x1d93, 0x7c5a, 0x1e57,
  0x7c2a, 0x1f1a, 0x7bf9, 0x1fdd, 0x7bc6, 0x209f, 0x7b92, 0x2162,
  0x7b5d, 0x2224, 0x7b27, 0x22e5, 0x7aef, 0x23a7, 0x7ab7, 0x2467,
  0x7a7d, 0x2528, 0x7a42, 0x25e8, 0x7a06, 0x26a8, 0x79c9, 0x2768,
  0x798a, 0x2827, 0x794a, 0x28e5, 0x790a, 0x29a4, 0x78c8, 0x2a62,
  0x7885, 0x2b1f, 0x7840, 0x2bdc, 0x77fb, 0x2c99, 0x77b4, 0x2d55,
  0x776c, 0x2e11, 0x7723, 0x2ecc, 0x76d9, 0x2f87, 0x768e, 0x3042,
  0x7642, 0x30fc, 0x75f4, 0x31b5, 0x75a6, 0x326e, 0x7556, 0x3327,
  0x7505, 0x33df, 0x74b3, 0x3497, 0x7460, 0x354e, 0x740b, 0x3604,
  0x73b6, 0x36ba, 0x735f, 0x3770, 0x7308, 0x3825, 0x72af, 0x38d9,
  0x7255, 0x398d, 0x71fa, 0x3a40, 0x719e, 0x3af3, 0x7141, 0x3ba5,
  0x70e3, 0x3c57, 0x7083, 0x3d08, 0x7023, 0x3db8, 0x6fc2, 0x3e68,
  0x6f5f, 0x3f17, 0x6efb, 0x3fc6, 0x6e97, 0x4074, 0x6e31, 0x4121,
  0x6dca, 0x41ce, 0x6d62, 0x427a, 0x6cf9, 0x4326, 0x6c8f, 0x43d1,
  0x6c24, 0x447b, 0x6bb8, 0x4524, 0x6b4b, 0x45cd, 0x6add, 0x4675,
  0x6a6e, 0x471d, 0x69fd, 0x47c4, 0x698c, 0x486a, 0x691a, 0x490f,
  0x68a7, 0x49b4, 0x6832, 0x4a58, 0x67bd, 0x4afb, 0x6747, 0x4b9e,
  0x66d0, 0x4c40, 0x6657, 0x4ce1, 0x65de, 0x4d81, 0x6564, 0x4e21,
  0x64e9, 0x4ec0, 0x646c, 0x4f5e, 0x63ef, 0x4ffb, 0x6371, 0x5098,
  0x62f2, 0x5134, 0x6272, 0x51cf, 0x61f1, 0x5269, 0x616f, 0x5303,
  0x60ec, 0x539b, 0x6068, 0x5433, 0x5fe4, 0x54ca, 0x5f5e, 0x5560,
  0x5ed7, 0x55f6, 0x5e50, 0x568a, 0x5dc8, 0x571e, 0x5d3e, 0x57b1,
  0x5cb4, 0x5843, 0x5c29, 0x58d4, 0x5b9d, 0x5964, 0x5b10, 0x59f4,
  0x5a82, 0x5a82, 0x59f4, 0x5b10, 0x5964, 0x5b9d, 0x58d4, 0x5c29,
  0x5843, 0x5cb4, 0x57b1, 0x5d3e, 0x571e, 0x5dc8, 0x568a, 0x5e50,
  0x55f6, 0x5ed7, 0x5560, 0x5f5e, 0x54ca, 0x5fe4, 0x5433, 0x6068,
  0x539b, 0x60ec, 0x5303, 0x616f, 0x5269, 0x61f1, 0x51cf, 0x6272,
  0x5134, 0x62f2, 0x5098, 0x6371, 0x4ffb, 0x63ef, 0x4f5e, 0x646c,
  0x4ec0, 0x64e9, 0x4e21, 0x6564, 0x4d81, 0x65de, 0x4ce1, 0x6657,
  0x4c40, 0x66d0, 0x4b9e, 0x6747, 0x4afb, 0x67bd, 0x4a58, 0x6832,
  0x49b4, 0x68a7, 0x490f, 0x691a, 0x486a, 0x698c, 0x47c4, 0x69fd,
  0x471d, 0x6a6e, 0x4675, 0x6add, 0x45cd, 0x6b4b, 0x4524, 0x6bb8,
  0x447b, 0x6c24, 0x43d1, 0x6c8f, 0x4326, 0x6cf9, 0x427a, 0x6d62,
  0x41ce, 0x6dca, 0x4121, 0x6e31, 0x4074, 0x6e97, 0x3fc6, 0x6efb,
  0x3f17, 0x6f5f, 0x3e68, 0x6fc2, 0x3db8, 0x7023, 0x3d08, 0x7083,
  0x3c57, 0x70e3, 0x3ba5, 0x7141, 0x3af3, 0x719e, 0x3a40, 0x71fa,
  0x398d, 0x7255, 0x38d9, 0x72af, 0x3825, 0x7308, 0x3770, 0x735f,
  0x36ba, 0x73b6, 0x3604, 0x740b, 0x354e, 0x7460, 0x3497, 0x74b3,
  0x33df, 0x7505, 0x3327, 0x7556, 0x326e, 0x75a6, 0x31b5, 0x75f4,
  0x30fc, 0x7642, 0x3042, 0x768e, 0x2f87, 0x76d9, 0x2ecc, 0x7723,
  0x2e11, 0x776c, 0x2d55, 0x77b4, 0x2c99, 0x77fb, 0x2bdc, 0x7840,
  0x2b1f, 0x7885, 0x2a62, 0x78c8, 0x29a4, 0x790a, 0x28e5, 0x794a,
  0x2827, 0x798a, 0x2768, 0x79c9, 0x26a8, 0x7a06, 0x25e8, 0x7a42,
  0x2528, 0x7a7d, 0x2467, 0x7ab7, 0x23a7, 0x7aef, 0x22e5, 0x7b27,
  0x2224, 0x7b5d, 0x2162, 0x7b92, 0x209f, 0x7bc6, 0x1fdd, 0x7bf9,
  0x1f1a, 0x7c2a, 0x1e57, 0x7c5a, 0x1d93, 0x7c89, 0x1cd0, 0x7cb7,
  0x1c0c, 0x7ce4, 0x1b47, 0x7d0f, 0x1a83, 0x7d3a, 0x19be, 0x7d63,
  0x18f9, 0x7d8a, 0x1833, 0x7db1, 0x176e, 0x7dd6, 0x16a8, 0x7dfb,
  0x15e2, 0x7e1e, 0x151c, 0x7e3f, 0x1455, 0x7e60, 0x138f, 0x7e7f,
  0x12c8, 0x7e9d, 0x1201, 0x7eba, 0x113a, 0x7ed6, 0x1073, 0x7ef0,
  0xfab, 0x7f0a, 0xee4, 0x7f22, 0xe1c, 0x7f38, 0xd54, 0x7f4e,
  0xc8c, 0x7f62, 0xbc4, 0x7f75, 0xafb, 0x7f87, 0xa33, 0x7f98,
  0x96b, 0x7fa7, 0x8a2, 0x7fb5, 0x7d9, 0x7fc2, 0x711, 0x7fce,
  0x648, 0x7fd9, 0x57f, 0x7fe2, 0x4b6, 0x7fea, 0x3ed, 0x7ff1,
  0x324, 0x7ff6, 0x25b, 0x7ffa, 0x192, 0x7ffe, 0xc9, 0x7fff,
  0x0, 0x7fff, 0xff37, 0x7fff, 0xfe6e, 0x7ffe, 0xfda5, 0x7ffa,
  0xfcdc, 0x7ff6, 0xfc13, 0x7ff1, 0xfb4a, 0x7fea, 0xfa81, 0x7fe2,
  0xf9b8, 0x7fd9, 0xf8ef, 0x7fce, 0xf827, 0x7fc2, 0xf75e, 0x7fb5,
  0xf695, 0x7fa7, 0xf5cd, 0x7f98, 0xf505, 0x7f87, 0xf43c, 0x7f75,
  0xf374, 0x7f62, 0xf2ac, 0x7f4e, 0xf1e4, 0x7f38, 0xf11c, 0x7f22,
  0xf055, 0x7f0a, 0xef8d, 0x7ef0, 0xeec6, 0x7ed6, 0xedff, 0x7eba,
  0xed38, 0x7e9d, 0xec71, 0x7e7f, 0xebab, 0x7e60, 0xeae4, 0x7e3f,
  0xea1e, 0x7e1e, 0xe958, 0x7dfb, 0xe892, 0x7dd6, 0xe7cd, 0x7db1,
  0xe707, 0x7d8a, 0xe642, 0x7d63, 0xe57d, 0x7d3a, 0xe4b9, 0x7d0f,
  0xe3f4, 0x7ce4, 0xe330, 0x7cb7, 0xe26d, 0x7c89, 0xe1a9, 0x7c5a,
  0xe0e6, 0x7c2a, 0xe023, 0x7bf9, 0xdf61, 0x7bc6, 0xde9e, 0x7b92,
  0xdddc, 0x7b5d, 0xdd1b, 0x7b27, 0xdc59, 0x7aef, 0xdb99, 0x7ab7,
  0xdad8, 0x7a7d, 0xda18, 0x7a42, 0xd958, 0x7a06, 0xd898, 0x79c9,
  0xd7d9, 0x798a, 0xd71b, 0x794a, 0xd65c, 0x790a, 0xd59e, 0x78c8,
  0xd4e1, 0x7885, 0xd424, 0x7840, 0xd367, 0x77fb, 0xd2ab, 0x77b4,
  0xd1ef, 0x776c, 0xd134, 0x7723, 0xd079, 0x76d9, 0xcfbe, 0x768e,
  0xcf04, 0x7642, 0xce4b, 0x75f4, 0xcd92, 0x75a6, 0xccd9, 0x7556,
  0xcc21, 0x7505, 0xcb69, 0x74b3, 0xcab2, 0x7460, 0xc9fc, 0x740b,
  0xc946, 0x73b6, 0xc890, 0x735f, 0xc7db, 0x7308, 0xc727, 0x72af,
  0xc673, 0x7255, 0xc5c0, 0x71fa, 0xc50d, 0x719e, 0xc45b, 0x7141,
  0xc3a9, 0x70e3, 0xc2f8, 0x7083, 0xc248, 0x7023, 0xc198, 0x6fc2,
  0xc0e9, 0x6f5f, 0xc03a, 0x6efb, 0xbf8c, 0x6e97, 0xbedf, 0x6e31,
  0xbe32, 0x6dca, 0xbd86, 0x6d62, 0xbcda, 0x6cf9, 0xbc2f, 0x6c8f,
  0xbb85, 0x6c24, 0xbadc, 0x6bb8, 0xba33, 0x6b4b, 0xb98b, 0x6add,
  0xb8e3, 0x6a6e, 0xb83c, 0x69fd, 0xb796, 0x698c, 0xb6f1, 0x691a,
  0xb64c, 0x68a7, 0xb5a8, 0x6832, 0xb505, 0x67bd, 0xb462, 0x6747,
  0xb3c0, 0x66d0, 0xb31f, 0x6657, 0xb27f, 0x65de, 0xb1df, 0x6564,
  0xb140, 0x64e9, 0xb0a2, 0x646c, 0xb005, 0x63ef, 0xaf68, 0x6371,
  0xaecc, 0x62f2, 0xae31, 0x6272, 0xad97, 0x61f1, 0xacfd, 0x616f,
  0xac65, 0x60ec, 0xabcd, 0x6068, 0xab36, 0x5fe4, 0xaaa0, 0x5f5e,
  0xaa0a, 0x5ed7, 0xa976, 0x5e50, 0xa8e2, 0x5dc8, 0xa84f, 0x5d3e,
  0xa7bd, 0x5cb4, 0xa72c, 0x5c29, 0xa69c, 0x5b9d, 0xa60c, 0x5b10,
  0xa57e, 0x5a82, 0xa4f0, 0x59f4, 0xa463, 0x5964, 0xa3d7, 0x58d4,
  0xa34c, 0x5843, 0xa2c2, 0x57b1, 0xa238, 0x571e, 0xa1b0, 0x568a,
  0xa129, 0x55f6, 0xa0a2, 0x5560, 0xa01c, 0x54ca, 0x9f98, 0x5433,
  0x9f14, 0x539b, 0x9e91, 0x5303, 0x9e0f, 0x5269, 0x9d8e, 0x51cf,
  0x9d0e, 0x5134, 0x9c8f, 0x5098, 0x9c11, 0x4ffb, 0x9b94, 0x4f5e,
  0x9b17, 0x4ec0, 0x9a9c, 0x4e21, 0x9a22, 0x4d81, 0x99a9, 0x4ce1,
  0x9930, 0x4c40, 0x98b9, 0x4b9e, 0x9843, 0x4afb, 0x97ce, 0x4a58,
  0x9759, 0x49b4, 0x96e6, 0x490f, 0x9674, 0x486a, 0x9603, 0x47c4,
  0x9592, 0x471d, 0x9523, 0x4675, 0x94b5, 0x45cd, 0x9448, 0x4524,
  0x93dc, 0x447b, 0x9371, 0x43d1, 0x9307, 0x4326, 0x929e, 0x427a,
  0x9236, 0x41ce, 0x91cf, 0x4121, 0x9169, 0x4074, 0x9105, 0x3fc6,
  0x90a1, 0x3f17, 0x903e, 0x3e68, 0x8fdd, 0x3db8, 0x8f7d, 0x3d08,
  0x8f1d, 0x3c57, 0x8ebf, 0x3ba5, 0x8e62, 0x3af3, 0x8e06, 0x3a40,
  0x8dab, 0x398d, 0x8d51, 0x38d9, 0x8cf8, 0x3825, 0x8ca1, 0x3770,
  0x8c4a, 0x36ba, 0x8bf5, 0x3604, 0x8ba0, 0x354e, 0x8b4d, 0x3497,
  0x8afb, 0x33df, 0x8aaa, 0x3327, 0x8a5a, 0x326e, 0x8a0c, 0x31b5,
  0x89be, 0x30fc, 0x8972, 0x3042, 0x8927, 0x2f87, 0x88dd, 0x2ecc,
  0x8894, 0x2e11, 0x884c, 0x2d55, 0x8805, 0x2c99, 0x87c0, 0x2bdc,
  0x877b, 0x2b1f, 0x8738, 0x2a62, 0x86f6, 0x29a4, 0x86b6, 0x28e5,
  0x8676, 0x2827, 0x8637, 0x2768, 0x85fa, 0x26a8, 0x85be, 0x25e8,
  0x8583, 0x2528, 0x8549, 0x2467, 0x8511, 0x23a7, 0x84d9, 0x22e5,
  0x84a3, 0x2224, 0x846e, 0x2162, 0x843a, 0x209f, 0x8407, 0x1fdd,
  0x83d6, 0x1f1a, 0x83a6, 0x1e57, 0x8377, 0x1d93, 0x8349, 0x1cd0,
  0x831c, 0x1c0c, 0x82f1, 0x1b47, 0x82c6, 0x1a83, 0x829d, 0x19be,
  0x8276, 0x18f9, 0x824f, 0x1833, 0x822a, 0x176e, 0x8205, 0x16a8,
  0x81e2, 0x15e2, 0x81c1, 0x151c, 0x81a0, 0x1455, 0x8181, 0x138f,
  0x8163, 0x12c8, 0x8146, 0x1201, 0x812a, 0x113a, 0x8110, 0x1073,
  0x80f6, 0xfab, 0x80de, 0xee4, 0x80c8, 0xe1c, 0x80b2, 0xd54,
  0x809e, 0xc8c, 0x808b, 0xbc4, 0x8079, 0xafb, 0x8068, 0xa33,
  0x8059, 0x96b, 0x804b, 0x8a2, 0x803e, 0x7d9, 0x8032, 0x711,
  0x8027, 0x648, 0x801e, 0x57f, 0x8016, 0x4b6, 0x800f, 0x3ed,
  0x800a, 0x324, 0x8006, 0x25b, 0x8002, 0x192, 0x8001, 0xc9,
  0x8000, 0x0, 0x8001, 0xff37, 0x8002, 0xfe6e, 0x8006, 0xfda5,
  0x800a, 0xfcdc, 0x800f, 0xfc13, 0x8016, 0xfb4a, 0x801e, 0xfa81,
  0x8027, 0xf9b8, 0x8032, 0xf8ef, 0x803e, 0xf827, 0x804b, 0xf75e,
  0x8059, 0xf695, 0x8068, 0xf5cd, 0x8079, 0xf505, 0x808b, 0xf43c,
  0x809e, 0xf374, 0x80b2, 0xf2ac, 0x80c8, 0xf1e4, 0x80de, 0xf11c,
  0x80f6, 0xf055, 0x8110, 0xef8d, 0x812a, 0xeec6, 0x8146, 0xedff,
  0x8163, 0xed38, 0x8181, 0xec71, 0x81a0, 0xebab, 0x81c1, 0xeae4,
  0x81e2, 0xea1e, 0x8205, 0xe958, 0x822a, 0xe892, 0x824f, 0xe7cd,
  0x8276, 0xe707, 0x829d, 0xe642, 0x82c6, 0xe57d, 0x82f1, 0xe4b9,
  0x831c, 0xe3f4, 0x8349, 0xe330, 0x8377, 0xe26d, 0x83a6, 0xe1a9,
  0x83d6, 0xe0e6, 0x8407, 0xe023, 0x843a, 0xdf61, 0x846e, 0xde9e,
  0x84a3, 0xdddc, 0x84d9, 0xdd1b, 0x8511, 0xdc59, 0x8549, 0xdb99,
  0x8583, 0xdad8, 0x85be, 0xda18, 0x85fa, 0xd958, 0x8637, 0xd898,
  0x8676, 0xd7d9, 0x86b6, 0xd71b, 0x86f6, 0xd65c, 0x8738, 0xd59e,
  0x877b, 0xd4e1, 0x87c0, 0xd424, 0x8805, 0xd367, 0x884c, 0xd2ab,
  0x8894, 0xd1ef, 0x88dd, 0xd134, 0x8927, 0xd079, 0x8972, 0xcfbe,
  0x89be, 0xcf04, 0x8a0c, 0xce4b, 0x8a5a, 0xcd92, 0x8aaa, 0xccd9,
  0x8afb, 0xcc21, 0x8b4d, 0xcb69, 0x8ba0, 0xcab2, 0x8bf5, 0xc9fc,
  0x8c4a, 0xc946, 0x8ca1, 0xc890, 0x8cf8, 0xc7db, 0x8d51, 0xc727,
  0x8dab, 0xc673, 0x8e06, 0xc5c0, 0x8e62, 0xc50d, 0x8ebf, 0xc45b,
  0x8f1d, 0xc3a9, 0x8f7d, 0xc2f8, 0x8fdd, 0xc248, 0x903e, 0xc198,
  0x90a1, 0xc0e9, 0x9105, 0xc03a, 0x9169, 0xbf8c, 0x91cf, 0xbedf,
  0x9236, 0xbe32, 0x929e, 0xbd86, 0x9307, 0xbcda, 0x9371, 0xbc2f,
  0x93dc, 0xbb85, 0x9448, 0xbadc, 0x94b5, 0xba33, 0x9523, 0xb98b,
  0x9592, 0xb8e3, 0x9603, 0xb83c, 0x9674, 0xb796, 0x96e6, 0xb6f1,
  0x9759, 0xb64c, 0x97ce, 0xb5a8, 0x9843, 0xb505, 0x98b9, 0xb462,
  0x9930, 0xb3c0, 0x99a9, 0xb31f, 0x9a22, 0xb27f, 0x9a9c, 0xb1df,
  0x9b17, 0xb140, 0x9b94, 0xb0a2, 0x9c11, 0xb005, 0x9c8f, 0xaf68,
  0x9d0e, 0xaecc, 0x9d8e, 0xae31, 0x9e0f, 0xad97, 0x9e91, 0xacfd,
  0x9f14, 0xac65, 0x9f98, 0xabcd, 0xa01c, 0xab36, 0xa0a2, 0xaaa0,
  0xa129, 0xaa0a, 0xa1b0, 0xa976, 0xa238, 0xa8e2, 0xa2c2, 0xa84f,
  0xa34c, 0xa7bd, 0xa3d7, 0xa72c, 0xa463, 0xa69c, 0xa4f0, 0xa60c,
  0xa57e, 0xa57e, 0xa60c, 0xa4f0, 0xa69c, 0xa463, 0xa72c, 0xa3d7,
  0xa7bd, 0xa34c, 0xa84f, 0xa2c2, 0xa8e2, 0xa238, 0xa976, 0xa1b0,
  0xaa0a, 0xa129, 0xaaa0, 0xa0a2, 0xab36, 0xa01c, 0xabcd, 0x9f98,
  0xac65, 0x9f14, 0xacfd, 0x9e91, 0xad97, 0x9e0f, 0xae31, 0x9d8e,
  0xaecc, 0x9d0e, 0xaf68, 0x9c8f, 0xb005, 0x9c11, 0xb0a2, 0x9b94,
  0xb140, 0x9b17, 0xb1df, 0x9a9c, 0xb27f, 0x9a22, 0xb31f, 0x99a9,
  0xb3c0, 0x9930, 0xb462, 0x98b9, 0xb505, 0x9843, 0xb5a8, 0x97ce,
  0xb64c, 0x9759, 0xb6f1, 0x96e6, 0xb796, 0x9674, 0xb83c, 0x9603,
  0xb8e3, 0x9592, 0xb98b, 0x9523, 0xba33, 0x94b5, 0xbadc, 0x9448,
  0xbb85, 0x93dc, 0xbc2f, 0x9371, 0xbcda, 0x9307, 0xbd86, 0x929e,
  0xbe32, 0x9236, 0xbedf, 0x91cf, 0xbf8c, 0x9169, 0xc03a, 0x9105,
  0xc0e9, 0x90a1, 0xc198, 0x903e, 0xc248, 0x8fdd, 0xc2f8, 0x8f7d,
  0xc3a9, 0x8f1d, 0xc45b, 0x8ebf, 0xc50d, 0x8e62, 0xc5c0, 0x8e06,
  0xc673, 0x8dab, 0xc727, 0x8d51, 0xc7db, 0x8cf8, 0xc890, 0x8ca1,
  0xc946, 0x8c4a, 0xc9fc, 0x8bf5, 0xcab2, 0x8ba0, 0xcb69, 0x8b4d,
  0xcc21, 0x8afb, 0xccd9, 0x8aaa, 0xcd92, 0x8a5a, 0xce4b, 0x8a0c,
  0xcf04, 0x89be, 0xcfbe, 0x8972, 0xd079, 0x8927, 0xd134, 0x88dd,
  0xd1ef, 0x8894, 0xd2ab, 0x884c, 0xd367, 0x8805, 0xd424, 0x87c0,
  0xd4e1, 0x877b, 0xd59e, 0x8738, 0xd65c, 0x86f6, 0xd71b, 0x86b6,
  0xd7d9, 0x8676, 0xd898, 0x8637, 0xd958, 0x85fa, 0xda18, 0x85be,
  0xdad8, 0x8583, 0xdb99, 0x8549, 0xdc59, 0x8511, 0xdd1b, 0x84d9,
  0xdddc, 0x84a3, 0xde9e, 0x846e, 0xdf61, 0x843a, 0xe023, 0x8407,
  0xe0e6, 0x83d6, 0xe1a9, 0x83a6, 0xe26d, 0x8377, 0xe330, 0x8349,
  0xe3f4, 0x831c, 0xe4b9, 0x82f1, 0xe57d, 0x82c6, 0xe642, 0x829d,
  0xe707, 0x8276, 0xe7cd, 0x824f, 0xe892, 0x822a, 0xe958, 0x8205,
  0xea1e, 0x81e2, 0xeae4, 0x81c1, 0xebab, 0x81a0, 0xec71, 0x8181,
  0xed38, 0x8163, 0xedff, 0x8146, 0xeec6, 0x812a, 0xef8d, 0x8110,
  0xf055, 0x80f6, 0xf11c, 0x80de, 0xf1e4, 0x80c8, 0xf2ac, 0x80b2,
  0xf374, 0x809e, 0xf43c, 0x808b, 0xf505, 0x8079, 0xf5cd, 0x8068,
  0xf695, 0x8059, 0xf75e, 0x804b, 0xf827, 0x803e, 0xf8ef, 0x8032,
  0xf9b8, 0x8027, 0xfa81, 0x801e, 0xfb4a, 0x8016, 0xfc13, 0x800f,
  0xfcdc, 0x800a, 0xfda5, 0x8006, 0xfe6e, 0x8002, 0xff37, 0x8001,
  0x0, 0x8000, 0xc9, 0x8001, 0x192, 0x8002, 0x25b, 0x8006,
  0x324, 0x800a, 0x3ed, 0x800f, 0x4b6, 0x8016, 0x57f, 0x801e,
  0x648, 0x8027, 0x711, 0x8032, 0x7d9, 0x803e, 0x8a2, 0x804b,
  0x96b, 0x8059, 0xa33, 0x8068, 0xafb, 0x8079, 0xbc4, 0x808b,
  0xc8c, 0x809e, 0xd54, 0x80b2, 0xe1c, 0x80c8, 0xee4, 0x80de,
  0xfab, 0x80f6, 0x1073, 0x8110, 0x113a, 0x812a, 0x1201, 0x8146,
  0x12c8, 0x8163, 0x138f, 0x8181, 0x1455, 0x81a0, 0x151c, 0x81c1,
  0x15e2, 0x81e2, 0x16a8, 0x8205, 0x176e, 0x822a, 0x1833, 0x824f,
  0x18f9, 0x8276, 0x19be, 0x829d, 0x1a83, 0x82c6, 0x1b47, 0x82f1,
  0x1c0c, 0x831c, 0x1cd0, 0x8349, 0x1d93, 0x8377, 0x1e57, 0x83a6,
  0x1f1a, 0x83d6, 0x1fdd, 0x8407, 0x209f, 0x843a, 0x2162, 0x846e,
  0x2224, 0x84a3, 0x22e5, 0x84d9, 0x23a7, 0x8511, 0x2467, 0x8549,
  0x2528, 0x8583, 0x25e8, 0x85be, 0x26a8, 0x85fa, 0x2768, 0x8637,
  0x2827, 0x8676, 0x28e5, 0x86b6, 0x29a4, 0x86f6, 0x2a62, 0x8738,
  0x2b1f, 0x877b, 0x2bdc, 0x87c0, 0x2c99, 0x8805, 0x2d55, 0x884c,
  0x2e11, 0x8894, 0x2ecc, 0x88dd, 0x2f87, 0x8927, 0x3042, 0x8972,
  0x30fc, 0x89be, 0x31b5, 0x8a0c, 0x326e, 0x8a5a, 0x3327, 0x8aaa,
  0x33df, 0x8afb, 0x3497, 0x8b4d, 0x354e, 0x8ba0, 0x3604, 0x8bf5,
  0x36ba, 0x8c4a, 0x3770, 0x8ca1, 0x3825, 0x8cf8, 0x38d9, 0x8d51,
  0x398d, 0x8dab, 0x3a40, 0x8e06, 0x3af3, 0x8e62, 0x3ba5, 0x8ebf,
  0x3c57, 0x8f1d, 0x3d08, 0x8f7d, 0x3db8, 0x8fdd, 0x3e68, 0x903e,
  0x3f17, 0x90a1, 0x3fc6, 0x9105, 0x4074, 0x9169, 0x4121, 0x91cf,
  0x41ce, 0x9236, 0x427a, 0x929e, 0x4326, 0x9307, 0x43d1, 0x9371,
  0x447b, 0x93dc, 0x4524, 0x9448, 0x45cd, 0x94b5, 0x4675, 0x9523,
  0x471d, 0x9592, 0x47c4, 0x9603, 0x486a, 0x9674, 0x490f, 0x96e6,
  0x49b4, 0x9759, 0x4a58, 0x97ce, 0x4afb, 0x9843, 0x4b9e, 0x98b9,
  0x4c40, 0x9930, 0x4ce1, 0x99a9, 0x4d81, 0x9a22, 0x4e21, 0x9a9c,
  0x4ec0, 0x9b17, 0x4f5e, 0x9b94, 0x4ffb, 0x9c11, 0x5098, 0x9c8f,
  0x5134, 0x9d0e, 0x51cf, 0x9d8e, 0x5269, 0x9e0f, 0x5303, 0x9e91,
  0x539b, 0x9f14, 0x5433, 0x9f98, 0x54ca, 0xa01c, 0x5560, 0xa0a2,
  0x55f6, 0xa129, 0x568a, 0xa1b0, 0x571e, 0xa238, 0x57b1, 0xa2c2,
  0x5843, 0xa34c, 0x58d4, 0xa3d7, 0x5964, 0xa463, 0x59f4, 0xa4f0,
  0x5a82, 0xa57e, 0x5b10, 0xa60c, 0x5b9d, 0xa69c, 0x5c29, 0xa72c,
  0x5cb4, 0xa7bd, 0x5d3e, 0xa84f, 0x5dc8, 0xa8e2, 0x5e50, 0xa976,
  0x5ed7, 0xaa0a, 0x5f5e, 0xaaa0, 0x5fe4, 0xab36, 0x6068, 0xabcd,
  0x60ec, 0xac65, 0x616f, 0xacfd, 0x61f1, 0xad97, 0x6272, 0xae31,
  0x62f2, 0xaecc, 0x6371, 0xaf68, 0x63ef, 0xb005, 0x646c, 0xb0a2,
  0x64e9, 0xb140, 0x6564, 0xb1df, 0x65de, 0xb27f, 0x6657, 0xb31f,
  0x66d0, 0xb3c0, 0x6747, 0xb462, 0x67bd, 0xb505, 0x6832, 0xb5a8,
  0x68a7, 0xb64c, 0x691a, 0xb6f1, 0x698c, 0xb796, 0x69fd, 0xb83c,
  0x6a6e, 0xb8e3, 0x6add, 0xb98b, 0x6b4b, 0xba33, 0x6bb8, 0xbadc,
  0x6c24, 0xbb85, 0x6c8f, 0xbc2f, 0x6cf9, 0xbcda, 0x6d62, 0xbd86,
  0x6dca, 0xbe32, 0x6e31, 0xbedf, 0x6e97, 0xbf8c, 0x6efb, 0xc03a,
  0x6f5f, 0xc0e9, 0x6fc2, 0xc198, 0x7023, 0xc248, 0x7083, 0xc2f8,
  0x70e3, 0xc3a9, 0x7141, 0xc45b, 0x719e, 0xc50d, 0x71fa, 0xc5c0,
  0x7255, 0xc673, 0x72af, 0xc727, 0x7308, 0xc7db, 0x735f, 0xc890,
  0x73b6, 0xc946, 0x740b, 0xc9fc, 0x7460, 0xcab2, 0x74b3, 0xcb69,
  0x7505, 0xcc21, 0x7556, 0xccd9, 0x75a6, 0xcd92, 0x75f4, 0xce4b,
  0x7642, 0xcf04, 0x768e, 0xcfbe, 0x76d9, 0xd079, 0x7723, 0xd134,
  0x776c, 0xd1ef, 0x77b4, 0xd2ab, 0x77fb, 0xd367, 0x7840, 0xd424,
  0x7885, 0xd4e1, 0x78c8, 0xd59e, 0x790a, 0xd65c, 0x794a, 0xd71b,
  0x798a, 0xd7d9, 0x79c9, 0xd898, 0x7a06, 0xd958, 0x7a42, 0xda18,
  0x7a7d, 0xdad8, 0x7ab7, 0xdb99, 0x7aef, 0xdc59, 0x7b27, 0xdd1b,
  0x7b5d, 0xdddc, 0x7b92, 0xde9e, 0x7bc6, 0xdf61, 0x7bf9, 0xe023,
  0x7c2a, 0xe0e6, 0x7c5a, 0xe1a9, 0x7c89, 0xe26d, 0x7cb7, 0xe330,
  0x7ce4, 0xe3f4, 0x7d0f, 0xe4b9, 0x7d3a, 0xe57d, 0x7d63, 0xe642,
  0x7d8a, 0xe707, 0x7db1, 0xe7cd, 0x7dd6, 0xe892, 0x7dfb, 0xe958,
  0x7e1e, 0xea1e, 0x7e3f, 0xeae4, 0x7e60, 0xebab, 0x7e7f, 0xec71,
  0x7e9d, 0xed38, 0x7eba, 0xedff, 0x7ed6, 0xeec6, 0x7ef0, 0xef8d,
  0x7f0a, 0xf055, 0x7f22, 0xf11c, 0x7f38, 0xf1e4, 0x7f4e, 0xf2ac,
  0x7f62, 0xf374, 0x7f75, 0xf43c, 0x7f87, 0xf505, 0x7f98, 0xf5cd,
  0x7fa7, 0xf695, 0x7fb5, 0xf75e, 0x7fc2, 0xf827, 0x7fce, 0xf8ef,
  0x7fd9, 0xf9b8, 0x7fe2, 0xfa81, 0x7fea, 0xfb4a, 0x7ff1, 0xfc13,
  0x7ff6, 0xfcdc, 0x7ffa, 0xfda5, 0x7ffe, 0xfe6e, 0x7fff, 0xff37
};


/**
* @brief Initialization function for the Q15 CFFT/CIFFT.
* @param[in,out] *S             points to an instance of the Q15 CFFT/CIFFT structure.
* @param[in]     fftLen         length of the FFT.
* @param[in]     ifftFlag       flag that selects forward (ifftFlag=0) or inverse (ifftFlag=1) transform.
* @param[in]     bitReverseFlag flag that enables (bitReverseFlag=1) or disables (bitReverseFlag=0) bit reversal of output.
* @return        The function returns ARM_MATH_SUCCESS if initialization is successful or ARM_MATH_ARGUMENT_ERROR if <code>fftLen</code> is not a supported value.
*
* \par Description:
* \par
* The parameter <code>ifftFlag</code> controls whether a forward or inverse transform is computed.
* Set(=1) ifftFlag for calculation of CIFFT otherwise  CFFT is calculated
* \par
* The parameter <code>bitReverseFlag</code> controls whether output is in normal order or bit reversed order.
* Set(=1) bitReverseFlag for output to be in normal order otherwise output is in bit reversed order.
* \par
* The parameter <code>fftLen</code>	Specifies length of CFFT/CIFFT process. Supported FFT Lengths are 16, 64, 256, 1024.
* \par
* This Function also initializes Twiddle factor table pointer and Bit reversal table pointer.
*/

arm_status arm_cfft_radix4_init_q15(
  arm_cfft_radix4_instance_q15 * S,
  uint16_t fftLen,
  uint8_t ifftFlag,
  uint8_t bitReverseFlag)
{
  /*  Initialise the default arm status */
  arm_status status = ARM_MATH_SUCCESS;
  /*  Initialise the FFT length */
  S->fftLen = fftLen;
  /*  Initialise the Twiddle coefficient pointer */
  S->pTwiddle = (q15_t *) twiddleCoefQ15;
  /*  Initialise the Flag for selection of CFFT or CIFFT */
  S->ifftFlag = ifftFlag;
  /*  Initialise the Flag for calculation Bit reversal or not */
  S->bitReverseFlag = bitReverseFlag;

  /*  Initializations of structure parameters depending on the FFT length */
  switch (S->fftLen)
  {
    /*  Initializations of structure parameters for 1024 point FFT */
  case 1024u:
    /*  Initialise the twiddle coef modifier value */
    S->twidCoefModifier = 1u;
    /*  Initialise the bit reversal table modifier */
    S->bitRevFactor = 1u;
    /*  Initialise the bit reversal table pointer */
    S->pBitRevTable = armBitRevTable;

    break;
  case 256u:
    /*  Initializations of structure parameters for 2566 point FFT */
    S->twidCoefModifier = 4u;
    S->bitRevFactor = 4u;
    S->pBitRevTable = &armBitRevTable[3];

    break;
  case 64u:
    /*  Initializations of structure parameters for 64 point FFT */
    S->twidCoefModifier = 16u;
    S->bitRevFactor = 16u;
    S->pBitRevTable = &armBitRevTable[15];

    break;
  case 16u:
    /*  Initializations of structure parameters for 16 point FFT */
    S->twidCoefModifier = 64u;
    S->bitRevFactor = 64u;
    S->pBitRevTable = &armBitRevTable[63];

    break;
  default:
    /*  Reporting argument error if fftSize is not valid value */
    status = ARM_MATH_ARGUMENT_ERROR;
    break;
  }

  return (status);
}

/**
 * @} end of CFFT_CIFFT group
 */
