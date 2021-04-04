/*
 * This file is part of COVID Calendar.
 *
 * COVID Calendar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * COVID Calendar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with COVID Calendar. If not, see <https://www.gnu.org/licenses/>.
 */
 
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <Time.h>
#include <TimeLib.h>

#include "config_struct.h"

Adafruit_AlphaNum4 alpha0 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alpha2 = Adafruit_AlphaNum4();
Adafruit_7segment numeric0 = Adafruit_7segment();
Adafruit_7segment numeric1 = Adafruit_7segment();
Adafruit_7segment numeric2 = Adafruit_7segment();
Adafruit_7segment matrix = Adafruit_7segment();

extern calConfig_t calendarConfig;

void render_setup()
{
  alpha0.begin(0x71);  // pass in the address
  alpha1.begin(0x72);  // pass in the address
  alpha2.begin(0x73);  // pass in the address

  numeric0.begin(0x74);
  numeric1.begin(0x75);
  numeric2.begin(0x76);

  matrix.begin(0x77);
}

static const uint8_t char_num_to_digit_num[] = 
{
  0,
  1,
  3,
  4,
  0,
  1,
  3,
  4,
  0,
  1,
  3,
  4
};

void render_time(time_t timestamp, uint8_t bright) {
  char line1[13], line2[13];
  uint32_t tmp;
  uint8_t digit_count = 0;
  char front_spacer[7];
  char end_spacer[7];
  uint8_t i, j;
  char c;
  bool dot;

  bright /= 16;

  if (bright == 0)
  {
    alpha0.clear();
    alpha1.clear();
    alpha2.clear();
    numeric0.clear();
    numeric1.clear();
    numeric2.clear();
    matrix.clear();

    alpha0.writeDisplay();
    alpha1.writeDisplay();
    alpha2.writeDisplay();
    numeric0.writeDisplay();
    numeric1.writeDisplay();
    numeric2.writeDisplay();
    matrix.writeDisplay();
    
    return;
  }

  alpha0.setBrightness(bright);
  alpha1.setBrightness(bright);
  alpha2.setBrightness(bright);
  numeric0.setBrightness(bright);
  numeric1.setBrightness(bright);
  numeric2.setBrightness(bright);
  matrix.setBrightness(bright);

  tmp = timestamp;

  while (tmp)
  {
    tmp /= 10;
    digit_count += 1;
  }

  for (i = 0; i < 6-(digit_count / 2); i++)
  {
    front_spacer[i] = ' ';
  }
  front_spacer[i] = 0;

  for (j = (i-1) + digit_count; j < 12; j++)
  {
    end_spacer[j] = ' ';
  }
  end_spacer[j] = 0;
  
  snprintf(line1, 13, "%s%u%s", front_spacer, timestamp, end_spacer);
  snprintf(line2, 13, " 0X%08X ", timestamp);

  for (i=0, j=0; i < strlen(line1); i++, j++)
  {
    c = line1[i];
    if (j < 4)
    {
      if (c != ' ')
      {
        numeric0.writeDigitNum(char_num_to_digit_num[j], c - '0', false);
      }
      else
      {
        numeric0.writeDigitRaw(char_num_to_digit_num[j], 0);
      }
    }
    else if (j < 8)
    {
      if (c != ' ')
      {
        numeric1.writeDigitNum(char_num_to_digit_num[j], c - '0', false);
      }
      else
      {
        numeric1.writeDigitRaw(char_num_to_digit_num[j], 0);
      }
    }
    else
    {
      if (c != ' ')
      {
        numeric2.writeDigitNum(char_num_to_digit_num[j], c - '0', false);
      }
      else
      {
        numeric2.writeDigitRaw(char_num_to_digit_num[j], 0);
      }
    }
  }

  alpha0.writeDigitAscii(0, line2[0], false);
  alpha0.writeDigitAscii(1, line2[1], false);
  alpha0.writeDigitAscii(2, line2[2], false);
  alpha0.writeDigitAscii(3, line2[3], false);
  alpha1.writeDigitAscii(0, line2[4], false);
  alpha1.writeDigitAscii(1, line2[5], false);
  alpha1.writeDigitAscii(2, line2[6], false);
  alpha1.writeDigitAscii(3, line2[7], false);
  alpha2.writeDigitAscii(0, line2[8], false);
  alpha2.writeDigitAscii(1, line2[9], false);
  alpha2.writeDigitAscii(2, line2[10], false);
  alpha2.writeDigitAscii(3, line2[11], false);

  matrix.writeDigitRaw(0, timestamp & 0xff);
  matrix.writeDigitRaw(1, (timestamp >> 8) & 0xff);
  matrix.writeDigitRaw(2, (timestamp >> 16) & 0xff);
  matrix.writeDigitRaw(3, (timestamp >> 24) & 0xff);
  
  alpha0.writeDisplay();
  alpha1.writeDisplay();
  alpha2.writeDisplay();
  numeric0.writeDisplay();
  numeric1.writeDisplay();
  numeric2.writeDisplay();
  matrix.writeDisplay();
}
