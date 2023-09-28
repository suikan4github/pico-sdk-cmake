#include "initgpio.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void init_gpio() {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}