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
* Description:  Implement the operation: getobjectpropvalue
*
*/


#include <mtp/cmtptypearray.h>
#include <f32file.h>

#include "cabstractmediamtpdataprovidergetobjectpropvalue.h"
#include "abstractmediamtpdataproviderconst.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpdefs.h"
#include "mmmtpdpconfig.h"

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropValue::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
MMmRequestProcessor* CAbstractMediaMtpDataProviderGetObjectPropValue::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CAbstractMediaMtpDataProviderGetObjectPropValue* self =
        new ( ELeave ) CAbstractMediaMtpDataProviderGetObjectPropValue( aFramework,
            aConnection,
            aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropValue::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderGetObjectPropValue::ConstructL()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropValue::~CAbstractMediaMtpDataProviderGetObjectPropValue
// Destructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderGetObjectPropValue::~CAbstractMediaMtpDataProviderGetObjectPropValue()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropValue::CAbstractMediaMtpDataProviderGetObjectPropValue
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CAbstractMediaMtpDataProviderGetObjectPropValue::CAbstractMediaMtpDataProviderGetObjectPropValue( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CGetObjectPropValue( aFramework, aConnection, aDpConfig )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CAbstractMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectProperty
// no need to do anything here, just leave, as this should never be
// invoked or invalid propcode
// -----------------------------------------------------------------------------
//
void CAbstractMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL( TUint16 aPropCode )
    {
    PRINT( _L( "MM MTP => CAbstractMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL" ) );
    switch ( aPropCode )
        {
        case EMTPObjectPropCodeRepresentativeSampleFormat:
            {
            iMTPTypeUint16.Set( 0 );
            SendDataL( iMTPTypeUint16 );
            }
            break;

        case EMTPObjectPropCodeRepresentativeSampleSize: // fall through
        case EMTPObjectPropCodeRepresentativeSampleHeight: // fall through
        case EMTPObjectPropCodeRepresentativeSampleWidth:
            {
            iMTPTypeUint32.Set( 0 );
            SendDataL( iMTPTypeUint32 );
            }
            break;

        case EMTPObjectPropCodeRepresentativeSampleData:
            {
            if ( iMTPTypeArray != NULL )
                {
                delete iMTPTypeArray;
                iMTPTypeArray = NULL;
                }
            iMTPTypeArray = CMTPTypeArray::NewL( EMTPTypeAUINT8 );

            const TDesC& suid = iObjectInfo->DesC( CMTPObjectMetaData::ESuid );
            PRINT1( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData file suid=%S" ), &suid );
            RFile sampleFile;
            User::LeaveIfError( sampleFile.Open( iFramework.Fs(), suid, EFileShareReadersOnly ) );
            CleanupClosePushL( sampleFile );    // + sampleFile

            TInt size = 0;
            User::LeaveIfError( sampleFile.Size( size ) );
            PRINT1( _L( "MM MTP <> EMTPObjectPropCodeRepresentativeSampleData sampleFile.Size() size=%d" ), size );

            if( size > 0 )
                {
                HBufC8* sampleData = HBufC8::NewLC( size );    // + sampleData
                TPtr8 samplePtr = sampleData->Des();
                User::LeaveIfError( sampleFile.Read( samplePtr ) );
                iMTPTypeArray->SetByDesL( samplePtr );
                CleanupStack::PopAndDestroy( sampleData );    // - sampleData
                }
            CleanupStack::PopAndDestroy( &sampleFile );    // - sampleFile

            SendDataL( *iMTPTypeArray );
            }
            break;

        default:
            PRINT( _L( "MM MTP <> CAbstractMediaMtpDataProviderGetObjectPropValue::ServiceSpecificObjectPropertyL, leave with KErrNotSupported"));
            User::Leave( KErrNotSupported );
        }
    }

// end of file
