/*
 * Digital GPIO Input Reading - Emergency Button
 * Reads button state and activates alarm when pressed
 * Common in industrial emergency stop systems
 */

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2       // Alarm indicator
#define BUTTON_PIN GPIO_NUM_0    // Boot button (active LOW)
#define TAG "EMERGENCY"

void app_main(void)
{
    // Configure LED as output
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);  // Start OFF
    
    // Configure button as input with pull-up
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    ESP_LOGI(TAG, "Emergency Button System Ready");
    ESP_LOGI(TAG, "Press BOOT button to activate alarm");
    
    int button_state;
    bool alarm_active = false;
    
    while(1) {
        // Read button state (0 = pressed, 1 = not pressed)
        button_state = gpio_get_level(BUTTON_PIN);
        
        if(button_state == 0) {  // Button pressed
            if(!alarm_active) {
                ESP_LOGE(TAG, "!!! EMERGENCY BUTTON PRESSED !!!");
                ESP_LOGE(TAG, "!!! ALARM ACTIVATED !!!");
                alarm_active = true;
            }
            
            // Fast blink when alarm active
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        else {  // Button released
            if(alarm_active) {
                ESP_LOGI(TAG, "Emergency cleared - System normal");
                alarm_active = false;
            }
            
            // Slow blink when normal
            gpio_set_level(LED_PIN, 1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            gpio_set_level(LED_PIN, 0);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}
