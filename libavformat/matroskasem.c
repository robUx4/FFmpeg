/*
 * Matroska file semantic definition
 * Copyright (c) 2003-2019 The FFmpeg Project
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

/**
 * @file
 * Matroska file semantic element definitions
 * @author Ronald Bultje <rbultje@ronald.bitfreak.net>
 * @author with a little help from Moritz Bunkus <moritz@bunkus.org>
 * @author totally reworked by Aurelien Jacobs <aurel@gnuage.org>
 * @author Split from decoder by Steve Lhomme <slhomme@matroska.org>
 * @see specs available on the Matroska project page: http://www.matroska.org/
 */

#include "config.h"

#include <inttypes.h>
#include <stdio.h>

#include "matroskasem.h"

#define CHILD_OF(parent) { .def = { .n = parent } }

static EbmlSyntax ebml_header[] = {
    { EBML_ID_EBMLREADVERSION,    EBML_UINT, 0, offsetof(Ebml, version),         { .u = EBML_VERSION } },
    { EBML_ID_EBMLMAXSIZELENGTH,  EBML_UINT, 0, offsetof(Ebml, max_size),        { .u = 8 } },
    { EBML_ID_EBMLMAXIDLENGTH,    EBML_UINT, 0, offsetof(Ebml, id_length),       { .u = 4 } },
    { EBML_ID_DOCTYPE,            EBML_STR,  0, offsetof(Ebml, doctype),         { .s = "(none)" } },
    { EBML_ID_DOCTYPEREADVERSION, EBML_UINT, 0, offsetof(Ebml, doctype_version), { .u = 1 } },
    { EBML_ID_EBMLVERSION,        EBML_NONE },
    { EBML_ID_DOCTYPEVERSION,     EBML_NONE },
    CHILD_OF(ebml_syntax)
};

static EbmlSyntax ebml_syntax[] = {
    { EBML_ID_HEADER,      EBML_NEST, 0, 0, { .n = ebml_header } },
    { MATROSKA_ID_SEGMENT, EBML_STOP },
    { 0 }
};

static EbmlSyntax matroska_info[] = {
    { MATROSKA_ID_TIMECODESCALE, EBML_UINT,  0, offsetof(MatroskaDemuxContext, time_scale), { .u = 1000000 } },
    { MATROSKA_ID_DURATION,      EBML_FLOAT, 0, offsetof(MatroskaDemuxContext, duration) },
    { MATROSKA_ID_TITLE,         EBML_UTF8,  0, offsetof(MatroskaDemuxContext, title) },
    { MATROSKA_ID_WRITINGAPP,    EBML_NONE },
    { MATROSKA_ID_MUXINGAPP,     EBML_UTF8, 0, offsetof(MatroskaDemuxContext, muxingapp) },
    { MATROSKA_ID_DATEUTC,       EBML_BIN,  0, offsetof(MatroskaDemuxContext, date_utc) },
    { MATROSKA_ID_SEGMENTUID,    EBML_NONE },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_mastering_meta[] = {
    { MATROSKA_ID_VIDEOCOLOR_RX, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, r_x), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_RY, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, r_y), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_GX, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, g_x), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_GY, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, g_y), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_BX, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, b_x), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_BY, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, b_y), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_WHITEX, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, white_x), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_WHITEY, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, white_y), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_LUMINANCEMIN, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, min_luminance), { .f=-1 } },
    { MATROSKA_ID_VIDEOCOLOR_LUMINANCEMAX, EBML_FLOAT, 0, offsetof(MatroskaMasteringMeta, max_luminance), { .f=-1 } },
    CHILD_OF(matroska_track_video_color)
};

