//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: sistema_de_control.cpp
//
// Code generated for Simulink model 'sistema_de_control'.
//
// Model version                  : 2.38
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Mon Feb 24 13:16:19 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#pragma GCC diagnostic ignored "-Wfloat-equal"
#include "AP_fuzzy_logic/sistema_de_control.h"
#include "AP_fuzzy_logic/rtwtypes.h"
#include <cmath>
#include <stddef.h>
#define NumBitsPerChar                 8U

//extern real_T rt_atan2d_snf(real_T u0, real_T u1);
static uint32_T plook_binx(real_T u, const real_T bp[], uint32_T maxIndex,
  real_T *fraction);
static real_T intrp2d_g(const uint32_T bpIndex[], const real_T frac[], const
  real_T table[], const uint32_T stride);
static uint32_T binsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex);

//===========*
//  Constants *
// ===========
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

//
//  UNUSED_PARAMETER(x)
//    Used to specify that a function parameter (argument) is required but not
//    accessed by the function body.

#ifndef UNUSED_PARAMETER
#if defined(__LCC__)
#define UNUSED_PARAMETER(x)                                      // do nothing
#else

//
//  This is the semi-ANSI standard way of indicating that an
//  unused function parameter is required.

#define UNUSED_PARAMETER(x)            (void) (x)
#endif
#endif
/*
extern "C"
{
  real_T rtInf;
  real_T rtMinusInf;
  real_T rtNaN;
  real32_T rtInfF;
  real32_T rtMinusInfF;
  real32_T rtNaNF;
}
*/
extern "C"
{
  //
  // Initialize rtNaN needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetNaN(void)
  {
    size_t bitsPerReal{ sizeof(real_T) * (NumBitsPerChar) };

    real_T nan{ 0.0 };

    if (bitsPerReal == 32U) {
      nan = rtGetNaNF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
    }

    return nan;
  }

  //
  // Initialize rtNaNF needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetNaNF(void)
  {
    IEEESingle nanF{ { 0.0F } };

    nanF.wordL.wordLuint = 0xFFC00000U;
    return nanF.wordL.wordLreal;
  }
}

extern "C"
{
  //
  // Initialize the rtInf, rtMinusInf, and rtNaN needed by the
  // generated code. NaN is initialized as non-signaling. Assumes IEEE.
  //
  /**/
  static void rt_InitInfAndNaN(size_t realSize)
  {
    (void) (realSize);
    rtNaN = rtGetNaN();
    rtNaNF = rtGetNaNF();
    rtInf = rtGetInf();
    rtInfF = rtGetInfF();
    rtMinusInf = rtGetMinusInf();
    rtMinusInfF = rtGetMinusInfF();
  }

  // Test if value is infinite
  static boolean_T rtIsInf(real_T value)
  {
    return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
  }

  // Test if single-precision value is infinite
  static boolean_T rtIsInfF(real32_T value)
  {
    return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
  }

  // Test if value is not a number
  static boolean_T rtIsNaN(real_T value)
  {
    boolean_T result{ (boolean_T) 0 };

    size_t bitsPerReal{ sizeof(real_T) * (NumBitsPerChar) };

    if (bitsPerReal == 32U) {
      result = rtIsNaNF((real32_T)value);
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.fltVal = value;
      result = (boolean_T)((tmpVal.bitVal.words.wordH & 0x7FF00000) ==
                           0x7FF00000 &&
                           ( (tmpVal.bitVal.words.wordH & 0x000FFFFF) != 0 ||
                            (tmpVal.bitVal.words.wordL != 0) ));
    }

    return result;
  }

  // Test if single-precision value is not a number
  static boolean_T rtIsNaNF(real32_T value)
  {
    IEEESingle tmp;
    tmp.wordL.wordLreal = value;
    return (boolean_T)( (tmp.wordL.wordLuint & 0x7F800000) == 0x7F800000 &&
                       (tmp.wordL.wordLuint & 0x007FFFFF) != 0 );
  }
}

extern "C"
{
  //
  // Initialize rtInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetInf(void)
  {
    size_t bitsPerReal{ sizeof(real_T) * (NumBitsPerChar) };

    real_T inf{ 0.0 };

    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0x7FF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      inf = tmpVal.fltVal;
    }

    return inf;
  }

  //
  // Initialize rtInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  //
  // Initialize rtMinusInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal{ sizeof(real_T) * (NumBitsPerChar) };

    real_T minf{ 0.0 };

    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      minf = tmpVal.fltVal;
    }

    return minf;
  }

  //
  // Initialize rtMinusInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}

