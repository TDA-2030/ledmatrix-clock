idf_component_register(SRC_DIRS "." "cgi" "adapt"
                        INCLUDE_DIRS .
                        PRIV_REQUIRES wpa_supplicant dns_server esp_http_server esp_timer esp_wifi nvs_flash
                        EMBED_FILES "html/cfg_favicon.ico"
                                    "html/medley.min.js"
                                    "html/connecting.html"
                                    "html/style.css"
                                    "html/wifi.png"
                                    "html/wifi.html")
