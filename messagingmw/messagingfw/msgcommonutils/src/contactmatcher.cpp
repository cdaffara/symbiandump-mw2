/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   CContactMatcher class implementation
*
*/



// System includes
#include <e32base.h>
#include <bamdesca.h>
#include <cntitem.h>
#include <eikenv.h>
#include <bautils.h>
#include <data_caging_path_literals.hrh>

#include <contactmatcher.h>

#include <CVPbkContactManager.h>
#include <MVPbkContactStoreList.h>
#include <MVPbkContactStore.h>
#include <CVPbkContactStoreUriArray.h>
#include <MVPbkContactLinkArray.h>
#include <MVPbkContactLink.h>
#include <MVPbkFieldType.h>
#include <MVPbkStoreContact.h>
#include <MVPbkStoreContactField.h>
#include <MVPbkStoreContactFieldCollection.h>
#include <MVPbkContactFieldData.h>
#include <MVPbkContactFieldTextData.h>
#include <MVPbkContactFieldDateTimeData.h>
#include <MVPbkContactFieldBinaryData.h>
#include <MVPbkContactOperationBase.h>
#include <MVPbkContactStoreProperties.h>
#include <TVPbkContactStoreUriPtr.h>
#include <VPbkContactStoreUris.h>
#include <MPbk2ContactNameFormatter.h>
#include <CVPbkFieldTypeSelector.h>
#include <TVPbkFieldVersitProperty.h>
#include <CVPbkFieldTypeRefsList.h>

#include <coemain.h>
#include <CPbk2SortOrderManager.h>
#include <Pbk2ContactNameFormatterFactory.h>

#include <msgcommonutils.rsg>


// ================= Static Constant Data ===================

typedef const TDesC& (*UriFuncPtr)();

// Number match store URIs in priority order.
// When doing number matching, order of the stores in the uri array will
// determine which stores are searched first (sequential match). We stop
// the search when first match is found.
static const UriFuncPtr NumberMatchStoreUris[] =
    {
    VPbkContactStoreUris::DefaultCntDbUri,
    // If we don't manage to open some store, we remove it from our array
    VPbkContactStoreUris::SimGlobalAdnUri,
    VPbkContactStoreUris::SimGlobalSdnUri,
    NULL,   // end marker
    };

// All store URIs except own number store
static const UriFuncPtr AllStoreUris[] =
    {
    VPbkContactStoreUris::DefaultCntDbUri,
    // If we don't manage to open some store, we remove it from our array
    VPbkContactStoreUris::SimGlobalAdnUri,
    VPbkContactStoreUris::SimGlobalSdnUri,
    VPbkContactStoreUris::SimGlobalFdnUri,
    NULL,   // end marker
    };

// Own number store URIs
static const UriFuncPtr OwnNumberStoreUris[] =
    {
    VPbkContactStoreUris::SimGlobalOwnNumberUri,
    NULL,   // end marker
    };

_LIT(KMsgCommonUtilsResourceFileName, "msgcommonutils.rsc");

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// Two-phase constructor for CContactMatcher class.
// ----------------------------------------------------------------------------
EXPORT_C CContactMatcher* CContactMatcher::NewL(
    RFs* aFsSession )
    {
    CContactMatcher* self = CContactMatcher::NewLC( aFsSession );
    CleanupStack::Pop(self);
    return self;
    }

// ----------------------------------------------------------------------------
// Two-phase constructor for CContactMatcher class.
// ----------------------------------------------------------------------------
EXPORT_C CContactMatcher* CContactMatcher::NewLC(
    RFs* aFsSession )
    {
    CContactMatcher* self = new ( ELeave ) CContactMatcher( aFsSession );
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ----------------------------------------------------------------------------
// C++ destructor.
// ----------------------------------------------------------------------------
EXPORT_C CContactMatcher::~CContactMatcher()
    {
    FreeOldOperation();
    CleanupNumberMatch();
    delete iStoreUris;
    delete iContactManager;
    delete iSortOrderManager;
    delete iNameFormatter;
    iResourceFile.Close();

    if ( iClientStatus )
        {
        User::RequestComplete( iClientStatus, KErrCancel );
        }
    if ( iASchedulerWait.IsStarted() )
        {
        iASchedulerWait.AsyncStop();
        }
    }

// ----------------------------------------------------------------------------
// C++ Constructor.
// ----------------------------------------------------------------------------
CContactMatcher::CContactMatcher( RFs* aFsSession) : iFsSession( aFsSession )
    {
    }

// ----------------------------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------------------------
void CContactMatcher::ConstructL()
    {
    iContactManager = CVPbkContactManager::NewL(
        *CVPbkContactStoreUriArray::NewLC(), iFsSession );
    CleanupStack::PopAndDestroy(); // CVPbkContactStoreUriArray

    // No stores open yet
    iStoreUris = CVPbkContactStoreUriArray::NewL();
    }

//******************* API-methods *********************************************

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenStoreL(
    const CVPbkContactStoreUriArray& aUriArray )
    {
    InitOperationL( EOpenStore );
    OpenStoreCommonL( aUriArray );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        // Wait until stores are open
        iASchedulerWait.Start();
        }
    User::LeaveIfError( iError );
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenStoreL(
    const CVPbkContactStoreUriArray& aUriArray, TRequestStatus&  aStatus )
    {
    InitOperationL( EOpenStore );
    OpenStoreCommonL( aUriArray );
    InitOperation( &aStatus );

    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        }
    }


