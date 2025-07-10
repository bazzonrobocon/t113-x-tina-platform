TARGET	= powerkey_suspend

INCLUDES	+= -I./include
LIBS		+= -lpthread

SRCS = main.c powerkey_suspend.c

LOCAL_CFLAGS = $(INCLUDES)

LOCAL_CFLAGS += -DCONFIG_TLOG_LEVEL=OPTION_TLOG_LEVEL_WARNING

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LOCAL_CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

####################################################################
all: $(TARGET) install

install:
	mkdir -p $(INSTALL_PREFIX)
	cp $(TARGET) $(INSTALL_PREFIX)

clean:
	rm -rf $(target)

.PHONY: all clean install
