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
* Description:  Definitions for general media type
*
*/



#ifndef CMPXMEDIAGENERALDEFS_H
#define CMPXMEDIAGENERALDEFS_H


#include <e32base.h>
#include <mpxattribute.h>

/**
* Content ID identifying general category of content provided
* in the media object and associated attributes. This should
* be supported by ALL media objects as a common base
*/

const TInt KMPXMediaIdGeneral=0x101FFC40;
/**
*  Attributes supported for KMPXMediaIdGeneral
*/
/**
*  TMPXGeneralType}; see below for enum definition
*/
const TMPXAttributeData KMPXMediaGeneralType={KMPXMediaIdGeneral,0x01};

/**
*  TMPXGeneralCategory}; see below for enum definition
*/
const TMPXAttributeData KMPXMediaGeneralCategory={KMPXMediaIdGeneral,0x02};

/**
*  Descriptor. File path if local (excluding scheme).
*/
const TMPXAttributeData KMPXMediaGeneralUri={KMPXMediaIdGeneral,0x04};

/**
*  Descriptor. Drive}; if local (and physical)
*/
const TMPXAttributeData KMPXMediaGeneralDrive={KMPXMediaIdGeneral,0x08};

/**
*  TInt}; in bytes
*/
const TMPXAttributeData KMPXMediaGeneralSize={KMPXMediaIdGeneral,0x10};

/**
*  TInt.
*  0 if static}; KMaxTInt if continous stream. If container};
*  then duration applies to sum of contents
*/
const TMPXAttributeData KMPXMediaGeneralDuration={KMPXMediaIdGeneral,0x20};

/**
*  Descriptor
*/
const TMPXAttributeData KMPXMediaGeneralTitle={KMPXMediaIdGeneral,0x40};

/**
*  TTime.Int64(). Storing as TTime instead TInt64
*  will result in compilation warning.
*/
const TMPXAttributeData KMPXMediaGeneralDate={KMPXMediaIdGeneral,0x80};

/**
*  Descriptor
*/
const TMPXAttributeData KMPXMediaGeneralComment={KMPXMediaIdGeneral,0x100};

/**
*  Descriptor
*/
const TMPXAttributeData KMPXMediaGeneralMimeType={KMPXMediaIdGeneral,0x200};

/**
*  CFbsBitmap - for now could
*  be file path in future to enable easier
*  porting of client (i.e. UI)
*/
const TMPXAttributeData KMPXMediaGeneralThumbnail1={KMPXMediaIdGeneral,0x400};

/**
*  CFbsBitmap - for now could
*  be file path in future to enable easier
*  porting of client (i.e. UI)
*/
const TMPXAttributeData KMPXMediaGeneralThumbnail2={KMPXMediaIdGeneral,0x800};

/**
*  TIconInfo - Bitmap
*/
const TMPXAttributeData KMPXMediaGeneralIcon={KMPXMediaIdGeneral,0x1000};

/**
*  TBool
*/
const TMPXAttributeData KMPXMediaGeneralRemote={KMPXMediaIdGeneral,0x2000};

/**
*  TBool
*/
const TMPXAttributeData KMPXMediaGeneralSynchronized={KMPXMediaIdGeneral,0x4000};

/**
*  TBool
*/
const TMPXAttributeData KMPXMediaGeneralDeleted={KMPXMediaIdGeneral,0x8000};

/**
* TBool
*/
const TMPXAttributeData KMPXMediaGeneralModified ={KMPXMediaIdGeneral, 0x10000};

/**
*  TInt. Number of items (if container}; else
*  would trivially be 1)
*/
const TMPXAttributeData KMPXMediaGeneralCount={KMPXMediaIdGeneral,0x20000};

/**
*  TMPXItemId (as def'd in CollectionPath)
*  ID of item
*/
const TMPXAttributeData KMPXMediaGeneralId={KMPXMediaIdGeneral,0x40000};

/**
*  Collection id of this media
*/
const TMPXAttributeData KMPXMediaGeneralCollectionId={KMPXMediaIdGeneral,0x80000};

/**
*  TBool}; indicates if Uri is a broken link
*  DEPRECATED - TO BE REMOVED IN INCREMENT 9 RELEASE
*  This attribute has been replaced by
*  KMPXMediaGeneralFlagsIsInvalid bit in
*  const TMPXAttributeData KMPXMediaGeneralFlags
*/
const TMPXAttributeData KMPXMediaGeneralBrokenLink={KMPXMediaIdGeneral,0x100000};

/**
*  TUint}; indicates which rights an end user
*  does not have to perform on the media.
*  Non-permissible actions are defined in
*  TMPXGeneralNonPermissibleActions. Values
*  can be combined.
*/
const TMPXAttributeData KMPXMediaGeneralNonPermissibleActions={KMPXMediaIdGeneral,0x200000};