static EbmlSyntax matroska_track_video_color[] = {
    { MATROSKA_ID_VIDEOCOLORMATRIXCOEFF,      EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, matrix_coefficients), { .u = AVCOL_SPC_UNSPECIFIED } },
    { MATROSKA_ID_VIDEOCOLORBITSPERCHANNEL,   EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, bits_per_channel), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORCHROMASUBHORZ,    EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, chroma_sub_horz), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORCHROMASUBVERT,    EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, chroma_sub_vert), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORCBSUBHORZ,        EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, cb_sub_horz), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORCBSUBVERT,        EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, cb_sub_vert), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORCHROMASITINGHORZ, EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, chroma_siting_horz), { .u = MATROSKA_COLOUR_CHROMASITINGHORZ_UNDETERMINED } },
    { MATROSKA_ID_VIDEOCOLORCHROMASITINGVERT, EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, chroma_siting_vert), { .u = MATROSKA_COLOUR_CHROMASITINGVERT_UNDETERMINED } },
    { MATROSKA_ID_VIDEOCOLORRANGE,            EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, range), { .u = AVCOL_RANGE_UNSPECIFIED } },
    { MATROSKA_ID_VIDEOCOLORTRANSFERCHARACTERISTICS, EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, transfer_characteristics), { .u = AVCOL_TRC_UNSPECIFIED } },
    { MATROSKA_ID_VIDEOCOLORPRIMARIES,        EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, primaries), { .u = AVCOL_PRI_UNSPECIFIED } },
    { MATROSKA_ID_VIDEOCOLORMAXCLL,           EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, max_cll), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORMAXFALL,          EBML_UINT, 0, offsetof(MatroskaTrackVideoColor, max_fall), { .u=0 } },
    { MATROSKA_ID_VIDEOCOLORMASTERINGMETA,    EBML_NEST, 0, offsetof(MatroskaTrackVideoColor, mastering_meta), { .n = matroska_mastering_meta } },
    CHILD_OF(matroska_track_video)
};

static EbmlSyntax matroska_track_video_projection[] = {
    { MATROSKA_ID_VIDEOPROJECTIONTYPE,        EBML_UINT,  0, offsetof(MatroskaTrackVideoProjection, type), { .u = MATROSKA_VIDEO_PROJECTION_TYPE_RECTANGULAR } },
    { MATROSKA_ID_VIDEOPROJECTIONPRIVATE,     EBML_BIN,   0, offsetof(MatroskaTrackVideoProjection, private) },
    { MATROSKA_ID_VIDEOPROJECTIONPOSEYAW,     EBML_FLOAT, 0, offsetof(MatroskaTrackVideoProjection, yaw), { .f=0.0 } },
    { MATROSKA_ID_VIDEOPROJECTIONPOSEPITCH,   EBML_FLOAT, 0, offsetof(MatroskaTrackVideoProjection, pitch), { .f=0.0 } },
    { MATROSKA_ID_VIDEOPROJECTIONPOSEROLL,    EBML_FLOAT, 0, offsetof(MatroskaTrackVideoProjection, roll), { .f=0.0 } },
    CHILD_OF(matroska_track_video)
};

static EbmlSyntax matroska_track_video[] = {
    { MATROSKA_ID_VIDEOFRAMERATE,      EBML_FLOAT, 0, offsetof(MatroskaTrackVideo, frame_rate) },
    { MATROSKA_ID_VIDEODISPLAYWIDTH,   EBML_UINT,  0, offsetof(MatroskaTrackVideo, display_width), { .u=-1 } },
    { MATROSKA_ID_VIDEODISPLAYHEIGHT,  EBML_UINT,  0, offsetof(MatroskaTrackVideo, display_height), { .u=-1 } },
    { MATROSKA_ID_VIDEOPIXELWIDTH,     EBML_UINT,  0, offsetof(MatroskaTrackVideo, pixel_width) },
    { MATROSKA_ID_VIDEOPIXELHEIGHT,    EBML_UINT,  0, offsetof(MatroskaTrackVideo, pixel_height) },
    { MATROSKA_ID_VIDEOCOLORSPACE,     EBML_BIN,   0, offsetof(MatroskaTrackVideo, color_space) },
    { MATROSKA_ID_VIDEOALPHAMODE,      EBML_UINT,  0, offsetof(MatroskaTrackVideo, alpha_mode) },
    { MATROSKA_ID_VIDEOCOLOR,          EBML_NEST,  sizeof(MatroskaTrackVideoColor), offsetof(MatroskaTrackVideo, color), { .n = matroska_track_video_color } },
    { MATROSKA_ID_VIDEOPROJECTION,     EBML_NEST,  0, offsetof(MatroskaTrackVideo, projection), { .n = matroska_track_video_projection } },
    { MATROSKA_ID_VIDEOPIXELCROPB,     EBML_NONE },
    { MATROSKA_ID_VIDEOPIXELCROPT,     EBML_NONE },
    { MATROSKA_ID_VIDEOPIXELCROPL,     EBML_NONE },
    { MATROSKA_ID_VIDEOPIXELCROPR,     EBML_NONE },
    { MATROSKA_ID_VIDEODISPLAYUNIT,    EBML_UINT,  0, offsetof(MatroskaTrackVideo, display_unit), { .u= MATROSKA_VIDEO_DISPLAYUNIT_PIXELS } },
    { MATROSKA_ID_VIDEOFLAGINTERLACED, EBML_UINT,  0, offsetof(MatroskaTrackVideo, interlaced),  { .u = MATROSKA_VIDEO_INTERLACE_FLAG_UNDETERMINED } },
    { MATROSKA_ID_VIDEOFIELDORDER,     EBML_UINT,  0, offsetof(MatroskaTrackVideo, field_order), { .u = MATROSKA_VIDEO_FIELDORDER_UNDETERMINED } },
    { MATROSKA_ID_VIDEOSTEREOMODE,     EBML_UINT,  0, offsetof(MatroskaTrackVideo, stereo_mode), { .u = MATROSKA_VIDEO_STEREOMODE_TYPE_NB } },
    { MATROSKA_ID_VIDEOASPECTRATIO,    EBML_NONE },
    CHILD_OF(matroska_track)
};

