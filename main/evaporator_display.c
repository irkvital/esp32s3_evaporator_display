#include "evaporator_display.h"

static displayData data = {
    .water=true,
    .tasty=true,
    .ion=true,
    .wifi=true,
    .timer=true,
    .small_num=18,
    .h=true,
    .big_num=88,
    .celsius=true,
    .persent=true,
    .auto_text=true,
    .auto_1=true,
    .auto_2=true,
    .auto_3=true,
    .auto_4=true,
};

static pinsData pins_data_gnd1 = {};
static pinsData pins_data_gnd2 = {};
static pinsData pins_data_gnd3 = {};
static pinsData pins_data_gnd4 = {};
static pinsData pins_data_gnd5 = {};
static pinsData pins_data_gnd6 = {};


static void initGndPin(int pin) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
    //Отключение gnd
    gpio_set_level(pin, 1);
}

static void offGndPins() {
    gpio_set_level(GND_1, 1);
    gpio_set_level(GND_2, 1);
    gpio_set_level(GND_3, 1);
    gpio_set_level(GND_4, 1);
    gpio_set_level(GND_5, 1);
    gpio_set_level(GND_6, 1);
}

static void initDispPin(int pin) {
    printf("%d\n", pin);
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    gpio_set_level(pin, 1);
}

static void refreshData(void* arg) {
    while(1) {
        // gnd1
        data.water = rand() % 2;
        data.tasty = rand() % 2;
        data.ion = rand() % 2;
        data.wifi = rand() % 2;
        data.timer = rand() % 2;
        // gnd2
        data.small_num = rand() % 20;
        // gnd3
        data.big_num = rand() % 100;
        data.h = rand() % 2;
        // gnd4
        data.celsius = rand() % 2;
        data.persent = rand() % 2;
        // gnd5
        data.auto_text = rand() % 2;
        data.auto_1 = rand() % 2;
        data.auto_2 = rand() % 2;
        // gnd6
        data.auto_3 = rand() % 2;
        data.auto_4 = rand() % 2;

        printf("CHANGE DATA = %d\n", data.big_num);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



// входной параметр - цифра (от 0 до 9), на выходе 
// структура с 7 сигналами для отрисовки цифры и двумя сигналами pin8 и pin9 в конце
static pinsData getNumberPins(uint8_t num, bool pin8, bool pin9) {
    pinsData out;
    switch (num)
    {
    case 0:
        out = (pinsData) {true,true,true,true,true,true,false, pin8, pin9};
        break;
    case 1:
        out = (pinsData) {false,true,true,false,false,false,false, pin8, pin9};
        break;
    case 2:
        out = (pinsData) {true,true,false,true,true,false,true, pin8, pin9};
        break;
    case 3:
        out = (pinsData) {true,true,true,true,false,false,true, pin8, pin9};
        break;
    case 4:
        out = (pinsData) {false,true,true,false,false,true,true, pin8, pin9};
        break;
    case 5:
        out = (pinsData) {true,false,true,true,false,true,true, pin8, pin9};
        break;
    case 6:
        out = (pinsData) {true,false,true,true,true,true,true, pin8, pin9};
        break;
    case 7:
        out = (pinsData) {true,true,true,false,false,false,false, pin8, pin9};
        break;
    case 8:
        out = (pinsData) {true,true,true,true,true,true,true, pin8, pin9};
        break;
    case 9:
        out = (pinsData) {true,true,true,true,false,true,true, pin8, pin9};
        break;
    default:
        out = (pinsData) {false,false,false,false,false,false,false, pin8, pin9};
        break;
    }
    return out;
}

static void refreshPinsData(){
    // gnd1
    pins_data_gnd1 = (pinsData) {data.water, 
                                data.tasty, 
                                data.ion, 
                                data.wifi, 
                                data.timer, 
                                data.timer, 
                                data.timer, 
                                false, 
                                false};

    // gnd2  если число двузначное, то выводится единица в старшем разряде
    bool one = (data.small_num > 9) ? true : false;
    pins_data_gnd2 = getNumberPins(data.small_num % 10, one, one);

    // gnd3
    uint8_t first = (data.big_num / 10) % 10; // разряд десятков
    pins_data_gnd3 = getNumberPins(first, false, data.h);
    // gnd4
    uint8_t second = data.big_num % 10; // разряд единиц
    pins_data_gnd4 = getNumberPins(second, data.celsius, data.persent);
    // gnd5
    pins_data_gnd5 = (pinsData) {data.auto_text,
                                false,
                                data.auto_text,
                                data.auto_1,
                                false,
                                data.auto_1,
                                data.auto_2,
                                false,
                                data.auto_2};
    // gnd6
    pins_data_gnd6 = (pinsData) {data.auto_3,
                                false,
                                data.auto_3,
                                data.auto_4,
                                false,
                                data.auto_4,
                                false,
                                false,
                                false};

    printf("REFRESH PINS\n");
}


static void displayShows(int gnd_num, pinsData pins_data) {
    // отключение всех gnd
    offGndPins();
    // зануление gnd
    gpio_set_level(gnd_num, 0);
    // выставление напряжения на пинах
    gpio_set_level(DISP_1, pins_data.pin1);
    gpio_set_level(DISP_2, pins_data.pin2);
    gpio_set_level(DISP_3, pins_data.pin3);
    gpio_set_level(DISP_4, pins_data.pin4);
    gpio_set_level(DISP_5, pins_data.pin5);
    gpio_set_level(DISP_6, pins_data.pin6);
    gpio_set_level(DISP_7, pins_data.pin7);
    gpio_set_level(DISP_8, pins_data.pin8);
    gpio_set_level(DISP_9, pins_data.pin9);

    vTaskDelay(1 / portTICK_PERIOD_MS);
}




//нужно будет обновлять по прерываниям, наверное
static void displayRefreshTask(void* arg) {
    while(1) {
        refreshPinsData();
        for(int i = 0; i < 100/4; i++) {
            displayShows(GND_1, pins_data_gnd1);
            displayShows(GND_2, pins_data_gnd2);
            displayShows(GND_3, pins_data_gnd3);
            displayShows(GND_4, pins_data_gnd4);
            displayShows(GND_5, pins_data_gnd6);
            displayShows(GND_6, pins_data_gnd6);
        }
        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}



void initDisplay() {
    initGndPin(GND_1);
    initGndPin(GND_2);
    initGndPin(GND_3);
    initGndPin(GND_4);
    initGndPin(GND_5);
    initGndPin(GND_6);

    initDispPin(DISP_1);
    initDispPin(DISP_2);
    initDispPin(DISP_3);
    initDispPin(DISP_4);
    initDispPin(DISP_5);
    initDispPin(DISP_6);
    initDispPin(DISP_7);
    initDispPin(DISP_8);
    initDispPin(DISP_9);

    refreshPinsData();


    xTaskCreatePinnedToCore(displayRefreshTask, "refresh_display_task", 4096, NULL, 10, NULL, 1);
    xTaskCreatePinnedToCore(refreshData, "refreshData", 4096, NULL, 2, NULL, tskNO_AFFINITY);
}