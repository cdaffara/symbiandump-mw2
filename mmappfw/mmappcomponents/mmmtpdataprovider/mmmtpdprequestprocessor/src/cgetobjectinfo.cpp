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
* Description:  Implement operation: GetObjectInfo
*
*/


#include <mtp/cmtptypeobjectinfo.h>
#include <f32file.h>

#include "mmmtpdputility.h"
#include "cgetobjectinfo.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "tobjectdescription.h"


// -----------------------------------------------------------------------------
// Verification data for GetObjectInfo request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPGetObjectInfoPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrNone,
        0,
        0,
        0
        }
    };

// -----------------------------------------------------------------------------
// CGetObjectInfo::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetObjectInfo::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetObjectInfo* self = new ( ELeave ) CGetObjectInfo( aFramework, aConnection, aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CGetObjectInfo::~CGetObjectInfo
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectInfo::~CGetObjectInfo()
    {
    delete iObjectInfo;
    }

// -----------------------------------------------------------------------------
// CGetObjectInfo::CGetObjectInfo
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetObjectInfo::CGetObjectInfo( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CRequestProcessor( aFramework,
        aConnection,
        sizeof ( KMTPGetObjectInfoPolicy ) / sizeof( TMTPRequestElementInfo ),
        KMTPGetObjectInfoPolicy ),
    iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: GetObjectInfo(0x1008)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectInfo::ConstructL
// Second-phase construction
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectInfo::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CGetObjectInfo::ServiceL
// GetObjectInfo request handler
// -----------------------------------------------------------------------------
//
void CGetObjectInfo::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetObjectInfo::ServiceL" ) );

    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);

    delete iObjectInfo;
    iObjectInfo = NULL;
    iObjectInfo = CMTPTypeObjectInfo::NewL();

    BuildObjectInfoL();

    SendDataL( *iObjectInfo );

    PRINT( _L( "MM MTP <= CGetObjectInfo::ServiceL" ) );
    }

void CGetObjectInfo::BuildObjectInfoL()
    {
    PRINT( _L( "MM MTP => CGetObjectInfo::BuildObjectInfoL" ) );
    // Get the request information
    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CGetObjectInfo::BuildObjectInfoL The object handle being queried is: 0x%x" ), objectHandle );

    CMTPObjectMetaData* object = iRequestChecker->GetObjectInfo( objectHandle );
    __ASSERT_DEBUG( object, Panic( EMmMTPDpObjectNull ) );

    // StorageID
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EStorageID,
        object->Uint( CMTPObjectMetaData::EStorageId ) );

    // Object Format
    iObjectInfo->SetUint16L( CMTPTypeObjectInfo::EObjectFormat,
        object->Uint( CMTPObjectMetaData::EFormatCode ) );
    PRINT1( _L( "MM MTP <> CGetObjectInfo::BuildObjectInfoL objet formatCode = 0x%x" ),
        object->Uint( CMTPObjectMetaData::EFormatCode ) );

    // Protection Status
    TPtrC suid( object->DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP <> CGetObjectInfo::BuildObjectInfo suid = %S" ), &suid );
    TUint16 status = MmMtpDpUtility::GetProtectionStatusL( iFramework.Fs(), suid );
    iObjectInfo->SetUint16L( CMTPTypeObjectInfo::EProtectionStatus, status);

    // Object Compressed Size
    TInt size = MmMtpDpUtility::GetObjectSizeL( iFramework.Fs(), suid );
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EObjectCompressedSize, size );

    // *Thumb Format
    iObjectInfo->SetUint16L( CMTPTypeObjectInfo::EThumbFormat, 0 );
    // *Thumb Compressed Size
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EThumbCompressedSize, 0 );
    // *Thumb Pix Width
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EThumbPixWidth, 0 );
    // *Thumb Pix Height
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EThumbPixHeight, 0 );

    TUint32 width(0);
    TUint32 height(0);
    TInt err = KErrNone;
    TRAP( err, iDpConfig.GetWrapperL().GetImageObjPropL( *object, width, height ) );
    if( err != KErrNone )
        PRINT1( _L( "MM MTP <> CGetObjectInfo::BuildObjectInfoL TRAP iWrapper.GetImageObjPropL err = %d" ), err );

    // Image Pix Width
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EImagePixWidth, width );
    // Image Pix Height
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EImagePixHeight, height );

    // Image Bit Depth
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EImageBitDepth, 0 );
    // Parent Object
    TUint32 parentHandle = object->Uint( CMTPObjectMetaData::EParentHandle );
    // refer to 5.3.1.9 of MTP Spec 1.0
    if ( parentHandle == KMTPHandleNoParent )
        {
        parentHandle = KMTPHandleNone;
        }
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EParentObject, parentHandle );
    // Association Type
    iObjectInfo->SetUint16L( CMTPTypeObjectInfo::EAssociationType, 0 );
    // Association Description
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::EAssociationDescription, 0 );
    // *Sequence Number
    iObjectInfo->SetUint32L( CMTPTypeObjectInfo::ESequenceNumber, 0 );
    // Filename
    TParsePtrC parse( suid );
    iObjectInfo->SetStringL( CMTPTypeObjectInfo::EFilename, parse.NameAndExt() );

    TBuf<KMtpMaxDateTimeStringLength> date;
    MmMtpDpUtility::GetObjectDateModifiedL( iFramework.Fs(), suid, date );

    PRINT1( _L( "MM MTP <> CGetObjectInfo::BuildObjectInfo date is %S" ), &date );

    // Date Created
    iObjectInfo->SetStringL( CMTPTypeObjectInfo::EDateCreated, date );
    // Date Modified
    iObjectInfo->SetStringL( CMTPTypeObjectInfo::EDateModified, date );
    // Keywords, not supported
    iObjectInfo->SetStringL( CMTPTypeObjectInfo::EKeywords, KNullDesC );

    PRINT( _L( "MM MTP <= CGetObjectInfo::BuildObjectInfoL" ) );
    }

// end of file
