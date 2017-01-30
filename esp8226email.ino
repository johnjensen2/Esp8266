/*
  
   Based on the work of SurferTim and modified to use the current 29 Jan 2017 gmail standards

   
   Email client sketch for IDE v1.0.1 and w5100/w5200
   Posted December 2012 by SurferTim
*/




#include <ESP8266WiFi.h>
 
const char* ssid = "ssid";
const char* password = "passord";
const char* server = "smtp.gmail.com";
 
WiFiClientSecure client;
 
void setup()
{
  Serial.begin(9600);
WiFi.begin(ssid, password);
 
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
 

  Serial.println(F("Ready. Press 'e' to send."));
}
 
void loop()
{
  byte inChar;
 
  inChar = Serial.read();
 
  if(inChar == 'e')
  {
      if(sendEmail()) Serial.println(F("Email sent"));
      else Serial.println(F("Email failed"));
  }
}
 
byte sendEmail()
{
  byte thisByte = 0;
  byte respCode;
 
  if(client.connect(server,465) == 1) {
    Serial.println(F("connected"));

  } else {
    Serial.println(F("connection failed"));
    return 0;
  }
 
  if(!eRcv()) return 0;
  Serial.println(F("Sending helo"));
  
 
// change to your public ip
  client.println(F("helo 1.2.3.4"));

      client.println(F("AUTH LOGIN"));
       if(!eRcv()) return 0;
       // This is your id in base64 not the @gmail.com part.
       // I used the wedbsite https://www.base64encode.org/
       // take the trailing = off the username and password
   client.println(F("dXNlcm5hbWU"));;
    if(!eRcv()) return 0;
   client.println(F("cGFzc3dvcmQ"));
    if(!eRcv()) return 0;

  Serial.println(F("Sending From"));
 
// change to your email address (sender)
  client.println(F("MAIL From: <name@gmail.com>"));

  if(!eRcv()) return 0;
 
// change to recipient address
  Serial.println(F("Sending To"));
  client.println(F("RCPT To: <name@company.com>"));
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending DATA"));
  client.println(F("DATA"));
 
  if(!eRcv()) return 0;
 
  Serial.println(F("Sending email"));
 
// change to recipient address
  client.println(F("To: You <name@company.com>"));
 
// change to your address
  client.println(F("From: Me <name@company.com>"));
 
  client.println(F("Subject: Arduino email Alert test\r\n"));
 
  client.println(F("This is from my Arduino! It was real the alert would be here"));
 
  client.println(F("."));


 
  Serial.println(F("Sending QUIT"));
  client.println(F("QUIT"));
 
  if(!eRcv()) return 0;
 
  client.stop();
 
  Serial.println(F("disconnected"));
 
  return 1;
}
 
byte eRcv()
{
  byte respCode;
  byte thisByte;
  int loopCount = 0;
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return 0;
    }
  }
 
  respCode = client.peek();
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  if(respCode >= '4')
  {
    efail();
    return 0;  
  }
 
  return 1;
}
 
 
void efail()
{
  byte thisByte = 0;
  int loopCount = 0;
 
  client.println(F("QUIT"));
 
  while(!client.available()) {
    delay(1);
    loopCount++;
 
    // if nothing received for 10 seconds, timeout
    if(loopCount > 10000) {
      client.stop();
      Serial.println(F("\r\nTimeout"));
      return;
    }
  }
 
  while(client.available())
  {  
    thisByte = client.read();    
    Serial.write(thisByte);
  }
 
  client.stop();
 
  Serial.println(F("disconnected"));
}
