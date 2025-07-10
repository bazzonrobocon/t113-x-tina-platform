TARGET = cedarSE_demo

SRCS = sfx_mgmt_demo.c tinyalsa/*.c

override LDFLAGS += -L$(TARGET_DIR)/usr/lib -lrpbuf
override LDFLAGS += -L$(INSTALL_PREFIX)/usr/lib -lcedarse
override INCLUDE_DIRS += -I ../include
override INCLUDE_DIRS += -I ./tinyalsa/include
override CFLAGS := $(WARNINGS) $(INCLUDE_DIRS) -fPIC

all: $(TARGET) install

$(TARGET): $(SRCS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@ -lm -lpthread

install:
	mkdir -p $(INSTALL_PREFIX)/usr/bin
	cp $(TARGET) $(INSTALL_PREFIX)/usr/bin

clean:
	rm -rf $(TARGET)

.PHONY: all clean install

