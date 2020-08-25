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
#include "sys_config.h"

#define PA_PRESENT

#ifdef PA_PRESENT
#include "nrf_drv_gpiote.h"

#define PA_TX_MODE 0
#define PA_RX_MODE 1
#define PA_OFF_MODE 2

#define INIT_STATE  0

#define PA_RX_PIN   20
#define PA_TX_PIN   24

#endif

struct tx_data{
    uint32_t pressure;
    int32_t temperature;
    uint32_t humidity;
    uint32_t vbat;
};


uint32_t esb_init( void );
void send_esb_packet(struct tx_data *data);
void nrf_esb_event_handler(nrf_esb_evt_t const * p_event);
void set_pa_mode(uint8_t mode);

#endif