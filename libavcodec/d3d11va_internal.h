/*
 * Direct3D11 HW acceleration
 *
 * copyright (c) 2010 Laurent Aimar
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

#ifndef AVCODEC_D3D11VA_INTERNAL_H
#define AVCODEC_D3D11VA_INTERNAL_H

#define COBJMACROS

#include "d3d11va.h"
#include <dxva.h>

#define dxva_context                      d3d11va_context
#define dxva_surface                      ID3D11VideoDecoderOutputView
#define dxva_buffer_desc                  D3D11_VIDEO_DECODER_BUFFER_DESC
#define dxva_buffer_type                  D3D11_VIDEO_DECODER_BUFFER_TYPE
#define dxva_get_buffer(ctx,t,b,s)        ID3D11VideoContext_GetDecoderBuffer(ctx->video_context, ctx->decoder, t,s,b)
#define dxva_submit_buffer(ctx,b,c)       ID3D11VideoContext_SubmitDecoderBuffers(ctx->video_context, ctx->decoder, c,b)
#define dxva_release_buffer(ctx,t)        ID3D11VideoContext_ReleaseDecoderBuffer(ctx->video_context, ctx->decoder, t)
#define dxva_begin_frame(ctx,s)           ID3D11VideoContext_DecoderBeginFrame(ctx->video_context, ctx->decoder, s, 0, NULL)
#define dxva_end_frame(ctx)               ID3D11VideoContext_DecoderEndFrame(ctx->video_context, ctx->decoder)

#define dxva_buftype_PictureParams        D3D11_VIDEO_DECODER_BUFFER_PICTURE_PARAMETERS
#define dxva_buftype_IQuantizationMatrix  D3D11_VIDEO_DECODER_BUFFER_INVERSE_QUANTIZATION_MATRIX
#define dxva_buftype_Bitstream            D3D11_VIDEO_DECODER_BUFFER_BITSTREAM
#define dxva_buftype_SliceControl         D3D11_VIDEO_DECODER_BUFFER_SLICE_CONTROL

#define dxva_set_buffer_type(dsc, type)   dsc->BufferType = type

#define ff_dxva_get_surface(f)                        ff_d3d11va_get_surface(f)
#define ff_dxva_get_surface_index(c, f)               ff_d3d11va_get_surface_index(c, f)
#define ff_dxva_commit_buffer(ctx,dc,d,t,p,ps,c)      ff_d3d11va_commit_buffer(ctx,dc,d,t,p,ps,c)
#define ff_dxva_common_end_frame(ctx,f,p,ps,q,qs,cb)  ff_d3d11va_common_end_frame(ctx,f,p,ps,q,qs,cb)

#include "dxva_internal.h"

#endif /* AVCODEC_D3D11VA_INTERNAL_H */
