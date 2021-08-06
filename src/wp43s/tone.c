/* This file is part of 43S.
 *
 * 43S is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * 43S is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 43S.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tone.h"

#include "screen.h"

#include <stdio.h>
#if defined(TESTSUITE_BUILD)
#elif defined(PC_BUILD) && defined(__MINGW64__)
  #define NOMINMAX
  #include <windows.h>
#elif defined(PC_BUILD) && defined(WITH_PULSEAUDIO)
  #include <pulse/simple.h>
#endif

#include "wp43s.h"

TO_QSPI uint32_t frequency[10] = {164814, 220000, 246942, 277183, 293665, 329628, 369995, 415305, 440000, 554365};

static void playTone(uint16_t toneNum) {
  if(getSystemFlag(FLAG_QUIET)) return;

#if defined(TESTSUITE_BUILD)
#elif defined(PC_BUILD) && defined(__MINGW64__)
  char filename[32];
  if(toneNum < 10) {
    sprintf(filename, "res\\tone\\tone%" PRIu16 ".wav", toneNum);
    PlaySoundA(filename, NULL, SND_FILENAME | SND_SYNC | SND_NODEFAULT);
  }
#elif defined(PC_BUILD) && defined(WITH_PULSEAUDIO)
  if(toneNum < 10) {
    pa_simple *s;
    pa_sample_spec ss;

    ss.format   = PA_SAMPLE_S16NE;
    ss.channels = 1;
    ss.rate     = 44100;

    s = pa_simple_new(NULL, "WP 43S", PA_STREAM_PLAYBACK, NULL, "BEEP/TONE", &ss, NULL, NULL, NULL);

    if(s) {
      size_t bufSize = ss.rate / 4;
      int16_t *samples = (int16_t *)malloc(bufSize * sizeof(int16_t));
      int errCode;
      uint64_t p = 0;

      for(unsigned int i = 0; i < bufSize; ++i) {
        samples[i] = p < ((uint64_t)ss.rate * 500) ? 10362 : -10362;
        p += (uint64_t)frequency[toneNum];
        p %= (uint64_t)ss.rate * 1000;
      }

      pa_simple_write(s, samples, bufSize * sizeof(int16_t), &errCode);
      pa_simple_drain(s, &errCode);
      free(samples);
      pa_simple_free(s);
    }
  }
#elif defined(DMCP_BUILD)
  if(toneNum < 10) {
    start_buzzer_freq(frequency[toneNum]);
    sys_delay(250);
    stop_buzzer();
  }
#endif
}

void fnTone(uint16_t toneNum) {
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  playTone(toneNum);
}

void fnBeep(uint16_t unusedButMandatoryParameter) {
  #ifdef DMCP_BUILD
    lcd_refresh();
  #else // !DMCP_BUILD
    refreshLcd(NULL);
  #endif // DMCP_BUILD

  playTone(8);
  playTone(5);
  playTone(9);
  playTone(8);
}
