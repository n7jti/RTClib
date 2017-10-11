// Library for the RTC_DS3231
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


#define DS3231_ADDRESS  0x68
#define DS3231_ALARM    0x07
#define DS3231_CONTROL  0x0E
#define DS3231_STATUSREG 0x0F


// RTC based on the DS3231 chip connected via I2C and the Wire library
enum Ds3231SqwPinMode { DS3231_OFF = 0x01, DS3231_SquareWave1Hz = 0x00, DS3231_SquareWave1kHz = 0x08, DS3231_SquareWave4kHz = 0x10, DS3231_SquareWave8kHz = 0x18 };

class RTC_DS3231 {
public:
    boolean begin(void);
    static void adjust(const time_t& dt);
    bool lostPower(void);
    static time_t now();
    static Ds3231SqwPinMode readSqwPinMode();
    static void writeSqwPinMode(Ds3231SqwPinMode mode);

  
    bool isAlarm();         // Is the alarm sounding?
    void resetAlarm();      // Stop the alarm sounding? 
    
    static void putAlarm(const time_t& dt); // Set the alarm time
    static time_t getAlarm();               // Get the alarm time
};
