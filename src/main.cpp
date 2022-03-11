#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>

#define LED 13
RTC_DS3231 rtc;


void setup() {
  // pinMode(LED, OUTPUT);

  Serial.begin(9600);

  // initializing the rtc
  if(!rtc.begin()) {
      Serial.println("Couldn't find RTC!");
      Serial.flush();
      while (1) delay(10);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("This is the beginning");
}

void loop() {
  // print current time
  char date[10] = "hh:mm:ss";
  rtc.now().toString(date);
  Serial.println(date); 
  //Serial.println("Test");
  delay(2000);
}