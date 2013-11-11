/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPX delete helper
*
*/


#include <e32base.h>
#include <f32file.h>
#include <mpxmedia.h>
#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mpxcollectioncommanddefs.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionplaylist.h>
#include <mpxplaybackutility.h>
#include <mpxcollectionutility.h>
#include <mpxharvesterutility.h>
#include <mpxplaybackutility.h>
#include <mpxlog.h>
#include <mpxmessagegeneraldefs.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionmessagedefs.h>
#include <mpxsubscription.h>
#include <mpxharvestercommon.h>

#ifdef RD_MPX_TNM_INTEGRATION
#include <hash.h>
#include <eikenv.h>
#include <f32file.h>
#include <sysutil.h>
#include <thumbnailmanager.h>
#endif //RD_MPX_TNM_INTEGRATION
// cenrep key need to be checked whether USB cable is connected in MTP/Combined Mode
#include <mtpprivatepskeys.h>
#include <mpxcollectionuihelper.h>
#include "mpxcollectionuihelperobserver.h"
#include "mpxcollectionhelpercommon.h"

#include "mpxdeletehelper.h"

// Constants
const TInt KSQLErrGeneral = -311; // SQL General error. Don't want to include sql header here
const TInt KDeleteUpdateCount = 50;  // Max # of times to update delete status during a group delete. Value (1-100)
#ifdef RD_MPX_TNM_INTEGRATION
_LIT( KImageFileType, "image/jpeg" );
#endif //RD_MPX_TNM_INTEGRATION

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------
//
CMPXDeleteHelper::CMPXDeleteHelper(MMPXCollectionUtility& aCollectionUtility,
                                   MMPXHarvesterUtility& aHarvesterUtility,
                                   MMPXCHelperObserver& aObserver)
