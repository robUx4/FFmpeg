/*
 * DXVA HW acceleration.
 *
 * copyright (c) 2010 Laurent Aimar
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <assert.h>
#include <string.h>

#include "libavutil/log.h"
#include "libavutil/time.h"

#include "avcodec.h"
#include "mpegvideo.h"
#include "dxva_internal.h"

dxva_surface *ff_dxva_get_surface(const AVFrame *frame)
{
    return (dxva_surface*) frame->data[3];
}

unsigned ff_dxva_get_surface_index(const struct dxva_context *ctx,
                                    const AVFrame *frame)
{
    dxva_surface *surface = ff_dxva_get_surface(frame);
    unsigned i;

    for (i = 0; i < ctx->surface_count; i++)
        if (ctx->surface[i] == surface)
            return i;

    assert(0);
    return 0;
}

int ff_dxva_commit_buffer(AVCodecContext *avctx,
                           struct dxva_context *ctx,
                           dxva_buffer_desc *dsc,
                           dxva_buffer_type type, const void *data, unsigned size,
                           unsigned mb_count)
{
    void     *dxva_data;
    unsigned dxva_size;
    int      result;
    HRESULT hr;

    hr = dxva_get_buffer(ctx, type, &dxva_data, &dxva_size);
    if (FAILED(hr)) {
        av_log(avctx, AV_LOG_ERROR, "Failed to get a buffer for %u: 0x%lx\n",
               type, hr);
        return -1;
    }
    if (size <= dxva_size) {
        memcpy(dxva_data, data, size);

        memset(dsc, 0, sizeof(*dsc));
        dxva_set_buffer_type(dsc, type);
        dsc->DataSize             = size;
        dsc->NumMBsInBuffer       = mb_count;

        result = 0;
    } else {
        av_log(avctx, AV_LOG_ERROR, "Buffer for type %u was too small\n", type);
        result = -1;
    }

    hr = dxva_release_buffer(ctx, type);
    if (FAILED(hr)) {
        av_log(avctx, AV_LOG_ERROR,
               "Failed to release buffer type %u: 0x%lx\n",
               type, hr);
        result = -1;
    }
    return result;
}

int ff_dxva_common_end_frame(AVCodecContext *avctx, AVFrame *frame,
                              const void *pp, unsigned pp_size,
                              const void *qm, unsigned qm_size,
                              int (*commit_bs_si)(AVCodecContext *,
                                                  dxva_buffer_desc *bs,
                                                  dxva_buffer_desc *slice))
{
    struct dxva_context *ctx = avctx->hwaccel_context;
    unsigned               buffer_count = 0;
    dxva_buffer_desc buffer[4];
    int result, runs = 0;
    dxva_surface *surface = ff_dxva_get_surface(frame);
    HRESULT hr;

    do {
        hr = dxva_begin_frame(ctx, surface);
        if (hr == E_PENDING)
            av_usleep(2000);
    } while (hr == E_PENDING && ++runs < 50);

    if (FAILED(hr)) {
        av_log(avctx, AV_LOG_ERROR, "Failed to begin frame: 0x%lx\n", hr);
        return -1;
    }

    result = ff_dxva_commit_buffer(avctx, ctx, &buffer[buffer_count],
                                    dxva_buftype_PictureParams,
                                    pp, pp_size, 0);
    if (result) {
        av_log(avctx, AV_LOG_ERROR,
               "Failed to add picture parameter buffer\n");
        goto end;
    }
    buffer_count++;

    if (qm_size > 0) {
        result = ff_dxva_commit_buffer(avctx, ctx, &buffer[buffer_count],
                                        dxva_buftype_IQuantizationMatrix,
                                        qm, qm_size, 0);
        if (result) {
            av_log(avctx, AV_LOG_ERROR,
                   "Failed to add inverse quantization matrix buffer\n");
            goto end;
        }
        buffer_count++;
    }

    result = commit_bs_si(avctx,
                          &buffer[buffer_count + 0],
                          &buffer[buffer_count + 1]);
    if (result) {
        av_log(avctx, AV_LOG_ERROR,
               "Failed to add bitstream or slice control buffer\n");
        goto end;
    }
    buffer_count += 2;

    /* TODO Film Grain when possible */

    assert(buffer_count == 1 + (qm_size > 0) + 2);

    hr = dxva_submit_buffer(ctx, buffer, buffer_count);
    if (FAILED(hr)) {
        av_log(avctx, AV_LOG_ERROR, "Failed to execute: 0x%lx\n", hr);
        result = -1;
    }

end:
    hr = dxva_end_frame(ctx);
    if (FAILED(hr)) {
        av_log(avctx, AV_LOG_ERROR, "Failed to end frame: 0x%lx\n", hr);
        result = -1;
    }

    return result;
}
