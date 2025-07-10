PQ_ROOT := $(shell dirname $(lastword $(MAKEFILE_LIST)))
PRODUCT_COPY_FILES += $(PQ_ROOT)/sunxi_pqdata:$(TARGET_COPY_OUT_VENDOR)/etc/sunxi_pqdata

# if DE support 10bit fcm, set CONFIG_AW_PQ_FCM_10BIT_SUPPORTED = 1
CONFIG_AW_PQ_FCM_10BIT_SUPPORTED ?= 0
ifeq ($(TARGET_BOARD_PLATFORM), saturn)
	CONFIG_AW_PQ_FCM_10BIT_SUPPORTED = 1
endif

# Setup pqd configuration in Soong namespace
SOONG_CONFIG_NAMESPACES += sunxi_pq
SOONG_CONFIG_sunxi_pq := fcm_10bit_supported
SOONG_CONFIG_sunxi_pq_fcm_10bit_supported := $(CONFIG_AW_PQ_FCM_10BIT_SUPPORTED)
