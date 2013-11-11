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


#ifndef MMMTPVIDEODEFS_H
#define MMMTPVIDEODEFS_H

// INCLUDES
#include <mmf/common/mmfmeta.h>

// CONSTANTS
_LIT( KMtpVideoTable,                "Video" );

// field
_LIT( KMtpVideoLocation,             "Location" );
_LIT( KMtpVideoName,                 "Name" );
_LIT( KMtpVideoArtist,               "Artist" );
_LIT( KMtpVideoTrack,                "Track" );
_LIT( KMtpVideoGenre,                "Genre" );
_LIT( KMtpVideoAlbumName,            "AlbumName" );
_LIT( KMtpVideoComposer,             "Composer" );
_LIT( KMtpVideoOrigReleaseDate,      "OrigReleaseDate" );
_LIT( KMtpVideoComment,              "Comment" );

_LIT( KMtpVideoWidth,                "Width" );
_LIT( KMtpVideoHeight,               "Height" );
_LIT( KMtpVideoDuration,             "Duration" );

_LIT( KMtpVideoSampleRate,           "SampleRate" );
_LIT( KMtpVideoNumberOfChannels,     "NumberOfChannels" );
_LIT( KMtpVideoAudioCodec,           "AudioCodec" );
_LIT( KMtpVideoAudioBitrate,         "AudioBitrate" );

_LIT( KMtpVideoVideoCodec,           "VideoCodec" );
_LIT( KMtpVideoVideoBitrate,         "VideoBitrate" );
_LIT( KMtpVideoFramesPer1000Sec,     "FramesPer1000Sec" );
_LIT( KMtpVideoKeyFrameDistance,     "KeyFrameDistance" );
_LIT( KMtpVideoScanType,             "ScanType" );
_LIT( KMtpVideoEncodingProfile,      "EncodingProfile" );

_LIT( KMtpVideoParentalRating,       "ParentalRating" );
_LIT( KMtpVideoDRM,                  "DRM" );

_LIT( KMtpVideoDeleted,              "Deleted" );

// field type
_LIT( KMtpVideoLocationType,         " LONG VARCHAR NOT NULL" );
_LIT( KMtpVideoNameType,             " LONG VARCHAR" );
_LIT( KMtpVideoArtistType,           " LONG VARCHAR" );
_LIT( KMtpVideoTrackType,            " UNSIGNED SMALLINT" );
_LIT( KMtpVideoGenreType,            " LONG VARCHAR" );
_LIT( KMtpVideoAlbumNameType,        " LONG VARCHAR" );
_LIT( KMtpVideoComposerType,         " LONG VARCHAR" );
_LIT( KMtpVideoOrigReleaseDateType,  " LONG VARCHAR" );
_LIT( KMtpVideoCommentType,          " LONG VARCHAR" );

_LIT( KMtpVideoWidthType,            " UNSIGNED INTEGER" );
_LIT( KMtpVideoHeightType,           " UNSIGNED INTEGER" );
_LIT( KMtpVideoDurationType,         " UNSIGNED INTEGER" );

_LIT( KMtpVideoSampleRateType,       " UNSIGNED INTEGER" );
_LIT( KMtpVideoNumberOfChannelsType, " UNSIGNED SMALLINT" );
_LIT( KMtpVideoAudioCodecType,       " UNSIGNED INTEGER" );
_LIT( KMtpVideoAudioBitrateType,     " UNSIGNED INTEGER" );

_LIT( KMtpVideoVideoCodecType,       " UNSIGNED INTEGER" );
_LIT( KMtpVideoVideoBitrateType,     " UNSIGNED INTEGER" );
_LIT( KMtpVideoFramesPer1000SecType, " UNSIGNED INTEGER" );
_LIT( KMtpVideoKeyFrameDistanceType, " UNSIGNED INTEGER" );
_LIT( KMtpVideoScanTypeType,         " UNSIGNED SMALLINT" );
_LIT( KMtpVideoEncodingProfileType,  " LONG VARCHAR" );

_LIT( KMtpVideoParentalRatingType,   " LONG VARCHAR" );
_LIT( KMtpVideoDRMType,              " UNSIGNED SMALLINT" );

_LIT( KMtpVideoDeletedType,          " BIT" );

// operation
_LIT( KMtpVideoStartCreateTable,     "CREATE TABLE " );
_LIT( KMtpVideoCommaSign,            "," ); 
_LIT( KMtpVideoSingleQuote,          "'" );
_LIT( KMtpVideoOpenBracket,          "(" );
_LIT( KMtpVideoCloseBracket,         ")" );

// Db file
_LIT( KMtpVideoDb,                   "MtpVideoDb.db" );

#endif // MMMTPVIDEODEFS_H
