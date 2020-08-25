#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "app_error.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "sys_config.h"
#include "timer.h"
#include "radio.h"

int main(void){
   
   APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
   NRF_LOG_DEFAULT_BACKENDS_INIT();

   NRF_LOG_INFO("Start");
   
   
   lfclk_request();
   configure_ram_retention();
   
   esb_init();
   user_bme280_init();
   init_timer();
     
    while (1) {
            // Enter System ON sleep mode
            if(!NRF_LOG_PROCESS()){
            __WFE();
            __SEV();
            __WFE();
            }
    }
}
