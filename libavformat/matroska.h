/*
 * Matroska constants
 * Copyright (c) 2003-2004 The FFmpeg project
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

#ifndef AVFORMAT_MATROSKA_H
#define AVFORMAT_MATROSKA_H

#include "libavcodec/avcodec.h"
#include "matroska_ids.h"
#include "metadata.h"
#include "internal.h"

/* EBML version supported */
#define EBML_VERSION 1

/* top-level master-IDs */
#define EBML_ID_HEADER             0x1A45DFA3

/* IDs in the HEADER master */
#define EBML_ID_EBMLVERSION        0x4286
#define EBML_ID_EBMLREADVERSION    0x42F7
#define EBML_ID_EBMLMAXIDLENGTH    0x42F2
#define EBML_ID_EBMLMAXSIZELENGTH  0x42F3
#define EBML_ID_DOCTYPE            0x4282
#define EBML_ID_DOCTYPEVERSION     0x4287
#define EBML_ID_DOCTYPEREADVERSION 0x4285

/* general EBML types */
#define EBML_ID_VOID               0xEC
#define EBML_ID_CRC32              0xBF

/*
 * Matroska Codec IDs, strings
 */

typedef struct CodecTags{
    char str[22];
    enum AVCodecID id;
}CodecTags;

/* max. depth in the EBML tree structure */
#define EBML_MAX_DEPTH 16

#define MATROSKA_VIDEO_STEREO_PLANE_COUNT  3

extern const CodecTags ff_mkv_codec_tags[];
extern const CodecTags ff_webm_codec_tags[];
extern const CodecMime ff_mkv_mime_tags[];
extern const CodecMime ff_mkv_image_mime_tags[];
extern const AVMetadataConv ff_mkv_metadata_conv[];
extern const char * const ff_matroska_video_stereo_mode[MATROSKA_VIDEO_STEREOMODE_TYPE_NB];
extern const char * const ff_matroska_video_stereo_plane[MATROSKA_VIDEO_STEREO_PLANE_COUNT];

/* AVStream Metadata tag keys for WebM Dash Manifest */
#define INITIALIZATION_RANGE "webm_dash_manifest_initialization_range"
#define CUES_START "webm_dash_manifest_cues_start"
#define CUES_END "webm_dash_manifest_cues_end"
#define FILENAME "webm_dash_manifest_file_name"
#define BANDWIDTH "webm_dash_manifest_bandwidth"
#define DURATION "webm_dash_manifest_duration"
#define CLUSTER_KEYFRAME "webm_dash_manifest_cluster_keyframe"
#define CUE_TIMESTAMPS "webm_dash_manifest_cue_timestamps"
#define TRACK_NUMBER "webm_dash_manifest_track_number"
#define CODEC_PRIVATE_SIZE "webm_dash_manifest_codec_priv_size"

int ff_mkv_stereo3d_conv(AVStream *st, MatroskaVideoStereoModeType stereo_mode);

#endif /* AVFORMAT_MATROSKA_H */
