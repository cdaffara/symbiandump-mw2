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
* Description: encapsulates information about a landmark database.
*
*
*/


// INCLUDE FILES
#include <EPos_LandmarksErrors.h>
#include "EPos_HPosLmDatabaseInfo.h"


// CONSTANTS
_LIT(KProtocolSeparator, "://");


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
HPosLmDatabaseInfo::HPosLmDatabaseInfo(const TDesC& aDatabaseUri)
    : iDatabaseMedia(EMediaUnknown),
      iDatabaseDrive(0),
      iIsDefault(EFalse)
    {
    SetDatabaseUri(aDatabaseUri);
    }

HPosLmDatabaseInfo::HPosLmDatabaseInfo(const HPosLmDatabaseInfo& aDatabaseInfo)
    : iSettings(aDatabaseInfo.iSettings),
      iDatabaseMedia(aDatabaseInfo.iDatabaseMedia),
      iDatabaseDrive(aDatabaseInfo.iDatabaseDrive),
      iIsDefault(aDatabaseInfo.iIsDefault)
    {
    SetDatabaseUri(aDatabaseInfo.DatabaseUri());
    }

// Two-phased constructor.
EXPORT_C HPosLmDatabaseInfo* HPosLmDatabaseInfo::NewLC(
    const TDesC& aDatabaseUri)
    {
    HPosLmDatabaseInfo* self = HPosLmDatabaseInfo::NewL(aDatabaseUri);
    CleanupStack::PushL(self);
    return self;
    }

// Two-phased constructor.
EXPORT_C HPosLmDatabaseInfo* HPosLmDatabaseInfo::NewL(
    const TDesC& aDatabaseUri)
    {
    TAny* address = AllocateL(aDatabaseUri);
    HPosLmDatabaseInfo* self = new(address) HPosLmDatabaseInfo(aDatabaseUri);

    return self;
    }

// Two-phased constructor.
EXPORT_C HPosLmDatabaseInfo* HPosLmDatabaseInfo::NewLC(
    const HPosLmDatabaseInfo& aDatabaseInfo)
    {
    HPosLmDatabaseInfo* self = HPosLmDatabaseInfo::NewL(aDatabaseInfo);
    CleanupStack::PushL(self);
    return self;
    }

// Two-phased constructor.
EXPORT_C HPosLmDatabaseInfo* HPosLmDatabaseInfo::NewL(
    const HPosLmDatabaseInfo& aDatabaseInfo)
    {
    TAny* address = AllocateL(aDatabaseInfo.DatabaseUri());
    HPosLmDatabaseInfo* self = new(address) HPosLmDatabaseInfo(aDatabaseInfo);

    return self;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::DatabaseUri
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC HPosLmDatabaseInfo::DatabaseUri() const
    {
    TInt bufferOffset = BufferOffset();

    // get length
    const TInt length =
        *(reinterpret_cast<const TInt*>(&iBuffer[bufferOffset]));

    return TPtrC(
        reinterpret_cast<const TUint16*>(&iBuffer[bufferOffset +
                                                  sizeof(TInt)]),
        length);
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::Protocol
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC HPosLmDatabaseInfo::Protocol() const
    {
    TPtrC uri = DatabaseUri();

    TInt separatorStart = uri.Find(KProtocolSeparator);

    return separatorStart == KErrNotFound ?
                             TPtrC() :
                             uri.Left(separatorStart);
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::IsDefault
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool HPosLmDatabaseInfo::IsDefault() const
    {
    return iIsDefault;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::DatabaseMedia
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TMediaType HPosLmDatabaseInfo::DatabaseMedia() const
    {
    return iDatabaseMedia;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::DatabaseDrive
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TChar HPosLmDatabaseInfo::DatabaseDrive() const
    {
    return iDatabaseDrive;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::Settings
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C const TPosLmDatabaseSettings& HPosLmDatabaseInfo::Settings() const
    {
    return iSettings;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::Settings
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPosLmDatabaseSettings& HPosLmDatabaseInfo::Settings()
    {
    return iSettings;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::Size
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt HPosLmDatabaseInfo::Size() const
    {
    TInt bufferOffset = BufferOffset();

    // get length
    const TInt length =
        *(reinterpret_cast<const TInt*>(&iBuffer[bufferOffset]));


    return _FOFF(HPosLmDatabaseInfo, iBuffer[0]) +
           bufferOffset +
           sizeof(TInt) +
           length * 2;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::SetDefault
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void HPosLmDatabaseInfo::SetDefault(TBool aIsDefault)
    {
    iIsDefault = aIsDefault;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::SetMediaType
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void HPosLmDatabaseInfo::SetMediaType(TMediaType aMediaType)
    {
    __ASSERT_ALWAYS(aMediaType >= EMediaNotPresent &&
                    aMediaType <= EMediaNANDFlash,
                    Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));
    iDatabaseMedia = aMediaType;
    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::SetDatabaseDrive
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void HPosLmDatabaseInfo::SetDatabaseDrive(TChar aDatabaseDrive)
    {
    __ASSERT_ALWAYS((aDatabaseDrive >= 'a' && aDatabaseDrive <= 'z') ||
                    (aDatabaseDrive >= 'A' && aDatabaseDrive <= 'Z') ||
                    aDatabaseDrive == 0,
                    Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));
    iDatabaseDrive = aDatabaseDrive;
    }


// ---------------------------------------------------------
// HPosLmDatabaseInfo::AllocateL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAny* HPosLmDatabaseInfo::AllocateL(const TDesC& aDatabaseUri)
    {
    TInt bufferOffset = BufferOffset();

    TUint requiredBufferSize = bufferOffset +
                               sizeof(TInt) +
                               aDatabaseUri.Size();

#if defined(__GCCE__)
  // allocate memory
    return User::AllocL(_FOFF(HPosLmDatabaseInfo,
                             iBuffer) + requiredBufferSize * sizeof (typeof(iBuffer)));
#else
    // allocate memory
    return User::AllocL(_FOFF(HPosLmDatabaseInfo,
                             iBuffer[requiredBufferSize]));
#endif


    }


// ---------------------------------------------------------
// HPosLmDatabaseInfo::SetDatabaseUriL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void HPosLmDatabaseInfo::SetDatabaseUri(const TDesC& aDatabaseUri)
    {

    TInt bufferOffset = BufferOffset();

    // extract and write length
    TInt length = aDatabaseUri.Length();
    TInt* buffer = reinterpret_cast<TInt*>(&iBuffer[bufferOffset]);
    *buffer = length;

    // extract and write data
    TUint16* databuffer =
        reinterpret_cast<TUint16*>(&iBuffer[bufferOffset + sizeof(TInt)]);
    for (TInt i = 0; i < length; i++)
        {
        databuffer[i] = aDatabaseUri[i];
        }

    }

// ---------------------------------------------------------
// HPosLmDatabaseInfo::BufferOffset
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt HPosLmDatabaseInfo::BufferOffset()
    {
    return (4 - _FOFF(HPosLmDatabaseInfo, iBuffer[0])&3) &3;
//    return  _FOFF(HPosLmDatabaseInfo, iBuffer[0])&3 ?
//            4 - _FOFF(HPosLmDatabaseInfo, iBuffer[0])&3 :
//            0;
    }

//  End of File
