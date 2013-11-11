/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#ifndef MEDIAMTPDATAPROVIDERCONST_H
#define MEDIAMTPDATAPROVIDERCONST_H

#include <mtp/mtpprotocolconstants.h>

#include "tobjectdescription.h"

_LIT( KMTPTxtColon, ":" );


enum TMmMtpNumberOfChannels
    {
    EMTPChannelMono = 0x0001,
    EMTPChannelStereo = 0x0002
    };

enum TMmMtpSampleRateValue
    {
    EMTPSampleRate8K = 0x00001F40,
    EMTPSampleRate11_025K = 0x00002B11,
    EMTPSampleRate16K = 0x00003E80,
    EMTPSampleRate22_05K = 0x00005622,
    EMTPSampleRate24K = 0x00005DC0,
    EMTPSampleRate32K = 0x00007D00,
    EMTPSampleRate44_1K = 0x0000AC44,
    EMTPSampleRate48K = 0x0000BB80
    };

// WAVECodec value
enum TMmMtpWAVECodecValue
    {
    EMTPAudioWAVECodecMP3       = 0x00000055,
    EMTPAudioWAVECodecWMA       = 0x00000161,
    EMTPAudioWAVECodecWMAPro    = 0x00000162,
    EMTPAudioWAVECodecAAC       = 0x0000A106,
    EMTPAudioWAVECodecPCM       = 0x00000001
    };

// Bitrate value scope
enum TMmMtpBitrateValue
    {
    EMTPMP3MinBitrate = 0x00001F40,         //8 kbps
    EMTPMP3MaxBitrate = 0x0004E200,         //320 kbps
    EMTPWMAMinBitrate = 0x00001388,         //5 kbps
    EMTPWMAMaxBitrate = 0x0005DC00,         //384 kbps
    EMTPWMAProMinBitrate = 0x00001388,      //5 kbps
    EMTPWMAProMaxBitrate = 0x000BB800,      //768 kbps
    EMTPAACMinBitrate = 0x00001F40,         //8 kbps
    EMTPAACMaxBitrate = 0x0004E200,         //320 kbps
    EMTPPCMMinBitrate = 0x00003E80,         //16 kbps
    EMTPPCMMaxBitrate = 0x00177000,         //1536 kbps
    EMTPAACPlusMinBitrate = 0x00003E80,     //16 kbps
    EMTPAACPlusMaxBitrate = 0x0008CA00,     //576 kbps
    EMTPAudioBitrateStep = 0x00000001
    };

enum TMmMtpDuration
    {
    EMTPMinDuration = 0x00000000,
    EMTPMaxDuration = 0xFFFFFFFF,
    EMTPDurationStep = 0x00000001
    };

enum TMmMtpWidthRange
    {
    EMTPMinWidth  = 0x000000b0,  // 176 pixels
    EMTPMaxWidth  = 0x00000500,  // 1280 pixels, standard for 720p content
    EMTPStepWidth = 0x00000001,
    };

enum TMmMtpHeightRange
    {
    EMTPMinHeight  = 0x00000090,  // 144 pixels
    EMTPMaxHeight  = 0x000002D0,  // 720 pixels, standard for 720p content
    EMTPStepHeight = 0x00000001,
    };

enum TMmMtpVideoBitRateRange
    {
    EMTPWMVMinBitrate  = 0x00000000,
    EMTPWMVMaxBitrate  = 0x01312D00,        // 20 Mbps, recommanded by Helix team
    EMTPVideoBitrateStep = 0x00000001
    };

enum TMmMtpScanType
    {
    EMTPScanTypeProgressive = 0x0001
    };

enum TMmMtpVideoFourCCCodec
    {
    EMTPVideoFourCCCodecWMV3 = 0x33564d57/*,    // this is intended, please keep it like this, comment out asf fourcc for mp2a and mp4a, not sure if this is needed
    EMTPVideoFourCCCodecMP2A = 0x4D503241,
    EMTPVideoFourCCCodecMP4A = 0x4D503441*/
    };

enum TMmMtpFramesPerThousandSecondsRange
    {
    EMTPMinFramesPerThousandSeconds  = 0x00000000,
    EMTPMaxFramesPerThousandSeconds  = 0x00007530,  // 30000 frame per thousand seconds (30 frame/s), standard for progressive content
    EMTPStepFramesPerThousandSeconds = 0x00000001
    };

enum TMmMtpKeyFrameDistanceRange
    {
    EMTPMinKeyFrameDistance  = 0x00000000,
    EMTPMaxKeyFrameDistance  = 0x00003a98,          // 15000 ms
    EMTPStepKeyFrameDistance = 0x00000001
    };

