#include "smtp.h"
#include "config.h"

static const char         host[]     = MAIL_HOST;
static const unsigned int smtpPort   = MAIL_PORT;

Smtp::Smtp()
  : _subject(NULL)
  , _msg(NULL)
  , _state(SMTP_DONE)
{
}

void Smtp::begin()
{
  return;
}

void Smtp::lineClear(SMTP_STATE newState)
{
  if(_client.available()){
    String line = _client.readStringUntil('\r');
    Serial.print(line);
  }
  else
  {
    _state = newState;
  }
}

SMTP_STATE Smtp::run()
{
  switch(_state)
  {
  case SMTP_CONNECT:
    if(!_client.connect(host, smtpPort)) {
      Serial.println("connection failed");
      _state = SMTP_DONE;
    }
    else
    {
      Serial.println("connecting...");
    }
    _state = SMTP_CONNECT_REPLY;
  case SMTP_CONNECT_REPLY:
    lineClear(SMTP_EHLO);
    break;
  case SMTP_EHLO:
    _client.print("EHLO " MAIL_CLIENT_HOST "\r\n");
    Serial.print("EHLO " MAIL_CLIENT_HOST "\r\n");
    _state = SMTP_EHLO_REPLY;
  case SMTP_EHLO_REPLY:
    lineClear(SMTP_LOGIN);
    break;
  case SMTP_LOGIN:
    _client.print("AUTH LOGIN\r\n");
    Serial.print("AUTH LOGIN\r\n");
    _state=SMTP_LOGIN_REPLY;
  case SMTP_LOGIN_REPLY:
    lineClear(SMTP_USER);
    break;
  case SMTP_USER:
    _client.print(MAIL_USER_BASE64 "\r\n");  //SECRET logn Base64 Encoded
    Serial.print(MAIL_USER_BASE64 "\r\n");
    _state = SMTP_USER_REPLY;
  case SMTP_USER_REPLY:
    lineClear(SMTP_PASS);
    break;
  case SMTP_PASS:
    _client.print(MAIL_PASS_BASE64 "\r\n");  //SECRET password Base64 Encoded
    Serial.print(MAIL_PASS_BASE64 "\r\n");  //SECRET password Base64 Encoded
    _state = SMTP_PASS_REPLY;
  case SMTP_PASS_REPLY:
    lineClear(SMTP_FROM);
    break;
  case SMTP_FROM:
    _client.print("MAIL FROM: " MAIL_FROM "\r\n");
    Serial.print("MAIL FROM: " MAIL_FROM "\r\n");
    _state = SMTP_FROM_REPLY;
  case SMTP_FROM_REPLY:
    lineClear(SMTP_TO);
    break;
  case SMTP_TO:
    _client.print("RCPT TO: " MAIL_TO "\r\n");
    Serial.print("RCPT TO: " MAIL_TO "\r\n");
    _state = SMTP_TO_REPLY;
  case SMTP_TO_REPLY:
    lineClear(SMTP_DATA);
    break;
  case SMTP_DATA:
    _client.print("DATA\r\n");
    Serial.print("DATA\r\n");
    _state = SMTP_DATA_REPLY;
  case SMTP_DATA_REPLY:
    lineClear(SMTP_TEXT);
    break;
  case SMTP_TEXT:
    //_client.printf("Subject: %s\r\n", _subject);
    _client.printf("%s\r\n", _msg);
    _client.print(".\r\n");
    _client.print("quit\r\n");

    //Serial.printf("Subject: %s\r\n", _subject);
    Serial.printf("%s\r\n", _msg);
    Serial.print(".\r\n");
    Serial.print("quit\r\n");
    
    _state = SMTP_TEXT_REPLY;
  case SMTP_TEXT_REPLY:
    lineClear(SMTP_DISCONNECT);
    break;
  case SMTP_DISCONNECT:
    _client.stop();
    Serial.println("client.Stop()");
    _state=SMTP_DONE;
  case SMTP_DONE:
    // we just sit here!!
    break;
  }
  return _state;
}

SMTP_STATE Smtp::getState()
{
  return _state;
}


SMTP_STATE Smtp::send(char *subject, char *msg)
{
  if(_state != SMTP_DONE)
  {
    // If we are in the middle of sending, abandon the connection and start again. 
    _client.stop();
    Serial.println("--client.stop()--");
  }
  
  _subject = subject;
  _msg = msg;
  _state = SMTP_CONNECT;
  return _state;
}

