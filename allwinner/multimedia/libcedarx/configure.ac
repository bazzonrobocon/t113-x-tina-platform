#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([cedarx], [VERSION], [BUG-REPORT-ADDRESS])
## ֧�ֺ�ʹ��
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([version.h])
AC_CONFIG_HEADERS([config.h])

## ����Ҫ���ɾ�̬��
AC_DISABLE_STATIC
AM_SILENT_RULES([yes])
## support as
AM_PROG_AS

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_AR
## use libtool
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_CPP

# Checks for libraries.
AC_ARG_ENABLE(mp3_muxer,
		[AC_HELP_STRING([--enable-mp3-muxer],[enable mp3 muxer])],
		[ case "${enableval}" in
		  yes) mp3_muxer=true ;;
		  no)  mp3_muxer=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mp3-muxer) ;;
	esac],
	[mp3_muxer=false])
 AM_CONDITIONAL([MP3_MUXER_ENABLE],  [test x$mp3_muxer = xtrue])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h limits.h locale.h malloc.h memory.h netdb.h netinet/in.h stddef.h stdint.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h sys/time.h sys/timeb.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_STRUCT_TIMEZONE
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_MKTIME
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero clock_gettime gethostbyname gettimeofday memchr memmove memset mkdir select socket strcasecmp strchr strdup strerror strncasecmp strrchr strspn strstr strtol strtoul strtoull])

raw_stream = "no"
checkRawStream()
{
    AC_ARG_ENABLE([raw_stream],
            [ --enable-raw-stream  support raw stream(h264/h265) into library],
            [raw_stream=$enableval], [RAW_STREAM=no])

    AM_CONDITIONAL([RAW_STREAM], [test "x$raw_stream" = "xyes"])
}

ssl = "no"
checkSSL()
{
    AC_ARG_ENABLE([ssl],
        AC_HELP_STRING([--enable-ssl] , [enable ssl, need by https [no]]),
                [ssl=$enableval], [SSL_ENABLE=no])
    AM_CONDITIONAL([SSL_ENABLE], [test "x$ssl" = "xyes"])
}

