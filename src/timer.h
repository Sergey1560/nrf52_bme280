#ifndef TIMER_H
#define TIMER_H

#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "radio.h"
#include "sys_config.h"

#define TIMER_MS  (uint32_t)60000

void init_timer(void);
void deinit_timer(void);

void timer_handler(void * p_context);

#endif