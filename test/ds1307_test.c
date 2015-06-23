#include <avrm/i2c.h>
#include <avrm/uart.h>
#include <ds1307.h>
#include <string.h>

int main(void)
{
  struct tm current_time;

  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Initialize the I2C bus.
  i2c_init(I2C_SCL_400KHZ);

  // Toggle the output.
  ds1307_set_output(TRUE);
  printf("OUT: 1\n");
  delay_ms(1000);
  ds1307_set_output(FALSE);
  printf("OUT: 0\n");

  // Turn on and set the square wave generation to 4.096kHz.
  ds1307_set_square_wave_freq(DS1307_4KHZ);
  ds1307_set_square_wave(TRUE);
  printf("SQR: 4.096kHz\n");

  // Set the RAM.
  char *set_data = "This is some data.";
  ds1307_set_ram(0x00, (byte *) set_data, 19);
  printf("RAM: 0x00 -> %s\n", set_data);

  // Get 0x00 in RAM.
  char get_data[19];
  ds1307_get_ram(0x00, (byte *) get_data, 19);
  printf("RAM: %s\n", get_data);

  // Set and get too much data.
  size_t count;
  memset(set_data, 0xFF, 19);
  set_data[18] = 0;
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  count = ds1307_set_ram(54, (byte *) set_data, 3);
  printf("RAM:%d: %s\n", count, set_data);
  count = ds1307_get_ram(54, (byte *) get_data, 3);
  get_data[18] = '\0';
  printf("RAM:%d: %s\n", count, get_data);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));

  // Turn off the device.
  ds1307_stop();
  printf("STOP: %u\n", ds1307_is_stopped());
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));

  // Turn on the device.
  ds1307_start();
  printf("START: %u\n", !ds1307_is_stopped());
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));

  // Clear the device.
  printf("CLEAR: %u\n", !ds1307_clear());
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  delay_ms(1000);
  current_time = ds1307_get_time();
  printf("GET: %s\n", asctime(&current_time));
  printf("-----\n");

  // Create the time structure.
  struct tm set_time =
  {
    .tm_hour = 23,
    .tm_min = 59,
    .tm_sec = 57,
    .tm_mday = 16,
    .tm_mon = 5,
    .tm_year = 115,
    .tm_isdst = -1,
  };
  mktime(&set_time);

  // Set the time.
  if (ds1307_set_time(set_time) == 0)
    printf("SET: %s\n", asctime(&set_time));
  else
    printf("ERR: Could not set RTC.\n");

  for (;;)
  {
    // Get the time.
    current_time = ds1307_get_time();
    // Print the time.
    delay_ms(1000);
    printf("GET: %s\n", asctime(&current_time));
  }

  return 0;
}
