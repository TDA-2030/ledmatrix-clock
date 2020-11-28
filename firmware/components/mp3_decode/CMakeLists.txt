set(COMPONENT_ADD_INCLUDEDIRS "helix/include" ".")
set(COMPONENT_SRCDIRS "helix" ".")
set(COMPONENT_REQUIRES pwm_audio)

register_component()
add_definitions(-DARM)
set_source_files_properties(
        ./helix/subband.c ./helix/scalfact.c ./helix/dqchan.c ./helix/huffman.c
        PROPERTIES COMPILE_FLAGS
        -Wno-unused-but-set-variable)
