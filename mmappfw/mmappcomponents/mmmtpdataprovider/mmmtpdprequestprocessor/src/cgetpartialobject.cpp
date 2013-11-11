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
* Description:  Implement the operation: GetPartialObject
*
*/


#include <mtp/cmtptypefile.h>

#include "cgetpartialobject.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdpconfig.h"

/**
* Verification data for the GetPartialObject request
*/
const TMTPRequestElementInfo KMTPGetPartialObjectPolicy[] =
    {
        {TMTPTypeRequest::ERequestParameter1, EMTPElementTypeObjectHandle, EMTPElementAttrFile, 0, 0, 0}
    };

// -----------------------------------------------------------------------------
// CGetPartialObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetPartialObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& /*aDpConfig*/ )
    {
    CGetPartialObject* self = new (ELeave) CGetPartialObject( aFramework, aConnection );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::~CGetPartialObject()
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetPartialObject::~CGetPartialObject()
    {
    delete iFileObject;
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::CGetPartialObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetPartialObject::CGetPartialObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof( KMTPGetPartialObjectPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPGetPartialObjectPolicy ),
        iFramework ( aFramework )
    {
    PRINT( _L( "Operation: GetPartialObject(0x101B)" ) );
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::ConstructL()
// Second-phase construction
// -----------------------------------------------------------------------------
//
void CGetPartialObject::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::CheckRequestL
// Check the GetPartialObject reqeust
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetPartialObject::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CGetPartialObject::CheckRequestL" ) );
    TMTPResponseCode result = CRequestProcessor::CheckRequestL();
    if( result == EMTPRespCodeOK && !VerifyParametersL() )
        {
        result = EMTPRespCodeInvalidParameter;
        }

    PRINT1( _L( "MM MTP <= CGetPartialObject::CheckRequestL result = ox%x" ), result );
    return result;
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::VerifyParametersL
// Verify if the parameter of the request (i.e. offset) is good.
// -----------------------------------------------------------------------------
//
TBool CGetPartialObject::VerifyParametersL()
    {
    PRINT( _L( "MM MTP => CGetPartialObject::VerifyParametersL" ) );

    __ASSERT_DEBUG( iRequestChecker, Panic( EMmMTPDpRequestCheckNull ) );
    TBool result = EFalse;
    iObjectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    iOffset = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    iPartialDataLength = Request().Uint32( TMTPTypeRequest::ERequestParameter3 );

    PRINT3( _L( "MM MTP <> CGetPartialObject::VerifyParametersL iObjectHandle = 0x%x, iOffset = 0x%x, iMaxLength = 0x%x " ),
        iObjectHandle,
        iOffset,
        iPartialDataLength );

    //get object info, but do not have the ownship of the object
    CMTPObjectMetaData* objectInfo = iRequestChecker->GetObjectInfo( iObjectHandle );
    __ASSERT_DEBUG( objectInfo, Panic( EMmMTPDpObjectNull ) );

    const TDesC& suid( objectInfo->DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP <> CGetPartialObject::VerifyParametersL suid = %S" ), &suid );

    TEntry fileEntry;
    User::LeaveIfError( iFramework.Fs().Entry( suid, fileEntry ) );
    if ( iOffset < fileEntry.FileSize() )
        {
        result = ETrue;
        }

    PRINT1( _L( "MM MTP <= CGetPartialObject::VerifyParametersL result = %d" ), result );
    return result;
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::ServiceL
// GetPartialObject request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetPartialObject::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetPartialObject::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);

    // Get file information
    CMTPObjectMetaData* objectInfo = iRequestChecker->GetObjectInfo( iObjectHandle );
    __ASSERT_DEBUG( objectInfo, Panic( EMmMTPDpObjectNull ) );

    TPtrC fileName = objectInfo->DesC( CMTPObjectMetaData::ESuid );

    iFileObject = CMTPTypeFile::NewL( iFramework.Fs(),
        fileName,
        ( TFileMode ) ( EFileRead | EFileShareReadersOnly ),
        iPartialDataLength,
        iOffset );

    SendDataL( *iFileObject );

    PRINT( _L( "MM MTP <= CGetPartialObject::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CGetPartialObject::DoHandleResponsePhaseL
// Signal to the initiator how much data has been sent
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CGetPartialObject::DoHandleResponsePhaseL()
    {
    PRINT( _L( "MM MTP => CGetPartialObject::DoHandleResponsePhaseL" ) );

    TUint32 dataLength = iFileObject->GetByteSent();
    SendResponseL( EMTPRespCodeOK, 1, &dataLength );

    PRINT1( _L( "MM MTP <= CGetPartialObject::DoHandleResponsePhaseL dataLength = %d" ), dataLength );
    return EFalse;
    }

// end of file
