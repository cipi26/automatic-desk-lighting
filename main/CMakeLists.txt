idf_component_register(SRCS "main.c" "leds/leds.c" "leds/leds_animations.c"
                    INCLUDE_DIRS "."
                    REQUIRES "led_strip" "led_animation"
                    )
