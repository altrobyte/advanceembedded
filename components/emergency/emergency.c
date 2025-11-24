#include <stdio.h>
#include <stdbool.h>
#include "emergency.h"
#include "esp_log.h"

#define TAG "EMERGENCY_ALARM"

/*
 * DEBOUNCING EXPLAINED (Industrial View):
 * ---------------------------------------
 * Industrial emergency buttons are mechanical.
 * When pressed, internal contacts physically move and "bounce":
 *  - For a few milliseconds they rapidly open/close
 *  - PLC or controller can see this as many ON/OFF events
 * 
 * In safety systems we want ONE clean event per press.
 * Software debounce = wait a short time, then recheck.
 *  - Here: 50 ms is used (commonly safe for panel push buttons)
 */
#define DEBOUNCE_DELAY 50  // Wait 50ms to confirm button press

void emergency_alarm_run(void)
{
    /*
     * GPIO SETUP - EMERGENCY PUSH BUTTON
     * ----------------------------------
     * Input with internal PULL‑UP:
     *  - Normal (not pressed)  → logic HIGH (1)
     *  - Pressed (to GND)      → logic LOW  (0)
     * 
     * This wiring is common in industrial panels:
     *  - Fewer external resistors
     *  - Better noise immunity
     */

    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    
    /*
     * GPIO SETUP - ALARM INDICATOR
     * -----------------------------
     * Output driving:
     *  - Panel LED
     *  - Tower light
     *  - Small siren via driver
     */

    gpio_reset_pin(ALARM_LED);
    gpio_set_direction(ALARM_LED, GPIO_MODE_OUTPUT);
    
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Industrial Emergency Alarm Module Ready");
    ESP_LOGI(TAG, "Button: GPIO %d (E-STOP simulation)", BUTTON_PIN);
    ESP_LOGI(TAG, "Alarm LED: GPIO %d (tower lamp / siren)", ALARM_LED);
    ESP_LOGI(TAG, "Press button to TOGGLE emergency alarm state");
    ESP_LOGI(TAG, "========================================");
    
    /*
     * STATE VARIABLES:
     * ----------------
     * alarm_active:
     *  - false → System in normal condition, alarm off
     *  - true  → Emergency state active, alarm blinking fast
     * 
     * last_button_state:
     *  - Used to detect the "edge" (transition) from not‑pressed → pressed
     *  - Avoids multiple triggers while button is held
     * 
     * alarm_count:
     *  - How many times emergency was activated
     *  - Useful for logging and analysis in real plants
     */

    bool alarm_active = false;
    int last_button_state = 1;  // Start HIGH due to pull‑up (button released)
    int alarm_count = 0;
    
    while(1) {
        /*
         * READ BUTTON STATE:
         * ------------------
         * gpio_get_level():
         *  - Returns 1 → input HIGH
         *  - Returns 0 → input LOW
         * 
         * With pull‑up wiring:
         *  - 1 → Button NOT pressed (normal)
         *  - 0 → Button PRESSED (emergency)
         */
        int current_state = gpio_get_level(BUTTON_PIN);
        
        /*
         * EDGE DETECTION (HIGH → LOW):
         * ----------------------------
         * Detect only the transition from:
         *  - last = 1 (released)
         *  - now  = 0 (just pressed)
         * 
         * This ensures ONE event per press, even if loop runs very fast.
         */
        if(last_button_state == 1 && current_state == 0) {
            
            /*
             * SOFTWARE DEBOUNCE STEP:
             * -----------------------
             * 1. Detected possible press
             * 2. Wait DEBOUNCE_DELAY ms for contacts to settle
             * 3. Read again to confirm it is a real press
             */
            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);
            current_state = gpio_get_level(BUTTON_PIN);
            
            if(current_state == 0) {  // Still LOW → confirmed valid press
                /*
                 * TOGGLE EMERGENCY STATE:
                 * -----------------------
                 * alarm_active = !alarm_active;
                 *  - If alarm OFF → turn ON (enter emergency state)
                 *  - If alarm ON  → turn OFF (acknowledge/reset)
                 */
                alarm_active = !alarm_active;
                alarm_count++;
                
                if(alarm_active) {
                    ESP_LOGE(TAG, "----------------------------------------");
                    ESP_LOGE(TAG, "!!! EMERGENCY ALARM TRIGGERED #%d !!!", alarm_count);
                    ESP_LOGE(TAG, "Status: CRITICAL");
                    ESP_LOGE(TAG, "Action: Stop machine / alert operator");
                    ESP_LOGE(TAG, "----------------------------------------");
                } else {
                    ESP_LOGI(TAG, "Emergency alarm reset - System back to NORMAL");
                }
            }
        }
        
        /*
         * ALARM VISUAL PATTERN:
         * ---------------------
         * When alarm_active == true:
         *  - Blink LED fast (100ms ON / 100ms OFF)
         *  - Represents high‑priority emergency in industrial panels
         * 
         * When alarm_active == false:
         *  - LED OFF (no active alarm)
         */
        if(alarm_active) {
            gpio_set_level(ALARM_LED, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(ALARM_LED, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        } else {
            gpio_set_level(ALARM_LED, 0);
            vTaskDelay(50 / portTICK_PERIOD_MS);  // Small delay to reduce CPU usage
        }
        
        // Save current state to detect edge in next loop
        last_button_state = current_state;
    }
}
