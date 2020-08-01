#ifndef BME280_HAL_H
#define BME280_HAL_H

#include "bme280.h"
#include "bme280_defs.h"


#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#define SPI_SCK_PIN 18
#define SPI_MOSI_PIN 16
#define SPI_MISO_PIN 15
#define SPI_SS_PIN 17

void spi_init(void);
void spi_uninit(void);
void user_delay_ms(uint32_t period, void *intf_ptr);
int8_t user_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr);
int8_t user_spi_write(uint8_t reg_addr,const uint8_t *reg_data, uint32_t len, void *intf_ptr);

void user_bme280_init(void);
struct bme280_data user_bme280_poll(void);
void print_sensor_data(struct bme280_data *comp_data);

#endif