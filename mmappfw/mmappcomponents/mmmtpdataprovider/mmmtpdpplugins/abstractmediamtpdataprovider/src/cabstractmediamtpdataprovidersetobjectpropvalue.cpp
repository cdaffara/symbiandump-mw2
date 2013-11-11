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
* Description:  Implement opeartion setobjectpropvalue
*
*/


#include <mtp/cmtptypearray.h>

#include "cabstractmediamtpdataprovidersetobjectpropvalue.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderSetObjectPropValue::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CAbstractMediaMtpDataProviderSetObjectPropValue* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderSetObjectPropValue( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderSetObjectPropValue::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::CAbstractMediaMtpDataProviderSetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderSetObjectPropValue::CAbstractMediaMtpDataProviderSetObjectPropValue( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CSetObjectPropValue( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::~CAbstractMediaMtpDataProviderSetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderSetObjectPropValue::~CAbstractMediaMtpDataProviderSetObjectPropValue()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::IsSpecificPropCodeReadOnly
// no need to do anything here, just leave, as this should never be invoked or
// invalid propcode
// -----------------------------------------------------------------------------
//
TBool CAbstractMediaMtpDataProviderSetObjectPropValue::IsSpecificPropCodeReadOnly( TUint16 aPropCode ) const
    {
    TBool ret = EFalse;
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeRepresentativeSampleFormat:
        case EMTPObjectPropCodeRepresentativeSampleSize:
        case EMTPObjectPropCodeRepresentativeSampleHeight:
        case EMTPObjectPropCodeRepresentativeSampleWidth:
            ret = ETrue;
            break;
        default:
            ret = EFalse;
            break;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::ReceiveDataSpecificObjectPropertyL
// no need to do anything here, just leave, as this should never be invoked or
// invalid propcode
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderSetObjectPropValue::ReceiveDataSpecificObjectPropertyL( TUint16 aPropCode )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderSetObjectPropValue::ReceiveDataSpecificObjectPropertyL" ) );
    if ( aPropCode == EMTPObjectPropCodeRepresentativeSampleData )
        {
        delete iMTPTypeArray;
        iMTPTypeArray = NULL;

        iMTPTypeArray = CMTPTypeArray::NewL( EMTPTypeAUINT8 );
        ReceiveDataL( *iMTPTypeArray );
        }
    else
        {
        PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderSetObjectPropValue::ReceiveDataSpecificObjectPropertyL leave with KErrNotSupported" ) );
        // Do nothing now.
        // May need add implementation here for further extension.
        User::Leave( KErrNotSupported );
        }
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL
// no need to do anything here, just leave, as this should never be invoked or
// invalid propcode
// -----------------------------------------------------------------------------
//
TMTPResponseCode CAbstractMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL( TUint16 aPropCode )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL" ) );
    TMTPResponseCode responseCode = EMTPRespCodeOK;

    if ( aPropCode == EMTPObjectPropCodeRepresentativeSampleData )
        {
        responseCode = ServiceMetaDataToWrapperL( aPropCode,
            *iMTPTypeArray,
            *iObjectInfo );
        }
    else
        {
        PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL leave with KErrNotSupported" ) );
        // Do nothing now.
        // May need add implementation here for further extension.
        User::Leave( KErrNotSupported );
        }
    PRINT( _L( "MM MTP <= CAbstractMediaMtpDataProviderSetObjectPropValue::ServiceSpecificObjectPropertyL" ) );
    return responseCode;
    }

// end of file
