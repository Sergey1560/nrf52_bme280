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
#include "saadc.h"

int main(void){
   
   APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
   NRF_LOG_DEFAULT_BACKENDS_INIT();

   NRF_LOG_INFO("Start");
   
   //Start HFCLK for SPI
   clocks_start();

   lfclk_request();
   configure_ram_retention();

   saadc_init();
   spi_init();
   esb_init();

   //Init BME280 
   user_bme280_init();

   //Init App timer 
   init_timer();
     
    while (1) {
            // Enter System ON sleep mode
            __WFE();
            __SEV();
            __WFE();
    }
}
