/**
 * The MIT License (MIT)
 * Copyright (c) 2017 by Alan Ludwig 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// Time Scroller
#include <Arduino.h>
#include <Time.h>
#include "DS3231.h"
#include "TimeScroller.h"
#include "bitmap.h"

TimeScroller::TimeScroller(LEDMatrix &matrix, RTC_DS3231 &rtc)
  : _matrix(matrix)
  , _rtc(rtc)
{
  
}

String TimeScroller::timeStr()
{
  time_t time = _rtc.now();
  String timeString;
  // hours
  int hours = hourFormat12(time);
  timeString += hours;
  
  timeString += ":";

  // minutes 
  int minutes = minute(time);
  if (minutes < 10) timeString += 0;
  timeString += minutes;

  return timeString;
}
 
void TimeScroller::displayTime(time_t time)
{
  // hours
  int hours = hourFormat12(time);
  bool am = isAM(time);
  
  int hours_tens = hours / 10;
  int hours_ones = hours % 10;

   // minutes 
  int minutes = minute(time);
  int minutes_tens = minutes / 10;
  int minutes_ones = minutes % 10;

  _matrix.backBuffer();
  _matrix.drawBitmap( 0, 0, clock_number_bmp[hours_tens],    3, 5, 32);
  _matrix.drawBitmap( 4, 0, clock_number_bmp[hours_ones],    3, 5, 32);
  _matrix.drawBitmap( 8, 0, clock_number_bmp[minutes_tens],  3, 5, 32);
  _matrix.drawBitmap(12, 0, clock_number_bmp[minutes_ones],  3, 5, 32);
  if (am == true)
  {
    drawA();
  }
  else
  {
    drawP();
  }
  _matrix.flip();
}

void TimeScroller::scrollTime()
{
  String timeString = timeStr();
  //Serial.println("Time String: " + timeString);

  for(int8_t x = 16; x >= -24; x--)
  {
    _matrix.backBuffer();
    _matrix.setCursor(x,0);
    _matrix.print(timeString);
    _matrix.flip();
  }

  
}

void TimeScroller::drawA()
{
  static uint8_t A[] = {B10110000};
  _matrix.drawBitmap(0, 6, A, 4, 1, 32);
}

void TimeScroller::drawP()
{
  static uint8_t P[] = {B10110110, B10000000};
  _matrix.drawBitmap(0, 6, P, 9, 1, 32);
}

