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
* Description:  Implement the operation: DeleteObject
*
*/


#include <mtp/mmtpobjectmgr.h>
#include <mtp/mmtpreferencemgr.h>

#include "cdeleteobject.h"
#include "mmmtpdplogger.h"
#include "mmmtpdpconfig.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "mmmtpdputility.h"

const TInt KMaxDeletionTimes = 10;
const TInt KMaxDeviceBusyTimes = 3;
const TInt KDeletionThreshold = 100 * 1000; // (100 millisec)

// -----------------------------------------------------------------------------
// Verification data for the DeleteObject request
// -----------------------------------------------------------------------------
//
const TMTPRequestElementInfo KMTPDeleteObjectPolicy[] =
    {
        {
        TMTPTypeRequest::ERequestParameter1,
        EMTPElementTypeObjectHandle,
        EMTPElementAttrWrite,
        1,
        KMTPHandleAll,
        0
        },
    };

// -----------------------------------------------------------------------------
// CDeleteObject::NewL
// Two-phase construction method
// CDeleteObject is an abstract class and shouldn't be instaniated.
// -----------------------------------------------------------------------------
//
EXPORT_C MMmRequestProcessor* CDeleteObject::NewL( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig )
    {
    CDeleteObject* self = new (ELeave) CDeleteObject( aFramework, aConnection, aDpConfig );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CDeleteObject::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::~CDeleteObject
// Destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CDeleteObject::~CDeleteObject()
    {
    Cancel();
    iObjectsToDelete.Close();
    }

// -----------------------------------------------------------------------------
// CDeleteObject::CDeleteObject
// Standard c++ constructor
// -----------------------------------------------------------------------------
//
CDeleteObject::CDeleteObject( MMTPDataProviderFramework& aFramework,
    MMTPConnection& aConnection,
    MMmMtpDpConfig& aDpConfig ) :
    CRequestProcessor( aFramework,
        aConnection,
        sizeof( KMTPDeleteObjectPolicy ) / sizeof( TMTPRequestElementInfo ),
        KMTPDeleteObjectPolicy ),
    iObjectsToDelete( KMmMtpRArrayGranularity ),
    iDeleteError( KErrNone ),
    iDpConfig( aDpConfig )
    {
    PRINT( _L( "Operation: DeleteObject(0x100B)" ) );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::ServiceL
// DeleteObject request handler
// -----------------------------------------------------------------------------
//
EXPORT_C void CDeleteObject::ServiceL()
    {
    MmMtpDpUtility::SetPSStatus( EMtpPSStatusActive );
    
    iObjectsToDelete.Reset();
    iDeleteError = KErrNone;
    TUint32 objectHandle = Request().Uint32( TMTPTypeRequest::ERequestParameter1 );

    PRINT3( _L( "MM MTP <> CDeleteObject::ServiceL, objectHandle = 0x%x, numObject = %d, iDeleteError = %d" ),
        objectHandle,
        iObjectsToDelete.Count(),
        iDeleteError );

    // Check to see whether the request is to delete all files or a specific file/directory
    if ( objectHandle == KMTPHandleAll )
        {
        iIsMultiDelete = ETrue;
        // Get the complete list of all the media files that are to be deleted
        GetObjectHandlesL( KMTPStorageAll, KMTPHandleNoParent );
        StartL();
        }
    else
        {
        iIsMultiDelete = EFalse;
        // Not Owned the object
        CMTPObjectMetaData* objectInfo = iRequestChecker->GetObjectInfo( objectHandle );

        if ( objectInfo->Uint( CMTPObjectMetaData::EFormatCode ) == EMTPFormatCodeAssociation )
            {
            TUint parentHandle = objectInfo->Uint( CMTPObjectMetaData::EHandle );
            GetObjectHandlesL( KMTPStorageAll, parentHandle );
            if ( iObjectsToDelete.Count() > 0 )
                iIsMultiDelete = ETrue;
            StartL();
            }
        else
            {
            DeleteObjectL( *objectInfo );
            ProcessFinalPhaseL();
            }
        }

    PRINT( _L( "MM MTP <= CDeleteObject::ServiceL" ) );
    }

// -----------------------------------------------------------------------------
// MTPDeleteObject::StartL
// Control the deletion
// -----------------------------------------------------------------------------
//
void CDeleteObject::StartL()
    {
    TInt numObjectsToDelete = iObjectsToDelete.Count();
    PRINT2( _L( "MM MTP <> CDeleteObject::StartL, numObjectsToDelete = %d, iDeleteError = %d" ),
        numObjectsToDelete,
        iDeleteError );

    TBool isOk = iDeleteError == KErrNone || iDeleteError == KErrNotFound;
    if ( numObjectsToDelete > 0  &&  isOk )
        {
        //Set the active object going to delete the file
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        SetActive();
        }
    else
        {
        ProcessFinalPhaseL();
        }
    PRINT( _L( "MM MTP <= CDeleteObject::StartL" ) );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::RunL
// AO Run method, deletes a selection of files on the system
// -----------------------------------------------------------------------------
//
EXPORT_C void CDeleteObject::RunL()
    {
    TInt numObjectsToDelete = iObjectsToDelete.Count();
    PRINT1( _L( "MM MTP => CDeleteObject::RunL, numObjectsToDelete = %d" ),
        numObjectsToDelete );

    if ( numObjectsToDelete > 0 )
        {
        // Get the next object
        CMTPObjectMetaData* objectInfo = CMTPObjectMetaData::NewLC(); // + objectInfo

        TUint32 handle = iObjectsToDelete[0];
        iFramework.ObjectMgr().ObjectL( handle, *objectInfo );
        PRINT2( _L( "MM MTP <> CDeleteObject::RunL delete object handle is 0x%x, fileName is %S" ),
            handle,
            &( objectInfo->DesC( CMTPObjectMetaData::ESuid ) ) );

        if ( objectInfo->Uint( CMTPObjectMetaData::EFormatCode ) == EMTPFormatCodeAssociation )
            {
            TUint parentHandle = objectInfo->Uint( CMTPObjectMetaData::EHandle );
            GetObjectHandlesL( KMTPStorageAll, parentHandle );
            }
        else
            {
            DeleteObjectL( *objectInfo );
            }

        CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo

        iObjectsToDelete.Remove( 0 );
        }

    // Start the process again to read the next row...
    StartL();
    PRINT( _L( "MM MTP <= CDeleteObject::RunL" ) );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::DeleteObjectL()
// Deletes the object from the file system and the object from the store
// -----------------------------------------------------------------------------
//
void CDeleteObject::DeleteObjectL( const CMTPObjectMetaData& aObjectInfo )
    {
    TPtrC fileName( aObjectInfo.DesC( CMTPObjectMetaData::ESuid ) );
    PRINT1( _L( "MM MTP <> CDeleteObject::DeleteObjectL fileName = %S" ), &fileName );

    // To capture special situation: After copy, move, rename playlist folder name,
    // record in MPX is not inlined with framework db, playlist should not be deleted
    // until next session.
    // This is used to keep the same behavior in mass storage and device file manager.
    if ( aObjectInfo.Uint( CMTPObjectMetaData::EFormatCode )
        == EMTPFormatCodeAbstractAudioVideoPlaylist
        && !iDpConfig.GetWrapperL().IsExistL( fileName ) )
        {
        iDeleteError = KErrGeneral;
        PRINT( _L( "MM MTP <= CDeleteObject::DeleteObjectL playlist file not exist in the MPX DB" ) );
        return;
        }

    // 1. Delete object from file system
    TEntry fileInfo;
    iFramework.Fs().Entry( fileName, fileInfo );
    if ( fileInfo.IsReadOnly() )
        {
        iDeleteError = KErrAccessDenied;
        PRINT1( _L( "MM MTP <= CDeleteObject::DeleteObjectL, \"%S\" is a read-only file"), &fileName );
        return;
        }
    // Some other component might be holding on to the file (MDS background harvesting),
    // try again after 100 millisec, up to 10 times, before give up
    TInt count = KMaxDeletionTimes;
    while ( count > 0 )
        {
        iDeleteError = iFramework.Fs().Delete( fileName );
        if ( iDeleteError == KErrNone || iDeleteError == KErrNotFound )
            {
            break;
            }
        else if ( ( iDeleteError == KErrInUse ) && ( count > 1 ) )
            {
            User::After( KDeletionThreshold );
            count--;
            }
        else
            {
            PRINT1( _L( "MM MTP <= CDeleteObject::DeleteObjectL, Delete from file system failed, err = %d" ), iDeleteError );
            return;
            }
        }

    // 2. Delete object from metadata db
    TRAP( iDeleteError, iDpConfig.GetWrapperL().DeleteObjectL( aObjectInfo ) );
    PRINT1( _L( "MM MTP <> CDeleteObject::DeleteObjectL, Delete from Media DB, err = %d" ), iDeleteError );

    // 3. Delete object from framework db
    iFramework.ObjectMgr().RemoveObjectL( aObjectInfo.Uint( CMTPObjectMetaData::EHandle ) );

    // 4. If the object has references, Delete references from reference manager
    if ( MmMtpDpUtility::HasReference( aObjectInfo.Uint( CMTPObjectMetaData::EFormatCode ) ) )
        iFramework.ReferenceMgr().RemoveReferencesL( aObjectInfo.DesC( CMTPObjectMetaData::ESuid ) );

    PRINT( _L( "MM MTP <= CDeleteObject::DeleteObjectL" ) );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::DoCancel
// Used to cancel the deletion of the files
// -----------------------------------------------------------------------------
//
EXPORT_C void CDeleteObject::DoCancel()
    {
    TRAP_IGNORE( ProcessFinalPhaseL() );
    }

// -----------------------------------------------------------------------------
// CDeleteObject::DoHandleCompletingPhaseL
// Completeing phase Handler
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CDeleteObject::DoHandleCompletingPhaseL()
    {
    CRequestProcessor::DoHandleCompletingPhaseL();

    // If the response code is EMTPRespCodeDeviceBusy, don't release this object
    TBool result = !iDeviceBusy;

    PRINT2( _L( "MM MTP <> CDeleteObject::DoHandleCompletingPhaseL iDeleteError= %d, result = %d" ),
            iDeleteError,
            result );

    return result;
    }

// -----------------------------------------------------------------------------
// CDeleteObject::ProcessFinalPhaseL
// Delete all of the associations if file deletion was successful
// Then signals that the deletion has been completed
// -----------------------------------------------------------------------------
//
void CDeleteObject::ProcessFinalPhaseL()
    {
    PRINT( _L( "MM MTP => CDeleteObject::ProcessFinalPhaseL" ) );

    TInt num = iObjectsToDelete.Count();
    TBool isOk = iDeleteError == KErrNone || iDeleteError == KErrNotFound;
    iDeviceBusy = EFalse;

    if ( num == 0 && isOk )
        {
        SendResponseL( EMTPRespCodeOK );
        }
    else if ( iIsMultiDelete && !isOk )
        {
        SendResponseL( EMTPRespCodePartialDeletion );
        }
    else if ( !iIsMultiDelete && iDeleteError == KErrAccessDenied )
        { 
        SendResponseL( EMTPRespCodeObjectWriteProtected );
        }
    else if ( iDeleteError == KErrInUse 
            && iCountDeviceBusyError < KMaxDeviceBusyTimes )
        {
        iCountDeviceBusyError++;
        SendResponseL( EMTPRespCodeDeviceBusy );
        iDeviceBusy = ETrue;
        }
    else
        {
        SendResponseL( EMTPRespCodeAccessDenied );
        }
    }

// -----------------------------------------------------------------------------
// CDeleteObject::GetObjectHandlesL
// Gets all object handles (for GetObjectHandlesL)
// -----------------------------------------------------------------------------
//
void CDeleteObject::GetObjectHandlesL( TUint32 aStorageId,
    TUint32 aParentHandle )
    {
    PRINT2( _L( "MM MTP => CDeleteObject::GetObjectHandlesL, StorageId = 0x%x, ParentHandle = 0x%x" ),
        aStorageId, aParentHandle );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( context ); // + context
    CleanupClosePushL( handles ); // + handles

    TMTPObjectMgrQueryParams params( aStorageId, KMTPFormatsAll, aParentHandle );

    do
        {
        iFramework.ObjectMgr().GetObjectHandlesL( params, context, handles );

        for ( TInt i = 0; i < handles.Count(); i++ )
            {
            if ( iFramework.ObjectMgr().ObjectOwnerId( handles[i] )
                == iFramework.DataProviderId() )
                {
                iObjectsToDelete.Append( handles[i] );
                }
            // TODO: should not know dp id of device dp,
            // else if ( iFramework.ObjectMgr().ObjectOwnerId( handles[i] ) == 0 ) // We know that the device dp id is always 0, otherwise the whole MTP won't work.
            //     iParentHandles.AppendL( handles[i] );
            else
                {
                CMTPObjectMetaData* objectInfo = CMTPObjectMetaData::NewLC(); // + objectInfo
                iFramework.ObjectMgr().ObjectL( handles[i], *objectInfo );
                if ( EMTPFormatCodeAssociation == objectInfo->Uint( CMTPObjectMetaData::EFormatCode ) )
                    {
                    GetObjectHandlesL( KMTPStorageAll, handles[i] );
                    }
                CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo
                }
            }
        }
    while ( !context.QueryComplete() );

    CleanupStack::PopAndDestroy( &handles ); // - handles
    CleanupStack::PopAndDestroy( &context ); // - context

    PRINT( _L( "MM MTP <= CDeleteObject::GetObjectHandlesL" ) );
    }

// end of file
