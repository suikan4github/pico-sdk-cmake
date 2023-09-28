#include "getduration.h"
#include "initgpio.h"
#include "pico/stdlib.h"

int main() {
  init_gpio();
  const unsigned int duration = get_duration();

  while (true) {
    gpio_put(LED_PIN, 1);
    sleep_ms(duration);
    gpio_put(LED_PIN, 0);
    sleep_ms(duration);
  }
}