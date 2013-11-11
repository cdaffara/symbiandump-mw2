/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Metadata Engine's literals.
*
*/

// conforms to Metadata schema version 1.6

#ifndef MDECONSTANTS_H
#define MDECONSTANTS_H

namespace MdeConstants
    {
    namespace Object
        {
        _LIT( KBaseObject, "Object" );
        _LIT( KAutomaticUri, "" );

        _LIT( KSizeProperty, "Size" );
        _LIT( KOriginProperty, "Origin" );
        _LIT( KUsageCountProperty, "UsageCount" );
        _LIT( KCreationDateProperty, "CreationDate" );
        _LIT( KLastModifiedDateProperty, "LastModifiedDate" );
        _LIT( KItemTypeProperty, "ItemType" );
        _LIT( KTitleProperty, "Title" );
        _LIT( KTimeOffsetProperty, "TimeOffset" );
        _LIT( KInDefaultFolder, "InDefaultFolder" );

        enum TOrigin
            {
            EOther      = 0,
            ECamera     = 1,
            EDownloaded = 2,
            ESideLoaded = 3,
            ERecorded   = 4
            };
        }
        typedef MdeConstants::Object::TOrigin Origin; // deprecated

    namespace Location
        {
        _LIT( KLocationObject, "Location" );
        _LIT( KLocationItemType, "application/vnd.nokia.mde.locationcontext" );

        _LIT( KCellIdProperty, "CellID" );
        _LIT( KCountryCodeProperty, "CountryCodeStr" );
        _LIT( KNetworkCodeProperty, "NetworkCodeStr" );
        _LIT( KLocationAreaCodeProperty, "LocationAreaCode" );
        _LIT( KSpeedProperty, "Speed" );
        _LIT( KDirectionProperty, "Direction" );
        _LIT( KQualityProperty, "Quality" );
        _LIT( KLatitudeProperty, "Latitude" );
        _LIT( KLongitudeProperty, "Longitude" );
        _LIT( KAltitudeProperty, "Altitude" );
        _LIT( KSatellitesProperty, "Satellites" );
        _LIT( KCountryProperty, "Country" );
        _LIT( KCellNameProperty, "CellName" );
        }

    namespace Calendar
        {
        _LIT( KCalendarObject, "Calendar" );
        _LIT( KCalendarItemType, "application/vnd.nokia.mde.calendarcontext" );

        _LIT( KCalendarTypeProperty, "CalendarType" );
        _LIT( KStartTimeProperty, "StartTime" );
        _LIT( KEndTimeProperty, "EndTime" );
        _LIT( KLocationProperty, "Location" );
        _LIT( KvCalendarProperty, "vCalendar" );
        _LIT( KLocalUidProperty, "LocalUid" );
        }

    namespace Contact
        {
        _LIT( KContactObject, "Contact" );
        _LIT( KContactItemType, "application/vnd.nokia.mde.contactcontext" );

        _LIT( KNumberProperty, "Number" );
        _LIT( KEmailAddressProperty, "EmailAddress" );
        _LIT( KCompanyProperty, "Company" );
        _LIT( KJobTitleProperty, "JobTitle" );
        _LIT( KAddressProperty, "Address" );
        _LIT( KURLProperty, "ContactURL" );
        _LIT( KvCardProperty, "vCard" );
        }

    namespace Message
        {
        _LIT( KMessageObject, "Message" );
        _LIT( KSMSItemType, "application/vnd.nokia.mde.sms" );
        _LIT( KMMSItemType, "application/vnd.nokia.mde.mms" );

        _LIT( KReceivedProperty, "Received" );
        _LIT( KSenderProperty, "Sender" );
        _LIT( KToWhomProperty, "ToWhom" );
        }

    namespace Album
        {
        _LIT( KAlbumObject, "Album" );
        _LIT( KAlbumItemType, "application/vnd.nokia.mde.album" );

        _LIT( KOrderedProperty, "Ordered" );
        _LIT( KTypeProperty, "Type" );
        
        // Depricated
        _LIT( KPropertiesProperty, "Properties" );
        _LIT( KSlideshowWallpaperProperty, "SlideshowWallpaper" );

        enum TAlbumType
            {
            EAlbumUser            = 0,
            EAlbumUserPredefined  = 1,
            EAlbumSystemFavourite = 2,
            EAlbumSystemCamera  = 3,
            };
        typedef MdeConstants::Album::TAlbumType AlbumType; // deprecated
        }

    namespace Tag
        {
        _LIT( KTagObject, "Tag" );
        _LIT( KTagItemType, "application/vnd.nokia.mde.tag" );
        enum TTagType
            {
            EUserDefineTags = 0,
            ESystemDefineCityTags = 1,
            ESystemDefineCountryTags = 2,
            EUnMappedLocationTags = 3
            };
        }

    namespace MediaObject
        {
        _LIT( KMediaObject, "MediaObject" );

        _LIT( KProtectedProperty, "Protected" );
        _LIT( KDRMProperty, "DRM" );
        _LIT( KThumbnailPresentProperty, "ThumbnailPresent" );
        _LIT( KResizedProperty, "Resized" );
        _LIT( KSampleProperty, "Sample" );
        _LIT( KRatingProperty, "Rating" );
        _LIT( KPrivacyProperty, "Privacy" );
        _LIT( KResolutionUnitProperty, "ResolutionUnit" );
        _LIT( KBitrateProperty, "Bitrate" );
        _LIT( KWidthProperty, "Width" );
        _LIT( KHeightProperty, "Height" );
        _LIT( KPreinstalledProperty, "Preinstalled" );
        _LIT( KAccessCountProperty, "AccessCount" );
        _LIT( KDownloadIdProperty, "DownloadID" );
        _LIT( KDurationProperty, "Duration" );
        _LIT( KReleaseDateProperty, "ReleaseDate" );
        _LIT( KCaptureDateProperty, "CaptureDate" );
        _LIT( KArtistProperty, "Artist" );
        _LIT( KAuthorProperty, "Author" );
        _LIT( KCopyrightProperty, "Copyright" );
        _LIT( KLegalProperty, "Legal" );
        _LIT( KDescriptionProperty, "Description" );
        _LIT( KCommentProperty, "Comment" );
        _LIT( KGenreProperty, "Genre" );
        _LIT( KTrackProperty, "Track" );
        _LIT( KAudioFourCCProperty, "AudioFourCC" );
        
        enum TPreinstalledFlag
        	{
        	ENotPreinstalled = 0,
        	EPreinstalled    = 1
        	};
        }

    namespace Video // derived from MediaObject
        {
        _LIT( KVideoObject, "Video" );

        _LIT( KAgeProfileProperty, "AgeProfile" );
        _LIT( KRecordingFlagsProperty, "RecordingFlags" );
        _LIT( KLastPlayPositionProperty, "LastPlayPosition" );
        _LIT( KFramerateProperty, "Framerate" );
        _LIT( KAudioLanguageProperty, "AudioLanguage" );
        }

    namespace Image // derived from MediaObject
        {
        _LIT( KImageObject, "Image" );

        _LIT( KDraftProperty, "Draft" );
        _LIT( KFocalPlaneResolutionUnitProperty, "FocalPlaneResolutionUnit" );
        _LIT( KExposureProgramProperty, "ExposureProgram" );
        _LIT( KFocalLengthIn35mmFilmProperty, "FocalLengthIn35mmFilm" );
        _LIT( KISOSpeedRatingsProperty, "ISOSpeedRatings" );
        _LIT( KMeteringModeProperty, "MeteringMode" );
        _LIT( KWhiteBalanceProperty, "WhiteBalance" );
        _LIT( KFlashProperty, "Flash" );
        _LIT( KColourSpaceProperty, "ColourSpace" );
        _LIT( KOrientationProperty, "Orientation" );
        _LIT( KSamplesPerPixelProperty, "SamplesPerPixel" );
        _LIT( KBitsPerSampleProperty, "BitsPerSample" );
        _LIT( KYCbCrPositioningProperty, "YCbCrPositioning" );
        _LIT( KThumbCompressionProperty, "ThumbCompression" );
        _LIT( KThumbResolutionUnitProperty, "ThumbResolutionUnit" );
        _LIT( KThumbOrientationProperty, "ThumbOrientation" );
        _LIT( KFrameCountProperty, "FrameCount" );
        _LIT( KPixelXDimensionProperty, "PixelXDimension" );
        _LIT( KPixelYDimensionProperty, "PixelYDimension" );
        _LIT( KExifVersionProperty, "ExifVersion" );
        _LIT( KComponentsConfigurationProperty, "ComponentsConfiguration" );
        _LIT( KFlashPixVersionProperty, "FlashPixVersion" );
        _LIT( KThumbXResolutionProperty, "ThumbXResolution" );
        _LIT( KThumbYResolutionProperty, "ThumbYResolution" );
        _LIT( KFocalPlaneXResolutionProperty, "FocalPlaneXResolution" );
        _LIT( KFocalPlaneYResolutionProperty, "FocalPlaneYResolution" );
        _LIT( KExposureTimeProperty, "ExposureTime" );
        _LIT( KExposureBiasValueProperty, "ExposureBiasValue" );
        _LIT( KFNumberProperty, "FNumber" );
        _LIT( KFocalLengthProperty, "FocalLength" );
        _LIT( KApertureValueProperty, "ApertureValue" );
        _LIT( KMaxApertureProperty, "MaxAperture" );
        _LIT( KShutterSpeedValueProperty, "ShutterSpeedValue" );
        _LIT( KXResolutionProperty, "XResolution" );
        _LIT( KYResolutionProperty, "YResolution" );
        _LIT( KDateTimeProperty, "DateTime" );
        _LIT( KDateTimeDigitizedProperty, "DateTimeDigitized" );
        _LIT( KDateTimeOriginalProperty, "DateTimeOriginal" );
        _LIT( KPixelFormatProperty, "PixelFormat" );
        _LIT( KModelProperty, "Model" );
        _LIT( KMakeProperty, "Make" );
        _LIT( KRelatedSoundFileProperty, "RelatedSoundFile" );
        }

    namespace Audio // derived from MediaObject
        {
        _LIT( KAudioObject, "Audio" );

        _LIT( KSamplingFrequencyProperty, "SamplingFrequency" );
        _LIT( KAlbumProperty, "Album" );
        _LIT( KComposerProperty, "Composer" );
        _LIT( KOriginalArtistProperty, "OriginalArtist" );
        _LIT( KAlbumArtistProperty, "AlbumArtist" );
        }

    namespace Events
        {
        _LIT( KCreated, "Created" );
        _LIT( KDeleted, "Deleted" );
        _LIT( KOpened, "Opened" );
        _LIT( KPlayed, "Played" );
        _LIT( KEdited, "Edited" );
        _LIT( KSent, "Sent" );
        _LIT( KReceived, "Received" );
        }

    namespace Relations
        {
        _LIT( KContains, "Contains" );
        _LIT( KReplaces, "Replaces" );
        _LIT( KIsVersionOf, "IsVersionOf" );
        _LIT( KRequires, "Requires" );
        _LIT( KUses, "Uses" );
        _LIT( KReferences, "References" );
        _LIT( KSummarises, "Summarises" );
        _LIT( KContainsLocation, "ContainsLocation" );
        }

    namespace TrackLog
    	{
    	_LIT( KTrackLogObject, "TrackLog" );
    	_LIT( KTrackLogItemType, "application/vnd.nokia.mde.tracklog" );
    	_LIT( KLengthProperty, "Length" );
    	_LIT( KStartTimeProperty, "StartTime" );
    	_LIT( KStopTimeProperty, "StopTime" );
    	}
    }

// type definitions

typedef MdeConstants::Object::TOrigin TOrigin;


#endif // MDECONSTANTS_H
