#include "ds1307.h"

//
// ds1307_set_time implementation.
//
int ds1307_set_time(struct tm time)
{
  // TODO: Check bounds of arguments.

  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(dec_to_bcd(time.tm_sec));
  i2c_write(dec_to_bcd(time.tm_min));
  i2c_write(dec_to_bcd(time.tm_hour));
  i2c_write(dec_to_bcd(time.tm_wday));
  i2c_write(dec_to_bcd(time.tm_mday));
  i2c_write(dec_to_bcd(time.tm_mon));
  i2c_write(dec_to_bcd(time.tm_yday));
  i2c_stop();

  return 0;
}

//
// ds1307_get_time implementation.
//
struct tm ds1307_get_time(void)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_start(DS1307_ID, I2C_READ);
  byte second = i2c_read_ack();
  byte minute = i2c_read_ack();
  byte hour   = i2c_read_ack();
  byte day    = i2c_read_ack();
  byte date   = i2c_read_ack();
  byte month  = i2c_read_ack();
  byte year   = i2c_read_ack();
  struct tm ret =
  {
    bcd_to_dec(second),
    bcd_to_dec(minute),
    bcd_to_dec(hour),
    bcd_to_dec(date),
    bcd_to_dec(day),
    bcd_to_dec(month),
    bcd_to_dec(year),
  };
  i2c_stop();

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

// TODO: Move BCD functions in avr library.

byte bcd_to_dec(byte data)
{
  return (((data & 0xF0) >> 4) * 10) + (data & 0x0F);
}

byte dec_to_bcd(byte data)
{
  return (((data / 10) << 4) & 0xF0) | (data % 10);
}
