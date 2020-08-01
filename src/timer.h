#ifndef TIMER_H
#define TIMER_H

#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_drv_timer.h"

//#include "radio.h"

#define TIMER_MS  (uint32_t)1000

//#define EN_TIMER 1
//#define DSBL_TIMER 0

void init_timer(void);
void deinit_timer(void);

void timer_handler(nrf_timer_event_t event_type, void* p_context);

#endif