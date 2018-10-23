#include <SPI.h>
#include <SD.h>

#define SDPIN 10

#include "schedule.hpp"

/**
     Read a line from the schedule file.  Return the line as a String object.
*/
static String read_line(File f) {
  String buf = "";
  char ch;
  while ((ch = f.read()) != '\n') {
    buf += ch;
  }
  return buf;
}

static bool read_csv(String *retArr, File schedule_file) {
  if (schedule_file.available()) {
    String str1 = read_line(schedule_file);
    Serial.println("read_csv: str1 = " + str1);
    retArr[0] = String(strtok(str1.c_str(), ":"));
    retArr[1] = String(strtok(NULL, ":"));
    retArr[2] = String(strtok(NULL, ":"));
    retArr[3] = String(strtok(NULL, ":"));
    return true;
  }
  else {
    return false;
  }
}


//// Schedule class

Schedule::Schedule(String file) {
  schedule_file_name = file;
}

void Schedule::open_file() {
  
}

/*
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
*/

// Load schedule from a file.
void Schedule::load() {
  const uint8_t STEP = 7;
  int sz = STEP;
  String record[4];
  if (! mt) {
    Serial.println("Schedule::load: Could not allocate memory");
  }

  File schedule_file;
  while ((schedule_file = SD.open(schedule_file_name)) == 0) {
    Serial.println("ERROR: Could not open a file.");
    break;
  }

  Serial.println("File opened.");

  while (read_csv(record, schedule_file) && (mt_size < MT_MAX_SIZE)) {
    mt[mt_size].hour     = record[0].toInt();
    mt[mt_size].min      = record[1].toInt();
    mt[mt_size].sec      = record[2].toInt();
    mt[mt_size].ringtone = record[3].toInt();
    mt_size++;
  }

  Serial.println("closing file...");
  schedule_file.close();
  delay(500);
}
