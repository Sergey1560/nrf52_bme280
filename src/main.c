#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_drv_saadc.h"


#include "bme280_hal.h"
#include "sys_config.h"
#include "timer.h"
#include "radio.h"

int main(void)
{
    clocks_start();

   APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
   NRF_LOG_DEFAULT_BACKENDS_INIT();

   NRF_LOG_INFO("Start at %d Mhz",SystemCoreClock);
   sys_info();
    
    esb_init();
    init_timer();
    //user_bme280_init();

    while (1) {
    // Make sure the event register is cleared
    __WFE();
    __SEV();
    // Enter System ON sleep mode
    __WFE();    
    }
}
