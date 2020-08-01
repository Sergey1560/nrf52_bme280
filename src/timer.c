#include "timer.h"
#include "bme280_hal.h"
#include "radio.h"
#include "nrf_drv_saadc.h"


const nrf_drv_timer_t work_timer = NRF_DRV_TIMER_INSTANCE(0);

int saadc_measure() {
  nrf_saadc_value_t value;
  nrf_drv_saadc_sample_convert(0, &value);
  return value;
}


void deinit_timer(void){
    nrfx_timer_uninit(&work_timer);
}


void init_timer(void){
    uint32_t time_ticks;
    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    
    APP_ERROR_CHECK(nrf_drv_timer_init(&work_timer, &timer_cfg, timer_handler));
    time_ticks = nrf_drv_timer_ms_to_ticks(&work_timer, TIMER_MS);
    nrf_drv_timer_extended_compare(&work_timer, NRF_TIMER_CC_CHANNEL0, time_ticks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    nrf_drv_timer_enable(&work_timer);
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


void timer_handler(nrf_timer_event_t event_type, void* p_context){
    //struct bme280_data bme_data;
    struct tx_data bme_tx_data;

    int16_t adc_value;
    double d;

    switch (event_type){
        case NRF_TIMER_EVENT_COMPARE0:
            saadc_init();
            //spi_init();
            adc_value = saadc_measure();
            
            if(adc_value < 0) adc_value = 0;
            
            d = (3.3 * adc_value) / 1024.0;
            d = d * 3.47826 * 100.0;

            //bme_data = user_bme280_poll();
            // bme_tx_data.temperature = bme_data.temperature;
            // bme_tx_data.humidity = bme_data.humidity;
            // bme_tx_data.pressure = bme_data.pressure;
            // bme_tx_data.vbat = (uint32_t)d;
    
            bme_tx_data.humidity = 1023;
            bme_tx_data.pressure = 5400;
            bme_tx_data.temperature = 2345;
            bme_tx_data.vbat = 796;
            send_esb_packet(&bme_tx_data);
            nrf_drv_saadc_uninit();
            spi_uninit();
            break;

        default:
            //Do nothing.
            break;
    }
}

