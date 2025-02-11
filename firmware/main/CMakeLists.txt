idf_component_register(SRC_DIRS . calendar ui
                    INCLUDE_DIRS .
                    EMBED_FILES SourceHanSansSC-Medium-Min.ttf)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LV_LVGL_H_INCLUDE_SIMPLE)


set(IMAGE_DIR ../rootfs)
if(EXISTS ${IMAGE_DIR})
    fatfs_create_spiflash_image(storage ${IMAGE_DIR} FLASH_IN_PROJECT PRESERVE_TIME)
else()
    message(FATAL_ERROR "${IMAGE_DIR} doesn't exit. ")
endif()
