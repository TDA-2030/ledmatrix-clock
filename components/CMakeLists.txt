set(srcs "src/led_matrix.c"
        "src/net_config.c"
        "src/nvs_storage.c"
        "src/rtc_time.c"
        "src/sht20.c"
        "src/weather.c"
        "src/app_sntp.c")

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS include
                    REQUIRES driver nvs_flash esp-tls)