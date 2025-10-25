#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "driver/usb_serial_jtag.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_mac.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"

void blink(void *arg) {
    // install gpio
    gpio_config_t init_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1 << 15,
    };
    ESP_ERROR_CHECK(gpio_config(&init_gpio_config));

    // local vars
    uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
    bool led = false;
    
    while (1)
    {        
        if ((xTaskGetTickCount() * portTICK_PERIOD_MS) - current_time > 1000) {
            current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
            gpio_set_level(15, led);
            led = !led;
        } 
        vTaskDelay(10);
    }
}

void print(void *arg) {
    // install serial jtag
    usb_serial_jtag_driver_config_t init_usb_serial_jtag_config = {
        .rx_buffer_size = 256,
        .tx_buffer_size = 256,
    };
    ESP_ERROR_CHECK(usb_serial_jtag_driver_install(&init_usb_serial_jtag_config));

    // local vars
    uint32_t current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;

    while (1)
    {        
        if ((xTaskGetTickCount() * portTICK_PERIOD_MS) - current_time > 1000) {
            current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
            usb_serial_jtag_write_bytes("scream!\r\n", 10, 0);
        } 
        vTaskDelay(10);
    }
}

void app_main(void)
{
    // disable twdt
    ESP_ERROR_CHECK(esp_task_wdt_deinit());

    // run tasks
    xTaskCreate(print, "print", 512, NULL, 10, NULL);
    xTaskCreate(blink, "blink", 512, NULL, 11, NULL);
}
