//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: controlador.cpp
//
// Code generated for Simulink model 'controlador'.
//
// Model version                  : 4.2
// Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
// C/C++ source code generated on : Wed Apr  2 19:07:53 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#pragma GCC diagnostic ignored "-Wfloat-equal"
#include "controlador.h"
#include <cmath>
#include "rtwtypes.h"
#include "cmath"
#include "limits"

extern real_T rt_atan2d_snf(real_T u0, real_T u1);
static uint32_T plook_binx(real_T u, const real_T bp[], uint32_T maxIndex,
  real_T *fraction);
static real_T intrp2d_g(const uint32_T bpIndex[], const real_T frac[], const
  real_T table[], const uint32_T stride);
static uint32_T binsearch_u32d(real_T u, const real_T bp[], uint32_T startIndex,
  uint32_T maxIndex);
static void rate_scheduler(controlador::RT_MODEL *const rtM);

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

extern "C"
{
  real_T rtNaN { -std::numeric_limits<real_T>::quiet_NaN() };

  real_T rtInf { std::numeric_limits<real_T>::infinity() };

  real_T rtMinusInf { -std::numeric_limits<real_T>::infinity() };

  real32_T rtNaNF { -std::numeric_limits<real32_T>::quiet_NaN() };

  real32_T rtInfF { std::numeric_limits<real32_T>::infinity() };

  real32_T rtMinusInfF { -std::numeric_limits<real32_T>::infinity() };
}

extern "C"
{
  // Return rtNaN needed by the generated code.
  static real_T rtGetNaN(void)
  {
    return rtNaN;
  }

  // Return rtNaNF needed by the generated code.
  static real32_T rtGetNaNF(void)
  {
    return rtNaNF;
  }
}

