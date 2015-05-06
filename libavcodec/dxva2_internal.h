/*
 * DirectX HW acceleration
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

#ifndef AVCODEC_DXVA2_INTERNAL_H
#define AVCODEC_DXVA2_INTERNAL_H

#include "config.h"

#include "avcodec.h"
#include "mpegvideo.h"

dxva_surface *ff_dxva2_get_surface(const AVFrame *frame);

unsigned ff_dxva2_get_surface_index(const struct dxva_context *,
                                    const AVFrame *frame);

int ff_dxva2_commit_buffer(AVCodecContext *, struct dxva_context *,
                           dxva_buffer_desc *,
                           dxva_buffer_type type, const void *data, unsigned size,
                           unsigned mb_count);


int ff_dxva2_common_end_frame(AVCodecContext *, AVFrame *,
                              const void *pp, unsigned pp_size,
                              const void *qm, unsigned qm_size,
                              int (*commit_bs_si)(AVCodecContext *,
                                                  dxva_buffer_desc *bs,
                                                  dxva_buffer_desc *slice));

#endif /* AVCODEC_DXVA2_INTERNAL_H */
