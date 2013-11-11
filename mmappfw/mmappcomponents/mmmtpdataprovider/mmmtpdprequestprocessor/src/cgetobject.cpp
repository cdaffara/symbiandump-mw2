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
* Description:  Implement the operation: GetObject
*
*/


#include <mtp/cmtptypefile.h>

#include "cgetobject.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// Verification data for the GetNumObjects request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPGetObjectPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrFile,
        0,
        0,
        0
        }
    };

// -----------------------------------------------------------------------------
// CGetObject::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& /*aDpConfig*/ )
    {
    CGetObject* self = new ( ELeave ) CGetObject( aFramework, aConnection );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CGetObject::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CGetObject::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CGetObject::~CGetObject
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObject::~CGetObject()
    {
    PRINT( _L( "MM MTP => CGetObject::~CGetObject" ) );
    delete iFileObject;
    PRINT( _L( "MM MTP <= CGetObject::~CGetObject" ) );
    }

// -----------------------------------------------------------------------------
// CGetObject::CGetObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetObject::CGetObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof( KMTPGetObjectPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPGetObjectPolicy ),
        iError( EMTPRespCodeOK )
    {
    PRINT( _L( "Operation: GetObject(0x1009)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObject::ServiceL
// GetObject request handler
// -----------------------------------------------------------------------------
//
void CGetObject::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetObject::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);

    // Get the objectinfo
    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CGetObject::ServiceL objectHandle = 0x%x" ), objectHandle );

    // NOTE: shouldn't be deleted in destructor, don't have the ownership
    iObjectInfo = iRequestChecker->GetObjectInfo( objectHandle );

    if ( iObjectInfo != NULL )
        {
        // Get the file
        GetObjectL( iObjectInfo->DesC( CMTPObjectMetaData::ESuid  ) );

        // Send the data (file)
        SendDataL( *iFileObject );
        }
    else
        {
        // The object handle has already been checked, so an invalid handle can
        // only occur if it was invalidated during a context switch between
        // the validation time and now.
        iError = EMTPRespCodeInvalidObjectHandle;
        }

    PRINT1( _L( "MM MTP <= CGetObject::ServiceL iError = 0x%x" ), iError );
    }

// -----------------------------------------------------------------------------
// CGetObject::GetObjectL
//
// -----------------------------------------------------------------------------
//
void CGetObject::GetObjectL( const TDesC& aFileName )
    {
    PRINT( _L( "MM MTP => CGetObject::GetObjectL" ) );

    // Create the file object
    delete iFileObject;
    iFileObject = NULL;
    iFileObject = CMTPTypeFile::NewL( iFramework.Fs(), aFileName, EFileRead  );

    PRINT( _L( "MM MTP <= CGetObject::GetObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObject::DoHandleResponsePhaseL
// Handle the response phase of the current request
// -----------------------------------------------------------------------------
//
TBool CGetObject::DoHandleResponsePhaseL()
    {
    PRINT( _L( "MM MTP => CGetObject::DoHandleResponsePhaseL" ) )

    TMTPResponseCode responseCode = ( iCancelled ? EMTPRespCodeIncompleteTransfer : iError );
    SendResponseL( responseCode );

    PRINT1( _L( "MM MTP <= CGetObject::DoHandleResponsePhaseL responseCode = 0x%x" ),  responseCode );

    return EFalse;
    }

// end of file
