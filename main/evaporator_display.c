#include "evaporator_display.h"

static const char *TAG = "Evaporator display";
static QueueHandle_t timer_queue = NULL;

static bool pins_data[6][9];
static displayData data = {
    .water=false,
    .tasty=false,
    .ion=false,
    .wifi=false,
    .timer=false,
    .small_num=18,
    .h=false,
    .big_num=88,
    .celsius=false,
    .persent=false,
    .auto_text=false,
    .auto_1=false,
    .auto_2=false,
    .auto_3=false,
    .auto_4=false,
};

static void initGndPin(int pin) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(pin, GPIO_PULLDOWN_ONLY);
    //Отключение gnd
    if (disp_gpio_gnd[pin] != GPIO_NUM_NC) {
        gpio_set_level(disp_gpio_gnd[pin], 1);
    }
}

static void offGndPins() {
    for (int i = 0; i < 6; i++) {
        if (disp_gpio_gnd[i] != GPIO_NUM_NC) {
            gpio_set_level(disp_gpio_gnd[i], 1);
        }
    }
}

static void initDispPin(int pin) {
    printf("%d\n", pin);
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_pull_mode(pin, GPIO_PULLUP_ONLY);
    gpio_set_level(disp_gpio_vcc[pin], 1);
}

// входной параметр - цифра (от 0 до 9), на выходе 
// структура с 7 сигналами для отрисовки цифры и двумя сигналами pin8 и pin9 в конце
static void getNumberPins(int num_pin, uint8_t num, bool pin8, bool pin9) {
    bool* out;
    switch (num)
    {
    case 0:
        out = (bool[9]) {true,true,true,true,true,true,false, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 1:
        out = (bool[9]) {false,true,true,false,false,false,false, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 2:
        out = (bool[9]) {true,true,false,true,true,false,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 3:
        out = (bool[9]) {true,true,true,true,false,false,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 4:
        out = (bool[9]) {false,true,true,false,false,true,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 5:
        out = (bool[9]) {true,false,true,true,false,true,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 6:
        out = (bool[9]) {true,false,true,true,true,true,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 7:
        out = (bool[9]) {true,true,true,false,false,false,false, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 8:
        out = (bool[9]) {true,true,true,true,true,true,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    case 9:
        out = (bool[9]) {true,true,true,true,false,true,true, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    default:
        out = (bool[9]) {false,false,false,false,false,false,false, pin8, pin9};
        memcpy(pins_data[num_pin], out, sizeof (bool[9]));
        break;
    }
}

static void refreshPinsData(){
    // gnd0
    pins_data[0][0] = data.water;
    pins_data[0][1] = data.tasty;
    pins_data[0][2] = data.ion;
    pins_data[0][3] = data.wifi;
    pins_data[0][4] = data.timer;
    pins_data[0][5] = data.timer;
    pins_data[0][6] = data.timer;
    pins_data[0][7] = false;
    pins_data[0][8] = false;
    // gnd1  если число двузначное, то выводится единица в старшем разряде
    bool one = (data.small_num > 9) ? true : false;
    getNumberPins(1, data.small_num % 10, one, one);
    // gnd2
    uint8_t first = (data.big_num / 10) % 10; // разряд десятков
    getNumberPins(2, first, false, data.h);
    // gnd3
    uint8_t second = data.big_num % 10; // разряд единиц
    getNumberPins(3, second, data.celsius, data.persent);
    // gnd4
    pins_data[4][0] = data.auto_text;
    pins_data[4][1] = false;
    pins_data[4][2] = data.auto_text;
    pins_data[4][3] = data.auto_1;
    pins_data[4][4] = false;
    pins_data[4][5] = data.auto_1;
    pins_data[4][6] = data.auto_2;
    pins_data[4][7] = false;
    pins_data[4][8] = data.auto_2;
    // gnd5
    pins_data[5][0] = data.auto_3;
    pins_data[5][1] = false;
    pins_data[5][2] = data.auto_3;
    pins_data[5][3] = data.auto_4;
    pins_data[5][4] = false;
    pins_data[5][5] = data.auto_4;
    pins_data[5][6] = false;
    pins_data[5][7] = false;
    pins_data[5][8] = false;
}


static void displayShows(int gnd_num) {
    // отключение всех gnd
    offGndPins();
    // зануление gnd
    gpio_set_level(disp_gpio_gnd[gnd_num], 0);
    // выставление напряжения на пинах
    for (int i = 0; i < 9; i++) {
        gpio_set_level(disp_gpio_vcc[i], pins_data[gnd_num][i]);
    }
    // период обновления экрана 1мс
    bool alarm;
    if(xQueueReceive(timer_queue, &alarm, 10)) {
    }
}


// обновление данных экрана 10 кадров в секунду
static void displayRefreshTask(void* arg) {
    int cyclesPerCadr = 1000 / DISP_FPS / DISP_GPIO_GND_NUM;
    while(1) {
        refreshPinsData();
        // цикл for выполняется за 100мс
        for(int i = 0; i < cyclesPerCadr; i++) {
            for (int j = 0; j < 6; j++) {
                if (disp_gpio_gnd[j] != GPIO_NUM_NC) {
                    displayShows(j);
                }
            }
        }
    }
    vTaskDelete(NULL);
}

// Обработчик прерывания по таймеру
static bool IRAM_ATTR timer_isr_callback(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    bool alarm = true;
    xQueueSendFromISR(timer_queue, &alarm, NULL);
    return true;
}

// Инициализация таймера
void initTimer() {
    timer_queue = xQueueCreate(10, sizeof(bool));
    // Настраиваем параметры таймера
    gptimer_handle_t gptimer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,            // Выбираем источник тактового сигнала для счетчиков
        .direction = GPTIMER_COUNT_UP,                 // Устанавливаем направление счета
        .resolution_hz = 1000000, // 1MHz  // Устанавливаем частоту счета, то есть минимальный интервал времени на 1 тик
    };
    gptimer_new_timer(&timer_config, &gptimer);

    // Подключаем функцию обратного вызова
    gptimer_event_callbacks_t cb_config = {
        .on_alarm = timer_isr_callback,
    };
    gptimer_register_event_callbacks(gptimer, &cb_config, NULL);

    // Задаем параметры счетчика таймера
    gptimer_alarm_config_t alarm_config = {
        .alarm_count = 1000,                        // Конечное значение счетчика = 1мс
        .reload_count = 0,                          // Значение счетчика при автосбросе
        .flags.auto_reload_on_alarm = true,         // Автоперезапуск счетчика таймера разрешен
    };
    gptimer_set_alarm_action(gptimer, &alarm_config);

    // Разрешаем прерывания для данного таймера
    gptimer_enable(gptimer);

    // Запускаем таймер
    gptimer_start(gptimer);
    ESP_LOGI("main", "Hardware timer stated");
}

// HTTP запрос от home assistant
static void refreshDataHumi(void* arg) {
    char* url = "http://192.168.0.249:8123/api/states/sensor.qingping_cgs1_humidity";
    // Параметры конфигурации HTTP-соединения
    esp_http_client_config_t request;
    memset(&request, 0, sizeof(request));
    // Начальный URI
    request.url = url;
    // Транспорт TCP/IP
    request.transport_type = HTTP_TRANSPORT_OVER_TCP;
    // Запрос типа GET
    request.method = HTTP_METHOD_GET;
    // Блокировка задачи на время выполнения обмена с сервером
    request.is_async = false;
    // Закрыть соединение сразу после отправки всех данных
    request.keep_alive_enable = false;
    // Таймаут передачи
    request.timeout_ms = 5000;
    // Разрешить автоматическую переадресацию без ограничений
    request.disable_auto_redirect = false;
    request.max_redirection_count = 0;

    esp_http_client_handle_t client = esp_http_client_init(&request);
    esp_http_client_set_header(client, "Authorization", HOME_ASSISTANT_TOKEN);
    int len_data = 500;
    char response_data[len_data];
    int humi = -1;

    TickType_t prew = xTaskGetTickCount();
    if (client) {
        while (1) {
            // Выполняем запрос
            esp_err_t open_status = esp_http_client_open(client, 0);
            ESP_LOGD(TAG, "OPEN %d", open_status);
            int64_t fetch_header = esp_http_client_fetch_headers(client);
            ESP_LOGD(TAG, "FETCH HEADERS %lld", fetch_header);
            int response = esp_http_client_get_status_code(client);
            ESP_LOGD(TAG, "STATUS CODE = %d", response);

            if (response == 200 && open_status == 0) {
                int response_len = esp_http_client_read(client, response_data, len_data);
                ESP_LOGD(TAG, "LEN READ = %d", response_len);
                ESP_LOGD(TAG, "response data = %s", response_data);
                // Считаем значение влажности
                char* tmp = strstr(response_data, "\"state\":");
                humi = strtol(tmp + 9, NULL, 10);
                data.wifi = true;
                data.big_num = humi;
                ESP_LOGD(TAG, "HUMIDITY = %d", humi);
            } else {
                ESP_LOGI(TAG, "NO INTERNET");
                data.wifi = false;
            };
            esp_http_client_close(client);
            vTaskDelayUntil(&prew, 10000 / portTICK_PERIOD_MS);
        }
        // Освободим ресурсы
        esp_http_client_cleanup(client);
    }
}

void initDisplay() {
    for (int i = 0; i < 6; i++) {
        if (disp_gpio_gnd[i] != GPIO_NUM_NC) {
            initGndPin(disp_gpio_gnd[i]);
        }
    }

    for (int i = 0; i < 9; i++) {
        initDispPin(disp_gpio_vcc[i]);
    }

    refreshPinsData();
    initTimer();

    xTaskCreatePinnedToCore(displayRefreshTask, "refresh_display_task", 4096, NULL, 20, NULL, 1);
    xTaskCreatePinnedToCore(refreshDataHumi, "refreshDataHumi", 4096, NULL, 2, NULL, tskNO_AFFINITY);
}