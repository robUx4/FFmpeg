<?xml version="1.0"?>
<!--
    File used to generate matroska_ids.h from ebml_matroska.xml or matroska_xsd.xml (the cleaned normative version)
    Usage: xsltproc -o matroskasem.c schema_2_lavf_sem_c.xsl ebml_matroska.xml
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" 
    xmlns:str="http://exslt.org/strings"
    exclude-result-prefixes="str xhtml ebml"
    xmlns:xhtml="http://www.w3.org/1999/xhtml"
    xmlns="https://ietf.org/cellar/ebml" xmlns:ebml="https://ietf.org/cellar/ebml">
  <xsl:output encoding="UTF-8" method="text" version="1.0" indent="yes" />

  <xsl:template match="ebml:EBMLSchema">
<xsl:text>/*
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
 * @author Ronald Bultje &lt;rbultje@ronald.bitfreak.net&gt;
 * @author with a little help from Moritz Bunkus &lt;moritz@bunkus.org&gt;
 * @author totally reworked by Aurelien Jacobs &lt;aurel@gnuage.org&gt;
 * @author Split from decoder by Steve Lhomme &lt;slhomme@matroska.org&gt;
 * @see specs available on the Matroska project page: http://www.matroska.org/
 */

#include "config.h"

#include &lt;inttypes.h&gt;

#include "matroskasem.h"

#define CHILD_OF(parent) { .def = { .n = parent } }

static EbmlSyntax ebml_header[] = {
    { EBML_ID_EBMLREADVERSION,    EBML_UINT, 0, offsetof(Ebml, version),         { .u = EBML_VERSION } },
    { EBML_ID_EBMLMAXSIZELENGTH,  EBML_UINT, 0, offsetof(Ebml, max_size),        { .u = 8 } },
    { EBML_ID_EBMLMAXIDLENGTH,    EBML_UINT, 0, offsetof(Ebml, id_length),       { .u = 4 } },
    { EBML_ID_DOCTYPE,            EBML_STR,  0, offsetof(Ebml, doctype),         { .s = "(none)" } },
    { EBML_ID_DOCTYPEREADVERSION, EBML_UINT, 0, offsetof(Ebml, doctype_version), { .u = 1 } },
    { EBML_ID_EBMLVERSION,        EBML_NONE,   },
    { EBML_ID_DOCTYPEVERSION,     EBML_NONE,   },
    CHILD_OF(ebml_syntax)
};

static EbmlSyntax ebml_syntax[] = {
    { EBML_ID_HEADER,      EBML_NEST, 0, 0, { .n = ebml_header } },
    { MATROSKA_ID_SEGMENT, EBML_STOP,   },
    { 0 }
};
</xsl:text>

    <xsl:apply-templates select="ebml:element[@name='Segment']"/>
    <!-- TODO even closer ordering with a "sort score"  https://stackoverflow.com/questions/1287651/xslt-custom-sort -->

    <xsl:for-each select="ebml:element">
        <!-- <Parent path>/<id> -->
        <xsl:sort select="concat(
            substring( @path, 1, string-length(@path)-string-length(@name) ),
            @id
        )" />

    </xsl:for-each>

<xsl:text>
static EbmlSyntax matroska_segments[] = {
    { MATROSKA_ID_SEGMENT, EBML_NEST, 0, 0, { .n = matroska_segment } },
    { 0 }
};

