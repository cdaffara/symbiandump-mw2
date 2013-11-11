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
* Description: Request processor which handle common property description
*
*/

#include <mtp/cmtptypeobjectpropdesc.h>

#include "cgetobjectpropdesc.h"
#include "cdescriptionutility.h"
#include "mmmtpdpconfig.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::NewL
// Constructor
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetObjectPropDesc::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetObjectPropDesc* self = new ( ELeave ) CGetObjectPropDesc( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::~CGetObjectPropDesc
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectPropDesc::~CGetObjectPropDesc()
    {
    delete iPropertyDesc;
    iPropertyDesc = NULL;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::CGetObjectPropDesc
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetObjectPropDesc::CGetObjectPropDesc( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL ),
        iDpConfig( aDpConfig ),
        iPropertyDesc( NULL )
    {
    PRINT( _L( "Operation: GetObjectPropDesc(0x9802)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::ConstructL
// The second phase constructor
// -----------------------------------------------------------------------------
//
void CGetObjectPropDesc::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckRequestL
// Verify the reqeust and returns it
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetObjectPropDesc::CheckRequestL()
    {
    TMTPResponseCode response = CRequestProcessor::CheckRequestL();

    iPropCode = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    iFormatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter2 );
    PRINT2( _L( "MM MTP <> CGetObjectPropDesc::CheckPropCode, propCode = 0x%x, iFormatCode = 0x%x" ),
        iPropCode,
        iFormatCode );

    if ( response == EMTPRespCodeOK )
        {
        response = CheckFormatL();
        }

    if ( response == EMTPRespCodeOK )
        {
        response = CheckPropCodeL();
        }

    PRINT1( _L("MM MTP <= CGetObjectPropDesc::CheckRequestL response = 0x%x"), response );
    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropList::CheckFormatL
// Ensure the object format operation parameter is valid
// -----------------------------------------------------------------------------
//
TMTPResponseCode CGetObjectPropDesc::CheckFormatL() const
    {
    TMTPResponseCode response = EMTPRespCodeInvalidObjectFormatCode;

    if( iFormatCode != KMTPFormatsAll )
        {
        const RArray<TUint>* format = iDpConfig.GetSupportedFormat();
        TInt count = format->Count();

        for ( TInt i = 0; i < count; i++ )
            {
            if ( iFormatCode == (*format)[i] )
                {
                response = EMTPRespCodeOK;
                break;
                }
            }
        }

    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::CheckPropCodeL
// Ensure the object prop code operation parameter is valid
// -----------------------------------------------------------------------------
//
TMTPResponseCode CGetObjectPropDesc::CheckPropCodeL() const
    {
    TMTPResponseCode response = EMTPRespCodeInvalidObjectPropCode;

    if ( iPropCode != KMTPNotSpecified32 && iPropCode != KMTPObjectPropCodeAll )
        {
        const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( iFormatCode );

        const TInt count = properties->Count();
        for( TInt i = 0; i < count; i++ )
            {
            if ( (*properties)[i] == iPropCode )
                {
                response = EMTPRespCodeOK;
                break;
                }
            }
        }

    return response;
    }

// -----------------------------------------------------------------------------
// CGetObjectPropDesc::ServiceL
// GetObjectPropDesc request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetObjectPropDesc::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetObjectPropDesc::ServiceL" ) );

    delete iPropertyDesc;
    iPropertyDesc = NULL;

    iPropertyDesc = iDpConfig.DescriptionUtility()->NewCommonObjectPropertyL( iPropCode );

    // if iPropertyDesc == NULL, iPropCode is not common property but dp specific one.
    if( iPropertyDesc == NULL )
        iPropertyDesc = iDpConfig.DescriptionUtility()->NewSpecificPropDescL( iFormatCode, iPropCode );

    __ASSERT_DEBUG( iPropertyDesc, Panic( EMmMTPDpObjectPropertyNull ) );

    // Set group code
    TUint32 groupCode = iDpConfig.DescriptionUtility()->GetGroupCode( iPropCode );
    PRINT1( _L("MM MTP <> CGetObjectPropDesc::ServiceL, groupCode = 0x%x"), groupCode );
    iPropertyDesc->SetUint32L( CMTPTypeObjectPropDesc::EGroupCode, groupCode );

    SendDataL( *iPropertyDesc );

    PRINT( _L( "MM MTP <= CGetObjectPropDesc::ServiceL" ) );
    }

// end of file
