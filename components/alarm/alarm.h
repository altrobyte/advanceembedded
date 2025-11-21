#ifndef ALARM_H
#define ALARM_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// GPIO Pin Definitions
#define LED_PIN GPIO_NUM_2      // Status LED
#define SENSOR_1 GPIO_NUM_0     // Critical sensor (Boot button on ESP32)
#define SENSOR_2 GPIO_NUM_15    // Warning sensor (External input)

// Function Declaration
void alarm_demo(void);

#endif
