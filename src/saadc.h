#ifndef SAADC_H
#define SAADC_H
#include "nrf_drv_saadc.h"


int saadc_measure(uint8_t chanel);
void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event);
int saadc_init(void);


#endif