static EbmlSyntax matroska_cluster_enter[] = {
    { MATROSKA_ID_CLUSTER,     EBML_NEST, 0, 0, { .n = &amp;matroska_cluster_parsing[2] } },
    { 0 }
};
</xsl:text>

  </xsl:template>

    <xsl:template match="ebml:element">
        <xsl:call-template name="parsePath">
            <xsl:with-param name="node" select="."/>
        </xsl:call-template>
    </xsl:template>

    <xsl:template name="parsePath">
        <xsl:param name="node"/>

        <!-- Master element comment header -->
        <xsl:if test="@type='master'">
            <xsl:text>&#10;static EbmlSyntax matroska_</xsl:text>
            <xsl:call-template name="masterListName">
                <xsl:with-param name="node" select="$node"/>
            </xsl:call-template>
            <xsl:text>[] = {&#10;</xsl:text>

            <xsl:for-each select="/ebml:EBMLSchema/ebml:element[@path = concat(concat($node/@path, '\'), @name)]">
                <xsl:sort select="not(@name='Info')" />
                <xsl:sort select="not(@name='Tracks')" />
                <xsl:sort select="not(@name='Cues')" />
                <xsl:sort select="not(@name='Tags')" />
                <xsl:sort select="@name='Chapters'" />
                <xsl:sort select="@name='Audio'" />
                <xsl:sort select="string-length(@id)" />
                <xsl:sort select="@id" />
                <xsl:variable name="lavfName">
                    <!-- Transform the ebml_matroska.xml name into the libavformat name -->
                    <xsl:choose>
                        <xsl:when test="@name='FileDescription'"><xsl:text>FileDesc</xsl:text></xsl:when>
                        <xsl:when test="@name='ChapLanguage'"><xsl:text>ChapLang</xsl:text></xsl:when>
                        <xsl:when test="@name='ReferenceBlock'"><xsl:text>BlockReference</xsl:text></xsl:when>
                        <xsl:when test="@name='Position'"><xsl:text>ClusterPosition</xsl:text></xsl:when>
                        <xsl:when test="@name='PrevSize'"><xsl:text>ClusterPrevSize</xsl:text></xsl:when>
                        <xsl:when test="@name='Timestamp'"><xsl:text>ClusterTimecode</xsl:text></xsl:when>
                        <xsl:when test="@name='CuePoint'"><xsl:text>PointEntry</xsl:text></xsl:when>
                        <xsl:when test="@name='CueTrackPositions'"><xsl:text>CueTrackPosition</xsl:text></xsl:when>
                        <xsl:when test="@name='TimestampScale'"><xsl:text>TimecodeScale</xsl:text></xsl:when>
                        <xsl:when test="@name='Seek'"><xsl:text>SeekEntry</xsl:text></xsl:when>
                        <xsl:when test="@name='TagLanguage'"><xsl:text>TagLang</xsl:text></xsl:when>
                        <xsl:when test="@name='Targets'"><xsl:text>TagTargets</xsl:text></xsl:when>
                        <xsl:when test="@name='TagAttachmentUID'"><xsl:text>TagTargets_AttachUID</xsl:text></xsl:when>
                        <xsl:when test="@name='TagChapterUID'"><xsl:text>TagTargets_ChapterUID</xsl:text></xsl:when>
                        <xsl:when test="@name='TagTrackUID'"><xsl:text>TagTargets_TrackUID</xsl:text></xsl:when>
                        <xsl:when test="@name='TargetType'"><xsl:text>TagTargets_Type</xsl:text></xsl:when>
                        <xsl:when test="@name='TargetTypeValue'"><xsl:text>TagTargets_TypeValue</xsl:text></xsl:when>
                        <xsl:when test="@name='ChapterPhysicalEquiv'"><xsl:text>ChapterPhysEquiv</xsl:text></xsl:when>
                        <xsl:when test="@name='OutputSamplingFrequency'"><xsl:text>AudioOutSamplingFreq</xsl:text></xsl:when>
                        <xsl:when test="@name='SamplingFrequency'"><xsl:text>AudioSamplingFreq</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodings'"><xsl:text>TrackContentEncodings</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncoding'"><xsl:text>TrackContentEncoding</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompression'"><xsl:text>EncodingCompression</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompAlgo'"><xsl:text>EncodingCompAlgo</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompSettings'"><xsl:text>EncodingCompSettings</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodingOrder'"><xsl:text>EncodingOrder</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodingScope'"><xsl:text>EncodingScope</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodingType'"><xsl:text>EncodingType</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncryption'"><xsl:text>EncodingEncryption</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncAESSettings'"><xsl:text>EncodingEncAESSettings</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncAlgo'"><xsl:text>EncodingEncAlgo</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncKeyID'"><xsl:text>EncodingEncKeyId</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentSigAlgo'"><xsl:text>EncodingSigAlgo</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentSignature'"><xsl:text>EncodingSignature</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentSigKeyID'"><xsl:text>EncodingSigKeyId</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentSigHashAlgo'"><xsl:text>EncodingSigHashAlgo</xsl:text></xsl:when>
                        <xsl:when test="@name='MaxBlockAdditionID'"><xsl:text>TrackMaxBlkAddID</xsl:text></xsl:when>
                        <xsl:when test="@name='SeekPreRoll'"><xsl:text>SeekPreRoll</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackTimestampScale'"><xsl:text>TrackTimecodeScale</xsl:text></xsl:when>
                        <xsl:when test="@name='AspectRatioType'"><xsl:text>VideoAspectRatio</xsl:text></xsl:when>
                        <xsl:when test="@name='Colour'"><xsl:text>VideoColor</xsl:text></xsl:when>
                        <xsl:when test="@name='ColourSpace'"><xsl:text>VideoColorSpace</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSubsamplingHorz'"><xsl:text>VideoColorChromaSubHorz</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSubsamplingVert'"><xsl:text>VideoColorChromaSubVert</xsl:text></xsl:when>
                        <xsl:when test="@name='CbSubsamplingHorz'"><xsl:text>VideoColorCbSubHorz</xsl:text></xsl:when>
                        <xsl:when test="@name='CbSubsamplingVert'"><xsl:text>VideoColorCbSubVert</xsl:text></xsl:when>
                        <xsl:when test="@name='MasteringMetadata'"><xsl:text>VideoColorMasteringMeta</xsl:text></xsl:when>
                        <xsl:when test="@name='MatrixCoefficients'"><xsl:text>VideoColorMatrixCoeff</xsl:text></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Audio\')"><xsl:text>Audio</xsl:text><xsl:value-of select="@name"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\PixelCrop')"><xsl:text>VideoPixelCrop</xsl:text><xsl:value-of select="substring(@name, 10, 1)"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\Colour\MasteringMetadata\Primary')"><xsl:text>VideoColor_</xsl:text><xsl:value-of select="substring(@name, 8, 1)"/><xsl:value-of select="substring(@name, string-length(@name))"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\Colour\MasteringMetadata\WhitePointChromaticity')"><xsl:text>VideoColor_WHITE</xsl:text><xsl:value-of select="substring(@name, string-length(@name))"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\Colour\MasteringMetadata\Luminance')"><xsl:text>VideoColor_</xsl:text><xsl:value-of select="@name"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\Colour\')"><xsl:text>VideoColor</xsl:text><xsl:value-of select="@name"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\Video\')"><xsl:text>Video</xsl:text><xsl:value-of select="@name"/></xsl:when>
                        <xsl:when test="contains(@path,'\TrackEntry\') and not(contains(@name,'Track')) and not(contains(@name,'Codec'))"><xsl:text>Track</xsl:text><xsl:value-of select="@name"/></xsl:when>
                        <xsl:otherwise><xsl:value-of select="@name"/></xsl:otherwise>
                    </xsl:choose>
                </xsl:variable>
                <xsl:variable name="lavfNameUpper">
                    <xsl:value-of select="translate($lavfName, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ')"/>
                </xsl:variable>
                <!-- Storage name in a structure if any -->
                <xsl:variable name="lavfStorage">
                    <xsl:choose>
                        <xsl:when test="@name='DateUTC'"><xsl:text>date_utc</xsl:text></xsl:when>
                        <xsl:when test="@name='Duration'"><xsl:text>duration</xsl:text></xsl:when>
                        <xsl:when test="@name='MuxingApp'"><xsl:text>muxingapp</xsl:text></xsl:when>
                        <xsl:when test="@name='Title'"><xsl:text>title</xsl:text></xsl:when>
                        <xsl:when test="@name='Cluster'"><xsl:text>STOP</xsl:text></xsl:when>
                        <xsl:when test="@name='TimestampScale'"><xsl:text>time_scale</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackEntry'"><xsl:text>tracks</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackNumber'"><xsl:text>num</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackName'"><xsl:text>name</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackUID'"><xsl:text>uid</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackType'"><xsl:text>type</xsl:text></xsl:when>
                        <xsl:when test="@name='CodecID'"><xsl:text>codec_id</xsl:text></xsl:when>
                        <xsl:when test="@name='FlagDefault'"><xsl:text>flag_default</xsl:text></xsl:when>
                        <xsl:when test="@name='FlagForced'"><xsl:text>flag_forced</xsl:text></xsl:when>
                        <xsl:when test="@name='CodecDelay'"><xsl:text>codec_delay</xsl:text></xsl:when>
                        <xsl:when test="@name='CodecPrivate'"><xsl:text>codec_priv</xsl:text></xsl:when>
                        <xsl:when test="@name='SeekPreRoll'"><xsl:text>seek_preroll</xsl:text></xsl:when>
                        <xsl:when test="@name='Video'"><xsl:text>video</xsl:text></xsl:when>
                        <xsl:when test="@name='Audio'"><xsl:text>audio</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackOperation'"><xsl:text>operation</xsl:text></xsl:when>
                        <xsl:when test="@name='Name'"><xsl:text>name</xsl:text></xsl:when>
                        <xsl:when test="@name='MaxBlockAdditionID'"><xsl:text>max_block_additional_id</xsl:text></xsl:when>
                        <xsl:when test="@name='Language'"><xsl:text>language</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackTimestampScale'"><xsl:text>time_scale</xsl:text></xsl:when>
                        <xsl:when test="@name='DefaultDuration'"><xsl:text>default_duration</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncoding'"><xsl:text>encodings</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodingScope'"><xsl:text>scope</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncodingType'"><xsl:text>type</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompression'"><xsl:text>compression</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncryption'"><xsl:text>encryption</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompAlgo'"><xsl:text>algo</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentCompSettings'"><xsl:text>settings</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncAlgo'"><xsl:text>algo</xsl:text></xsl:when>
                        <xsl:when test="@name='ContentEncKeyID'"><xsl:text>key_id</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackPlane'"><xsl:text>combine_planes</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackPlaneUID'"><xsl:text>uid</xsl:text></xsl:when>
                        <xsl:when test="@name='TrackPlaneType'"><xsl:text>type</xsl:text></xsl:when>
                        <xsl:when test="@name='FlagInterlaced'"><xsl:text>interlaced</xsl:text></xsl:when>
                        <xsl:when test="@name='FieldOrder'"><xsl:text>field_order</xsl:text></xsl:when>
                        <xsl:when test="@name='PixelWidth'"><xsl:text>pixel_width</xsl:text></xsl:when>
                        <xsl:when test="@name='PixelHeight'"><xsl:text>pixel_height</xsl:text></xsl:when>
                        <xsl:when test="@name='StereoMode'"><xsl:text>stereo_mode</xsl:text></xsl:when>
                        <xsl:when test="@name='AlphaMode'"><xsl:text>alpha_mode</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayWidth'"><xsl:text>display_width</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayHeight'"><xsl:text>display_height</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayUnit'"><xsl:text>display_unit</xsl:text></xsl:when>
                        <xsl:when test="@name='Colour'"><xsl:text>color</xsl:text></xsl:when>
                        <xsl:when test="@name='Projection'"><xsl:text>projection</xsl:text></xsl:when>
                        <xsl:when test="@name='FrameRate'"><xsl:text>frame_rate</xsl:text></xsl:when>
                        <xsl:when test="@name='ColourSpace'"><xsl:text>color_space</xsl:text></xsl:when>
                        <xsl:when test="@name='MatrixCoefficients'"><xsl:text>matrix_coefficients</xsl:text></xsl:when>
                        <xsl:when test="@name='BitsPerChannel'"><xsl:text>bits_per_channel</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSubsamplingHorz'"><xsl:text>chroma_sub_horz</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSubsamplingVert'"><xsl:text>chroma_sub_vert</xsl:text></xsl:when>
                        <xsl:when test="@name='CbSubsamplingHorz'"><xsl:text>cb_sub_horz</xsl:text></xsl:when>
                        <xsl:when test="@name='CbSubsamplingVert'"><xsl:text>cb_sub_vert</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSitingHorz'"><xsl:text>chroma_siting_horz</xsl:text></xsl:when>
                        <xsl:when test="@name='ChromaSitingVert'"><xsl:text>chroma_siting_vert</xsl:text></xsl:when>
                        <xsl:when test="@name='Range'"><xsl:text>range</xsl:text></xsl:when>
                        <xsl:when test="@name='TransferCharacteristics'"><xsl:text>transfer_characteristics</xsl:text></xsl:when>
                        <xsl:when test="@name='Primaries'"><xsl:text>primaries</xsl:text></xsl:when>
                        <xsl:when test="@name='MaxCLL'"><xsl:text>max_cll</xsl:text></xsl:when>
                        <xsl:when test="@name='MaxFALL'"><xsl:text>max_fall</xsl:text></xsl:when>
                        <xsl:when test="@name='MasteringMetadata'"><xsl:text>mastering_meta</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryRChromaticityX'"><xsl:text>r_x</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryRChromaticityY'"><xsl:text>r_y</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryGChromaticityX'"><xsl:text>g_x</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryGChromaticityY'"><xsl:text>g_y</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryBChromaticityX'"><xsl:text>b_x</xsl:text></xsl:when>
                        <xsl:when test="@name='PrimaryBChromaticityY'"><xsl:text>b_y</xsl:text></xsl:when>
                        <xsl:when test="@name='WhitePointChromaticityX'"><xsl:text>white_x</xsl:text></xsl:when>
                        <xsl:when test="@name='WhitePointChromaticityY'"><xsl:text>white_y</xsl:text></xsl:when>
                        <xsl:when test="@name='LuminanceMax'"><xsl:text>max_luminance</xsl:text></xsl:when>
                        <xsl:when test="@name='LuminanceMin'"><xsl:text>min_luminance</xsl:text></xsl:when>
                        <xsl:when test="@name='ProjectionType'"><xsl:text>type</xsl:text></xsl:when>
                        <xsl:when test="@name='ProjectionPrivate'"><xsl:text>private</xsl:text></xsl:when>
                        <xsl:when test="@name='ProjectionPoseYaw'"><xsl:text>yaw</xsl:text></xsl:when>
                        <xsl:when test="@name='ProjectionPosePitch'"><xsl:text>pitch</xsl:text></xsl:when>
                        <xsl:when test="@name='ProjectionPoseRoll'"><xsl:text>roll</xsl:text></xsl:when>
                        <xsl:when test="@name='Channels'"><xsl:text>channels</xsl:text></xsl:when>
                        <xsl:when test="@name='SamplingFrequency'"><xsl:text>samplerate</xsl:text></xsl:when>
                        <xsl:when test="@name='OutputSamplingFrequency'"><xsl:text>out_samplerate</xsl:text></xsl:when>
                        <xsl:when test="@name='BitDepth'"><xsl:text>bitdepth</xsl:text></xsl:when>
                        <xsl:when test="@name='CuePoint'"><xsl:text>index</xsl:text></xsl:when>
                        <xsl:when test="@name='CueTime'"><xsl:text>time</xsl:text></xsl:when>
                        <xsl:when test="@name='CueTrackPositions'"><xsl:text>pos</xsl:text></xsl:when>
                        <xsl:when test="@name='CueTime'"><xsl:text>time</xsl:text></xsl:when>
                        <xsl:when test="@name='CueClusterPosition'"><xsl:text>pos</xsl:text></xsl:when>
                        <xsl:when test="@name='CueTrack'"><xsl:text>track</xsl:text></xsl:when>
                        <xsl:when test="@name='Targets'"><xsl:text>target</xsl:text></xsl:when>
                        <xsl:when test="@name='SimpleTag'"><xsl:text>tag</xsl:text></xsl:when>
                        <xsl:when test="@name='AttachedFile'"><xsl:text>attachments</xsl:text></xsl:when>
                        <xsl:when test="@name='FileData'"><xsl:text>bin</xsl:text></xsl:when>
                        <xsl:when test="@name='FileMimeType'"><xsl:text>mime</xsl:text></xsl:when>
                        <xsl:when test="@name='FileName'"><xsl:text>filename</xsl:text></xsl:when>
                        <xsl:when test="@name='FileUID'"><xsl:text>uid</xsl:text></xsl:when>
                        <xsl:when test="@name='SimpleBlock'"><xsl:text>bin</xsl:text></xsl:when>
                        <xsl:when test="@name='Timestamp'"><xsl:text>timecode</xsl:text></xsl:when>
                        <xsl:when test="@name='BlockDuration'"><xsl:text>duration</xsl:text></xsl:when>
                        <xsl:when test="@name='Block'"><xsl:text>bin</xsl:text></xsl:when>
                        <xsl:when test="@name='ReferenceBlock'"><xsl:text>reference</xsl:text></xsl:when>
                        <xsl:when test="@name='DiscardPadding'"><xsl:text>discard_padding</xsl:text></xsl:when>
                        <xsl:when test="@name='Tag'"><xsl:text>tags</xsl:text></xsl:when>
                        <xsl:when test="@name='TagChapterUID'"><xsl:text>chapteruid</xsl:text></xsl:when>
                        <xsl:when test="@name='TagTrackUID'"><xsl:text>trackuid</xsl:text></xsl:when>
                        <xsl:when test="@name='TagAttachmentUID'"><xsl:text>attachuid</xsl:text></xsl:when>
                        <xsl:when test="@name='TargetType'"><xsl:text>type</xsl:text></xsl:when>
                        <xsl:when test="@name='TargetTypeValue'"><xsl:text>typevalue</xsl:text></xsl:when>
                        <xsl:when test="@name='Seek'"><xsl:text>seekhead</xsl:text></xsl:when>
                        <xsl:when test="@name='SeekID'"><xsl:text>id</xsl:text></xsl:when>
                        <xsl:when test="@name='SeekPosition'"><xsl:text>pos</xsl:text></xsl:when>
                        <xsl:when test="@name='ChapterAtom'"><xsl:text>chapters</xsl:text></xsl:when>
                        
                    </xsl:choose>
                </xsl:variable>

                <!-- Structure name for master elements stored in an EbmlList -->
                <xsl:variable name="lavfListElementSize">
                    <xsl:call-template name="ebmlListStructure">
                        <xsl:with-param name="node" select="."/>
                    </xsl:call-template>
                </xsl:variable>
                <!-- Default value to use -->
                <xsl:variable name="lavfDefault">
                    <xsl:choose>
                        <xsl:when test="@type='master'">
                            <xsl:text>matroska_</xsl:text>
                            <xsl:call-template name="masterListName">
                                <xsl:with-param name="node" select="."/>
                            </xsl:call-template>
                        </xsl:when>
                        <xsl:when test="@name='FlagInterlaced'"><xsl:text>MATROSKA_VIDEO_INTERLACE_FLAG_UNDETERMINED</xsl:text></xsl:when>
                        <xsl:when test="@name='FieldOrder'"><xsl:text>MATROSKA_VIDEO_FIELDORDER_UNDETERMINED</xsl:text></xsl:when>
                        <xsl:when test="@name='StereoMode'"><xsl:text>MATROSKA_VIDEO_STEREOMODE_TYPE_NB</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayWidth'"><xsl:text>-1</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayHeight'"><xsl:text>-1</xsl:text></xsl:when>
                        <xsl:when test="@name='DisplayUnit'"><xsl:text>MATROSKA_VIDEO_DISPLAYUNIT_PIXELS</xsl:text></xsl:when>
                        <xsl:when test="@default='0x1p+0'"><xsl:text>1.0</xsl:text></xsl:when>
                        <xsl:when test="@default='0x0p+0'"><xsl:text>0.0</xsl:text></xsl:when>
                        <xsl:when test="@default='0x1.f4p+12'"><xsl:text>8000.0</xsl:text></xsl:when>
                        <xsl:when test="@type='master'">
                            <xsl:value-of select="@default"/>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:if test="not($lavfStorage='')">
                                <xsl:value-of select="@default"/>
                            </xsl:if>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:variable>


                <!-- generate EbmlSyntax.id -->
                <xsl:text>    { MATROSKA_ID_</xsl:text>
                <xsl:choose>
                    <xsl:when test="string-length($lavfNameUpper) &lt; 18">
                        <xsl:value-of select="substring(concat($lavfNameUpper, ',                                   '),0,26)"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="$lavfNameUpper"/><xsl:text>,</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>
                <xsl:text> </xsl:text>

                <!-- generate EbmlSyntax.type -->
                <xsl:choose>
                    <xsl:when test="@name='Cluster'">
                        <xsl:text>EBML_STOP,   </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='master'">
                        <xsl:choose>
                            <xsl:when test="$node/@name='Segment'">
                                <xsl:text>EBML_LEVEL1, </xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:text>EBML_NEST,   </xsl:text>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>
                    <xsl:when test="$lavfStorage=''">
                        <xsl:text>EBML_NONE,   </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='uinteger'">
                        <xsl:text>EBML_UINT,   </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='integer'">
                        <xsl:text>EBML_SINT,   </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='utf-8'">
                        <xsl:text>EBML_UTF8,   </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='string'">
                        <xsl:text>EBML_STR,    </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='binary'">
                        <xsl:text>EBML_BIN,    </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='date'">
                        <xsl:text>EBML_BIN,    </xsl:text>
                    </xsl:when>
                    <xsl:when test="@type='float'">
                        <xsl:text>EBML_FLOAT,  </xsl:text>
                    </xsl:when>
                </xsl:choose>

                <!-- generate EbmlSyntax.list_elem_size -->
                <xsl:choose>
                    <xsl:when test="$lavfStorage='STOP'">
                    </xsl:when>
                    <xsl:when test="$lavfListElementSize='' and not($lavfDefault='')">
                        <xsl:text>0, </xsl:text>
                    </xsl:when>
                    <xsl:when test="$lavfListElementSize='' and $lavfStorage=''">
                    </xsl:when>
                    <xsl:when test="$lavfListElementSize='' and not($lavfStorage='')">
                        <xsl:text>0, </xsl:text>
                    </xsl:when>
                    <xsl:when test="$lavfListElementSize=''">
                    </xsl:when>
                    <xsl:when test="$lavfListElementSize">
                        <xsl:text>sizeof(</xsl:text>
                        <xsl:value-of select="$lavfListElementSize"/>
                        <xsl:text>), </xsl:text>
                    </xsl:when>
                </xsl:choose>

                <!-- generate EbmlSyntax.data_offset -->
                <xsl:choose>
                    <xsl:when test="$lavfStorage='STOP'">
                    </xsl:when>
                    <xsl:when test="$lavfStorage='' and not($lavfDefault='')">
                        <xsl:text>0</xsl:text>
                    </xsl:when>
                    <xsl:when test="$lavfStorage=''">
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:variable name="parentFullPath" select="substring(@path, 0, string-length(@path)-string-length(@name))"/>
                    
                        <!-- <xsl:variable name="parentNode" select="../ebml:element[@path = $parentFullPath]"/> -->
<!-- <xsl:value-of select="$parentFullPath"/> -->
<!-- <xsl:value-of select="$parentNode/@name"/> -->

                        <xsl:variable name="parentStructureFromList">
                            <xsl:call-template name="ebmlListStructure">
                                <xsl:with-param name="node" select="../ebml:element[@path = $parentFullPath]"/>
                            </xsl:call-template>
                        </xsl:variable>
                        <xsl:variable name="parentStructure">
                            <xsl:choose>
                                <xsl:when test="not($parentStructureFromList='')">
                                    <xsl:value-of select="$parentStructureFromList"/>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xsl:call-template name="hardcodedStructure">
                                        <xsl:with-param name="node" select="../ebml:element[@path = $parentFullPath]"/>
                                    </xsl:call-template>
                                </xsl:otherwise>
                            </xsl:choose>
                        </xsl:variable>
<!-- <xsl:value-of select="$parentStructure"/> -->

                        <xsl:text>offsetof(</xsl:text>
                        <xsl:value-of select="$parentStructure"/>
                        <xsl:text>, </xsl:text>
                        <xsl:value-of select="$lavfStorage"/>
                        <xsl:text>)</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>

                <!-- generate EbmlSyntax.def -->
                <xsl:choose>
                    <xsl:when test="$lavfStorage='STOP'">
                    </xsl:when>
                    <xsl:when test="$lavfDefault=''">
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:text>, { </xsl:text>
                        <xsl:choose>
                            <xsl:when test="@type='uinteger'">
                                <xsl:text>.u = </xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='integer'">
                                <xsl:text>.i = </xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='float'">
                                <xsl:text>.f = </xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='string'">
                                <xsl:text>.s = "</xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='utf-8'">
                                <xsl:text>.s = "</xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='master'">
                                <xsl:text>.n = </xsl:text>
                            </xsl:when>
                        </xsl:choose>
                        <xsl:value-of select="$lavfDefault"/>
                        <xsl:choose>
                            <xsl:when test="@type='string'">
                                <xsl:text>"</xsl:text>
                            </xsl:when>
                            <xsl:when test="@type='utf-8'">
                                <xsl:text>"</xsl:text>
                            </xsl:when>
                        </xsl:choose>
                        <xsl:text> }</xsl:text>
                    </xsl:otherwise>
                </xsl:choose>

                <xsl:text> },&#10;</xsl:text>
            </xsl:for-each>
            <xsl:choose>
                <xsl:when test="@name='Segment'">
                    <xsl:text>    { 0 }   /* We don't want to go back to level 0, so don't add the parent. */&#10;</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text>    CHILD_OF(matroska_</xsl:text>
                    <xsl:variable name="parentFullPath" select="substring(@path, 0, string-length(@path)-string-length(@name))"/>
                    <xsl:call-template name="masterListName">
                        <xsl:with-param name="node" select="../ebml:element[@path = $parentFullPath]"/>
                    </xsl:call-template>
                    <xsl:text>)&#10;</xsl:text>
                </xsl:otherwise>
            </xsl:choose>
            <xsl:text>};&#10;</xsl:text>

            <xsl:for-each select="../ebml:element[@path = concat(concat($node/@path, '\'), @name)]">
                <xsl:sort select="not(@name='Info')" />
                <xsl:sort select="not(@name='Tracks')" />
                <xsl:sort select="not(@name='Cues')" />
                <xsl:sort select="not(@name='Tags')" />
                <xsl:sort select="@name='Chapters'" />
                <xsl:sort select="@name='Audio'" />
                <xsl:sort select="@name" />
                <xsl:if test="@type='master'">
                    <xsl:call-template name="parsePath">
                        <xsl:with-param name="node" select="."/>
                    </xsl:call-template>
                </xsl:if>
            </xsl:for-each>

        </xsl:if>

    </xsl:template>

    <xsl:template name="parentName">
        <xsl:param name="pText"/>
        <xsl:choose>
            <xsl:when test="contains($pText, '\')">
                <xsl:call-template name="parentName">
                    <xsl:with-param name="pText" select="substring-after($pText, '\')"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="translate($pText, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

    <!-- Type of elements stored in an EbmlList -->
    <xsl:template name="ebmlListStructure">
        <xsl:param name="node"/>
        <xsl:choose>
            <xsl:when test="$node/@type='master'">
                <xsl:choose>
                    <xsl:when test="$node/@name='TrackEntry'"><xsl:text>MatroskaTrack</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='ContentEncoding'"><xsl:text>MatroskaTrackEncoding</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='TrackPlane'"><xsl:text>MatroskaTrackPlane</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Colour'"><xsl:text>MatroskaTrackVideoColor</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='CuePoint'"><xsl:text>MatroskaIndex</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='CueTrackPositions'"><xsl:text>MatroskaIndexPos</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='AttachedFile'"><xsl:text>MatroskaAttachment</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Seek'"><xsl:text>MatroskaSeekhead</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Tag'"><xsl:text>MatroskaTags</xsl:text></xsl:when>
                    
                </xsl:choose>
            </xsl:when>
        </xsl:choose>
    </xsl:template>

    <!-- Type of elements stored in an Structure -->
    <xsl:template name="hardcodedStructure">
        <xsl:param name="node"/>
        <xsl:choose>
            <xsl:when test="$node/@type='master'">
                <xsl:choose>
                    <xsl:when test="$node/@name='Info'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Tracks'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Cues'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Tags'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Attachments'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='SeekHead'"><xsl:text>MatroskaDemuxContext</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='TrackCombinePlanes'"><xsl:text>MatroskaTrackOperation</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='ContentEncryption'"><xsl:text>MatroskaTrackEncryption</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='ContentCompression'"><xsl:text>MatroskaTrackCompression</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='ContentEncodings'"><xsl:text>MatroskaTrack</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Video'"><xsl:text>MatroskaTrackVideo</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Audio'"><xsl:text>MatroskaTrackAudio</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='MasteringMetadata'"><xsl:text>MatroskaMasteringMeta</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Projection'"><xsl:text>MatroskaTrackVideoProjection</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Cluster'"><xsl:text>MatroskaBlock</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='BlockGroup'"><xsl:text>MatroskaBlock</xsl:text></xsl:when>
                    <xsl:when test="$node/@name='Targets'"><xsl:text>MatroskaTagTarget</xsl:text></xsl:when>
                </xsl:choose>
            </xsl:when>
        </xsl:choose>
    </xsl:template>

    <xsl:template name="masterListName">
        <xsl:param name="node"/>
        <xsl:choose>
            <xsl:when test="$node/@name='TrackEntry'"><xsl:text>track</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Cues'"><xsl:text>index</xsl:text></xsl:when>
            <xsl:when test="$node/@name='CuePoint'"><xsl:text>index_entry</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Audio'"><xsl:text>track_audio</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Video'"><xsl:text>track_video</xsl:text></xsl:when>
            <xsl:when test="$node/@name='TrackOperation'"><xsl:text>track_operation</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Seek'"><xsl:text>SeekPoint</xsl:text></xsl:when>
            <xsl:when test="$node/@name='ContentEncodings'"><xsl:text>track_encodings</xsl:text></xsl:when>
            <xsl:when test="$node/@name='ContentEncoding'"><xsl:text>track_encoding</xsl:text></xsl:when>
            <xsl:when test="$node/@name='ContentCompression'"><xsl:text>track_encoding_compression</xsl:text></xsl:when>
            <xsl:when test="$node/@name='ContentEncryption'"><xsl:text>track_encoding_encryption</xsl:text></xsl:when>
            <xsl:when test="$node/@name='TrackCombinePlanes'"><xsl:text>track_combine_planes</xsl:text></xsl:when>
            <xsl:when test="$node/@name='TrackPlane'"><xsl:text>track_plane</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Colour'"><xsl:text>track_video_color</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Projection'"><xsl:text>track_video_projection</xsl:text></xsl:when>
            <xsl:when test="$node/@name='MasteringMetadata'"><xsl:text>mastering_meta</xsl:text></xsl:when>
            <xsl:when test="$node/@name='CueTrackPositions'"><xsl:text>index_pos</xsl:text></xsl:when>
            <xsl:when test="$node/@name='Targets'"><xsl:text>tagtargets</xsl:text></xsl:when>
            <xsl:when test="$node/@name='AttachedFile'"><xsl:text>attachment</xsl:text></xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="translate($node/@name, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ ', 'abcdefghijklmnopqrstuvwxyz_')"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>
