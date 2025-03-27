#include "main.h"

void app_main(void)
{
    initWifi();
    buttonFixInit();
    initDisplay();
    touchTask();
}
    
