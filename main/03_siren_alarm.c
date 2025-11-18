/*
 * Industrial Siren Alarm Pattern
 * Creates rising and falling frequency effect
 * Used for emergency situations, fire alarms, evacuations
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2
#define TAG "SIREN"

void siren_pattern(void)
{
    // Rising frequency effect (8 steps)
    for(int i = 0; i < 8; i++) {
        int delay_ms = 50 + (i * 10);  // 50ms to 120ms
        
        for(int j = 0; j < 5; j++) {  // 5 pulses per frequency
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
    }
    
    // Falling frequency effect (8 steps)
    for(int i = 7; i >= 0; i--) {
        int delay_ms = 50 + (i * 10);  // 120ms to 50ms
        
        for(int j = 0; j < 5; j++) {
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGE(TAG, "=== EMERGENCY SIREN ACTIVATED ===");
    ESP_LOGE(TAG, "Evacuate immediately!");
    
    while(1) {
        siren_pattern();  // Creates visual siren effect
    }
}
