#include <stdio.h>
#include "mode_selector.h"
#include "esp_log.h"

#define TAG "MODE_SELECTOR"

#define DEBOUNCE_DELAY 50  // 50ms debounce

// Mode names for logging
static const char* mode_names[] = {"MANUAL", "AUTO", "MAINTENANCE"};

void mode_selector_run(void)
{
    // Configure button as input with pull-up
    gpio_reset_pin(MODE_BUTTON_PIN);
    gpio_set_direction(MODE_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(MODE_BUTTON_PIN, GPIO_PULLUP_ONLY);

    // Configure status LED as output
    gpio_reset_pin(MODE_STATUS_LED);
    gpio_set_direction(MODE_STATUS_LED, GPIO_MODE_OUTPUT);

    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "Industrial Machine Mode Selector Ready");
    ESP_LOGI(TAG, "Button: GPIO %d  |  LED: GPIO %d",
             MODE_BUTTON_PIN, MODE_STATUS_LED);
    ESP_LOGI(TAG, "Each valid press = switch to NEXT mode:");
    ESP_LOGI(TAG, "  MANUAL  → AUTO → MAINTENANCE → MANUAL ...");
    ESP_LOGI(TAG, "========================================");

    operation_mode_t current_mode = MODE_MANUAL;
    int last_state = 1;  // HIGH initially (pull-up)

    while (1) {
        int current_state = gpio_get_level(MODE_BUTTON_PIN);

        // Detect falling edge (released -> pressed) with debounce
        if (last_state == 1 && current_state == 0) {
            vTaskDelay(DEBOUNCE_DELAY / portTICK_PERIOD_MS);
            current_state = gpio_get_level(MODE_BUTTON_PIN);

            if (current_state == 0) {
                // ONE confirmed press → move to next mode
                if (current_mode == MODE_MANUAL) {
                    current_mode = MODE_AUTO;
                } else if (current_mode == MODE_AUTO) {
                    current_mode = MODE_MAINTENANCE;
                } else {
                    current_mode = MODE_MANUAL;
                }

                ESP_LOGI(TAG, "Mode changed to: %s", mode_names[current_mode]);

                // Small feedback blink on change
                gpio_set_level(MODE_STATUS_LED, 1);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                gpio_set_level(MODE_STATUS_LED, 0);
            }
        }

        // Show mode with blink rate (runs continuously)
        int blink_delay;
        switch (current_mode) {
            case MODE_MANUAL:      // slow
                blink_delay = 1000;
                break;
            case MODE_AUTO:        // medium
                blink_delay = 500;
                break;
            case MODE_MAINTENANCE: // fast
                blink_delay = 200;
                break;
            default:
                blink_delay = 1000;
                break;
        }

        gpio_set_level(MODE_STATUS_LED, 1);
        vTaskDelay(blink_delay / portTICK_PERIOD_MS);
        gpio_set_level(MODE_STATUS_LED, 0);
        vTaskDelay(blink_delay / portTICK_PERIOD_MS);

        last_state = current_state;
    }
}