/**
*  TMPXItemId (as def'd in CollectionPath)
*  ID of item's container or container
*/
const TMPXAttributeData KMPXMediaGeneralContainerId={KMPXMediaIdGeneral,0x400000};

/**
*  Descriptor}; subtitle for this media
*/
const TMPXAttributeData KMPXMediaGeneralSubTitle={KMPXMediaIdGeneral,0x800000};

/**
* Copyright information
*/
const TMPXAttributeData KMPXMediaGeneralCopyright={KMPXMediaIdGeneral,0x1000000};

/**
* Flags stored in the database}; TUint
*/
const TMPXAttributeData KMPXMediaGeneralFlags={KMPXMediaIdGeneral,0x2000000};

/**
* Last playback position}; TInt
*/
const TMPXAttributeData KMPXMediaGeneralLastPlaybackPosition={KMPXMediaIdGeneral,0x4000000};

/**
* Last playback Time
*  TTime.Int64(). Storing as TTime instead TInt64
*  will result in compilation warning.
*/
const TMPXAttributeData KMPXMediaGeneralLastPlaybackTime={KMPXMediaIdGeneral,0x8000000};

/**
* Play Count}; TUInt
*/
const TMPXAttributeData KMPXMediaGeneralPlayCount={KMPXMediaIdGeneral,0x10000000};

/**
* Media container path
*/
const TMPXAttributeData KMPXMediaGeneralContainerPath={KMPXMediaIdGeneral,0x20000000};

/**
* Media collection path}; CMPXCollectionPath
*/
const TMPXAttributeData KMPXMediaGeneralPath={KMPXMediaIdGeneral,0x40000000};

/** TInt
*
* A single word of general value users defined
*
* In practice, this word can be used to hold a pointer to allocated
* memory; this makes that memory available to all code running on behalf
* of the same thread.
*
*/
const TMPXAttributeData KMPXMediaGeneralValue={KMPXMediaIdGeneral,0x80000000};

/**
* Every attribute
*/
const TMPXAttributeData KMPXMediaGeneralAll={KMPXMediaIdGeneral,0xffffffff};

/**
* Basic set of attributes, typically used for browsing where minimal data is
* required. Type, category, title, duration
*/
const TMPXAttributeData KMPXMediaGeneralBasic={KMPXMediaIdGeneral,0x01|0x02|0x20|0x40};