extern "C"
{
  // Test if value is infinite
  static boolean_T rtIsInf(real_T value)
  {
    return std::isinf(value);
  }

  // Test if single-precision value is infinite
  static boolean_T rtIsInfF(real32_T value)
  {
    return std::isinf(value);
  }

  // Test if value is not a number
  static boolean_T rtIsNaN(real_T value)
  {
    return std::isnan(value);
  }

  // Test if single-precision value is not a number
  static boolean_T rtIsNaNF(real32_T value)
  {
    return std::isnan(value);
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
//         This function updates active task flag for each subrate.
//         The function is called at model base rate, hence the
//         generated code self-manages all its subrates.
//
static void rate_scheduler(controlador::RT_MODEL *const rtM)
{
  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (rtM->Timing.TaskCounters.TID[1])++;
  if ((rtM->Timing.TaskCounters.TID[1]) > 3) {// Sample time: [0.01s, 0.0s]
    rtM->Timing.TaskCounters.TID[1] = 0;
  }
}

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

// Model step function
void controlador::step()
{
  real_T tmp[8];
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
  real_T rtb_Gain;
  real_T rtb_Peq_alpha;
  real_T rtb_Peq_dT;
  real_T rtb_V;
  real_T rtb_V_lookup_o2;
  real_T rtb_Vd;
  real_T rtb_gamma_lookup_o2;
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
  uint32_T rtb_V_lookup_o1;
  uint32_T rtb_gamma_lookup1_o1;

  // Outputs for Atomic SubSystem: '<Root>/controlador'
  // Gain: '<S6>/Gain' incorporates:
  //   Inport: '<Root>/gammad'

  rtb_Gain = 0.017453292519943295 * rtU.gammad;

  // PreLookup: '<S6>/gamma_lookup1'
  rtb_gamma_lookup1_o1 = plook_binx(rtb_Gain, rtConstP.pooled1, 12U,
    &rtb_V_lookup_o2);

  // Interpolation_n-D: '<S6>/Peq_dT' incorporates:
  //   Inport: '<Root>/Vd'
  //   PreLookup: '<S6>/V_lookup1'

  bpIndex[1] = plook_binx(rtU.Vd, rtConstP.pooled2, 13U, &rtb_gamma_lookup_o2);
  frac[0] = rtb_V_lookup_o2;
  frac[1] = rtb_gamma_lookup_o2;
  bpIndex[0] = rtb_gamma_lookup1_o1;
  rtb_Peq_dT = intrp2d_g(bpIndex, frac, rtConstP.pooled3, 13U);

  // MATLAB Function: '<S6>/MATLAB Function1' incorporates:
  //   Constant: '<S6>/Constant'
  //   Inport: '<Root>/Vd'

  if (rtb_Peq_dT < 0.0) {
    rtb_V_lookup_o1 = rtb_gamma_lookup1_o1 + /*MW:OvSatOk*/ 1U;
    if (rtb_gamma_lookup1_o1 + 1U < rtb_gamma_lookup1_o1) {
      rtb_V_lookup_o1 = MAX_uint32_T;
    }

    rtb_Vd = rtConstP.Constant_Value[static_cast<int32_T>(rtb_V_lookup_o1) - 1];
  } else if (rtb_Peq_dT > 4.7382) {
    rtb_V_lookup_o1 = rtb_gamma_lookup1_o1 + /*MW:OvSatOk*/ 1U;
    if (rtb_gamma_lookup1_o1 + 1U < rtb_gamma_lookup1_o1) {
      rtb_V_lookup_o1 = MAX_uint32_T;
    }

    rtb_Vd = rtConstP.Constant_Value[static_cast<int32_T>(rtb_V_lookup_o1) + 12];
  } else {
    rtb_Vd = rtU.Vd;
  }

  // End of MATLAB Function: '<S6>/MATLAB Function1'

  // Interpolation_n-D: '<S6>/Peq_alpha' incorporates:
  //   PreLookup: '<S6>/V_lookup2'

  bpIndex_0[1] = plook_binx(rtb_Vd, rtConstP.pooled2, 13U, &rtb_gamma_lookup_o2);
  frac_0[0] = rtb_V_lookup_o2;
  frac_0[1] = rtb_gamma_lookup_o2;
  bpIndex_0[0] = rtb_gamma_lookup1_o1;
  rtb_Peq_alpha = intrp2d_g(bpIndex_0, frac_0, rtConstP.Peq_alpha_Table, 13U);

  // MATLAB Function: '<S1>/MATLAB Function2' incorporates:
  //   Inport: '<Root>/theta'
  //   Inport: '<Root>/u'
  //   Inport: '<Root>/w'

  rtb_Peq_dT = rtU.theta - rt_atan2d_snf(rtU.w, rtU.u);
  rtb_V = std::sqrt(rtU.u * rtU.u + rtU.w * rtU.w);

  // PreLookup: '<S7>/gamma_lookup'
  rtb_gamma_lookup1_o1 = plook_binx(rtb_Peq_dT, rtConstP.pooled1, 12U,
    &rtb_gamma_lookup_o2);

  // PreLookup: '<S7>/V_lookup'
  rtb_V_lookup_o1 = plook_binx(rtb_V, rtConstP.pooled2, 13U, &rtb_V_lookup_o2);

  // Interpolation_n-D: '<S7>/k1de'
  frac_1[0] = rtb_gamma_lookup_o2;
  frac_1[1] = rtb_V_lookup_o2;
  bpIndex_1[0] = rtb_gamma_lookup1_o1;
  bpIndex_1[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k2de'
  frac_2[0] = rtb_gamma_lookup_o2;
  frac_2[1] = rtb_V_lookup_o2;
  bpIndex_2[0] = rtb_gamma_lookup1_o1;
  bpIndex_2[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k3de'
  frac_3[0] = rtb_gamma_lookup_o2;
  frac_3[1] = rtb_V_lookup_o2;
  bpIndex_3[0] = rtb_gamma_lookup1_o1;
  bpIndex_3[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k4de'
  frac_4[0] = rtb_gamma_lookup_o2;
  frac_4[1] = rtb_V_lookup_o2;
  bpIndex_4[0] = rtb_gamma_lookup1_o1;
  bpIndex_4[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k1dT'
  frac_5[0] = rtb_gamma_lookup_o2;
  frac_5[1] = rtb_V_lookup_o2;
  bpIndex_5[0] = rtb_gamma_lookup1_o1;
  bpIndex_5[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k2dT'
  frac_6[0] = rtb_gamma_lookup_o2;
  frac_6[1] = rtb_V_lookup_o2;
  bpIndex_6[0] = rtb_gamma_lookup1_o1;
  bpIndex_6[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k3dT'
  frac_7[0] = rtb_gamma_lookup_o2;
  frac_7[1] = rtb_V_lookup_o2;
  bpIndex_7[0] = rtb_gamma_lookup1_o1;
  bpIndex_7[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/k4dT'
  frac_8[0] = rtb_gamma_lookup_o2;
  frac_8[1] = rtb_V_lookup_o2;
  bpIndex_8[0] = rtb_gamma_lookup1_o1;
  bpIndex_8[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/Peq_de'
  frac_9[0] = rtb_gamma_lookup_o2;
  frac_9[1] = rtb_V_lookup_o2;
  bpIndex_9[0] = rtb_gamma_lookup1_o1;
  bpIndex_9[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/Peq_dT'
  frac_a[0] = rtb_gamma_lookup_o2;
  frac_a[1] = rtb_V_lookup_o2;
  bpIndex_a[0] = rtb_gamma_lookup1_o1;
  bpIndex_a[1] = rtb_V_lookup_o1;

  // Interpolation_n-D: '<S7>/Peq_wd'
  frac_b[0] = rtb_gamma_lookup_o2;
  frac_b[1] = rtb_V_lookup_o2;
  bpIndex_b[0] = rtb_gamma_lookup1_o1;
  bpIndex_b[1] = rtb_V_lookup_o1;
  rtb_V_lookup_o2 = intrp2d_g(bpIndex_b, frac_b, rtConstP.Peq_wd_Table, 13U);

  // MATLAB Function: '<S1>/MATLAB Function' incorporates:
  //   Inport: '<Root>/q'
  //   Inport: '<Root>/theta'
  //   Inport: '<Root>/u'
  //   Inport: '<Root>/w'
  //   Interpolation_n-D: '<S7>/Peq_dT'
  //   Interpolation_n-D: '<S7>/Peq_de'
  //   Interpolation_n-D: '<S7>/k1dT'
  //   Interpolation_n-D: '<S7>/k1de'
  //   Interpolation_n-D: '<S7>/k2dT'
  //   Interpolation_n-D: '<S7>/k2de'
  //   Interpolation_n-D: '<S7>/k3dT'
  //   Interpolation_n-D: '<S7>/k3de'
  //   Interpolation_n-D: '<S7>/k4dT'
  //   Interpolation_n-D: '<S7>/k4de'
  //   MATLAB Function: '<S6>/MATLAB Function'

  tmp[0] = -intrp2d_g(bpIndex_1, frac_1, rtConstP.k1de_Table, 13U);
  tmp[2] = -intrp2d_g(bpIndex_2, frac_2, rtConstP.k2de_Table, 13U);
  tmp[4] = -intrp2d_g(bpIndex_3, frac_3, rtConstP.k3de_Table, 13U);
  tmp[6] = -intrp2d_g(bpIndex_4, frac_4, rtConstP.k4de_Table, 13U);
  tmp[1] = -intrp2d_g(bpIndex_5, frac_5, rtConstP.k1dT_Table, 13U);
  tmp[3] = -intrp2d_g(bpIndex_6, frac_6, rtConstP.k2dT_Table, 13U);
  tmp[5] = -intrp2d_g(bpIndex_7, frac_7, rtConstP.k3dT_Table, 13U);
  tmp[7] = -intrp2d_g(bpIndex_8, frac_8, rtConstP.k4dT_Table, 13U);
  rtb_gamma_lookup_o2 = rtU.u - rtb_Vd * std::cos(rtb_Peq_alpha);
  rtb_Vd = rtU.w - rtb_V_lookup_o2;
  rtb_Gain = rtU.theta - (rtb_Peq_alpha + rtb_Gain);
  for (i = 0; i < 2; i++) {
    frac[i] = ((tmp[i + 2] * rtb_Vd + tmp[i] * rtb_gamma_lookup_o2) + tmp[i + 4]
               * rtb_Gain) + tmp[i + 6] * rtU.q;
  }

  rtb_Gain = frac[0] + intrp2d_g(bpIndex_9, frac_9, rtConstP.Peq_de_Table, 13U);
  rtb_Vd = frac[1] + intrp2d_g(bpIndex_a, frac_a, rtConstP.pooled3, 13U);

  // End of MATLAB Function: '<S1>/MATLAB Function'

  // MATLAB Function: '<S1>/Ajuste de salida para implementacion de'
  if (rtb_Gain < -0.3490658503988659) {
    // Outport: '<Root>/de'
    rtY.de = -1.0;
  } else if (rtb_Gain > 0.52359877559829882) {
    // Outport: '<Root>/de'
    rtY.de = 1.0;
  } else if ((rtb_Gain > -0.3490658503988659) && (rtb_Gain < 0.0)) {
    // Outport: '<Root>/de'
    rtY.de = 2.8647889756541161 * rtb_Gain;
  } else {
    // Outport: '<Root>/de'
    rtY.de = 1.9098593171027443 * rtb_Gain;
  }

  // End of MATLAB Function: '<S1>/Ajuste de salida para implementacion de'

  // MATLAB Function: '<S1>/Ajuste de salida para implementacion dT'
  if (rtb_Vd < 0.0) {
    // Outport: '<Root>/dT'
    rtY.dT = 0.0;
  } else if (rtb_Vd > 4.873) {
    // Outport: '<Root>/dT'
    rtY.dT = 1.0;
  } else {
    // Outport: '<Root>/dT'
    rtY.dT = 0.20521239482864764 * rtb_Vd;
  }

  // End of MATLAB Function: '<S1>/Ajuste de salida para implementacion dT'
  // End of Outputs for SubSystem: '<Root>/controlador'

  // Outport: '<Root>/wd'
  rtY.wd = rtb_V_lookup_o2;

  // Outport: '<Root>/V'
  rtY.V = rtb_V;

  // Outport: '<Root>/gamma'
  rtY.gamma = rtb_Peq_dT;
  rate_scheduler((&rtM));
}

// Model initialize function
void controlador::initialize()
{
  // (no initialization code required)
}

const char_T* controlador::RT_MODEL::getErrorStatus() const
{
  return (errorStatus);
}

void controlador::RT_MODEL::setErrorStatus(const char_T* const volatile
  aErrorStatus)
{
  (errorStatus = aErrorStatus);
}

// Constructor
controlador::controlador() :
  rtU(),
  rtY(),
  rtM()
{
  // Currently there is no constructor body generated.
}

// Destructor
// Currently there is no destructor body generated.
controlador::~controlador() = default;

// Real-Time Model get method
controlador::RT_MODEL * controlador::getRTM()
{
  return (&rtM);
}

//
// File trailer for generated code.
//
// [EOF]
//
