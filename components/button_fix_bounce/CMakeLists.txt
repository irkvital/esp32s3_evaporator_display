set(reqs
    "esp_timer"
)
idf_component_register(REQUIRES driver
SRCS "button_fix_bounce.c"
                    INCLUDE_DIRS "include"
                    REQUIRES ${reqs})
