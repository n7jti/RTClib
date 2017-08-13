#pragma once
#include <ESP8266WiFi.h>

// smtp.h
void sendSMTP(char *subject, char *msg);
void waitTillLineClear(WiFiClient &client);

// Door states
enum SMTP_STATE {
  SMTP_BEGIN,
  SMTP_CONNECT,
  SMTP_CONNECT_REPLY,
  SMTP_EHLO,
  SMTP_EHLO_REPLY,
  SMTP_LOGIN,
  SMTP_LOGIN_REPLY,
  SMTP_USER,
  SMTP_USER_REPLY,
  SMTP_PASS,
  SMTP_PASS_REPLY,
  SMTP_FROM,
  SMTP_FROM_REPLY,
  SMTP_TO,
  SMTP_TO_REPLY,
  SMTP_DATA,
  SMTP_DATA_REPLY,
  SMTP_TEXT,
  SMTP_TEXT_REPLY,
  SMTP_DISCONNECT,
  SMTP_DONE,
};

class Smtp{
public:
  Smtp();
  void begin();
  SMTP_STATE run();
  SMTP_STATE getState();
  SMTP_STATE send(char *subject, char *msg);
  
private:
  void lineClear(SMTP_STATE newState);
  char *_subject;
  char *_msg;
  SMTP_STATE _state;
  WiFiClient _client;
  
};