{
    checkRawStream
    checkSSL
}

 AC_ARG_ENABLE(file_stream,
        [AC_HELP_STRING([--enable-file-stream],[enable file stream])],
        [ case "${enableval}" in
          yes) file_stream=true ;;
          no)  file_stream=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-file-stream) ;;
    esac],
    [file_stream=true])
 AM_CONDITIONAL([FILE_STREAM_ENABLE],  [test x$file_stream = xtrue])

 AC_ARG_ENABLE(http_stream,
        [AC_HELP_STRING([--enable-http-stream],[enable http stream])],
        [ case "${enableval}" in
          yes) http_stream=true ;;
          no)  http_stream=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-http-stream) ;;
    esac],
    [http_stream=true])
 AM_CONDITIONAL([HTTP_STREAM_ENABLE],  [test x$http_stream = xtrue])

 AC_ARG_ENABLE(https_stream,
        [AC_HELP_STRING([--enable-https-stream],[enable https stream])],
        [ case "${enableval}" in
          yes) https_stream=true ;;
          no)  https_stream=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-https-stream) ;;
    esac],
    [https_stream=true])
 AM_CONDITIONAL([HTTPS_STREAM_ENABLE],  [test x$https_stream = xtrue])

 AC_ARG_ENABLE(rtsp_stream,
        [AC_HELP_STRING([--enable-rtsp-stream],[enable rtsp stream])],
        [ case "${enableval}" in
          yes) rtsp_stream=true ;;
          no)  rtsp_stream=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-rtsp-stream) ;;
    esac],
    [rtsp_stream=true])
 AM_CONDITIONAL([RTSP_STREAM_ENABLE],  [test x$rtsp_stream = xtrue])

 AC_ARG_ENABLE(rtmp_stream,
        [AC_HELP_STRING([--enable-rtmp-stream],[enable rtmp stream])],
        [ case "${enableval}" in
          yes) rtmp_stream=true ;;
          no)  rtmp_stream=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-rtmp-stream) ;;
    esac],
    [rtmp_stream=true])
 AM_CONDITIONAL([RTMP_STREAM_ENABLE],  [test x$rtmp_stream = xtrue])

 AC_ARG_ENABLE(aac_muxer,
        [AC_HELP_STRING([--enable-aac-muxer],[enable aac muxer])],
        [ case "${enableval}" in
          yes) aac_muxer=true ;;
          no)  aac_muxer=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-aac-muxer) ;;
    esac],
    [aac_muxer=true])
 AM_CONDITIONAL([AAC_MUXER_ENABLE],  [test x$aac_muxer = xtrue])

 AC_ARG_ENABLE(ts_muxer,
        [AC_HELP_STRING([--enable-ts-muxer],[enable ts muxer])],
        [ case "${enableval}" in
          yes) ts_muxer=true ;;
          no)  ts_muxer=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-ts-muxer) ;;
    esac],
    [ts_muxer=true])
 AM_CONDITIONAL([TS_MUXER_ENABLE],  [test x$ts_muxer = xtrue])

 AC_ARG_ENABLE(mp4_muxer,
        [AC_HELP_STRING([--enable-mp4-muxer],[enable mp4 muxer])],
        [ case "${enableval}" in
          yes) mp4_muxer=true ;;
          no)  mp4_muxer=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mp4-muxer) ;;
    esac],
    [mp4_muxer=true])
 AM_CONDITIONAL([MP4_MUXER_ENABLE],  [test x$mp4_muxer = xtrue])

 AC_ARG_ENABLE(mp3_muxer,
        [AC_HELP_STRING([--enable-mp3-muxer],[enable mp3 muxer])],
        [ case "${enableval}" in
          yes) mp3_muxer=true ;;
          no)  mp3_muxer=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mp3-muxer) ;;
    esac],
    [mp3_muxer=true])
 AM_CONDITIONAL([MP3_MUXER_ENABLE],  [test x$mp3_muxer = xtrue])

 AC_ARG_ENABLE(aac_parser,
        [AC_HELP_STRING([--enable-aac-parser],[enable aac parser])],
        [ case "${enableval}" in
          yes) aac_parser=true ;;
          no)  aac_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-aac-parser) ;;
    esac],
    [aac_parser=true])
 AM_CONDITIONAL([AAC_PARSER_ENABLE],  [test x$aac_parser = xtrue])

 AC_ARG_ENABLE(mp3_parser,
        [AC_HELP_STRING([--enable-mp3-parser],[enable mp3 parser])],
        [ case "${enableval}" in
          yes) mp3_parser=true ;;
          no)  mp3_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mp3-parser) ;;
    esac],
    [mp3_parser=true])
 AM_CONDITIONAL([MP3_PARSER_ENABLE],  [test x$mp3_parser = xtrue])

 AC_ARG_ENABLE(wav_parser,
        [AC_HELP_STRING([--enable-wav-parser],[enable wav parser])],
        [ case "${enableval}" in
          yes) wav_parser=true ;;
          no)  wav_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-wav-parser) ;;
    esac],
    [wav_parser=true])
 AM_CONDITIONAL([WAV_PARSER_ENABLE],  [test x$wav_parser = xtrue])

 AC_ARG_ENABLE(id3v2_parser,
        [AC_HELP_STRING([--enable-id3v2-parser],[enable id3v2 parser])],
        [ case "${enableval}" in
          yes) id3v2_parser=true ;;
          no)  id3v2_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-id3v2-parser) ;;
    esac],
    [id3v2_parser=true])
 AM_CONDITIONAL([ID3V2_PARSER_ENABLE],  [test x$id3v2_parser = xtrue])

 AC_ARG_ENABLE(flac_parser,
        [AC_HELP_STRING([--enable-flac-parser],[enable flac parser])],
        [ case "${enableval}" in
          yes) flac_parser=true ;;
          no)  flac_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-flac-parser) ;;
    esac],
    [flac_parser=true])
 AM_CONDITIONAL([FLAC_PARSER_ENABLE],  [test x$flac_parser = xtrue])

 AC_ARG_ENABLE(ape_parser,
        [AC_HELP_STRING([--enable-ape-parser],[enable ape parser])],
        [ case "${enableval}" in
          yes) ape_parser=true ;;
          no)  ape_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-ape-parser) ;;
    esac],
    [ape_parser=true])
 AM_CONDITIONAL([APE_PARSER_ENABLE],  [test x$ape_parser = xtrue])

 AC_ARG_ENABLE(amr_parser,
        [AC_HELP_STRING([--enable-amr-parser],[enable amr parser])],
        [ case "${enableval}" in
          yes) amr_parser=true ;;
          no)  amr_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-amr-parser) ;;
    esac],
    [amr_parser=true])
 AM_CONDITIONAL([AMR_PARSER_ENABLE],  [test x$amr_parser = xtrue])

 AC_ARG_ENABLE(ogg_parser,
        [AC_HELP_STRING([--enable-ogg-parser],[enable ogg parser])],
        [ case "${enableval}" in
          yes) ogg_parser=true ;;
          no)  ogg_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-ogg-parser) ;;
    esac],
    [ogg_parser=true])
 AM_CONDITIONAL([OGG_PARSER_ENABLE],  [test x$ogg_parser = xtrue])

 AC_ARG_ENABLE(ts_parser,
        [AC_HELP_STRING([--enable-ts-parser],[enable ts parser])],
        [ case "${enableval}" in
          yes) ts_parser=true ;;
          no)  ts_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-ts-parser) ;;
    esac],
    [ts_parser=true])
 AM_CONDITIONAL([TS_PARSER_ENABLE],  [test x$ts_parser = xtrue])

 AC_ARG_ENABLE(mkv_parser,
        [AC_HELP_STRING([--enable-mkv-parser],[enable mkv parser])],
        [ case "${enableval}" in
          yes) mkv_parser=true ;;
          no)  mkv_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mkv-parser) ;;
    esac],
    [mkv_parser=true])
 AM_CONDITIONAL([MKV_PARSER_ENABLE],  [test x$mkv_parser = xtrue])

 AC_ARG_ENABLE(mov_parser,
        [AC_HELP_STRING([--enable-mov-parser],[enable mov parser])],
        [ case "${enableval}" in
          yes) mov_parser=true ;;
          no)  mov_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mov-parser) ;;
    esac],
    [mov_parser=true])
 AM_CONDITIONAL([MOV_PARSER_ENABLE],  [test x$mov_parser = xtrue])

 AC_ARG_ENABLE(avi_parser,
        [AC_HELP_STRING([--enable-avi-parser],[enable avi parser])],
        [ case "${enableval}" in
          yes) avi_parser=true ;;
          no)  avi_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-avi-parser) ;;
    esac],
    [avi_parser=true])
 AM_CONDITIONAL([AVI_PARSER_ENABLE],  [test x$avi_parser = xtrue])

 AC_ARG_ENABLE(flv_parser,
        [AC_HELP_STRING([--enable-flv-parser],[enable flv parser])],
        [ case "${enableval}" in
          yes) flv_parser=true ;;
          no)  flv_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-flv-parser) ;;
    esac],
    [flv_parser=true])
 AM_CONDITIONAL([FLV_PARSER_ENABLE],  [test x$flv_parser = xtrue])

 AC_ARG_ENABLE(asf_parser,
        [AC_HELP_STRING([--enable-asf-parser],[enable asf parser])],
        [ case "${enableval}" in
          yes) asf_parser=true ;;
          no)  asf_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-asf-parser) ;;
    esac],
    [asf_parser=true])
 AM_CONDITIONAL([ASF_PARSER_ENABLE],  [test x$asf_parser = xtrue])

 AC_ARG_ENABLE(hls_parser,
        [AC_HELP_STRING([--enable-hls-parser],[enable hls parser])],
        [ case "${enableval}" in
          yes) hls_parser=true ;;
          no)  hls_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-hls-parser) ;;
    esac],
    [hls_parser=true])
 AM_CONDITIONAL([HLS_PARSER_ENABLE],  [test x$hls_parser = xtrue])

 AC_ARG_ENABLE(mpg_parser,
        [AC_HELP_STRING([--enable-mpg-parser],[enable mpg parser])],
        [ case "${enableval}" in
          yes) mpg_parser=true ;;
          no)  mpg_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mpg-parser) ;;
    esac],
    [mpg_parser=true])
 AM_CONDITIONAL([MPG_PARSER_ENABLE],  [test x$mpg_parser = xtrue])

 AC_ARG_ENABLE(pls_parser,
        [AC_HELP_STRING([--enable-pls-parser],[enable pls parser])],
        [ case "${enableval}" in
          yes) pls_parser=true ;;
          no)  pls_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-pls-parser) ;;
    esac],
    [pls_parser=true])
 AM_CONDITIONAL([PLS_PARSER_ENABLE],  [test x$pls_parser = xtrue])

 AC_ARG_ENABLE(caf_parser,
        [AC_HELP_STRING([--enable-caf-parser],[enable caf parser])],
        [ case "${enableval}" in
          yes) caf_parser=true ;;
          no)  caf_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-caf-parser) ;;
    esac],
    [caf_parser=true])
 AM_CONDITIONAL([CAF_PARSER_ENABLE],  [test x$caf_parser = xtrue])

 AC_ARG_ENABLE(remux_parser,
        [AC_HELP_STRING([--enable-remux-parser],[enable remux parser])],
        [ case "${enableval}" in
          yes) remux_parser=true ;;
          no)  remux_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-remux-parser) ;;
    esac],
    [remux_parser=true])
 AM_CONDITIONAL([REMUX_PARSER_ENABLE],  [test x$remux_parser = xtrue])

 AC_ARG_ENABLE(pmp_parser,
        [AC_HELP_STRING([--enable-pmp-parser],[enable pmp parser])],
        [ case "${enableval}" in
          yes) pmp_parser=true ;;
          no)  pmp_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-pmp-parser) ;;
    esac],
    [pmp_parser=true])
 AM_CONDITIONAL([PMP_PARSER_ENABLE],  [test x$pmp_parser = xtrue])

 AC_ARG_ENABLE(atrac_parser,
        [AC_HELP_STRING([--enable-atrac-parser],[enable atrac parser])],
        [ case "${enableval}" in
          yes) atrac_parser=true ;;
          no)  atrac_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-atrac-parser) ;;
    esac],
    [atrac_parser=true])
 AM_CONDITIONAL([ATRAC_PARSER_ENABLE],  [test x$atrac_parser = xtrue])

 AC_ARG_ENABLE(g729_parser,
        [AC_HELP_STRING([--enable-g729-parser],[enable g729 parser])],
        [ case "${enableval}" in
          yes) g729_parser=true ;;
          no)  g729_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-g729-parser) ;;
    esac],
    [g729_parser=true])
 AM_CONDITIONAL([G729_PARSER_ENABLE],  [test x$g729_parser = xtrue])

 AC_ARG_ENABLE(mmf_parser,
        [AC_HELP_STRING([--enable-mmf-parser],[enable mmf parser])],
        [ case "${enableval}" in
          yes) mmf_parser=true ;;
          no)  mmf_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mmf-parser) ;;
    esac],
    [mmf_parser=true])
 AM_CONDITIONAL([MMF_PARSER_ENABLE],  [test x$mmf_parser = xtrue])

 AC_ARG_ENABLE(dsd_parser,
        [AC_HELP_STRING([--enable-dsd-parser],[enable dsd parser])],
        [ case "${enableval}" in
          yes) dsd_parser=true ;;
          no)  dsd_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-dsd-parser) ;;
    esac],
    [dsd_parser=true])
 AM_CONDITIONAL([DSD_PARSER_ENABLE],  [test x$dsd_parser = xtrue])

 AC_ARG_ENABLE(wv_parser,
        [AC_HELP_STRING([--enable-wv-parser],[enable wv parser])],
        [ case "${enableval}" in
          yes) wv_parser=true ;;
          no)  wv_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-wv-parser) ;;
    esac],
    [dsd_parser=true])
 AM_CONDITIONAL([WV_PARSER_ENABLE],  [test x$wv_parser = xtrue])

 AC_ARG_ENABLE(mms_parser,
        [AC_HELP_STRING([--enable-mms-parser],[enable mms parser])],
        [ case "${enableval}" in
          yes) mms_parser=true ;;
          no)  mms_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-mms-parser) ;;
    esac],
    [mms_parser=true])
 AM_CONDITIONAL([MMS_PARSER_ENABLE],  [test x$mms_parser = xtrue])

 AC_ARG_ENABLE(muxer_test,
        [AC_HELP_STRING([--enable-muxer-test],[enable muxer test])],
        [ case "${enableval}" in
          yes) mms_parser=true ;;
          no)  mms_parser=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-muxer-test) ;;
    esac],
    [muxer_test=true])
 AM_CONDITIONAL([MUXER_TEST_ENABLE],  [test x$mutex_test = xtrue])

 AC_ARG_ENABLE(parser_demo,
        [AC_HELP_STRING([--enable-parser-demo],[enable parser demo])],
        [ case "${enableval}" in
          yes) parser_demo=true ;;
          no)  parser_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-parser-demo) ;;
    esac],
    [parser_demo=true])
 AM_CONDITIONAL([PARSER_DEMO_ENABLE],  [test x$parser_demo = xtrue])

 AC_ARG_ENABLE(vdec_demo,
        [AC_HELP_STRING([--enable-vdec-demo],[enable vdec demo])],
        [ case "${enableval}" in
          yes) vdec_demo=true ;;
          no)  vdec_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-vdec-demo) ;;
    esac],
    [vdec_demo=false])
 AM_CONDITIONAL([VDEC_DEMO_ENABLE],  [test x$vdec_demo = xtrue])

 AC_ARG_ENABLE(venc_demo,
        [AC_HELP_STRING([--enable-venc-demo],[enable venc demo])],
        [ case "${enableval}" in
          yes) venc_demo=true ;;
          no)  venc_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-venc-demo) ;;
    esac],
    [venc_demo=false])
 AM_CONDITIONAL([VENC_DEMO_ENABLE],  [test x$venc_demo = xtrue])

 AC_ARG_ENABLE(xplayer_demo,
        [AC_HELP_STRING([--enable-xplayer-demo],[enable xplayer demo])],
        [ case "${enableval}" in
          yes) xplayer_demo=true ;;
          no)  xplayer_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-xplayer-demo) ;;
    esac],
    [xplayer_demo=false])
 AM_CONDITIONAL([XPLAYER_DEMO_ENABLE],  [test x$xplayer_demo = xtrue])

 AC_ARG_ENABLE(xmeta_demo,
        [AC_HELP_STRING([--enable-xmeta-demo],[enable xmeta demo])],
        [ case "${enableval}" in
          yes) xmeta_demo=true ;;
          no)  xmeta_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-xmeta-demo) ;;
    esac],
    [xmeta_demo=true])
 AM_CONDITIONAL([XMETA_DEMO_ENABLE],  [test x$xmeta_demo = xtrue])

 AC_ARG_ENABLE(jpeg_demo,
        [AC_HELP_STRING([--enable-jpeg-demo],[enable jpeg demo])],
        [ case "${enableval}" in
          yes) jpeg_demo=true ;;
          no)  jpeg_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-jpeg-demo) ;;
    esac],
    [jpeg_demo=false])
 AM_CONDITIONAL([JPEG_DEMO_ENABLE],  [test x$jpeg_demo = xtrue])

 AC_ARG_ENABLE(recoder_demo,
        [AC_HELP_STRING([--enable-recoder-demo],[enable recoder demo])],
        [ case "${enableval}" in
          yes) recoder_demo=true ;;
          no)  recoder_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-recoder-demo) ;;
    esac],
    [recoder_demo=false])
 AM_CONDITIONAL([RECODER_DEMO_ENABLE],  [test x$recoder_demo = xtrue])

 AC_ARG_ENABLE(omx_demo,
        [AC_HELP_STRING([--enable-omx-demo],[enable omx demo])],
        [ case "${enableval}" in
          yes) omx_demo=true ;;
          no)  omx_demo=false ;;
          *)   AC_MSG_ERROR(bad value ${enableval} for --enable-omx-demo) ;;
    esac],
    [omx_demo=false])
 AM_CONDITIONAL([OMX_DEMO_ENABLE],  [test x$omx_demo = xtrue])

