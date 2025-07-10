
################################################################################
## set flags for golobal compile and link setting.
################################################################################

CONFIG_FOR_COMPILE = $(CFLAGS) $(LOCAL_CFLAGS)
CONFIG_FOR_LINK = -Wl,--no-undefined


################################################################################

BuildPath      = ./build
ObjectPath     = $(BuildPath)/obj
OutputPath     = ./
DependFilePath = $(BuildPath)/dep
Target         = $(OutputPath)/libtplayer.so   ## output target.

ifneq ($(BuildPath),wildcard($(BuildPath)))
a := $(shell mkdir -p $(BuildPath))
endif
ifneq ($(ObjectPath),wildcard($(ObjectPath)))
a := $(shell mkdir -p $(ObjectPath))
endif
ifneq ($(OutputPath),wildcard($(OutputPath)))
a := $(shell mkdir -p $(OutputPath))
endif
ifneq ($(DependFilePath),wildcard($(DependFilePath)))
a := $(shell mkdir -p $(DependFilePath))
endif

CEDARC_LIBS_PATH = ../libcedarc/library/toolchain-sunxi-aarch64-glibc-gcc-1020

CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-glibc-gcc-1020
#CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-glibc-gcc-1020

ifeq ($(LICHEE_LINUX_DEV), "openwrt")
include $(TOPDIR)/package/subpackage/allwinner/multimedia/tina_multimedia/machinfo/$(LICHEE_IC)/config.mk
else ifeq ($(TINA_PACKAGE_PATH_PREFIX), buildroot)
include $(LICHEE_TOP_DIR)/buildroot/package/allwinner/multimedia/tina_multimedia/machinfo/$(LICHEE_IC)/config.mk
else
include $(LICHEE_TOP_DIR)/openwrt/package/allwinner/multimedia/tina_multimedia/machinfo/$(LICHEE_IC)/config.mk
endif
################################################################################
## set the source files, object files and dependency files
################################################################################
## set the source path to VPATH.
SourcePath = $(shell find ./ -type d)
SourcePath := $(filter-out $(BuildPath) $(ObjectPath) $(DependFilePath), $(SourcePath))
VPATH := $(SourcePath)

## set the source files.
ifeq ($(ONLY_ENABLE_AUDIO),y)
SourceFiles  = ./tplayer.c ./awsink/tsound_ctrl.c
else
ifeq ($(ONLY_DISABLE_AUDIO),y)
SourceFiles	 = $(filter-out ./awsink/tsound_ctrl.c, $(foreach dir,$(SourcePath),$(shell find $(dir) -maxdepth 1 -name "*.c")))
else
SourceFiles  = $(foreach dir,$(SourcePath),$(shell find $(dir) -maxdepth 1 -name "*.c"))
endif

endif

## set the object files.
ObjectFiles = $(addprefix $(ObjectPath)/, $(addsuffix .o ,$(basename $(notdir $(SourceFiles)))))

## set the dependency files.
DependFiles = $(addprefix $(DependFilePath)/, $(addsuffix .d ,$(notdir $(basename $(SourceFiles)))))


################################################################################
## set flags for compile and link
################################################################################

## set the include path for compile flags.
ifeq ($(ONLY_ENABLE_AUDIO),y)
SourceIncludePath = $(foreach dir,$(SourcePath),-I$(dir)) \
				-I ../ \
			        -I../libcedarx/ \
			        -I../libcedarx/external/include/adecoder \
			        -I../libcedarx/libcore/playback/include \
			        -I../libcedarx/libcore/common/iniparser\
			        -I../libcedarx/libcore/parser/include/ \
			        -I../libcedarx/libcore/stream/include/ \
			        -I../libcedarx/libcore/base/include/ \
			        -I../libcedarx/xplayer/include \
					-I.
else
SourceIncludePath = $(foreach dir,$(SourcePath),-I$(dir)) \
				-I ../ \
				-I../$(TPLAYER_LIBCEDARC_DIR)/include \
			        -I../$(TPLAYER_LIBCEDARC_DIR)/vdecoder/include \
			        -I../libcedarx/ \
			        -I../libcedarx/external/include/adecoder \
			        -I../libcedarx/external/include/sdecoder \
			        -I../libcedarx/libcore/playback/include \
			        -I../libcedarx/libcore/common/iniparser\
			        -I../libcedarx/libcore/parser/include/ \
			        -I../libcedarx/libcore/stream/include/ \
			        -I../libcedarx/libcore/base/include/ \
			        -I../libcedarx/xplayer/include \
					-I.
endif

#config the arm libs path
ifeq ($(CPU_TYPE),cortex-a7)
    ifeq ($(C_LIB_TYPE),musl)
        CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm-musl
        CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm-musl
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm-glibc
        CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm-glibc
    endif
