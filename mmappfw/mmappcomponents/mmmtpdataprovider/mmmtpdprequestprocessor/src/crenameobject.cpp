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
* Description:  Rename Object
*
*/


#include <f32file.h>

#include <mtp/mmtpobjectmgr.h>
#include <mtp/tmtptypeuint32.h>

#include "crenameobject.h"
#include "mmmtpdplogger.h"
#include "cmmmtpdpmetadataaccesswrapper.h"
#include "crequestprocessor.h" // refer to KMmMtpRArrayGranularity
#include "mmmtpdputility.h"

// -----------------------------------------------------------------------------
// CRenameObject::NewL
// Two phase constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRenameObject* CRenameObject::NewL( MMTPDataProviderFramework& aFramework,
    CMmMtpDpMetadataAccessWrapper& aWrapper )
    {
    PRINT( _L( "MM MTP => CRenameObject::NewL" ) );

    CRenameObject* self = new ( ELeave ) CRenameObject( aFramework, aWrapper );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    PRINT( _L( "MM MTP <= CRenameObject::NewL" ) );
    return self;
    }

// -----------------------------------------------------------------------------
// CRenameObject::CRenameObject
// Standard C++ Constructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRenameObject::CRenameObject( MMTPDataProviderFramework& aFramework,
         CMmMtpDpMetadataAccessWrapper& aWrapper ) :
    CActive( EPriorityStandard ),
    iFramework( aFramework ),
    iObjectHandles( KMmMtpRArrayGranularity ),
    iWrapper ( aWrapper )
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRenameObject::~CRenameObject
// destructor
// -----------------------------------------------------------------------------
//
EXPORT_C CRenameObject::~CRenameObject()
    {
    Cancel();
    iObjectHandles.Close();
    delete iObjectInfo;
    iOldFileName.Close();
    iRightPartName.Close();
    iFileName.Close();
    iOldFolderFullName.Close();
    iNewFolderName.Close();

    delete iRenameWaiter;
    }

// -----------------------------------------------------------------------------
// CRenameObject::StartL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRenameObject::StartL( const TUint32 aParentHandle,
        const TDesC& aOldFolderName )
    {
    PRINT2( _L( "MM MTP => CRenameObject::StartL aParentHandle(0x%x), aOldFolderName(%S)" ),
        aParentHandle, &aOldFolderName);

    iObjectHandles.Reset();

    GenerateObjectHandleListL( aParentHandle );

    iCount = iObjectHandles.Count();
    PRINT1( _L( "MM MTP <> CRenameObject::StartL, handle count = %d" ), iCount );
    if ( iCount > 0 )
        {
        MmMtpDpUtility::SetPSStatus(EMtpPSStatusActive);
        
        // get the old/new folder full file name
        GetParentSuidL( aParentHandle, aOldFolderName );

        iIndex = 0;

        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();

        iRenameWaiter->Start();
        }

    PRINT( _L( "MM MTP <= CRenameObject::StartL" ) );
    }

// -----------------------------------------------------------------------------
// CRenameObject::DoCancel()
// Cancel the rename object process
// -----------------------------------------------------------------------------
//
EXPORT_C void CRenameObject::DoCancel()
    {
    // Do nothing
    }

// -----------------------------------------------------------------------------
// CRenameObject::RunL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRenameObject::RunL()
    {
    PRINT1( _L( "MM MTP => CRenameObject::RunL iIndex = %d" ), iIndex );
    if ( iIndex < iCount )
        {
        if ( iFramework.ObjectMgr().ObjectL( iObjectHandles[iIndex], *iObjectInfo ) )
            {
            iOldFileName.Zero();
            iOldFileName = iObjectInfo->DesC( CMTPObjectMetaData::ESuid );
            PRINT1( _L( "MM MTP <> CRenameObject::RunL old file name(%S)" ), &iOldFileName );

            iRightPartName.Zero();
            iRightPartName = iOldFileName.Right( iOldFileName.Length() - iOldFolderFullName.Length() );
            PRINT1( _L( "MM MTP <> CRenameObject::RunL right part name(%S)" ), &iRightPartName );

            if ( iNewFolderName.Length() + iRightPartName.Length() <= KMaxFileName )
                {
                iFileName.Zero();
                iFileName.Append( iNewFolderName );
                iFileName.Append( iRightPartName );
                PRINT1( _L( "MM MTP <> CRenameObject::RunL new file name(%S)" ), &iFileName );

                PerformAdditionalActionL();

                // update framework metadata DB
                iObjectInfo->SetDesCL( CMTPObjectMetaData::ESuid, iFileName );
                iObjectInfo->SetUint( CMTPObjectMetaData::EObjectMetaDataUpdate, 1 );
                iFramework.ObjectMgr().ModifyObjectL( *iObjectInfo );
                }
            }

        iIndex++;

        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, iStatus.Int() );
        SetActive();
        }
    else
        {
        if( iRenameWaiter->IsStarted() )
            iRenameWaiter->AsyncStop();
        
        MmMtpDpUtility::SetPSStatus(EMtpPSStatusReadyToSync);  
        }

    PRINT( _L( "MM MTP <= CRenameObject::RunL" ) );
    }

