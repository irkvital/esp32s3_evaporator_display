#ifndef EVAPORATOR_DISPLAY_H
#define EVAPORATOR_DISPLAY_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "esp_http_client.h"

#define DISP_FPS 10

#define DISP_GPIO_GND_NUM 4
static gpio_num_t disp_gpio_gnd[6] =    {
                                            GPIO_NUM_4,
                                            GPIO_NUM_5,
                                            GPIO_NUM_6,
                                            GPIO_NUM_7,
                                            GPIO_NUM_NC,
                                            GPIO_NUM_NC
                                        };

static gpio_num_t disp_gpio_vcc[9] =    {
                                            GPIO_NUM_17,
                                            GPIO_NUM_18,
                                            GPIO_NUM_8,
                                            GPIO_NUM_3,
                                            GPIO_NUM_46,
                                            GPIO_NUM_9,
                                            GPIO_NUM_10,
                                            GPIO_NUM_11,
                                            GPIO_NUM_12
                                        };


#define BEARER "Bearer "
#define HOME_ASSISTANT_TOKEN BEARER CONFIG_HOME_ASSISTANT_TOKEN

typedef struct {
    bool water;
    bool tasty;
    bool ion;
    bool wifi;
    bool timer;
    uint8_t small_num;
    bool h;
    uint8_t big_num;
    bool celsius;
    bool persent;
    bool auto_text;
    bool auto_1;
    bool auto_2;
    bool auto_3;
    bool auto_4;
} displayData;

void initDisplay();
int getDataFromHomeAssistant();

#endif 