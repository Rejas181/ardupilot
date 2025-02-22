//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: controlador.h
//
// Code generated for Simulink model 'controlador'.
//
// Model version                  : 2.31
// Simulink Coder version         : 23.2 (R2023b) 01-Aug-2023
// C/C++ source code generated on : Fri Feb 21 18:43:09 2025
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-A (64-bit)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_controlador_h_
#define RTW_HEADER_controlador_h_
#include "AP_gain_scheduling/rtwtypes.h"
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

// Class declaration for model controlador
class controlador final
{
  // public data and function members
 public:
  // Constant parameters (default storage)
  struct ConstP {
    // Expression: Vind
    //  Referenced by: '<S4>/Constant'

    real_T Constant_Value[26];

    // Pooled Parameter (Expression: [gammali:pgamma:gammals])
    //  Referenced by:
    //    '<S4>/gamma_lookup1'
    //    '<S5>/gamma_lookup'

    real_T pooled1[13];

    // Pooled Parameter (Expression: [Vli:pV:Vls])
    //  Referenced by:
    //    '<S4>/V_lookup1'
    //    '<S4>/V_lookup2'
    //    '<S5>/V_lookup'

    real_T pooled2[14];

    // Pooled Parameter (Expression: Peq_dT)
    //  Referenced by:
    //    '<S4>/Peq_dT'
    //    '<S5>/Peq_dT'

    real_T pooled3[182];

    // Expression: Peq_alpha
    //  Referenced by: '<S4>/Peq_alpha'

    real_T Peq_alpha_Table[182];

    // Expression: k1de
    //  Referenced by: '<S5>/k1de'

    real_T k1de_Table[182];

    // Expression: k2de
    //  Referenced by: '<S5>/k2de'

    real_T k2de_Table[182];

    // Expression: k3de
    //  Referenced by: '<S5>/k3de'

    real_T k3de_Table[182];

    // Expression: k4de
    //  Referenced by: '<S5>/k4de'

    real_T k4de_Table[182];

    // Expression: k1dT
    //  Referenced by: '<S5>/k1dT'

    real_T k1dT_Table[182];

    // Expression: k2dT
    //  Referenced by: '<S5>/k2dT'

    real_T k2dT_Table[182];

    // Expression: k3dT
    //  Referenced by: '<S5>/k3dT'

    real_T k3dT_Table[182];

    // Expression: k4dT
    //  Referenced by: '<S5>/k4dT'

    real_T k4dT_Table[182];

    // Expression: Peq_de
    //  Referenced by: '<S5>/Peq_de'

    real_T Peq_de_Table[182];

    // Expression: Peq_w
    //  Referenced by: '<S5>/Peq_wd'

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
    real_T de;                         // '<Root>/de'
    real_T dT;                         // '<Root>/dT'
    real_T wd;                         // '<Root>/wd'
    real_T V;                          // '<Root>/V'
    real_T gamma;                      // '<Root>/gamma'
  };

  // Real-time Model Data Structure
  struct RT_MODEL {
    const char_T * volatile errorStatus;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      struct {
        uint8_T TID[2];
      } TaskCounters;
    } Timing;
  };

  // Copy Constructor
  controlador(controlador const&) = delete;

  // Assignment Operator
  controlador& operator= (controlador const&) & = delete;

  // Move Constructor
  controlador(controlador &&) = delete;

  // Move Assignment Operator
  controlador& operator= (controlador &&) = delete;

  // Real-Time Model get method
  controlador::RT_MODEL * getRTM();

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  controlador();

  // Destructor
  ~controlador();

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL rtM;
};

// Constant parameters (default storage)
extern const controlador::ConstP rtConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S4>/Scope' : Unused code path elimination


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
//  hilite_system('gain_scheduling_simulacion_V_gam_/controlador')    - opens subsystem gain_scheduling_simulacion_V_gam_/controlador
//  hilite_system('gain_scheduling_simulacion_V_gam_/controlador/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'gain_scheduling_simulacion_V_gam_'
//  '<S1>'   : 'gain_scheduling_simulacion_V_gam_/controlador'
//  '<S2>'   : 'gain_scheduling_simulacion_V_gam_/controlador/MATLAB Function'
//  '<S3>'   : 'gain_scheduling_simulacion_V_gam_/controlador/MATLAB Function2'
//  '<S4>'   : 'gain_scheduling_simulacion_V_gam_/controlador/alphad'
//  '<S5>'   : 'gain_scheduling_simulacion_V_gam_/controlador/selector'
//  '<S6>'   : 'gain_scheduling_simulacion_V_gam_/controlador/alphad/MATLAB Function'
//  '<S7>'   : 'gain_scheduling_simulacion_V_gam_/controlador/alphad/MATLAB Function1'

#endif                                 // RTW_HEADER_controlador_h_

//
// File trailer for generated code.
//
// [EOF]
//
