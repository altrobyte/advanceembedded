idf_component_register(SRCS "main.c" "siren.c"
                       INCLUDE_DIRS "."
                       REQUIRES alarm indicator emergency long_press_power mode_selector
                       )
