idf_component_register(SRCS "main.c" "leds/leds.c"
                    INCLUDE_DIRS "."
                    REQUIRES "led_strip" "led_animation"
                    )
