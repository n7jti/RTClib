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



// Bitmaph.h

// Bitmaps for 3x5 numbers for clock. 
static const uint8_t 
zero_bmp[] = 
{
  B11100000,
  B10100000,
  B10100000,
  B10100000,
  B11100000
},
one_bmp[] = 
{ 
  B11000000,
  B01000000,
  B01000000,
  B01000000,
  B11100000
},
two_bmp[] =
{
  B11100000,
  B00100000,
  B11100000,
  B10000000,
  B11100000,
},
three_bmp[] = 
{
  B11100000,
  B00100000,
  B01100000,
  B00100000,
  B11100000
},
four_bmp[] = 
{
  B10100000,
  B10100000,
  B11100000,
  B00100000,
  B00100000
},
five_bmp [] = 
{
  B11100000,
  B10000000,
  B11100000,
  B00100000,
  B11100000
},
six_bmp[] = 
{
  B11100000,
  B10000000,
  B11100000,
  B10100000,
  B11100000
},
seven_bmp[] = 
{
  B11100000,
  B00100000,
  B00100000,
  B00100000,
  B00100000
},
eight_bmp[] = 
{
  B11100000,
  B10100000,
  B11100000,
  B10100000,
  B11100000
},
nine_bmp[] = 
{
  B11100000,
  B10100000,
  B11100000,
  B00100000,
  B00100000
},
colon_bmp[] =
{
  B00000000,
  B10000000,
  B00000000,
  B10000000,
  B00000000
};

static const  uint8_t* clock_number_bmp[] = 
{
  zero_bmp,
  one_bmp,
  two_bmp,
  three_bmp,
  four_bmp,
  five_bmp,
  six_bmp,
  seven_bmp,
  eight_bmp,
  nine_bmp,
  colon_bmp
};

