#pragma once

// LEDMatrix.h
//
//
//  This extends the Adafruit_IS31FL3731_Wing class to display
//  scrolling text on the LEDMatrix without flashing.  This is
//  done by displaying to the hardware's back buffer and then 
//  flipping the back buffer to the active display. 
//
//  call backBuffer() to render everyting into a non-displayed back-buffer
//  call flip() to flip the back buffer to the foreground quickly without flashing

#include <Adafruit_IS31FL3731.h>


class LEDMatrix : public Adafruit_IS31FL3731_Wing
{
public:
  LEDMatrix();

  void backBuffer();
  void flip();

private:
  uint8_t _frame;
};

