/*
 * DXVA2 HW acceleration
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

#ifndef AVCODEC_DXVA2_INTERNAL_H
#define AVCODEC_DXVA2_INTERNAL_H

#define COBJMACROS

#include "dxva2.h"
#include <dxva.h>

#define dxva_context                      dxva_context
#define dxva_surface                      IDirect3DSurface9
#define dxva_buffer_desc                  DXVA2_DecodeBufferDesc
#define dxva_buffer_type                  unsigned
#define dxva_get_buffer(ctx,t,b,s)        IDirectXVideoDecoder_GetBuffer(ctx->decoder, t,b,s)

static inline HRESULT dxva_submit_buffer(struct dxva_context *ctx, dxva_buffer_desc *buffer, unsigned buffer_count) {
    DXVA2_DecodeExecuteParams exec = {
        .pCompressedBuffers = buffer,
        .NumCompBuffers = buffer_count,
        .pExtensionData = NULL,
    };
    return IDirectXVideoDecoder_Execute(ctx->decoder, &exec);
}

#define dxva_release_buffer(ctx,t)        IDirectXVideoDecoder_ReleaseBuffer(ctx->decoder, t)
#define dxva_begin_frame(ctx,s)           IDirectXVideoDecoder_BeginFrame(ctx->decoder, s, NULL)
#define dxva_end_frame(ctx)               IDirectXVideoDecoder_EndFrame(ctx->decoder, NULL)

#define dxva_buftype_PictureParams        DXVA2_PictureParametersBufferType
#define dxva_buftype_IQuantizationMatrix  DXVA2_InverseQuantizationMatrixBufferType
#define dxva_buftype_Bitstream            DXVA2_BitStreamDateBufferType
#define dxva_buftype_SliceControl         DXVA2_SliceControlBufferType

#define dxva_set_buffer_type(dsc, type)   dsc->CompressedBufferType = type

#define ff_dxva_get_surface(f)                        ff_dxva2_get_surface(f)
#define ff_dxva_get_surface_index(c, f)               ff_dxva2_get_surface_index(c, f)
#define ff_dxva_commit_buffer(ctx,dc,d,t,p,ps,c)      ff_dxva2_commit_buffer(ctx,dc,d,t,p,ps,c)
#define ff_dxva_common_end_frame(ctx,f,p,ps,q,qs,cb)  ff_dxva2_common_end_frame(ctx,f,p,ps,q,qs,cb)

#include "dxva_internal.h"

#endif /* AVCODEC_DXVA2_INTERNAL_H */
