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
* Description:  Implement the operation: getobjectproplist
 *
*/


#include <mtp/cmtptypeobjectproplist.h>
#include <mtp/cmtptypearray.h>
#include <f32file.h>

#include "cabstractmediamtpdataprovidergetobjectproplist.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropList::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderGetObjectPropList::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CAbstractMediaMtpDataProviderGetObjectPropList* self =
        new (ELeave) CAbstractMediaMtpDataProviderGetObjectPropList( aFramework,
            aConnection,
            aDpConfig );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropList::~CAbstractMediaMtpDataProviderGetObjectPropList
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderGetObjectPropList::~CAbstractMediaMtpDataProviderGetObjectPropList()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropList::CAbstractMediaMtpDataProviderGetObjectPropList
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderGetObjectPropList::CAbstractMediaMtpDataProviderGetObjectPropList( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CGetObjectPropList( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropList::ConstructL
// 2nd Phase Constructor
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderGetObjectPropList::ConstructL()
    {
    CGetObjectPropList::ConstructL();
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL
// do nothing here, just leave, as this should never be invoked or invalid propcode
// -----------------------------------------------------------------------------
//
TInt CAbstractMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL( TUint16 aPropCode,
    TUint32 aHandle )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL" ) );
    TInt err = KErrNone;
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeRepresentativeSampleFormat:
            {
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint16L( CMTPTypeObjectPropListElement::EValue, 0 );
            }
            break;
        case EMTPObjectPropCodeRepresentativeSampleSize:
        case EMTPObjectPropCodeRepresentativeSampleHeight:
        case EMTPObjectPropCodeRepresentativeSampleWidth:
            {
            iPropertyElement = &( iPropertyList->ReservePropElemL( aHandle, aPropCode ) );
            iPropertyElement->SetUint32L( CMTPTypeObjectPropListElement::EValue, 0 );
            }
            break;
        case EMTPObjectPropCodeRepresentativeSampleData:
            {
            const TDesC& suid( iObject->DesC( CMTPObjectMetaData::ESuid ) );
            CMTPTypeArray* desData = CMTPTypeArray::NewLC( EMTPTypeAUINT8 );    // + desData

            RFile sampleFile;
            TInt err = sampleFile.Open( iFramework.Fs(), suid, EFileShareReadersOnly );
            PRINT2( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData open filename=%S, err=%d" ), &suid, err );

            CleanupClosePushL( sampleFile );    // + sampleFile
            TInt size = 0;
            if( err == KErrNone )
                {
                err = sampleFile.Size( size );
                }
            PRINT2( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData sampleFile.Size size=%d, err=%d" ), size, err );
            if( err == KErrNone && size > 0 )
                {
                HBufC8* sampleData = HBufC8::NewLC( size );    // + sampleData
                TPtr8 samplePtr = sampleData->Des();
                err = sampleFile.Read( samplePtr );
                PRINT1( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData sampleFile.Read err=%d" ), err );
                PRINT1( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData samplePtr.Size=%d" ), samplePtr.Size() );
                if( err == KErrNone )
                    {
                    desData->SetByDesL( samplePtr );
                    }
                CleanupStack::PopAndDestroy( sampleData );    // - sampleData
                }
            iPropertyElement = &(iPropertyList->ReservePropElemL(aHandle, aPropCode));
            iPropertyElement->SetArrayL( CMTPTypeObjectPropListElement::EValue, *desData );

            CleanupStack::PopAndDestroy( &sampleFile );    // - sampleFile
            CleanupStack::PopAndDestroy( desData );        // - desData
            }
            break;

        default:
            err = KErrNotSupported;
        }

    PRINT1( _L( "MM MTP <= CAbstractMediaMtpDataProviderGetObjectPropList::ServiceSpecificObjectPropertyL, err = %d" ), err );
    return err;
    }

// end of file
