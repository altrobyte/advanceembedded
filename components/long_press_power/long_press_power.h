#ifndef LONG_PRESS_POWER_H
#define LONG_PRESS_POWER_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Industrial Power Control (Long-Press)
 * -------------------------------------
 * One push button used to safely power ON/OFF a controller or machine:
 *  - Short press  → ignored (safety: avoid accidental power change)
 *  - Long press   → 3 seconds hold required
 *      * If system OFF  → start BOOT sequence
 *      * If system ON   → start SHUTDOWN sequence
 * 
 * LED shows current power state:
 *  - OFF        → LED OFF
 *  - BOOTING    → blinking progress
 *  - ON         → LED solid ON
 *  - SHUTDOWN   → blinking countdown
 */

#define POWER_BUTTON_PIN  GPIO_NUM_33   // Front-panel power button
#define POWER_LED_PIN     GPIO_NUM_26    // Power status indicator LED

// System states for trainees to understand lifecycle
typedef enum {
    SYSTEM_OFF = 0,           // Controller fully powered down
    SYSTEM_BOOTING = 1,       // Power up sequence in progress
    SYSTEM_ON = 2,            // Controller running
    SYSTEM_SHUTTING_DOWN = 3  // Graceful shutdown in progress
} system_state_t;

// Start the long-press power controller (blocking loop)
void long_press_power_run(void);

#endif
