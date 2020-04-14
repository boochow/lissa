/*
 * File: lissa.cpp
 */

#include "userdelfx.h"

#include "simplelfo.hpp"

#include "osc_api.h"

#include "delayline.hpp"

// lowest note frequency is 8.2 Hz. 
// longest delay is wave length/4 = 1/(8.2 * 4).
// so 1/32 sec is sufficient for delay buffer.
#define BUFSIZE 1500

// lfo default frequency. Actually, this value is not used
// because lfo freqency is changed by DELFX_PARAM() when
// the delay instance is started and its value is always 330
// (frequency of MIDI note #64, which is 0.5*128)
#define DEFAULTFREQ 330

static dsp::DelayLine s_delay;

static __sdram float s_delay_ram[BUFSIZE];

static float s_len_z, s_len;

static dsp::SimpleLFO s_lfo;
static const float s_fs_recip = 1.f / 48000.f;
static float amp = 1.0;
static float offset = 0;

enum {
    SWEEP_SINE = 0,
    SWEEP_SAW = 1,
    SWEEP_IN = 2,
};
static int32_t lfo_type = 0;

void DELFX_INIT(uint32_t platform, uint32_t api)
{
  s_delay.setMemory(s_delay_ram, BUFSIZE);  
  s_len = s_len_z = 48000.f / (4 * DEFAULTFREQ);
  s_lfo.reset();
  s_lfo.setF0(DEFAULTFREQ, s_fs_recip);
  amp = 1.0;
  offset = 0;
  lfo_type = 0;
}

void DELFX_PROCESS(float *xn, uint32_t frames)
{
  float * __restrict x = xn;
  const float * x_e = x + 2*frames;

  const float len = s_len;
  float len_z = s_len_z;

  for (; x != x_e; ) {
    len_z = linintf(0.00004f, len_z, len);
    const float r = s_delay.readFrac(len_z);

    float wave;
    s_lfo.cycle();
    
    switch(lfo_type) {
    case SWEEP_SINE:
        wave = amp * s_lfo.sine_bi_off(offset);
        break;
    case SWEEP_SAW:
        wave = amp * s_lfo.saw_bi_off(offset);
        break;
    case SWEEP_IN:
        len_z = linintf(0.00004f, len_z, len);
        wave = 2.5 * amp * s_delay.readFrac(len_z);
        break;
    default:
        break;
    }
    s_delay.write(*x);
    *(x++) = wave;
    *(x++);
  }
  s_len_z = len_z;
}


void DELFX_PARAM(uint8_t index, int32_t value)
{
  const float valf = q31_to_f32(value);
  float hz;

  switch (index) {
  case k_user_delfx_param_time:
      hz = osc_notehzf(valf * 128);
      s_lfo.setF0(hz, s_fs_recip);
      s_len = 48000.f / (4 * hz);      /* delay length = 1/4 * wave length */
    break;
  case k_user_delfx_param_depth:
      amp = valf;
    break;
  case k_user_delfx_param_shift_depth:
      if (valf <= 0.4) {
          offset = valf * 2.5 - 0.5;   /* offset = -0.5 .. 0.5 */
          lfo_type = SWEEP_SINE;
      } else if (valf >= 0.6) {
          offset = (1.0 - valf) * 2.5; /* offset = 0.0 .. 1.0 */
          lfo_type = SWEEP_SAW;
      } else {
          lfo_type = SWEEP_IN;
      }
    break;
  default:
    break;
  }
}

void DELFX_RESUME(void) {
  s_lfo.reset();
}
