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

#define dxva_context                      av_d3d11va_context
#define dxva_surface_t                    ID3D11VideoDecoderOutputView
#define DECODER_BUFFER_DESC               D3D11_VIDEO_DECODER_BUFFER_DESC
#define DECODER_BUFFER_TYPE               D3D11_VIDEO_DECODER_BUFFER_TYPE
#define DECODER_GET_BUFFER(ctx,t,b,s)     ID3D11VideoContext_GetDecoderBuffer(ctx->video_context, ctx->decoder, t,s,b)
#define DECODER_SUBMIT_BUFFER(ctx,b,c)    ID3D11VideoContext_SubmitDecoderBuffers(ctx->video_context, ctx->decoder, c,b)
#define DECODER_RELEASE_BUFFER(ctx,t)     ID3D11VideoContext_ReleaseDecoderBuffer(ctx->video_context, ctx->decoder, t)
#define DECODER_BEGIN_FRAME(ctx,s)        ID3D11VideoContext_DecoderBeginFrame(ctx->video_context, ctx->decoder, s, 0, NULL)
#define DECODER_END_FRAME(ctx)            ID3D11VideoContext_DecoderEndFrame(ctx->video_context, ctx->decoder)

#define DECODER_BUFTYPE_PICTURE_PARAMS    D3D11_VIDEO_DECODER_BUFFER_PICTURE_PARAMETERS
#define DECODER_BUFTYPE_QUANT_MATRIX      D3D11_VIDEO_DECODER_BUFFER_INVERSE_QUANTIZATION_MATRIX
#define DECODER_BUFTYPE_BITSTREAM         D3D11_VIDEO_DECODER_BUFFER_BITSTREAM
#define DECODER_BUFTYPE_SLICE_CONTROL     D3D11_VIDEO_DECODER_BUFFER_SLICE_CONTROL

#define DECODER_BUFFER_DESC_SET_TYPE(dsc, type)   dsc->BufferType = type

#define ff_dxva2_get_surface(f)                        ff_d3d11va_get_surface(f)
#define ff_dxva2_get_surface_index(c, f)               ff_d3d11va_get_surface_index(c, f)
#define ff_dxva2_commit_buffer(ctx,dc,d,t,p,ps,c)      ff_d3d11va_commit_buffer(ctx,dc,d,t,p,ps,c)
#define ff_dxva2_common_end_frame(ctx,f,p,ps,q,qs,cb)  ff_d3d11va_common_end_frame(ctx,f,p,ps,q,qs,cb)

#include "dxva_internal_template.h"

#endif /* AVCODEC_D3D11VA_INTERNAL_H */