endif

#config the aarch64 libs path
ifeq ($(CPU_TYPE),cortex-a53)
    ifeq ($(KERNEL64_USER32),y)
        ifeq ($(C_LIB_TYPE),musl)
            CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm-musl
            CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm-musl
        endif
        ifeq ($(C_LIB_TYPE),glibc)
            CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm-glibc
            CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm-glibc
        endif
    else
        ifeq ($(C_LIB_TYPE),musl)
            CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-aarch64-musl
            CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-musl
            ifeq ($(CONFIG_EXT_TOOLCHAIN_GCC_8_3_0),y)
                CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-aarch64-musl-gcc-830
                CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-musl-gcc-830
            endif
        endif
        ifeq ($(C_LIB_TYPE),glibc)
            CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-aarch64-glibc
            CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-glibc
            ifeq ($(CONFIG_EXT_TOOLCHAIN_GCC_8_3_0),y)
                #CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-aarch64-glibc-gcc-830
                CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-aarch64-glibc-gcc-1020
                CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-aarch64-glibc-gcc-1020
            endif
        endif
    endif
endif

#config the arm9 libs path
ifeq ($(CPU_TYPE),arm926ej-s)
    ifeq ($(C_LIB_TYPE),musl)
	CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm9-musl
        CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm926-musl
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-arm9-glibc
        CEDARX_LIBS_PATH = ../libcedarx/external/lib32/openwrt-arm926-glibc
    endif
endif

#config the riscv libs path
ifeq ($(CPU_TYPE),c910)
    ifeq ($(C_LIB_TYPE),glibc)
        CEDARC_LIBS_PATH = ../$(TPLAYER_LIBCEDARC_DIR)/library/toolchain-sunxi-riscv-glibc
        CEDARX_LIBS_PATH = ../libcedarx/external/lib64/openwrt-riscv-glibc
    endif
endif

#config the aarch64 libs path
ifeq ($(CONFIG_ARCH),aarch64)
	ifeq ($(C_LIB_TYPE),glibc)
		ifeq ($(CONFIG_TOOLCHAIN_GCC_VERSION),11.3.0)
			CEDARC_LIBS_PATH = libcedarc/library/toolchain-sunxi-aarch64-glibc-gcc-1130
			CEDARX_LIBS_PATH = libcedarx/external/lib64/openwrt-aarch64-glibc-gcc-1130
		endif
		ifeq ($(CONFIG_TOOLCHAIN_GCC_VERSION),10.2.0)
			CEDARC_LIBS_PATH = ../libcedarc/library/toolchain-sunxi-aarch64-glibc-gcc-1020
			CEDARX_LIBS_PATH = ../libcedarx/external/lib64/openwrt-aarch64-glibc-gcc-1020
		endif

                ifeq ($(CONFIG_TOOLCHAIN_GCC_VERSION),8.4.0)
			CEDARC_LIBS_PATH = ../libcedarc/library/toolchain-sunxi-aarch64-glibc-gcc-830
			CEDARX_LIBS_PATH = ../libcedarx/external/lib64/openwrt-aarch64-glibc-gcc-830
		endif

	endif
endif

# setting for buldroot.
ifeq ($(LICHEE_CROSS_COMPILER),arm-linux-gnueabi)
		CEDARC_LIBS_PATH = ../../libcedarc/library/arm-linux-gnueabi
endif

ifeq ($(MEDIA_TARGET_PLATFORM), buildroot)
    ifeq ($(LICHEE_CROSS_COMPILER),aarch64-none-linux-gnu)
        CEDARC_LIBS_PATH = ../../libcedarc/library/aarch64-none-linux-gnu
        CEDARX_LIBS_PATH = ../libcedarx/external/lib32/aarch64-none-linux-gnu
    endif
endif


## set compile flags
CompileFlags = $(CONFIG_FOR_COMPILE) $(SourceIncludePath) -Wall -O2 -ldl -DVE_OFFSET_ADDR=$(VE_OFFSET)
ifeq ($(ONLY_ENABLE_AUDIO),y)
LocalLibPath = -L../libcedarx/xplayer/.libs/ -L../libcedarx/libcore/base/.libs/ -L../libcedarx/libcore/parser/base/.libs/\
			-L../libcedarx/libcore/playback/.libs/ -L../libcedarx/libcore/stream/base/.libs/ -L../libcedarx/libcore/common/.libs/ \
			-L$(CEDARX_LIBS_PATH)
