ifeq ($(LICHEE_ARCH),arm64)
CEDARSE_LIB_PATH=./arm64
else ifeq ($(LICHEE_ARCH),arm)
CEDARSE_LIB_PATH=./arm32
else ifeq ($(LICHEE_ARCH),riscv)
CEDARSE_LIB_PATH=./riscv
endif

# CEDARSE_LIB_PATH = ./arm64
# CEDARSE_CONFIG_PATH = ../config

.PHONY: all

all: install

install:
	mkdir -p $(INSTALL_PREFIX)/usr/lib
	cp $(CEDARSE_LIB_PATH)/*.so $(INSTALL_PREFIX)/usr/lib