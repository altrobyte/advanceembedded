#include "siren.h"
#include "esp_log.h"
#include <stdio.h>
#include "driver/gpio.h"

#define TAG "SIREN"
#define LED_PIN GPIO_NUM_2

/**
 * @brief Creates a siren pattern with rising and falling frequency effect
 * 
 * This function simulates an emergency siren by:
 * 1. Gradually increasing LED blink speed (rising frequency)
 * 2. Gradually decreasing LED blink speed (falling frequency)
 * 
 * Commonly used in industrial emergency systems, fire alarms, and evacuation alerts
 */
void siren_pattern(void)
{
    ESP_LOGI(TAG, "Starting siren pattern - Rising frequency phase");
    
    // Rising frequency effect - LED blinks faster gradually
    for(int i = 0; i < 8; i++) {
        // Calculate delay: starts at 50ms, increases by 10ms each step
        int delay_ms = 50 + (i * 10);  // 50, 60, 70, 80, 90, 100, 110, 120ms
        
        ESP_LOGI(TAG, "Rising step %d/%d: Blink interval = %dms", i+1, 8, delay_ms);
        
        // Repeat 5 blinks at current frequency
        for(int j = 0; j < 5; j++) {
            gpio_set_level(LED_PIN, 1);  // LED ON
            ESP_LOGD(TAG, "  Blink %d: LED ON  [%dms delay]", j+1, delay_ms);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
            
            gpio_set_level(LED_PIN, 0);  // LED OFF
            ESP_LOGD(TAG, "  Blink %d: LED OFF [%dms delay]", j+1, delay_ms);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
        
        ESP_LOGI(TAG, "Completed 5 blinks at %dms interval", delay_ms);
    }
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Transition: Rising → Falling frequency");
    ESP_LOGI(TAG, "========================================");
    
    // Falling frequency effect - LED blinks slower gradually
    for(int i = 7; i >= 0; i--) {
        // Calculate delay: starts at 120ms, decreases by 10ms each step
        int delay_ms = 50 + (i * 10);  // 120, 110, 100, 90, 80, 70, 60, 50ms
        
        ESP_LOGI(TAG, "Falling step %d/%d: Blink interval = %dms", 8-i, 8, delay_ms);
        
        // Repeat 5 blinks at current frequency
        for(int j = 0; j < 5; j++) {
            gpio_set_level(LED_PIN, 1);  // LED ON
            ESP_LOGD(TAG, "  Blink %d: LED ON  [%dms delay]", j+1, delay_ms);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
            
            gpio_set_level(LED_PIN, 0);  // LED OFF
            ESP_LOGD(TAG, "  Blink %d: LED OFF [%dms delay]", j+1, delay_ms);
            vTaskDelay(delay_ms / portTICK_PERIOD_MS);
        }
        
        ESP_LOGI(TAG, "Completed 5 blinks at %dms interval", delay_ms);
    }
    
    ESP_LOGI(TAG, "One complete siren cycle finished");
}

/**
 * @brief Continuously runs the emergency siren
 * 
 * This function:
 * - Logs emergency activation with timestamp
 * - Runs siren pattern in infinite loop
 * - Tracks total blinks and elapsed time
 * 
 * Training Note: In real systems, this would be triggered by:
 * - Fire detection sensor
 * - Gas leak sensor  
 * - Emergency stop button
 * - Safety system fault
 */
void siren_run(void)
{
    ESP_LOGE(TAG, "========================================");
    ESP_LOGE(TAG, "   EMERGENCY SIREN ACTIVATED");
    ESP_LOGE(TAG, "========================================");
    ESP_LOGE(TAG, "System Status: CRITICAL ALERT");
    ESP_LOGE(TAG, "Action Required: Immediate evacuation/response");
    ESP_LOGE(TAG, "Blink Pattern: 8 rising + 8 falling frequency steps");
    ESP_LOGE(TAG, "Each step: 5 blinks | Total per cycle: 80 blinks");
    
    int cycle_count = 0;
    int total_blinks = 0;
    
    while(1) {
        cycle_count++;
        ESP_LOGW(TAG, "========================================");
        ESP_LOGW(TAG, "Starting CYCLE #%d", cycle_count);
        ESP_LOGW(TAG, "========================================");
        
        // Execute one complete siren pattern (rise + fall)
        siren_pattern();
        
        total_blinks += 80;  // 16 steps × 5 blinks each
        
        ESP_LOGW(TAG, "CYCLE #%d completed | Total blinks: %d", cycle_count, total_blinks);
        ESP_LOGE(TAG, "Emergency still ACTIVE - awaiting manual reset");
        
        // Small pause between cycles for clarity
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
