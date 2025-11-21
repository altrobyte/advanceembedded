#ifndef SIREN_H
#define SIREN_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void siren_pattern(void);
void siren_run(void);

#endif
