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
* Description:  Implement the operation: GetInterdenpendentPropDesc
*
*/

#include <mtp/cmtptypeinterdependentpropdesc.h>

#include "cgetinterdependentpropdesc.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "cdescriptionutility.h"

// -----------------------------------------------------------------------------
// CGetInterdependentPropDesc::NewL
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetInterdependentPropDesc::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetInterdependentPropDesc* self = new ( ELeave ) CGetInterdependentPropDesc( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CGetInterdependentPropDesc::~CGetInterdependentPropDesc
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetInterdependentPropDesc::~CGetInterdependentPropDesc()
    {
    delete iDataset;
    iDataset = NULL;
    }

// -----------------------------------------------------------------------------
// CMTPGetInterDependentPropDesc::CMTPGetInterDependentPropDesc
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetInterdependentPropDesc::CGetInterdependentPropDesc( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL ),
        iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: GetInterdependentPropDesc(0x9807)" ) );
    }

// -----------------------------------------------------------------------------
// CMediaDpMtpGetInterDependentPropDesc::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CGetInterdependentPropDesc::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CGetInterdependentPropDesc::CheckRequestL
// Verify the reqeust and returns it
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetInterdependentPropDesc::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CGetInterdependentPropDesc::CheckRequestL" ) );

    TMTPResponseCode responseCode = CRequestProcessor::CheckRequestL();
    iFormatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );

    // check if it is what dp supported
    responseCode = EMTPRespCodeInvalidObjectFormatCode;

    const RArray<TUint>* format = iDpConfig.GetSupportedFormat();
    TInt count = format->Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iFormatCode == ( *format )[i] )
            {
            responseCode = EMTPRespCodeOK;
            break;
            }
        }

    // Clear the data set.
    delete iDataset;
    iDataset = NULL;

    PRINT2( _L( "MM MTP <= CGetInterdependentPropDesc::CheckRequestL, iFormatCode = 0x%x, responseCode = 0x%x" ),
        iFormatCode,
        responseCode );

    return responseCode;
    }

// -----------------------------------------------------------------------------
// CGetInterdependentPropDesc::ServiceL
// service a request at request phase
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetInterdependentPropDesc::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetInterdependentPropDesc::ServiceL" ) );

    iDataset = iDpConfig.DescriptionUtility()->NewInterdepentPropDescL( iDpConfig, iFormatCode );

    // Send the dataset.
    SendDataL( *iDataset );
    PRINT( _L( "MM MTP <= CGetInterdependentPropDesc::ServiceL" ) );
    }

// end of file
