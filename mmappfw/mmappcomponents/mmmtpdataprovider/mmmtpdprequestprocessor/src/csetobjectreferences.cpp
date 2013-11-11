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
* Description:  Implement the operation: SetObjectReferences
*
*/


#include <mtp/cmtptypearray.h>
#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpreferencemgr.h>

#include "csetobjectreferences.h"
#include "tmmmtpdppanic.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdputility.h"
#include "mmmtpdpconfig.h"

const TInt KMmMtpRefArrayGranularity = 3;

// -----------------------------------------------------------------------------
// Verification data for the SetReferences request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPSetObjectReferencesPolicy[] =
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
// CSetObjectReferences::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CSetObjectReferences::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CSetObjectReferences* self = new ( ELeave ) CSetObjectReferences( aFramework,
        aConnection,
        aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CSetObjectReferences::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::~CSetObjectReferences
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CSetObjectReferences::~CSetObjectReferences()
    {
    delete iReferences;
    delete iReferenceSuids;
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::CSetObjectReferences
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CSetObjectReferences::CSetObjectReferences( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof( KMTPSetObjectReferencesPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPSetObjectReferencesPolicy ),
        iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: SetObjectReferences(0x9811)" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::DoSetObjectReferencesL
// set references to DB
// -----------------------------------------------------------------------------
//
void CSetObjectReferences::DoSetObjectReferencesL( const CMTPObjectMetaData& aObject )
    {
    TUint formatCode = aObject.Uint( CMTPObjectMetaData::EFormatCode );
    TBool hasReference = MmMtpDpUtility::HasReference( formatCode );

    if ( hasReference )
        {
        iDpConfig.GetWrapperL().SetReferenceL( aObject, *iReferenceSuids );
        }
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::ServiceL
// SetReferences request handler
// start receiving reference data from the initiator
// -----------------------------------------------------------------------------
//
EXPORT_C void CSetObjectReferences::ServiceL()
    {
    PRINT( _L( "MM MTP => CSetObjectReferences::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
    
    delete iReferences;
    iReferences = NULL;
    iReferences = CMTPTypeArray::NewL( EMTPTypeAUINT32 );
    ReceiveDataL( *iReferences );
    PRINT( _L( "MM MTP <= CSetObjectReferences::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::DoHandleResponsePhaseL
// Apply the references to the specified object
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSetObjectReferences::DoHandleResponsePhaseL()
    {
    PRINT( _L("MM MTP => CSetObjectReferences::DoHandleResponsePhaseL" ) );

    delete iReferenceSuids;
    iReferenceSuids = NULL;
    iReferenceSuids = new ( ELeave ) CDesCArrayFlat( KMmMtpRefArrayGranularity );

    if ( !VerifyReferenceHandlesL() )
        {
        SendResponseL( EMTPRespCodeInvalidObjectReference );
        }
    else
        {
        MMTPReferenceMgr& referenceMgr = iFramework.ReferenceMgr();
        TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
        PRINT1( _L( "MM MTP <> CSetObjectReferences::DoHandleResponsePhaseL objectHandle = 0x%x" ), objectHandle );
        referenceMgr.SetReferencesL( TMTPTypeUint32( objectHandle ),
            *iReferences );

        CMTPObjectMetaData* object = CMTPObjectMetaData::NewLC(); // + object
        iFramework.ObjectMgr().ObjectL( objectHandle, *object );
        PRINT1( _L( "MM MTP <> object file name is %S" ), &(object->DesC( CMTPObjectMetaData::ESuid ) ) );
        DoSetObjectReferencesL( *object );
        
        // set it to something else other than EMTPSubFormatCodeUnknown to prevent being queried from MPX later in the same session
        object->SetUint( CMTPObjectMetaData::EFormatSubCode, EMTPSubFormatCodeUndefined );
        iFramework.ObjectMgr().ModifyObjectL( *object );

        CleanupStack::PopAndDestroy( object ); // - object

        SendResponseL( EMTPRespCodeOK );
        }
    PRINT( _L("MM MTP <= CSetObjectReferences::DoHandleResponsePhaseL" ) );
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::VerifyReferenceHandlesL
// Verify if the references are valid handles to objects
// -----------------------------------------------------------------------------
//
TBool CSetObjectReferences::VerifyReferenceHandlesL() const
    {
    PRINT( _L( "MM MTP => CSetObjectReferences::VerifyReferenceHandlesL" ) );
    __ASSERT_DEBUG( iReferences, Panic( EMmMTPDpReferencesNull ) );
    TBool result = ETrue;
    TInt count = iReferences->NumElements();
    PRINT1( _L( "MM MTP <> CSetObjectReferences::VerifyReferenceHandlesL count = %d" ), count );
    CMTPObjectMetaData* object( CMTPObjectMetaData::NewLC() ); // + object
    MMTPObjectMgr& objectMgr = iFramework.ObjectMgr();

    for ( TInt i = 0; i < count; i++ )
        {
        TMTPTypeUint32 handle;
        iReferences->ElementL( i, handle );
        if ( !objectMgr.ObjectL( handle, *object ) )
            {
            result = EFalse;
            break;
            }
        // Only audio in playlist is supported
        if ( iDpConfig.GetWrapperL().Category( *object ) == EMPXSong )
            iReferenceSuids->AppendL( object->DesC( CMTPObjectMetaData::ESuid ) );
        }
    CleanupStack::PopAndDestroy( object ); // - object
    PRINT( _L( "MM MTP <= CSetObjectReferences::VerifyReferenceHandlesL" ) );
    return result;
    }

// -----------------------------------------------------------------------------
// CSetObjectReferences::HasDataphase
// Derived from CRequestProcessor, can not be neglected
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CSetObjectReferences::HasDataphase() const
    {
    return ETrue;
    }

// end of file
