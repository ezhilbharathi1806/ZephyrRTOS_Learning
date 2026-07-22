#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(bme280_readings, LOG_LEVEL_INF);

static const struct i2c_dt_spec dev = I2C_DT_SPEC_GET(DT_NODELABEL(bme280));

/* Calibration coefficients (read once at startup) */
static uint16_t dig_T1, dig_P1;
static int16_t  dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static uint8_t  dig_H1;
static int16_t  dig_H2;
static uint8_t  dig_H3;
static int16_t  dig_H4, dig_H5;
static int8_t   dig_H6;

/* Read 16-bit value from two registers (MSB, LSB) */
static int read_u16(uint8_t reg, uint16_t *val)
{
    uint8_t buf[2];
    int ret = i2c_write_read_dt(&dev, &reg, 1, buf, 2);
    if (ret) return ret;
    *val = (buf[0] << 8) | buf[1];
    return 0;
}

static int read_s16(uint8_t reg, int16_t *val)
{
    uint16_t u;
    int ret = read_u16(reg, &u);
    if (ret) return ret;
    *val = (int16_t)u;
    return 0;
}

static int bme280_read_calibration(void)
{
    if (read_u16(0x88, &dig_T1)) return -1;
    if (read_s16(0x8A, &dig_T2)) return -1;
    if (read_s16(0x8C, &dig_T3)) return -1;

    if (read_u16(0x8E, &dig_P1)) return -1;
    if (read_s16(0x90, &dig_P2)) return -1;
    if (read_s16(0x92, &dig_P3)) return -1;
    if (read_s16(0x94, &dig_P4)) return -1;
    if (read_s16(0x96, &dig_P5)) return -1;
    if (read_s16(0x98, &dig_P6)) return -1;
    if (read_s16(0x9A, &dig_P7)) return -1;
    if (read_s16(0x9C, &dig_P8)) return -1;
    if (read_s16(0x9E, &dig_P9)) return -1;

    uint8_t h1;
    if (i2c_reg_read_byte_dt(&dev, 0xA1, &h1)) return -1;
    dig_H1 = h1;

    if (read_s16(0xE1, &dig_H2)) return -1;

    uint8_t h3;
    if (i2c_reg_read_byte_dt(&dev, 0xE3, &h3)) return -1;
    dig_H3 = h3;

    uint8_t buf[3];
    if (i2c_write_read_dt(&dev, (uint8_t[]){0xE4}, 1, buf, 3)) return -1;
    dig_H4 = (int16_t)((buf[0] << 4) | (buf[1] & 0x0F));
    dig_H5 = (int16_t)((buf[2] << 4) | (buf[1] >> 4));

    int8_t h6;
    if (i2c_reg_read_byte_dt(&dev, 0xE7, (uint8_t*)&h6)) return -1;
    dig_H6 = h6;

    return 0;
}

/* Very simple forced mode + read raw values */
static int bme280_start_measurement(void)
{
    /* ctrl_hum = 001 (×1 oversampling) */
    i2c_reg_write_byte_dt(&dev, 0xF2, 0x01);

    /* ctrl_meas = temp×1, press×1, forced mode */
    return i2c_reg_write_byte_dt(&dev, 0xF4, 0b00100101);   // osrs_t=1, osrs_p=1, mode=01
}

static int bme280_read_raw(int32_t *temp, int32_t *press, int32_t *hum)
{
    uint8_t data[8];
    uint8_t reg = 0xF7;     // pressure MSB

    int ret = i2c_write_read_dt(&dev, &reg, 1, data, 8);
    if (ret) return ret;

    *press = (int32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));
    *temp  = (int32_t)((data[3] << 12) | (data[4] << 4) | (data[5] >> 4));
    *hum   = (int32_t)((data[6] << 8)  |  data[7]);

    return 0;
}

/* Basic temperature compensation (from Bosch datasheet) */
static float compensate_temperature(int32_t adc_T)
{
    int32_t var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    int32_t var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) *
                    ((int32_t)dig_T3)) >> 14;
    int32_t t_fine = var1 + var2;
    return (t_fine * 5 + 128) >> 8;   // t_fine / 512 → °C × 100
}

/* Very simplified pressure (needs t_fine from temp) – approximate */
static float compensate_pressure(int32_t adc_P, int32_t t_fine)
{
    int64_t var1 = (int64_t)t_fine - 128000;
    int64_t var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((int64_t)1 << 47) - var1) * ((int64_t)dig_P1) >> 33;

    if (var1 == 0) return 0;

    int64_t p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9 * (p >> 13) * (p >> 13)) >> 25);
    var2 = (((int64_t)dig_P8 * p) >> 19);

    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (float)p / 25600.0f;   // hPa
}

/* Very basic humidity compensation */
static float compensate_humidity(int32_t adc_H, int32_t t_fine)
{
    int32_t v_x1_u32r = t_fine - ((int32_t)76800);
    int32_t v_x1 = (adc_H << 14) - (((int32_t)dig_H4) << 20) - (((int32_t)dig_H5) * v_x1_u32r);
    v_x1 = (v_x1 + ((int32_t)16384)) >> 15;
    v_x1 *= ((((((v_x1_u32r * (int32_t)dig_H6) >> 10) * (((v_x1_u32r * (int32_t)dig_H3) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * (int32_t)dig_H2 + 8192) >> 14;
    v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * (int32_t)dig_H1) >> 4));
    v_x1 = (v_x1 < 0) ? 0 : v_x1;
    v_x1 = (v_x1 > 419430400) ? 419430400 : v_x1;
    return (float)(v_x1 >> 12) / 1024.0f;   // %RH
}


int main(void){

    if (!device_is_ready(dev.bus)) {
        LOG_ERR("I2C bus device not ready");
        return 0;
    }

    if (bme280_read_calibration()) {
        LOG_ERR("Failed to read calibration data");
        return 0;
    }

    LOG_INF("BME280 is ready - reading ever 2 seconds");

    while(1) {
        if(bme280_start_measurement()) {
            LOG_ERR("Failed to start measurement");
            k_sleep(K_SECONDS(2));
            continue;
        }

        k_sleep(K_MSEC(100));   // wait for measurement to complete

        int32_t adc_T, adc_P, adc_H;
        if (bme280_read_raw(&adc_T, &adc_P, &adc_H)) {
            LOG_ERR("Failed to read raw data");
            k_sleep(K_SECONDS(2));
            continue;
        }
        
        float temp  = compensate_temperature(adc_T);
        float press = compensate_pressure(adc_P, (int32_t)(temp * 512.0f));  // rough t_fine
        float hum   = compensate_humidity(adc_H, (int32_t)(temp * 512.0f));

        LOG_INF("T: %.2f °C   P: %.2f hPa   H: %.2f %%", temp/100.0f, press, hum);

        k_sleep(K_SECONDS(2));
    }


    return 0;
}