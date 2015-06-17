#include "ds1307.h"

//
// ds1307_set_time implementation.
//
int ds1307_set_time(ds1307_time_t time)
{
  // TODO: Check bounds of arguments.

  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(time.second);
  i2c_write(time.minute);
  i2c_write(time.hour);
  i2c_write(time.day);
  i2c_write(time.date);
  i2c_write(time.month);
  i2c_write(time.year);
  i2c_stop();

  return 0;
}

//
// ds1307_set_tm implementation.
//
int ds1307_set_tm(struct tm time)
{
  ds1307_time_t raw =
  {
    (((time.tm_sec / 10) << 4) & 0x70) | (time.tm_sec % 10),
    (((time.tm_min / 10) << 4) & 0x70) | (time.tm_min % 10),
    (((time.tm_hour / 10) << 4) & 0x30) | (time.tm_hour % 10),
    time.tm_wday % 10,
    (((time.tm_mday / 10) << 4) & 0x30) | (time.tm_mday % 10),
    (((time.tm_mon / 10) << 4) & 0x10) | (time.tm_mon % 10),
    (((time.tm_year / 10) << 4) & 0xF0) | (time.tm_year % 10),
  };
  return ds1307_set_time(raw);
}

//
// ds1307_get_time implementation.
//
ds1307_time_t ds1307_get_time(void)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_start(DS1307_ID, I2C_READ);
  ds1307_time_t ret =
  {
    i2c_read_ack(),
    i2c_read_ack(),
    i2c_read_ack(),
    i2c_read_ack(),
    i2c_read_ack(),
    i2c_read_ack(),
    i2c_read_ack(),
  };
  i2c_stop();

  return ret;
}

//
// ds1307_get_tm implementation.
//
struct tm ds1307_get_tm(void)
{
  ds1307_time_t raw = ds1307_get_time();
  struct tm ret =
  {
    (((raw.second & 0x70) >> 4) * 10) + (raw.second & 0x0F),
    (((raw.minute & 0x70) >> 4) * 10) + (raw.minute & 0x0F),
    (((raw.hour & 0x30) >> 4) * 10) + (raw.hour & 0x0F),
    (((raw.date & 0x30) >> 4) * 10) + (raw.date & 0x0F),
    raw.day & 0x07,
    (((raw.month & 0x10) >> 4) * 10) + (raw.month & 0x0F),
    (((raw.year & 0xF0) >> 4) * 10) + (raw.year & 0x0F),
  };
  return ret;
}

// ds1307_set_ram
size_t ds1307_set_ram(byte address, byte *buffer, size_t length)
{
  // TODO: Bound checking.

  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(address + 0x08);
  size_t i;
  for (i = 0; i < length; i++)
    i2c_write(buffer[i]);
  i2c_stop();

  return i;
}

// ds1307_get_ram
size_t ds1307_get_ram(byte address, byte *buffer, size_t length)
{
  // TODO: Bound checking.

  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(address + 0x08);
  i2c_start(DS1307_ID, I2C_READ);
  size_t i;
  for (i = 0; i < length; i++)
    buffer[i] = i2c_read_ack();
  i2c_stop();

  return i;
}
