#include <stdio.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "indicator.h"
#include "alarm.h"
#include "siren.h"

#define TAG "MAIN"

void app_main(void)
{
    // GPIO init
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "=== Industrial Control System ===");
    ESP_LOGI(TAG, "Select mode to run:");
    
    // Uncomment one function to run:
    
    // indicator_basic();                // Mode 1: Basic indicator
    //  indicator_multi_mode();       // Mode 2: Multi-mode
    // alarm_multi_sensor();        // Mode 3: Sensor alarm
    // siren_run();                // Mode 4: Siren
    // alarm_demo();              // Mode 5: Alarm demo   
    long_press_power_run();
}
