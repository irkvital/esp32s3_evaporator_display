#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "esp_log.h"
#include "esp_err.h"

void ledInit();
void ledOn();
void ledOff();
#endif