static uint32_T plook_binx(real_T u, const real_T bp[], uint32_T maxIndex,
  real_T *fraction)
{
  uint32_T bpIndex;

  // Prelookup - Index and Fraction
  // Index Search method: 'binary'
  // Extrapolation method: 'Linear'
  // Use previous index: 'off'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Remove protection against out-of-range input in generated code: 'off'

  if (u <= bp[0U]) {
    bpIndex = 0U;
    *fraction = (u - bp[0U]) / (bp[1U] - bp[0U]);
  } else if (u < bp[maxIndex]) {
    bpIndex = binsearch_u32d(u, bp, maxIndex >> 1U, maxIndex);
    *fraction = (u - bp[bpIndex]) / (bp[bpIndex + 1U] - bp[bpIndex]);
  } else {
    bpIndex = maxIndex - 1U;
    *fraction = (u - bp[maxIndex - 1U]) / (bp[maxIndex] - bp[maxIndex - 1U]);
  }

  return bpIndex;
}

static real_T intrp2d_g(const uint32_T bpIndex[], const real_T frac[], const
  real_T table[], const uint32_T stride)
{
  uint32_T offset_0d;
  uint32_T offset_1d;

  // Column-major Interpolation 2-D
  // Interpolation method: 'Linear Lagrange'
  // Use last breakpoint for index at or above upper limit: 'off'
  // Overflow mode: 'wrapping'

  offset_1d = bpIndex[1U] * stride + bpIndex[0U];
  offset_0d = offset_1d + stride;
  return ((1.0 - frac[0U]) * table[offset_1d] + table[offset_1d + 1U] * frac[0U])
    * (1.0 - frac[1U]) + ((1.0 - frac[0U]) * table[offset_0d] + table[offset_0d
    + 1U] * frac[0U]) * frac[1U];
}

static uint32_T binsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex)
{
  uint32_T bpIdx;
  uint32_T bpIndex;
  uint32_T iRght;

  // Binary Search
  bpIdx = startIndex;
  bpIndex = 0U;
  iRght = maxIndex;
  while (iRght - bpIndex > 1U) {
    if (u < bp[bpIdx]) {
      iRght = bpIdx;
    } else {
      bpIndex = bpIdx;
    }

    bpIdx = (iRght + bpIndex) >> 1U;
  }

  return bpIndex;
}

