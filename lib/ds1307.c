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



// I2C Implementation
// TODO: Move into avr.

#define F_SCL 100000UL // SCL frequency
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16 ) / 2)

void i2c_init(void){
  TWBR = TWBR_val;
}

byte i2c_start(byte address, byte config){
  // reset TWI control register
  TWCR = 0;
  // transmit START condition
  TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  // check if the start condition was successfully transmitted
  if((TWSR & 0xF8) != TW_START){ return 1; }

  // load slave address into data register
  TWDR = ((address << 1) & 0xFE) | (config & 0x01);
  // start transmission of address
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  // check if the device has acknowledged the READ / WRITE mode
  byte twst = TW_STATUS & 0xF8;
  if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

  return 0;
}

void i2c_write(byte data){
  // load data into data register
  TWDR = data;
  // start transmission of data
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );

  if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
    return;

  return;
}

byte i2c_read_ack(void){

  // start TWI module and acknowledge data after reception
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );
  // return received data from TWDR
  return TWDR;
}

byte i2c_read_nack(void){

  // start receiving without acknowledging reception
  TWCR = (1<<TWINT) | (1<<TWEN);
  // wait for end of transmission
  while( !(TWCR & (1<<TWINT)) );
  // return received data from TWDR
  return TWDR;
}

void i2c_stop(void){
  // transmit STOP condition
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}