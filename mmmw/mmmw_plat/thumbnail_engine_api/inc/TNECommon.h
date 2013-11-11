/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __TNECOMMON_H__
#define __TNECOMMON_H__

#include <e32std.h>

/**
 * Enumeration for video clip classes.
 */
enum TTNEVideoClipClass
    {
    ETNEVideoClipClassFile = 15001,
    ETNEVideoClipClassGenerated
    };



/**
 * Enumeration for video bitstream modes.
 */
enum TTNEVideoBitstreamMode
    {
    ETNEVideoBitstreamModeUnknown = 0,          /* unrecognized mode; outside of H.263 Profile 0 Level 10, or MPEG-4 Visual Simple Profile */
    ETNEVideoBitstreamModeH263,                 /* H.263 Simple Profile (Profile 0, Level 10) */                                    
    ETNEVideoBitstreamModeMPEG4ShortHeader,     /* MPEG-4 Visual Simple Profile - Short Header */
    ETNEVideoBitstreamModeMPEG4Regular,         /* MPEG-4 Visual Simple Profile - Regular */
    ETNEVideoBitstreamModeMPEG4Resyn,           /* MPEG-4 Visual Simple Profile - Regular with Resynchronization Markers */
    ETNEVideoBitstreamModeMPEG4DP,              /* MPEG-4 Visual Simple Profile - Data Partitioned */
    ETNEVideoBitstreamModeMPEG4DP_RVLC,         /* MPEG-4 Visual Simple Profile - Data Partitioned with Reversible VLCs */
    ETNEVideoBitstreamModeMPEG4Resyn_DP,        /* MPEG-4 Visual Simple Profile - Data Partitioned with Resynchronization Markers */
    ETNEVideoBitstreamModeMPEG4Resyn_DP_RVLC    /* MPEG-4 Visual Simple Profile - Data Partitioned with Reversible VLCs & Resynchronization Markers */
    };



/* Video format of bitstream - used everywhere */
enum TTNEVideoType
    {
    ETNEVideoTypeUnrecognized = 13001,  /* should always be the first one */
    ETNEVideoTypeNoVideo,               /* video is not present */
    ETNEVideoTypeH263Profile0Level10,   /* H.263 Simple Profile (Profile 0, Level 10) */
    ETNEVideoTypeH263Profile0Level45,   /* H.263 Simple Profile (Profile 0, Level 45) */
    ETNEVideoTypeMPEG4SimpleProfile,    /* MPEG-4 Visual Simple Profile (any mode) */
    ETNEVideoTypeAVCProfileBaseline,
    ETNEVideoTypeLast                   /* should always be the last one */
    };
	
/**
 * Enumeration for video formats.
 */
enum TTNEVideoFormat
    {
    ETNEVideoFormatUnrecognized = 11001,  // should always be the first one
    ETNEVideoFormat3GPP,
    ETNEVideoFormatMP4,
    ETNEVideoFormatLast  // should always be the last one
    };


/**
 * Output parameter class
 */ 

class TTNEOutputParameters
    {
    public:
        // default constructor; initializes optional parameters to values which mean that editor can decide the value
        inline TTNEOutputParameters() 
        : iVideoBitrate(0), iVideoFrameRate(0)
            {}
            
    public:

        // video codec
        TTNEVideoType iVideoType;
        // target resolution
        TSize iVideoResolution;
        // target video bitrate, 0 = no preference
        TInt iVideoBitrate;
        // target video framerate, 0 = no preference
        TReal iVideoFrameRate;
        
    };


/**
 * Transcode factor.
 */
struct TTNETranscodeFactor
    {
    TInt iTRes;
    TTNEVideoBitstreamMode iStreamType;
    };


/**
 * Enumerates video editor engine panic codes and 
 * provides a static Panic() function.
 *
 */
class TTNEPanic
    {
public:
    enum TTNEPanicCodes 
        {
        EInternal = 1,  // internal error (that is, a bug in the video editor engine)
        EDeprecated,    // deprecated class or method
        EVideoClipInfoNotReady,
        EVideoClipInfoIllegalVideoFrameIndex,
        EVideoClipInfoIllegalVideoFrameTime,
        EVideoClipInfoFrameOperationAlreadyRunning,
        EVideoClipInfoIllegalFrameResolution,
        EVideoClipInfoNoFileAssociated,
        EVideoClipInfoNoGeneratorAssociated,
        EAudioClipInfoNotReady,  //10
        EMovieAddOperationAlreadyRunning,
        EMovieIllegalStartTransitionEffect,
        EMovieIllegalMiddleTransitionEffect,
        EMovieIllegalEndTransitionEffect,
        EMovieProcessingOperationAlreadyRunning,
        EMovieObserverAlreadyRegistered,
        EMovieObserverNotRegistered,
        EMovieIllegalQuality,
        EVideoClipIllegalIndex, //20
        EVideoClipIllegalSpeed, 
        EVideoClipIllegalColorEffect,
        EVideoClipIllegalCutInTime,
        EVideoClipIllegalCutOutTime,
        EVideoClipNoFileAssociated,
        EAudioClipIllegalStartTime,
        EAudioClipIllegalCutInTime,
        EAudioClipIllegalCutOutTime,
        EIllegalDynamicLevelMark,
        EIllegalDynamicLevelMarkIndex,
        EVideoClipGeneratorNotOveray,
        EAnimationClipGeneratorNotReady
        };

public:
    inline static void Panic(TInt aPanic);
    };


#include "TNECommon.inl"

#endif // __TNECOMMON_H__

