/*
 * Direct3D11 HEVC HW acceleration.
 *
 * copyright (c) 2014 - 2015 Hendrik Leppkes
 * copyright (c) 2015 Steve Lhomme
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

#include "d3d11va_internal.h"

#define dxva2_hevc_start_frame   d3d11va_hevc_start_frame
#define dxva2_hevc_decode_slice  d3d11va_hevc_decode_slice
#define dxva2_hevc_end_frame     d3d11va_hevc_end_frame

#include "dxva2_hevc.c"

AVHWAccel ff_hevc_d3d11va_hwaccel = {
    .name           = "hevc_d3d11va",
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_HEVC,
    .pix_fmt        = AV_PIX_FMT_D3D11VA_VLD,
    .start_frame    = d3d11va_hevc_start_frame,
    .decode_slice   = d3d11va_hevc_decode_slice,
    .end_frame      = d3d11va_hevc_end_frame,
    .frame_priv_data_size = sizeof(struct hevc_dxva2_picture_context),
};
