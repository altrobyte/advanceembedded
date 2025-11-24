#include <stdio.h>
#include <stdbool.h>
#include "long_press_power.h"
#include "esp_log.h"

#define TAG "POWER_SYSTEM"

#define DEBOUNCE_DELAY    20     // 50ms debounce for button
#define LONG_PRESS_TIME 3000     // 3000ms (3 seconds) long-press threshold

// For logging readable state names
static const char* state_names[] = {
    "OFF", "BOOTING", "ON", "SHUTTING DOWN"
};

void long_press_power_run(void)
{
    /*
     * BUTTON AS INPUT WITH PULL-UP:
     *  - Normal (not pressed) → reads HIGH (1)
     *  - Pressed (to GND)     → reads LOW  (0)
     */
    gpio_reset_pin(POWER_BUTTON_PIN);
    gpio_set_direction(POWER_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(POWER_BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    /*
     * LED AS OUTPUT:
     *  - Used to show power/system state to operator
     */
    gpio_reset_pin(POWER_LED_PIN);
    gpio_set_direction(POWER_LED_PIN, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Industrial Long-Press Power Controller");
    ESP_LOGI(TAG, "Button: GPIO %d  |  Power LED: GPIO %d",
             POWER_BUTTON_PIN, POWER_LED_PIN);
    ESP_LOGI(TAG, "Hold button for 3 seconds to POWER ON/OFF safely");
    ESP_LOGI(TAG, "Short presses are ignored (safety feature).");
    ESP_LOGI(TAG, "========================================");
    
    /*
     * VARIABLES:
     *  - state            → current power state
     *  - press_start_time → when user started pressing (ms)
     *  - button_active    → are we currently timing a press?
     *  - last_level       → previous button logic level (for edge detection)
     */
    system_state_t state = SYSTEM_OFF;
    uint32_t press_start_time = 0;
    bool button_active = false;
    int last_level = 1;  // starts HIGH due to pull-up
    int boot_cycles = 0;
    
    while(1) {
        int level = gpio_get_level(POWER_BUTTON_PIN);
        uint32_t now_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        /*
         * FALLING EDGE DETECTION (button just pressed):
         *  last_level = 1 (released) → level = 0 (pressed)
         */
        if(last_level == 1 && level == 0) {
            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);  // Debounce
            level = gpio_get_level(POWER_BUTTON_PIN);
            
            if(level == 0) {  // Confirmed press
                press_start_time = now_ms;
                button_active = true;
                ESP_LOGI(TAG, "Button pressed - hold for 3 seconds to toggle power");
            }
        }
        
        /*
         * WHILE BUTTON IS HELD:
         *  - Measure press duration
         *  - Provide visual feedback using LED while counting
         */
        if(button_active && level == 0) {
            uint32_t press_duration = now_ms - press_start_time;
            
            // Feedback: blink LED slowly while user holds button
            if(press_duration % 500 < 250) {
                gpio_set_level(POWER_LED_PIN, 1);  // ON half the time
            } else {
                gpio_set_level(POWER_LED_PIN, 0);  // OFF half the time
            }
            
            /*
             * LONG-PRESS REACHED:
             *  - If held for at least LONG_PRESS_TIME (3000ms)
             *  - Perform BOOT or SHUTDOWN depending on current state
             */
            if(press_duration >= LONG_PRESS_TIME) {
                button_active = false;  // Don't re-trigger until next press
                
                if(state == SYSTEM_OFF) {
                    // BOOT SEQUENCE
                    state = SYSTEM_BOOTING;
                    boot_cycles++;
                    ESP_LOGI(TAG, "========================================");
                    ESP_LOGI(TAG, "LONG PRESS DETECTED - Starting BOOT sequence #%d", boot_cycles);
                    ESP_LOGI(TAG, "========================================");
                    
                    // Fake boot progress for training
                    for(int progress = 0; progress <= 100; progress += 25) {
                        ESP_LOGI(TAG, "Boot progress: %d%%", progress);
                        gpio_set_level(POWER_LED_PIN, 1);
                        vTaskDelay(250 / portTICK_PERIOD_MS);
                        gpio_set_level(POWER_LED_PIN, 0);
                        vTaskDelay(150 / portTICK_PERIOD_MS);
                    }
                    
                    state = SYSTEM_ON;
                    ESP_LOGI(TAG, "System state: %s", state_names[state]);
                    ESP_LOGI(TAG, "Controller is now ONLINE and ready.");
                }
                else if(state == SYSTEM_ON) {
                    // SHUTDOWN SEQUENCE
                    state = SYSTEM_SHUTTING_DOWN;
                    ESP_LOGW(TAG, "========================================");
                    ESP_LOGW(TAG, "LONG PRESS DETECTED - Starting SHUTDOWN sequence");
                    ESP_LOGW(TAG, "========================================");
                    
                    // Fake shutdown progress for training
                    for(int progress = 100; progress >= 0; progress -= 25) {
                        ESP_LOGW(TAG, "Shutdown progress: %d%%", progress);
                        gpio_set_level(POWER_LED_PIN, 1);
                        vTaskDelay(150 / portTICK_PERIOD_MS);
                        gpio_set_level(POWER_LED_PIN, 0);
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                    }
                    
                    state = SYSTEM_OFF;
                    ESP_LOGI(TAG, "System state: %s", state_names[state]);
                    ESP_LOGI(TAG, "Controller is now safely powered OFF.");
                }
                
                // Wait until user releases button to avoid re-trigger
                while(gpio_get_level(POWER_BUTTON_PIN) == 0) {
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                }
            }
        }
        
        /*
         * BUTTON RELEASED BEFORE LONG-PRESS TIME:
         *  - Short press (<3s) is intentionally ignored
         *  - This prevents accidental on/off events
         */
        if(button_active && last_level == 0 && level == 1) {
            uint32_t press_duration = now_ms - press_start_time;
            if(press_duration < LONG_PRESS_TIME) {
                ESP_LOGI(TAG,
                         "Short press ignored (held %d ms, need %d ms for power action)",
                         press_duration, LONG_PRESS_TIME);
            }
            button_active = false;
        }
        
        /*
         * LED INDICATION OF FINAL STATE:
         *  - SYSTEM_ON  → LED solid ON
         *  - SYSTEM_OFF → LED OFF
         *  (BOOTING/SHUTTING_DOWN blinks are handled in their sequences)
         */
        if(state == SYSTEM_ON) {
            gpio_set_level(POWER_LED_PIN, 1);
        } else if(state == SYSTEM_OFF && !button_active) {
            gpio_set_level(POWER_LED_PIN, 0);
        }
        
        last_level = level;
        vTaskDelay(50 / portTICK_PERIOD_MS);  // Small delay per loop
    }
}
 