#include "timer.h"
#include "bme280_hal.h"
#include "radio.h"
#include "saadc.h"

APP_TIMER_DEF(timer_id);


void init_timer(void){
    ret_code_t err_code;

    err_code=app_timer_init();
    APP_ERROR_CHECK(err_code);
    
    // Create timers
    err_code = app_timer_create(&timer_id, APP_TIMER_MODE_REPEATED, timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(timer_id, APP_TIMER_TICKS(TIMER_MS), NULL);
    APP_ERROR_CHECK(err_code);

}


void timer_handler(void * p_context){
    NRF_LOG_INFO("Timer");
    struct bme280_data bme_data;
    struct tx_data bme_tx_data;
    static uint32_t count=0;

    int16_t adc_value;
    int16_t solar_value;
    double d,p;

    //Start HFCLK for SPI
    clocks_start();
    
    adc_value = saadc_measure(0);
    solar_value = saadc_measure(1);
    
    if(adc_value < 0) adc_value = 0;
    if(solar_value < 0) solar_value = 0;
    
    d = (3.3 * adc_value) / 1024.0;
    d = d * 1.55 * 100.0;

    p = (3.3 * solar_value) / 1024.0;
    p = p * 2 * 1.1 * 100.0;

    bme_data = user_bme280_poll();

    bme_tx_data.count = count;
    bme_tx_data.temperature = bme_data.temperature;
    bme_tx_data.humidity = bme_data.humidity;
    bme_tx_data.pressure = bme_data.pressure;
    bme_tx_data.vbat = (uint32_t)d;
    bme_tx_data.solar_bat = (uint32_t)p;
   
    NRF_LOG_INFO("Cnt: %d Bat %d Solar %d Temp: %d",count++,d,p,bme_tx_data.temperature);

    send_esb_packet(&bme_tx_data);
    
}

