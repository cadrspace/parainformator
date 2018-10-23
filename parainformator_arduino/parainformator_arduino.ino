//#define debug 1
#define SDPIN 10
#include <SPI.h>
#include <DS1302.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySerial(6, 7); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

#include "schedule.hpp"

const char FILE_NAME[] = "s1.txt";
const char FILE_NAME_H[] = "s1.txt";

Schedule schedule(FILE_NAME);
Schedule schedule_h(FILE_NAME_H);

DS1302 rtc(2, 3, 4); //создание объекта часы

void setup() {
  rtc.halt(false);
  Serial.begin(9600);
  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  myDFPlayer.volume(25);

  rtc.setDOW(FRIDAY);        // Set Day-of-Week to FRIDAY
  rtc.setTime(16, 44, 00);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(11, 05, 2018);   // Set the date to August 6th, 2010
  //*/


  delay(1000);
  // Инициализация SD карты памяти
  if (!SD.begin(SDPIN)) {
    return;
  }
  //pinMode(9, OUTPUT);
  delay(1000);
  schedule.load();
  delay(500);
  schedule_h.load();

  uint8_t sz = schedule.size();
  for (int idx = 0; idx < sz; ++idx) {
    Serial.println("--- idx = " + String(idx));
    Serial.println(schedule[idx].hour);
    Serial.println(schedule[idx].min);
    Serial.println(schedule[idx].sec);
    Serial.println("---");
  }
  Serial.println("loaded");
}

inline bool isHoliday(Time t) {
  return (t.dow == 6);
}

inline bool isTimeEq(struct marked_time* mt, Time* ct)
{
  Serial.println("H (ct = mt): " + String(ct->hour) + "=" + String(mt->hour)
                 + "; M: " + String(ct->min) + "=" + String(mt->min)
                 + "; S: " + String(ct->sec) + "=" + String(mt->sec));
  return ((ct->hour == mt->hour ) &&
          (ct->min == mt->min) &&
          (ct->sec == mt->sec));
}

void loop() {
  Time t = rtc.getTime();
  if (isHoliday(t)) {
    handleDays(t, schedule_h);
  } else {
    handleDays(t, schedule);
  }
  delay(200);
}

void handleDays(Time t, Schedule sched)
{
  uint8_t sz = sched.size();
  Serial.println("qwe");
  for (int idx = 0; idx < sz; ++idx)
  {

    Serial.print(sched[idx].hour);
    Serial.print(" ");
    Serial.print(sched[idx].min);
    Serial.print(" ");
    Serial.print(sched[idx].sec);
    Serial.print(" ");
    Serial.println(sched[idx].ringtone);

    if (isTimeEq(&sched[idx], &t)) {
      //digitalWrite(LED_PIN, HIGH);
      Serial.println("**** DING! ****");
      ringtone(sched[idx].ringtone);
      //digitalWrite(LED_PIN, LOW);
    }
  }
  //Serial.println("asd");
}


void ringtone(int num)
{
  String fname = String(num);
  Serial.println("ringtone: " + fname);
  myDFPlayer.play(num);

  delay(6000);
}
