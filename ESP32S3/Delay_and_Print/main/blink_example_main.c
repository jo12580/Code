/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sys/unistd.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 6

static uint8_t s_led_state = 0;

#ifdef CONFIG_BLINK_LED_RMT
static led_strip_t *pStrip_a;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
            /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        pStrip_a->set_pixel(pStrip_a, 0, 16, 16, 16);
            /* Refresh the strip to send data */
        pStrip_a->refresh(pStrip_a, 100);
    } else {
            /* Set all LED off to clear all pixels */
        pStrip_a->clear(pStrip_a, 50);
    }
}

static void configure_led(void)
{
    // ESP_LOGI(TAG, "Example configured to blink addressable LED! 123");
        /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, BLINK_GPIO, 1);
    pStrip_a->clear(pStrip_a, 50); /* Set all LED off to clear all pixels */
}

#elif CONFIG_BLINK_LED_GPIO

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    // ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT); /* Set the GPIO as a push/pull output */
}

#endif

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    esp_chip_info_t chip_info;  //宏定义芯片信息结构体
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), WiFi%s%s, ",   //返回芯片信息
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
    printf("silicon revision %d, ", chip_info.revision);        //返回芯片版本

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),        //返回芯片使用内存
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    while (1)
    {
        ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        s_led_state = !s_led_state;/* Toggle the LED state */
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);   //延时函数
        // gpio_set_level(BLINK_GPIO, s_led_state);
        // vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        // gpio_set_level(BLINK_GPIO, 1);
    }
}
