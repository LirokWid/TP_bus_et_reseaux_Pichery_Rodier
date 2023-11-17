#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include "main.h"

#define BMP280_ADDRESS 		0x77

#define REG_ID				0xD0
#define REG_RESET			0xE0
#define REG_STATUS			0xF3
#define REG_CTRL_MEAS		0xF4
#define REG_CONFIG			0xF5

#define BMP280_I2C_ADDRESS_0  0x76
#define BMP280_I2C_ADDRESS_1  0x77

#define BMP280_CHIP_ID  0x58 /* BMP280 has chip-id 0x58 */
#define BME280_CHIP_ID  0x60 /* BME280 has chip-id 0x60 */


//Storing of the pressure
#define REG_PRESS_LSB		0xF7
#define REG_PRESS_MSB		0xF8
#define REG_PRESS_XLSB		0xF9

//Storing of the temperature
#define REG_TEMP_LSB		0xFA
#define REG_TEMP_MSB		0xFB
#define REG_TEMP_XLSB		0xFC

//OVERSAMPLING mode
#define	OVR_SMPL_12x
typedef enum {
    BMP280_MODE_SLEEP = 0,
    BMP280_MODE_FORCED = 1,
    BMP280_MODE_NORMAL = 3
} BMP280_Mode;

typedef enum {
    BMP280_FILTER_OFF = 0,
    BMP280_FILTER_2 = 1,
    BMP280_FILTER_4 = 2,
    BMP280_FILTER_8 = 3,
    BMP280_FILTER_16 = 4
} BMP280_Filter;

/**
 * Pressure oversampling settings
 */
typedef enum {
    BMP280_SKIPPED = 0,          /* no measurement  */
    BMP280_ULTRA_LOW_POWER = 1,  /* oversampling x1 */
    BMP280_LOW_POWER = 2,        /* oversampling x2 */
    BMP280_STANDARD = 3,         /* oversampling x4 */
    BMP280_HIGH_RES = 4,         /* oversampling x8 */
    BMP280_ULTRA_HIGH_RES = 5    /* oversampling x16 */
} BMP280_Oversampling;

/**
 * Stand by time between measurements in normal mode
 */
typedef enum {
    BMP280_STANDBY_05 = 0,      /* stand by time 0.5ms */
    BMP280_STANDBY_62 = 1,      /* stand by time 62.5ms */
    BMP280_STANDBY_125 = 2,     /* stand by time 125ms */
    BMP280_STANDBY_250 = 3,     /* stand by time 250ms */
    BMP280_STANDBY_500 = 4,     /* stand by time 500ms */
    BMP280_STANDBY_1000 = 5,    /* stand by time 1s */
    BMP280_STANDBY_2000 = 6,    /* stand by time 2s BMP280, 10ms BME280 */
    BMP280_STANDBY_4000 = 7,    /* stand by time 4s BMP280, 20ms BME280 */
} BMP280_StandbyTime;

/**
 * Configuration parameters for BMP280 module.
 * Use function bmp280_init_default_params to use default configuration.
 */
typedef struct {
    BMP280_Mode mode;
    BMP280_Filter filter;
    BMP280_Oversampling oversampling_pressure;
    BMP280_Oversampling oversampling_temperature;
    BMP280_Oversampling oversampling_humidity;
    BMP280_StandbyTime standby;
} bmp280_params_t;


typedef struct {
    uint16_t dig_T1;
    int16_t  dig_T2;
    int16_t  dig_T3;
    uint16_t dig_P1;
    int16_t  dig_P2;
    int16_t  dig_P3;
    int16_t  dig_P4;
    int16_t  dig_P5;
    int16_t  dig_P6;
    int16_t  dig_P7;
    int16_t  dig_P8;
    int16_t  dig_P9;

    /* Humidity compensation for BME280 */
    uint8_t  dig_H1;
    int16_t  dig_H2;
    uint8_t  dig_H3;
    int16_t  dig_H4;
    int16_t  dig_H5;
    int8_t   dig_H6;

    uint16_t addr;

    I2C_HandleTypeDef* i2c;

    bmp280_params_t params;

    uint8_t  id;        /* Chip ID */

} BMP280_HandleTypedef;
typedef struct reg_data_t{
	uint8_t reg_addr;
	uint8_t reg_val;
}reg_data_t;


int bmp280_init(BMP280_HandleTypedef *dev, bmp280_params_t *params);
void bmp280_init_default_params(bmp280_params_t *params);

int read_register16(BMP280_HandleTypedef *dev, uint8_t addr, uint16_t *value);
int write_register8(BMP280_HandleTypedef *dev, uint8_t addr, uint8_t value);

int read_data(BMP280_HandleTypedef *dev, uint8_t addr, uint8_t *value,uint8_t len);
int read_calibration_data(BMP280_HandleTypedef *dev);
int read_hum_calibration_data(BMP280_HandleTypedef *dev);


int read_register(I2C_HandleTypeDef *hi2c,uint8_t address,uint8_t reg, uint8_t *pBuf);
int write_register(I2C_HandleTypeDef *hi2c,uint8_t address,uint8_t reg, uint8_t value);

int read_calibration_data(BMP280_HandleTypedef *dev);

int bmp280_read_fixed(BMP280_HandleTypedef *dev, int32_t *temperature, uint32_t *pressure,uint32_t *humidity);
int bmp280_read_float(BMP280_HandleTypedef *dev, float *temperature, float *pressure,float *humidity);


int bmp280_read_float(BMP280_HandleTypedef *dev, float *temperature, float *pressure,float *humidity);
int bmp280_force_measurement(BMP280_HandleTypedef *dev);

int bmp280_is_measuring(BMP280_HandleTypedef *dev);
int32_t compensate_temperature(BMP280_HandleTypedef *dev, int32_t adc_temp,int32_t *fine_temp);
uint32_t compensate_pressure(BMP280_HandleTypedef *dev, int32_t adc_press,int32_t fine_temp);
uint32_t compensate_humidity(BMP280_HandleTypedef *dev, int32_t adc_hum,int32_t fine_temp);

#endif
