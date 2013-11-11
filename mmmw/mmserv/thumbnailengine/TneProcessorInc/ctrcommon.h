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
* Description:   Common elements / data types
*
*/



#ifndef CTRCOMMON_H
#define CTRCOMMON_H

// INCLUDES
#include <e32std.h>

// Video data format
enum TTRVideoDataType
{
	ETRDuCodedPicture = 0,  // Each data unit contains one coded video picture
	ETRDuVideoSegment,  // Each data unit contains one coded video segment (according resync value (MPEG4) and GOB header in H.263)
	ETRYuvRawData420,   // Each data unit represents uncompressed video picture in YUV 4:2:0 format
	ETRYuvRawData422    // Each data unit represents uncompressed video picture in YUV 4:2:2 format
};


/**
*  Video coding options
*  @lib TRANSCODER.LIB
*  @since 3.1
*/
class TTRVideoCodingOptions
    {
    public:
        // Segment interval in picture. In H.263 baseline this means number of non-empty GOB headers 
        // (1=every GOB has a header), in MB-based systems number of MBs per segment. 
        // Default is 0 == no segments inside picture
        // Coding standard & used profile etc. limit the value.
        TInt iSyncIntervalInPicture;
        
        // Time between random access points (I-Frame)
        TUint iMinRandomAccessPeriodInSeconds;

        // Relevant to MPEG4 only. Specifies whether data partitioning is in use. 
        // When equal to ETrue, data partitioning is in use.
        TBool iDataPartitioning;

        // Relevant to MPEG4 only. Specifies whether reversible variable length coding is in use. 
        // When equal to ETrue, reversible variable length coding is in use. 
        // Valid only if iDataPartitioned is equal to ETrue.
        TBool iReversibleVLC;

        // Relevant to MPEG4 only. Header extension code. 
        TUint iHeaderExtension;
    };


/**
*  Video format
*  @lib TRANSCODER.LIB
*  @since 3.1
*/
class TTRVideoFormat
    {
    public:
        // Video picture size
        TSize iSize;

        // Video data type
        TInt iDataType;
    };



/**
*  Video picture
*  @lib TRANSCODER.LIB
*  @since 3.1
*/
class TTRVideoPicture
    {
    public:
        // Picture data
        TPtr8* iRawData;

        // Picture size in pixels
        TSize iDataSize;

        // Picture timestamp
        TTimeIntervalMicroSeconds iTimestamp;

        // Queue element
        TDblQueLink iLink; 

        // Misc user info
        TAny* iUser;
    };


/**
*  Display options
*  @lib TRANSCODER.LIB
*  @since 3.1
*/
class TTRDisplayOptions
    {
    public:
        // The video output rectangle on screen
        TRect   iVideoRect;

        // Initial clipping region to use
        TRegion iClipRegion;
    };


/**
*  TTREventItem
*  @lib TRANSCODER.LIB
*/
class CTREventItem
    {
    public:
        // Timestamp from which to start iAction
        TTimeIntervalMicroSeconds iTimestamp;
        
        // EnableEncoder setting status
        TBool iEnableEncoderStatus; 
        
        // Enable PS setting status
        TBool iEnablePictureSinkStatus;
        
        // RandomAccess client's setting
        TBool iRandomAccessStatus;
        
        // Enable / Disable encoder client setting
        TBool iEnableEncoderClientSetting;
        
        // Enable / Disable picture sink client's setting
        TBool iEnablePictureSinkClientSetting;
        
        // Queue link
        TDblQueLink iLink;
        
    public:
        // Reset item's setting
        inline void Reset()
            {
            // Reset setting statuses & ts
            iTimestamp = -1;
            iEnableEncoderStatus = EFalse;
            iEnablePictureSinkStatus = EFalse;
            iRandomAccessStatus = EFalse;
            };
    };

#endif