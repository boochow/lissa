/*
 * File: lissa.cpp
 */

#include "userdelfx.h"

#include "simplelfo.hpp"

#include "osc_api.h"

static dsp::SimpleLFO s_lfo;
static float s_param_z;
static const float s_fs_recip = 1.f / 48000.f;
static float amp = 1.0;

void DELFX_INIT(uint32_t platform, uint32_t api)
{
  s_lfo.reset();
  s_lfo.setF0(220.f,s_fs_recip);
}

void DELFX_PROCESS(float *xn, uint32_t frames)
{
  float * __restrict x = xn;
  const float * x_e = x + 2*frames;

//  const float p = s_param;
  float p_z = s_param_z;
  
  for (; x != x_e; ) {
    s_lfo.cycle();
    
    float wave = amp * s_lfo.sine_bi();
    
    *(x++); // leave left channel un-delayed for comparitive earing
    *(x++) = wave;
  }
  s_param_z = p_z;
}


void DELFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_delfx_param_time:
//    s_lfo.setF0(20.f + 420.f * valf, s_fs_recip);
      s_lfo.setF0(osc_notehzf(valf * 128), s_fs_recip);
    break;
  case k_user_delfx_param_depth:
    amp = valf;
    break;
  case k_user_delfx_param_shift_depth:
    break;
  default:
    break;
  }
}

