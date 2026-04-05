idf_component_register(SRCS "main.c" "leds/leds.c" "leds/leds_animations.c" "controller/controller.c" "buttons/buttons.c"
                    INCLUDE_DIRS "."
                    REQUIRES "led_strip" "led_animation" "esp_driver_gpio"
                    )
