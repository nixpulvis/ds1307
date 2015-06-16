#include <avr.h>
#include <ds1307.h>

int main(void)
{
  // Setup the UART, necessary for stdio actions.
  uart_init();

  // Initialize the I2C bus.
  i2c_init();

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
  if (ds1307_set_tm(set_time) == 0)
    printf("SET: %s\n", asctime(&set_time));
  else
    printf("ERR: Could not set RTC.\n");

  for (;;)
  {
    // Get the time.
    struct tm current_time = ds1307_get_tm();
    // Print the time.
    printf("GET: %s\n", asctime(&current_time));
    delay_ms(1000);
  }

  return 0;
}
