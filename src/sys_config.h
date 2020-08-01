#ifndef SYSCFG_H
#define SYSCFG_H

#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


void sys_info(void);
void clocks_start(void);

#endif
