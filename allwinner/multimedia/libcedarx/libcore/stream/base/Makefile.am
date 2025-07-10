include $(top_srcdir)/Makefile.inc

lib_LTLIBRARIES = libcdx_stream.la

## set the source files.
libcdx_stream_la_SOURCES = CdxStream.c

libcdx_stream_la_CFLAGS = $(CFLAGS_CDXG)
LOCAL_INCLUDE = -I../include \
		-I../../include \
		-I../../base/include \
		-I$(top_srcdir)/

if SSL_ENABLE
libcdx_stream_la_CFLAGS += -DSSL_ENABLE
endif
libcdx_stream_la_CFLAGS += $(LOCAL_INCLUDE)

libcdx_stream_la_LDFLAGS = $(LDFLAGS_CDXG)

libcdx_stream_la_LIBADD = $(top_srcdir)/libcore/stream/customer/libcdx_customer_stream.la \
            $(top_srcdir)/libcore/stream/udp/libcdx_udp_stream.la \
            $(top_srcdir)/libcore/stream/tcp/libcdx_tcp_stream.la \
            $(top_srcdir)/libcore/stream/mms/libcdx_mms_stream.la \
            $(top_srcdir)/libcore/base/libcdx_base.la

if FILE_STREAM_ENABLE
libcdx_stream_la_LIBADD += $(top_srcdir)/libcore/stream/file/libcdx_file_stream.la
libcdx_stream_la_CFLAGS += -DFILE_STREAM_ENABLED
endif

if HTTP_STREAM_ENABLE
libcdx_stream_la_LIBADD += $(top_srcdir)/libcore/stream/http/libcdx_http_stream.la
libcdx_stream_la_CFLAGS += -DHTTP_STREAM_ENABLE
endif

if RTSP_STREAM_ENABLE
libcdx_stream_la_LIBADD += $(top_srcdir)/libcore/stream/rtsp/libcdx_rtsp_stream.la
libcdx_stream_la_CFLAGS += -DRTSP_STREAM_ENABLE
libcdx_stream_la_LIBADD += -llive555
endif

if RTMP_STREAM_ENABLE
libcdx_stream_la_LIBADD += $(top_srcdir)/libcore/stream/rtmp/libcdx_rtmp_stream.la
libcdx_stream_la_CFLAGS += -DRTMP_STREAM_ENABLE
endif

if SSL_ENABLE
libcdx_stream_la_LIBADD += $(top_srcdir)/libcore/stream/ssl/libcdx_ssl_stream.la
libcdx_stream_la_LIBADD += -lssl -lcrypto
endif

LOCAL_LIB = -lz -ldl -lstdc++
libcdx_stream_la_LDFLAGS += $(LOCAL_LIB)