else
LocalLibPath = -L../libcedarx/xplayer/.libs/ -L../libcedarx/libcore/base/.libs/ -L../libcedarx/libcore/parser/base/.libs/\
			-L../libcedarx/libcore/playback/.libs/ -L../libcedarx/libcore/stream/base/.libs/ -L../libcedarx/libcore/common/.libs/ \
			-L$(CEDARX_LIBS_PATH) \
			-L../$(TPLAYER_LIBCEDARC_DIR)/vdecoder/.libs/ -L../$(TPLAYER_LIBCEDARC_DIR)/memory/.libs/ -L../$(TPLAYER_LIBCEDARC_DIR)/base/.libs/ -L$(CEDARC_LIBS_PATH)
endif

ifeq ($(ONLY_ENABLE_AUDIO),y)
LocalLinkFlags = -lxplayer -ladecoder \
               -lcdx_parser  -lcdx_playback -lcdx_stream -lcdx_base \
              -lpthread -ldl  -lrt -lm -lc -lz -lasound
else

ifeq ($(TPLAYER_LIBCEDARC_DIR), mpp_libcedarc)
LocalLinkFlags =  -lxplayer -lvdecoder -lcdc_base -lVE -lMemAdapter \
               -lcdx_parser  -lcdx_playback -lcdx_stream -lcdx_base \
              -lpthread -ldl -luapi -lrt -lm -lc -lz -lasound
else

#LocalLinkFlags =  -lxplayer -lvdecoder -lsubdecoder -lcdc_base -lVE -lMemAdapter \

LocalLinkFlags =  -lxplayer -lvdecoder -lcdc_base -lVE -lMemAdapter \
               -lcdx_parser  -lcdx_playback -lcdx_stream -lcdx_base \
              -lpthread -ldl -luapi -lrt -lm -lc -lz -lasound

endif
ifneq ($(ONLY_DISABLE_AUDIO),y)
LocalLinkFlags += -lasound -ladecoder
endif
endif

ifeq ($(ONLY_ENABLE_AUDIO),y)
CompileFlags += -DONLY_ENABLE_AUDIO
endif

ifeq ($(ONLY_DISABLE_AUDIO), y)
CompileFlags += -DONLY_DISABLE_AUDIO
endif
ifneq ($(ONLY_DISABLE_AUDIO),y)
##set link flag path
ifeq ($(AUDIO_GAIN_TOOLCHAIN_OF_DIFFERENT_KERNEL_BIT),y)
    ifeq ($(C_LIB_TYPE),musl)
        ifeq ($(KERNEL64_USER32),y)
            LocalAudioGain += ./arm-musl/libAudioGain.a
           LocalAudioPostProc += ./arm-musl/libAudioPostProc.a
            LocalAudioSurround += ./arm-musl/libAudioSurround.a
        else
            LocalAudioGain += ./aarch64-musl/libAudioGain.a
           LocalAudioPostProc += ./aarch64-musl/libAudioPostProc.a
            LocalAudioSurround += ./aarch64-musl/libAudioSurround.a
            ifeq ($(CONFIG_EXT_TOOLCHAIN_GCC_8_3_0),y)
                LocalAudioGain += ./aarch64-musl-gcc-830/libAudioGain.a
               LocalAudioPostProc += ./aarch64-musl-gcc-830/libAudioPostProc.a
                LocalAudioSurround += ./aarch64-musl-gcc-830/libAudioSurround.a
            endif
        endif
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        ifeq ($(KERNEL64_USER32),y)
            LocalAudioGain += ./arm-glibc/libAudioGain.a
           LocalAudioPostProc += ./arm-glibc/libAudioPostProc.a
            LocalAudioSurround += ./arm-glibc/libAudioSurround.a
        else
            LocalAudioGain += ./aarch64-glibc/libAudioGain.a
           LocalAudioPostProc += ./aarch64-glibc/libAudioPostProc.a
            LocalAudioSurround += ./aarch64-glibc/libAudioSurround.a

            ifeq ($(CONFIG_EXT_TOOLCHAIN_GCC_8_3_0),y)
                LocalAudioGain += ./aarch64-glibc-gcc-830/libAudioGain.a
               LocalAudioPostProc += ./aarch64-glibc-gcc-830/libAudioPostProc.a
                LocalAudioSurround += ./aarch64-glibc-gcc-830/libAudioSurround.a
            endif
        endif
    endif
endif

ifeq ($(AUDIO_GAIN_TOOLCHAIN_OF_ARM),y)
    ifeq ($(C_LIB_TYPE),musl)
        LocalAudioGain += ./arm-musl/libAudioGain.a
       LocalAudioPostProc += ./arm-musl/libAudioPostProc.a
        LocalAudioSurround += ./arm-musl/libAudioSurround.a
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        LocalAudioGain += ./arm-glibc/libAudioGain.a
       LocalAudioPostProc += ./arm-glibc/libAudioPostProc.a
        LocalAudioSurround += ./arm-glibc/libAudioSurround.a
    endif
	ifeq ($(LICHEE_CROSS_COMPILER),arm-linux-gnueabi)
		LocalAudioPostProc += ./arm-linux-gnueabi/libAudioPostProc.a
		LocalAudioGain += ./arm-linux-gnueabi/libAudioGain.a
	endif