// ----------------------------------------------------------------------------
// Common code to sync/async versions.
// ----------------------------------------------------------------------------
void CContactMatcher::OpenStoreCommonL(
    const CVPbkContactStoreUriArray& aUriArray )
    {
    if (iStoreUris->Count())
        {
        // Opening more stores when some stores are already open is not
        // supported. Support would require managing iStoreUris properly
        // so that it contains all open stores.
        User::Leave(KErrGeneral);
        }

    const TInt count = aUriArray.Count();
    
    for (TInt i = 0; i < count; ++i)
        {
        // Appended Uri:s to the array. If store fails to open it is removed
        // from the array. This keeps Uri's in priority order in the array.
        TVPbkContactStoreUriPtr uriPtr = aUriArray[i];
        iStoreUris->AppendL( uriPtr );

        iContactManager->LoadContactStoreL( uriPtr );
        }
    MVPbkContactStoreList& storeList = iContactManager->ContactStoresL();
    storeList.OpenAllL( *this );
    }

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenAllStoresL()
    {
    OpenStoreL(AllStoreUris);
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenAllStoresL( TRequestStatus& aStatus )
    {
    OpenStoreL(AllStoreUris, aStatus);
    }

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenDefaultMatchStoresL()
    {
    OpenStoreL(NumberMatchStoreUris);
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenDefaultMatchStoresL( TRequestStatus& aStatus )
    {
    OpenStoreL(NumberMatchStoreUris, aStatus);
    }

// ----------------------------------------------------------------------------
// Open OwnNumber stores.
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenOwnNumberStoresL()
    {
    OpenStoreL(OwnNumberStoreUris);
    }

// ----------------------------------------------------------------------------
// Open OwnNumber stores.
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::OpenOwnNumberStoresL( TRequestStatus& aStatus )
    {
    OpenStoreL(OwnNumberStoreUris, aStatus);
    }

// ----------------------------------------------------------------------------
// Close all open stores.
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::CloseStoresL()
    {
    // Closing stores does not work. MatchDataL() finds contacts from
    // closed stores.

    InitOperationL( ECloseStores );

    iApiMethodStatus = EExecuting;
    TRAPD( err, iContactManager->ContactStoresL().CloseAll( *this ) );
    iApiMethodStatus = EFinished;
    if ( err == KErrNone)
        {
        delete iStoreUris; iStoreUris = NULL;
        iStoreUris = CVPbkContactStoreUriArray::NewL();        
        }
    else
        {
        User::Leave(err);
        }
    }

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::MatchPhoneNumberL(
    const TDesC& aData, TInt aDigits,
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags,
    CVPbkContactLinkArray& aLinkArray )
    {
    InitOperationL( EMatchPhoneNumber );
    iResultContactLinkArray = &aLinkArray;

    // Start asynchronous matching and wait until results are ready
    MatchPhoneNumberCommonL( aData, aDigits, aFlags );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        iASchedulerWait.Start();
        }
    User::LeaveIfError( iError );
    }


// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::MatchPhoneNumberL(
    const TDesC& aData, TInt aDigits,
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags,
    CVPbkContactLinkArray& aLinkArray, TRequestStatus& aStatus )
    {
    InitOperationL( EMatchPhoneNumber );
    iResultContactLinkArray = &aLinkArray;
    // Start asynchronous matching
    MatchPhoneNumberCommonL( aData, aDigits, aFlags );
    InitOperation( &aStatus );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        }
    }