:   CActive(CActive::EPriorityLow),
    iCollectionUtil(aCollectionUtility),
    iHarvester(aHarvesterUtility),
    iObserver(aObserver),
    iMoreToDo(ETrue),
    iHadInUse(EFalse),
    iState(EMPXIdle),
    iDeletePercent(0),
    iDeleteIncFactor(1),
    iDeleteCount(0),
    iUsbManConnected( EFalse )
    {
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// 2nd Phase Constructor
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::ConstructL()
    {
    iFiles = new(ELeave)CDesCArrayFlat(4);
    iMessageArray = CMPXMessageArray::NewL();
    // Connect to usbman
    ConnectUsbMan();

#ifdef __USE_MESSAGE_SUBSCRIPTION
    // Subscribe to only a few messages from collection utility
    CMPXSubscription* subscription( CMPXSubscription::NewL() );
    CleanupStack::PushL( subscription );
    CMPXSubscriptionItem* subItem1( CMPXSubscriptionItem::NewL() );
    CleanupStack::PushL( subItem1 );
    subItem1->SetTObjectValueL( KMPXMessageGeneralId, KMPXMessageGeneral );
    subItem1->SetTObjectValueL( KMPXMessageGeneralEvent, TMPXCollectionMessage::EBroadcastEvent );
    subItem1->SetTObjectValueL( KMPXMessageGeneralType, EMcMsgFormatStart );
    subscription->AddItemL( *subItem1 );
    CMPXSubscriptionItem* subItem2( CMPXSubscriptionItem::CopyL( *subItem1 ));
    CleanupStack::PushL( subItem2 );
    subItem2->SetTObjectValueL( KMPXMessageGeneralType, EMcMsgDiskRemoved );
    subscription->AddItemL( *subItem2 );
    CMPXSubscriptionItem* subItem3( CMPXSubscriptionItem::CopyL( *subItem1 ));
    CleanupStack::PushL( subItem3 );
    subItem3->SetTObjectValueL( KMPXMessageGeneralType, EMcMsgUSBMassStorageStart );
    subscription->AddItemL( *subItem3 );
    CMPXSubscriptionItem* subItem4( CMPXSubscriptionItem::CopyL( *subItem1 ));
    CleanupStack::PushL( subItem4 );
    subItem4->SetTObjectValueL( KMPXMessageGeneralType, EMcMsgUSBMTPStart );
    subscription->AddItemL( *subItem4 );
    CMPXSubscriptionItem* subItem5( CMPXSubscriptionItem::CopyL( *subItem1 ));
    CleanupStack::PushL( subItem5 );
    iCollectionUtil.Collection().AddSubscriptionL( *subscription );
    CleanupStack::PopAndDestroy( subItem5 );
    CleanupStack::PopAndDestroy( subItem4 );
    CleanupStack::PopAndDestroy( subItem3 );
    CleanupStack::PopAndDestroy( subItem2 );
    CleanupStack::PopAndDestroy( subItem1 );
    CleanupStack::PopAndDestroy( subscription );
#endif

#ifdef RD_MPX_TNM_INTEGRATION

    // Create Thumbnail Manager instance. This object is the observer.
    iTNManager = CThumbnailManager::NewL( *this );
#endif //RD_MPX_TNM_INTEGRATION
    }


// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
CMPXDeleteHelper* CMPXDeleteHelper::NewL(MMPXCollectionUtility& aCollectionUtility,
                                         MMPXHarvesterUtility& aHarvesterUtility,
                                         MMPXCHelperObserver& aObserver)
    {
    CMPXDeleteHelper* self =
        new(ELeave)CMPXDeleteHelper( aCollectionUtility,
                                     aHarvesterUtility,
                                     aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMPXDeleteHelper::~CMPXDeleteHelper()
    {
    if( iPbUtil )
        {
        iPbUtil->Close();
        }
    delete iDeletePath;
    delete iFiles;
    delete iMessageArray;
#ifdef RD_MPX_TNM_INTEGRATION

    delete iTNManager;
#endif //RD_MPX_TNM_INTEGRATION
    if ( iUsbManConnected )
        {
        iUsbMan.Close();
        }
    }

// ---------------------------------------------------------------------------
// Start the delete operation
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DeleteL( CMPXCollectionPath& aPath )
    {
    MPX_DEBUG1("CMPXDeleteHelper::DeleteL()");
    MPX_DEBUG_PATH(aPath);
    TInt deletePercentFactor = 100 / KDeleteUpdateCount;

    if( iPbUtil )
        {
        iPbUtil->Close();
        iPbUtil = NULL;
        }
    iPbUtil = MMPXPlaybackUtility::UtilityL();

    iDeletePath = CMPXCollectionPath::NewL( aPath );
    iState = EMPXInitDelete;
    iDeleteCount = 0;
    iMessageArray->Reset();
    RArray<TMPXItemId> selections;
    CleanupClosePushL(selections);
    iDeletePath->SelectionL(selections);
    iItemsCount = selections.Count();
    CleanupStack::PopAndDestroy(&selections);
    iRetrievedItemsCount = 0;

    // Calculate percent increment factor used during a delete.
    if ( (iItemsCount > 0) && (iItemsCount < KDeleteUpdateCount) )
        {
        TReal factor = (KDeleteUpdateCount/iItemsCount) * deletePercentFactor;
        iDeleteIncFactor = factor;
        }
    else
        {
        iDeleteIncFactor = deletePercentFactor;
        }

    TRequestStatus* status = &iStatus;
    *status = KRequestPending;
    User::RequestComplete(status, KErrNone);

    SetActive();
    }

// ---------------------------------------------------------------------------
// From MMPXCollectionObserver
// to-do: this should be changed to HandleCollectionMessage
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::HandleCollectionMessage(CMPXMessage* aMessage, TInt /*aErr*/)
    {
    if( aMessage &&
        aMessage->IsSupported(KMPXMessageGeneralEvent) &&
        aMessage->IsSupported(KMPXMessageGeneralType) )
        {
        TInt event( aMessage->ValueTObjectL<TInt>( KMPXMessageGeneralEvent ) );
        TInt op( aMessage->ValueTObjectL<TInt>( KMPXMessageGeneralType ) );

        MPX_DEBUG3( "CMPXDeleteHelper::HandleCollectionMessageL event = %d, type = %d",
                    event, op );

        if( event == TMPXCollectionMessage::EBroadcastEvent )
            {
            if( op == EMcMsgFormatStart ||
                op == EMcMsgDiskRemoved ||
                op == EMcMsgUSBMassStorageStart ||
                op == EMcMsgUSBMTPStart )
                {
                iCancelled = ETrue;
                Cancel();
                }
            }

        }
    }

// ---------------------------------------------------------------------------
// Start the delete operation
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DoTaskStep()
    {
    MPX_DEBUG1("CMPXDeleteHelper::DoTaskStep()");

    TRequestStatus* status = &iStatus;
    *status = KRequestPending;

    TRAPD( error, DoTaskStepL() );

    User::RequestComplete( status, error );
    }

// ---------------------------------------------------------------------------
// Start the delete operation
// to-do: delete file first before removing it from the collection
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DoTaskStepL()
    {
    MPX_DEBUG1("CMPXDeleteHelper::DoTaskStepL()");

    switch(iState)
        {
        case EMPXInitDelete:
            {
            StartDeleteL();  
            iState = EMPXPreparation;
            break;
            }
        case EMPXPreparation:
            {
            RetrieveFileListL();
            iState = EMPXDelete;
            break;
            }

        case EMPXDelete:
            {
            DeleteL();
            break;
            }

        default:
            break;
        }
    }

// ----------------------------------------------------------------------------
// Handles request completion event
// ----------------------------------------------------------------------------
//
void CMPXDeleteHelper::RunL()
    {
    MPX_DEBUG3("CMPXDeleteHelper::RunL. [iMoreToDo %d] [iStatus %d]", iMoreToDo, iStatus.Int());

    TInt mtpStatus = EMtpPSStatusUninitialized;
    RProperty::Get( KMtpPSUid, KMtpPSStatus, mtpStatus);
        
    MPX_DEBUG2("CMPXCollectionViewHgImp::ConstructL, mtpstatus = %d", mtpStatus);

    if (mtpStatus != EMtpPSStatusUninitialized)
        {
        MPX_DEBUG1("MTP is active, Stop Delete");
        CompleteDelete( KErrLocked );
        return;
        }
    
    if (iMoreToDo && iStatus.Int() == KErrNone)
        {
        DoTaskStep();
        SetActive();
        }
    else
        {
        CompleteDelete(iStatus.Int());
        }
    }

// ---------------------------------------------------------------------------
// Cancel the delete operation
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DoCancel()
    {
    MPX_DEBUG3("CMPXDeleteHelper::DoCancel iStatus %d, iState %d", iStatus.Int(), iState);

    CompleteDelete(iStatus.Int());
    }

// ---------------------------------------------------------------------------
// End state for a delete operation
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::CompleteDelete( TInt aErr )
    {
    MPX_DEBUG2("CMPXDeleteHelper::CompleteDelete %d", aErr);

    TRAP_IGNORE( DoCompleteDeleteL( aErr ) );
    }

// ---------------------------------------------------------------------------
// End state for a delete operation
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DoCompleteDeleteL( TInt aErr )
    {
    MPX_DEBUG3("CMPXDeleteHelper::CompleteDeleteL error %d cancelled %d", aErr, iCancelled);
    if( iState != EMPXIdle )
        {
        // Finally callback to observer and complete task queue event
        // to-do: change HandleOperationCompletedL to HandleOperationComplete
        TInt error = iHadInUse ? KErrInUse : aErr;
        // Error cases that need to be ignored when mmc card is removed
        if( error == KErrBadName ||
            error == KErrNotReady ||
            error == KErrAbort ||
            error == KSQLErrGeneral ||
            (iCancelled && error == KErrNotFound) )
            {
            error = KErrNone;
            }

        // Reset all states
        Reset();

        // Close Harvester database transaction
        TRAP_IGNORE(iHarvester.CloseTransactionL() );
        
        // Send a complete delete command to finish off the delete operation
        //
        CMPXCommand* cmd = CMPXCommand::NewL();
        CleanupStack::PushL( cmd );
        cmd->SetTObjectValueL(KMPXCommandGeneralId,KMPXCommandIdCollectionCompleteDelete);
        cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
        cmd->SetTObjectValueL(KMPXCommandCollectionDeleteCompactDb, ETrue);
        cmd->SetCObjectValueL(KMPXCommandCollectionDeleteMsgArray, iMessageArray);
        iCollectionUtil.Collection().CommandL(*cmd);
        CleanupStack::PopAndDestroy( cmd );

        TRAP_IGNORE(iObserver.HandleOperationCompleteL( EDeleteOp, error, NULL ) );

        // Message array ownership passed to engine
        delete iMessageArray;
        iMessageArray = NULL;
        iMessageArray = CMPXMessageArray::NewL();

        if( iPbUtil )
            {
            iPbUtil->Close();
            iPbUtil = NULL;
            }
        iCancelled = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// Resets members to get ready for the next client request
//
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::Reset()
    {
    MPX_DEBUG1("CMPXDeleteHelper::Reset");
    iMoreToDo = ETrue;
    iHadInUse = EFalse;
    iDeleteCount = 0;
    iDeletePercent = 0;
    iDeleteIncFactor = 1;

    iState = EMPXIdle;

    delete iDeletePath;
    iDeletePath = NULL;

    iFiles->Reset();
    }

// ---------------------------------------------------------------------------
// Perfom cleanup of all unused data before delete
//
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::StartDeleteL()
    {
    //
    // set up the command to send to the collection
    //
    CMPXCommand* command = CMPXCommand::NewL();
    CleanupStack::PushL(command);
    
    command->SetTObjectValueL<TMPXCommandId>(
        TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralId), KMPXCommandIdCollectionPrepareDelete);
    command->SetTObjectValueL<TBool>(
        TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralDoSync), ETrue);
    command->SetCObjectValueL(
        TMPXAttribute(KMPXCommandIdCollectionPrepareDelete, EMPXCommandCollectionRetrievePath), 
        iDeletePath);
    
    // send sync prepare delete URI command    
    iCollectionUtil.Collection().CommandL(*command);
    CleanupStack::PopAndDestroy(command);
}
    
//
// Retrieve files associated with the path
//
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::RetrieveFileListL()
    {
    //
    // set up the command to send to the collection
    //
    CMPXCommand* command = CMPXCommand::NewL();
    CleanupStack::PushL(command);

    command->SetTObjectValueL<TMPXCommandId>(
        TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralId), KMPXCommandIdCollectionRetrieveUriForDeletion);
    command->SetTObjectValueL<TBool>(
        TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralDoSync), ETrue);
    command->SetCObjectValueL(
        TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrievePath),
        iDeletePath);

    // send sync retrieve URI command
    iCollectionUtil.Collection().CommandL(*command);

    //
    // return command should contain error and URI array
    //
    if (!command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrieveUriError)) ||
        !command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrieveMediaUriArray)))
        {
        User::Leave(KErrAbort);
        }

    //
    // abandon operation if an error occured removing a media from the collection
    //
    TInt error =
         command->ValueTObjectL<TInt>(
            TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrieveUriError));
    User::LeaveIfError(error);

    //
    // retrieve the list of files to be deleted
    //
    CDesCArray* files =
        command->ValueNoNewLCObjectL<CDesCArray>(
            TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrieveMediaUriArray));
    CleanupStack::PushL(files);
    ::CopyArrayL(*files, *iFiles);
    iRetrievedItemsCount +=files->MdcaCount();
    CleanupStack::PopAndDestroy(files);
    //
    // retrieve the updated path. If the original path ends at a particular artist,
    // album, genre, or composer, the path will have been expended to include the
    // songs under that category
    //
    delete iDeletePath;
    iDeletePath = NULL;
    iDeletePath =
        command->ValueCObjectL<CMPXCollectionPath>(
            TMPXAttribute(KMPXCommandIdCollectionRetrieveUriForDeletion, EMPXCommandCollectionRetrievePath));

    CleanupStack::PopAndDestroy(command);
    }

