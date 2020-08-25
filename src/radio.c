#include "radio.h"

static nrf_esb_payload_t  tx_payload = {
.length = sizeof(struct tx_data),
.pipe = 0,
.noack = true
};


uint32_t esb_init( void ){
    uint32_t err_code;
    uint8_t base_addr_0[4] = {0xE7, 0xE7, 0xE7, 0xE7};
    uint8_t base_addr_1[4] = {0xC2, 0xC2, 0xC2, 0xC2};
    uint8_t addr_prefix[8] = {0xE7, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8 };

    nrf_esb_config_t nrf_esb_config         = NRF_ESB_DEFAULT_CONFIG;
    nrf_esb_config.protocol                 = NRF_ESB_PROTOCOL_ESB_DPL;
    nrf_esb_config.retransmit_delay         = 600;
    nrf_esb_config.bitrate                  = RADIO_MODE_MODE_Nrf_2Mbit;
    nrf_esb_config.event_handler            = nrf_esb_event_handler;
    nrf_esb_config.mode                     = NRF_ESB_MODE_PTX;
    nrf_esb_config.selective_auto_ack       = false;
    nrf_esb_config.crc                      = NRF_ESB_CRC_16BIT;
    nrf_esb_config.payload_length           = 32;
    nrf_esb_config.retransmit_count         = 0xF;
    //nrf_esb_config.tx_output_power          = RADIO_TXPOWER_TXPOWER_Pos4dBm;
    err_code = nrf_esb_init(&nrf_esb_config);

    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_0(base_addr_0);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_base_address_1(base_addr_1);
    VERIFY_SUCCESS(err_code);

    err_code = nrf_esb_set_prefixes(addr_prefix, NRF_ESB_PIPE_COUNT);
    VERIFY_SUCCESS(err_code);

    #ifdef PA_PRESENT

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_out_config_t out_config = NRFX_GPIOTE_CONFIG_OUT_SIMPLE(INIT_STATE);
    
    err_code = nrf_drv_gpiote_out_init(PA_RX_PIN, &out_config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_gpiote_out_init(PA_TX_PIN, &out_config);
    APP_ERROR_CHECK(err_code);

    #endif
    
    
    return err_code;
}


void nrf_esb_event_handler(nrf_esb_evt_t const * p_event){
    switch (p_event->evt_id) {
        case NRF_ESB_EVENT_TX_SUCCESS:
            NRF_LOG_DEBUG("TX SUCCESS EVENT");
            break;
        case NRF_ESB_EVENT_TX_FAILED:
            NRF_LOG_DEBUG("TX FAILED EVENT");
            (void) nrf_esb_flush_tx();
            (void) nrf_esb_start_tx();
            break;
        default:
            NRF_LOG_DEBUG("RX RECEIVED EVENT");
            (void) nrf_esb_flush_tx();
            (void) nrf_esb_start_tx();
            break;
    }
    set_pa_mode(PA_OFF_MODE);
    clocks_stop();
}

void set_pa_mode(uint8_t mode){

    switch (mode){
    case PA_TX_MODE:
        nrf_drv_gpiote_out_clear(PA_RX_PIN);
        nrf_drv_gpiote_out_set(PA_TX_PIN);
        break;
    case PA_RX_MODE:
        nrf_drv_gpiote_out_set(PA_RX_PIN);
        nrf_drv_gpiote_out_clear(PA_TX_PIN);
        break;
    case PA_OFF_MODE:
        nrf_drv_gpiote_out_clear(PA_RX_PIN);
        nrf_drv_gpiote_out_clear(PA_TX_PIN);
        break;
    
    default:
        break;
    }

}


void send_esb_packet(struct tx_data *data){
    set_pa_mode(PA_TX_MODE);
    memcpy(tx_payload.data, (uint32_t *)data, sizeof(struct tx_data));
    NRF_LOG_INFO("Transmitting packet");
    if (nrf_esb_write_payload(&tx_payload) != NRF_SUCCESS){
        NRF_LOG_WARNING("write packet failed");
    }
}