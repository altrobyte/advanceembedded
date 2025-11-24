#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Our three training demos
#include "emergency.h"        // Single press toggle emergency alarm
#include "mode_selector.h"    // Multi-press mode selection (MANUAL/AUTO/MAINT)
#include "long_press_power.h" // Long-press power on/off controller

#define TAG "MAIN_CONTROL_PANEL"

/*
 * HOW TO USE THIS MAIN FILE (FOR TRAINEES):
 * ----------------------------------------
 * We have 3 separate applications, each in its own .c/.h file:
 *  1) Emergency alarm     → emergency_alarm_run()
 *  2) Mode selector       → mode_selector_run()
 *  3) Long-press power    → long_press_power_run()
 * 
 * To run a specific demo:
 *  - Go to app_main()
 *  - Comment out the others
 *  - Uncomment the one you want to try
 * 
 * Only ONE of these functions should be active at a time because
 * each contains an infinite while(1) loop.
 */

void app_main(void)
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Industrial Automation Training - Button & LED Demos");
    ESP_LOGI(TAG, "Board: ESP32  |  OS: FreeRTOS");
    ESP_LOGI(TAG, "Select which demo to run by editing app_main()");
    ESP_LOGI(TAG, "========================================");
    
    /*
     * DEMO 1: Emergency Alarm System
     *  - Single press toggles an emergency alarm
     *  - Fast blink = critical alarm
     * 
     * Typical use:
     *  - Panic/E-STOP button indicator
     */
    // emergency_alarm_run();
    
    /*
     * DEMO 2: Machine Mode Selector
     *  - 1 press  → MANUAL mode
     *  - 2 presses → AUTO mode
     *  - 3+ presses → MAINTENANCE mode
     *  
     * LED blink speed shows current mode.
     */
    // mode_selector_run();
    
    /*
     * DEMO 3: Long-Press Power Control
     *  - Hold button for 3 seconds to POWER ON/OFF
     *  - Short presses are ignored (safety)
     *  - LED shows power state (OFF/BOOTING/ON/SHUTTING_DOWN)
     */
    long_press_power_run();
    
    /*
     * NOTE:
     *  The selected function above never returns (infinite loop).
     *  In advanced projects, each demo would run in its own FreeRTOS task
     *  and a higher-level menu would control which task is active.
     */
}
