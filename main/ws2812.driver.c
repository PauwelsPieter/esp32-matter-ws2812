#include <esp_matter.h>
#include "led_strip.h"


#define LED_STRIP_LENGTH 30                     // Number of LEDs in the strip
#define LED_STRIP_PIN 33                        // GPIO pin used to control the strip
#define LED_STRIP_RMT_RES_HZ (10 * 1000 * 1000) // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)

led_strip_handle_t led_strip;

int rValue = 255;
int gValue = 255;
int bValue = 255;
int brightnessValue = 255;

void ws2812_init() {
    /* Initialize LED stirp */
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_STRIP_PIN,          // The GPIO that connected to the LED strip's data line
        .max_leds = LED_STRIP_LENGTH,             // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,            // LED strip model
        .flags.invert_out = false,                // whether to invert the output signal
    };
    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
        .flags.with_dma = false,               // DMA feature is available on ESP target like ESP32-S3
    };
    led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip);
    led_strip_clear(led_strip);
}

static esp_err_t ws2812_set_power(
    led_indicator_handle_t handle,
    esp_matter_attr_val_t *val
) {
    esp_err_t err = ESP_OK;

    if (val->val.b) {
        for (int i = 0; i < LED_STRIP_LENGTH; i++)
        {
            int rAdjValue = (rValue * brightnessValue) / 255;
            int gAdjValue = (gValue * brightnessValue) / 255;
            int bAdjValue = (bValue * brightnessValue) / 255;
            led_strip_set_pixel(led_strip, i, rAdjValue, gAdjValue, bAdjValue);
        }
        err = led_strip_refresh(led_strip);
    } else {
        err = led_strip_clear(led_strip);
    }

    return err;
}

static esp_err_t ws2812_set_brightness(
    led_indicator_handle_t handle,
    esp_matter_attr_val_t *val
) {
    esp_err_t err = ESP_OK;

    brightnessValue = val->val.u8
    for (int i = 0; i < LED_STRIP_LENGTH; i++)
    {
        int rAdjValue = (rValue * val->val.u8) / 255;
        int gAdjValue = (gValue * val->val.u8) / 255;
        int bAdjValue = (bValue * val->val.u8) / 255;
        led_strip_set_pixel(led_strip, i, rAdjValue, gAdjValue, bAdjValue);
    }
    err = led_strip_refresh(led_strip);

    return err;
}