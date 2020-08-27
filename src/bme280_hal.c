#include "bme280_hal.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */

static uint8_t m_tx_buf[64]; /**< TX buffer. */
static uint8_t m_rx_buf[64];    /**< RX buffer. */

struct bme280_dev dev;
uint8_t dev_addr=0;

int8_t def_stream_sensor_data_forced_mode(struct bme280_dev *dev, struct bme280_data *comp_data){
    int8_t rslt;
    uint8_t settings_sel;
	uint32_t req_delay;
    
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_1X;
    dev->settings.osr_t = BME280_OVERSAMPLING_1X;
    dev->settings.filter = BME280_FILTER_COEFF_OFF;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    rslt = bme280_set_sensor_settings(settings_sel, dev);
	
    req_delay = bme280_cal_meas_delay(&dev->settings);
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    dev->delay_us(req_delay, dev->intf_ptr);
    nrf_delay_ms(40);
    rslt = bme280_get_sensor_data(BME280_ALL, comp_data, dev);
    return rslt;
}

void spi_uninit(void){
    nrf_drv_spi_uninit(&spi);
    NRF_LOG_INFO("SPI stoped.");
}

void spi_init(void){
    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = SPI_SS_PIN;
    spi_config.miso_pin = SPI_MISO_PIN;
    spi_config.mosi_pin = SPI_MOSI_PIN;
    spi_config.sck_pin  = SPI_SCK_PIN;
    spi_config.frequency = SPI_FREQUENCY_FREQUENCY_M2;

    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, NULL, NULL));
    NRF_LOG_INFO("SPI started.");
}

void user_bme280_init(void){
	dev.intf_ptr = &dev_addr;
	dev.intf = BME280_SPI_INTF;
	dev.read = user_spi_read;
	dev.write = user_spi_write;
	dev.delay_us = user_delay_ms;
	bme280_init(&dev);
}

struct bme280_data user_bme280_poll(void){
    struct bme280_data comp_data;
    def_stream_sensor_data_forced_mode(&dev,&comp_data);
    return comp_data;
}


void user_delay_ms(uint32_t period, void *intf_ptr){
    nrf_delay_ms(period);
}

int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr){
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter intf_ptr can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |----------------+---------------------+-------------|
     * | MOSI           | MISO                | Chip Select |
     * |----------------+---------------------|-------------|
     * | (don't care)   | (don't care)        | HIGH        |
     * | (reg_addr)     | (don't care)        | LOW         |
     * | (don't care)   | (reg_data[0])       | LOW         |
     * | (....)         | (....)              | LOW         |
     * | (don't care)   | (reg_data[len - 1]) | LOW         |
     * | (don't care)   | (don't care)        | HIGH        |
     * |----------------+---------------------|-------------|
     */
    //NRF_LOG_INFO("SPI Read reg: %d len: %d",reg_addr,len);
    memset(m_tx_buf, 0xFF, len+1); // +1 reg addr
    m_tx_buf[0]= reg_addr;

    // NRF_LOG_INFO("TX:");
    // NRF_LOG_HEXDUMP_INFO(m_tx_buf, len+1);
    nrf_drv_spi_transfer(&spi, m_tx_buf, len+1, m_rx_buf, len+1);

    memcpy(reg_data, &m_rx_buf[1], len);

    // NRF_LOG_INFO("TX:");
    // NRF_LOG_HEXDUMP_INFO(m_rx_buf, strlen((const char *)m_rx_buf));


    return rslt;
}

int8_t user_spi_write(uint8_t reg_addr,const uint8_t *reg_data, uint32_t len, void *intf_ptr){
    int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter intf_ptr can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |---------------------+--------------+-------------|
     * | MOSI                | MISO         | Chip Select |
     * |---------------------+--------------|-------------|
     * | (don't care)        | (don't care) | HIGH        |
     * | (reg_addr)          | (don't care) | LOW         |
     * | (reg_data[0])       | (don't care) | LOW         |
     * | (....)              | (....)       | LOW         |
     * | (reg_data[len - 1]) | (don't care) | LOW         |
     * | (don't care)        | (don't care) | HIGH        |
     * |---------------------+--------------|-------------|
     */

    //NRF_LOG_INFO("SPI WRITE reg: %d len: %d",reg_addr,len);


    memset(m_tx_buf, 0xFF, len+1); // +1 reg addr
    m_tx_buf[0]= reg_addr;

    // NRF_LOG_INFO("TX:");
    // NRF_LOG_HEXDUMP_INFO(m_tx_buf, len+1);

    memcpy(&m_tx_buf[1], reg_data, len);
    nrf_drv_spi_transfer(&spi, m_tx_buf, len+1, m_rx_buf, len+1);

    return rslt;
}


