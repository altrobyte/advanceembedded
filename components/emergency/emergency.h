#ifndef EMERGENCY_H
#define EMERGENCY_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*
 * Industrial Emergency Alarm Module
 * ---------------------------------
 * This module simulates an industrial emergency alarm system:
 * - One emergency push button (E‑STOP)
 * - One alarm indicator (tower lamp / siren LED)
 * 
 * Use this in factories to demonstrate:
 * - How an emergency stop toggles alarm state
 * - How debouncing ensures reliable input
 * - How fast blinking indicates critical condition
 */

#define BUTTON_PIN  GPIO_NUM_33   // Emergency push button (E‑STOP)
#define ALARM_LED   GPIO_NUM_26    // Alarm indicator (tower light / siren)

/*
 * @brief Run the emergency alarm task (blocking)
 * 
 * Call this from app_main() to start monitoring the emergency button.
 * In a real project, this would normally run in its own FreeRTOS task.
 */
void emergency_alarm_run(void);

#endif
