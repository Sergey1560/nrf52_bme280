#include "saadc.h"


int saadc_measure(uint8_t chanel) {
  nrf_saadc_value_t value;

  nrf_drv_saadc_sample_convert(chanel, &value);

  return value;
}


void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event) {
	// Do nothing, as we use blocking mode
}

int saadc_init() {

  int ret = nrf_drv_saadc_init(NULL, saadc_event_handler);
  if (ret) return ret;
  
  nrf_saadc_channel_config_t config_0 = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN4);
  ret = nrf_drv_saadc_channel_init(0, &config_0);

  nrf_saadc_channel_config_t config_1 = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);
  ret = nrf_drv_saadc_channel_init(1, &config_1);

  return ret;
}
