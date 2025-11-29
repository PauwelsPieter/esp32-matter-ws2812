#include <stdio.h>

#include <esp_matter.h>

static const char *TAG = "app_main";
uint16_t ws2812_endpoint_id = 1;

esp_err_t app_driver_init()
{
    ESP_LOGI(TAG, "Initialising driver");

    ws2812_init();

    app_driver_attribute_set_defaults();
    return ESP_OK;
}

esp_err_t app_driver_attribute_update(
    uint16_t endpoint_id,
    uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val
) {
    esp_err_t err = ESP_OK;

    if (endpoint_id == ws2812_endpoint_id) {
        if (cluster_id == OnOff::Id) {
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                err = ws2812_set_power(val);
            }
        } else if (cluster_id == LevelControl::Id) {
            if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
                err = ws2812_set_brightness(val);
            }
        } else if (cluster_id == ColorControl::Id) {
            if (attribute_id == ColorControl::Attributes::CurrentHue::Id) {

            } else if (attribute_id == ColorControl::Attributes::CurrentSaturation::Id) {

            } else if (attribute_id == ColorControl::Attributes::ColorTemperature::Id) {

            }
        }
    }
    return err;
}

// Following function is used by Matter to notify the application of change
esp_err_t app_attribute_update_cb(
    callback_type_t type,
    uint16_t endpoint_id,
    uint32_t cluster_id,
    uint32_t attribute_id,
    esp_matter_attr_val_t *val,
    void *priv_data)
{
    esp_err_t err = ESP_OK;

    // If the callback type is PRE_UPDATE, the driver is updated first
    if (type == PRE_UPDATE)
    {
        /* Driver update */
        err = app_driver_attribute_update(endpoint_id, cluster_id, attribute_id, val);
    }

    return err;
}

void app_main(void)
{
    // Initialize Matter node + light endpoint
    node::config_t node_config;
    node_t *node = node::create(&node_config, app_attribute_update_cb, NULL);

    color_temperature_light::config_t light_config;
    light_config.on_off.on_off = DEFAULT_POWER;
    light_config.level_control.current_level = DEFAULT_BRIGHTNESS;
    endpoint_t *endpoint = color_temperature_light::create(node, &light_config, ENDPOINT_FLAG_NONE);
}
