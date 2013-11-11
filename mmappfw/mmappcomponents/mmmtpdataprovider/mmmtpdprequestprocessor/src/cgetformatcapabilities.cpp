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

#include <mtp/cmtptypeinterdependentpropdesc.h>
#include <mtp/cmtptypeobjectpropdesc.h>

#include "cgetformatcapabilities.h"
#include "cdescriptionutility.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdplogger.h"

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::NewL
// Constructor
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetFormatCapabilities::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetFormatCapabilities* self = new ( ELeave ) CGetFormatCapabilities( aFramework, aConnection, aDpConfig );

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::CGetFormatCapabilities
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetFormatCapabilities::CGetFormatCapabilities( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework, aConnection, 0, NULL ),
        iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: CGetFormatCapabilities(0x930A)" ) );
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::ConstructL
// The second phase constructor
// -----------------------------------------------------------------------------
//
void CGetFormatCapabilities::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::~CGetFormatCapabilities
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetFormatCapabilities::~CGetFormatCapabilities()
    {
    delete iCapabilityList;
    iCapabilityList = NULL;
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::CheckRequestL
// Ensures the object prop code operation parameter is valid
// -----------------------------------------------------------------------------
//
EXPORT_C TMTPResponseCode CGetFormatCapabilities::CheckRequestL()
    {
    PRINT( _L( "MM MTP => CGetFormatCapabilities::CheckRequestL" ) );
    TMTPResponseCode response = EMTPRespCodeOK;
    iFormatCode = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );

    if ( iFormatCode != KMTPFormatsAll )
        {
        response = EMTPRespCodeInvalidObjectFormatCode;

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

    PRINT1( _L( "MM MTP <= CGetFormatCapabilities::CheckRequestL, response = 0x%x" ), response );
    return response;
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::ServiceL
// GetFormatCapabilities request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CGetFormatCapabilities::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetFormatCapabilities::ServiceL" ) );

    delete iCapabilityList;
    iCapabilityList = NULL;

    iCapabilityList = CMTPTypeFormatCapabilityList::NewL();

    if ( KMTPFormatsAll == iFormatCode )
        {
        const RArray<TUint>* formatArray = iDpConfig.GetSupportedFormat();
        TInt count = formatArray->Count();
        for( TInt i = 0; i < count; i++ )
            {
            PRINT( _L( "MM MTP <> CGetFormatCapabilities::ServiceL, KMTPFormatsAll!!!" ) );

            iFormatCode = ( *formatArray )[i];
            ServiceOneFormatCapabilitiesL( iFormatCode );
            }
        }
    else
        ServiceOneFormatCapabilitiesL( iFormatCode );

    SendDataL( *iCapabilityList );
    PRINT( _L( "MM MTP <= CGetFormatCapabilities::ServiceL" ) );
    }

void CGetFormatCapabilities::ServiceOneFormatCapabilitiesL( TUint aFormatCode )
    {
    CMTPTypeInterdependentPropDesc* interdependentPropDesc = iDpConfig.DescriptionUtility()->NewInterdepentPropDescL( iDpConfig, aFormatCode );
    CMTPTypeFormatCapability* frmCap = CMTPTypeFormatCapability::NewL( aFormatCode, interdependentPropDesc );

    const RArray<TUint>* properties = iDpConfig.GetSupportedPropertiesL( aFormatCode );
    TInt num = properties->Count();
    for ( TInt i = 0; i < num; i++ )
        {
        CMTPTypeObjectPropDesc* propertyDesc = ServiceOnePropDescL( ( *properties )[i] );
        if ( propertyDesc != NULL )
            {
            frmCap->AppendL( propertyDesc );
            }
        }
    iCapabilityList->AppendL( frmCap );
    }

// -----------------------------------------------------------------------------
// CGetFormatCapabilities::ServicePropDescL
// Get MTPTypeObjectPropDesc according to property code
// -----------------------------------------------------------------------------
//
CMTPTypeObjectPropDesc* CGetFormatCapabilities::ServiceOnePropDescL( TUint16 aPropCode )
    {
    PRINT1( _L( "MM MTP => CGetFormatCapabilities::ServicePropDescL, aPropCode = 0x%x" ), aPropCode );

    // implement just like what we did in CGetpropertyDescc::ServiceL
    CMTPTypeObjectPropDesc* propertyDesc =
        iDpConfig.DescriptionUtility()->NewCommonObjectPropertyL( aPropCode );

    if ( propertyDesc == NULL )
        propertyDesc = iDpConfig.DescriptionUtility()->NewSpecificPropDescL( iFormatCode, aPropCode );

    if ( propertyDesc != NULL )
        {
        TUint32 groupCode = iDpConfig.DescriptionUtility()->GetGroupCode( aPropCode );
        propertyDesc->SetUint32L( CMTPTypeObjectPropDesc::EGroupCode, groupCode );
        }

    PRINT( _L( "MM MTP <= CGetFormatCapabilities::ServicePropDescL" ) );

    return propertyDesc;
    }

// end of file