AC_CONFIG_FILES([Makefile
        xmetadataretriever/Makefile
        awrecorder/Makefile
        xplayer/Makefile
        demo/Makefile
        demo/muxtest/Makefile
        demo/xmetademo/Makefile
        demo/jpegdemo/Makefile
        demo/recoderdemo/Makefile
        demo/parserdemo/Makefile
        demo/demoVdecoder/Makefile
        demo/xplayerdemo/Makefile
        demo/demoOmxVenc/Makefile
        demo/demoOmxVdec/Makefile
        demo/demoVencVdec/Makefile
        libcore/Makefile
        libcore/playback/Makefile
        libcore/common/Makefile
        libcore/stream/Makefile
        libcore/stream/customer/Makefile
        libcore/stream/file/Makefile
        libcore/stream/http/Makefile
        libcore/stream/udp/Makefile
        libcore/stream/tcp/Makefile
        libcore/stream/mms/Makefile
        libcore/stream/rtmp/Makefile
        libcore/stream/rtsp/Makefile
        libcore/stream/ssl/Makefile
        libcore/stream/base/Makefile
        libcore/parser/Makefile
        libcore/parser/ts/Makefile
        libcore/parser/flv/Makefile
        libcore/parser/avi/Makefile
        libcore/parser/mov/Makefile
        libcore/parser/mkv/Makefile
        libcore/parser/asf/Makefile
        libcore/parser/remux/Makefile
        libcore/parser/hls/Makefile
        libcore/parser/ogg/Makefile
        libcore/parser/mpg/Makefile
        libcore/parser/ape/Makefile
        libcore/parser/flac/Makefile
        libcore/parser/amr/Makefile
        libcore/parser/mp3/Makefile
        libcore/parser/aac/Makefile
        libcore/parser/wav/Makefile
        libcore/parser/id3v2/Makefile
        libcore/parser/pls/Makefile
        libcore/parser/pmp/Makefile
        libcore/parser/mmf/Makefile
        libcore/parser/wv/Makefile
        libcore/parser/dsd/Makefile
        libcore/parser/g729/Makefile
        libcore/parser/atrac/Makefile
        libcore/parser/mms/Makefile
        libcore/parser/mmshttp/Makefile
        libcore/parser/base/Makefile
        libcore/parser/AwRawStream/Makefile
        libcore/parser/caf/Makefile
        libcore/muxer/Makefile
        libcore/muxer/aac/Makefile
        libcore/muxer/mp3/Makefile
        libcore/muxer/mp4/Makefile
        libcore/muxer/ts/Makefile
        libcore/muxer/base/Makefile
        libcore/base/Makefile
        demo/libion/Makefile
        ])
AC_OUTPUT

#plugin compile
#       libcore/libplugin/Makefile
#       libcore/libplugin/rxx/Makefile
#       libcore/libplugin/rtp/Makefile
