#ifndef MODE_SELECTOR_H
#define MODE_SELECTOR_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Industrial Machine Mode Selector
 * --------------------------------
 * Single push-button used to select machine operating mode:
 *  - 1 press  → MANUAL mode
 *  - 2 presses → AUTO mode
 *  - 3+ presses → MAINTENANCE mode
 * 
 * LED indicates current mode using different blink speeds.
 */

// Button and LED pins (can be shared with other modules if needed)
#define MODE_BUTTON_PIN  GPIO_NUM_33   // Front-panel mode select push button
#define MODE_STATUS_LED  GPIO_NUM_26    // Panel status indicator LED

// Human-readable machine modes
typedef enum {
    MODE_MANUAL = 0,        // Operator controlled
    MODE_AUTO = 1,          // Automatic cycle
    MODE_MAINTENANCE = 2    // Service/maintenance
} operation_mode_t;

// Start the mode selector application (blocking loop)
void mode_selector_run(void);

#endif
