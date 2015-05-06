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

#define dxva_surface_t                    IDirect3DSurface9
#define DECODER_BUFFER_DESC               DXVA2_DecodeBufferDesc
#define DECODER_BUFFER_TYPE               unsigned
#define DECODER_GET_BUFFER(ctx,t,b,s)     IDirectXVideoDecoder_GetBuffer(ctx->decoder, t,b,s)

static inline HRESULT DECODER_SUBMIT_BUFFER(struct dxva_context *ctx, DECODER_BUFFER_DESC *buffer, unsigned buffer_count) {
    DXVA2_DecodeExecuteParams exec = {
        .pCompressedBuffers = buffer,
        .NumCompBuffers = buffer_count,
        .pExtensionData = NULL,
    };
    return IDirectXVideoDecoder_Execute(ctx->decoder, &exec);
}

#define DECODER_RELEASE_BUFFER(ctx,t)     IDirectXVideoDecoder_ReleaseBuffer(ctx->decoder, t)
#define DECODER_BEGIN_FRAME(ctx,s)        IDirectXVideoDecoder_BeginFrame(ctx->decoder, s, NULL)
#define DECODER_END_FRAME(ctx)            IDirectXVideoDecoder_EndFrame(ctx->decoder, NULL)

#define DECODER_BUFTYPE_PICTURE_PARAMS    DXVA2_PictureParametersBufferType
#define DECODER_BUFTYPE_QUANT_MATRIX      DXVA2_InverseQuantizationMatrixBufferType
#define DECODER_BUFTYPE_BITSTREAM         DXVA2_BitStreamDateBufferType
#define DECODER_BUFTYPE_SLICE_CONTROL     DXVA2_SliceControlBufferType

#define DECODER_BUFFER_DESC_SET_TYPE(dsc, type)   dsc->CompressedBufferType = type

#include "dxva_internal_template.h"

#endif /* AVCODEC_DXVA2_INTERNAL_H */