enum TMmMtpDrmPropertyValues
    {
    EMTPDrmNoProtection    = 0x0000,
    EMTPDrmProtection      = 0x0001,
    EMTPDrmReserveForMTP   = 0x4000,
    EMTPDrmVenderExtension = 0x4001
    };

enum TMmMtpOMADrmPropertyValues
    {
    EMTPOMADrmNoProtection    = 0x00,
    EMTPOMADrmProtection      = 0x01
    };

/**
* define all the operations that are supported by the media mtp data provider
*/
static const TUint16 KMediaMtpDataProviderSupportedFormats[] =
    {
    EMTPFormatCodeMP3,
#ifdef __WINDOWS_MEDIA
    EMTPFormatCodeWMA,
#endif
    EMTPFormatCodeMP4Container,
    EMTPFormatCode3GPContainer,
    EMTPFormatCodeAAC,
    EMTPFormatCodeWAV,
#ifdef __WINDOWS_MEDIA
    EMTPFormatCodeWMV,
    EMTPFormatCodeASF
#endif
    };

/**
* define all the operations that are supported by the media mtp data provider
*/
static const TUint16 KMediaMtpDataProviderSupportedOperations[] =
    {
    // basic mode
    EMTPOpCodeGetObjectInfo,
    EMTPOpCodeGetObject,
    EMTPOpCodeDeleteObject,
    EMTPOpCodeSendObjectInfo,
    EMTPOpCodeSendObject,
    EMTPOpCodeMoveObject,
    EMTPOpCodeCopyObject,
    EMTPOpCodeGetPartialObject,
    EMTPOpCodeGetObjectPropsSupported,
    EMTPOpCodeGetObjectPropDesc,
    EMTPOpCodeGetObjectPropValue,
    EMTPOpCodeSetObjectPropValue,

    // enhanced mode
    EMTPOpCodeGetObjectPropList,
    EMTPOpCodeSetObjectPropList,
    EMTPOpCodeGetInterdependentPropDesc,
    EMTPOpCodeSendObjectPropList,

    // window 7 extension
    EMTPOpCodeGetFormatCapabilities
    };

// Mandatory for audio formats
static const TUint16 KMmMtpDpSupportedPropMandatoryAudio[] =
    {
    EMTPObjectPropCodeArtist,
    EMTPObjectPropCodeTrack,
    EMTPObjectPropCodeGenre,
    EMTPObjectPropCodeAlbumName,
    EMTPObjectPropCodeSampleRate,
    EMTPObjectPropCodeNumberOfChannels,
    EMTPObjectPropCodeAudioWAVECodec,
    EMTPObjectPropCodeAudioBitRate,
    EMTPObjectPropCodeAlbumArtist
    };

// Additional for audio formats
static const TUint16 KMmMtpDpSupportedPropAdditionalAudio[] =
    {
    EMTPObjectPropCodeDuration,
    EMTPObjectPropCodeOriginalReleaseDate,
    EMTPObjectPropCodeDescription,
    EMTPObjectPropCodeComposer,
    EMTPObjectPropCodeDRMStatus,
    EMTPExtObjectPropCodeOmaDrmStatus,
    };

// Mandatory properties for WMV/ASF
static const TUint16 KMmMtpDpSupportedPropMandatoryWMV[] =
    {
    EMTPObjectPropCodeWidth,
    EMTPObjectPropCodeHeight,
    EMTPObjectPropCodeDuration,
    EMTPObjectPropCodeGenre,
    EMTPObjectPropCodeSampleRate,
    EMTPObjectPropCodeNumberOfChannels,
    EMTPObjectPropCodeScanType,
    EMTPObjectPropCodeAudioWAVECodec,
    EMTPObjectPropCodeAudioBitRate,
    EMTPObjectPropCodeVideoFourCCCodec,
    EMTPObjectPropCodeVideoBitRate,
    EMTPObjectPropCodeFramesPerThousandSeconds,
    EMTPObjectPropCodeKeyFrameDistance,
    EMTPObjectPropCodeEncodingProfile,
    };

// Additional for WMV/ASF
static const TUint16 KMmMtpDpSupportedPropAdditionalWMV[] =
    {
    EMTPObjectPropCodeParentalRating,
    EMTPObjectPropCodeDRMStatus,
    EMTPExtObjectPropCodeOmaDrmStatus,
    };

#endif // MEDIAMTPDATAPROVIDERCONST_H
