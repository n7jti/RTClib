// Code by JeeLabs http://news.jeelabs.org/code/
// Released to the public domain! Enjoy!


/**
 * While the base library is public-domain.  Alan Ludwig's additions are coverd by the MIT license. These
 * include the alarm based functions, isAlarm, resetAlarm, getAlarm, putAlarm
 * 
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

#include <Wire.h>
#include <Arduino.h> // capital A so it is error prone on case-sensitive filesystems
#include <TimeLib.h>
#include "DS3231.h"

static uint8_t read_i2c_register(uint8_t addr, uint8_t reg) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.endTransmission();

  Wire.requestFrom(addr, (byte)1);
  return Wire.read();
}

static void write_i2c_register(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write((byte)reg);
  Wire.write((byte)val);
  Wire.endTransmission();
}

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

////////////////////////////////////////////////////////////////////////////////
// RTC_DS3231 implementation

boolean RTC_DS3231::begin(void) {
  Wire.begin();
  return true;
}

bool RTC_DS3231::lostPower(void) {
  return (read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG) >> 7);
}

void RTC_DS3231::adjust(const time_t& dt) {
   
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)0); // start at location 0
  Wire.write(bin2bcd(second(dt)));
  Wire.write(bin2bcd(minute(dt)));
  Wire.write(bin2bcd(hour(dt)));
  Wire.write(bin2bcd(0));
  Wire.write(bin2bcd(day(dt)));
  Wire.write(bin2bcd(month(dt)));
  Wire.write(bin2bcd(year(dt) - 2000));
  Wire.endTransmission();

  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  statreg &= ~0x80; // flip OSF bit
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg);
}

time_t RTC_DS3231::now() {
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)0);  
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 7);
  uint8_t ss = bcd2bin(Wire.read());
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  Wire.read();
  uint8_t d = bcd2bin(Wire.read());
  uint8_t m = bcd2bin(Wire.read());
  uint16_t y = bcd2bin(Wire.read()) + 2000; 

  tmElements_t te;
  te.Second = ss;
  te.Minute = mm;
  te.Hour = hh;
  te.Day = d;
  te.Month = m;
  te.Year = y - 1970;  // Time element year is offset from 1970
  
  time_t dt; 
  dt = makeTime(te);
  
  return dt;
}

Ds3231SqwPinMode RTC_DS3231::readSqwPinMode() {
  int mode;

  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write(DS3231_CONTROL);
  Wire.endTransmission();
  
  Wire.requestFrom((uint8_t)DS3231_ADDRESS, (uint8_t)1);
  mode = Wire.read();

  mode &= 0x93;
  return static_cast<Ds3231SqwPinMode>(mode);
}

void RTC_DS3231::writeSqwPinMode(Ds3231SqwPinMode mode) {
  uint8_t ctrl;
  ctrl = read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL);

  ctrl &= ~0x04; // turn off INTCON
  ctrl &= ~0x18; // set freq bits to 0

  if (mode == DS3231_OFF) {
    ctrl |= 0x04; // turn on INTCN
  } else {
    ctrl |= mode;
  } 
  write_i2c_register(DS3231_ADDRESS, DS3231_CONTROL, ctrl);

  //Serial.println( read_i2c_register(DS3231_ADDRESS, DS3231_CONTROL), HEX);
}

bool RTC_DS3231::isAlarm(void)
{
  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  return (statreg & 0x01) == 1;
}

void RTC_DS3231::resetAlarm()
{
  // write a 0 to the first bit of the DS3231_STATUSREG
  uint8_t statreg = read_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG);
  statreg &= (byte)~0x01; // turn off Alarm 1 flag. 
  write_i2c_register(DS3231_ADDRESS, DS3231_STATUSREG, statreg); 
}

void RTC_DS3231::putAlarm(const time_t& dt)
{
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)DS3231_ALARM);   // start at the alarm regiters
  Wire.write(bin2bcd(second(dt)) & 0x7F );  // sets A1M1 low 
  Wire.write(bin2bcd(minute(dt)) & 0x7F );  // sets A1M2 low
  Wire.write(bin2bcd(hour(dt)) & 0x7F );    // sets A1M3 low
  Wire.write((byte)0x81);                 // set A1M4 high, 
  Wire.endTransmission();
}

time_t RTC_DS3231::getAlarm()
{
  Wire.beginTransmission(DS3231_ADDRESS);
  Wire.write((byte)DS3231_ALARM);  
  Wire.endTransmission();

  Wire.requestFrom(DS3231_ADDRESS, 4);
  uint8_t ss = bcd2bin(Wire.read());
  uint8_t mm = bcd2bin(Wire.read());
  uint8_t hh = bcd2bin(Wire.read());
  Wire.read();

  tmElements_t te;
  memset(&te, 0, sizeof(te));
  te.Second = ss;
  te.Minute = mm;
  te.Hour = hh;
  te.Day = 1;
  te.Month = 1;
  te.Year = 0;
 
  time_t dt; 
  dt = makeTime(te);
  
  return dt;  
}