//
// Output and update for atomic system:
//    '<S11>/generador de matriz'
//    '<S12>/generador de matriz'
//    '<S13>/generador de matriz'
//    '<S14>/generador de matriz'
//
void sistema_de_control::generadordematriz(real_T rtu_k1e, real_T rtu_k2e,
  real_T rtu_k3e, real_T rtu_k4e, real_T rtu_k1dT, real_T rtu_k2dT, real_T
  rtu_k3dT, real_T rtu_k4dT, real_T rty_K1[8])
{
  rty_K1[0] = rtu_k1e;
  rty_K1[2] = rtu_k2e;
  rty_K1[4] = rtu_k3e;
  rty_K1[6] = rtu_k4e;
  rty_K1[1] = rtu_k1dT;
  rty_K1[3] = rtu_k2dT;
  rty_K1[5] = rtu_k3dT;
  rty_K1[7] = rtu_k4dT;
}
/*
real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = (rtNaN);
  } else if (std::isinf(u0) && std::isinf(u1)) {
    int32_T tmp;
    int32_T tmp_0;
    if (u0 > 0.0) {
      tmp = 1;
    } else {
      tmp = -1;
    }

    if (u1 > 0.0) {
      tmp_0 = 1;
    } else {
      tmp_0 = -1;
    }

    y = std::atan2(static_cast<real_T>(tmp), static_cast<real_T>(tmp_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}
*/
// Model step function
void sistema_de_control::step()
{
  real_T rtb_K1[8];
  real_T rtb_K1_d[8];
  real_T rtb_K1_n[8];
  real_T rtb_K1_p[8];
  real_T frac[2];
  real_T frac_0[2];
  real_T frac_1[2];
  real_T frac_2[2];
  real_T frac_3[2];
  real_T frac_4[2];
  real_T frac_5[2];
  real_T frac_6[2];
  real_T frac_7[2];
  real_T frac_8[2];
  real_T frac_9[2];
  real_T frac_a[2];
  real_T frac_b[2];
  real_T frac_c[2];
  real_T frac_d[2];
  real_T frac_e[2];
  real_T frac_f[2];
  real_T frac_g[2];
  real_T frac_h[2];
  real_T frac_i[2];
  real_T frac_j[2];
  real_T frac_k[2];
  real_T frac_l[2];
  real_T frac_m[2];
  real_T frac_n[2];
  real_T frac_o[2];
  real_T frac_p[2];
  real_T frac_q[2];
  real_T frac_r[2];
  real_T frac_s[2];
  real_T frac_t[2];
  real_T frac_u[2];
  real_T frac_v[2];
  real_T frac_w[2];
  real_T frac_x[2];
  real_T frac_y[2];
  real_T frac_z[2];
  real_T WS;
  real_T g1;
  real_T g3;
  real_T rtb_V;
  real_T rtb_V_lookup2_o2;
  real_T rtb_V_lookup_o2;
  real_T rtb_gamma;
  real_T rtb_gamma_lookup1_o2;
  real_T rtb_gamma_lookup_o2;
  real_T rtb_mu_idx_1;
  real_T rtb_mu_idx_3;
  real_T w2;
  real_T w3;
  real_T w4;
  int32_T i;
  uint32_T bpIndex[2];
  uint32_T bpIndex_0[2];
  uint32_T bpIndex_1[2];
  uint32_T bpIndex_2[2];
  uint32_T bpIndex_3[2];
  uint32_T bpIndex_4[2];
  uint32_T bpIndex_5[2];
  uint32_T bpIndex_6[2];
  uint32_T bpIndex_7[2];
  uint32_T bpIndex_8[2];
  uint32_T bpIndex_9[2];
  uint32_T bpIndex_a[2];
  uint32_T bpIndex_b[2];
  uint32_T bpIndex_c[2];
  uint32_T bpIndex_d[2];
  uint32_T bpIndex_e[2];
  uint32_T bpIndex_f[2];
  uint32_T bpIndex_g[2];
  uint32_T bpIndex_h[2];
  uint32_T bpIndex_i[2];
  uint32_T bpIndex_j[2];
  uint32_T bpIndex_k[2];
  uint32_T bpIndex_l[2];
  uint32_T bpIndex_m[2];
  uint32_T bpIndex_n[2];
  uint32_T bpIndex_o[2];
  uint32_T bpIndex_p[2];
  uint32_T bpIndex_q[2];
  uint32_T bpIndex_r[2];
  uint32_T bpIndex_s[2];
  uint32_T bpIndex_t[2];
  uint32_T bpIndex_u[2];
  uint32_T bpIndex_v[2];
  uint32_T bpIndex_w[2];
  uint32_T bpIndex_x[2];
  uint32_T bpIndex_y[2];
  uint32_T bpIndex_z[2];
  uint32_T qY;
  uint32_T rtb_V_lookup_o1;
  uint32_T rtb_gamma_lookup1_o1;
  uint32_T rtb_gamma_lookup_o1;

  // Outputs for Atomic SubSystem: '<Root>/sistema_de_control'
  // MATLAB Function: '<S1>/MATLAB Function2' incorporates:
  //   Inport: '<Root>/theta'
  //   Inport: '<Root>/u'
  //   Inport: '<Root>/w'

  rtb_gamma = rtU.theta - rt_atan2d_snf(rtU.w, rtU.u);
  rtb_V = std::sqrt(rtU.u * rtU.u + rtU.w * rtU.w);

  // PreLookup: '<S7>/gamma_lookup'
  rtb_gamma_lookup_o1 = plook_binx(rtb_gamma, rtConstP.pooled2, 12U,
    &rtb_gamma_lookup_o2);

  // PreLookup: '<S7>/V_lookup'
  rtb_V_lookup_o1 = plook_binx(rtb_V, rtConstP.pooled3, 13U, &rtb_V_lookup_o2);

  // Interpolation_n-D: '<S11>/k1de'
  frac[0] = 0.0;
  frac[1] = 0.0;
  bpIndex[0] = rtb_gamma_lookup_o1;
  bpIndex[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k2de'
  frac_0[0] = 0.0;
  frac_0[1] = 0.0;
  bpIndex_0[0] = rtb_gamma_lookup_o1;
  bpIndex_0[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k3de'
  frac_1[0] = 0.0;
  frac_1[1] = 0.0;
  bpIndex_1[0] = rtb_gamma_lookup_o1;
  bpIndex_1[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k4de'
  frac_2[0] = 0.0;
  frac_2[1] = 0.0;
  bpIndex_2[0] = rtb_gamma_lookup_o1;
  bpIndex_2[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k1dT'
  frac_3[0] = 0.0;
  frac_3[1] = 0.0;
  bpIndex_3[0] = rtb_gamma_lookup_o1;
  bpIndex_3[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k2dT'
  frac_4[0] = 0.0;
  frac_4[1] = 0.0;
  bpIndex_4[0] = rtb_gamma_lookup_o1;
  bpIndex_4[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k3dT'
  frac_5[0] = 0.0;
  frac_5[1] = 0.0;
  bpIndex_5[0] = rtb_gamma_lookup_o1;
  bpIndex_5[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S11>/k4dT'
  frac_6[0] = 0.0;
  frac_6[1] = 0.0;
  bpIndex_6[0] = rtb_gamma_lookup_o1;
  bpIndex_6[1] = rtb_V_lookup_o1;

  // MATLAB Function: '<S11>/generador de matriz' incorporates:
  //   Interpolation_n-D: '<S11>/k1dT'
  //   Interpolation_n-D: '<S11>/k1de'
  //   Interpolation_n-D: '<S11>/k2dT'
  //   Interpolation_n-D: '<S11>/k2de'
  //   Interpolation_n-D: '<S11>/k3dT'
  //   Interpolation_n-D: '<S11>/k3de'
  //   Interpolation_n-D: '<S11>/k4dT'
  //   Interpolation_n-D: '<S11>/k4de'

  generadordematriz(intrp2d_g(bpIndex, frac, rtConstP.pooled4, 13U), intrp2d_g
                    (bpIndex_0, frac_0, rtConstP.pooled5, 13U), intrp2d_g
                    (bpIndex_1, frac_1, rtConstP.pooled6, 13U), intrp2d_g
                    (bpIndex_2, frac_2, rtConstP.pooled7, 13U), intrp2d_g
                    (bpIndex_3, frac_3, rtConstP.pooled8, 13U), intrp2d_g
                    (bpIndex_4, frac_4, rtConstP.pooled9, 13U), intrp2d_g
                    (bpIndex_5, frac_5, rtConstP.pooled10, 13U), intrp2d_g
                    (bpIndex_6, frac_6, rtConstP.pooled11, 13U), rtb_K1_p);

  // Interpolation_n-D: '<S12>/k1de'
  frac_7[0] = 0.0;
  frac_7[1] = 0.0;
  bpIndex_7[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k2de'
  frac_8[0] = 0.0;
  frac_8[1] = 0.0;
  bpIndex_8[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k1de' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_7[1] = 12U;
  } else {
    bpIndex_7[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k3de'
  frac_9[0] = 0.0;
  frac_9[1] = 0.0;
  bpIndex_9[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k2de' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_8[1] = 12U;
  } else {
    bpIndex_8[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k4de'
  frac_a[0] = 0.0;
  frac_a[1] = 0.0;
  bpIndex_a[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k3de' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_9[1] = 12U;
  } else {
    bpIndex_9[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k1dT'
  frac_b[0] = 0.0;
  frac_b[1] = 0.0;
  bpIndex_b[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k4de' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_a[1] = 12U;
  } else {
    bpIndex_a[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k2dT'
  frac_c[0] = 0.0;
  frac_c[1] = 0.0;
  bpIndex_c[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k1dT' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_b[1] = 12U;
  } else {
    bpIndex_b[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k3dT'
  frac_d[0] = 0.0;
  frac_d[1] = 0.0;
  bpIndex_d[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k2dT' incorporates:
  //   Constant: '<S12>/Constant'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_c[1] = 12U;
  } else {
    bpIndex_c[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S12>/k4dT'
  frac_e[0] = 0.0;
  frac_e[1] = 0.0;
  bpIndex_e[0] = rtb_gamma_lookup_o1;

  // Interpolation_n-D: '<S12>/k3dT' incorporates:
  //   Constant: '<S12>/Constant'
  //   Interpolation_n-D: '<S12>/k4dT'
  //   Sum: '<S12>/Sum'

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_d[1] = 12U;
    bpIndex_e[1] = 12U;
  } else {
    bpIndex_d[1] = rtb_V_lookup_o1 + 1U;
    bpIndex_e[1] = rtb_V_lookup_o1 + 1U;
  }

  // MATLAB Function: '<S12>/generador de matriz' incorporates:
  //   Interpolation_n-D: '<S12>/k1dT'
  //   Interpolation_n-D: '<S12>/k1de'
  //   Interpolation_n-D: '<S12>/k2dT'
  //   Interpolation_n-D: '<S12>/k2de'
  //   Interpolation_n-D: '<S12>/k3dT'
  //   Interpolation_n-D: '<S12>/k3de'
  //   Interpolation_n-D: '<S12>/k4dT'
  //   Interpolation_n-D: '<S12>/k4de'

  generadordematriz(intrp2d_g(bpIndex_7, frac_7, rtConstP.pooled4, 13U),
                    intrp2d_g(bpIndex_8, frac_8, rtConstP.pooled5, 13U),
                    intrp2d_g(bpIndex_9, frac_9, rtConstP.pooled6, 13U),
                    intrp2d_g(bpIndex_a, frac_a, rtConstP.pooled7, 13U),
                    intrp2d_g(bpIndex_b, frac_b, rtConstP.pooled8, 13U),
                    intrp2d_g(bpIndex_c, frac_c, rtConstP.pooled9, 13U),
                    intrp2d_g(bpIndex_d, frac_d, rtConstP.pooled10, 13U),
                    intrp2d_g(bpIndex_e, frac_e, rtConstP.pooled11, 13U),
                    rtb_K1_n);

  // Interpolation_n-D: '<S13>/k1de' incorporates:
  //   Constant: '<S13>/Constant'
  //   Interpolation_n-D: '<S13>/k2de'
  //   Sum: '<S13>/Sum'

  frac_f[0] = 0.0;
  frac_f[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_f[0] = 11U;
    bpIndex_g[0] = 11U;
  } else {
    bpIndex_f[0] = rtb_gamma_lookup_o1 + 1U;
    bpIndex_g[0] = rtb_gamma_lookup_o1 + 1U;
  }

  bpIndex_f[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k2de'
  frac_g[0] = 0.0;
  frac_g[1] = 0.0;
  bpIndex_g[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k3de' incorporates:
  //   Constant: '<S13>/Constant'
  //   Interpolation_n-D: '<S13>/k4de'
  //   Sum: '<S13>/Sum'

  frac_h[0] = 0.0;
  frac_h[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_h[0] = 11U;
    bpIndex_i[0] = 11U;
  } else {
    bpIndex_h[0] = rtb_gamma_lookup_o1 + 1U;
    bpIndex_i[0] = rtb_gamma_lookup_o1 + 1U;
  }

  bpIndex_h[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k4de'
  frac_i[0] = 0.0;
  frac_i[1] = 0.0;
  bpIndex_i[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k1dT' incorporates:
  //   Constant: '<S13>/Constant'
  //   Interpolation_n-D: '<S13>/k2dT'
  //   Sum: '<S13>/Sum'

  frac_j[0] = 0.0;
  frac_j[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_j[0] = 11U;
    bpIndex_k[0] = 11U;
  } else {
    bpIndex_j[0] = rtb_gamma_lookup_o1 + 1U;
    bpIndex_k[0] = rtb_gamma_lookup_o1 + 1U;
  }

  bpIndex_j[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k2dT'
  frac_k[0] = 0.0;
  frac_k[1] = 0.0;
  bpIndex_k[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k3dT' incorporates:
  //   Constant: '<S13>/Constant'
  //   Sum: '<S13>/Sum'

  frac_l[0] = 0.0;
  frac_l[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_l[0] = 11U;
  } else {
    bpIndex_l[0] = rtb_gamma_lookup_o1 + 1U;
  }

  bpIndex_l[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S13>/k4dT'
  frac_m[0] = 0.0;
  frac_m[1] = 0.0;
  bpIndex_m[0] = 0U;
  bpIndex_m[1] = rtb_V_lookup_o1;

  // MATLAB Function: '<S13>/generador de matriz' incorporates:
  //   Interpolation_n-D: '<S13>/k1dT'
  //   Interpolation_n-D: '<S13>/k1de'
  //   Interpolation_n-D: '<S13>/k2dT'
  //   Interpolation_n-D: '<S13>/k2de'
  //   Interpolation_n-D: '<S13>/k3dT'
  //   Interpolation_n-D: '<S13>/k3de'
  //   Interpolation_n-D: '<S13>/k4dT'
  //   Interpolation_n-D: '<S13>/k4de'

  generadordematriz(intrp2d_g(bpIndex_f, frac_f, rtConstP.pooled4, 13U),
                    intrp2d_g(bpIndex_g, frac_g, rtConstP.pooled5, 13U),
                    intrp2d_g(bpIndex_h, frac_h, rtConstP.pooled6, 13U),
                    intrp2d_g(bpIndex_i, frac_i, rtConstP.pooled7, 13U),
                    intrp2d_g(bpIndex_j, frac_j, rtConstP.pooled8, 13U),
                    intrp2d_g(bpIndex_k, frac_k, rtConstP.pooled9, 13U),
                    intrp2d_g(bpIndex_l, frac_l, rtConstP.pooled10, 13U),
                    intrp2d_g(bpIndex_m, frac_m, rtConstP.pooled11, 13U),
                    rtb_K1_d);

  // Interpolation_n-D: '<S14>/k1de' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_n[0] = 0.0;
  frac_n[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_n[0] = 11U;
  } else {
    bpIndex_n[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_n[1] = 12U;
  } else {
    bpIndex_n[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k2de' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_o[0] = 0.0;
  frac_o[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_o[0] = 11U;
  } else {
    bpIndex_o[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_o[1] = 12U;
  } else {
    bpIndex_o[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k3de' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_p[0] = 0.0;
  frac_p[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_p[0] = 11U;
  } else {
    bpIndex_p[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_p[1] = 12U;
  } else {
    bpIndex_p[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k4de' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_q[0] = 0.0;
  frac_q[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_q[0] = 11U;
  } else {
    bpIndex_q[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_q[1] = 12U;
  } else {
    bpIndex_q[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k1dT' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_r[0] = 0.0;
  frac_r[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_r[0] = 11U;
  } else {
    bpIndex_r[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_r[1] = 12U;
  } else {
    bpIndex_r[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k2dT' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_s[0] = 0.0;
  frac_s[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_s[0] = 11U;
  } else {
    bpIndex_s[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_s[1] = 12U;
  } else {
    bpIndex_s[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k3dT' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_t[0] = 0.0;
  frac_t[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_t[0] = 11U;
  } else {
    bpIndex_t[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_t[1] = 12U;
  } else {
    bpIndex_t[1] = rtb_V_lookup_o1 + 1U;
  }

  // Interpolation_n-D: '<S14>/k4dT' incorporates:
  //   Constant: '<S14>/Constant'
  //   Constant: '<S14>/Constant1'
  //   Sum: '<S14>/Sum'
  //   Sum: '<S14>/Sum1'

  frac_u[0] = 0.0;
  frac_u[1] = 0.0;
  if (static_cast<real_T>(rtb_gamma_lookup_o1) + 1.0 > 11.0) {
    bpIndex_u[0] = 11U;
  } else {
    bpIndex_u[0] = rtb_gamma_lookup_o1 + 1U;
  }

  if (static_cast<real_T>(rtb_V_lookup_o1) + 1.0 > 12.0) {
    bpIndex_u[1] = 12U;
  } else {
    bpIndex_u[1] = rtb_V_lookup_o1 + 1U;
  }

  // MATLAB Function: '<S14>/generador de matriz' incorporates:
  //   Interpolation_n-D: '<S14>/k1dT'
  //   Interpolation_n-D: '<S14>/k1de'
  //   Interpolation_n-D: '<S14>/k2dT'
  //   Interpolation_n-D: '<S14>/k2de'
  //   Interpolation_n-D: '<S14>/k3dT'
  //   Interpolation_n-D: '<S14>/k3de'
  //   Interpolation_n-D: '<S14>/k4dT'
  //   Interpolation_n-D: '<S14>/k4de'

  generadordematriz(intrp2d_g(bpIndex_n, frac_n, rtConstP.pooled4, 13U),
                    intrp2d_g(bpIndex_o, frac_o, rtConstP.pooled5, 13U),
                    intrp2d_g(bpIndex_p, frac_p, rtConstP.pooled6, 13U),
                    intrp2d_g(bpIndex_q, frac_q, rtConstP.pooled7, 13U),
                    intrp2d_g(bpIndex_r, frac_r, rtConstP.pooled8, 13U),
                    intrp2d_g(bpIndex_s, frac_s, rtConstP.pooled9, 13U),
                    intrp2d_g(bpIndex_t, frac_t, rtConstP.pooled10, 13U),
                    intrp2d_g(bpIndex_u, frac_u, rtConstP.pooled11, 13U), rtb_K1);

  // MATLAB Function: '<S7>/MATLAB Function' incorporates:
  //   Constant: '<S7>/Constant'
  //   Constant: '<S7>/Constant4'
  //   Constant: '<S7>/Constant5'
  //   Constant: '<S7>/Constant7'

  rtb_mu_idx_3 = std::floor((rtb_gamma - -24.0) / 4.0) * 4.0 - 24.0;
  rtb_mu_idx_1 = std::floor((rtb_V - 10.0) / 2.0) * 2.0 + 10.0;
  w2 = rtb_gamma - rtb_mu_idx_3;
  g1 = rtb_V - rtb_mu_idx_1;
  w2 *= w2;
  rtb_V_lookup2_o2 = g1 * g1;
  rtb_gamma_lookup1_o2 = std::sqrt(w2 + rtb_V_lookup2_o2);
  g3 = rtb_V - (rtb_mu_idx_1 + 2.0);
  g1 = g3 * g3;
  w2 = std::sqrt(w2 + g1);
  g3 = rtb_gamma - (rtb_mu_idx_3 + 4.0);
  g3 *= g3;
  w3 = std::sqrt(g3 + rtb_V_lookup2_o2);
  w4 = std::sqrt(g3 + g1);
  rtb_gamma_lookup1_o2 = 1.0 / (rtb_gamma_lookup1_o2 * rtb_gamma_lookup1_o2);
  w2 = 1.0 / (w2 * w2);
  w3 = 1.0 / (w3 * w3);
  w4 = 1.0 / (w4 * w4);
  WS = ((rtb_gamma_lookup1_o2 + w2) + w3) + w4;
  g1 = ((rtb_gamma == rtb_mu_idx_3) && (rtb_V == rtb_mu_idx_1));
  if ((rtb_gamma == rtb_mu_idx_3) && (rtb_mu_idx_1 + 2.0 == rtb_V)) {
    g1 = 0.0;
    rtb_V_lookup2_o2 = 1.0;
  } else {
    rtb_V_lookup2_o2 = 0.0;
  }

  if ((rtb_mu_idx_3 + 4.0 == rtb_gamma) && (rtb_V == rtb_mu_idx_1)) {
    g1 = 0.0;
    rtb_V_lookup2_o2 = 0.0;
    g3 = 1.0;
  } else {
    g3 = 0.0;
  }

  if ((rtb_mu_idx_3 + 4.0 == rtb_gamma) && (rtb_mu_idx_1 + 2.0 == rtb_V)) {
    g1 = 0.0;
    rtb_V_lookup2_o2 = 0.0;
    g3 = 0.0;
    rtb_mu_idx_3 = 1.0;
  } else {
    rtb_mu_idx_3 = 0.0;
  }

  if ((g1 == 0.0) && (rtb_V_lookup2_o2 == 0.0) && (g3 == 0.0)) {
    g1 = rtb_gamma_lookup1_o2 / WS;
    rtb_V_lookup2_o2 = w2 / WS;
    g3 = w3 / WS;
    rtb_mu_idx_3 = w4 / WS;
  }

  rtb_mu_idx_1 = rtb_V_lookup2_o2;
  w2 = g3;

  // Interpolation_n-D: '<S7>/Peq_de'
  frac_v[0] = rtb_gamma_lookup_o2;
  frac_v[1] = rtb_V_lookup_o2;
  bpIndex_v[0] = rtb_gamma_lookup_o1;
  bpIndex_v[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/Peq_dT'
  frac_w[0] = rtb_gamma_lookup_o2;
  frac_w[1] = rtb_V_lookup_o2;
  bpIndex_w[0] = rtb_gamma_lookup_o1;
  bpIndex_w[1] = rtb_V_lookup_o1;

  // Gain: '<S6>/Gain' incorporates:
  //   Inport: '<Root>/gammad'

  g3 = 0.017453292519943295 * rtU.gammad;

  // PreLookup: '<S6>/gamma_lookup1'
  rtb_gamma_lookup1_o1 = plook_binx(g3, rtConstP.pooled2, 12U,
    &rtb_gamma_lookup1_o2);

  // Interpolation_n-D: '<S6>/Peq_dT' incorporates:
  //   Inport: '<Root>/Vd'
  //   PreLookup: '<S6>/V_lookup1'

  bpIndex_x[1] = plook_binx(rtU.Vd, rtConstP.pooled3, 13U, &rtb_V_lookup2_o2);
  frac_x[0] = rtb_gamma_lookup1_o2;
  frac_x[1] = rtb_V_lookup2_o2;
  bpIndex_x[0] = rtb_gamma_lookup1_o1;
  w3 = intrp2d_g(bpIndex_x, frac_x, rtConstP.pooled12, 13U);

  // MATLAB Function: '<S6>/MATLAB Function1' incorporates:
  //   Constant: '<S6>/Constant'
  //   Inport: '<Root>/Vd'

  if (w3 < 0.0) {
    qY = rtb_gamma_lookup1_o1 + /*MW:OvSatOk*/ 1U;
    if (rtb_gamma_lookup1_o1 + 1U < rtb_gamma_lookup1_o1) {
      qY = MAX_uint32_T;
    }

    w3 = rtConstP.Constant_Value[static_cast<int32_T>(qY) - 1];
  } else if (w3 > 4.7382) {
    qY = rtb_gamma_lookup1_o1 + /*MW:OvSatOk*/ 1U;
    if (rtb_gamma_lookup1_o1 + 1U < rtb_gamma_lookup1_o1) {
      qY = MAX_uint32_T;
    }

    w3 = rtConstP.Constant_Value[static_cast<int32_T>(qY) + 12];
  } else {
    w3 = rtU.Vd;
  }

  // End of MATLAB Function: '<S6>/MATLAB Function1'

  // Interpolation_n-D: '<S6>/Peq_alpha' incorporates:
  //   PreLookup: '<S6>/V_lookup2'

  bpIndex_y[1] = plook_binx(w3, rtConstP.pooled3, 13U, &rtb_V_lookup2_o2);
  frac_y[0] = rtb_gamma_lookup1_o2;
  frac_y[1] = rtb_V_lookup2_o2;
  bpIndex_y[0] = rtb_gamma_lookup1_o1;
  rtb_gamma_lookup1_o2 = intrp2d_g(bpIndex_y, frac_y, rtConstP.Peq_alpha_Table,
    13U);

  // Interpolation_n-D: '<S7>/Peq_wd'
  frac_z[0] = rtb_gamma_lookup_o2;
  frac_z[1] = rtb_V_lookup_o2;
  bpIndex_z[0] = rtb_gamma_lookup_o1;
  bpIndex_z[1] = rtb_V_lookup_o1;
  rtb_gamma_lookup_o2 = intrp2d_g(bpIndex_z, frac_z, rtConstP.Peq_wd_Table, 13U);

  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   Inport: '<Root>/q'
  //   Inport: '<Root>/theta'
  //   Inport: '<Root>/u'
  //   Inport: '<Root>/w'
  //   Interpolation_n-D: '<S7>/Peq_dT'
  //   Interpolation_n-D: '<S7>/Peq_de'
  //   MATLAB Function: '<S6>/MATLAB Function'
  //   MATLAB Function: '<S7>/MATLAB Function'

  rtb_V_lookup_o2 = rtU.u - w3 * std::cos(rtb_gamma_lookup1_o2);
  rtb_V_lookup2_o2 = rtU.w - rtb_gamma_lookup_o2;
  g3 = rtU.theta - (rtb_gamma_lookup1_o2 + g3);
  for (i = 0; i < 2; i++) {
    frac[i] = -((((((rtb_K1_p[i + 2] * g1 * rtb_V_lookup2_o2 + g1 * rtb_K1_p[i] *
                     rtb_V_lookup_o2) + rtb_K1_p[i + 4] * g1 * g3) + rtb_K1_p[i
                   + 6] * g1 * rtU.q) + (((rtb_K1_n[i + 2] * rtb_mu_idx_1 *
      rtb_V_lookup2_o2 + rtb_mu_idx_1 * rtb_K1_n[i] * rtb_V_lookup_o2) +
      rtb_K1_n[i + 4] * rtb_mu_idx_1 * g3) + rtb_K1_n[i + 6] * rtb_mu_idx_1 *
      rtU.q)) + (((rtb_K1_d[i + 2] * w2 * rtb_V_lookup2_o2 + w2 * rtb_K1_d[i] *
                   rtb_V_lookup_o2) + rtb_K1_d[i + 4] * w2 * g3) + rtb_K1_d[i +
                 6] * w2 * rtU.q)) + (((rtb_K1[i + 2] * rtb_mu_idx_3 *
      rtb_V_lookup2_o2 + rtb_mu_idx_3 * rtb_K1[i] * rtb_V_lookup_o2) + rtb_K1[i
      + 4] * rtb_mu_idx_3 * g3) + rtb_K1[i + 6] * rtb_mu_idx_3 * rtU.q));
  }

  rtb_V_lookup_o2 = frac[0] + intrp2d_g(bpIndex_v, frac_v, rtConstP.Peq_de_Table,
    13U);
  g3 = frac[1] + intrp2d_g(bpIndex_w, frac_w, rtConstP.pooled12, 13U);

  // End of MATLAB Function: '<S1>/MATLAB Function'

  // MATLAB Function: '<S1>/Ajuste de salida para implementacion de'
  if (rtb_V_lookup_o2 < -0.3490658503988659) {
    // Outport: '<Root>/delta_e'
    rtY.delta_e = -1.0;
  } else if (rtb_V_lookup_o2 > 0.52359877559829882) {
    // Outport: '<Root>/delta_e'
    rtY.delta_e = 1.0;
  } else if ((rtb_V_lookup_o2 > -0.3490658503988659) && (rtb_V_lookup_o2 < 0.0))
  {
    // Outport: '<Root>/delta_e'
    rtY.delta_e = 2.8647889756541161 * rtb_V_lookup_o2;
  } else {
    // Outport: '<Root>/delta_e'
    rtY.delta_e = 1.9098593171027443 * rtb_V_lookup_o2;
  }

  // End of MATLAB Function: '<S1>/Ajuste de salida para implementacion de'

  // MATLAB Function: '<S1>/Ajuste de salida para implementacion dT'
  if (g3 < 0.0) {
    // Outport: '<Root>/delta_T'
    rtY.delta_T = 0.0;
  } else if (g3 > 4.873) {
    // Outport: '<Root>/delta_T'
    rtY.delta_T = 1.0;
  } else {
    // Outport: '<Root>/delta_T'
    rtY.delta_T = 0.20521239482864764 * g3;
  }

  // End of MATLAB Function: '<S1>/Ajuste de salida para implementacion dT'
  // End of Outputs for SubSystem: '<Root>/sistema_de_control'

  // Outport: '<Root>/wd'
  rtY.wd = rtb_gamma_lookup_o2;

  // Outport: '<Root>/V'
  rtY.V = rtb_V;

  // Outport: '<Root>/gamma'
  rtY.gamma = rtb_gamma;
}

// Model initialize function
void sistema_de_control::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));
}

// Constructor
sistema_de_control::sistema_de_control() :
  rtU(),
  rtY(),
  rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
sistema_de_control::~sistema_de_control() = default;

// Real-Time Model get method
sistema_de_control::RT_MODEL * sistema_de_control::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