// -----------------------------------------------------------------------------
// CRenameObject::RunError
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CRenameObject::RunError( TInt aError )
    {
    if ( aError != KErrNone )
        PRINT1( _L( "MM MTP <> CRenameObject::RunError with error %d" ), aError );

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CRenameObject::ConstructL
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CRenameObject::ConstructL()
    {
    CActiveScheduler::Add( this );

    iObjectInfo = CMTPObjectMetaData::NewL();

    iNewFolderName.CreateL( KMaxFileName );
    iOldFolderFullName.CreateL( KMaxFileName );
    iRightPartName.CreateL( KMaxFileName );
    iFileName.CreateL( KMaxFileName );
    iOldFileName.CreateL( KMaxFileName );

    iRenameWaiter = new( ELeave ) CActiveSchedulerWait;
    }

// -----------------------------------------------------------------------------
// CRenameObject::GenerateObjectHandleListL
//
// -----------------------------------------------------------------------------
//
void CRenameObject::GenerateObjectHandleListL( TUint32 aParentHandle )
    {
    PRINT1( _L( "MM MTP => CRenameObject::GenerateObjectHandleListL aParentHandle(0x%x)" ), aParentHandle );
    RMTPObjectMgrQueryContext context;
    RArray<TUint> handles;
    CleanupClosePushL( context ); // + context
    CleanupClosePushL( handles ); // + handles

    TMTPObjectMgrQueryParams params( KMTPStorageAll, KMTPFormatsAll, aParentHandle );
    do
        {
        iFramework.ObjectMgr().GetObjectHandlesL( params, context, handles );

        TInt numberOfObjects = handles.Count();
        for ( TInt i = 0; i < numberOfObjects; i++ )
            {
            if ( iFramework.ObjectMgr().ObjectOwnerId( handles[i] ) == iFramework.DataProviderId() )
                {
                iObjectHandles.AppendL( handles[i] );
                // NOTE: Fw changed the mechanism of notification, no need to iterate

                }
            }
        }
    while ( !context.QueryComplete() );

    CleanupStack::PopAndDestroy( &handles ); // - handles
    CleanupStack::PopAndDestroy( &context ); // - context

    PRINT( _L( "MM MTP <= CRenameObject::GenerateObjectHandleListL" ) );
    }

// -----------------------------------------------------------------------------
// CRenameObject::GetParentSuidL
// Get the full file name of old/new folder
// -----------------------------------------------------------------------------
//
void CRenameObject::GetParentSuidL( TUint32 aHandle,
    const TDesC& aFolderName )
    {
    PRINT2( _L( "MM MTP => CRenameObject::GetParentSuidL aHandle(0x%x), aFolderName(%S)" ),
        aHandle,
        &aFolderName );
    CMTPObjectMetaData* objectInfo( CMTPObjectMetaData::NewLC() ); // + objectInfo
    // get the old folder suid
    if ( iFramework .ObjectMgr().ObjectL( aHandle, *objectInfo ) )
        {
        iNewFolderName.Zero();
        iNewFolderName = objectInfo->DesC( CMTPObjectMetaData::ESuid );
        PRINT1( _L( "MM MTP <> CRenameObject::GetParentSuidL new folder full file name(%S)" ), &iNewFolderName );

        iOldFolderFullName.Zero();
        iOldFolderFullName.Append( aFolderName );

        PRINT1( _L( "MM MTP <> CRenameObject::GetParentSuidL = %S" ), &iOldFolderFullName );
        }
    else
        User::Leave( KErrCorrupt );

    CleanupStack::PopAndDestroy( objectInfo ); // - objectInfo

    PRINT( _L( "MM MTP <= CRenameObject::GetParentSuidL" ) );
    }

// -----------------------------------------------------------------------------
// CRenameObject::PerformAdditionalActionL
//
// -----------------------------------------------------------------------------
//
void CRenameObject::PerformAdditionalActionL()
    {
    PRINT( _L( "MM MTP => CRenameObject::PerformAdditionalActionL" ) );

    // update MPX DB
    TRAPD( err, iWrapper.RenameObjectL( *iObjectInfo, iFileName ) );

    // should not fail for 1 file, keep it going, as folder already renamed
    if ( err != KErrNone )
        PRINT1( _L( "MM MTP <> CRenameObject::PerformAdditionalActionL err = %d" ), err );

    PRINT( _L( "MM MTP <= CRenameObject::PerformAdditionalActionL" ) );
    }

//end of file
