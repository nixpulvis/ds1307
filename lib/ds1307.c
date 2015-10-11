#include "ds1307.h"

//
// ds1307_init implementation.
//
int ds1307_init(long long i2c_frequency)
{
  // Initialize the I2C bus.
  return i2c_init(I2C_SCL_100KHZ);
}

//
// ds1307_set_square_wave implementation.
//
int ds1307_set_square_wave(bool state)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_CONTROL);
  i2c_start(DS1307_ID, I2C_READ);
  byte control = i2c_read_ack();
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(control | state << 4);
  i2c_stop();
  return 0;
}

// ds1307_set_square_wave_freq
int ds1307_set_square_wave_freq(byte value)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_CONTROL);
  i2c_start(DS1307_ID, I2C_READ);
  byte control = i2c_read_ack();
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(control | (value & 0x03));
  i2c_stop();
  return 0;
}

//
// ds1307_set_output implementation.
//
int ds1307_set_output(bool state)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_CONTROL);
  i2c_start(DS1307_ID, I2C_READ);
  byte control = i2c_read_ack();
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(control | ((state << 7) & 0x80));
  i2c_stop();
  return 0;
}

//
// ds1307_clear implementation.
//
int ds1307_clear(void)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(0x80);
  for (int i = 0; i < 6; i++)
    i2c_write(0x00);
  i2c_stop();
  return 0;
}

//
// ds1307_start implementation.
//
int ds1307_start(void)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_start(DS1307_ID, I2C_READ);
  byte second = i2c_read_ack();
  i2c_stop();
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(second & (0x7F));
  i2c_stop();
  return 0;
}

//
// ds1307_stop implementation.
//
int ds1307_stop(void)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_start(DS1307_ID, I2C_READ);
  byte second = i2c_read_ack();
  i2c_stop();
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(second | 0x80);
  i2c_stop();
  return 0;
}

//
// ds1307_stopped implementation.
//
bool ds1307_is_stopped(void)
{
  return i2c_read_register_byte(DS1307_ID, DS1307_REGISTER_SECONDS) >> 0x07;
}

//
// ds1307_set_time implementation.
//
int ds1307_set_time(struct tm time)
{
  i2c_start(DS1307_ID, I2C_WRITE);
  i2c_write(DS1307_REGISTER_SECONDS);
  i2c_write(dec_to_bcd(time.tm_sec  & 0x7F));
  i2c_write(dec_to_bcd(time.tm_min  & 0x7F));
  i2c_write(dec_to_bcd(time.tm_hour & 0x7F));
  i2c_write(dec_to_bcd(time.tm_wday & 0x07));
  i2c_write(dec_to_bcd(time.tm_mday & 0x3F));
  i2c_write(dec_to_bcd(time.tm_mon  & 0x1F));
  i2c_write(dec_to_bcd(time.tm_year));
  i2c_stop();
  return 0;
}

//
// ds1307_get_time implementation.
//
struct tm ds1307_get_time(void)
{
  byte buffer[7];
  i2c_read_register_bytes(DS1307_ID, DS1307_REGISTER_SECONDS, buffer, 7);
  struct tm ret =
  {
    bcd_to_dec(buffer[0] & 0x7F),
    bcd_to_dec(buffer[1] & 0x7F),
    bcd_to_dec(buffer[2] & 0x7F),
    bcd_to_dec(buffer[4] & 0x3F),
    bcd_to_dec(buffer[3] & 0x07),
    bcd_to_dec(buffer[5] & 0x1F),
    bcd_to_dec(buffer[6]),
  };
  return ret;
}

// ds1307_set_ram
size_t ds1307_set_ram(byte address, byte *buffer, size_t length)
{
  if (address + length >= DS1307_RAM_SIZE)
    return -1;
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
  if (address + length >= DS1307_RAM_SIZE)
    return -1;
  i2c_read_register_bytes(DS1307_ID, address + 0x08, buffer, length);
  return length;
}
