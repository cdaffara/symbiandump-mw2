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


#ifndef TOBJECTDESCRIPTION_H
#define TOBJECTDESCRIPTION_H

#include <mtp/mtpprotocolconstants.h>

// vendor extended property code
const TUint16 EMTPExtObjectPropCodeOmaDrmStatus = 0xDB01;
const TUint32 KMTPMaxDescriptionLen = 0x00000100;  // 256
const TInt KMtpMaxStringLength = 255;
const TInt KMtpMaxDateTimeStringLength = 15;
_LIT( KMtpDateTimeFormat, "%F%Y%M%DT%H%T%S" );


struct TObjectDescription
    {
    TUint16 iPropCode;
    TUint32 iGroupCode;
    };

enum TGroupCodeType
    {
    EGroupCodeGeneral    = 0x00000001,
    EGroupCodeSample     = 0x00000100,
    EGroupCodeMediaDB    = 0x00FF0000,
    EGroupCodeNotDefined = 0xFFFFFFFF   // TODO: need to reconsider
    };

static const TGroupCodeType KSupportedGroupCode[] =
    {
    EGroupCodeGeneral,
    EGroupCodeSample,
    EGroupCodeMediaDB
    };

static const TObjectDescription KPropGroupMapTable[] =
    {
        // The first group, mandatory for all
        { EMTPObjectPropCodeStorageID,        EGroupCodeGeneral },
        { EMTPObjectPropCodeObjectFormat,     EGroupCodeGeneral },
        { EMTPObjectPropCodeProtectionStatus, EGroupCodeGeneral },
        { EMTPObjectPropCodeObjectSize,       EGroupCodeGeneral },
        { EMTPObjectPropCodeObjectFileName,   EGroupCodeGeneral },
        { EMTPObjectPropCodeParentObject,     EGroupCodeGeneral },
        { EMTPObjectPropCodePersistentUniqueObjectIdentifier,
                                              EGroupCodeGeneral },
        { EMTPObjectPropCodeNonConsumable,    EGroupCodeGeneral },

        // The first group, addtional for all
        { EMTPObjectPropCodeName,             EGroupCodeGeneral },  // only for winlogo
        { EMTPObjectPropCodeDateCreated,      EGroupCodeGeneral },
        { EMTPObjectPropCodeDateModified,     EGroupCodeGeneral },

        // The first group, mandatory for video
        { EMTPObjectPropCodeWidth,            EGroupCodeGeneral },  // only for winlogo
        { EMTPObjectPropCodeHeight,           EGroupCodeGeneral },  // only for winlogo

        // The second group, for non-embedded album art
        { EMTPObjectPropCodeRepresentativeSampleFormat, EGroupCodeGeneral }, // changed for winlogo
        { EMTPObjectPropCodeRepresentativeSampleSize,   EGroupCodeGeneral }, // changed for winlogo
        { EMTPObjectPropCodeRepresentativeSampleHeight, EGroupCodeGeneral }, // changed for winlogo
        { EMTPObjectPropCodeRepresentativeSampleWidth,  EGroupCodeGeneral }, // changed for winlogo
        { EMTPObjectPropCodeRepresentativeSampleData,   EGroupCodeGeneral }, // changed for winlogo

        // The third group, additional for all
        { EMTPObjectPropCodeDateAdded,        EGroupCodeMediaDB },

        // The third group, mandatory for audio
        { EMTPObjectPropCodeArtist,           EGroupCodeMediaDB },
        { EMTPObjectPropCodeTrack,            EGroupCodeMediaDB },
        { EMTPObjectPropCodeGenre,            EGroupCodeMediaDB },
        { EMTPObjectPropCodeAlbumName,        EGroupCodeMediaDB },
        { EMTPObjectPropCodeSampleRate,       EGroupCodeMediaDB },
        { EMTPObjectPropCodeNumberOfChannels, EGroupCodeMediaDB },
        { EMTPObjectPropCodeAudioWAVECodec,   EGroupCodeMediaDB },
        { EMTPObjectPropCodeAudioBitRate,     EGroupCodeMediaDB },
        { EMTPObjectPropCodeDuration,         EGroupCodeMediaDB },
        { EMTPObjectPropCodeOriginalReleaseDate, EGroupCodeMediaDB },
        { EMTPObjectPropCodeDescription,      EGroupCodeMediaDB },
        { EMTPObjectPropCodeComposer,         EGroupCodeMediaDB },
        { EMTPObjectPropCodeAlbumArtist,      EGroupCodeMediaDB },

        // The third group, mandatory for video
        { EMTPObjectPropCodeScanType,         EGroupCodeMediaDB },
        { EMTPObjectPropCodeVideoFourCCCodec, EGroupCodeMediaDB },
        { EMTPObjectPropCodeVideoBitRate,     EGroupCodeMediaDB },
        { EMTPObjectPropCodeFramesPerThousandSeconds, EGroupCodeMediaDB },
        { EMTPObjectPropCodeKeyFrameDistance, EGroupCodeMediaDB },
        { EMTPObjectPropCodeEncodingProfile,  EGroupCodeMediaDB },

        // The third group, additional for video
        { EMTPObjectPropCodeParentalRating,   EGroupCodeMediaDB },
        { EMTPObjectPropCodeDRMStatus,        EGroupCodeMediaDB },
        { EMTPExtObjectPropCodeOmaDrmStatus,  EGroupCodeMediaDB }
    };

#endif // TOBJECTDESCRIPTION_H

