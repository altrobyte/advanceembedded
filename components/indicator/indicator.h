#ifndef INDICATOR_H
#define INDICATOR_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO Pin Definition
#define LED_PIN GPIO_NUM_2

// Industrial operation modes
typedef enum {
    MODE_NORMAL = 0,   // Slow blink - 1 sec (all systems normal)
    MODE_WARNING = 1,  // Medium blink - 0.5 sec (caution required)
    MODE_ALARM = 2     // Fast blink - 0.2 sec (critical condition)
} operation_mode_ind_t;

// Function declarations
void indicator_basic(void);
void indicator_multi_mode(void);

#endif
