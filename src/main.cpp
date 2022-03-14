#include <Arduino.h>
#include <RTClib.h>
#include <SPI.h>

void onAlarm();

RTC_DS3231 rtc;

// the pin that is connected to SQW
#define CLOCK_INTERRUPT_PIN 2

#define LED 13

void setup() {
  // pinMode(LED, OUTPUT);

  Serial.begin(9600);

  Serial.println("This is the beginning");

  //we don't need the 32K Pin, so disable it
  // rtc.disable32K();

  // initializing the rtc
  if(!rtc.begin()) {
      Serial.println("Couldn't find RTC!");
      Serial.flush();
      while (1) delay(10);
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Making it so, that the alarm will trigger an interrupt
  pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);

  // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
  // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  
  Serial.println("Alarms are cleared");
  // stop oscillating signals at SQW Pin
  // otherwise setAlarm1 will fail
  rtc.writeSqwPinMode(DS3231_OFF);

  // turn off alarm 2 (in case it isn't off already)
  // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
  rtc.disableAlarm(2);

  // schedule an alarm 10 seconds in the future
  if(!rtc.setAlarm1(
          rtc.now() + TimeSpan(10),
          DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
  )) {
      Serial.println("Error, alarm wasn't set!");
  }else {
      Serial.println("Alarm will happen in 10 seconds!");
  }

  Serial.println("Code setup is over");
}

void loop() {
  // print current time
  char date[10] = "hh:mm:ss";
  rtc.now().toString(date);
  Serial.println(date); 
  

  // using setAlarm1, the next alarm could now be configurated
  if(rtc.alarmFired(1)) {
    rtc.clearAlarm(1);
    Serial.println("Alarm cleared");
  }
  delay(1000);
}

void onAlarm() {
  Serial.println("Alarm occured!");
}