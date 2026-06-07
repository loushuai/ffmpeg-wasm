#include <stdio.h>
#include <emscripten.h>
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"

EMSCRIPTEN_KEEPALIVE
int get_version_info(void) {
    printf("FFmpeg WebAssembly Demo\n");
    printf("=======================\n");
    printf("libavcodec version:  %s\n", av_version_info());
    printf("libavformat version: %d.%d.%d\n",
           LIBAVFORMAT_VERSION_MAJOR,
           LIBAVFORMAT_VERSION_MINOR,
           LIBAVFORMAT_VERSION_MICRO);
    printf("libavcodec version:  %d.%d.%d\n",
           LIBAVCODEC_VERSION_MAJOR,
           LIBAVCODEC_VERSION_MINOR,
           LIBAVCODEC_VERSION_MICRO);
    return 0;
}

EMSCRIPTEN_KEEPALIVE
int test_h264_decoder(void) {
    printf("\n--- H.264 Decoder Test ---\n");

    const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        printf("FAIL: H.264 decoder not found\n");
        return -1;
    }
    printf("PASS: Found decoder: %s (%s)\n", codec->name, codec->long_name);

    AVCodecContext *ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        printf("FAIL: Could not allocate codec context\n");
        return -1;
    }
    printf("PASS: Allocated codec context\n");

    int ret = avcodec_open2(ctx, codec, NULL);
    if (ret < 0) {
        char errbuf[128];
        av_strerror(ret, errbuf, sizeof(errbuf));
        printf("FAIL: Could not open codec: %s\n", errbuf);
        avcodec_free_context(&ctx);
        return -1;
    }
    printf("PASS: Opened H.264 decoder successfully\n");

    avcodec_free_context(&ctx);
    printf("PASS: Freed codec context\n");
    return 0;
}

EMSCRIPTEN_KEEPALIVE
int test_aac_decoder(void) {
    printf("\n--- AAC Decoder Test ---\n");

    const AVCodec *codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
    if (!codec) {
        printf("FAIL: AAC decoder not found\n");
        return -1;
    }
    printf("PASS: Found decoder: %s (%s)\n", codec->name, codec->long_name);

    AVCodecContext *ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        printf("FAIL: Could not allocate codec context\n");
        return -1;
    }
    printf("PASS: Allocated codec context\n");

    int ret = avcodec_open2(ctx, codec, NULL);
    if (ret < 0) {
        char errbuf[128];
        av_strerror(ret, errbuf, sizeof(errbuf));
        printf("FAIL: Could not open codec: %s\n", errbuf);
        avcodec_free_context(&ctx);
        return -1;
    }
    printf("PASS: Opened AAC decoder successfully\n");

    avcodec_free_context(&ctx);
    printf("PASS: Freed codec context\n");
    return 0;
}

EMSCRIPTEN_KEEPALIVE
int test_packet_alloc(void) {
    printf("\n--- Packet/Frame Allocation Test ---\n");

    AVPacket *pkt = av_packet_alloc();
    if (!pkt) {
        printf("FAIL: Could not allocate packet\n");
        return -1;
    }
    printf("PASS: Allocated AVPacket\n");

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        printf("FAIL: Could not allocate frame\n");
        av_packet_free(&pkt);
        return -1;
    }
    printf("PASS: Allocated AVFrame\n");

    av_packet_free(&pkt);
    av_frame_free(&frame);
    printf("PASS: Freed packet and frame\n");
    return 0;
}

int main(void) {
    int failures = 0;

    failures += (get_version_info() != 0);
    failures += (test_h264_decoder() != 0);
    failures += (test_aac_decoder() != 0);
    failures += (test_packet_alloc() != 0);

    printf("\n=======================\n");
    if (failures == 0) {
        printf("All tests PASSED!\n");
    } else {
        printf("%d test(s) FAILED\n", failures);
    }
    return failures;
}
