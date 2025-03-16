#ifndef EVAPORATOR_DISPLAY_H
#define EVAPORATOR_DISPLAY_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define GND_1 GPIO_NUM_13
#define GND_2 GPIO_NUM_12
#define GND_3 GPIO_NUM_11
#define GND_4 GPIO_NUM_10
#define GND_5 GPIO_NUM_9
#define GND_6 GPIO_NUM_4

#define DISP_1 GPIO_NUM_3
#define DISP_2 GPIO_NUM_8
#define DISP_3 GPIO_NUM_18
#define DISP_4 GPIO_NUM_17
#define DISP_5 GPIO_NUM_16
#define DISP_6 GPIO_NUM_15
#define DISP_7 GPIO_NUM_7
#define DISP_8 GPIO_NUM_6
#define DISP_9 GPIO_NUM_5

#define DISP_FPS 10


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

typedef struct {
    bool pin1;
    bool pin2;
    bool pin3;
    bool pin4;
    bool pin5;
    bool pin6;
    bool pin7;
    bool pin8;
    bool pin9;
} pinsData;


void initDisplay();

#endif 