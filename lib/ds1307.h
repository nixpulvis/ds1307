#ifndef __DS1307_H__
#define __DS1307_H__

#include <avr.h>
#include <time.h>

#define DS1307_ID 0x68
#define DS1307_RAM_SIZE 56
#define DS1307_REGISTER_SECONDS 0x00
#define DS1307_REGISTER_MINUTES 0x01
#define DS1307_REGISTER_HOURS   0x02
#define DS1307_REGISTER_DAY     0x03
#define DS1307_REGISTER_DATE    0x04
#define DS1307_REGISTER_MONTH   0x05
#define DS1307_REGISTER_YEAR    0x06
#define DS1307_REGISTER_CONTROL 0x07

// ds1307_set_square_wave
int ds1307_set_square_wave(bool state);

// ds1307_set_square_wave_freq
int ds1307_set_square_wave_freq(byte value);

// ds1307_set_output
//
// The square wave generation for the device must be disabled.
// i.e. `ds1307_set_square_wave(FALSE);
//
int ds1307_set_output(bool state);

// ds1307_clear
int ds1307_clear(void);

// ds1307_start
int ds1307_start(void);

// ds1307_stop
int ds1307_stop(void);

// ds1307_is_stopped
bool ds1307_is_stopped(void);

// ds1307_set_time
int ds1307_set_time(struct tm time);

// ds1307_get_time
struct tm ds1307_get_time(void);

// ds1307_set_ram
size_t ds1307_set_ram(byte address, byte *buffer, size_t length);

// ds1307_get_ram
size_t ds1307_get_ram(byte address, byte *buffer, size_t length);

#endif
