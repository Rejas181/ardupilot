//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: sistema_de_control.h
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
#ifndef RTW_HEADER_sistema_de_control_h_
#define RTW_HEADER_sistema_de_control_h_
#include "AP_fuzzy_logic/rtwtypes.h"
#include <stddef.h>

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

extern "C"
{
  static real_T rtGetNaN(void);
  static real32_T rtGetNaNF(void);
}                                      // extern "C"

#define NOT_USING_NONFINITE_LITERALS   1

extern "C"
{
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  static void rt_InitInfAndNaN(size_t realSize);
  static boolean_T rtIsInf(real_T value);
  static boolean_T rtIsInfF(real32_T value);
  static boolean_T rtIsNaN(real_T value);
  static boolean_T rtIsNaNF(real32_T value);
  struct BigEndianIEEEDouble {
    struct {
      uint32_T wordH;
      uint32_T wordL;
    } words;
  };

  struct LittleEndianIEEEDouble {
    struct {
      uint32_T wordL;
      uint32_T wordH;
    } words;
  };

  struct IEEESingle {
    union {
      real32_T wordLreal;
      uint32_T wordLuint;
    } wordL;
  };
}                                      // extern "C"

extern "C"
{
  static real_T rtGetInf(void);
  static real32_T rtGetInfF(void);
  static real_T rtGetMinusInf(void);
  static real32_T rtGetMinusInfF(void);
}                                      // extern "C"

// Class declaration for model sistema_de_control
class sistema_de_control final
{
  // public data and function members
 public:
  // Constant parameters (default storage)
  struct ConstP {
    // Expression: Vind
    //  Referenced by: '<S6>/Constant'

    real_T Constant_Value[26];

    // Pooled Parameter (Expression: [gammali:pgamma:gammals])
    //  Referenced by:
    //    '<S6>/gamma_lookup1'
    //    '<S7>/gamma_lookup'

    real_T pooled2[13];

    // Pooled Parameter (Expression: [Vli:pV:Vls])
    //  Referenced by:
    //    '<S6>/V_lookup1'
    //    '<S6>/V_lookup2'
    //    '<S7>/V_lookup'

    real_T pooled3[14];

    // Pooled Parameter (Expression: k1deLD)
    //  Referenced by:
    //    '<S11>/k1de'
    //    '<S12>/k1de'
    //    '<S13>/k1de'
    //    '<S14>/k1de'

    real_T pooled4[182];

    // Pooled Parameter (Expression: k2deLD)
    //  Referenced by:
    //    '<S11>/k2de'
    //    '<S12>/k2de'
    //    '<S13>/k2de'
    //    '<S14>/k2de'

    real_T pooled5[182];

    // Pooled Parameter (Expression: k3deLD)
    //  Referenced by:
    //    '<S11>/k3de'
    //    '<S12>/k3de'
    //    '<S13>/k3de'
    //    '<S14>/k3de'

    real_T pooled6[182];

    // Pooled Parameter (Expression: k4deLD)
    //  Referenced by:
    //    '<S11>/k4de'
    //    '<S12>/k4de'
    //    '<S13>/k4de'
    //    '<S14>/k4de'

    real_T pooled7[182];

    // Pooled Parameter (Expression: k1dTLD)
    //  Referenced by:
    //    '<S11>/k1dT'
    //    '<S12>/k1dT'
    //    '<S13>/k1dT'
    //    '<S14>/k1dT'

    real_T pooled8[182];

    // Pooled Parameter (Expression: k2dTLD)
    //  Referenced by:
    //    '<S11>/k2dT'
    //    '<S12>/k2dT'
    //    '<S13>/k2dT'
    //    '<S14>/k2dT'

    real_T pooled9[182];

    // Pooled Parameter (Expression: k3dTLD)
    //  Referenced by:
    //    '<S11>/k3dT'
    //    '<S12>/k3dT'
    //    '<S13>/k3dT'
    //    '<S14>/k3dT'

    real_T pooled10[182];

    // Pooled Parameter (Expression: k4dTLD)
    //  Referenced by:
    //    '<S11>/k4dT'
    //    '<S12>/k4dT'
    //    '<S13>/k4dT'
    //    '<S14>/k4dT'

