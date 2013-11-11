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
* Description:  Implement the operation: GetObjectReferences
*
*/


#include <mtp/cmtptypearray.h>
#include <mtp/mmtpreferencemgr.h>
#include <mtp/mmtpobjectmgr.h>
#ifdef _DEBUG
#include <mtp/tmtptypeuint128.h>
#endif

#include "cgetobjectreferences.h"
#include "mmmtpdputility.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpperflog.h"
#include "mmmtpdpconfig.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "tmmmtpdppanic.h"

const TInt KMTPDriveGranularity = 5;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
_LIT( KMpxQueryAbstractMediaReference, "MpxQueryAbstractMediaReference" );
_LIT( KReferenceManagerSetReference, "ReferenceManagerSetReference" );
_LIT( KObjectManagerHandle, "ObjectManagerHandle" );
#endif

// -----------------------------------------------------------------------------
// Verification data for the GetReferences request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPGetObjectReferencesPolicy[] =
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
// CGetObjectReferences::NewL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CGetObjectReferences::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CGetObjectReferences* self = new ( ELeave ) CGetObjectReferences( aFramework,
        aConnection,
        aDpConfig );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CGetObjectReferences::ConstructL
// Two-phase construction method
// -----------------------------------------------------------------------------
//
void CGetObjectReferences::ConstructL()
    {
#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    iPerfLog = CMmMtpDpPerfLog::NewL( _L( "CAbstractMediaMtpDataProviderEnumerator" ) );
#endif
    }

// -----------------------------------------------------------------------------
// CGetObjectReferences::~CGetObjectReferences
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CGetObjectReferences::~CGetObjectReferences()
    {
    delete iReferences;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    delete iPerfLog;
#endif // _DEBUG
    }

// -----------------------------------------------------------------------------
// CGetObjectReferences::CGetObjectReferences
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CGetObjectReferences::CGetObjectReferences( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
        CRequestProcessor( aFramework,
            aConnection,
            sizeof ( KMTPGetObjectReferencesPolicy ) / sizeof( TMTPRequestElementInfo ),
            KMTPGetObjectReferencesPolicy ),
        iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: GetObjectReferences(0x9810)" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectReferences::ServiceL
// GetObjectInfo request handler
// -----------------------------------------------------------------------------
//
void CGetObjectReferences::ServiceL()
    {
    PRINT( _L( "MM MTP => CGetObjectReferences::ServiceL" ) );
    
    MmMtpDpUtility::SetPSStatus( EMtpPSStatusActive );

    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );
    PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL objectHandle = 0x%x" ),
        objectHandle );

    CMTPObjectMetaData* parentObject = iRequestChecker->GetObjectInfo( objectHandle );
    __ASSERT_DEBUG( parentObject, Panic( EMmMTPDpObjectNull ) );

    iFramework.ObjectMgr().ObjectL( objectHandle, *parentObject );
    TUint subFormatCode = parentObject->Uint( CMTPObjectMetaData::EFormatSubCode );
    PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL subFormatCode = 0x%x" ),
        subFormatCode );
    
    if ( MmMtpDpUtility::HasReference( parentObject->Uint( CMTPObjectMetaData::EFormatCode ) )
        && ( subFormatCode == EMTPSubFormatCodeUnknown ) )
        {
        CDesCArray* references = new ( ELeave ) CDesCArrayFlat( KMTPDriveGranularity );
        CleanupStack::PushL( references ); // + references

        PERFLOGSTART( KMpxQueryAbstractMediaReference );
        TRAP_IGNORE( iDpConfig.GetWrapperL().GetAllReferenceL( *parentObject, *references ) );
        PERFLOGSTOP( KMpxQueryAbstractMediaReference );

        // insert references into reference db
        TPtrC parentSuid( parentObject->DesC( CMTPObjectMetaData::ESuid ) );
        AddReferencesL( parentSuid, *references );
        
        CleanupStack::PopAndDestroy( references );  // - references

        parentObject->SetUint( CMTPObjectMetaData::EFormatSubCode, EMTPSubFormatCodeUndefined );    // set it to something else
        iFramework.ObjectMgr().ModifyObjectL( *parentObject );
        }


    delete iReferences;
    iReferences = NULL;
    MMTPReferenceMgr& referenceMgr = iFramework.ReferenceMgr();
    iReferences = referenceMgr.ReferencesLC( TMTPTypeUint32( objectHandle ) );
    CleanupStack::Pop( iReferences );
    SendDataL( *iReferences );

#if _DEBUG
    TUint32 handle = 0;
    TUint count = iReferences->NumElements();
    PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL count = %d" ), count );
    CMTPObjectMetaData* object = NULL;
    for( TUint i = 0; i < count; i++ )
        {
        object = CMTPObjectMetaData::NewLC(); // + object
        handle = iReferences->ElementUint( i );
        PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL handle = 0x%x" ), handle );
        iFramework.ObjectMgr().ObjectL( handle, *object);
        PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL object file name is %S" ), &object->DesC( CMTPObjectMetaData::ESuid ) );
        CleanupStack::PopAndDestroy( object ); // - object
        TMTPTypeUint128 puoid;
        puoid.Set( 0, 0 );
        puoid = iFramework.ObjectMgr().PuidL( handle );
        PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL higher value of puoid = 0x%04Lx" ), puoid.UpperValue() );
        PRINT1( _L( "MM MTP <> CGetObjectReferences::ServiceL lower value of puoid = 0x%04Lx" ), puoid.LowerValue() );
        }
#endif

    PRINT( _L( "MM MTP <= CGetObjectReferences::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// CGetObjectReferences::AddReferencesL
// GetObjectInfo request handler
// -----------------------------------------------------------------------------
//
void CGetObjectReferences::AddReferencesL( const TDesC& aRefOwnerSuid,
    CDesCArray& aReferences )
    {
    TInt count = aReferences.Count();
    PRINT2( _L("MM MTP => CGetObjectReferences::AddReferencesL aRefOwnerSuid = %S, ref count = %d"), &aRefOwnerSuid, count );

    // check if references are valid
    for ( TInt i = count-1; i >= 0; i-- )
        {
        TPtrC temp( aReferences[i] );
        PRINT2( _L( "MM MTP <> CGetObjectReferences::AddReferencesL ref[%d]'s name = %S" ), i, &temp );
        PERFLOGSTART( KObjectManagerHandle );
        TUint32 handle = iFramework.ObjectMgr().HandleL( temp );
        PERFLOGSTOP( KObjectManagerHandle );
        if ( handle == KMTPHandleNone ) // object doesn't exist
            {
            PRINT1( _L( "MM MTP <> CGetObjectReferences::AddReferencesL, [%S] doesn't existed in handle db, remove this from reference array" ), &temp );

            // if handle is invalid, remove from reference array
            aReferences.Delete( i );
            }
        }

    // add all references into references db
    MMTPReferenceMgr& referenceMgr = iFramework.ReferenceMgr();
    PERFLOGSTART( KReferenceManagerSetReference );
    referenceMgr.SetReferencesL( aRefOwnerSuid, aReferences );
    PERFLOGSTOP( KReferenceManagerSetReference );

    PRINT( _L( "MM MTP <= CGetObjectReferences::AddReferencesL" ) );
    }

// end of file