static EbmlSyntax matroska_track_audio[] = {
    { MATROSKA_ID_AUDIOSAMPLINGFREQ,    EBML_FLOAT, 0, offsetof(MatroskaTrackAudio, samplerate), { .f = 8000.0 } },
    { MATROSKA_ID_AUDIOOUTSAMPLINGFREQ, EBML_FLOAT, 0, offsetof(MatroskaTrackAudio, out_samplerate) },
    { MATROSKA_ID_AUDIOBITDEPTH,        EBML_UINT,  0, offsetof(MatroskaTrackAudio, bitdepth) },
    { MATROSKA_ID_AUDIOCHANNELS,        EBML_UINT,  0, offsetof(MatroskaTrackAudio, channels),   { .u = 1 } },
    CHILD_OF(matroska_track)
};

static EbmlSyntax matroska_track_encoding_compression[] = {
    { MATROSKA_ID_ENCODINGCOMPALGO,     EBML_UINT, 0, offsetof(MatroskaTrackCompression, algo), { .u = 0 } },
    { MATROSKA_ID_ENCODINGCOMPSETTINGS, EBML_BIN,  0, offsetof(MatroskaTrackCompression, settings) },
    CHILD_OF(matroska_track_encoding)
};

static EbmlSyntax matroska_track_encoding_encryption[] = {
    { MATROSKA_ID_ENCODINGENCALGO,        EBML_UINT, 0, offsetof(MatroskaTrackEncryption,algo), {.u = 0} },
    { MATROSKA_ID_ENCODINGENCKEYID,       EBML_BIN, 0, offsetof(MatroskaTrackEncryption,key_id) },
    { MATROSKA_ID_ENCODINGENCAESSETTINGS, EBML_NONE },
    { MATROSKA_ID_ENCODINGSIGALGO,        EBML_NONE },
    { MATROSKA_ID_ENCODINGSIGHASHALGO,    EBML_NONE },
    { MATROSKA_ID_ENCODINGSIGKEYID,       EBML_NONE },
    { MATROSKA_ID_ENCODINGSIGNATURE,      EBML_NONE },
    CHILD_OF(matroska_track_encoding)
};
static EbmlSyntax matroska_track_encoding[] = {
    { MATROSKA_ID_ENCODINGSCOPE,       EBML_UINT, 0, offsetof(MatroskaTrackEncoding, scope),       { .u = 1 } },
    { MATROSKA_ID_ENCODINGTYPE,        EBML_UINT, 0, offsetof(MatroskaTrackEncoding, type),        { .u = 0 } },
    { MATROSKA_ID_ENCODINGCOMPRESSION, EBML_NEST, 0, offsetof(MatroskaTrackEncoding, compression), { .n = matroska_track_encoding_compression } },
    { MATROSKA_ID_ENCODINGENCRYPTION,  EBML_NEST, 0, offsetof(MatroskaTrackEncoding, encryption),  { .n = matroska_track_encoding_encryption } },
    { MATROSKA_ID_ENCODINGORDER,       EBML_NONE },
    CHILD_OF(matroska_track_encodings)
};