    real_T pooled11[182];

    // Expression: Peq_de
    //  Referenced by: '<S7>/Peq_de'

    real_T Peq_de_Table[182];

    // Pooled Parameter (Expression: Peq_dT)
    //  Referenced by:
    //    '<S6>/Peq_dT'
    //    '<S7>/Peq_dT'

    real_T pooled12[182];

    // Expression: Peq_alpha
    //  Referenced by: '<S6>/Peq_alpha'

    real_T Peq_alpha_Table[182];

    // Expression: Peq_w
    //  Referenced by: '<S7>/Peq_wd'

    real_T Peq_wd_Table[182];
  };

  // External inputs (root inport signals with default storage)
  struct ExtU {
    real_T u;                          // '<Root>/u'
    real_T w;                          // '<Root>/w'
    real_T theta;                      // '<Root>/theta'
    real_T q;                          // '<Root>/q'
    real_T gammad;                     // '<Root>/gammad'
    real_T Vd;                         // '<Root>/Vd'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY {
    real_T delta_e;                    // '<Root>/delta_e'
    real_T delta_T;                    // '<Root>/delta_T'
    real_T wd;                         // '<Root>/wd'
    real_T V;                          // '<Root>/V'
    real_T gamma;                      // '<Root>/gamma'
  };

  // Real-time Model Data Structure
  struct RT_MODEL {
    const char_T * volatile errorStatus;
  };

  // Copy Constructor
  sistema_de_control(sistema_de_control const&) = delete;

  // Assignment Operator
  sistema_de_control& operator= (sistema_de_control const&) & = delete;

  // Move Constructor
  sistema_de_control(sistema_de_control &&) = delete;

  // Move Assignment Operator
  sistema_de_control& operator= (sistema_de_control &&) = delete;

  // Real-Time Model get method
  sistema_de_control::RT_MODEL * getRTM();

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  sistema_de_control();

  // Destructor
  ~sistema_de_control();

  // private data and function members
 private:
  // private member function(s) for subsystem '<S11>/generador de matriz'
  static void generadordematriz(real_T rtu_k1e, real_T rtu_k2e, real_T rtu_k3e,
    real_T rtu_k4e, real_T rtu_k1dT, real_T rtu_k2dT, real_T rtu_k3dT, real_T
    rtu_k4dT, real_T rty_K1[8]);

  // Real-Time Model
  RT_MODEL rtM;
};

// Constant parameters (default storage)
extern const sistema_de_control::ConstP rtConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S1>/Scope' : Unused code path elimination
//  Block '<S1>/Scope1' : Unused code path elimination
//  Block '<S6>/Scope' : Unused code path elimination
//  Block '<S7>/Data Type Conversion' : Unused code path elimination
//  Block '<S7>/Data Type Conversion1' : Unused code path elimination
//  Block '<S7>/Scope1' : Unused code path elimination


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control')    - opens subsystem Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control
//  hilite_system('Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Logica_Difusa_simulacion_V_gam_ACTUAL'
//  '<S1>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control'
//  '<S2>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/Ajuste de salida para implementacion dT'
//  '<S3>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/Ajuste de salida para implementacion de'
//  '<S4>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/MATLAB Function'
//  '<S5>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/MATLAB Function2'
//  '<S6>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/alphad'
//  '<S7>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia'
//  '<S8>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/alphad/MATLAB Function'
//  '<S9>'   : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/alphad/MATLAB Function1'
//  '<S10>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/MATLAB Function'
//  '<S11>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto1'
//  '<S12>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto2'
//  '<S13>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto3'
//  '<S14>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto4'
//  '<S15>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto1/generador de matriz'
//  '<S16>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto2/generador de matriz'
//  '<S17>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto3/generador de matriz'
//  '<S18>'  : 'Logica_Difusa_simulacion_V_gam_ACTUAL/sistema_de_control/selector//memebresia/Punto4/generador de matriz'

#endif                                 // RTW_HEADER_sistema_de_control_h_

//
// File trailer for generated code.
//
// [EOF]
//
