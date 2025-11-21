#include "indicator.h"
#include "esp_log.h"
#include <stdio.h>
#include "driver/gpio.h"

#define TAG "INDICATOR"
#define LED_PIN GPIO_NUM_2

/**
 * @brief Basic industrial status indicator - Normal operation mode
 * 
 * This function creates a slow, steady blink pattern (1 second ON/OFF)
 * Used in factories to show machine is running normally
 * 
 * Training Note: This is the most common indicator pattern in industrial automation
 * - Slow blink = Machine operating normally
 * - No blink = Machine stopped/powered off
 * 
 * 
 * ==========================================================================
 * 
 * vTaskDelay() explanation:
 * 
 * vTaskDelay(1000 / portTICK_PERIOD_MS);
 * 
 * - vTaskDelay() = FreeRTOS function to pause/delay a task
 * - 1000 = delay in milliseconds (1000ms = 1 second)
 * - portTICK_PERIOD_MS = converts milliseconds to system ticks
 * - On ESP32, 1 tick = 1ms typically
 * 
 * Why divide by portTICK_PERIOD_MS?
 * - vTaskDelay() accepts TICKS, not milliseconds
 * - portTICK_PERIOD_MS = how many ms per tick
 * - So: milliseconds / portTICK_PERIOD_MS = ticks
 * 
 * Example:
 * vTaskDelay(1000 / portTICK_PERIOD_MS);  // 1000ms = 1 second delay
 * vTaskDelay(500 / portTICK_PERIOD_MS);   // 500ms = 0.5 second delay
 * vTaskDelay(100 / portTICK_PERIOD_MS);   // 100ms = 0.1 second delay
 * 
 * Important: This is NON-BLOCKING delay
 * - Other tasks can run during this delay
 * - Better than Arduino delay() which blocks everything
 */


void indicator_basic(void)
{
    int cycle_count = 0;
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Basic Status Indicator Initialized");
    ESP_LOGI(TAG, "Mode: NORMAL OPERATION");
    ESP_LOGI(TAG, "Pattern: 1000ms ON | 1000ms OFF");
    ESP_LOGI(TAG, "========================================");
    
    while(1) {
        // LED ON phase
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "Cycle: %d | LED: ON  | Duration: 1000ms | Status: RUNNING", cycle_count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // LED OFF phase
        gpio_set_level(LED_PIN, 0);
        ESP_LOGD(TAG, "Cycle: %d | LED: OFF | Duration: 1000ms", cycle_count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        cycle_count++;
        
        // Log milestone every 10 cycles
        if(cycle_count % 10 == 0) {
            ESP_LOGI(TAG, "Milestone: %d cycles completed | Uptime: ~%d seconds", 
                     cycle_count, cycle_count * 2);
        }
    }
}

/**
 * @brief Multi-mode industrial indicator with automatic condition escalation
 * 
 * This function demonstrates three operational states:
 * - NORMAL (1000ms): Machine running smoothly
 * - WARNING (500ms): Machine needs attention (temperature high, vibration detected)
 * - ALARM (200ms): Critical condition (immediate action required)
 * 
 * Training Note: Real systems use sensors to trigger mode changes:
 * - Temperature sensors
 * - Vibration sensors
 * - Pressure sensors
 * - Quality control checks
 */
void indicator_multi_mode(void)
{
    operation_mode_t current_mode = MODE_NORMAL;
    int cycle_count = 0;
    int on_time_ms, off_time_ms;
    const char* mode_name;
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Multi-Mode Industrial Indicator Started");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Mode Timings:");
    ESP_LOGI(TAG, "  NORMAL:  1000ms (slow blink)");
    ESP_LOGI(TAG, "  WARNING:  500ms (medium blink)");
    ESP_LOGI(TAG, "  ALARM:    200ms (fast blink)");
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Starting in NORMAL mode...");
    
    while(1) {
        // Select timing and name based on current mode
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
            default:
                on_time_ms = 1000;
                off_time_ms = 1000;
                mode_name = "UNKNOWN";
                break;
        }
        
        // LED ON phase
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "Mode: %s | Cycle: %d | LED: ON  | Interval: %dms", 
                 mode_name, cycle_count, on_time_ms);
        vTaskDelay(on_time_ms / portTICK_PERIOD_MS);
        
        // LED OFF phase
        gpio_set_level(LED_PIN, 0);
        ESP_LOGD(TAG, "Mode: %s | Cycle: %d | LED: OFF | Interval: %dms", 
                 mode_name, cycle_count, off_time_ms);
        vTaskDelay(off_time_ms / portTICK_PERIOD_MS);
        
        cycle_count++;
        
        // Simulate condition deterioration - automatic mode escalation
        // In real systems, these would be triggered by sensor readings
        
        if(cycle_count == 10 && current_mode == MODE_NORMAL) {
            ESP_LOGW(TAG, "========================================");
            ESP_LOGW(TAG, "!!! CONDITION CHANGE DETECTED !!!");
            ESP_LOGW(TAG, "Switching: NORMAL → WARNING");
            ESP_LOGW(TAG, "Reason: Simulated condition deterioration");
            ESP_LOGW(TAG, "Real trigger: Temperature/Vibration threshold exceeded");
            ESP_LOGW(TAG, "========================================");
            current_mode = MODE_WARNING;
        }
        else if(cycle_count == 20 && current_mode == MODE_WARNING) {
            ESP_LOGE(TAG, "========================================");
            ESP_LOGE(TAG, "!!! CRITICAL CONDITION DETECTED !!!");
            ESP_LOGE(TAG, "Switching: WARNING → ALARM");
            ESP_LOGE(TAG, "Reason: Condition worsened");
            ESP_LOGE(TAG, "Real trigger: Critical threshold reached");
            ESP_LOGE(TAG, "Action: Operator intervention required");
            ESP_LOGE(TAG, "========================================");
            current_mode = MODE_ALARM;
        }
        
        // Log summary every 5 cycles in WARNING or ALARM mode
        if(current_mode != MODE_NORMAL && cycle_count % 5 == 0) {
            ESP_LOGW(TAG, "Status Update: %d cycles in %s mode | Total uptime: ~%d seconds", 
                     cycle_count, mode_name, (cycle_count * on_time_ms * 2) / 1000);
        }
    }
}
