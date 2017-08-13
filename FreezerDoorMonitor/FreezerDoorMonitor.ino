#include <ESP8266WiFi.h>
#include "config.h"
#include "door.h"
#include "smtp.h"

#define DOOR_PIN 14
#define RED_PIN 12
#define GREEN_PIN 13
#define BUZZER_PIN 15

Door door = Door();
Smtp smtp = Smtp();

char openSubject[] = ALARM_SUBJECT;
char openMsg[] = ALARM_MSG;
char closeSubject[] = ALARM_CLEAR_SUBJECT;
char closeMsg[] = ALARM_CLEAR_MSG;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  door.begin(DOOR_PIN);
  smtp.begin();

  digitalWrite(RED_PIN, LOW);    // Low pin turns lights on.
  digitalWrite(GREEN_PIN, LOW);  // Low pin turns ligts on
  
  // attempt to connect to Wifi network:
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // check WIFI connection
  if (WiFi.status() != WL_CONNECTED)
  {
    // attempt to connect to Wifi network:
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  static DOOR_STATE lastDoorState = DOOR_GREEN;
  
  DOOR_STATE doorState = door.run();
  SMTP_STATE smtpState = smtp.run();

  if (doorState == DOOR_GREEN)
  {
    noTone(BUZZER_PIN);
    digitalWrite(RED_PIN, HIGH);  // OFF
    digitalWrite(GREEN_PIN, LOW); // ON
  }
  else if (doorState == DOOR_YELLOW) // red LED
  {
    digitalWrite(RED_PIN, LOW);    // ON
    digitalWrite(GREEN_PIN, HIGH); // OFF
  }
  else // DOOR_RED -- Flashing red LED
  {
    unsigned int ms = millis();
    if ((ms % 1000) < 500)
    {
      digitalWrite(RED_PIN, LOW); // ON
    }
    else
    {
      digitalWrite(RED_PIN, HIGH); // OFF
    }
    digitalWrite(GREEN_PIN, HIGH); // OFF
  }
 
  // return if the value hasn't changed
  if(doorState == lastDoorState)
    return;

  // state changed!
  if (DOOR_RED == doorState)
  { 
    // Alarm!
    tone(BUZZER_PIN, 440); 
    smtp.send(openSubject, openMsg);
  }
  else if (DOOR_GREEN == doorState && DOOR_RED == lastDoorState)
  {
    // Alarm Cleared
    smtp.send(closeSubject, closeMsg);
  }
  
  lastDoorState = doorState;
}
