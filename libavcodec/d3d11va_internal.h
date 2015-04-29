/*
 * DXVA2 HW acceleration
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

#ifndef AVCODEC_D3D11VA_INTERNAL_H
#define AVCODEC_D3D11VA_INTERNAL_H

#define COBJMACROS

#include "config.h"

#include "d3d11va.h"
#if HAVE_DXVA_H
#include <dxva.h>
#endif

#include "avcodec.h"
#include "mpegvideo.h"

ID3D11VideoDecoderOutputView *ff_d3d11va_get_surface(const AVFrame *frame);

unsigned ff_d3d11va_get_surface_index(const struct d3d11va_context *,
                                    const AVFrame *frame);

int ff_d3d11va_commit_buffer(AVCodecContext *, struct d3d11va_context *,
                           D3D11_VIDEO_DECODER_BUFFER_DESC *,
                           D3D11_VIDEO_DECODER_BUFFER_TYPE type, const void *data, unsigned size,
                           unsigned mb_count);


int ff_d3d11va_common_end_frame(AVCodecContext *, AVFrame *,
                              const void *pp, unsigned pp_size,
                              const void *qm, unsigned qm_size,
                              int (*commit_bs_si)(AVCodecContext *,
                                                  D3D11_VIDEO_DECODER_BUFFER_DESC *bs,
                                                  D3D11_VIDEO_DECODER_BUFFER_DESC *slice));

#endif /* AVCODEC_D3D11VA_INTERNAL_H */
