/*
 * Matroska file semantic header
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
 * @see specs available on the Matroska project page: http://www.matroska.org/
 */

#ifndef AVFORMAT_MATROSKASEM_H
#define AVFORMAT_MATROSKASEM_H

#include "config.h"

#include <inttypes.h>

#include "matroska.h"
#include "avformat.h"
#include "libavutil/frame.h"

typedef enum {
    EBML_NONE,
    EBML_UINT,
    EBML_SINT,
    EBML_FLOAT,
    EBML_STR,
    EBML_UTF8,
    EBML_BIN,
    EBML_NEST,
    EBML_LEVEL1,
    EBML_STOP,
    EBML_TYPE_COUNT
} EbmlType;

typedef const struct EbmlSyntax {
    uint32_t id;
    EbmlType type;
    size_t list_elem_size;
    size_t data_offset;
    union {
        int64_t     i;
        uint64_t    u;
        double      f;
        const char *s;
        const struct EbmlSyntax *n;
    } def;
} EbmlSyntax;

typedef struct EbmlList {
    int nb_elem;
    unsigned int alloc_elem_size;
    void *elem;
} EbmlList;

typedef struct EbmlBin {
    int      size;
    AVBufferRef *buf;
    uint8_t *data;
    int64_t  pos;
} EbmlBin;

typedef struct Ebml {
    uint64_t version;
    uint64_t max_size;
    uint64_t id_length;
    char    *doctype;
    uint64_t doctype_version;
} Ebml;

typedef struct MatroskaTrackCompression {
    uint64_t algo;
    EbmlBin  settings;
} MatroskaTrackCompression;

typedef struct MatroskaTrackEncryption {
    uint64_t algo;
    EbmlBin  key_id;
} MatroskaTrackEncryption;

typedef struct MatroskaTrackEncoding {
    uint64_t scope;
    uint64_t type;
    MatroskaTrackCompression compression;
    MatroskaTrackEncryption encryption;
} MatroskaTrackEncoding;

typedef struct MatroskaMasteringMeta {
    double r_x;
    double r_y;
    double g_x;
    double g_y;
    double b_x;
    double b_y;
    double white_x;
    double white_y;
    double max_luminance;
    double min_luminance;
} MatroskaMasteringMeta;

typedef struct MatroskaTrackVideoColor {
    uint64_t matrix_coefficients;
    uint64_t bits_per_channel;
    uint64_t chroma_sub_horz;
    uint64_t chroma_sub_vert;
    uint64_t cb_sub_horz;
    uint64_t cb_sub_vert;
    uint64_t chroma_siting_horz;
    uint64_t chroma_siting_vert;
    uint64_t range;
    uint64_t transfer_characteristics;
    uint64_t primaries;
    uint64_t max_cll;
    uint64_t max_fall;
    MatroskaMasteringMeta mastering_meta;
} MatroskaTrackVideoColor;

typedef struct MatroskaTrackVideoProjection {
    uint64_t type;
    EbmlBin private;
    double yaw;
    double pitch;
    double roll;
} MatroskaTrackVideoProjection;

typedef struct MatroskaTrackVideo {
    double   frame_rate;
    uint64_t display_width;
    uint64_t display_height;
    uint64_t pixel_width;
    uint64_t pixel_height;
    EbmlBin  color_space;
    uint64_t display_unit;
    uint64_t interlaced;
    uint64_t field_order;
    uint64_t stereo_mode;
    uint64_t alpha_mode;
    EbmlList color;
    MatroskaTrackVideoProjection projection;
} MatroskaTrackVideo;

typedef struct MatroskaTrackAudio {
    double   samplerate;
    double   out_samplerate;
    uint64_t bitdepth;
    uint64_t channels;

    /* real audio header (extracted from extradata) */
    int      coded_framesize;
    int      sub_packet_h;
    int      frame_size;
    int      sub_packet_size;
    int      sub_packet_cnt;
    int      pkt_cnt;
    uint64_t buf_timecode;
    uint8_t *buf;
} MatroskaTrackAudio;

typedef struct MatroskaTrackPlane {
    uint64_t uid;
    uint64_t type;
} MatroskaTrackPlane;

typedef struct MatroskaTrackOperation {
    EbmlList combine_planes;
} MatroskaTrackOperation;

