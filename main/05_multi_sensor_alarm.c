/*
 * Multi-Sensor Industrial Monitoring
 * Reads multiple digital inputs (sensors/switches)
 * Activates different alarm patterns based on sensor states
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define SENSOR_1 GPIO_NUM_0   // Boot button (simulates sensor 1)
#define SENSOR_2 GPIO_NUM_15  // External sensor input (if available)
#define TAG "MULTI_SENSOR"

void alarm_pattern_fast(void)
{
    // Fast blink - 5 times
    for(int i = 0; i < 5; i++) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void alarm_pattern_slow(void)
{
    // Slow blink - once
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
}

void app_main(void)
{
    // Configure LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    // Configure sensor inputs
    gpio_reset_pin(SENSOR_1);
    gpio_set_direction(SENSOR_1, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SENSOR_1, GPIO_PULLUP_ONLY);
    
    gpio_reset_pin(SENSOR_2);
    gpio_set_direction(SENSOR_2, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SENSOR_2, GPIO_PULLUP_ONLY);
    
    ESP_LOGI(TAG, "Multi-Sensor Monitoring System Started");
    
    while(1) {
        int sensor1_state = gpio_get_level(SENSOR_1);
        int sensor2_state = gpio_get_level(SENSOR_2);
        
        // Priority checking
        if(sensor1_state == 0) {  // Sensor 1 triggered (critical)
            ESP_LOGE(TAG, "CRITICAL: Sensor 1 triggered!");
            alarm_pattern_fast();
        }
        else if(sensor2_state == 0) {  // Sensor 2 triggered (warning)
            ESP_LOGW(TAG, "WARNING: Sensor 2 triggered!");
            alarm_pattern_slow();
        }
        else {  // All sensors normal
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }
}
