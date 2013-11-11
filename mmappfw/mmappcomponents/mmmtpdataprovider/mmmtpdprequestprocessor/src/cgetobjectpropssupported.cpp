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
* Description:  Implement operation GetObjectPropsSupported
*
*/


#include <mtp/cmtptypearray.h>

#include "cgetobjectpropssupported.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// Verification data for the GetObjectPropSupported request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPGetObjectPropSupportedPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeFormatCode,
        EMTPElementAttrNone,
        1,
        EMTPFormatCodeUndefined,
        0
        },
    };

// -----------------------------------------------------------------------------
// CGetObjectPropsSupported::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetObjectPropsSupported::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetObjectPropsSupported* self = new ( ELeave ) CGetObjectPropsSupported( aFramework, aConnection, aDpConfig );
    // CleanupStack::PushL( self );
    // self->ConstructL();
    // CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropsSupported::~CGetObjectPropsSupported(
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropsSupported::~CGetObjectPropsSupported()
    {
    delete iObjectPropsSupported;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropsSupported::~CGetObjectPropsSupported(
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetObjectPropsSupported::CGetObjectPropsSupported( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CRequestProcessor( aFramework,
        aConnection,
        sizeof( KMTPGetObjectPropSupportedPolicy ) / sizeof( TMTPRequestElementInfo ),
        KMTPGetObjectPropSupportedPolicy ),
    iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: GetObjectPropsSupported(0x9801)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropsSupported::ServiceL
// GetObjectPropSupported request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropsSupported::ServiceL()
    {
    delete iObjectPropsSupported;
    iObjectPropsSupported = NULL;
    iObjectPropsSupported = CMTPTypeArray::NewL( EMTPTypeAUINT16 );

    TUint32 formatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CGetObjectPropsSupported::ServiceL formatCode = 0x%x" ), formatCode );

    const RArray<TUint>* format = iDpConfig.GetSupportedFormat();
    TInt count = format->Count();

    TInt i = 0;
    for ( ; i < count; i++ )
        {
        if ( formatCode == (*format)[i] ) // only process the support format
            {
            const RArray<TUint>* properties;
            properties = iDpConfig.GetSupportedPropertiesL( formatCode ) ;

            TInt count = properties->Count();

            PRINT1( _L( "MM MTP <> CGetObjectPropsSupported::ServiceL properties count = %d"), count );
            for ( TInt j = 0; j < count; j++ )
                {
                iObjectPropsSupported->AppendUintL( (*properties)[j] );
                }

            break;
            }
        }

    if( i == count )
        SendResponseL( EMTPRespCodeInvalidObjectFormatCode );
    else
        SendDataL( *iObjectPropsSupported );
    }

// end of file
