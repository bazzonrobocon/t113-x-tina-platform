ifeq ($(LICHEE_CHIP), $(filter $(LICHEE_CHIP), sun8iw20p1))
	CEDARSE_CONFIG_PATH = ./config-sun8iw20p1
else ifeq ($(LICHEE_CHIP), $(filter $(LICHEE_CHIP), sun55iw3p1))
	CEDARSE_CONFIG_PATH = ./config-sun55iw3p1
else
	CEDARSE_CONFIG_PATH = ./config-common
endif

.PHONY: all

all: install

install:
	mkdir -p $(INSTALL_PREFIX)/usr/config/cedarSE
	cp $(CEDARSE_CONFIG_PATH)/* $(INSTALL_PREFIX)/usr/config/cedarSE