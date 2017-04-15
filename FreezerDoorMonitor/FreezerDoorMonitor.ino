#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#include "door.h"
 
#define DOOR_PIN 14
#define RED_PIN 12
#define GREEN_PIN 13

AdafruitIO_Feed *doorFeed = io.feed("Freezer Door");
Adafruit_SSD1306 display = Adafruit_SSD1306();
Door door = Door();

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  door.begin(DOOR_PIN);
  display.display();
  delay(1000);  

  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Connecting to AdafruitIo.");
  display.display();
  io.connect();

  while(io.status() < AIO_CONNECTED) {
    display.print(".");
    display.display();
    delay(500);
  }

  // we are connected
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(io.statusText());
  display.display();
  delay(500);

}

void loop() {
  // door state.  true indicates door is open.  We'll fixup the logic from the sensor to give this 
  // logical meaning 
  static DOOR_STATE lastDoorState = DOOR_GREEN;

  io.run();
  DOOR_STATE doorState = door.run();

  if (doorState == DOOR_GREEN)
  {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  }
  else
  {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
  }
 
  // return if the value hasn't changed
  if(doorState == lastDoorState)
    return;

  // save the current state to the 'Freezer Door' feed on adafruit io
  display.clearDisplay();
  display.setCursor(0,0);
  display.println(io.statusText());
  display.print("sending door -> ");
  display.println(doorState);
  display.display();

  lastDoorState = doorState;
  doorFeed->save(doorState);
}