// ---------------------------------------------------------------------------
// Deletes the file from the file system then deletes it from the collection
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::DeleteL()
    {
    MPX_FUNC("CMPXDeleteHelper::DeleteL()");
    // Close the item that we are about to delete
    //
    RArray<TMPXItemId> selections;
    CleanupClosePushL(selections);
    iDeletePath->SelectionL(selections);

    TMPXItemId mediaId(0);
    if (selections.Count())
        {
        // If more than one item
        mediaId = selections[0];
        }
    else
        {
        mediaId = iDeletePath->Id(iDeletePath->Levels() - 1);
        }

    CleanupStack::PopAndDestroy(&selections);

    // Send the real id to playback engine for deletion
    iPbUtil->CommandL( EPbCmdCloseItem, mediaId );

    //
    // delete the media file from the file system and harvester's database first
    // before deleting it from the collection
    //
    if (DeleteFileL())
        {
        //
        // set up the command to send to the collection
        //
        CMPXCommand* command = CMPXMedia::NewL();
        CleanupStack::PushL(command);

        command->SetTObjectValueL<TMPXCommandId>(
            TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralId), KMPXCommandIdCollectionRemove);
        command->SetTObjectValueL<TBool>(
            TMPXAttribute(KMPXCommandContentIdGeneral, EMPXCommandGeneralDoSync), ETrue);
        command->SetCObjectValueL(
            TMPXAttribute(KMPXCommandIdCollectionRemove,EMPXCommandCollectionRemovePath),
            iDeletePath);
        command->SetTObjectValueL<TInt>(
            TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemoveMediaCount), 1);
        command->SetTObjectValueL<TBool>(KMPXCommandCollectionRemoveSuppressMsgs,ETrue);
        command->SetCObjectValueL<CMPXMessageArray>(KMPXCommandCollectionChangeMsgs, iMessageArray);
        // send sync remove command
        MPX_PERF_START( MPX_PERF_DELETE_COLLECTION );
        iCollectionUtil.Collection().CommandL(*command);
		MPX_PERF_END( MPX_PERF_DELETE_COLLECTION );

        //
        // return command should contain error, completed and media Id information
        //
        if (!command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemoveError)) ||
            !command->IsSupported(TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemoveCompleted)))
            {
            User::Leave(KErrAbort);
            }

        //
        // abandon operation if an error occured removing a media from the collection
        //
        TInt error =
             command->ValueTObjectL<TInt>(
                TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemoveError));
        User::LeaveIfError(error);

        //
        // require to send async remove command again if command completed is EFalse
        //
        TBool completed =
             command->ValueTObjectL<TBool>(
                TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemoveCompleted));
        iMoreToDo = !completed;

        //
        // retrieve the updated path with the removed media deselected and use this
        // path for the next remove command
        //
        delete iDeletePath;
        iDeletePath = NULL;
        iDeletePath =
            command->ValueCObjectL<CMPXCollectionPath>(
                TMPXAttribute(KMPXCommandIdCollectionRemove, EMPXCommandCollectionRemovePath));

        CleanupStack::PopAndDestroy(command);

        if (iFiles->MdcaCount())
            {
            iFiles->Delete(0);
            }

        // Commit every 100 items deleted, message array ownership passed to engine
        //
        iDeleteCount++;
        if( iDeleteCount%KBatchCommit == 0 )
            {
			// Commit Harvester DB
            TRAP_IGNORE( iHarvester.CloseTransactionL() );

            // Commit Collection DB
            CMPXCommand* cmd = CMPXCommand::NewL();
            CleanupStack::PushL( cmd );
            cmd->SetTObjectValueL(KMPXCommandGeneralId,KMPXCommandIdCollectionCompleteDelete);
            cmd->SetTObjectValueL(KMPXCommandGeneralDoSync, ETrue);
            cmd->SetTObjectValueL(KMPXCommandCollectionDeleteCompactDb, ETrue);
            cmd->SetCObjectValueL(KMPXCommandCollectionDeleteMsgArray, iMessageArray);
            iCollectionUtil.Collection().CommandL(*cmd);

            delete iMessageArray;
            iMessageArray = NULL;
            iMessageArray = CMPXMessageArray::NewL();
            CleanupStack::PopAndDestroy( cmd );
            }
        }
    //
    // File not deleted from the file system, skip to the next
    //
    else
        {
        TArray<TInt> selectionIndices = iDeletePath->Selection();
        if (selectionIndices.Count())
            {
            iDeletePath->Deselect(selectionIndices[0]);
            }

        iFiles->Delete(0);
        }
    
    if (iFiles->MdcaCount() == 0)
        {
        if (iRetrievedItemsCount >= iItemsCount)
            {
            iMoreToDo = EFalse;    
            iRetrievedItemsCount = 0;
            }
        else
            {
            iState = EMPXPreparation;
            }
        }
    
    // Send delete status when necessary.
    TInt deleteThreshold = ((iDeletePercent+iDeleteIncFactor)*iItemsCount)/100;
    if ( deleteThreshold > iItemsCount )
        {
        deleteThreshold = iItemsCount;
        }
    if ( iDeleteCount >= deleteThreshold )
        {
        iDeletePercent += iDeleteIncFactor;
        if ( (iDeletePercent > 100) || (iDeleteCount == iItemsCount) )
            {
            iDeletePercent = 100;
            }
        CMPXMedia* media = CMPXMedia::NewL();
        CleanupStack::PushL( media );
        media->SetTObjectValueL( KMPXMediaGeneralCount, iDeletePercent );
        iObserver.HandleOperationCompleteL( EDeleteStatusOp, KErrNone, media );
        CleanupStack::Pop( media );
        }
    }

