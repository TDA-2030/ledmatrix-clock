idf_component_register(SRC_DIRS "."
                    INCLUDE_DIRS "."
                    )

set(IMAGE_DIR "../spiffs_image")
if(EXISTS ${IMAGE_DIR})
    spiffs_create_partition_image(storage ${IMAGE_DIR} FLASH_IN_PROJECT)
else()
    message(FATAL_ERROR "${IMAGE_DIR}/dist doesn't exit. ")
endif()