/** 
*  Attributes supported for KMPXMediaIdGeneral
*
*  @deprecated Use the above consts
*/
enum
    {
    /**
    *  TMPXGeneralType, see below for enum definition
    */
    EMPXMediaGeneralType=0x01,

    /**
    *  TMPXGeneralCategory, see below for enum definition
    */
    EMPXMediaGeneralCategory=0x02,

    /**
    *  Descriptor. File path if local (excluding scheme).
    */
    EMPXMediaGeneralUri=0x04,

    /**
    *  Descriptor. Drive, if local (and physical)
    */
    EMPXMediaGeneralDrive=0x08,

    /**
    *  TInt, in bytes
    */
    EMPXMediaGeneralSize=0x10,

    /**
    *  TInt.
    *  0 if static, KMaxTInt if continous stream. If container,
    *  then duration applies to sum of contents
    */
    EMPXMediaGeneralDuration=0x20,

    /**
    *  Descriptor
    */
    EMPXMediaGeneralTitle=0x40,

    /**
    *  TTime.Int64(). Storing as TTime instead TInt64
    *  will result in compilation warning.
    */
    EMPXMediaGeneralDate=0x80,

    /**
    *  Descriptor
    */
    EMPXMediaGeneralComment=0x100,

    /**
    *  Descriptor
    */
    EMPXMediaGeneralMimeType=0x200,

    /**
    *  CFbsBitmap - for now could
    *  be file path in future to enable easier
    *  porting of client (i.e. UI)
    */
    EMPXMediaGeneralThumbnail1=0x400,

    /**
    *  CFbsBitmap - for now could
    *  be file path in future to enable easier
    *  porting of client (i.e. UI)
    */
    EMPXMediaGeneralThumbnail2=0x800,

    /**
    *  TIconInfo - Bitmap
    */
    EMPXMediaGeneralIcon=0x1000,

    /**
    *  TBool
    */
    EMPXMediaGeneralRemote=0x2000,

    /**
    *  TBool
    */
    EMPXMediaGeneralSynchronized=0x4000,

    /**
    *  TBool
    */
    EMPXMediaGeneralDeleted=0x8000,

    /**
    * TBool
    */
    EMPXMediaGeneralModified = 0x10000,

    /**
    *  TInt. Number of items (if container, else
    *  would trivially be 1)
    */
    EMPXMediaGeneralCount=0x20000,

    /**
    *  TMPXItemId (as def'd in CollectionPath)
    *  ID of item
    */
    EMPXMediaGeneralId=0x40000,

    /**
    *  Collection id of this media
    */
    EMPXMediaGeneralCollectionId=0x80000,

    /**
    *  TBool, indicates if Uri is a broken link
    *  DEPRECATED - TO BE REMOVED IN INCREMENT 9 RELEASE
    *  This attribute has been replaced by
    *  KMPXMediaGeneralFlagsIsInvalid bit in
    *  EMPXMediaGeneralFlags
    */
    EMPXMediaGeneralBrokenLink=0x100000,

    /**
    *  TUint, indicates which rights an end user
    *  does not have to perform on the media.
    *  Non-permissible actions are defined in
    *  TMPXGeneralNonPermissibleActions. Values
    *  can be combined.
    */
    EMPXMediaGeneralNonPermissibleActions=0x200000,

   /**
    *  TMPXItemId (as def'd in CollectionPath)
    *  ID of item's container or container
    */
    EMPXMediaGeneralContainerId=0x400000,

    /**
    *  Descriptor, subtitle for this media
    */
    EMPXMediaGeneralSubTitle=0x800000,

    /**
    * Copyright information
    */
    EMPXMediaGeneralCopyright=0x1000000,

    /**
    * Flags stored in the database, TUint
    */
    EMPXMediaGeneralFlags=0x2000000,

    /**
    * Last playback position, TInt
    */
    EMPXMediaGeneralLastPlaybackPosition=0x4000000,

    /**
    * Last playback Time
    *  TTime.Int64(). Storing as TTime instead TInt64
    *  will result in compilation warning.
    */
    EMPXMediaGeneralLastPlaybackTime=0x8000000,

    /**
    * Play Count, TUInt
    */
    EMPXMediaGeneralPlayCount=0x10000000,

    /**
    * Media container path
    */
    EMPXMediaContainerPath=0x20000000,

    /**
    * Media collection path, CMPXCollectionPath
    */
    EMPXMediaPath=0x40000000,

    /**
    * Every attribute
    */
    EMPXMediaGeneralAll=0xffffffff,

    /**
    *  Basic set of attributes, typically used for
    *  browsing where minimal data is required
    */
    EMPXMediaGeneralBasic=EMPXMediaGeneralType|
                          EMPXMediaGeneralCategory|
                          EMPXMediaGeneralTitle|
                          EMPXMediaGeneralDuration
    };


enum TMPXGeneralType
    {
    EMPXNoType,
    EMPXGroup,
    EMPXItem,
    EMPXOtherType
    };

enum TMPXGeneralCategory
    {
    EMPXNoCategory,
    EMPXCollection,
    EMPXPlaylist,
    EMPXSong,
    EMPXPodcast,
    EMPXArtist,
    EMPXAlbum,
    EMPXGenre,
    EMPXComposer,
    EMPXFile,
    EMPXImage,
    EMPXVideo,
    EMPXAbstractAlbum,
    EMPXMessage,
    EMPXTag,
    EMPXCommand,
    EMPXMonth,
    EMPXOther
    };

enum TMPXGeneralNonPermissibleActions
    {
    EMPXRead=0x01,
    EMPXWrite=0x02,
    EMPXCache=0x04
    };

const TUint KMPXMediaGeneralFlagsDriveInfo=0x1F; // lower 5 bits

// Top bit indicates whether to set or reset flag value.
// If this bit is set, then the parameter that is passed in will
// specify which bits to be set.
// If this bit is unset, then the parameter that is passed in will
// specify which bits will be unset.
const TUint KMPXMediaGeneralFlagsSetOrUnsetBit=0x80000000; // top bit
const TUint KMPXMediaGeneralFlagsIsCorrupted=0x40000000; // second bit
const TUint KMPXMediaGeneralFlagsIsInvalid=0x20000000; // third bit
const TUint KMPXMediaGeneralFlagsIsDrmProtected=0x10000000; // forth bit
const TUint KMPXMediaGeneralFlagsIsDrmLicenceInvalid=0x08000000; // fifth bit
const TUint KMPXMediaGeneralFlagsIsVirtual=0x04000000; // sixth bit
const TUint KMPXMediaGeneralFlagsIsMissingDetails=0x02000000; // seventh bit

/*
* Simple structure to hold icon info
*/
NONSHARABLE_STRUCT( TIconInfo )
    {
    TInt bitmapId;
    TInt maskId;
    TFileName bmpfile;
    };

#endif // CMPXMEDIAGENERALDEFS_H
