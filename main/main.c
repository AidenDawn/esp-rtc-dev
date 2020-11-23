/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "i2c_bus.h"
#include "ds3231.h"

#ifndef CONFIG_I2C_AUX_ENABLE
    #define CONFIG_I2C_AUX_ENABLE
    #define CONFIG_I2C_AUX_PORT_NUM                 (I2C_NUM_1)
    #define CONFIG_I2C_AUX_SDA_IO_PIN               21
    #define CONFIG_I2C_AUX_SCL_IO_PIN               22
    #define CONFIG_I2C_AUX_BUS_SPEED                400000
#endif

static i2c_dev_t ds3231_rtc;

void app_main(){
    printf("Hello world!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    //esp_restart();
    
    i2c_config_t i2c_aux_config = {
		.mode = I2C_MODE_MASTER,
		.sda_io_num = CONFIG_I2C_AUX_SDA_IO_PIN,
		.scl_io_num = CONFIG_I2C_AUX_SCL_IO_PIN,
		.sda_pullup_en = GPIO_PULLUP_ENABLE,
		.scl_pullup_en = GPIO_PULLUP_ENABLE,
		.master.clk_speed = CONFIG_I2C_AUX_BUS_SPEED,
	};
    i2c_port_t i2c_aux_port = (i2c_port_t) CONFIG_I2C_AUX_PORT_NUM;
    i2c_bus_handle_t auxBus = i2c_bus_create(i2c_aux_port, &i2c_aux_config);

    ds3231_rtc.port = i2c_aux_port;
    ds3231_rtc.addr = 0x68;

    ds3231_init();
    ds3231_disable_alarm_interrupts(&ds3231_rtc);
    ds3231_set_sqw_mode(&ds3231_rtc, one);
}