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
* Description:   Header file for video related settings/constants
*
*/



#ifndef __TNEVIDEOSETTINGS_H__
#define __TNEVIDEOSETTINGS_H__

// VIDEO ENCODER RELATED SETTINGS / DEFAULTS

// Maximum supported resolution
const TUint KTNEMaxVideoWidth = 640;
const TUint KTNEMaxVideoHeight = 480;

// Max duration for video frame in microseconds. This limits the slow motion effect.
// This now limits the duration to 30 seconds which is already a very extreme case. This has impact to MPEG4's module_time_base variable 
// which should not be longer than 32 bits since there are some variables e.g. in video decoder than can handle only 32-bit fields. 
const TInt KTNEMaxFrameDuration(30000000);

// target/maximum bitrates  
const TUint KTNEBitRateH263Level10 = 64000;
const TUint KTNEBitRateH263Level20 = 512000;
const TUint KTNEBitRateH263Level45 = 128000;
const TUint KTNEBitRateMPEG4Level0 = 64000;
const TUint KTNEBitRateMPEG4Level0B = 128000;
const TUint KTNEBitRateMPEG4Level1 = 64000;
const TUint KTNEBitRateMPEG4Level2 = 512000;
const TUint KTNEBitRateMPEG4Level3 = 384000;
const TUint KTNEBitRateMPEG4Level4A = 4000000;

const TReal KTNEMaxVideoFrameRate = 15.0;

// number of frames for transition effect - NOTE: This must be an even number !!!
const TUint KNumTransitionFrames = 10; 

#define KTNEResolutionSubQCIF       (TSize(128,96))
#define KTNEResolutionQCIF          (TSize(176,144))
#define KTNEResolutionCIF           (TSize(352,288))
#define KTNEResolutionQVGA          (TSize(320,240))
#define KTNEResolutionVGA           (TSize(640,480))


const TUint KMaxCodedPictureSizeQCIF = 8192; // QCIF and smaller
const TUint KMaxCodedPictureSizeCIF = 32768; // CIF and smaller
const TUint KMaxCodedPictureSizeMPEG4QCIF = 20480; // QCIF and smaller
const TUint KMaxCodedPictureSizeMPEG4L0BQCIF = 40960; // QCIF and smaller
const TUint KMaxCodedPictureSizeMPEG4CIF = 81920; // MPEG-4 CIF
const TUint KMaxCodedPictureSizeVGA = 163840; // For vga support

_LIT8(KTNEMimeTypeH263, "video/H263-2000");
_LIT8(KTNEMimeTypeH263BaselineProfile, "video/H263-2000; profile=0");
_LIT8(KTNEMimeTypeH263Level45, "video/H263-2000; profile=0; level=45");
_LIT8(KTNEMimeTypeMPEG4Visual, "video/mp4v-es");
_LIT8(KTNEMimeTypeMPEG4SimpleVisualProfile, "video/mp4v-es; profile-level-id=8");
_LIT8(KTNEMimeTypeMPEG4SimpleVisualProfileLevel2, "video/mp4v-es; profile-level-id=2");
_LIT8(KTNEMimeTypeMPEG4SimpleVisualProfileLevel3, "video/mp4v-es; profile-level-id=3");
_LIT8(KTNEMimeTypeMPEG4SimpleVisualProfileLevel4A, "video/mp4v-es; profile-level-id=4");

#endif