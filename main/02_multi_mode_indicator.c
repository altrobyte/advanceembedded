/*
 * Multi-Mode Industrial Indicator
 * Three modes: Normal, Warning, Alarm
 * Automatically switches based on cycles (simulates condition change)
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define TAG "MULTI_MODE"

// Industrial operation modes
typedef enum {
    MODE_NORMAL = 0,   // Slow blink - 1 sec
    MODE_WARNING = 1,  // Medium blink - 0.5 sec
    MODE_ALARM = 2     // Fast blink - 0.2 sec
} operation_mode_t;

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "Multi-Mode Industrial Indicator Started");
    
    operation_mode_t current_mode = MODE_NORMAL;
    int cycle_count = 0;
    int on_time_ms, off_time_ms;
    const char* mode_name;
    
    while(1) {
        // Select timing based on current mode
        switch(current_mode) {
            case MODE_NORMAL:
                on_time_ms = 1000;
                off_time_ms = 1000;
                mode_name = "NORMAL";
                break;
            case MODE_WARNING:
                on_time_ms = 500;
                off_time_ms = 500;
                mode_name = "WARNING";
                break;
            case MODE_ALARM:
                on_time_ms = 200;
                off_time_ms = 200;
                mode_name = "ALARM";
                break;
        }
        
        // Blink cycle
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "Mode: %s | Cycle: %d | LED: ON", mode_name, cycle_count);
        vTaskDelay(on_time_ms / portTICK_PERIOD_MS);
        
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(off_time_ms / portTICK_PERIOD_MS);
        
        cycle_count++;
        
        // Simulate condition deterioration - mode changes
        if(cycle_count == 10 && current_mode == MODE_NORMAL) {
            current_mode = MODE_WARNING;
            ESP_LOGW(TAG, "!!! Switching to WARNING mode !!!");
        }
        else if(cycle_count == 20 && current_mode == MODE_WARNING) {
            current_mode = MODE_ALARM;
            ESP_LOGE(TAG, "!!! ALARM MODE ACTIVATED !!!");
        }
    }
}
