#ifndef __DS1307_H__
#define __DS1307_H__

#include <time.h>
#include <avr.h>

#define DS1307_ID 0x68
#define DS1307_REGISTER_SECONDS 0x00
#define DS1307_REGISTER_MINUTES 0x01
#define DS1307_REGISTER_HOURS   0x02
#define DS1307_REGISTER_DAY     0x03
#define DS1307_REGISTER_DATE    0x04
#define DS1307_REGISTER_MONTH   0x05
#define DS1307_REGISTER_YEAR    0x06
#define DS1307_REGISTER_CONTROL 0x07

typedef struct
{
  byte second;
  byte minute;
  byte hour;
  byte day;
  byte date;
  byte month;
  byte year;
} ds1307_time_t;

// ds1307_set_time
int ds1307_set_time(ds1307_time_t time);

// ds1307_set_time
int ds1307_set_tm(struct tm time);

// ds1307_get_time
ds1307_time_t ds1307_get_time(void);

// ds1307_get_tm
struct tm ds1307_get_tm(void);

// TODO: Write BCD functions in avr library.

// TODO: Move this into avr library.
#include <util/twi.h>
#define I2C_READ  0x01
#define I2C_WRITE 0x00
void i2c_init(void);
byte i2c_start(byte address, byte config);
void i2c_write(byte data);
byte i2c_read_ack(void);
byte i2c_read_nack(void);
void i2c_stop(void);

#endif