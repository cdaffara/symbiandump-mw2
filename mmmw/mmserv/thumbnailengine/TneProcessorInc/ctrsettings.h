/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Video settings
*
*/




#ifndef CTRSETTINGS_H
#define CTRSETTINGS_H

// INCLUDES
#include <devvideoconstants.h>


// MACROS
#ifdef _DEBUG
#define DEBUGPRINT
#endif

// Debug print macro
#ifdef DEBUGPRINT
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

// KTRCodecPriority specifies priority of decoder type 
// (ETrue - accelerated; EFalse - Non-accelerated)
// to be loaded first; 
const TBool KTRAccelerationPriorityDecoder = ETrue;


// CONSTANTS


// Decoder states
enum TRStates
    {
    ETRNone, 
    ETROpened, 
    ETRInitializing, 
    ETRInitialized, 
    ETRRunning, 
    ETRPaused, 
    ETRStopping, 
    ETRStopped, 
    ETRFatalError
    };

enum TRCodecTypes
    {
    EH263, 
    EMpeg4,
    EH264
    };

// Input format parameters
// Planar (first Y frame, then U, then V)
const TYuvDataLayout KTRYuvLayout = EYuvDataPlanar;

// YUV-RGB conversion coefficients to use: YUV data with range 0 (Y [16-235]) and conversion coefficients according to BT.601.5 
const TYuvCoefficients KTRYuvCoefficients = EYuvBt601Range0;

// Aspect ratio, in H.263 default is 12:11, but camera probably provides 1:1
const TInt KTRAspectRatioNum = 12;
const TInt KTRAspectRatioDenom = 11;


// Quality is more important than delay in non real-time mode
const TReal KTRLatencyQualityTradeoffNonRT = 1.0;

// Tradeoff for real-time mode
const TReal KTRLatencyQualityTradeoffRT = 0.3;

// range is [0...100]
const TUint KTRPictureQuality = 50;

// (1.0 means the picture quality is sacrificed 
// "all the way" to match the target fps
const TReal KTRQualityTemporalTradeoff = 0.8;


// 720x576,720x288,360x288
// PAL 1, 2, 3

// KTRDefaultSrcRate
const TReal KTRDefaultSrcRate = 15.0;

// KTRDefaultAccessRate
const TReal KTRDefaultAccessRate = 0.2;

// Number of video segments
const TUint KTRMinNumberOfBuffersVideoSegment = 25;
const TUint KTRMinNumberOfBuffersCodedPicture = 4;

// Min number of bitstream buffers for decoder
// @@ YHK Incerasing buffer to 4 from 2 
const TUint KTRDecoderMinNumberOfBuffers = 2;

// Number of internal buffers for resampled uncompressed data
const TUint KTRMinNumberOfResampledPictureBuffers = 4;

// Codec levels
const TUint KTRH263CodecLevel10 = 10;
const TUint KTRH263CodecLevel20 = 20;
const TUint KTRH263CodecLevel30 = 30;
const TUint KTRH263CodecLevel40 = 40;
const TUint KTRH263CodecLevel45 = 45;
const TUint KTRH263CodecLevel50 = 50;
const TUint KTRH263CodecLevel60 = 60;
const TUint KTRH263CodecLevel70 = 70;

const TUint KTRMPEG4CodecLevel0 = 0;
const TUint KTRMPEG4CodecLevel1 = 1;
const TUint KTRMPEG4CodecLevel2 = 2;
const TUint KTRMPEG4CodecLevel3 = 3;
const TUint KTRMPEG4CodecLevel0b = 9;
const TUint KTRMPEG4CodecLevel4a = 4;

// Bitstream buffer size
const TUint KTRMaxBufferSizeLevel10 = 8192;
const TUint KTRMaxBufferSizeLevel20 = 32768;
const TUint KTRMaxBufferSizeLevel30 = 32768;
const TUint KTRMaxBufferSizeLevel40 = 32768;    // FIXME When supported by codecs
const TUint KTRMaxBufferSizeLevel45 = 16384;
const TUint KTRMaxBufferSizeLevel50 = 32768;    // FIXME
const TUint KTRMaxBufferSizeLevel60 = 32768;    // FIXME
const TUint KTRMaxBufferSizeLevel70 = 32768;    // FIXME

const TUint KTRMaxBufferSizeLevel0 = 20480;
const TUint KTRMaxBufferSizeLevel0b = 40960;
const TUint KTRMaxBufferSizeLevel1 = 20480;
const TUint KTRMaxBufferSizeLevel2 = 40960;
const TUint KTRMaxBufferSizeLevel3 = 40960;
const TUint KTRMaxBufferSizeLevel4a = 163840;

// Bit rates for different profiles / levels, [kbps]
const TUint KTRMaxBitRateH263Level10 = 64000;
const TUint KTRMaxBitRateH263Level20 = 128000;
const TUint KTRMaxBitRateH263Level30 = 384000;
const TUint KTRMaxBitRateH263Level40 = 384000;  // FIXME !!!
const TUint KTRMaxBitRateH263Level45 = 128000;
const TUint KTRMaxBitRateH263Level50 = 384000;  // FIXME !!!
const TUint KTRMaxBitRateH263Level60 = 384000;  // FIXME !!!
const TUint KTRMaxBitRateH263Level70 = 384000;  // FIXME !!!

const TUint KTRMaxBitRateMPEG4Level0 = 64000;
const TUint KTRMaxBitRateMPEG4Level0b = 128000;
const TUint KTRMaxBitRateMPEG4Level1 = 64000;
const TUint KTRMaxBitRateMPEG4Level2 = 128000;
const TUint KTRMaxBitRateMPEG4Level3 = 384000;
const TUint KTRMaxBitRateMPEG4Level4a = 4000000;


// Target framerate default
const TReal KTRTargetFrameRateDefault = 15.0;

// Default segment size
const TUint KTRDefaultSegmentSize = 256;

// Min segment size
const TUint KTRMinSegmentSize = 256;

// Max desc8 length
const TUint KMaxDesC8Length = 256;

// Image size 
const TUint KTRQCIFWidth = 176;
const TUint KTRQCIFHeight = 144;
const TUint KTRSubQCIFWidth = 128;
const TUint KTRSubQCIFHeight = 96;
const TUint KTRCIFWidth = 352;
const TUint KTRCIFHeight = 288;
const TUint KTRPALWidth = 720;
const TUint KTRPAL2Height = 288;
const TUint KTRPAL1Height = 576;
const TUint KTRQVGAWidth = 320;
const TUint KTRQVGAHeight = 240;
const TUint KTRVGAWidth = 640;
const TUint KTRVGAHeight = 480;
const TUint KTR4CIFWidth = 704;
const TUint KTR4CIFHeight = 576;

// Number of TVideoPictures in TranscoderPicture queue 
const TUint KTRPictureBuffersNumber = 2;

// Number fo TTRVideoPicture containers in TRTranscoderQueue
const TUint KTRPictureContainersNumber = 5;

// Number of picture buffers allocated by encoder plugin (if BufferManagementCI is available)
// Only few (one or two) buffers can be used: See mdf extensions spec for details
const TUint KTRPictureBuffersNumberBMCI = 2;

// X0 would be enough.. ? 
const TUint KNumberOfEvents = 50;

#endif // CTRSETTINGS_H
