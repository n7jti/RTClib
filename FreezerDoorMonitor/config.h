#pragma once

#define WIFI_SSID           "<YOUR SSID>"
#define WIFI_PASS           "<YOUR WIFI PASSWORD>"

#define ALARM_SUBJECT       "FREEZER_OPEN"
#define ALARM_MSG           "The freezer is open."
#define ALARM_CLEAR_SUBJECT "FREEZER CLOSED"
#define ALARM_CLEAR_MSG     "The freezer is closed again."
#define ALARM_TIMEOUT_MS (60000)

// The hosname of the SMTP server
#define MAIL_HOST           "<YOUR MAILSERVER>"
#define MAIL_PORT           <YOUR SMTP PORT -- NO SSL/TLS>

// Who the mail is from
#define MAIL_FROM           "youraddress@host.com"
#define MAIL_CLIENT_HOST    "<EVERYTHING AFTER @ IN THE MAIL_FROM>"

// Who the mail is to 
#define MAIL_TO             "youraddress@host.c om"

// username and password must be base645 encoded. 
// TODO: implement base64 encoding and have this parameter in the clear
// In the meantime -- https://www.base64decode.org/ 
#define MAIL_USER_BASE64    "<BASE64 ENCODED USERNAME FOR YOUR SMTP LOGIN>"
#define MAIL_PASS_BASE64    "<BASE64 ENCODED PASSWORD FOR YOUR SMTP LOGIN>"


