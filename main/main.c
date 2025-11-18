/*
 * Basic Industrial Status Indicator
 * Normal operation mode - slow blink (1 second ON/OFF)
 * Used in factories to show machine running status
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define TAG "INDICATOR"

void app_main(void)
{
    // Configure LED GPIO as output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "Industrial Status Indicator Started");
    ESP_LOGI(TAG, "Mode: NORMAL OPERATION");
    
    int cycle_count = 0;
    
    while(1) {
        // LED ON - Machine Running
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "Status: RUNNING | Cycle: %d", cycle_count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // LED OFF
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        cycle_count++;
    }
}