static EbmlSyntax matroska_track_encodings[] = {
    { MATROSKA_ID_TRACKCONTENTENCODING, EBML_NEST, sizeof(MatroskaTrackEncoding), offsetof(MatroskaTrack, encodings), { .n = matroska_track_encoding } },
    CHILD_OF(matroska_track)
};

static EbmlSyntax matroska_track_plane[] = {
    { MATROSKA_ID_TRACKPLANEUID,  EBML_UINT, 0, offsetof(MatroskaTrackPlane,uid) },
    { MATROSKA_ID_TRACKPLANETYPE, EBML_UINT, 0, offsetof(MatroskaTrackPlane,type) },
    CHILD_OF(matroska_track_combine_planes)
};

static EbmlSyntax matroska_track_combine_planes[] = {
    { MATROSKA_ID_TRACKPLANE, EBML_NEST, sizeof(MatroskaTrackPlane), offsetof(MatroskaTrackOperation,combine_planes), {.n = matroska_track_plane} },
    CHILD_OF(matroska_track_operation)
};

static EbmlSyntax matroska_track_operation[] = {
    { MATROSKA_ID_TRACKCOMBINEPLANES, EBML_NEST, 0, 0, {.n = matroska_track_combine_planes} },
    CHILD_OF(matroska_track)
};

static EbmlSyntax matroska_track[] = {
    { MATROSKA_ID_TRACKNUMBER,           EBML_UINT,  0, offsetof(MatroskaTrack, num) },
    { MATROSKA_ID_TRACKNAME,             EBML_UTF8,  0, offsetof(MatroskaTrack, name) },
    { MATROSKA_ID_TRACKUID,              EBML_UINT,  0, offsetof(MatroskaTrack, uid) },
    { MATROSKA_ID_TRACKTYPE,             EBML_UINT,  0, offsetof(MatroskaTrack, type) },
    { MATROSKA_ID_CODECID,               EBML_STR,   0, offsetof(MatroskaTrack, codec_id) },
    { MATROSKA_ID_CODECPRIVATE,          EBML_BIN,   0, offsetof(MatroskaTrack, codec_priv) },
    { MATROSKA_ID_CODECDELAY,            EBML_UINT,  0, offsetof(MatroskaTrack, codec_delay) },
    { MATROSKA_ID_TRACKLANGUAGE,         EBML_UTF8,  0, offsetof(MatroskaTrack, language),     { .s = "eng" } },
    { MATROSKA_ID_TRACKDEFAULTDURATION,  EBML_UINT,  0, offsetof(MatroskaTrack, default_duration) },
    { MATROSKA_ID_TRACKTIMECODESCALE,    EBML_FLOAT, 0, offsetof(MatroskaTrack, time_scale),   { .f = 1.0 } },
    { MATROSKA_ID_TRACKFLAGDEFAULT,      EBML_UINT,  0, offsetof(MatroskaTrack, flag_default), { .u = 1 } },
    { MATROSKA_ID_TRACKFLAGFORCED,       EBML_UINT,  0, offsetof(MatroskaTrack, flag_forced),  { .u = 0 } },
    { MATROSKA_ID_TRACKVIDEO,            EBML_NEST,  0, offsetof(MatroskaTrack, video),        { .n = matroska_track_video } },
    { MATROSKA_ID_TRACKAUDIO,            EBML_NEST,  0, offsetof(MatroskaTrack, audio),        { .n = matroska_track_audio } },
    { MATROSKA_ID_TRACKOPERATION,        EBML_NEST,  0, offsetof(MatroskaTrack, operation),    { .n = matroska_track_operation } },
    { MATROSKA_ID_TRACKCONTENTENCODINGS, EBML_NEST,  0, 0,                                     { .n = matroska_track_encodings } },
    { MATROSKA_ID_TRACKMAXBLKADDID,      EBML_UINT,  0, offsetof(MatroskaTrack, max_block_additional_id) },
    { MATROSKA_ID_SEEKPREROLL,           EBML_UINT,  0, offsetof(MatroskaTrack, seek_preroll) },
    { MATROSKA_ID_TRACKFLAGENABLED,      EBML_NONE },
    { MATROSKA_ID_TRACKFLAGLACING,       EBML_NONE },
    { MATROSKA_ID_CODECNAME,             EBML_NONE },
    { MATROSKA_ID_CODECDECODEALL,        EBML_NONE },
    { MATROSKA_ID_CODECINFOURL,          EBML_NONE },
    { MATROSKA_ID_CODECDOWNLOADURL,      EBML_NONE },
    { MATROSKA_ID_TRACKMINCACHE,         EBML_NONE },
    { MATROSKA_ID_TRACKMAXCACHE,         EBML_NONE },
    CHILD_OF(matroska_tracks)
};

