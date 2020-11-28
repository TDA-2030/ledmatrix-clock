
CFLAGS += -DARM -DCONFIG_AUDIO_HELIX
COMPONENT_ADD_INCLUDEDIRS := helix/include .
COMPONENT_SRCDIRS:=helix .
./helix/subband.o ./helix/scalfact.o ./helix/dqchan.o ./helix/huffman.o: CFLAGS += -Wno-unused-but-set-variable

