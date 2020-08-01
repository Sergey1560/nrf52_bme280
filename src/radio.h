#ifndef RADIO_H
#define RADIO_H

#include "nrf.h"
#include "bsp.h"
#include "boards.h"
#include "app_error.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "nrf_esb.h"
#include "nrf_esb.h"
#include "nrf_error.h"
#include "nrf_esb_error_codes.h"

#include "bme280_hal.h"


struct tx_data{
    uint32_t pressure;
    int32_t temperature;
    uint32_t humidity;
    uint32_t vbat;
};


uint32_t esb_init( void );
void send_esb_packet(struct tx_data *data);
void nrf_esb_event_handler(nrf_esb_evt_t const * p_event);

#endif