static EbmlSyntax matroska_tracks[] = {
    { MATROSKA_ID_TRACKENTRY, EBML_NEST, sizeof(MatroskaTrack), offsetof(MatroskaDemuxContext, tracks), { .n = matroska_track } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_attachment[] = {
    { MATROSKA_ID_FILEUID,      EBML_UINT, 0, offsetof(MatroskaAttachment, uid) },
    { MATROSKA_ID_FILENAME,     EBML_UTF8, 0, offsetof(MatroskaAttachment, filename) },
    { MATROSKA_ID_FILEMIMETYPE, EBML_STR,  0, offsetof(MatroskaAttachment, mime) },
    { MATROSKA_ID_FILEDATA,     EBML_BIN,  0, offsetof(MatroskaAttachment, bin) },
    { MATROSKA_ID_FILEDESC,     EBML_NONE },
    CHILD_OF(matroska_attachments)
};

static EbmlSyntax matroska_attachments[] = {
    { MATROSKA_ID_ATTACHEDFILE, EBML_NEST, sizeof(MatroskaAttachment), offsetof(MatroskaDemuxContext, attachments), { .n = matroska_attachment } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_chapter_display[] = {
    { MATROSKA_ID_CHAPSTRING,  EBML_UTF8, 0, offsetof(MatroskaChapter, title) },
    { MATROSKA_ID_CHAPLANG,    EBML_NONE },
    { MATROSKA_ID_CHAPCOUNTRY, EBML_NONE },
    CHILD_OF(matroska_chapter_entry)
};

static EbmlSyntax matroska_chapter_entry[] = {
    { MATROSKA_ID_CHAPTERTIMESTART,   EBML_UINT, 0, offsetof(MatroskaChapter, start), { .u = AV_NOPTS_VALUE } },
    { MATROSKA_ID_CHAPTERTIMEEND,     EBML_UINT, 0, offsetof(MatroskaChapter, end),   { .u = AV_NOPTS_VALUE } },
    { MATROSKA_ID_CHAPTERUID,         EBML_UINT, 0, offsetof(MatroskaChapter, uid) },
    { MATROSKA_ID_CHAPTERDISPLAY,     EBML_NEST, 0,                        0,         { .n = matroska_chapter_display } },
    { MATROSKA_ID_CHAPTERFLAGHIDDEN,  EBML_NONE },
    { MATROSKA_ID_CHAPTERFLAGENABLED, EBML_NONE },
    { MATROSKA_ID_CHAPTERPHYSEQUIV,   EBML_NONE },
    { MATROSKA_ID_CHAPTERATOM,        EBML_NONE },
    CHILD_OF(matroska_chapter)
};

static EbmlSyntax matroska_chapter[] = {
    { MATROSKA_ID_CHAPTERATOM,        EBML_NEST, sizeof(MatroskaChapter), offsetof(MatroskaDemuxContext, chapters), { .n = matroska_chapter_entry } },
    { MATROSKA_ID_EDITIONUID,         EBML_NONE },
    { MATROSKA_ID_EDITIONFLAGHIDDEN,  EBML_NONE },
    { MATROSKA_ID_EDITIONFLAGDEFAULT, EBML_NONE },
    { MATROSKA_ID_EDITIONFLAGORDERED, EBML_NONE },
    CHILD_OF(matroska_chapters)
};

static EbmlSyntax matroska_chapters[] = {
    { MATROSKA_ID_EDITIONENTRY, EBML_NEST, 0, 0, { .n = matroska_chapter } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_index_pos[] = {
    { MATROSKA_ID_CUETRACK,           EBML_UINT, 0, offsetof(MatroskaIndexPos, track) },
    { MATROSKA_ID_CUECLUSTERPOSITION, EBML_UINT, 0, offsetof(MatroskaIndexPos, pos) },
    { MATROSKA_ID_CUERELATIVEPOSITION,EBML_NONE },
    { MATROSKA_ID_CUEDURATION,        EBML_NONE },
    { MATROSKA_ID_CUEBLOCKNUMBER,     EBML_NONE },
    CHILD_OF(matroska_index_entry)
};

static EbmlSyntax matroska_index_entry[] = {
    { MATROSKA_ID_CUETIME,          EBML_UINT, 0,                        offsetof(MatroskaIndex, time) },
    { MATROSKA_ID_CUETRACKPOSITION, EBML_NEST, sizeof(MatroskaIndexPos), offsetof(MatroskaIndex, pos), { .n = matroska_index_pos } },
    CHILD_OF(matroska_index)
};

static EbmlSyntax matroska_index[] = {
    { MATROSKA_ID_POINTENTRY, EBML_NEST, sizeof(MatroskaIndex), offsetof(MatroskaDemuxContext, index), { .n = matroska_index_entry } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_simpletag[] = {
    { MATROSKA_ID_TAGNAME,        EBML_UTF8, 0,                   offsetof(MatroskaTag, name) },
    { MATROSKA_ID_TAGSTRING,      EBML_UTF8, 0,                   offsetof(MatroskaTag, string) },
    { MATROSKA_ID_TAGLANG,        EBML_STR,  0,                   offsetof(MatroskaTag, lang), { .s = "und" } },
    { MATROSKA_ID_TAGDEFAULT,     EBML_UINT, 0,                   offsetof(MatroskaTag, def) },
    { MATROSKA_ID_TAGDEFAULT_BUG, EBML_UINT, 0,                   offsetof(MatroskaTag, def) },
    { MATROSKA_ID_SIMPLETAG,      EBML_NEST, sizeof(MatroskaTag), offsetof(MatroskaTag, sub),  { .n = matroska_simpletag } },
    CHILD_OF(matroska_tag)
};

static EbmlSyntax matroska_tagtargets[] = {
    { MATROSKA_ID_TAGTARGETS_TYPE,       EBML_STR,  0, offsetof(MatroskaTagTarget, type) },
    { MATROSKA_ID_TAGTARGETS_TYPEVALUE,  EBML_UINT, 0, offsetof(MatroskaTagTarget, typevalue), { .u = 50 } },
    { MATROSKA_ID_TAGTARGETS_TRACKUID,   EBML_UINT, 0, offsetof(MatroskaTagTarget, trackuid) },
    { MATROSKA_ID_TAGTARGETS_CHAPTERUID, EBML_UINT, 0, offsetof(MatroskaTagTarget, chapteruid) },
    { MATROSKA_ID_TAGTARGETS_ATTACHUID,  EBML_UINT, 0, offsetof(MatroskaTagTarget, attachuid) },
    CHILD_OF(matroska_tag)
};

static EbmlSyntax matroska_tag[] = {
    { MATROSKA_ID_SIMPLETAG,  EBML_NEST, sizeof(MatroskaTag), offsetof(MatroskaTags, tag),    { .n = matroska_simpletag } },
    { MATROSKA_ID_TAGTARGETS, EBML_NEST, 0,                   offsetof(MatroskaTags, target), { .n = matroska_tagtargets } },
    CHILD_OF(matroska_tags)
};

static EbmlSyntax matroska_tags[] = {
    { MATROSKA_ID_TAG, EBML_NEST, sizeof(MatroskaTags), offsetof(MatroskaDemuxContext, tags), { .n = matroska_tag } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_seekhead_entry[] = {
    { MATROSKA_ID_SEEKID,       EBML_UINT, 0, offsetof(MatroskaSeekhead, id) },
    { MATROSKA_ID_SEEKPOSITION, EBML_UINT, 0, offsetof(MatroskaSeekhead, pos), { .u = -1 } },
    CHILD_OF(matroska_seekhead)
};

static EbmlSyntax matroska_seekhead[] = {
    { MATROSKA_ID_SEEKENTRY, EBML_NEST, sizeof(MatroskaSeekhead), offsetof(MatroskaDemuxContext, seekhead), { .n = matroska_seekhead_entry } },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_segment[] = {
    { MATROSKA_ID_CLUSTER,     EBML_STOP },
    { MATROSKA_ID_INFO,        EBML_LEVEL1, 0, 0, { .n = matroska_info } },
    { MATROSKA_ID_TRACKS,      EBML_LEVEL1, 0, 0, { .n = matroska_tracks } },
    { MATROSKA_ID_ATTACHMENTS, EBML_LEVEL1, 0, 0, { .n = matroska_attachments } },
    { MATROSKA_ID_CHAPTERS,    EBML_LEVEL1, 0, 0, { .n = matroska_chapters } },
    { MATROSKA_ID_CUES,        EBML_LEVEL1, 0, 0, { .n = matroska_index } },
    { MATROSKA_ID_TAGS,        EBML_LEVEL1, 0, 0, { .n = matroska_tags } },
    { MATROSKA_ID_SEEKHEAD,    EBML_LEVEL1, 0, 0, { .n = matroska_seekhead } },
    { 0 }   /* We don't want to go back to level 0, so don't add the parent. */
};

static EbmlSyntax matroska_segments[] = {
    { MATROSKA_ID_SEGMENT, EBML_NEST, 0, 0, { .n = matroska_segment } },
    { 0 }
};

static EbmlSyntax matroska_blockmore[] = {
    { MATROSKA_ID_BLOCKADDID,      EBML_UINT, 0, offsetof(MatroskaBlock,additional_id) },
    { MATROSKA_ID_BLOCKADDITIONAL, EBML_BIN,  0, offsetof(MatroskaBlock,additional) },
    CHILD_OF(matroska_blockadditions)
};

static EbmlSyntax matroska_blockadditions[] = {
    { MATROSKA_ID_BLOCKMORE, EBML_NEST, 0, 0, {.n = matroska_blockmore} },
    CHILD_OF(matroska_blockgroup)
};

static EbmlSyntax matroska_blockgroup[] = {
    { MATROSKA_ID_BLOCK,          EBML_BIN,  0, offsetof(MatroskaBlock, bin) },
    { MATROSKA_ID_BLOCKADDITIONS, EBML_NEST, 0, 0, { .n = matroska_blockadditions} },
    { MATROSKA_ID_BLOCKDURATION,  EBML_UINT, 0, offsetof(MatroskaBlock, duration) },
    { MATROSKA_ID_DISCARDPADDING, EBML_SINT, 0, offsetof(MatroskaBlock, discard_padding) },
    { MATROSKA_ID_BLOCKREFERENCE, EBML_SINT, 0, offsetof(MatroskaBlock, reference), { .i = INT64_MIN } },
    { MATROSKA_ID_CODECSTATE,     EBML_NONE },
    {                          1, EBML_UINT, 0, offsetof(MatroskaBlock, non_simple), { .u = 1 } },
    CHILD_OF(matroska_cluster_parsing)
};

// The following array contains SimpleBlock and BlockGroup twice
// in order to reuse the other values for matroska_cluster_enter.
static EbmlSyntax matroska_cluster_parsing[] = {
    { MATROSKA_ID_SIMPLEBLOCK,     EBML_BIN,  0, offsetof(MatroskaBlock, bin) },
    { MATROSKA_ID_BLOCKGROUP,      EBML_NEST, 0, 0, { .n = matroska_blockgroup } },
    { MATROSKA_ID_CLUSTERTIMECODE, EBML_UINT, 0, offsetof(MatroskaCluster, timecode) },
    { MATROSKA_ID_SIMPLEBLOCK,     EBML_STOP },
    { MATROSKA_ID_BLOCKGROUP,      EBML_STOP },
    { MATROSKA_ID_CLUSTERPOSITION, EBML_NONE },
    { MATROSKA_ID_CLUSTERPREVSIZE, EBML_NONE },
    CHILD_OF(matroska_segment)
};

static EbmlSyntax matroska_cluster_enter[] = {
    { MATROSKA_ID_CLUSTER,     EBML_NEST, 0, 0, { .n = &matroska_cluster_parsing[2] } },
    { 0 }
};
