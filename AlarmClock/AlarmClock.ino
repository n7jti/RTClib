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

#include <SPI.h>
#include <SD.h>
#include <adafruit_feather.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <TimeAlarms.h>
#include <TimeLib.h>
#include <Adafruit_VS1053.h>

#include "LEDMatrix.h"
#include "UdpNtpClient.h"
#include "DS3231.h"
#include "TimeScroller.h"
#include "config.h"


#define SWITCH1_PIN PB5
#define SWITCH2_PIN PA13
#define SWITCH3_PIN PA4

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)
#define VS1053_CS       PC7     // VS1053 chip select pin (output)
#define VS1053_DCS      PB4     // VS1053 Data/command select pin (output)
#define CARDCS          PC5     // Card chip select pin
#define VS1053_DREQ     PA15    // VS1053 Data request, ideally an Interrupt pin

LEDMatrix matrix = LEDMatrix();
RTC_DS3231 RTC;
TimeScroller timeScroller(matrix, RTC);
Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

bool connectAP(void)
{
  // Attempt to connect to an AP
  Serial.print("Please wait while connecting to: '" WIFI_SSID "' ... ");
  
  if ( Feather.connect(WIFI_SSID, WIFI_PASS) )
  {
    Serial.println("Connected!");
  }
  else
  {
    Serial.printf("Failed! %s (%d)", Feather.errstr(), Feather.errnum());
    Serial.println();
  }
  Serial.println();

  return Feather.connected();
}

void SetRTCFromNTP(RTC_DS3231 rtc)
{
  AdafruitUDP ntpUDP;
  UdpNtpClient timeClient(ntpUDP, "time.nist.gov", -25200);
  timeClient.update();
  time_t time = timeClient.getEpochTime();
  rtc.adjust(time);
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void setup() {
  Serial.begin(115200);
  delay(100);


  // Print all software versions
  Feather.printVersions();

  pinMode(SWITCH1_PIN, INPUT_PULLUP);
  pinMode(SWITCH2_PIN, INPUT_PULLUP);
  pinMode(SWITCH3_PIN, INPUT_PULLUP);
  
  if (! matrix.begin()) {
    Serial.println("IS31 not found");
    while (1);
  }

  matrix.setFrame(0);
  matrix.displayFrame(0);
  matrix.setTextSize(.5);
  matrix.setTextWrap(false);  
  matrix.setTextColor(16);
  
  Serial.println("IS31 Found!");
  
  while ( !connectAP() ) {
    delay ( 500 );
  }

  Feather.printNetwork();

  Serial.println("RTC.begin()");
  RTC.begin();
  if (RTC.lostPower()) {
    time_t alarmTime;
    alarmTime = 7 * SECS_PER_HOUR;
    RTC.putAlarm(alarmTime);
  }

  

  // Set the time in the RTC
  Serial.println("SetRTCFromNTP()");
  SetRTCFromNTP(RTC);
  
  // Hook up timelib to the RTC
  setSyncProvider(RTC_DS3231::now);

  if (!musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(10,10);
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);
  
  Serial.println("Setup Complete!");
}

time_t normalizeTime(const time_t &tm)
{
  time_t out;
  out = hour(tm) * SECS_PER_HOUR + minute(tm) * SECS_PER_MIN;
  return out;
}


void dumpTime(const time_t &tm)
{
  String str;
  str = "Time: ";
  str += hour(tm);
  str += ":";
  str += minute(tm);
  str += ":";
  str += second(tm);
  str += " ";
  str += month(tm);
  str += "/";
  str += day(tm);
  str += "/";
  str += year(tm);

  Serial.println(str);
}

void loop() {
  static int counter = 0;
 
  // Read Controls
  bool alarmOn     = (digitalRead(SWITCH1_PIN) == HIGH);
  bool alarmShow   = (digitalRead(SWITCH2_PIN) == HIGH);
  bool alarmChange = (digitalRead(SWITCH3_PIN) == HIGH);

  // Alarm Sounding Logic
  bool soundAlarm = RTC.isAlarm();
 
  if(true == soundAlarm)
  {
     Serial.println("Sound Alarm!!");
    if (true == alarmOn)
    {
      if (musicPlayer.stopped())
      {
        // Play a file in the background, REQUIRES interrupts!
        Serial.println(F("Playing full track 001"));
        musicPlayer.startPlayingFile("track001.mp3");
      }
    }
    else // (false == alarmOn)
    {
      Serial.println("Resetting Alarm Flag.");
      RTC.resetAlarm();
      if (!musicPlayer.stopped())
      {
        Serial.println("Stop Playing");
        musicPlayer.stopPlaying();
      }
    }
  }

  time_t alarmTime = RTC.getAlarm();
  alarmTime = normalizeTime(alarmTime);
  
  // Update the Alarm
  if (true == alarmShow){
    if (true == alarmChange){
      // find the unit of alarm change
      time_t unit = 0;
      if (counter < 4){
        // increment by 1 minute
        unit = SECS_PER_MIN;
      }
      else if (minute(alarmTime) == 0){
        // increment hours
        unit = SECS_PER_HOUR;
      }
      else if(minute(alarmTime) % 15 == 0){
        // increment by 15 minutes
        unit = 15 * SECS_PER_MIN;
      }
      else if (minute(alarmTime) % 5 == 0) {
        // increment by 5 minutes
        unit = 5 * SECS_PER_MIN;
      }
      else{
        unit = SECS_PER_MIN;
      }
      


      // move the number of units forward or backwards. 
      if (true ==alarmOn){
        // move forwards
        alarmTime += unit;
      }
      else{
        // move backwards
        alarmTime -= unit;
      }

      counter += 1;
      alarmTime = normalizeTime(alarmTime);
      RTC.putAlarm(alarmTime);
    }
    else{ // alarmChange == false
      counter = 0;
    }
  }

  

  // Update Display
  if (alarmShow){
    timeScroller.displayTime(alarmTime);
    delay(500);
  }
  else {
    timeScroller.scrollTime();
  }

  
}



