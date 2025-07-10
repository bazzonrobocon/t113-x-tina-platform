
include $(top_srcdir)/Makefile.inc

noinst_LTLIBRARIES = libcdx_mms_stream.la

## set the source files.
libcdx_mms_stream_la_SOURCES =  CdxMmshStream.c \
                CdxMmsHttpStream.c \
                CdxMmsStream.c \
                CdxMmsTcp.c \
                CdxMmstStream.c

libcdx_mms_stream_la_CFLAGS = $(CFLAGS_CDXG)
LOCAL_INCLUDE = -I../include \
                -I../../include \
                -I../../base/include \
                -I../../../external/include/zlib \
                -I$(top_srcdir)/

libcdx_mms_stream_la_CFLAGS += $(LOCAL_INCLUDE)


libcdx_mms_stream_la_LDFLAGS = $(LDFLAGS_CDXG)

libcdx_mms_stream_la_LIBADD = $(top_srcdir)/libcore/base/libcdx_base.la

#LOCAL_LIB = -lpthread
libcdx_mms_stream_la_LDFLAGS += $(LOCAL_LIB)