// ----------------------------------------------------------------------------
// Common code for sync and async versions
// ----------------------------------------------------------------------------
void CContactMatcher::MatchPhoneNumberCommonL(
    const TDesC& aData, TInt aDigits,
    CVPbkPhoneNumberMatchStrategy::TVPbkPhoneNumberMatchFlags aFlags )
    {
    // Delete resources allocated for previous match
    CleanupNumberMatch();

    // iStoreUris is filled when stores are opened

    iStratConfig = new (ELeave) CVPbkPhoneNumberMatchStrategy::TConfig(
        aDigits,
        *iStoreUris,
        CVPbkPhoneNumberMatchStrategy::EVPbkSequentialMatch,
        aFlags);
    iMatchStrategy = CVPbkPhoneNumberMatchStrategy::NewL(
                *iStratConfig,
                *iContactManager,
                *this);
    // Start asynchronous matching
    iMatchStrategy->MatchL( aData );
    }


// ----------------------------------------------------------------------------
// Find from a store succeeded
// ----------------------------------------------------------------------------
void CContactMatcher::FindFromStoreSucceededL( MVPbkContactStore& /*aStore*/,
        MVPbkContactLinkArray* aResultsFromStore )
    {
    __ASSERT_DEBUG( aResultsFromStore, ContactMatcherPanics::Panic(
        ContactMatcherPanics::EPanNullPointer ));

    // Take the ownership of the result immediately
    CleanupDeletePushL( aResultsFromStore );

    CopyFindResultsL( aResultsFromStore );

    CleanupStack::PopAndDestroy(); // aResultsFromStore
    }

// ----------------------------------------------------------------------------
// Copy the found results for a store into array
// ----------------------------------------------------------------------------
void CContactMatcher::CopyFindResultsL( MVPbkContactLinkArray*
    aResults )
    {
    const TInt count = aResults->Count();
    if ( iResultContactLinkArray )
        {
        // Copy links to the member array
        for ( TInt i = 0; i < count; ++i )
            {
            iResultContactLinkArray->AppendL( aResults->At( i ).CloneLC() );
            CleanupStack::Pop(); // cloned link
            }
        }
    else
        {
        iResultContactLinkCnt += count;
        }
    }


// ----------------------------------------------------------------------------
// Find failed
// ----------------------------------------------------------------------------
void CContactMatcher::FindFromStoreFailed( MVPbkContactStore& /*aStore*/, TInt /*aError*/ )
    {
    //no operation, search to continue from the other stores
    }


// ----------------------------------------------------------------------------
// Find complete
// ----------------------------------------------------------------------------
void CContactMatcher::FindFromStoresOperationComplete()
    {
    if (!iResultContactLinkArray)
        {
        // Links were not copied. Result is whether any links found or not.
        OperationComplete( iResultContactLinkCnt ? KErrNone:KErrNotFound );
        }
    else
        {
        OperationComplete();
        iResultContactLinkArray = NULL;
        }
    }

// ----------------------------------------------------------------------------
// Return global list of field types.
// ----------------------------------------------------------------------------
EXPORT_C const MVPbkFieldTypeList& CContactMatcher::FieldTypes() const
    {
    return iContactManager->FieldTypes();
    }

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::GetStoreContactL(
    const MVPbkContactLink& aLink, MVPbkStoreContact** aStoreContact )
    {
    InitOperationL( EGetStoreContact );
    iResultStoreContact = aStoreContact;

    // Start asynchronous operation and wait until results are ready
    FreeOldOperation();
    iOperation = iContactManager->RetrieveContactL( aLink, *this );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        iASchedulerWait.Start();
        }
    User::LeaveIfError( iError );
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::GetStoreContactL(
    const MVPbkContactLink& aLink, MVPbkStoreContact** aStoreContact,
    TRequestStatus& aStatus )
    {
    InitOperationL( EGetStoreContact );
    iResultStoreContact = aStoreContact;
    // Start asynchronous operation
    FreeOldOperation();
    iOperation = iContactManager->RetrieveContactL( aLink, *this );
    InitOperation( &aStatus );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        }
    }

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::IsOwnNumberL( const TDesC& aNumber, TBool& aResult )
    {
    InitOperationL( EMatchPhoneNumber );

     // Not interested in links, only whether found or not
    iResultContactLinkArray = NULL;
    iResultContactLinkCnt = 0;

    // Start asynchronous matching and wait until results are ready
    MatchPhoneNumberCommonL( aNumber, aNumber.Length(),
        CVPbkPhoneNumberMatchStrategy::EVPbkStopOnFirstMatchFlag );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        iASchedulerWait.Start();
        }
    User::LeaveIfError( iError );

    aResult = iResultContactLinkCnt > 0;
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::IsOwnNumberL( const TDesC& aNumber,
    TRequestStatus& aStatus )
    {
    InitOperationL( EMatchPhoneNumber );

     // Not interested in links, only whether found or not
    iResultContactLinkArray = NULL;
    iResultContactLinkCnt = 0;

    // Start asynchronous matching
    MatchPhoneNumberCommonL( aNumber, aNumber.Length(),
        CVPbkPhoneNumberMatchStrategy::EVPbkStopOnFirstMatchFlag );
    InitOperation( &aStatus );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        }
    }

