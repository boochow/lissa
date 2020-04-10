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
static float offset = 0;
static int32_t lfo_type = 0;

void DELFX_INIT(uint32_t platform, uint32_t api)
{
  s_lfo.reset();
  s_lfo.setF0(220.f,s_fs_recip);
  amp = 1.0;
  offset = 0;
  lfo_type = 0;
}

void DELFX_PROCESS(float *xn, uint32_t frames)
{
  float * __restrict x = xn;
  const float * x_e = x + 2*frames;

  for (; x != x_e; ) {
    float wave;
    s_lfo.cycle();
    
    switch(lfo_type) {
    case 1:
        wave = amp * s_lfo.saw_bi_off(offset);
        break;
    default:
        wave = amp * s_lfo.sine_bi_off(offset);
    }
    *(x++) = wave;
    *(x++);
  }
}


void DELFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  switch (index) {
  case k_user_delfx_param_time:
      s_lfo.setF0(osc_notehzf(valf * 128), s_fs_recip);
    break;
  case k_user_delfx_param_depth:
      amp = valf;
    break;
  case k_user_delfx_param_shift_depth:
      if (valf <= 0.499) {
          offset = valf * M_PI;
          lfo_type = 0;
      } else if (valf >= 0.501) {
          offset = (1.0 - valf) * M_PI;
          lfo_type = 1;
      }
    break;
  default:
    break;
  }
}

void DELFX_RESUME(void) {
  s_lfo.reset();
}
