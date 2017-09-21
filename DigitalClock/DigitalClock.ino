// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include "Adafruit_LEDBackpack.h"
#include "crazydigits.h"

Adafruit_7segment matrix = Adafruit_7segment();
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup () {

  matrix.begin(0x70);
  
  rtc.begin();

  if (rtc.lostPower()) {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  matrix.drawColon(true);
  matrix.writeColon();
}

void loop () {
    DateTime now = rtc.now();

    //hours is in 24 hour format, let's get it in 2-12 hour ranges
    int moduloHours = now.hour() % 12;

    // I want the 0 hour to actually be '12' and not '0'
    if (moduloHours == 0)
    {
      moduloHours = 12;
    }
    
    int tenHours = moduloHours / 10;
    int hours    = moduloHours % 10;
    int tenMinutes = now.minute() / 10;
    int minutes    = now.minute() % 10;

    switch(now.minute() % 4){
    case 0:
      // Top of the hour!  All normal!
      if (tenHours > 0)
      {
        matrix.writeDigitNum(0, tenHours,   true);
      }
      else
      {
        matrix.writeDigitRaw(0, 1 << 7);
      }
      matrix.writeDigitNum(1, hours,      false);
      matrix.writeDigitNum(3, tenMinutes, false);
      matrix.writeDigitNum(4, minutes,    false);
      break;
    case 1:
      // 15-30 -- backwards
      if (tenHours > 0)
      {
        matrix.writeDigitRaw(4, back[tenHours]);
      }
      else
      {
        matrix.writeDigitRaw(4, 0);
      }
      matrix.writeDigitRaw(3, back[hours]);
      matrix.writeDigitRaw(1, back[tenMinutes] | (1 << 7));
      matrix.writeDigitRaw(0, back[minutes]);
      break;
    case 2:
      // 30-45 Upsidown
      if (tenHours > 0)
      {
        matrix.writeDigitRaw(0, up[tenHours]);
      }
      else
      {
        matrix.writeDigitRaw(0,0);
      }
      matrix.writeDigitRaw(1, up[hours]);
      matrix.writeDigitRaw(3, up[tenMinutes] | (1<< 7));
      matrix.writeDigitRaw(4, up[minutes]);
      break;
      break;
    case 3:
    // 45-59 -- upsideown and backwards
      if (tenHours > 0)
      {
        matrix.writeDigitRaw(4, upback[tenHours] | (1 << 7));
      }
      else
      {
        matrix.writeDigitRaw(4, 1 << 7);
      }
      matrix.writeDigitRaw(3, upback[hours]);
      matrix.writeDigitRaw(1, upback[tenMinutes]);
      matrix.writeDigitRaw(0, upback[minutes]);
      break;
    }

    matrix.writeDisplay();

    delay(1000);
}
