#include "timer.h"
#include "bme280_hal.h"
#include "radio.h"
#include "nrf_drv_saadc.h"

APP_TIMER_DEF(timer_id);

int saadc_measure() {
  nrf_saadc_value_t value;
  nrf_drv_saadc_sample_convert(0, &value);
  return value;
}


void init_timer(void){
   ret_code_t err_code;

    app_timer_init();
    // Create timers
    err_code = app_timer_create(&timer_id, APP_TIMER_MODE_REPEATED, timer_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(timer_id, APP_TIMER_TICKS(TIMER_MS), NULL);
    APP_ERROR_CHECK(err_code);

}


void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event) {
	// Do nothing, as we use blocking mode
}

int saadc_init() {
  int ret = nrf_drv_saadc_init(NULL, saadc_event_handler);
  if (ret) return ret;
  nrf_saadc_channel_config_t config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);
  ret = nrf_drv_saadc_channel_init(0, &config);
  return ret;
}

int saadc_deinit() {
  int ret = nrf_drv_saadc_init(NULL, saadc_event_handler);
  if (ret) return ret;
  nrf_saadc_channel_config_t config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);
  ret = nrf_drv_saadc_channel_init(0, &config);
  return ret;
}


void timer_handler(void * p_context){
    NRF_LOG_INFO("Timer");
    struct bme280_data bme_data;
    struct tx_data bme_tx_data;

    int16_t adc_value;
    double d;

    clocks_start();

    saadc_init();
    spi_init();
    adc_value = saadc_measure();
    
    if(adc_value < 0) adc_value = 0;
    
    d = (3.3 * adc_value) / 1024.0;
    d = d * 3.47826 * 100.0;

    bme_data = user_bme280_poll();

    nrf_drv_saadc_uninit();
    spi_uninit();

    bme_tx_data.temperature = bme_data.temperature;
    bme_tx_data.humidity = bme_data.humidity;
    bme_tx_data.pressure = bme_data.pressure;
    bme_tx_data.vbat = (uint32_t)d;
   
    send_esb_packet(&bme_tx_data);
   
}