typedef struct MatroskaTrack {
    uint64_t num;
    uint64_t uid;
    uint64_t type;
    char    *name;
    char    *codec_id;
    EbmlBin  codec_priv;
    char    *language;
    double time_scale;
    uint64_t default_duration;
    uint64_t flag_default;
    uint64_t flag_forced;
    uint64_t seek_preroll;
    MatroskaTrackVideo video;
    MatroskaTrackAudio audio;
    MatroskaTrackOperation operation;
    EbmlList encodings;
    uint64_t codec_delay;
    uint64_t codec_delay_in_track_tb;

    AVStream *stream;
    int64_t end_timecode;
    int ms_compat;
    uint64_t max_block_additional_id;

    uint32_t palette[AVPALETTE_COUNT];
    int has_palette;
} MatroskaTrack;

typedef struct MatroskaAttachment {
    uint64_t uid;
    char *filename;
    char *mime;
    EbmlBin bin;

    AVStream *stream;
} MatroskaAttachment;

typedef struct MatroskaChapter {
    uint64_t start;
    uint64_t end;
    uint64_t uid;
    char    *title;

    AVChapter *chapter;
} MatroskaChapter;

typedef struct MatroskaIndexPos {
    uint64_t track;
    uint64_t pos;
} MatroskaIndexPos;

typedef struct MatroskaIndex {
    uint64_t time;
    EbmlList pos;
} MatroskaIndex;

typedef struct MatroskaTag {
    char *name;
    char *string;
    char *lang;
    uint64_t def;
    EbmlList sub;
} MatroskaTag;

typedef struct MatroskaTagTarget {
    char    *type;
    uint64_t typevalue;
    uint64_t trackuid;
    uint64_t chapteruid;
    uint64_t attachuid;
} MatroskaTagTarget;

typedef struct MatroskaTags {
    MatroskaTagTarget target;
    EbmlList tag;
} MatroskaTags;

typedef struct MatroskaSeekhead {
    uint64_t id;
    uint64_t pos;
} MatroskaSeekhead;

typedef struct MatroskaLevel {
    uint64_t start;
    uint64_t length;
} MatroskaLevel;

typedef struct MatroskaBlock {
    uint64_t duration;
    int64_t  reference;
    uint64_t non_simple;
    EbmlBin  bin;
    uint64_t additional_id;
    EbmlBin  additional;
    int64_t  discard_padding;
} MatroskaBlock;

typedef struct MatroskaCluster {
    MatroskaBlock block;
    uint64_t timecode;
    int64_t pos;
} MatroskaCluster;

typedef struct MatroskaLevel1Element {
    int64_t  pos;
    uint32_t id;
    int parsed;
} MatroskaLevel1Element;

typedef struct MatroskaDemuxContext {
    const AVClass *class;
    AVFormatContext *ctx;

    /* EBML stuff */
    MatroskaLevel levels[EBML_MAX_DEPTH];
    int      num_levels;
    uint32_t current_id;
    int64_t  resync_pos;
    int      unknown_count;

    uint64_t time_scale;
    double   duration;
    char    *title;
    char    *muxingapp;
    EbmlBin  date_utc;
    EbmlList tracks;
    EbmlList attachments;
    EbmlList chapters;
    EbmlList index;
    EbmlList tags;
    EbmlList seekhead;

    /* byte position of the segment inside the stream */
    int64_t segment_start;

    /* the packet queue */
    AVPacketList *queue;
    AVPacketList *queue_end;

    int done;

    /* What to skip before effectively reading a packet. */
    int skip_to_keyframe;
    uint64_t skip_to_timecode;

    /* File has a CUES element, but we defer parsing until it is needed. */
    int cues_parsing_deferred;

    /* Level1 elements and whether they were read yet */
    MatroskaLevel1Element level1_elems[64];
    int num_level1_elems;

    MatroskaCluster current_cluster;

    /* WebM DASH Manifest live flag */
    int is_live;

    /* Bandwidth value for WebM DASH Manifest */
    int bandwidth;
} MatroskaDemuxContext;

// The following forward declarations need their size because
// a tentative definition with internal linkage must not be an
// incomplete type (6.7.2 in C90, 6.9.2 in C99).
// Removing the sizes breaks MSVC.
static EbmlSyntax ebml_syntax[3], matroska_segment[9], matroska_track_video_color[15], matroska_track_video[19],
                  matroska_track[27], matroska_track_encoding[6], matroska_track_encodings[2],
                  matroska_track_combine_planes[2], matroska_track_operation[2], matroska_tracks[2],
                  matroska_attachments[2], matroska_chapter_entry[9], matroska_chapter[6], matroska_chapters[2],
                  matroska_index_entry[3], matroska_index[2], matroska_tag[3], matroska_tags[2], matroska_seekhead[2],
                  matroska_blockadditions[2], matroska_blockgroup[8], matroska_cluster_parsing[8];


#endif /* AVFORMAT_MATROSKASEM_H */