// ----------------------------------------------------------------------------
// Cancel asynchronous operation
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::CancelOperation()
    {
    if (iApiMethodStatus != EExecuting)
        {
        return;
        }

    __ASSERT_DEBUG(!iSync, ContactMatcherPanics::Panic(
        ContactMatcherPanics::EPanInvalidOp));

    switch(iApiMethod)
        {
        case EMatchData:
        case EGetStoreContact:
            FreeOldOperation(); // deleting the operation cancels it
            break;
        case EMatchPhoneNumber:
            CleanupNumberMatch();
            break;
        default:
            ;
        }

    User::RequestComplete( iClientStatus, KErrCancel );

    iApiMethod = ENoMethod;
    iApiMethodStatus = EFinished;
    }

// ----------------------------------------------------------------------------
// GetFieldData, for EVPbkFieldStorageTypeText
// ----------------------------------------------------------------------------
EXPORT_C TPtrC CContactMatcher::GetFieldDataTextL(
    const MVPbkStoreContact& aContact,
    const MVPbkFieldType& aFType ) const
    {
    TPtrC ret(KNullDesC);
    const MVPbkStoreContactField* field = FindField( aContact, aFType);
    if (field)
        {
        const MVPbkContactFieldData& fdata = field->FieldData();
        if (fdata.DataType() == EVPbkFieldStorageTypeText)
            {
            const MVPbkContactFieldTextData& fdata2 =
                MVPbkContactFieldTextData::Cast(fdata);
            ret.Set( fdata2.Text() );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// GetFieldData, for EVPbkFieldStorageTypeDateTime
// ----------------------------------------------------------------------------
EXPORT_C TTime CContactMatcher::GetFieldDataDateTimeL(
    const MVPbkStoreContact& aContact,
    const MVPbkFieldType& aFType ) const
    {
    //               YYYYMMDD:HHMMSS.MMMMMM
    _LIT(KNullTime, "11110000:010101.00000");
    TTime ret(KNullTime);
    const MVPbkStoreContactField* field = FindField( aContact, aFType);
    if (field)
        {
        const MVPbkContactFieldData& fdata = field->FieldData();
        if (fdata.DataType() == EVPbkFieldStorageTypeDateTime)
            {
            const MVPbkContactFieldDateTimeData& fdata2 =
                MVPbkContactFieldDateTimeData::Cast( fdata );
            ret = fdata2.DateTime();
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }
    return ret;
    }

// ----------------------------------------------------------------------------
// GetFieldData, for EVPbkFieldStorageTypeBinary
// ----------------------------------------------------------------------------
EXPORT_C TPtrC8 CContactMatcher::GetFieldDataBinaryL(
    const MVPbkStoreContact& aContact,
    const MVPbkFieldType& aFType ) const
    {
    TPtrC8 ret(KNullDesC8);
    const MVPbkStoreContactField* field = FindField( aContact, aFType);
    if (field)
        {
        const MVPbkContactFieldData& fdata = field->FieldData();
        if (fdata.DataType() == EVPbkFieldStorageTypeBinary)
            {
            const MVPbkContactFieldBinaryData& fdata2 =
                MVPbkContactFieldBinaryData::Cast( fdata );
            ret.Set( fdata2.BinaryData() );
            }
        else
            {
            User::Leave( KErrArgument );
            }
        }
    return ret;
    }


//******************************** Private Methods ***************************

// ----------------------------------------------------------------------------
// Finds a field of given type from contact.
// Returns pointer to field or NULL if not found.
// ----------------------------------------------------------------------------
 const MVPbkStoreContactField* CContactMatcher::FindField(
    const MVPbkStoreContact& aContact,
    const MVPbkFieldType& aFType ) const
    {
    const MVPbkStoreContactFieldCollection& coll = aContact.Fields();
    TInt n = coll.FieldCount();

    const MVPbkStoreContactField* field = NULL;
    TBool bFound = EFalse;
    for(TInt i=0; i < n && !bFound; ++i)
        {
        field = &coll.FieldAt( i );
        const MVPbkFieldType* ftype = field->MatchFieldType( 0 );
        if ( ftype )
            {
            if ( ftype->IsSame( aFType ))
                {
                bFound = ETrue;
                }
            }
        }
    if ( !bFound )
        {
        field = NULL;
        }
    return field;
    }

// ----------------------------------------------------------------------------
// Get URI array with stores
// ----------------------------------------------------------------------------
CVPbkContactStoreUriArray* CContactMatcher::GetStoreArrayLC(
    const TDesC& (* const aFuncPtrs[])() )
    {
    CVPbkContactStoreUriArray* uriArray = CVPbkContactStoreUriArray::NewLC();

    // Add stores
    for(TInt i = 0; aFuncPtrs[i]; i++)
        {
        TVPbkContactStoreUriPtr uriPtr(aFuncPtrs[i]());
        uriArray->AppendL(uriPtr);
        }
    return uriArray;
    }

// ----------------------------------------------------------------------------
// Open stores. Synchronous version
// ----------------------------------------------------------------------------
void CContactMatcher::OpenStoreL(const TDesC& (* const aFuncPtrs[])())
    {
    CVPbkContactStoreUriArray* uriArray = GetStoreArrayLC(aFuncPtrs);

    CContactMatcher::OpenStoreL(*uriArray);
    CleanupStack::PopAndDestroy(uriArray);
    }

// ----------------------------------------------------------------------------
// Open stores. Asynchronous version
// ----------------------------------------------------------------------------
void CContactMatcher::OpenStoreL(const TDesC& (* const aFuncPtrs[])(),
    TRequestStatus&  aStatus)
    {
    CVPbkContactStoreUriArray* uriArray = GetStoreArrayLC(aFuncPtrs);

    CContactMatcher::OpenStoreL(*uriArray, aStatus);
    CleanupStack::PopAndDestroy(uriArray);
    }

// ----------------------------------------------------------------------------
// Called when the opening process is complete,
// ie. all stores have been reported either failed or successfully opened.
// ----------------------------------------------------------------------------
//
void CContactMatcher::OpenComplete()
    {
    TInt error = KErrNone;
    if ( iStoreUris->Count() == 0 )
        {
        // unable to open any of the specified stores
        error = KErrNotSupported;
        }
    OperationComplete( error );
    }

// ----------------------------------------------------------------------------
// Called when a contact store is ready to use.
// ----------------------------------------------------------------------------
void CContactMatcher::StoreReady( MVPbkContactStore& /*aContactStore*/ )
    {
    }

// ----------------------------------------------------------------------------
// Called when a contact store becomes unavailable.
// Client may inspect the reason of the unavailability and decide whether or not
// it will keep the store opened (ie. listen to the store events).
// @param aContactStore The store that became unavailable.
// @param aReason The reason why the store is unavailable.
//                This is one of the system wide error codes.
// ----------------------------------------------------------------------------
void CContactMatcher::StoreUnavailable( MVPbkContactStore& aContactStore,
    TInt /*aReason*/ )
    {
    // Remove contact store from uri list
    iStoreUris->Remove( aContactStore.StoreProperties().Uri() );
    }

// ----------------------------------------------------------------------------
// Called when changes occur in the contact store.
// @see TVPbkContactStoreEvent
//
// @param aStoreEvent Event that has occured.
// ----------------------------------------------------------------------------
void CContactMatcher::HandleStoreEventL(
        MVPbkContactStore& /*aContactStore*/,
        TVPbkContactStoreEvent aStoreEvent)
    {
    // Contact and group events can be ignored, but we pass backup events for the observer.
    switch ( aStoreEvent.iEventType )
        {
        case TVPbkContactStoreEvent::EStoreBackupBeginning:
        case TVPbkContactStoreEvent::EStoreRestoreBeginning:
            {
            iBackup = ETrue;
            break;
            }
        case TVPbkContactStoreEvent::EStoreBackupRestoreCompleted:
            {
            iBackup = EFalse;
            break;
            }
        default:
            break;
        }
    }


// ----------------------------------------------------------------------------
// Called when find is complete. Callee takes ownership of the results.
// In case of an error during find, the aResults may contain only
// partial results of the find.
//
// @param aResults Array of contact links that matched the find.
// ----------------------------------------------------------------------------
void CContactMatcher::FindCompleteL( MVPbkContactLinkArray* aResults )
    {
    __ASSERT_DEBUG( aResults, ContactMatcherPanics::Panic(
        ContactMatcherPanics::EPanNullPointer ));

    // Take the ownership of the result immediately
    CleanupDeletePushL( aResults );

    CopyFindResultsL( aResults );

    CleanupStack::PopAndDestroy(); // aResults

    if (!iResultContactLinkArray)
        {
        // No need to copy links. Only interested whether found or not
        OperationComplete( iResultContactLinkCnt ? KErrNone:KErrNotFound );
        }
    else
        {
        OperationComplete();
        iResultContactLinkArray = NULL;
        }
    }

// ----------------------------------------------------------------------------
// Called in case the find fails for some reason.
//
// @param aError One of the system wide error codes.
// ----------------------------------------------------------------------------
void CContactMatcher::FindFailed( TInt aError )
    {
    OperationFailed( aError );
    iResultContactLinkArray = NULL;
    }

// ----------------------------------------------------------------------------
// Free old VPbk-operation.
// ----------------------------------------------------------------------------
void CContactMatcher::FreeOldOperation()
    {
    delete iOperation;
    iOperation = NULL;
    }

// ----------------------------------------------------------------------------
// Called when operation is completed.
// ----------------------------------------------------------------------------
void CContactMatcher::VPbkSingleContactOperationComplete(
        MVPbkContactOperationBase& /*aOperation*/, MVPbkStoreContact* aContact)
    {
    *iResultStoreContact = aContact;
    iResultStoreContact  = NULL;
    OperationComplete();
    }

// ----------------------------------------------------------------------------
// Called if the operation fails.
// ----------------------------------------------------------------------------
void CContactMatcher::VPbkSingleContactOperationFailed(
    MVPbkContactOperationBase& /*aOperation*/, TInt aError)
    {
    OperationFailed( aError );
    }

// ----------------------------------------------------------------------------
// Set member variables for sync operation
// ----------------------------------------------------------------------------
void CContactMatcher::InitOperationL( TMethodId aMethod )
    {
    if ( iBackup )
        {
        User::Leave( KErrAccessDenied );
        }

    // Check whether operation is in progress
    if ( iApiMethodStatus == EExecuting )
        {
        User::Leave( KErrInUse );
        }

    iSync  = ETrue;
    iError = KErrNone;
    iApiMethod = aMethod;
    iApiMethodStatus = EIdle;
    }

// ----------------------------------------------------------------------------
// Set member variables for async operation
// ----------------------------------------------------------------------------
void CContactMatcher::InitOperationL( TMethodId aMethod, TRequestStatus* aStatus )
    {
    InitOperationL( aMethod );

    iSync  = EFalse;
    iClientStatus  = aStatus;
    *iClientStatus = KRequestPending;
    }
    
// ----------------------------------------------------------------------------
// Set member variables for async operation
// ----------------------------------------------------------------------------
void CContactMatcher::InitOperation( TRequestStatus* aStatus )
    {
    iSync  = EFalse;
    iClientStatus  = aStatus;
    *iClientStatus = KRequestPending;
    }

// ----------------------------------------------------------------------------
// Sync/async operation finished succesfully, return results to method caller.
// ----------------------------------------------------------------------------
void CContactMatcher::OperationComplete( TInt aErrorCode )
    {
    if (iSync)
        {
        if ( iASchedulerWait.IsStarted() )
            {
            iASchedulerWait.AsyncStop();
            }
        }
    else
        {
        if ( iClientStatus )
            {
            User::RequestComplete( iClientStatus, aErrorCode );
            iClientStatus = NULL;
            }
        }
    iApiMethodStatus = EFinished;
    }

// ----------------------------------------------------------------------------
// Sync/async operation failed, return results to method caller.
// ----------------------------------------------------------------------------
void CContactMatcher::OperationFailed( TInt aError )
    {
    iError = aError;
    OperationComplete( aError );
    }

// ----------------------------------------------------------------------------
// Free resources allocated for number matching
// ----------------------------------------------------------------------------
void CContactMatcher::CleanupNumberMatch()
{
    delete iMatchStrategy;
    iMatchStrategy = NULL;

    delete iStratConfig;
    iStratConfig = NULL;

    // store uris are not deleted here - opened array remains valid
    // until new set of stores is opened.
}

// ---------------------------------------------------------------------------
// CContactMatcher::GetContactStoresL
// ---------------------------------------------------------------------------
EXPORT_C MVPbkContactStoreList& CContactMatcher::GetContactStoresL()
    {
    return iContactManager->ContactStoresL();
    }


// -----------------------------------------------------------------------------
// TInt CContactMatcher::GetName
//
// Returns the formatted name fo the contact
// -----------------------------------------------------------------------------
EXPORT_C HBufC* CContactMatcher::GetNameL( MVPbkStoreContactFieldCollection&
                                                                aFieldCollection )
    {
    MPbk2ContactNameFormatter& nameFormatter = ContactNameFormatterL();
    
    HBufC* formattedName = nameFormatter.GetContactTitleOrNullL( aFieldCollection, 
    						                                     MPbk2ContactNameFormatter::EUseSeparator );
    return formattedName;
    }

// -----------------------------------------------------------------------------
// TInt CContactMatcher::ContactHasFieldOfTypeL( )
// -----------------------------------------------------------------------------
EXPORT_C TInt CContactMatcher::ContactHasFieldOfTypeL
        ( TAiwAddressSelectType aAddressSelectType,  const MVPbkStoreContact& aContact )
  {
    TInt resId = 0;

    switch ( aAddressSelectType )
        {
      case EAiwPhoneNumberSelect:
          resId = R_PHONE_NUMBER_SELECTOR;
          break;
      case EAiwEMailSelect:
          resId = R_EMAIL_ADDRESS_SELECTOR;
          break;
      case EAiwMMSSelect:
          resId = R_MMS_ADDRESS_SELECTOR;
          break;
      default:
          resId = R_PHONE_NUMBER_SELECTOR;
          break;
        }

    MVPbkContactFieldSelector* fieldTypeSelector =
          CreateFieldTypeSelectorLC( resId );

    // Check if the specified field type is included in the contact
    const MVPbkStoreContactFieldCollection& fields = aContact.Fields();
  TInt fieldCount = fields.FieldCount();

  TInt ret = KErrNotFound;
  for ( TInt i = 0; i < fieldCount && ret == KErrNotFound; ++i )
    {
    const MVPbkBaseContactField& field = fields.FieldAt( i );
    if ( fieldTypeSelector->IsFieldIncluded( field ) )
      {
      ret = i;
      }
    }

  CleanupStack::PopAndDestroy(); // fieldTypeSelector

  return ret;
  }

// -----------------------------------------------------------------------------
// CVPbkContactManager& CContactMatcher::GetContactManager( )
// -----------------------------------------------------------------------------
EXPORT_C CVPbkContactManager& CContactMatcher::GetContactManager()
    {
    return *iContactManager;
    }
    

// ----------------------------------------------------------------------------
// Synchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::MatchDataL( const TDesC& aData,
    const MVPbkFieldTypeList& aFieldTypes,
    CVPbkContactLinkArray& aLinkArray)
    {
    InitOperationL( EMatchData );
    iResultContactLinkArray = &aLinkArray;

    // Start asynchronous matching and wait until results are ready
    FreeOldOperation();
    iOperation = iContactManager->FindL(aData, aFieldTypes, *this);
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        iASchedulerWait.Start();
        }

    User::LeaveIfError( iError );
    RemoveSimilarEmailAddressesL( aData, aLinkArray, aFieldTypes );
   	}

// ----------------------------------------------------------------------------
// Remove contacts that do not have exactly the correct email address
// e.g. if cbd@test.com address is requested, the for example a contact with address abcd@test.com will be removed
// from the result.
// This filtering is done only in the syncronous version of MatchDataL
// ----------------------------------------------------------------------------
void CContactMatcher::RemoveSimilarEmailAddressesL( const TDesC& aData, CVPbkContactLinkArray& aLinkArray, const MVPbkFieldTypeList& aFieldTypes )
    {
    TVPbkFieldVersitProperty prop;
    prop.SetName( EVPbkVersitNameEMAIL );
    // do extra checks for email addresses
    
    const MVPbkFieldType* foundType = NULL;
    // Continue only if at least one type is EVPbkVersitNameEMAIL
    TInt i;
    for ( i = 0 ; i < aFieldTypes.FieldTypeCount() ; i++ )
        {
        foundType = &(aFieldTypes.FieldTypeAt( i ));
        if ( foundType->VersitProperties().Count() > 0
            && foundType->VersitProperties()[0].Name() == prop.Name() )
            {
            break;
            }
        }
    if ( i == aFieldTypes.FieldTypeCount() )
    	{
    	// no email types
    	return;
    	}
    
    const MVPbkFieldTypeList& fieldTypeList = FieldTypes();

    TInt index = 0;
	TBool isExactMatch;
    while( index < aLinkArray.Count() )
    	{
	    MVPbkStoreContact* storeContact;
	    GetStoreContactL( aLinkArray.At( index ), &storeContact );
	    storeContact->PushL();
	    
	    isExactMatch = EFalse;
        for ( TInt i = 0; i < fieldTypeList.FieldTypeCount(); i++ )
            {
            // find the email property
            foundType = &(fieldTypeList.FieldTypeAt( i ));
            if ( foundType->VersitProperties().Count() > 0
                && foundType->VersitProperties()[0].Name() == prop.Name() )
                {
                TPtrC src = GetFieldDataTextL(*storeContact, *foundType );
                if ( aData.CompareF( src ) == 0 )
        	    	{
        	    	isExactMatch = ETrue;
        	    	}
                }
            }
	    if ( isExactMatch )
            {
            // go for the next contact
            index++;
            }
        else
        	{
            // remove the contact, because the email address does not match the one queried. 
            // the next one will take plce of this contact in the list (=do not increase index)
            aLinkArray.Delete( index ); 
        	}
	    CleanupStack::PopAndDestroy( storeContact );
    	}
    }

// ----------------------------------------------------------------------------
// Asynchronous version
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::MatchDataL( const TDesC& aData,
    const MVPbkFieldTypeList& aFieldTypes,
    CVPbkContactLinkArray& aLinkArray,
    TRequestStatus& aStatus)
    {
    InitOperationL( EMatchData );
    iResultContactLinkArray = &aLinkArray;

    // Start asynchronous matching
    FreeOldOperation();
    iOperation = iContactManager->FindL(aData, aFieldTypes, *this);
    InitOperation( &aStatus );
    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        }
    }
// ----------------------------------------------------------------------------
// MatchData for searchstrings
// ----------------------------------------------------------------------------
EXPORT_C void CContactMatcher::MatchDataL( const MDesC16Array& aSearchStrings,
    const MVPbkFieldTypeList& aFieldTypes,
    CVPbkContactLinkArray& aLinkArray,
    const TCallBack& aWordParserCallBack )
    {
    InitOperationL( EMatchData );
    iResultContactLinkArray = &aLinkArray;

    // Start asynchronous matching and wait here until results are ready
    FreeOldOperation();
    iOperation = iContactManager->FindL( aSearchStrings, aFieldTypes,
        *this, aWordParserCallBack );

    if ( iApiMethodStatus != EFinished )
        {
        iApiMethodStatus = EExecuting;
        iASchedulerWait.Start();
        }
    User::LeaveIfError( iError );
    }

// ----------------------------------------------------------------------------
// CContactMatcher::ContactNameFormatterL
// ----------------------------------------------------------------------------
EXPORT_C MPbk2ContactNameFormatter& CContactMatcher::ContactNameFormatterL()
    {
    //first initialise, if not already initialised
    if ( !iSortOrderManager )
        {
        iSortOrderManager = CPbk2SortOrderManager::NewL( FieldTypes() );
        }
        
    if ( !iNameFormatter )
        {
        iNameFormatter = Pbk2ContactNameFormatterFactory::CreateL( FieldTypes(),
                                                                  *iSortOrderManager );
        }
    return *iNameFormatter;
    }


// -----------------------------------------------------------------------------
// MVPbkContactFieldSelector* CContactMatcher::CreateFieldTypeSelectorLC( )
// -----------------------------------------------------------------------------
MVPbkContactFieldSelector* CContactMatcher::CreateFieldTypeSelectorLC
  ( TInt aResId )
  {
    if ( !iResourceFileInitialized )
        {
        TFileName tmpName;
        // Append the Resource Files Directory
        tmpName.Append( KDC_RESOURCE_FILES_DIR );     
        // Append the Ressource File Name
        tmpName.Append( KMsgCommonUtilsResourceFileName );
  
        // Obtain the drive where the DLL is installed
        TFileName dllDrive;
        Dll::FileName( dllDrive );
  
        // Obtain the Complete path for the Resource File
        TParse parse;
        parse.Set( dllDrive, NULL, NULL );
        parse.Set( parse.Drive(), &tmpName, NULL );
        TFileName fileName;
        fileName.Append( parse.FullName());

        iResourceFile.OpenL( *iFsSession, fileName );
        iResourceFile.ConfirmSignatureL( 0 );
        iResourceFileInitialized = ETrue;
        }

    HBufC8* dataBuffer = iResourceFile.AllocReadLC( aResId );

    TResourceReader reader;
    reader.SetBuffer( dataBuffer );

    CVPbkFieldTypeSelector* fieldTypeSelector =
        CVPbkFieldTypeSelector::NewL( reader,
                    FieldTypes() );

    CleanupStack::PopAndDestroy( dataBuffer );

    CleanupStack::PushL( fieldTypeSelector );
    return fieldTypeSelector;
    }

// ---------------------------------------------------------------------------
// ContactMatcherPanics::Panic
//
// Panic function
// ---------------------------------------------------------------------------
void ContactMatcherPanics::Panic( TPanic aPanic )
    {
    _LIT(KPanicCategory, "ContactMatcher");
    User::Panic( KPanicCategory, aPanic );
    }

// End of File