// ---------------------------------------------------------------------------
// Deletes the file from the file system and harvester's database
// ---------------------------------------------------------------------------
//
TBool CMPXDeleteHelper::DeleteFileL()
    {
    TBool deleted(ETrue);

    if (iFiles->MdcaCount() > 0)
        {
        TPtrC uri = iFiles->MdcaPoint(0);

        if( uri.Length() > 0 )
            {
            // For playlist files, it's possible that we are deleting the file which
            // is not where the playlist is originated from. e.g. Playlist has been
            // renamed to Playlist(01) in the collection, after deleting Playlist(01)
            // from the collection, we are now attempting to delete Playlist(01).m3u
            // which is not the originating file. This is a known risk.
            CDesCArrayFlat* files = new (ELeave)CDesCArrayFlat(1);
            CleanupStack::PushL(files);
            files->AppendL(uri);

			MPX_PERF_START( MPX_PERF_DELHELPER_HARVESTER_DELETE );
            TRAPD(err, iHarvester.DeleteFilesL(*files, EFalse)); // to-do: create a sync DeleteFileL in harvester
			MPX_PERF_END( MPX_PERF_DELHELPER_HARVESTER_DELETE );

            // if the file cannot be found or is currently in use, skip to the next
            // media removal, but inform the client at the end that one of the files
            // is in use should that be the case
            if (err == KErrInUse)
                {
                iHadInUse = ETrue;
                deleted = EFalse;
                }
            else if ( err == KErrNotFound ||
                      err == KErrPathNotFound )
                {
                // Cleanup harvester for broken links
                // Have to trap ignore because .vir virtual playlists
                // do not exist and we do not know the file is a playlist
                // Since it is already KErrNotFound or KErrPathNotFound,
                //
                TRAP_IGNORE(iHarvester.RemoveFilesL(*files));
                }
            else
                {
                User::LeaveIfError(err);
                }

            CleanupStack::PopAndDestroy(files);  //lint !e961
#ifdef RD_MPX_TNM_INTEGRATION
            const TDesC& file = iFiles->MdcaPoint(0);
            // remove from thumbnail manager
            CThumbnailObjectSource* source = CThumbnailObjectSource::NewLC(
                    file, KImageFileType );
            iTNManager->DeleteThumbnails( *source );
            CleanupStack::PopAndDestroy( source );
            // remove from local drive
#endif //RD_MPX_TNM_INTEGRATION
            }
        }
    return deleted;
    }

// ---------------------------------------------------------------------------
// Stop deleting
// ---------------------------------------------------------------------------
//
void CMPXDeleteHelper::Stop()
    {
    if ( iState != EMPXIdle )
        {
        iMoreToDo = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXDeleteHelper::ThumbnailPreviewReady( 
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId /*aId*/ )
    {
    }
        

// ---------------------------------------------------------------------------
// Callback but not used here
// ---------------------------------------------------------------------------
void CMPXDeleteHelper::ThumbnailReady( TInt /*aError*/, 
        MThumbnailData& /*aThumbnail*/, TThumbnailRequestId /*aId*/ )
    {
    }
    
// ---------------------------------------------------------------------------
// CMPXDeleteHelper::ConnectUsbMan
// ---------------------------------------------------------------------------
void CMPXDeleteHelper::ConnectUsbMan()
    {
    MPX_FUNC("CMPXDeleteHelper::ConnectUsbMan()");
    if ( iUsbMan.Connect() == KErrNone )
        {
        iUsbManConnected = ETrue;
        }
    }

// END OF FILE
