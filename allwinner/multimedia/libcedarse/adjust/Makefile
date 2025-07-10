TARGET = cedarSE_parm_adj

SRCS = sfx_parm_adjust.c

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
