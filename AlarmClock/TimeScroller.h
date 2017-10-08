#pragma once
#include "UdpNtpClient.h"
#include "LedMatrix.h"

// TimeScroller.h

class TimeScroller
{
public:
  TimeScroller(LEDMatrix &matrix, RTC_DS3231 &rtc);
  String timeStr();
  void displayTime(time_t time);
  void scrollTime();

private: 

  void drawA();
  void drawP();

  LEDMatrix &_matrix;
  RTC_DS3231 &_rtc;
};

