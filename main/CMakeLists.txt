idf_component_register(
    SRCS "ws2812.driver.c" "main.c"
    INCLUDE_DIRS "."
    REQUIRES led_strip esp_matter
)
