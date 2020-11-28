idf_component_register(SRCS "bt_app_av.c"
                            "bt_app_core.c"
                            "app_bt_audio.c"
                    INCLUDE_DIRS "include"
                    REQUIRES bt pwm_audio)

                    