
include $(top_srcdir)/Makefile.inc

noinst_LTLIBRARIES = libcdx_mmshttp_parser.la

## set the source files.
libcdx_mmshttp_parser_la_SOURCES =  CdxMmshttpParser.c

libcdx_mmshttp_parser_la_CFLAGS = $(CFLAGS_CDXG)
LOCAL_INCLUDE = -I../include \
                -I../../include \
                -I../../base/include \
                -I../../stream/include \
                -I$(top_srcdir)/ \
                -I$(top_srcdir)/external/include/adecoder \
                -I$(top_srcdir)/../libcedarc/include

libcdx_mmshttp_parser_la_CFLAGS += $(LOCAL_INCLUDE)


libcdx_mmshttp_parser_la_LDFLAGS = $(LDFLAGS_CDXG)

libcdx_mmshttp_parser_la_LIBADD =

#LOCAL_LIB = -lpthread
libcdx_mmshttp_parser_la_LDFLAGS += $(LOCAL_LIB)

