#ifndef __SCHEDULE_HPP__
#define __SCHEDULE_HPP__
#include <SPI.h>
#include <SD.h>

using namespace std;

const int MT_MAX_SIZE = 12;

struct marked_time {
  byte hour : 6;
  byte min  : 6;
  byte sec  : 6;
  byte ringtone : 6;
};

class Schedule {
  private:
    /**
       Schedule file format:
         hour:min:sec:ringtone
    */
    String schedule_file_name;

    struct marked_time mt[MT_MAX_SIZE];
    uint8_t mt_size = 0;

    void open_file();

  public:
    Schedule(String file);
    void load();
    uint8_t size() {
      return mt_size;
    }

    struct marked_time operator[](size_t idx) {
      return mt[idx];
    }
};

#endif /* __SCHEDULE_HPP__ */
