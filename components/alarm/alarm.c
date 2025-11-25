#include "alarm.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define TAG "ALARM"
#define LED_PIN GPIO_NUM_2

/**
 * @brief Fast alarm pattern - 5 rapid blinks
 * Used for critical alerts
 */
static void alarm_pattern_fast(void)
{
    ESP_LOGE(TAG, "Fast alarm pattern started");
    
    for(int i = 0; i < 5; i++) {
        gpio_set_level(LED_PIN, 1);  // LED ON
        ESP_LOGI(TAG, "Blink %d/5: ON", i++);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
        gpio_set_level(LED_PIN, 0);  // LED OFF
        ESP_LOGI(TAG, "Blink %d/5: OFF", i++);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Slow alarm pattern - 1 slow blink
 * Used for warnings
 */
static void alarm_pattern_slow(void)
{
    ESP_LOGW(TAG, "Slow alarm pattern started");
    
    gpio_set_level(LED_PIN, 1);  // LED ON
    ESP_LOGI(TAG, "LED: ON [500ms]");
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
    gpio_set_level(LED_PIN, 0);  // LED OFF
    ESP_LOGI(TAG, "LED: OFF [500ms]");
    vTaskDelay(500 / portTICK_PERIOD_MS);
}

/**
 * @brief Runs alarm patterns continuously
 * Demonstrates different alarm speeds
 */
void alarm_demo(void)
{
    ESP_LOGI(TAG, "Alarm Demo Started");
    ESP_LOGI(TAG, "Pattern 1: Fast blinks (100ms)");
    ESP_LOGI(TAG, "Pattern 2: Slow blinks (500ms)");
    
    int cycle = 0;
    
    while(1) {
        cycle++;
        ESP_LOGI(TAG, "=== Cycle %d ===", cycle);
        
        // Fast pattern
        ESP_LOGE(TAG, "CRITICAL alarm");
        alarm_pattern_fast();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        // Slow pattern
        ESP_LOGW(TAG, "WARNING alarm");
        alarm_pattern_slow();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