endif

ifeq ($(AUDIO_GAIN_TOOLCHAIN_OF_ARM9),y)
    ifeq ($(C_LIB_TYPE),musl)
        LocalAudioGain += ./arm9-musl/libAudioGain.a
       LocalAudioPostProc += ./arm9-musl/libAudioPostProc.a
        LocalAudioSurround += ./arm9-musl/libAudioSurround.a
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        LocalAudioGain += ./arm9-glibc/libAudioGain.a
       LocalAudioPostProc += ./arm9-glibc/libAudioPostProc.a
        LocalAudioSurround += ./arm9-glibc/libAudioSurround.a
    endif
endif

ifeq ($(AUDIO_GAIN_TOOLCHAIN_TYPE_OF_RISCV),y)
    ifeq ($(C_LIB_TYPE),glibc)
        LocalAudioGain += ./riscv-glibc/libAudioGain.a
       LocalAudioPostProc += ./riscv-glibc/libAudioPostProc.a
        LocalAudioSurround += ./riscv-glibc/libAudioSurround.a
    endif
endif

ifeq ($(AUDIO_GAIN_TOOLCHAIN_OF_AARCH64),y)
    ifeq ($(C_LIB_TYPE),musl)
        LocalAudioGain += ./arm-musl/libAudioGain.a
       LocalAudioPostProc += ./arm-musl/libAudioPostProc.a
        LocalAudioSurround += ./arm-musl/libAudioSurround.a
    endif
    ifeq ($(C_LIB_TYPE),glibc)
        LocalAudioGain += ./aarch64-glibc-gcc-830/libAudioGain.a
       LocalAudioPostProc += ./aarch64-glibc-gcc-830/libAudioPostProc.a
        LocalAudioSurround += ./aarch64-glibc-gcc-830/libAudioSurround.a
    endif
endif

ifeq ($(MEDIA_TARGET_PLATFORM), buildroot)
    ifeq ($(LICHEE_CROSS_COMPILER),aarch64-none-linux-gnu)
        LocalAudioPostProc += ./aarch64-none-linux-gnu/libAudioPostProc.a
        LocalAudioGain += ./aarch64-none-linux-gnu/libAudioGain.a
    endif
endif

else
CompileFlags += -DONLY_DISABLE_AUDIO
endif

ifeq ($(CONF_DI_V1XX),y)
CompileFlags += -DDI_V1XX
endif
################################################################################
## make commands, all/clean/cleanall
################################################################################

## define commands for make, sush as all, clean
.PHONY: all clean cleantarget cleanall
all:$(Target)
clean:
	-rm -f $(ObjectPath)/*
	-rm -f $(Target)

cleanall: clean
	-rm -f $(DependFilePath)/*
	-rm -rf $(BuildPath)


################################################################################
## define target dependencies.
################################################################################

## compile source files to object files.
#$(ObjectPath)/%.o:%.c
#	$(CC) $(CompileFlags) -o $@ -c $< -fPIC -shared
$(ObjectFiles):$(ObjectPath)/%.o:%.c
	@echo "======tplayer CEDARC_LIBS_PATH:$(CEDARC_LIBS_PATH)==="
	@echo "======tplayer CEDARX_LIBS_PATH:$(CEDARX_LIBS_PATH)==="
	@echo "======tplayer LocalAudioGain:$(LocalAudioGain)==="
	@echo "======tplayer LocalLibPath:$(LocalLibPath)==="
	@echo "======tplayer LocalLinkFlags:$(LocalLinkFlags)==="
	$(CC) $(CompileFlags) -o $@ -c $< -fPIC -shared -v
## link object files to the target share library.
$(Target):$(ObjectFiles)
	$(CC) -o $@ $^ -fPIC -shared  $(LDFLAGS) $(LocalAudioGain) $(LocalAudioPostProc) $(LocalAudioSurround) $(LocalLibPath) $(LocalLinkFlags)

## set rules to generate .d files.
$(DependFilePath)/%.d:%.c
	set -e; rm -f $@; \
	$(CC) -MM $(CompileFlags) $< > $@.$$$$; \
	sed 's,\($*\)\.o[:]*,$(ObjectPath)/\1.o $@: ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

## include the .d files to set dependency rules.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),cleantarget)
ifneq ($(MAKECMDGOALS),cleanall)
-include $(DependFiles)
endif
endif
endif
