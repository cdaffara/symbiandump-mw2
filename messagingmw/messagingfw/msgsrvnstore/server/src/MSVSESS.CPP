// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#ifdef _DEBUG
#undef _NO_SERVER_LOGGING_
#endif



#include <e32std.h>
#include <s32mem.h>
#include <tmsvsystemprogress.h>

#include "MSVSTD.H"
#include "MSVIPC.H"
#include "MSVREG.H"
#include "MSVREGIP.H"
#include "MSVIDS.H"
#include "MSVUIDS.H"
#include "MSVENTRY.H"
#include "MSERVER.H"
#include "MSVSERV.H"
#include "MSVLOPS.H"
#include "MSVROPS.H"
#include "MSVUTILS.H"
#include "MTSR.H"
#include "MSVPANIC.H"
#include "CMsvChangeBuffer.h"
#include "MsvSecurityCapabilitySet.h"
#include "cmsvcopystoreoperation.h"
#include "cmsvdeletestoreoperation.h"
#include "MSVAPI.H"
#include "msvindexadapter.h"
#include "msvcacheentry.h"
#include "msvsearchsortcacheentry.h"
#include "msvsearchsortcachemanager.h"
#include "msvsearchsortdeltacache.h"
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>

#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include <u32std.h>
#include "cmsvconverterwaiter.h"
#endif

const TInt KMsvOperationIndexGranularity=8;
const TInt KMsvOperationDataArrayGranularity=4;
const TInt KMsvMtmRefCountArrayGranularity=4;

_LIT(KMsvClientPanicString, "MSGS Client");

//**********************************
// TMtmGroupRefCount
//**********************************

TMtmGroupRefCount::TMtmGroupRefCount(TUid aMtmTypeUid)
: iMtmTypeUid(aMtmTypeUid), iRefCount(0)
	{}

//**********************************
// CMsvOperationData
//**********************************

CMsvOperationData::CMsvOperationData(TMsvOp aOpId)
: iOpId(aOpId)
	{
	__DECLARE_NAME(_S("CMsvOperationData"));
	}

CMsvOperationData::~CMsvOperationData()
	{
	delete iBuffer;
	}

//**********************************
// CMsvServerSession
//**********************************

CMsvServerSession::CMsvServerSession(CMsvServer& aServer)
:   CSession2(),
	iMsvServer(aServer),
	iOperations(KMsvOperationIndexGranularity),
	iMtmGroupRefCountArray(KMsvMtmRefCountArrayGranularity),
	iOperationDataArray(KMsvOperationDataArrayGranularity),
	iReceiveEntryEvents(ETrue)
	{
	__DECLARE_NAME(_S("CMsvServerSession"));
	iSessionId = iMsvServer.SessionId();
	}

CMsvServerSession* CMsvServerSession::NewL(CMsvServer& aServer, const RMessage2 &aMessage)
	{
	CMsvServerSession* self = new(ELeave) CMsvServerSession(aServer);
	CleanupStack::PushL(self);
	self->ConstructL(aMessage);
	CleanupStack::Pop();
	return self;
	}


void CMsvServerSession::ConstructL(const RMessage2 &aMessage)
	{
	iChangeBuffer = CMsvChangeBuffer::NewL();
	iBuffer = HBufC8::NewL(KMsvSessionBufferLength);
	iChildrenSelection = new(ELeave) CArrayPtrSeg<const TMsvEntry>(16);
	iChildrenSelectionIds = new(ELeave) CMsvEntrySelection;
	
	// if the index is already loaded, tell the client
	if (iMsvServer.HasContext() && iMsvServer.Context().State()==TMsvIndexLoadProgress::EIndexComplete)
		{
		TMsvNotifBuffer buffer;
		TMsvPackedChangeNotification package(buffer);
		package.Pack(EMsvIndexLoaded, 0, 0, 0);
		NotifyChangedL(buffer);

		// Send disk status notifications
		if (iMsvServer.StartupState() != EMsvNullNotification)
			{
			package.Pack(iMsvServer.StartupState(), KMsvNullIndexEntryId, iMsvServer.Context().Config().iDrive, 0);
			NotifyChangedL(buffer);
			}
	
	#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
			// if we have unsupported drives, notify the client
		if(FoundUnSupportedDrives())
			{
			package.Pack(EMsvMessageStoreNotSupported, 0, 0, 0);
			NotifyChangedL(buffer);
			}
	#endif
		}

	RThread thread;
	aMessage.Client(thread);
	iProcessName = thread.Name();
	thread.Close();
	}


CMsvServerSession::~CMsvServerSession()
	{
	// releasing locks
	__ASSERT_DEBUG(iLockedStores.Count()==0, PanicClient(iQueuedMessage, EMsvLockedStoresOnExit));
	TInt count = iLockedStores.Count();
	while (count--)
		{
		iMsvServer.IndexAdapter().ReleaseStore(iLockedStores.At(count)); // ignore any errors
		}


	// deleting outstanding operations
	__ASSERT_DEBUG(iOperations.Count()==0, PanicClient(iQueuedMessage, EMsvOutstandingOperationsOnExit));
	count=iOperations.Count();
	while (count--)
		{
		delete iOperations.At(count); // operation are deleted last in first out order
		iOperations.Delete(count);
		}

	iOperationDataArray.ResetAndDestroy();
	iMtmGroupRefCountArray.Reset();

	// Set request queued flag to false so we're not notified to close the session
	iRequestQueued = EFalse;

	iMsvServer.ClosingSession(iSessionId);

	delete iChildrenSelection;
	delete iChildrenSelectionIds;
	if(iChangeBuffer != NULL)
		{
		delete iChangeBuffer;
		}
	if(iBuffer != NULL)
		{
		delete iBuffer;
		
		}
	}

void CMsvServerSession::ServiceL(const RMessage2& aMessage)
//
//
//
	{
	if ((!iMsvServer.HasContext() || iMsvServer.Context().State()!=TMsvIndexLoadProgress::EIndexComplete) && aMessage.Function()!=EMsvNotifySessionEvent && aMessage.Function()!=EMsvCancelSessionEventNotification)
		aMessage.Complete(KErrNotReady);
	else
		{
		TRAPD(error, DoServiceL(aMessage));
		if (error)
			aMessage.Complete(error);
		}
	}

void CMsvServerSession::DoServiceL(const RMessage2& aMessage)
//
// service a client request; test the opcode and then do appropriate servicing
//
	{
	// all functions called should ensure that all synchronous messages have been completed
	switch (aMessage.Function())
		{
		case EMsvNotifySessionEvent:
			QueueSessionEventRequestL(aMessage);
			break;
		case EMsvCancelSessionEventNotification:
			CancelSessionEventRequest(aMessage);
			break;
		case EMsvOperationData:
			CopyOperationDataL(aMessage);
			break;
		case EMsvCommandData:
			CopyCommandDataL(aMessage);
			break;
		case EMsvCreateEntry:
			CreateEntryL(aMessage);
			break;
		case EMsvGetEntry:
			GetEntryL(aMessage);
			break;
		case EMsvChangeEntry:
			ChangeEntryL(aMessage);
			break;
		case EMsvGetChildren:
			GetChildrenL(aMessage);
			break;
		case EMsvGetRemainingChildren:
			GetRemainingChildrenL(aMessage);
			break;
		case EMsvDeleteEntries:
			DeleteEntriesL(aMessage);
			break;
		case EMsvLockEntry:
			LockEntryL(aMessage);
			break;
		case EMsvReleaseEntry:
			ReleaseEntryL(aMessage);
			break;
		case EMsvReadStore:
			ReadStoreL(aMessage);
			break;
		case EMsvLockStore:
			LockStoreL(aMessage);
			break;
		case EMsvReleaseStore:
			ReleaseStoreL(aMessage);
			break;
		case EMsvCancelOperation:
			CancelOperationL(aMessage);
			break;
		case EMsvOperationCompletion:
			OperationCompletionL(aMessage);
			break;
		case EMsvOperationProgress:
			OperationProgressL(aMessage);
			break;
		case EMsvOperationSystemProgress:
			OperationSystemProgressL(aMessage);
			break;
		case EMsvOperationMtm:
			OperationMtmL(aMessage);
			break;
		case EMsvMoveEntries:
			MoveEntriesL(aMessage);
			break;
		case EMsvCopyEntries:
			CopyEntriesL(aMessage);
			break;
		case EMsvMtmCommand:
			TransferCommandL(aMessage);
			break;
		case EMsvFillRegisteredMtmDllArray:
			FillRegisteredMtmDllArray(aMessage);
			break;
		case EMsvInstallMtmGroup:
			MtmGroupL(aMessage, ETrue);
			break;
		case EMsvDeInstallMtmGroup:
			MtmGroupL(aMessage, EFalse);
			break;
		case EMsvUseMtmGroup:
			UseMtmGroup(aMessage);
			break;
		case EMsvReleaseMtmGroup:
			ReleaseMtmGroup(aMessage);
			break;
		case EMsvGetMtmGroupData:
			GetMtmGroupDataL(aMessage);
			break;
		case EMsvGetMtmRequiredCapabilities:
			GetMtmRequiredCapabilitiesL(aMessage);
			break;
		case EMsvCloseServer:
			iMsvServer.CloseServer(aMessage);
			break;
		case EMsvStopService:
			iMsvServer.StopServiceL(aMessage);
			break;
		case EMsvServiceActive:
			iMsvServer.ServiceActive(aMessage);
			break;
		case EMsvServiceProgress:
			ServiceProgressL(aMessage);
			break;
		case EMsvRemoveEntry:
			RemoveEntry(aMessage);
			break;
		case EMsvGetMessageDirectory:
			MessageFolderL(aMessage);
			break;
		case EMsvSlotAvailable:
			if (iOperations.Count()<KMsvMaxSlotsAvailable)
				aMessage.Complete(KErrNone);
			else
				aMessage.Complete(KErrServerBusy);
			break;
		case EMsvSetSessionAsObserver:
			iObserverOnly=ETrue;
			aMessage.Complete(KErrNone);
			break;
		case EMsvSetFailure:
			iMsvServer.SetFailure((TMsvFailure)aMessage.Int0(), aMessage.Int1(), aMessage.Int2(), aMessage.Int3());
			aMessage.Complete(KErrNone);
			break;
		case EMsvChangeAttributes:
			ChangeAttributesL(aMessage);
			break;
		case EMsvGetChildIds:
			GetChildIdsL(aMessage);
			break;
		case EMsvChangeDrive:
			ChangeDriveL(aMessage);
			break;
		case EMsvOutstandingOperations:
			OutstandingOperationsL(aMessage);
			break;
		case EMsvGetNotifySequence:
			GetNotifySequenceL(aMessage);
			break;
		case EMsvSetReceiveEntyEvents:
			SetReceiveEntryEvents(aMessage);
			break;
		case EMsvDecStoreReaderCount:
			DecStoreReaderCountL(aMessage);
			break;
		case EMsvGetMessageDrive:
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
			MessageDrive(aMessage);
#else
			aMessage.Complete(iMsvServer.Context().Config().iDrive);
#endif
			break;
		case EMsvCreateAttachmentForWrite:
			CreateAttachmentForWriteL(aMessage);
			break;
		case EMsvReplaceAttachmentForWrite:
			ReplaceAttachmentForWriteL(aMessage);
			break;
		case EMsvOpenAttachment:
			OpenAttachmentL(aMessage);
			break;
		case EMsvOpenAttachmentForWrite:
			OpenAttachmentForWriteL(aMessage);
			break;
		case EMsvDeleteAttachment:
			DeleteAttachmentL(aMessage);
			break;
		case EMsvRenameAttachment:
			RenameAttachmentL(aMessage);
			break;
		case EMsvFileExists :
			FileExistsL(aMessage);
			break;
		case EMsvGetAttachmentFilePath:
			GetAttachmentFilePathL(aMessage);
			break;
		case EMsvOpenFileStoreForRead:
			OpenFileStoreForReadL(aMessage);
			break;
		case EMsvOpenTempStoreFile:
			OpenTempStoreFileL(aMessage);
			break;
		case EMsvReplaceFileStore:
			ReplaceFileStoreL(aMessage);
			break;
		case EMsvDeleteFileStore:
			DeleteFileStoreL(aMessage);
			break;
		case EMsvFileStoreExists:
			FileStoreExistsL(aMessage);
			break;
		case EMsvGetAndClearIndexCorruptFlag:
			aMessage.Complete(iMsvServer.Context().GetAndClearIndexCorruptFlagL());
			break;
		case EMsvCopyStore:
			CopyStoreL(aMessage);
			break;
		case EMsvDeleteStore:
			DeleteStoreL(aMessage);
			break;
		case EMsvDriveContainsStore:
			aMessage.Complete(MessageServer::DriveContainsStore(iMsvServer.FileSession(),aMessage.Int0()));
			break;
		case EMsvMessageStoreDrivePresent:
			aMessage.Complete(MessageServer::IsMessageStoreDrivePresentL(iMsvServer.FileSession()));			
			break;
		case EMsvGetBodyTextFilePath:
			 BodyTextFilePathL(aMessage);
			 break;
		case EMsvOpenTextFileForRead:
			 OpenPlainTextFileL(aMessage);
			 break;
		case EMsvCreatePlainTextFile:
			 CreatePlainTextFileL(aMessage);
			 break;
		case EMsvDeletePlainTextFile:
			DeletePlainTextFileL(aMessage); 
			break;
		case EMsvReplacePlainTextFile:
			ReplacePlainTextFileL(aMessage);
			break;

		case EMsvGetNonOperationMtmData:
			{
			GetNonOperationMtmDataL(aMessage);
			break;
			}
		case EMsvSearchSortOperation: // On HEADER AND BODY
			SearchSortOnHeaderAndBodytMsgL(aMessage);
			break;
	
		case EMsvSearchSortOnIndexEntry: //On INDEX ENTRY.
			SearchSortOnIndexEntryL(aMessage);
			break;

		case EMsvGetResult ://Index entry result.
			SendSearchSortIndexEntryResultL(aMessage);
			break;
		
		case EMsvGetIdsOrResult://header result.
			SendResultOrIdsToSearchL(aMessage);
			break;
		
		case EMsvGetResultCount:
			GetSearchSortResultCountL(aMessage);
			break;
			
		case EMsvGetNextId:
			GetResultInIteratorL(aMessage);
			break;
		
		case EMsvGetNextEntry:
			GetNextEntryInIteratorL(aMessage);
			break;
		
		case EMsvGetQueryId:
			GetQueryIDL(aMessage);
			break;
	
		case EMsvUnmarQueryId:
			QueryUnMarkedL(aMessage);
			break;

		case EMsvIdWithSortFiled:
			UpdateSearchSortCacheWithSortFiledL(aMessage);
			break;
	
		case EMsvSearchSortQueryId:
			GetResultForQueryIDL(aMessage);
			break;

		case EMsvQueryData:
			CopyQueryDataL(aMessage);
			break;
			
		case EMsvGetSearchSortProgress:
			GetSearchSortProgressInfoL(aMessage);
			break;
			
		case EMsvCancelSearchSortOp:
			SearchSortOperationCancelL(aMessage);
			break;
			
		case EMsvUpdateAndSort:
			UpdateSearchSortResultsAndSortByEntryL(aMessage);
			break;

// Code changes for PREQ 557.	   
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		case EMsvGetChildrenALL:
			GetChildrenAllL(aMessage);
			break;
		case EMsvGetChildIdsALL:
			GetChildIdsAllL(aMessage);
			break;
		case EMsvGetCurrentDriveInfo:
			CurrentDriveInfoL(aMessage);
			break;
		case EMsvGetDriveList:
			DriveListL(aMessage);	   
			break;
		case EMsvGetAvailableDriveList:
			AvailableDriveListL(aMessage);
			break;
		case EMsvAddDriveToDriveList:
			AddDriveL(aMessage);
			break;
		case EMsvRemoveDriveFromDriveList:
			RemoveDriveL(aMessage); 
			break;
		case EMsvUpdateDrivePriority:
			UpdateDrivePriorityL(aMessage);
			break;
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
		case EMsvResetRepository:
			ResetRepositoryL(aMessage);
			break;
		case EMsvPrintCache:
			PrintCache(aMessage);
			break;
#endif	  // #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#endif	  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		case EMsvCreateStore:
			CreateHeaderTableL(aMessage);
			break;
		case EMsvCheckStoreExists:
			DoesStoreExistsL(aMessage);
			break;
		case EMsvLastErrorMessage:
			LastErrorMessageL(aMessage);
			break;
		case EMsvCreateHeaderEntry:
			CreateHeaderEntryL(aMessage);
			break;
		case EMsvLoadHeaderEntry:
			LoadHeaderEntryL(aMessage);
			break;
		case EMsvUpdateHeaderEntry:
			UpdateHeaderEntryL(aMessage);
			break;
		case EMsvDeleteHeaderEntry:
			DeleteHeaderEntryL(aMessage);
			break;							  
		case EMsvCheckAnyStoreExists:
			DoesAnyStoreExists(aMessage);
			break;
		case EMsvCheckHeaderTableExist:
			{
			DoesHeaderTableExist(aMessage);
			break;
			}
		case EMsvGetConvertibleDriveList:
			GetConvertibleDriveListL(aMessage);
			break;
		case EMsvConvertMessageStore:
			ConvertMessageStoreL(aMessage);
			break;
		case EMsvGetConversionStatus:
			GetConversionStatus(aMessage);
			break;
		case EMsvCancelConversionRequest:
			CancelConversionRequestL(aMessage);
			break;
#endif	  // #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		case EMsvChangeEntries:
		     ChangeEntriesL(aMessage);
		     break;
		default:
			PanicClient(aMessage, EMsvBadRequest);
			break;
		}
	}


void CMsvServerSession::QueueSessionEventRequestL(const RMessage2 &aMessage)
//
// The client is requesting notifcation of any changes
//
	{
	if (iRequestQueued)
		{
		PanicClient(aMessage, EMsvDuplicatedChangeRequest);
		return;
		}

	if (iChangeBuffer->IsEmpty())
		{
		// queue this request until a change occurs
		iQueuedMessage = aMessage;
		iRequestQueued = ETrue;
		}
	else
		{
		// inform client of change which has been queued
		SendNotificationL(aMessage);
		}
	}

void CMsvServerSession::SendNotificationL(const RMessage2& aMessage)
	{
	// inform client of change which has been queued
	TPckgBuf<TUint32> sequence;
	aMessage.WriteL(0, iChangeBuffer->Next(sequence()));

	// Tell the client the notification sequence number
	aMessage.WriteL(1, sequence);

#ifndef _NO_SERVER_LOGGING_
	iMsvServer.Log(_L("Sending notification sequence %d"), sequence());
#endif

	aMessage.Complete(KErrNone);
	iChangeBuffer->Out();
	}

void CMsvServerSession::CancelSessionEventRequest(const RMessage2 &aMessage)
//
// The client is cancelling the outstanding request
//
	{
	if (iRequestQueued)
		{
		iQueuedMessage.Complete(KErrNone);
		iRequestQueued = EFalse;
		}
	aMessage.Complete(KErrNone);
	}



void CMsvServerSession::ReadBufferL(const RMessage2& aMessage, TInt aParam, HBufC8*& aBuffer)
//
// Copies a buffer from the client
// Only fails if there is not enough memory to increase the buffer size (if needed)
//
	{
	TInt desLen = aMessage.GetDesLength(aParam);

	if(desLen >= 0)
		{
		TBool alloced=EFalse;
		HBufC8* localBuffer=NULL;
		
		if (aBuffer==NULL)
			{
			localBuffer = HBufC8::NewLC(desLen);
			alloced=ETrue;
			}
		else if (desLen > aBuffer->Des().MaxLength())
			{
			// we have to increase the size of iBuffer
			aBuffer->Des().SetLength(0); // to avoid copying the contents
			localBuffer = aBuffer->ReAlloc(desLen);
			if (localBuffer==NULL)
				{
				User::Leave(KErrNoMemory); // unable to create buffer large enough
				}
			}
		
		if(localBuffer)
			{
			TPtr8 desPtr = localBuffer->Des();
			aMessage.ReadL(aParam, desPtr);
				
			if (alloced)
				{
				CleanupStack::Pop(); // localBuffer
				}
			
			aBuffer = localBuffer;  
			}
		//use aBuffer
		else
			{
			TPtr8 desPtr = aBuffer->Des();
			aMessage.ReadL(aParam, desPtr); 
			}
		}
	else
		{
		// desLen is negative leave with an error.
		User::Leave(KErrArgument);
		}
	}

void CMsvServerSession::WriteBufferL(const RMessage2& aMessage, TInt aParam)
//
// Copies the packed entry buffer from the client
// Only fails if the client buffer is not large enough
//
	{
	TInt desLen = aMessage.GetDesMaxLength(aParam);
	if (desLen < iBuffer->Des().Length())
		User::Leave(KErrOverflow);
	aMessage.WriteL(aParam, iBuffer->Des());
	}


void CMsvServerSession::WriteL(const RMessage2& aMessage, TInt aParam, const TDesC8& aDes)
//
// Write a descriptor to the client thread; if unsuccessful,leave so that client can handle it.
//
	{
	TInt error = aMessage.Write(aParam, aDes);
	if (error)
		{
		User::Leave(error);
		}
	}


void CMsvServerSession::ReadL(const RMessage2& aMessage, TInt aParam, TDes8& aDes)
//
// Reads a descriptor from the client thread; if unsuccessful,leave so that client can handle it
//
	{
	TInt error = aMessage.Read(aParam, aDes);
	if (error)
		{
		User::Leave(error);
		}
	}



void CMsvServerSession::CopyOperationDataL(const RMessage2 &aMessage)
//
// Copies operation data from the client and stores for the asynchronous operation request
//
	{
	TMsvOp operationId = aMessage.Int0();
	CMsvOperationData* opData = new (ELeave) CMsvOperationData(operationId);
	CleanupStack::PushL(opData);
	ReadBufferL(aMessage, 1, opData->iBuffer);
	iOperationDataArray.AppendL(opData);
	CleanupStack::Pop(); // opData
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::CopyCommandDataL(const RMessage2 &aMessage)
//
// Copies operation data from the client and stores for the asynchronous operation request
//
	{
	TMsvOp operationId = aMessage.Int0();
	// operation data
	CMsvOperationData* opData = new (ELeave) CMsvOperationData(operationId);
	CleanupStack::PushL(opData);
	ReadBufferL(aMessage, 1, opData->iBuffer);

	// This is to check if the buffer is a valid , before it is added to 
	// the operationData array
	TMsvPackedOperation packedOperation(opData->iBuffer);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt command, temp;
	packedOperation.UnpackL(*selection, command, temp);
	CleanupStack::PopAndDestroy(selection);

	iOperationDataArray.AppendL(opData);
	
	// additional parameter
	CMsvOperationData* opParam = new (ELeave) CMsvOperationData(operationId);
	CleanupStack::PushL(opParam);
	ReadBufferL(aMessage, 2, opParam->iBuffer);
	iOperationDataArray.AppendL(opParam);
	CleanupStack::Pop(2, opData); // opParam, opData
	//
	aMessage.Complete(KErrNone);
	}


HBufC8* CMsvServerSession::RecoverOperationData(TMsvOp aOpId)
//
// Returns the operation data previously copied from the client
// NOTE the calling function is responsible for deleting the returned HBufC
//
	{
	// find the data
	TInt count = 0;
	TInt totalCount=iOperationDataArray.Count();
	for (; count<totalCount; count++)
		{
		if (iOperationDataArray.At(count)->OperationId()==aOpId)
			break;
		}
	// Panic during debug if cannot find data
	__ASSERT_DEBUG(count<iOperationDataArray.Count(), PanicServer(EMsvEntryDataNotFound));
	// At runtime, return NULL.
	if(count >= iOperationDataArray.Count())
		{
		return NULL;
		}
	// return the data to the caller, and delete the container object
	HBufC8* opData = iOperationDataArray.At(count)->iBuffer;
	iOperationDataArray.At(count)->iBuffer = NULL;
	delete iOperationDataArray.At(count);
	iOperationDataArray.Delete(count);
	return opData;
	}


void CMsvServerSession::CreateEntryL(const RMessage2 &aMessage)
//
// Create a entry in the index
//
	{
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	CleanupStack::PushL(opData);
	// Unpack the data
	TMsvPackedEntry packedEntry(opData);
	TMsvEntry entry;
	packedEntry.UnpackEntry(entry);

	// Check the content of the entry are conforming to policy
	if (!MsvUtils::ValidEntry(entry, ETrue))
		{
		PanicClient(aMessage, EMsvIncorrectEntry);
		User::Leave(KErrNotSupported);
		}

	// Check if the entry is local or the entry is a service
	TBool local=ETrue;
	// Police request - client must be able to create the entry.
	iMsvServer.PoliceCreateEntryL(aMessage, entry, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CreateEntryL"));

	// For purpose of creating a service entry, it is considered local as an MTM
	// is not required to do the work.
	if( entry.iType == KUidMsvServiceEntry )
		local = ETrue;

	// start the change for local or remote entries
	if (local)
		{
		TSecureId ownerId = aMessage.Int1();

		DoCreateLocalEntryL(entry, operationId, aMessage, ownerId);
		}
	else
		DoCreateRemoteEntryL(entry, operationId, aMessage);

	CleanupStack::PopAndDestroy(); // opData
	}

void CMsvServerSession::DoCreateLocalEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId)
//
// Create a local entry in the index
//
	{
	CMsvLocalCreateOperation* operation = new(ELeave) CMsvLocalCreateOperation(aMessage, aOpId, aEntry, iMsvServer);
	CleanupStack::PushL(operation);
	iOperations.AppendL(operation);
	CleanupStack::Pop(operation);
	operation->Start(aOwnerId);
	}

void CMsvServerSession::DoCreateRemoteEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage)
//
// Create a remote entry in the index
//
	{
	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// create the operation and pass it to the server for starting/queuing
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, aEntry.iMtm, aEntry.iServiceId, iSessionId, iMsvServer);
	CleanupStack::PushL(operation);
	operation->CreateL(aEntry);
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, ETrue);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::ChangeEntryL(const RMessage2 &aMessage)
//
// Changes the entry
//
	{
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
		
	CleanupStack::PushL(opData);
	// Unpack the data
	TMsvPackedEntry packedEntry(opData);
	TMsvEntry entry;
	packedEntry.UnpackEntry(entry);

	// Check the content of the entry are conforming to policy
	__ASSERT_DEBUG(MsvUtils::ValidEntry(entry), PanicClient(aMessage, EMsvIncorrectEntry));
	if (!MsvUtils::ValidEntry(entry))
		User::Leave(KErrNotSupported);

	// Check if the entry is local or the entry is a service
	TBool local=ETrue;
	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, entry, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ChangeEntryL"));

	// For purpose of changing a service entry, it is considered local as an MTM
	// is not required to do the work.
	if( entry.iType == KUidMsvServiceEntry )
		local = ETrue;

	// start the change for local or remote entries
	if (local)
		{
		// Extract the owner ID from the message.
		TSecureId ownerId = aMessage.Int1();

		DoChangeLocalEntryL(entry, operationId, aMessage, ownerId);
		}
	else
		DoChangeRemoteEntryL(entry, operationId, aMessage);

	CleanupStack::PopAndDestroy(); // opData
	}

//
// Changes the selection of id.
//

void CMsvServerSession::ChangeEntriesL(const RMessage2 &aMessage)
    {
     // Recover the operation data
     TMsvOp operationId = aMessage.Int0();
     HBufC8* opData = RecoverOperationData(operationId);
     // Check for NULL data entry to be changed, must have been given incorrect id for argument.
     if(opData == NULL)
         {
         aMessage.Complete(KErrArgument);
         return;
         }
     CleanupStack::PushL(opData);

     // unpack the data
     TMsvPackedOperation packedOperation(opData);
     
     CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
     CleanupStack::PushL(selection);
     TInt target,temp;
     packedOperation.UnpackL(*selection, target, temp);

     if (selection->Count() == 0)
         {
         PanicClient(aMessage, EMsvNoEntriesInChangeSelection);
         aMessage.Complete(KErrNotFound);
         return;
         }
     // Find the first entry in the selection which exists
     TInt count=selection->Count();
     while (count--)
         {
         TBool entryExsists = EFalse;
         entryExsists = iMsvServer.IndexAdapter().EntryExists(selection->At(count));
         if (entryExsists)
             break;
         }
 
       // Check if the entry is local or the entry is a service
      TBool local=ETrue;
      TMsvEntry* entry=NULL;
      User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(selection->At(0), entry));
     
      // Police request - client must be able to modify the entry.
      iMsvServer.PoliceModifyEntryL(aMessage, *entry, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ChangeEntryL"));
             
      // For purpose of changing a service entry, it is considered local as an MTM
      // is not required to do the work.
      if( entry->iType == KUidMsvServiceEntry )
          local = ETrue;

      // start the change for local or remote entries
      if (local)
          {
          // Extract the owner ID from the message.
          TSecureId ownerId = aMessage.Int1();

          DoChangeLocalEntriesL(selection, operationId, aMessage, ownerId, target);
          }
      else
          DoChangeRemoteEntriesL(selection, operationId, aMessage,target);

      CleanupStack::PopAndDestroy(); // opData
   }

void CMsvServerSession::DoChangeLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId, TInt mark)
    {
    CMsvLocalChangeEntriesOperation* operation = new(ELeave) CMsvLocalChangeEntriesOperation(aMessage, aOpId, aSelection, iMsvServer,mark);
    CleanupStack::Pop(); // selection
    CleanupStack::PushL(operation);
   
    TBool forcedUpdate = (aOwnerId != aMessage.SecureId());
    operation->StartL(aOwnerId, forcedUpdate);

    iOperations.AppendL(operation);
    CleanupStack::Pop(); // operation

    
    }

void CMsvServerSession::DoChangeRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage,TInt mark)
//
// Change a selection of entry under a remote service
//
    {
    // make sure that the operation can be added to the list
     iOperations.SetReserveL(iOperations.Count()+1);

     TMsvEntry* entry=NULL;
     User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aSelection->At(0), entry));
 
     // create the operation
     CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, entry->iMtm, entry->iServiceId, iSessionId, iMsvServer);
     CleanupStack::Pop(); // aSelection
     CleanupStack::PushL(operation);
     operation->ChangeEntriesL(aSelection,mark);
     
     iMsvServer.StartOperationL(*operation, iSessionId, aMessage, ETrue);
     iOperations.AppendL(operation); // will not fail - see start of function
     CleanupStack::Pop(); // operation
   
    }


void CMsvServerSession::DoChangeLocalEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage, TSecureId aOwnerId)
//
// Create a local entry in the index
//
	{
	CMsvLocalChangeOperation* operation = new(ELeave) CMsvLocalChangeOperation(aMessage, aOpId, aEntry, iMsvServer);
	CleanupStack::PushL(operation);
	// Check the given owner ID - if NULL then this is not a forced update, but
	// need to set the ID to that of the requesting process.
	TBool forcedUpdate = (aOwnerId != aMessage.SecureId());

	operation->StartL(aOwnerId, forcedUpdate);
	iOperations.AppendL(operation);
	CleanupStack::Pop(); //operation
	}


void CMsvServerSession::DoChangeRemoteEntryL(const TMsvEntry& aEntry, TMsvOp aOpId, const RMessage2 &aMessage)
//
// Change a entry under a remote service
//
	{
	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// create the operation
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, aEntry.iMtm, aEntry.iServiceId, iSessionId, iMsvServer);
	CleanupStack::PushL(operation);
	operation->ChangeL(aEntry);
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, ETrue);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::PackEntryAndWriteBufferL(const RMessage2 &aMessage, const TInt aParam, const TMsvEntry& aEntry, const TMsvId& aServiceId)
//
//
	{
	// package the entry and service id in same buffer
	TMsvPackedEntry packedEntry(iBuffer);
	TInt error = packedEntry.PackEntryAndService(aEntry, aServiceId);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = packedEntry.PackEntryAndService(aEntry, aServiceId);
		}
	// max destination length is passed from client
	TInt maxDesLen = aMessage.Int2();
    if (maxDesLen < iBuffer->Des().Length())
		{
        User::Leave(KErrOverflow);
		}

    aMessage.WriteL(aParam, iBuffer->Des());
	}

void CMsvServerSession::GetEntryL(const RMessage2 &aMessage)
//
// Returns the index entry for the given id
//
	{
	// copy across the entry id to get
	TMsvId id = aMessage.Int0();
	TMsvEntry* entryPtr;
	TSecureId ownerId;
	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().GetEntry(id, entryPtr, ownerId);
	if (error==KErrNone)
		{
		// Police request - client must be able to read the entry.
		iMsvServer.PoliceReadEntryL(aMessage, ownerId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetEntryL"));

		// get the owning service and write that back
		TMsvId service;
		if (id == KMsvRootIndexEntryId)
			service = KMsvRootIndexEntryId;
		else
			{
			iMsvServer.IndexAdapter().OwningService(id, service); // error ignore as the entry exists
			}
		// write the entry back
		PackEntryAndWriteBufferL(aMessage, 1, *entryPtr, service);
		}

	aMessage.Complete(error);
	}

void CMsvServerSession::OutstandingOperationsL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> outstanding;
	outstanding() = iMsvServer.OutstandingOperations() > 0;
	WriteL(aMessage, 0, outstanding);
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::GetNotifySequenceL(const RMessage2& aMessage)
	{
	TPckgBuf<TUint32> sequence;
	sequence() = iChangeBuffer->NotifySequence();
	WriteL(aMessage, 0, sequence);
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::GetChildrenL(const RMessage2 &aMessage)
//
// Gets the children of an entry
//
	{
	// reset
	iChildrenSelection->Reset();
	iChildrenSelectionIds->Reset();

	// copy across the children details structure
	TPckgBuf<TMsvChildrenDetails> children;
	aMessage.ReadL(0, children);

	// Check that the children details arguments are empty. Panic in debug mode but try to handle it gracefully in
	// release code.
	__ASSERT_DEBUG( children().iTotalNumberChildren==0 &&
					children().iNumberChildrenInArray==0, PanicServer(EMsvChildrenDetailsNotEmpty2));

	if( children().iTotalNumberChildren != 0 || children().iNumberChildrenInArray != 0 )
		{
		aMessage.Complete(KErrArgument);
		return;
		}
		
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Return error, if the passed parent-id 
	// is a non-current standard id.
	if( (IsStandardId(children().iParentId)) &&
		(KCurrentDriveId != GetDriveId(children().iParentId))
	  )
		{
		aMessage.Complete(KErrArgument);
		return;
		}	   
#endif

	// copy across the sort order
	TPckgBuf<TMsvSelectionOrdering> order;
	aMessage.ReadL(1, order);

	// get the children as a selection
	// Need to filter the list via the client secure ID if the client is not
	// trusted with Read User Data.
	TBool filterByOwnerId = !aMessage.HasCapability(ECapabilityReadUserData);
	iMsvServer.IndexAdapter().GetChildrenL(children().iParentId, *iChildrenSelection, order(), KUidMsvNullEntry, filterByOwnerId, aMessage.SecureId());
	// return number of children
	children().iTotalNumberChildren = iChildrenSelection->Count();

	// package up the entries
	iBuffer->Des().SetMax();
	TMsvPackedEntryArray packedEntryArray(iBuffer);
	TInt count=0;
	TInt error=KErrNone;
	TInt totalCount=iChildrenSelection->Count();
	for (; count<totalCount; count++)
		{
		error = packedEntryArray.PackEntry(*iChildrenSelection->At(count));
		if (error)
			{
			children().iLastEntryInArray = count;
			break;
			}
		}
	// return number of children in the array
	children().iNumberChildrenInArray = count;

	// Try to write the buffer to the client (if any entries are in the array)
    if (children().iNumberChildrenInArray)
    	{
        TRAPD(bufferError, WriteL(aMessage, 2, iBuffer->Des()));
        if(bufferError!=KErrNone)
        	{
        	// Writing data into client buffer is failed, we should not send incorrect count to client.
			error = bufferError;
        	children().iNumberChildrenInArray = 0;
        	children().iLastEntryInArray = 0;
        	}
        }

	// write the children to client
	WriteL(aMessage, 0, children);

	if (error==KErrNone)
		{
		// reset the member data
		iChildrenSelection->Reset();
		}
	else
		{
		// keep an list of the ids separatelyin case they are deleted by another client
		TInt totalCount=iChildrenSelection->Count();
		for (count=0; count<totalCount; count++)
			iChildrenSelectionIds->AppendL(iChildrenSelection->At(count)->Id());
		iChildrenDetails = children();
		}

	// signal the client and finished with selection
	aMessage.Complete(error);
	}




void CMsvServerSession::GetRemainingChildrenL(const RMessage2 &aMessage)
//
// Gets the remaining children of an entry when the buffer is to small
//
	{
	// Check the arguments. Panic in debug mode but try to handle it gracefully in
	// release code.
	__ASSERT_DEBUG(iChildrenSelection->Count()!=0, PanicServer(EMsvChildrenSelectionNull));
	__ASSERT_DEBUG(iChildrenSelection->Count() == iChildrenSelectionIds->Count(), PanicServer(EMsvChildrenSelectionCountsMismatch));

	if( (iChildrenSelection->Count()==0) || (iChildrenSelection->Count() != iChildrenSelectionIds->Count()) )
		{
		aMessage.Complete(KErrArgument);
		return;
		}

	// copy across the children details structure
	TPckgBuf<TMsvChildrenDetails> pDetails;
	aMessage.ReadL(0, pDetails);

	// Check the arguments. Panic in debug mode but try to handle it gracefully in
	// release code.
	__ASSERT_DEBUG( iChildrenDetails.iParentId == pDetails().iParentId &&
					iChildrenDetails.iTotalNumberChildren == pDetails().iTotalNumberChildren &&
					iChildrenDetails.iNumberChildrenInArray == pDetails().iNumberChildrenInArray, PanicServer(EMsvChildrenDetailsNotEmpty1));

	if( iChildrenDetails.iParentId != pDetails().iParentId ||
		iChildrenDetails.iTotalNumberChildren != pDetails().iTotalNumberChildren ||
		iChildrenDetails.iNumberChildrenInArray != pDetails().iNumberChildrenInArray )
		{
		aMessage.Complete(KErrArgument);
		return;
		}

	iChildrenDetails=pDetails();
	// Make a local copy of details, this needs to be used when the client buffer is not sufficient enough to
	// pack data.
	TMsvChildrenDetails localCopy = pDetails();
	
	// package up the entries
	iBuffer->Des().SetMax();
	TMsvPackedEntryArray packedEntryArray(iBuffer);
	TInt error=KErrNone;
	TInt count=iChildrenDetails.iLastEntryInArray;
	iChildrenDetails.iLastEntryInArray=-1;

	TInt childCount=iChildrenSelection->Count();
	TInt missingCount = 0;

	for (; count<childCount; count++)
		{
		// The iChildrenSelection contains pointers to TMsvEntry objects. If one of those
		// objects has been deleted then the pointer will no longer be valid so we need
		// to get the entry again using the ID from iChildrenSelectionIds.
		TMsvEntry* entryPtr;
		TInt err = KErrNone;
		err = iMsvServer.IndexAdapter().GetEntry(iChildrenSelectionIds->At(count), entryPtr);
		if (err != KErrNotFound)
			{
#ifndef _NO_SERVER_LOGGING_
			if (entryPtr != iChildrenSelection->At(count))
				{
				iMsvServer.Log(_L("GetRemainingChildrenL() - Ptr mismatch for id %d (%x %x)"), iChildrenSelectionIds->At(count), iChildrenSelection->At(count), entryPtr);
				}
#endif

			error = packedEntryArray.PackEntry(*entryPtr);

			if (error)
				{
				// the buffer is not large enough to pack all the entries
				iChildrenDetails.iLastEntryInArray = count;
				if ((count - missingCount - pDetails().iLastEntryInArray) == 0)
					{
					// Couldn't fit entry in buffer
					// Increase size of buffer - client will do this as well
					iBuffer->Des().SetLength(0);
					iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
					}
				break;

				}
			}
		else
			{
#ifndef _NO_SERVER_LOGGING_
			iMsvServer.Log(_L("GetRemainingChildrenL() - Ignore missing id %d"), iChildrenSelectionIds->At(count));
#endif

			++missingCount;
			}
		}

	// return number of children in the array
	iChildrenDetails.iNumberChildrenInArray = count - missingCount - pDetails().iLastEntryInArray;

	// write the array to the client (if any entries are in the array)
	TRAPD(err, WriteBufferL(aMessage, 2));
	if(err)
		{
		if(err == KErrOverflow)
		 {
		 localCopy.iNumberChildrenInArray = 0;
		 iChildrenDetails = localCopy;
		 pDetails() = iChildrenDetails;
		 WriteL(aMessage, 0, pDetails);
		 User::Leave(KErrOverflow);
		 }
	 else
		 {
		 User::Leave(err);
		 }   
	 }   
			 

	// write the details to client
	pDetails() = iChildrenDetails;
	WriteL(aMessage, 0, pDetails);

	if (error==KErrNone)
		{
		// reset the member data
		iChildrenDetails = TMsvChildrenDetails();
		iChildrenSelection->Reset();
		iChildrenSelectionIds->Reset();
		}

	// signal the client and finished with selection
	aMessage.Complete(error);
	}


void CMsvServerSession::DeleteEntriesL(const RMessage2 &aMessage)
//
// Deleting entries
//
	{
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	CleanupStack::PushL(opData);
	// unpack the data
	TMsvPackedOperation packedOperation(opData);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt temp1, temp2;
	packedOperation.UnpackL(*selection, temp1, temp2);

/**
Added code to begin transaction for bulk deletion.
*/
	iMsvServer.IndexAdapter().GetDbAdapter()->BeginTransactionL();

	if (selection->Count())
		{
		// Check if the entries are local (inc services) or remote
		TBool local;
		TMsvEntry* entry=NULL;
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(selection->At(0), entry));
		// Police request - client must be able to modify the entry.
		iMsvServer.PoliceModifyEntryL(aMessage, *entry, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DeleteEntriesL"));
		// start the move for local or remote messages
		if (local || entry->iType==KUidMsvServiceEntry)
			{
			DoDeleteLocalEntriesL(selection, operationId, aMessage);
			TMsgType aType(EDeletedMsg);
			if(CMSvSearchSortCacheManager::Instance()->iManagerEntry != NULL)
				{
				if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0)
					{
					TInt count=selection->Count();
					TInt i=0;
					while(i < count)
						{
						CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(selection->At(i),aType);
						i++;	
						}
					}
				}
			}
		else
			{
			DoDeleteRemoteEntriesL(selection, operationId, aMessage);
			TMsgType aType(EDeletedMsg);
			if(CMSvSearchSortCacheManager::Instance()->iManagerEntry != NULL)
				{
				if(CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count()>0)
					{
					TInt count=selection->Count();
					TInt i=0;
					while(i < count)
						{
						CMsvSearchSortDeltaCache::Instance()->EntryInDeltaCache(selection->At(i),aType);
						i++;	
						}
					}
				}
			}
		}
	else
		{
		PanicClient(aMessage, EMsvNoEntriesInDeleteSelection);
		aMessage.Complete(KErrNotFound);
		}

	CleanupStack::PopAndDestroy(); // opData
	}


void CMsvServerSession::DoDeleteLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage)
//
//
//
	{
	CMsvLocalDeleteOperation* operation = new(ELeave) CMsvLocalDeleteOperation(aMessage, aOpId, aSelection, iMsvServer);
	CleanupStack::Pop(); // selection
	CleanupStack::PushL(operation);
	operation->StartL();
	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::DoDeleteRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvOp aOpId, const RMessage2 &aMessage)
//
//
//
	{
	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// find the first entry being acted on
	TMsvEntry* entry=NULL;
	User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aSelection->At(0), entry));
	// create the operation
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, entry->iMtm, entry->iServiceId, iSessionId, iMsvServer);
	CleanupStack::Pop();
	CleanupStack::PushL(operation);

	// set up the operation type and pass the data
	operation->DeleteAll(aSelection);

	// either start the operation or queue it
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, ETrue);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::LockEntryL(const RMessage2 &aMessage)
//
// Locks an entry in the index
//
	{
	TMsvId id = aMessage.Int0();
	aMessage.Complete(iMsvServer.IndexAdapter().LockEntry(id));
	}


void CMsvServerSession::ReleaseEntryL(const RMessage2 &aMessage)
//
// Releases the lock on an entry in the index
//
	{
	TMsvId id = aMessage.Int0();
	aMessage.Complete(iMsvServer.IndexAdapter().ReleaseEntry(id));
	}

void CMsvServerSession::ReadStoreL(const RMessage2 &aMessage)
//
// Can only read from a store that has nnot been locked
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iMsvServer.IndexAdapter().ErrorState());
	TMsvId id = aMessage.Int0();

	// Police request - client must be able to read the entry.
	iMsvServer.PoliceReadEntryL(aMessage, id, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ReadStoreL"));

	TBool locked;
	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().IsStoreLocked(id, locked);
	if (error==KErrNone && locked)
		error = KErrAccessDenied;
	if(error==KErrNone)
		{
		iMsvServer.IndexAdapter().IncStoreReaderCount(id);
		}

	aMessage.Complete(error);
	}


void CMsvServerSession::LockStoreL(const RMessage2 &aMessage)
//
// Tries to locks the store
// Adds the id to the list of locked entries
//
	{
	// Fail now if the index says it's not available
	User::LeaveIfError(iMsvServer.IndexAdapter().ErrorState());
	TMsvId id = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, id, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::LockStoreL"));

	iLockedStores.AppendL(id); // ensure that the entry cannot be locked but not added to the list
	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().LockStore(id);
	if (error)
		iLockedStores.Delete(iLockedStores.Count()-1);

	aMessage.Complete(error);
	}


void CMsvServerSession::ReleaseStoreL(const RMessage2 &aMessage)
//
// Releases the lock on a store
// Removes the id from the list of locked entries
//
	{
	TMsvId id = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, id, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ReleaseStoreL"));
	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().ReleaseStore(id);
	TInt count = iLockedStores.Count();
	while (count--)
		{
		if (iLockedStores.At(count)==id)
			{
			iLockedStores.Delete(count);
			break;
			}
		}

	__ASSERT_DEBUG(count>=0, PanicClient(aMessage, EMsvReleasingUnknownStore));

	aMessage.Complete(error);
	}


void CMsvServerSession::DecStoreReaderCountL(const RMessage2 &aMessage)
//
// Decrements the reader count on a store
//
	{
	TMsvId id = aMessage.Int0();

	// Police request - client must be able to read the entry.
	iMsvServer.PoliceReadEntryL(aMessage, id, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DecStoreReaderCountL"));

	TInt error = KErrNone;
	error = iMsvServer.IndexAdapter().DecStoreReaderCount(id);
	aMessage.Complete(error);
	}

void CMsvServerSession::NotifyChangedL(TMsvNotifBuffer& aChange, TBool aQueue)
//
// This session is being informed that a change has occured
// Will only leave if a write to a client fails
//
	{
	// queue the change in FIFO buffer
	iChangeBuffer->InL(aChange);

#ifndef _NO_SERVER_LOGGING_
	iMsvServer.Log(_L("Queued notification sequence %d"), iChangeBuffer->NotifySequence());
#endif

	// Only actually send the notification when we have a context
	if (iRequestQueued && !aQueue &&  iQueuedMessage.Handle())
		{
		// inform client of change
		SendNotificationL(iQueuedMessage);
		iRequestQueued = EFalse;
		}
	}


TBool CMsvServerSession::IsInUse(TUid aMtmTypeUid)
	{
	TBool isinuse=EFalse;
	TInt index=0;
	TInt count=iMtmGroupRefCountArray.Count();
	for (; (index<count) && !isinuse; index++)
		if (iMtmGroupRefCountArray[index].iMtmTypeUid==aMtmTypeUid)
			isinuse=ETrue;
	return isinuse;
	}

void CMsvServerSession::CancelOperationL(const RMessage2 &aMessage)
//
// Cancels an operation
//
	{
	TMsvOp op = aMessage.Int0();

	TInt error=KErrNotFound;
	for (TInt count=0; count<iOperations.Count(); count++)
		{
		if (iOperations.At(count)->Id()==op)
			{
			error=KErrNone;

			iOperations.At(count)->Cancel();

			if (iOperations.At(count)->State()!=EMsvOperationQueued)
				{
				TPtrC8 desPtr = iOperations.At(count)->Progress();
				if (desPtr.Length() > KMsvProgressBufferLength)
					{
					PanicClient(aMessage, EMsvProgressBufferExceeds256);
					error = KErrOverflow;
					}
				else
					{
					WriteL(aMessage, 1, desPtr);
					error = desPtr.Length();
					}
				}
			else
				{
				error=KErrNotReady;
				}

			delete iOperations.At(count);
			iOperations.Delete(count);
			break;
			}
		}

	aMessage.Complete(error);
	}


void CMsvServerSession::OperationCompletionL(const RMessage2 &aMessage)
//
// Gets the final progress of an operation and deletes all objects
//
	{
	TMsvOp op = aMessage.Int0();

	TInt error=KErrNotFound;
	for (TInt count=0; count<iOperations.Count(); count++)
		{
		if (iOperations.At(count)->Id()==op)
			{
			__ASSERT_DEBUG(iOperations.At(count)->State() == EMsvOperationCompleted || iOperations.At(count)->State() == EMsvOperationFailed, PanicServer(EMsvOperationNotCompleted));
			if (iOperations.At(count)->State() == EMsvOperationFailed)
				{
				// there will be no progress available
				error=KErrNone;
				}
			else
				{
				TPtrC8 desPtr = iOperations.At(count)->Progress();
				if (desPtr.Length() > KMsvProgressBufferLength)
					{
					PanicClient(aMessage, EMsvProgressBufferExceeds256);
					error = KErrOverflow;
					}
				else
					{
					WriteL(aMessage, 1, desPtr);
					error = desPtr.Length();
					}
				}
			delete iOperations.At(count);
			iOperations.Delete(count);
			break;
			}
		}

	aMessage.Complete(error);
	}


void CMsvServerSession::OperationProgressL(const RMessage2 &aMessage)
//
// Gets the progress of an operation
//
	{
	TMsvOp op = aMessage.Int0();

	TInt error=KErrNotFound;
	for (TInt count=0; count<iOperations.Count(); count++)
		{
		if (iOperations.At(count)->Id()==op)
			{
			if (iOperations.At(count)->State() == EMsvOperationQueued)
				{
				// the operation has not started
				error = KErrNotReady;
				}
			else
				{
				// get the progress from the running operation
				TPtrC8 desPtr = iOperations.At(count)->Progress();
				if (desPtr.Length() > KMsvProgressBufferLength)
					{
					PanicClient(aMessage, EMsvProgressBufferExceeds256);
					error = KErrOverflow;
					}
				else
					{
					WriteL(aMessage, 1, desPtr);
					error = desPtr.Length();
					}
				break;
				}
			}
		}
	aMessage.Complete(error);
	}

void CMsvServerSession::OperationSystemProgressL(const RMessage2 &aMessage)
//
// Get the progress of a system operation
//
	{
	TMsvOp op = aMessage.Int0();
	TInt error=KErrNotFound;
	for (TInt count=0; count<iOperations.Count(); count++)
		{
		if (iOperations.At(count)->Id()==op)
			{
			if (iOperations.At(count)->State() == EMsvOperationQueued)
				{
				// the operation has not started
				error = KErrNotReady;
				}
			else
				{
				// get the progress from the running operation
				TPckgBuf<TMsvSystemProgress> systemProgress;
				error = iOperations.At(count)->SystemProgress(systemProgress());
				WriteL(aMessage, 1, systemProgress);
				break;
				}
			}
		}
	aMessage.Complete(error);
	}

void CMsvServerSession::OperationMtmL(const RMessage2 &aMessage)
//
// Returns the MTM which will perform the operation using the two entries
//
	{
	// read the two ids
	TMsvId id1 = aMessage.Int0();
	TMsvId id2 = aMessage.Int1();

	// the uid to be writen back
	TPckgBuf<TUid> uid;
	TPckgBuf<TMsvId> service;

	// get the first entries
	TBool local1;
	TMsvEntry* entry1=NULL;
	User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(id1, entry1));
	User::LeaveIfError(iMsvServer.IndexAdapter().IsLocal(id1, local1));
	if (id1==id2)
		{
		// single entry either under local or a remote service
		if (local1)
			{
			uid() = KUidMsvLocalServiceMtm;
			service() = KMsvLocalServiceIndexEntryId;
			}
		else
			{
			uid() = entry1->iMtm;
			service() = entry1->iServiceId;
			}
		}
	else
		{
		// get the second entry
		TBool local2;
		TMsvEntry* entry2=NULL;
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(id2, entry2));
		User::LeaveIfError(iMsvServer.IndexAdapter().IsLocal(id2, local2));
		// work out the responsible MTM
		if (local1 && local2)
			{
			// both entries under the local service
			uid() = KUidMsvLocalServiceMtm;
			service() = KMsvLocalServiceIndexEntryId;
			}
		else if (entry1->iMtm == entry2->iMtm)
			{
			__ASSERT_DEBUG(entry1->iMtm!=KUidMsvLocalServiceMtm, PanicServer(EMsvOperationMtm3));
			// both entries use the same MTM
			uid() = entry1->iMtm;
			service() = entry1->iServiceId;
			}
		else if (entry1->iMtm == KUidMsvLocalServiceMtm)
			{
			__ASSERT_DEBUG(entry2->iMtm!=KUidMsvLocalServiceMtm, PanicServer(EMsvOperationMtm1));
			// entry1 is a local entry - hence entry 2 must be MTM dependent
			uid() = entry2->iMtm;
			service() = entry2->iServiceId;
			}
		else
			{
#ifdef _DEBUG
			if (entry2->iMtm != KUidMsvLocalServiceMtm)
				{
				// Neither entry belongs to local mtm, check they have the same technology type
				__ASSERT_DEBUG(iMsvServer.Registry().TechnologyTypeUid(entry1->iMtm) == iMsvServer.Registry().TechnologyTypeUid(entry2->iMtm), PanicServer(EMsvOperationMtm2));
				}
#endif

			// entry1 is not a local entry - assume entry 2 is local
			uid() = entry1->iMtm;
			service() = entry1->iServiceId;
			}
		}

	WriteL(aMessage, 2, uid);
	WriteL(aMessage, 3, service);
	aMessage.Complete(KErrNone);
	}



void CMsvServerSession::MoveEntriesL(const RMessage2 &aMessage)
//
// Moving entries
//
	{
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	CleanupStack::PushL(opData);
	// unpack the data
	TMsvPackedOperation packedOperation(opData);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt target, temp;
	packedOperation.UnpackL(*selection, target, temp);

	TInt count = selection->Count();
	if ( count == 0)
		{
		PanicClient(aMessage, EMsvNoEntriesInMoveSelection);
		aMessage.Complete(KErrNotFound);
		return;
		}

	// Find the first entry in the selection which exists
	TInt pos=-1;
	TMsvId source = KMsvNullIndexEntryIdValue;
	while (++pos<count)
		{
		TMsvEntry* entryPtr;
		TInt error = KErrNone;
		error = iMsvServer.IndexAdapter().GetEntry(selection->At(pos), entryPtr);
		if( error == KErrNone )
			{
			source = entryPtr->Parent();
			break;
			}
		}
	if (pos==count)
		pos = 0;

	// Check if the target and source are local or remote
	TBool targetLocal=EFalse;
	TBool sourceLocal=EFalse;
	TBool hasCapability=EFalse;
	// Police request - client must be able to move the entries.
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	TRAPD(err, iMsvServer.PoliceMoveEntriesL(aMessage, *selection, target, source, targetLocal, sourceLocal, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::MoveEntriesL")));
	if(err == KErrNone)
		{
		hasCapability = ETrue;
		}
	else if((err != KErrNone) && (targetLocal && sourceLocal))
		{
		User::LeaveIfError(err);
		}
#else
	iMsvServer.PoliceMoveEntriesL(aMessage, *selection, target, source, targetLocal, sourceLocal, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::MoveEntriesL"));
#endif
	
	// start the move for local or remote messages
	if (targetLocal && sourceLocal)
		DoMoveLocalEntriesL(selection, target, operationId, aMessage);
	else
		DoMoveRemoteEntriesL(selection, target, operationId, aMessage, sourceLocal, targetLocal, hasCapability);

	CleanupStack::PopAndDestroy(); // opData
	}

void CMsvServerSession::DoMoveLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage)
//
// NOTE aSelection is the top item on the cleanupstack when the function is called
//
	{
	CMsvLocalMoveOperation* operation = new(ELeave) CMsvLocalMoveOperation(aMessage, aOpId, aSelection, aTarget, iMsvServer);
	CleanupStack::Pop(); // selection
	CleanupStack::PushL(operation);
	operation->StartL();
	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::DoMoveRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage, TBool aSourceLocal, TBool aTargetLocal, TBool aHasCapability)
//
// NOTE aSelection is the top item on the cleanupstack when the function is called
//
	{
	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	TMsvEntry* entry=NULL;
	if (!aTargetLocal)
		{
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aTarget, entry));
		}
	else
		{
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aSelection->At(0), entry));
		}


	// create the operation
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, entry->iMtm, entry->iServiceId, iSessionId, iMsvServer);
	CleanupStack::Pop();
	CleanupStack::PushL(operation);

	// set up the operation type and pass the data
	if (!(aSourceLocal || aTargetLocal))
		operation->MoveWithinService(aSelection, aTarget);
	else if (aSourceLocal)
		operation->MoveFromLocal(aSelection, aTarget);
	else
		operation->MoveToLocal(aSelection, aTarget);

	// either start the operation or queue it
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, aHasCapability);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	}


void CMsvServerSession::CopyEntriesL(const RMessage2 &aMessage)
//
// Moving entries
//
	{
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	CleanupStack::PushL(opData);
	// unpack the data
	TMsvPackedOperation packedOperation(opData);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt target,temp;
	packedOperation.UnpackL(*selection, target, temp);

	if (selection->Count() == 0)
		{
		PanicClient(aMessage, EMsvNoEntriesInCopySelection);
		aMessage.Complete(KErrNotFound);
		return;
		}

	// Find the first entry in the selection which exists
	TInt pos=-1;
	TInt count=selection->Count();
	while (++pos<count)
		{
		TBool entryExsists = EFalse;
		entryExsists = iMsvServer.IndexAdapter().EntryExists(selection->At(pos));
		if (entryExsists)
			break;
		}
	if (pos==count)
		pos = 0;

	// Check if the target and source are local or remote
	TBool targetLocal=EFalse;
	TBool sourceLocal=EFalse;
	TBool hasCapability = EFalse;
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	// Police request - client must be able to copy the entries.
	TRAPD(err, iMsvServer.PoliceCopyEntriesL(aMessage, *selection, target, targetLocal, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CopyEntriesL")));
	
	if(err == KErrNone)
		{
		hasCapability = ETrue;
		}
#else
	iMsvServer.PoliceCopyEntriesL(aMessage, *selection, target, targetLocal, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CopyEntriesL"));
#endif
	TInt error = KErrNone;
	// what happens about the rest? Inconsistent behaviour
	error = iMsvServer.IndexAdapter().IsLocal(selection->At(pos), sourceLocal);
	
#if (defined SYMBIAN_USER_PROMPT_SERVICE)   
	if((err != KErrNone) && (targetLocal && sourceLocal))
		{
		User::LeaveIfError(err);
		}
#endif
		
	if (error)
		{
		CleanupStack::PopAndDestroy(2); // selection & opData
		aMessage.Complete(error);
		return;
		}


	// start the move for local or remote messages
	if (targetLocal && sourceLocal)
		DoCopyLocalEntriesL(selection, target, operationId, aMessage);
	else
		DoCopyRemoteEntriesL(selection, target, operationId, aMessage, sourceLocal, targetLocal, hasCapability);

	CleanupStack::PopAndDestroy(); // opData
	}

void CMsvServerSession::DoCopyLocalEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage)
//
// NOTE aSelection is the top item on the cleanupstack when the function is called
//
	{
	CMsvLocalCopyOperation* operation = new(ELeave) CMsvLocalCopyOperation(aMessage, aOpId, aSelection, aTarget, iMsvServer);
	CleanupStack::Pop(); // selection
	CleanupStack::PushL(operation);

	operation->StartL();

	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}

void CMsvServerSession::DoCopyRemoteEntriesL(CMsvEntrySelection*& aSelection, TMsvId aTarget, TMsvOp aOpId, const RMessage2 &aMessage, TBool aSourceLocal, TBool aTargetLocal, TBool aHasCapability)
//
// NOTE aSelection is the top item on the cleanupstack when the function is called
//
	{
	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	TMsvEntry* entry=NULL;
	if (!aTargetLocal)
		{
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aTarget, entry));
		}

	else
		{
		User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(aSelection->At(0), entry));
		}

	// create the operation
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, aOpId, entry->iMtm, entry->iServiceId, iSessionId, iMsvServer);
	CleanupStack::Pop(); // aSelection
	CleanupStack::PushL(operation);

	// set up the operation type and pass the data
	if (!(aSourceLocal || aTargetLocal))
		operation->CopyWithinService(aSelection, aTarget);
	else if (aSourceLocal)
		operation->CopyFromLocal(aSelection, aTarget);
	else
		operation->CopyToLocal(aSelection, aTarget);

	// either start the operation or queue it
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, aHasCapability);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	}



void CMsvServerSession::TransferCommandL(const RMessage2 &aMessage)
//
//
//
	{
	// Recover the operation data and parameter
	TMsvOp operationId = aMessage.Int0();
	HBufC8* opData = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opData == NULL)
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	HBufC8* opParam = RecoverOperationData(operationId);
	// Check for NULL data entry to be changed, must have been given incorrect id for argument.
	if(opParam == NULL)
		{
		delete opData;
		aMessage.Complete(KErrArgument);
		return;
		}
	
	CleanupStack::PushL(opData);
	CleanupStack::PushL(opParam);

	// Unpack the data
	TMsvPackedOperation packedOperation(opData);
	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);
	TInt command, temp;
	packedOperation.UnpackL(*selection, command, temp);

	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// find the first entry being acted on
	TMsvEntry* entry=NULL;
	User::LeaveIfError(iMsvServer.IndexAdapter().GetEntry(selection->At(0), entry));
	TBool hasCapability = EFalse;
	// Police request - client must be able to access the MTM functionality.
#if (defined SYMBIAN_USER_PROMPT_SERVICE)
	TRAPD(err, iMsvServer.PoliceMtmTransferCommandL(aMessage, entry->iMtm, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::TransferCommandL")));
	
	if(err == KErrNone)
		{
		hasCapability = ETrue;
		}
	else if(err != KErrPermissionDenied)
		{
		User::LeaveIfError(err);
		}
#else
	iMsvServer.PoliceMtmTransferCommandL(aMessage, entry->iMtm, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::TransferCommandL"));
#endif

	// create the operation
	CMsvMtmOperation* operation = CMsvMtmOperation::NewL(aMessage, operationId, entry->iMtm, entry->iServiceId, iSessionId, iMsvServer);
	CleanupStack::Pop(); // selection
	CleanupStack::Pop(); // opParam
	CleanupStack::PushL(operation);

	// set up the operation type and pass the data
	operation->StartCommand(selection, command, opParam);

	// either start the operation or queue it
	iMsvServer.StartOperationL(*operation, iSessionId, aMessage, hasCapability);
	iOperations.AppendL(operation); // will not fail - see start of function
	CleanupStack::Pop(); // operation
	CleanupStack::PopAndDestroy(); // opParam
	}


void CMsvServerSession::FillRegisteredMtmDllArray(const RMessage2 &aMessage)
	{
	TRAPD(error,DoFillRegisteredMtmDllArrayL(aMessage));
	aMessage.Complete(error);
	}

void CMsvServerSession::DoFillRegisteredMtmDllArrayL(const RMessage2 &aMessage)
	{
	CRegisteredMtmDllArray* registeredMtmDllArray = new(ELeave) CRegisteredMtmDllArray;
	CleanupStack::PushL(registeredMtmDllArray);

	TUid mtmdlltypeuid = {aMessage.Int0()};
	TInt error=iMsvServer.FillRegisteredMtmDllArray(mtmdlltypeuid,*registeredMtmDllArray);
	User::LeaveIfError(error);

	TPtr8 bufferPtr = iBuffer->Des();
	bufferPtr.Zero();
	RDesWriteStream writeStream(bufferPtr);
	TMsvPackedRegisteredMtmDllArray::PackRegisteredMtmDllArrayL(writeStream, *registeredMtmDllArray);
	writeStream.CommitL();

	WriteBufferL(aMessage, 1);
	CleanupStack::PopAndDestroy(); // registeredMtmDllArray
	}


void CMsvServerSession::MtmGroupL(const RMessage2 &aMessage, TBool aInstall)
	{
	TFullName fullname;
	TInt  desLen = aMessage.GetDesLengthL(0);
	fullname.SetLength(desLen);
	TInt error = aMessage.Read(0,fullname);
	if (error)
		{
		PanicClient(aMessage, EMsvBadDescriptorRead);
		User::Leave(error);
		}
	TUid mtmtypeuid=KNullUid;
	if (aInstall)
		{
		User::LeaveIfError(iMsvServer.InstallMtmGroup(fullname,mtmtypeuid));
		iMsvServer.NotifyChanged(EMsvMtmGroupInstalled,mtmtypeuid);
		}
	else
		{
		User::LeaveIfError(iMsvServer.DeInstallMtmGroup(fullname,mtmtypeuid));
		iMsvServer.NotifyChanged(EMsvMtmGroupDeInstalled,mtmtypeuid);
		}
	aMessage.Complete(KErrNone);
	}


void CMsvServerSession::UseMtmGroup(const RMessage2 &aMessage)
	{
	TUid mtmtypeuid = {aMessage.Int0()};
	TInt index=0;
	TInt count=iMtmGroupRefCountArray.Count();
	for (; (index<count) && (mtmtypeuid!=iMtmGroupRefCountArray[index].iMtmTypeUid); index++)
		{
		}
	TInt error=KErrNone;
	if (index==count)
		{
		TMtmGroupRefCount mtmgrouprefcount(mtmtypeuid);
		TRAP(error,iMtmGroupRefCountArray.AppendL(mtmgrouprefcount));
		}
	if (error==KErrNone)
		iMtmGroupRefCountArray[index].iRefCount++;
	aMessage.Complete(error);
	}

void CMsvServerSession::ReleaseMtmGroup(const RMessage2 &aMessage)
	{
	TUid mtmtypeuid = {aMessage.Int0()};
	TInt index=0;
	TInt count=iMtmGroupRefCountArray.Count();
	for (; (index<count) && (mtmtypeuid!=iMtmGroupRefCountArray[index].iMtmTypeUid); index++)
		{
		}
	TInt error=KErrNone;
	if (index==count)
		error=KErrNotFound;
	else
		{
		iMtmGroupRefCountArray[index].iRefCount--;
		if (iMtmGroupRefCountArray[index].iRefCount==0)
			iMtmGroupRefCountArray.Delete(index);
		}
	aMessage.Complete(error);
	}

void CMsvServerSession::GetMtmGroupDataL(const RMessage2 &aMessage)
	{
	TUid mtmtypeuid = {aMessage.Int0()};
	CMtmGroupData* mtmgroupdata=iMsvServer.GetMtmGroupDataL(mtmtypeuid);
	CleanupStack::PushL(mtmgroupdata);
	TPtr8 bufferPtr = iBuffer->Des();
	bufferPtr.Zero();
	RDesWriteStream writeStream(bufferPtr);
	writeStream << *mtmgroupdata;
	writeStream.CommitL();

	WriteBufferL(aMessage, 1);
	CleanupStack::PopAndDestroy();
	aMessage.Complete(KErrNone);
	}


void CMsvServerSession::GetMtmRequiredCapabilitiesL(const RMessage2& aMessage)
	{
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	TCapabilitySet mtmRequiredCaps;
	iMsvServer.GetMtmRequiredCapabilitiesL(mtmTypeUid,mtmRequiredCaps);
	TPtr8 ptr = iBuffer->Des();
	ptr.Zero();
	RDesWriteStream writeStream(ptr);
	MsvSecurityCapabilitySetUtils::ExternalizeL(writeStream,mtmRequiredCaps);
	writeStream.CommitL();
	WriteBufferL(aMessage, 1);
	aMessage.Complete(KErrNone);
	}


void CMsvServerSession::PanicClient(const RMessage2& aMessage, TMsvClientPanic aPanic) const
//
// Panic the client
//
	{
	aMessage.Panic(KMsvClientPanicString, aPanic);
	}


void CMsvServerSession::StopOperations(TMsvId aServiceId)
//
// Cancels all operations which use the service (the service is being stopped)
//
	{
	TInt count=iOperations.Count();
	while (count--)
		{
		if (iOperations.At(count)->ServiceId() == aServiceId)
			{
			delete iOperations.At(count);
			iOperations.Delete(count);
			}
		}
	}


void CMsvServerSession::ServiceProgressL(const RMessage2 &aMessage)
//
// Returns the progress from the server MTM
//
	{
	TPtrC8 desPtr = iMsvServer.ServiceProgressL(aMessage.Int0());
	TInt desLength = aMessage.GetDesMaxLength(1);
	if (desLength < desPtr.Length())
		aMessage.Complete(KErrOverflow);
	else
		{
		WriteL(aMessage, 1, desPtr);
		aMessage.Complete(desPtr.Length());
		}
	}

void CMsvServerSession::RemoveEntry(const RMessage2& aMessage)
//
// Removes the entry from the index. Client not interested in result.
//
	{
	TMsvId id = aMessage.Int0();

	TBool local;
	// Police request - client must be able to modify the entry.
	TRAPD(error, iMsvServer.PoliceModifyEntryL(aMessage, id, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::RemoveEntry")));

	if( error == KErrPermissionDenied )
		{
		// The request was denied - complete with this error code.
		aMessage.Complete(KErrPermissionDenied);
		return;
		}
	else if( error == KErrNone )
		{
		if (local)
			iMsvServer.RemoveEntry(id);
		else
			{
			TMsvId service;
			iMsvServer.IndexAdapter().OwningService(id, service); // error ignored
			if (service==id)
				iMsvServer.RemoveEntry(id);
			else
				PanicClient(aMessage, EMsvRemovingNonLocalEntry);
			}
		}

	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::MessageFolderL(const RMessage2 &aMessage)
//
//
//
	{
	aMessage.WriteL(0, iMsvServer.Context().MessageFolder());
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::ChangeAttributesL(const RMessage2& aMessage)
//
//
//
	{
	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 0, buffer);
	CleanupStack::PushL(buffer);

	TMsvPackedOperation packedOperation(buffer);

	CMsvEntrySelection* selection = new (ELeave) CMsvEntrySelection;
	CleanupStack::PushL(selection);

	TInt setAttributes, clearAttributes;
	packedOperation.UnpackL(*selection, setAttributes, clearAttributes);

	if (selection->Count() == 0)
		{
		PanicClient(aMessage, EMsvNoEntriesInChangeAttributesSelection);
		aMessage.Complete(KErrNotFound);
		return;
		}

	// get a copy of the selection
	CMsvEntrySelection* changedEntries = selection->CopyL();
	CleanupStack::PushL(changedEntries);

	// create a selection for the later notification
	CMsvEntrySelection* notify = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(notify);
	notify->SetReserveL(selection->Count());

	TInt error = KErrNone;

	// lock all the selection
	TInt count1 = selection->Count();
	while (count1--)
		{
		error = iMsvServer.IndexAdapter().LockEntry(selection->At(count1));
		if (error)
			break;
		}

	// change the attributes if all were locked
	if (error == KErrNone)
		{
		error = iMsvServer.IndexAdapter().ChangeAttributes(*changedEntries, setAttributes, clearAttributes);
		}


	// release all that were locked
	TInt count2 = selection->Count();
	while (--count2 > count1)
		{
		iMsvServer.IndexAdapter().ReleaseEntry(selection->At(count2));
		}


	if (error == KErrNone)
		{
		// Note: Code in the following loop shouldn't leave
		while(changedEntries->Count())
			{
			TMsvId id = changedEntries->At(0);
			changedEntries->Delete(0);

			// Get the index entry associated with this entry - Shouldn't fail
		
			TInt err = KErrNone;
			CMsvCacheEntry* entry = NULL;
			err = iMsvServer.IndexAdapter().GetInternalEntry(id, entry);
			if (err != KErrNone)
				continue;

			// Get the parent of this entry. It will be notified that its children have changed
			TMsvId parent = entry->Entry().Parent();
			notify->AppendL(id);

			// Find other entries that have the same parent
			TInt count = changedEntries->Count();
			while(count--)
				{
				iMsvServer.IndexAdapter().GetInternalEntry(changedEntries->At(count), entry);
				if (entry && entry->Entry().Parent() == parent)
					{
					// Add entry to list - Shouldn't fail as we've already reserved space
					notify->AppendL(changedEntries->At(count));
					changedEntries->Delete(count);
					}
				}

			// Do the notification
			iMsvServer.NotifyChanged(EMsvEntriesChanged, *notify, parent);
			notify->Reset();
			}
		}

	aMessage.Complete(error);
	CleanupStack::PopAndDestroy(4); // notify, changedEntries, selection, buffer
	}

void CMsvServerSession::GetChildIdsL(const RMessage2& aMessage)
	{
	TMsvId id = aMessage.Int1();

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	// Return error, if the passed parent-id 
	// is a non-current standard id.
	if( (IsStandardId(id)) &&
		(KCurrentDriveId != GetDriveId(id))
	  )
		{
		aMessage.Complete(KErrArgument);		
		return;
		}
#endif
	
	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);

	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 0, buffer);
	CleanupStack::PushL(buffer);

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	TMsvPackedEntryFilter package(buffer);
	package.UnpackFilter(*filter);

	// Need to filter the list via the client secure ID if the client is not
	// trusted with Read User Data.
	TBool filterByOwnerId = !aMessage.HasCapability(ECapabilityReadUserData);

	iMsvServer.IndexAdapter().GetChildrenId(id, *filter, *selection, filterByOwnerId, aMessage.SecureId());
	TMsvPackedOperation op(iBuffer);

	TInt error = op.Pack(*selection, 0, 0);
	while (error != KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = op.Pack(*selection, 0, 0);
		}

	WriteBufferL(aMessage, 0);

	aMessage.Complete(KErrNone);
	CleanupStack::PopAndDestroy(3); // selection, buffer, filter
	}



void CMsvServerSession::ChangeDriveL(const RMessage2 &aMessage)
//
// Change the drive containing the Message Store
//
	{
	// Not allowed to do this if operations are outstanding
	if (iMsvServer.OutstandingOperations() > 0)
		User::Leave(KErrServerBusy);

	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// Start the operation
	CMsvChangeDriveOperation* operation = new(ELeave)CMsvChangeDriveOperation(aMessage, aMessage.Int0(), aMessage.Int1(), iMsvServer);
	CleanupStack::PushL(operation);
	User::LeaveIfError(operation->Start());

	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}




void CMsvServerSession::CopyStoreL(const RMessage2 &aMessage)
//
// Copy the Message Store to another location
//
	{
	// Not allowed to do this if operations are outstanding
	if (iMsvServer.OutstandingOperations() > 0)
		User::Leave(KErrServerBusy);

	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// Start the operation
	CMsvCopyStoreOperation* operation = CMsvCopyStoreOperation::NewL(aMessage, iMsvServer);
	CleanupStack::PushL(operation);
	operation->StartL();

	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}

void CMsvServerSession::DeleteStoreL(const RMessage2 &aMessage)
//
// Delete the Message Store from the specified location
//
	{
	// Not allowed to do this if operations are outstanding
	if (iMsvServer.OutstandingOperations() > 0)
		User::Leave(KErrServerBusy);

	// make sure that the operation can be added to the list
	iOperations.SetReserveL(iOperations.Count()+1);

	// Start the operation
	CMsvDeleteStoreOperation* operation = new(ELeave)CMsvDeleteStoreOperation(aMessage, iMsvServer);
	CleanupStack::PushL(operation);
	operation->StartL();

	iOperations.AppendL(operation);
	CleanupStack::Pop(); // operation
	}



void CMsvServerSession::SetReceiveEntryEvents(const RMessage2 &aMessage)
//
// Request/UnRequest Entry Events
//
	{
	iReceiveEntryEvents=aMessage.Int0();
	aMessage.Complete(KErrNone);
	}


void CMsvServerSession::CreateAttachmentForWriteL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	TBool local;
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CreateAttachmentForWriteL"));

	if( local )
		{
		// Update the owner ID of the entry. NOTE - doesn't matter if actually
		// attachment creation fails after this - safer that owner ID is updated
		// at very least.
		UpdateOwnerIdL(msgId, aMessage.SecureId());
		}

	// Get the file name from slot 1
	TFileName fileName;
	aMessage.ReadL(1, fileName);

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	TBool fileNameChanged = storemanager.CreateAttachmentForWriteL(msgId, fileName, file);
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 2
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(2,pckgSubSessionHandle);

	// Write the file changed status in slot 3
	TPckg<TBool> pckgFileNameChanged(fileNameChanged);
	aMessage.WriteL(3,pckgFileNameChanged);

	// Write back the file path to the client.
	TInt desLen = aMessage.GetDesMaxLength(1);
	if( desLen < fileName.Length() )
		User::Leave(KErrOverflow);
	aMessage.WriteL(1, fileName);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,2));

	CleanupStack::PopAndDestroy(2, &fs); // fs , file
	}

void CMsvServerSession::ReplaceAttachmentForWriteL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ReplaceAttachmentForWriteL"));

	// Get the file name from slot 1
	TFileName fileName;
	aMessage.ReadL(1, fileName);

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	storemanager.ReplaceAttachmentForWriteL(msgId, fileName, file);
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 2
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(2,pckgSubSessionHandle);

	// Write back the file path to the client.
	TInt desLen = aMessage.GetDesMaxLength(1);
	if( desLen < fileName.Length() )
		User::Leave(KErrOverflow);
	aMessage.WriteL(1, fileName);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,2));

	CleanupStack::PopAndDestroy(2, &fs); // fs , file
	}

void CMsvServerSession::OpenAttachmentL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to read the entry.
	iMsvServer.PoliceReadEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::OpenAttachmentL"));

	// Get the file name from slot 1
	TFileName filePath;
	aMessage.ReadL(1, filePath);

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	storemanager.OpenAttachmentL(msgId, filePath, file);
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 2
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(2,pckgSubSessionHandle);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,2));

	CleanupStack::PopAndDestroy(2,&fs); // fs, file
	}

void CMsvServerSession::OpenAttachmentForWriteL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to read the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::OpenAttachmentForWriteL"));

	// Get the file name from slot 1
	TFileName filePath;
	aMessage.ReadL(1, filePath);

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	storemanager.OpenAttachmentForWriteL(msgId, filePath, file);
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 2
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(2,pckgSubSessionHandle);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,2));

	CleanupStack::PopAndDestroy(2,&fs); // fs, file
	}

void CMsvServerSession::DeleteAttachmentL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DeleteAttachmentL"));

	// Get the file name from slot 1
	TFileName filePath;
	aMessage.ReadL(1, filePath);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());

	TInt error = storemanager.DeleteAttachment(msgId, filePath);
	aMessage.Complete(error);
	}

void CMsvServerSession::RenameAttachmentL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::RenameAttachmentL"));

	// Get the old file name from slot 1
	TFileName oldFileName;
	aMessage.ReadL(1, oldFileName);

	// Get the new file name from slot 2
	TFileName newFileName;
	aMessage.ReadL(2, newFileName);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());

	TInt error = storemanager.RenameAttachment(msgId, oldFileName, newFileName);
	aMessage.Complete(error);
	}

void CMsvServerSession::FileExistsL(const RMessage2 &aMessage)
	{
	// Get the file name from slot 10
	TFileName filePath;
	aMessage.ReadL(0, filePath);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer, iMsvServer.FileSession());

	TInt err = KErrNotFound;
	if( storemanager.FileExistsL(filePath) )
		{
		err = KErrNone;
		}

	aMessage.Complete(err);
	}

void CMsvServerSession::GetAttachmentFilePathL(const RMessage2& aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Get the file path buffer from slot 1
	TFileName filePath;
	aMessage.ReadL(1, filePath);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storeManager;
	storeManager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());
	storeManager.AttachmentFilePathL(msgId, filePath);

	// Write the attachment file path back to slot 1
	TInt desLen = aMessage.GetDesMaxLength(1);
	if( desLen < filePath.Length() )
		{
		User::Leave(KErrOverflow);
		}

	aMessage.WriteL(1, filePath);
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::OpenFileStoreForReadL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to read the entry.
	iMsvServer.PoliceReadEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::OpenFileStoreForReadL"));

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	User::LeaveIfError( storemanager.OpenFileStoreForRead(msgId,file));
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 1
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(1,pckgSubSessionHandle);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,1));

	CleanupStack::PopAndDestroy(2,&fs); // fs, file
	}

void CMsvServerSession::OpenTempStoreFileL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::OpenTempStoreFileL"));

	RFs fs;
	ConnectAndShareFileSessionLC(fs);

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,fs);

	RFile file;
	storemanager.OpenTempStoreFileL(msgId,file);
	CleanupClosePushL(file);

	// Write the sub session handle (RFile) in slot 1
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle());
	aMessage.WriteL(1,pckgSubSessionHandle);

	// Takes care of completing with session (RFs),
	// SendReceieve returns session handle at client side
	User::LeaveIfError(file.TransferToClient(aMessage,1));

	CleanupStack::PopAndDestroy(2,&fs); // fs, file
	}

void CMsvServerSession::ReplaceFileStoreL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	TBool local;
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ReplaceFileStoreL"));

	if( local )
		{
		// Update the owner ID of the entry. NOTE - doesn't matter if actually
		// attachment creation fails after this - safer that owner ID is updated
		// at very least.
		UpdateOwnerIdL(msgId, aMessage.SecureId());
		}

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());

	storemanager.ReplaceFileStoreL(msgId);
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::DeleteFileStoreL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, msgId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DeleteFileStoreL"));

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());

	storemanager.DeleteFileStoreL(msgId);
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::FileStoreExistsL(const RMessage2 &aMessage) const
	{
	// Get the Message Id from slot 0
	TMsvId msgId = aMessage.Int0();

	// Set the CMsvServer and file session
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession());

	TInt storeExists = storemanager.FileStoreExistsL(msgId);
	aMessage.Complete(storeExists);
	}

void CMsvServerSession::ConnectAndShareFileSessionLC(RFs& aFs )
	{
	User::LeaveIfError(aFs.Connect());
	CleanupClosePushL(aFs);
	User::LeaveIfError(aFs.ShareProtected());
	}

void CMsvServerSession::UpdateOwnerIdL(TMsvId aId, TSecureId aOwnerId)
	{
	User::LeaveIfError(iMsvServer.IndexAdapter().LockEntry(aId));
	TMsvEntry* entry;
	iMsvServer.IndexAdapter().GetEntry(aId, entry); // error ignored
	User::LeaveIfError(iMsvServer.IndexAdapter().ChangeEntry(*entry, aOwnerId, ETrue));
	iMsvServer.IndexAdapter().ReleaseEntry(aId);		// error ignored
	}


/**
Gets the path for the file in message store for which the message Id is passed in RMessage2.
@param aMessage RMessage2 encapsulating client request and data.
@return none.
*/
void CMsvServerSession::BodyTextFilePathL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId bodyTextId = aMessage.Int0();
	
	// Get the file name from slot 1
	TFileName filePath;
	aMessage.ReadL(1, filePath);
		
	// Set the CMsvServer and file session 
	TMsvServerStoreManager storemanager; 
	storemanager.SetMsvServerAndFileSession(iMsvServer, iMsvServer.FileSession());
	
	storemanager.BodyTextFilePathL(bodyTextId, filePath);
	aMessage.WriteL(1, filePath);   
	aMessage.Complete(KErrNone);
	}


/**
Open the RFile located in filepath encapsulted in RMessage2 in Read mode.
@param aMessage RMessage2 encapsulating client request and data.
@return none.
*/
void CMsvServerSession::OpenPlainTextFileL(const RMessage2 &aMessage)
	{
	TMsvId bodyTextId = aMessage.Int0();
	
	TFileName filePath;
	aMessage.ReadL(1, filePath);
	iMsvServer.PoliceReadEntryL(aMessage, bodyTextId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::OpenPlainTextFileL"));
	
	RFs fs;
	ConnectAndShareFileSessionLC(fs);
	RFile file;
	CleanupClosePushL(file);
		
	// Set the CMsvServer and file session 
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer, fs); 
	storemanager.OpenBodyTextFileForReadL(file, bodyTextId, filePath);

	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle()); 
	aMessage.WriteL(2,pckgSubSessionHandle);
	User::LeaveIfError(file.TransferToClient(aMessage, 2));
	CleanupStack::PopAndDestroy(2, &fs);
	}


/**
Creates the RFile in location specified by filepath encapsulted in RMessage2.
@param aMessage RMessage2 encapsulating client request and data.
@return none.
*/  
void CMsvServerSession::CreatePlainTextFileL(const RMessage2 &aMessage)
	{
	TMsvId bodyTextId = aMessage.Int0();
	
	iMsvServer.PoliceModifyEntryL(aMessage, bodyTextId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CreatePlainTextFileL")); 
	
	RFs fs;
	ConnectAndShareFileSessionLC(fs);
	
	RFile file;
	CleanupClosePushL(file);
		
	// Set the CMsvServer and file session 
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer, fs); 
	storemanager.CreatePlainTextFileL(file, bodyTextId);
	
	TPckg<TInt> pckgSubSessionHandle(file.SubSessionHandle()); 
	aMessage.WriteL(1,pckgSubSessionHandle);
	User::LeaveIfError(file.TransferToClient(aMessage,1));
	CleanupStack::PopAndDestroy(2, &fs);
	}


/**
Deletes the RFile located in filepath encapsulted in RMessage2
@param aMessage RMessage2 encapsulating client request and data.
@return none.
*/
void CMsvServerSession::DeletePlainTextFileL(const RMessage2& aMessage)
	{
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer, iMsvServer.FileSession()); 
	TMsvId bodyTextId = aMessage.Int0();

	// Police request - client must be able to modify the entry.
	iMsvServer.PoliceModifyEntryL(aMessage, bodyTextId, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DeletePlainTextFileL"));
	storemanager.DeletePlainTextFileL(bodyTextId);
	}

/**
Replace the RFile for the message Id encapsulted in RMessage2.
@param aMessage RMessage2 encapsulating client request and data.
@return none.
*/  
void CMsvServerSession::ReplacePlainTextFileL(const RMessage2 &aMessage)
	{
	// Get the Message Id from slot 0
	TMsvId bodyTextId = aMessage.Int0();
	
	// Police request - client must be able to modify the entry.
	TBool local;
	iMsvServer.PoliceModifyEntryL(aMessage, bodyTextId, local, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ReplacePlainTextFileL")); 

	if( local )
		{
		// Update the owner ID of the entry. 
		UpdateOwnerIdL(bodyTextId, aMessage.SecureId());
		}

	// Set the CMsvServer and file session 
	TMsvServerStoreManager storemanager;
	storemanager.SetMsvServerAndFileSession(iMsvServer,iMsvServer.FileSession()); 
		
	storemanager.ReplacePlainTextFileL(bodyTextId);
	aMessage.Complete(KErrNone);
	}

/**
Gets data from a server MTM that is not related to any operation being performed on
that MTM.

@param aMessage Received message
*/
void CMsvServerSession::GetNonOperationMtmDataL(const RMessage2 &aMessage)
	{
	TPtrC8 desPtr;

	iMsvServer.GetNonOperationMtmDataL(static_cast<TMsvId>(aMessage.Int0()), static_cast<TNonOperationMtmDataType>(aMessage.Int1()), desPtr);

	TInt desLength = aMessage.GetDesMaxLength(2);
	if (desLength < desPtr.Length())
		{
		aMessage.Complete(KErrOverflow);
		}
	else
		{
		WriteL(aMessage, 2, desPtr);
		aMessage.Complete(KErrNone);
		}
	}


/*****************************************
 * SearchSortOnHeaderAndBodytMsgL() :
 * Get the Query data from  client to make Search-Sort on HEADER abd BODY
 * in meassage server.
 * 
 * @param aMessage Received message
 ****************************************/ 
void CMsvServerSession::SearchSortOnHeaderAndBodytMsgL(const RMessage2& aMessage)
	{
	//For Simultaneously query
	if(CMSvSearchSortCacheManager::Instance()->OutstandingSOSOperations() > 0)
       		{
			if(CMSvSearchSortCacheManager::Instance()->iProgress != KMsvSearchSortOpCompleted)
				{
				User::Leave(KErrServerBusy); // One is SOS request is under proress.
				}
       		}
	CMSvSearchSortCacheManager::Instance()->AddOutstandingSOSOperation();

	//Header and body.
	TMsvOp operationId = aMessage.Int0();
		
	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 1, buffer);
	CleanupStack::PushL(buffer);

	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::SearchSortOnHeaderAndBodytMsgL"));
	
	CMsvSearchSortQuery* query= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(query);

	TMsvPackQuery packedQuery(buffer);
	packedQuery.UnpackQuery(query);
		
	TBool markQuery = aMessage.Int2();
	
	CMsvSearchSortCacheEntry *aEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(query, markQuery);
	CleanupStack::PushL(aEntry);
	
	TInt indexCount =CMSvSearchSortCacheManager::Instance()->QueryExists(*aEntry);
	//Check the query is exist or not.
	
	if(CMSvSearchSortCacheManager::Instance()->QueryExists(*aEntry)<0)
		{
	
		//NEW Query
		if(!aEntry->IsSortQuery())
			{
			CMSvSearchSortCacheManager::Instance()->TypeOfSearchQuery(*aEntry);
			}
		else
			{
			CMSvSearchSortCacheManager::Instance()->TypeOfSortQuery(*aEntry);
			}
		//Do process query.
		CMSvSearchSortCacheManager::Instance()->DoProcessQueryL(*aEntry);
		}
	else 
		{
		//Existing Query
		CMSvSearchSortCacheManager::Instance()->DoProcessQueryL(indexCount);
		}
	CleanupStack::Pop();//  aEntry
	CleanupStack::PopAndDestroy(2); //  query, buffer
	aMessage.Complete(KErrNone);
	}
	

/*****************************************
 * SearchSortOnIndexEntry() :
 * Get the Query data from  client to make Search-Sort on INDEX ENTRY
 * in meassage server.
 * 
 * @param aMessage Received message
 ****************************************/ 

void CMsvServerSession::SearchSortOnIndexEntryL(const RMessage2& aMessage)
	{
  	//For Simultaneously query
   	if(CMSvSearchSortCacheManager::Instance()->OutstandingSOSOperations() > 0)
       	{
       	if(CMSvSearchSortCacheManager::Instance()->iProgress != KMsvSearchSortOpCompleted)
			{
            User::Leave(KErrServerBusy); // One is SOS request is under proress.
			}
       	}
 	CMSvSearchSortCacheManager::Instance()->AddOutstandingSOSOperation();

	//Search sort on index entry.
	// Recover the operation data
	TMsvOp operationId = aMessage.Int0();
	
	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 1, buffer);
	CleanupStack::PushL(buffer);
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::SearchSortOnIndexEntryL"));

	CMsvSearchSortQuery* query= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(query);

	TMsvPackQuery packedQuery(buffer);
	packedQuery.UnpackQuery(query);

		
	TBool markQuery = aMessage.Int2();
	TInt  iteratorCount = aMessage.Int3();
	
	CMsvSearchSortCacheEntry *aEntry = CMsvSearchSortCacheEntry::CreateQueryEntryL(query, markQuery,iteratorCount);
	CleanupStack::PushL(aEntry);
	
	TInt indexCount =CMSvSearchSortCacheManager::Instance()->QueryExists(*aEntry);
	//Query Exist or not
	if(CMSvSearchSortCacheManager::Instance()->QueryExists(*aEntry)<0)
		{
		//NEW Query
		if(!aEntry->IsSortQuery())
			{
			CMSvSearchSortCacheManager::Instance()->TypeOfSearchQuery(*aEntry);
			}
		else
			{
			CMSvSearchSortCacheManager::Instance()->TypeOfSortQuery(*aEntry);
			}
		//Do process query.
		CMSvSearchSortCacheManager::Instance()->DoProcessQueryL(*aEntry,iteratorCount);
		}
	else 
		{
		//Existing Query
		CMSvSearchSortCacheManager::Instance()->DoProcessQueryL(indexCount);
		}
	
	CleanupStack::Pop();		// aEntry
	CleanupStack::PopAndDestroy(2, buffer); // buffer, CmsvSearchSortQuery
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::SendSearchSortIndexEntryResultL(const RMessage2 &aMessage)
	{
	TBool resultType = aMessage.Int0();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::SendSearchSortIndexEntryResultL"));
	//id's
	if(resultType) // EResultAsTMsvID
		{
		TMsvPackedIdOperation op(iBuffer);
		TInt error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
		while (error != KErrNone)
			{
			// increase the size of the buffer and try again
			iBuffer->Des().SetLength(0); // to avoid copying contents
			iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
			error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
			}
		}
	else  // EResultAsTMsvEntry
		{
		//As of now it takes TMsvId.
		}
	WriteBufferL(aMessage, 1);
	CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray.Reset();
	CMSvSearchSortCacheManager::Instance()->iToFindResultAsIdArray.Reset();
	CMSvSearchSortCacheManager::Instance()->iMsvIdWithSortFieldArray.Reset();
	aMessage.Complete(KErrNone);
	}



//sending resultas
void CMsvServerSession::SendResultOrIdsToSearchL(const RMessage2& aMessage)
	{
	// result type (is it a Final result, Partial Result or New query)
	TPckgBuf<TInt> resultType;
	TInt errorMem = KErrNone;
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::SendResultOrIdsToSearchL"));   
	//resuttype
	resultType = CMSvSearchSortCacheManager::Instance()->iReturnResultType;
	
	if(CMSvSearchSortCacheManager::Instance()->iReturnResultType == KMsvSearchSortQueryIdNotFound)
		{
		WriteL(aMessage, 1, resultType);
		}
	else
		{
		// need to send data to client according resultType 
		// result type is KFinalResult, KNewQuery or KPartilaResult
		TInt error = KErrNone;
		
		TMsvPackedIdOperation op(iBuffer);
		
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
		error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
		while (error != KErrNone)
			{
			// increase the size of the buffer and try again
			iBuffer->Des().SetLength(0); // to avoid copying contents
			iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
			error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
			}

#else   // else SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB	
	
		if(CMSvSearchSortCacheManager::Instance()->iReturnResultType == KFinalResult)
			{
			CMSvSearchSortCacheManager::Instance()->iSearchSortDeltaCache->iIsHeaderSearchEnabled = EFalse;
			error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
			
			while (error != KErrNone)
				{
				// increase the size of the buffer and try again
				iBuffer->Des().SetLength(0); // to avoid copying contents
				iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
				error = op.Pack(CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray);
				}
			}
		else if(CMSvSearchSortCacheManager::Instance()->iReturnResultType == KNewQuery)
			{
			error = op.Pack(CMSvSearchSortCacheManager::Instance()->iToFindResultAsIdArray);
			while (error != KErrNone)
				{
				// increase the size of the buffer and try again
				iBuffer->Des().SetLength(0); // to avoid copying contents
				iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
				error = op.Pack(CMSvSearchSortCacheManager::Instance()->iToFindResultAsIdArray);
				}
			}
		else // for KPartialResult
			{
			error = op.Pack(CMSvSearchSortCacheManager::Instance()->iDeltaCacheIdArray);
			while (error != KErrNone)
				{
				// increase the size of the buffer and try again
				iBuffer->Des().SetLength(0); // to avoid copying contents
				iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
				error = op.Pack(CMSvSearchSortCacheManager::Instance()->iDeltaCacheIdArray);
				}
			}
#endif // SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB
			
		// send Id's in buffer
		TRAP(errorMem,WriteBufferL(aMessage,0));
			
		//send resultType to client, whether it's final result, partial result or newQuery.
		WriteL(aMessage,1,resultType);
		}
	
	//needs to reset RArray
	if(errorMem==KErrNone)
		{
		CMSvSearchSortCacheManager::Instance()->iFinalResultAsIdArray.Reset();
		CMSvSearchSortCacheManager::Instance()->iToFindResultAsIdArray.Reset();
		CMSvSearchSortCacheManager::Instance()->iMsvIdWithSortFieldArray.Reset();
		}

	aMessage.Complete(errorMem);
	}

void CMsvServerSession::GetSearchSortResultCountL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> resultCount;
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetSearchSortResultCountL"));  
		
	TInt  index = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();
	CMSvSearchSortCacheManager::Instance()->ResultCountL(index-1);
	
	resultCount() = CMSvSearchSortCacheManager::Instance()->iFinalResultCount;//Contain the result.
	WriteL(aMessage, 0, resultCount);
	CMSvSearchSortCacheManager::Instance()->iFinalResultCount = 0;
	aMessage.Complete(KErrNone);
	}
	
	
void CMsvServerSession::GetResultInIteratorL(const RMessage2& aMessage)
	{
	TPckgBuf<TMsvId> messageId;
	TPckgBuf<TInt> count;
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetResultInIteratorL"));   
		
	TInt  index = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();
	CMSvSearchSortCacheManager::Instance()->NextResultForInteraratorL(index-1);
	messageId() = CMSvSearchSortCacheManager::Instance()->iIteratorId;
	count() = CMSvSearchSortCacheManager::Instance()->iIteratorRemainingResultCount;
	
	//result as TMsvId
	WriteL(aMessage, 0, messageId);
	//for remaing count
	WriteL(aMessage, 1, count);
	
	CMSvSearchSortCacheManager::Instance()->iIteratorId = 0;
	aMessage.Complete(KErrNone);
	}
	
void CMsvServerSession::GetNextEntryInIteratorL(const RMessage2& aMessage)
	{
	//need this varible to send remaining count to client
	TPckgBuf<TInt> count;
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetNextEntryInIteratorL"));	
	
	TInt  index = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();
	CMSvSearchSortCacheManager::Instance()->NextResultForInteraratorL(index-1);
	
	// package the entry
	TMsvPackedEntry packedEntry(iBuffer);
	TInt error = packedEntry.PackEntry(CMSvSearchSortCacheManager::Instance()->iIteratorEntry);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = packedEntry.PackEntry(CMSvSearchSortCacheManager::Instance()->iIteratorEntry);
		}
	//needs fill the remaing count
	count() =  CMSvSearchSortCacheManager::Instance()->iIteratorRemainingResultCount;
	
	//result as TMsvEntry
	WriteBufferL(aMessage, 0);
	//for remaing count
	WriteL(aMessage, 1, count);
	
	aMessage.Complete(KErrNone);
	}
	

void CMsvServerSession::GetQueryIDL(const RMessage2& aMessage)
	{
	TPckgBuf<TInt> queryId;
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetQueryIDL"));	
	
	queryId() = CMSvSearchSortCacheManager::Instance()->QueryID();
	
	WriteL(aMessage, 0, queryId);
	aMessage.Complete(KErrNone);
	}
	
void CMsvServerSession::UpdateSearchSortCacheWithSortFiledL(const RMessage2& aMessage)
	{
	TInt  index = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::UpdateSearchSortCacheWithSortFiledL"));	
	
	// get the length
	TInt desLen = aMessage.GetDesLength(1);
	if(desLen == 0)
		{
		aMessage.Complete(KErrArgument);
		return;
		}

	// allocate buffer copy the data
	CBufFlat* buffer = CBufFlat::NewL(desLen);
	CleanupStack::PushL(buffer);
	buffer->ExpandL(0, desLen);
	RBufReadStream readStream(*buffer);
	CleanupClosePushL (readStream);
	TPtr8 desPtr = buffer->Ptr(0);
	
	TRAPD(err,  aMessage.ReadL(1, desPtr));
	User::LeaveIfError(err);
	// object of TMsvPackedIdAndMessagePart 
	TMsvPackedIdAndMessagePart packIdAndMessagePart;
	
	//extract TMsvId with Sort field
	packIdAndMessagePart.InternalizeL(readStream, CMSvSearchSortCacheManager::Instance()->iMsvIdWithSortFieldArray);
	
	//Manager class update the Db.
	CMSvSearchSortCacheManager::Instance()->StoreSortResultL(index,CMSvSearchSortCacheManager::Instance()->iMsvIdWithSortFieldArray);
	
	CleanupStack::PopAndDestroy(2, buffer);
	aMessage.Complete(KErrNone);
	}
	
void CMsvServerSession::QueryUnMarkedL(const RMessage2& aMessage)
	{
	
	TInt  queryID = aMessage.Int0();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::PoliceSearchSortQueryReadRequestL"));  
	
	TInt index =  CMSvSearchSortCacheManager::Instance()->QueryExists(queryID);
	if(index>0)
		{
		CMSvSearchSortCacheManager::Instance()->UnMarkedQuery(index);
		}
	else
		{
		aMessage.Complete(KErrNotFound);	
		}   
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::GetResultForQueryIDL(const RMessage2& aMessage)
	{
    //For Simultaneously query
    if(CMSvSearchSortCacheManager::Instance()->OutstandingSOSOperations() > 0)
        {
		if(CMSvSearchSortCacheManager::Instance()->iProgress != KMsvSearchSortOpCompleted )
			{
			User::Leave(KErrServerBusy); // One is SOS request is under proress.
			}
         }
    CMSvSearchSortCacheManager::Instance()->AddOutstandingSOSOperation();
	
	TInt operationId = aMessage.Int0();
	TInt queryID = aMessage.Int1();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetResultForQueryIDL"));   
	
	TInt index =  CMSvSearchSortCacheManager::Instance()->QueryExists(queryID);
	if(index>=0)
		{
		CMSvSearchSortCacheManager::Instance()->DoProcessQueryL(index);
		}
	else
		{
		//Invalid query id.
		CMSvSearchSortCacheManager::Instance()->iReturnResultType = KMsvSearchSortQueryIdNotFound;
		}
	
	aMessage.Complete(KErrNone);
	}


void CMsvServerSession::GetSearchSortProgressInfoL(const RMessage2 &aMessage)
	{
	TInt operationId = aMessage.Int0();
	TPckg<TInt> progress(KMsvSearchSortOpNone);
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetSearchSortProgressInfoL")); 
	
	progress() = CMSvSearchSortCacheManager::Instance()->ReturnProgressInfo();
	
	WriteL(aMessage, 1, progress);
	aMessage.Complete(KErrNone);
	}
	
void CMsvServerSession::SearchSortOperationCancelL(const RMessage2& aMessage)
	{
	TInt operationId = aMessage.Int0();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::SearchSortOperationCancel"));  
	
	CMSvSearchSortCacheManager::Instance()->CancelSearchSortOperation();
	//cancel the current oeration and delete the query and correspnding entry from DB.
	aMessage.Complete(KErrNone);
	}

//Update search sort results to DB, and sort by TMsvEntry fields invalid id's
void CMsvServerSession::UpdateSearchSortResultsAndSortByEntryL(const RMessage2& aMessage)
	{
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::UpdateSearchSortResultsAndSortByEntryL")); 
	
	TInt  index = CMSvSearchSortCacheManager::Instance()->iManagerEntry->Count();

	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 1, buffer);
	CleanupStack::PushL(buffer);

	// Unpack the entries from the buffer
	TMsvPackedIdOperation packedIdOperation(buffer);
	packedIdOperation.UnpackL(CMSvSearchSortCacheManager::Instance()->iUpdateIdsToCacheArray);
	
	// Update DB and sort by TMsvEnty field with existing partil query
	// Invalid ID need to delete.
	CMSvSearchSortCacheManager::Instance()->StoreResultL(index,CMSvSearchSortCacheManager::Instance()->iUpdateIdsToCacheArray);
	
	CleanupStack::PopAndDestroy(buffer); // buffer
	
	aMessage.Complete(KErrNone);
	}

void CMsvServerSession::CopyQueryDataL(const RMessage2& aMessage)
	{
	TInt operationId = aMessage.Int0();
	TInt queryId = aMessage.Int1();
	
	iMsvServer.PoliceSearchSortQueryReadRequestL(aMessage,__PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CopyQueryDataL")); 
	
	CMsvSearchSortQuery* searchSortQuery= CMsvSearchSortQuery::NewL() ;
	CleanupStack::PushL(searchSortQuery);
	
	TInt index =  CMSvSearchSortCacheManager::Instance()->QueryExists(queryId);
	if(index>=0)
		{
		//This below function will return  TMsvSearchSortQuery object .
		CMSvSearchSortCacheManager::Instance()->RetrunQuery(index, searchSortQuery);
		}
	else
		{
		//Query id not found.
		CleanupStack::PopAndDestroy(searchSortQuery); // searchSortQuery
		aMessage.Complete(KErrNotFound);
		return;
		}
	
	TMsvPackQuery packedQuery(iBuffer);
	
	TInt error = KErrNone;
	error = packedQuery.PackQuery(searchSortQuery);
	
	WriteBufferL(aMessage, 2);
	CleanupStack::PopAndDestroy(searchSortQuery); // searchSortQuery
	aMessage.Complete(error);
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * GetChildrenAllL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvGetChildrenALL.
 * If the parent id is a standard folder the function fetches
 * the child entries from all drives in the preferred drive list
 * as maintained by server.
 */
 void CMsvServerSession::GetChildrenAllL(const RMessage2 &aMessage)
	{
	// Reset internal datastructure.
	iChildrenSelection->Reset();
	iChildrenSelectionIds->Reset();

	// Copy across the children details structure.
	TPckgBuf<TMsvChildrenDetails> children;
	aMessage.ReadL(0, children);

	// Check that the children details arguments are empty. 
	// Panic in debug mode but try to handle it gracefully 
	// in release code.
	__ASSERT_DEBUG( children().iTotalNumberChildren==0 &&
					children().iNumberChildrenInArray==0, PanicServer(EMsvChildrenDetailsNotEmpty2));

	if( children().iTotalNumberChildren != 0 || children().iNumberChildrenInArray != 0 )
		{
		aMessage.Complete(KErrArgument);
		return;
		}
	
	// The function is allowed only for following standard 
	// folders of current drive:
	// Inbox, Outbox, Draft, Sent and Deleted.
	if( (KCurrentDriveId != GetDriveId(children().iParentId)) ||
		( KMsvLocalServiceIndexEntryId >= UnmaskTMsvId(children().iParentId) ||
		  KMsvUnknownServiceIndexEntryId <= UnmaskTMsvId(children().iParentId) )
	  )
		{
		aMessage.Complete(KErrArgument);
		return;
		}
		
	// Copy across the sort order.
	TPckgBuf<TMsvSelectionOrdering> order;
	aMessage.ReadL(1, order);

	// Get the children as a selection. Need to 
	// filter the list via the client secure ID 
	// if the client is not trusted with Read User Data.
	TBool filterByOwnerId = !aMessage.HasCapability(ECapabilityReadUserData);
	
	iMsvServer.IndexAdapter().GetChildrenAllL(children().iParentId, *iChildrenSelection, order(), KUidMsvNullEntry, filterByOwnerId, aMessage.SecureId());

	// Return number of children.
	children().iTotalNumberChildren = iChildrenSelection->Count();

	// Package up the entries.
	iBuffer->Des().SetMax();
	TMsvPackedEntryArray packedEntryArray(iBuffer);
	
	TInt count=0;
	TInt error=KErrNone;
	TInt totalCount=iChildrenSelection->Count();
	for (; count<totalCount; count++)
		{
		error = packedEntryArray.PackEntry(*iChildrenSelection->At(count));
		if (error)
			{
			children().iLastEntryInArray = count;
			break;
			}
		}
	// return number of children in the array
	children().iNumberChildrenInArray = count;

	// write the children to client
	WriteL(aMessage, 0, children);
	
	// write the array to the client (if any entries are in the array)
	if (children().iNumberChildrenInArray)
		WriteL(aMessage, 2, iBuffer->Des());

	if (error==KErrNone)
		{
		// reset the member data
		iChildrenSelection->Reset();
		}
	else
		{
		// keep an list of the ids separatelyin case they are deleted by another client
		TInt totalCount=iChildrenSelection->Count();
		for (count=0; count<totalCount; count++)
			iChildrenSelectionIds->AppendL(iChildrenSelection->At(count)->Id());
		iChildrenDetails = children();
		}

	// signal the client and finished with selection
	aMessage.Complete(error);
	}



/** 
 * GetChildIdsAll()
 *
 * Gets the children of a parent entry specified as first argument.
 * If the passed parent is a standard folder the function will fetch 
 * entries from all drives currently present in the preferred drive list.
 */
void CMsvServerSession::GetChildIdsAllL(const RMessage2& aMessage)
	{
	TMsvId id = aMessage.Int1();

	// The function is allowed only for following standard 
	// folders of current drive:
	// Inbox, Outbox, Draft, Sent and Deleted.
	if( (KCurrentDriveId != GetDriveId(id)) ||
		(KMsvLocalServiceIndexEntryId >= UnmaskTMsvId(id)) ||
		(KMsvUnknownServiceIndexEntryId <= UnmaskTMsvId(id))
	  )
		{
		aMessage.Complete(KErrArgument);
		return;
		}

	CMsvEntrySelection* selection = new(ELeave)CMsvEntrySelection;
	CleanupStack::PushL(selection);
	HBufC8* buffer = NULL;
	ReadBufferL(aMessage, 0, buffer);
	CleanupStack::PushL(buffer);

	CMsvEntryFilter* filter = CMsvEntryFilter::NewLC();
	TMsvPackedEntryFilter package(buffer);
	package.UnpackFilter(*filter);

	// Need to filter the list via the client secure ID if the client is not
	// trusted with Read User Data.
	TBool filterByOwnerId = !aMessage.HasCapability(ECapabilityReadUserData);

	TInt error = iMsvServer.IndexAdapter().GetChildrenIdAll(id, *filter, *selection, filterByOwnerId, aMessage.SecureId());
	if(KErrNone != error)
		{
		aMessage.Complete(error);
		CleanupStack::PopAndDestroy(3); // selection, buffer, filter
		}
		
	TMsvPackedOperation op(iBuffer);

	error = op.Pack(*selection, 0, 0);
	while (error != KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = op.Pack(*selection, 0, 0);
		}

	WriteBufferL(aMessage, 0);

	aMessage.Complete(KErrNone);
	CleanupStack::PopAndDestroy(3); // selection, buffer, filter
	}
	
	


/**
 * MessageDrive()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvGetMessageDrive.
 */
void CMsvServerSession::MessageDrive(const RMessage2 &aMessage)
	{
	aMessage.Complete(CMsvPreferredDriveList::GetDriveList()->CurrentDriveNumber());		
	}




/**
 * CurrentDriveInfoL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvGetCurrentDriveInfo.
 */	 
void CMsvServerSession::CurrentDriveInfoL(const RMessage2 &aMessage)
	{
	TMsvPreferredDrive driveInfo;
	TUint currentDriveIndex = CMsvPreferredDriveList::GetDriveList()->CurrentDriveIndex();
	TRAPD(err, CMsvPreferredDriveList::GetDriveList()->DriveInfoL(currentDriveIndex, driveInfo));
	if(KErrNone == err)
		{
		TPckgBuf<TDriveNumber> driveNum;
		TPckgBuf<TUint> priority;
		
		driveNum() = driveInfo.driveNum;
		priority() = currentDriveIndex + 1;
		
		WriteL(aMessage, 0, driveNum);
		WriteL(aMessage, 1, priority);
		}
	aMessage.Complete(err);
	}
	


/**
 * DriveListL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvGetDriveList.
 */	 
void CMsvServerSession::DriveListL(const RMessage2 &aMessage)
	{
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	RArray<TDriveNumber> driveNumList;
	CleanupClosePushL(driveNumList);
	
	for(TInt index=0; index<driveList->Count(); index++)
		{
		driveNumList.AppendL((*driveList)[index].driveNum);	 
		}
	
	TMsvPackedDriveIdOperation driveOp(iBuffer);
	User::LeaveIfError(driveOp.Pack(driveNumList));
	WriteBufferL(aMessage, 0);
	
	CleanupStack::PopAndDestroy();	  // driveNumList
	aMessage.Complete(KErrNone);
	}
	


/**
 * AvailableDriveListL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvGetAvailableDriveList.
 */ 
void CMsvServerSession::AvailableDriveListL(const RMessage2 &aMessage)
	{
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	RArray<TDriveNumber> driveNumList;
	CleanupClosePushL(driveNumList);
	
	for(TInt index=0; index<driveList->Count(); index++)
		{
		if(EMsvMessageStoreAvailableStatus == (*driveList)[index].status)
			{
			driveNumList.AppendL((*driveList)[index].driveNum);	 
			}
		}
	
	TMsvPackedDriveIdOperation driveOp(iBuffer);
	User::LeaveIfError(driveOp.Pack(driveNumList));
	WriteBufferL(aMessage, 0);
	
	CleanupStack::PopAndDestroy();	  // driveNumList
	aMessage.Complete(KErrNone);
	}
	
	
	

/**
 * AddDriveL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvAddDriveToDriveList.
 */	 
void CMsvServerSession::AddDriveL(const RMessage2 &aMessage)
	{
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	TUint priority = (TUint) aMessage.Int1();

	TRAPD(err, iMsvServer.AddDriveToListL(driveNumber, priority, this));
	if(KErrNone == err)
		{
		TPckgBuf<TUint> priorityBuf;
		priorityBuf() = priority;
		WriteL(aMessage, 2, priorityBuf);
		}
	aMessage.Complete(err);
	}
	
	

/**
 * RemoveDriveL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvRemoveDriveFromDriveList.
 */ 
void CMsvServerSession::RemoveDriveL(const RMessage2 &aMessage)
	{
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	TRAPD(err, iMsvServer.RemoveDriveFromListL(driveNumber));
	aMessage.Complete(err);
	}
	
	

/**
 * UpdateDrivePriorityL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvUpdateDrivePriority.
 */	 
void CMsvServerSession::UpdateDrivePriorityL(const RMessage2 &aMessage)
	{
	TDriveNumber driveNumber = (TDriveNumber) aMessage.Int0();
	TUint priority = (TUint) aMessage.Int1();

	TRAPD(err, iMsvServer.UpdateDrivePriorityL(driveNumber, priority));
	if(KErrNone == err)
		{
		TPckgBuf<TUint> priorityBuf;
		priorityBuf() = priority;
		WriteL(aMessage, 2, priorityBuf);
		}
	aMessage.Complete(err);
	}
	
	
/**
 * ResetRepositoryL()
 *
 * @param RMessage2.
 *
 * Code changes for PREQ 557.
 * Service request EMsvResetRepository.
 */ 
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void CMsvServerSession::ResetRepositoryL(const RMessage2 &aMessage)
	{
	TRAPD(err, iMsvServer.ResetRepositoryL());
	aMessage.Complete(err);
	}


void CMsvServerSession::PrintCache(const RMessage2 &aMessage)
	{
	#ifdef _DEBUG
		iMsvServer.Context().IndexAdapter()->PrintL();
	#endif
	aMessage.Complete(KErrNone);
	}

#endif	  // #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

#endif		  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)







#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/*
 * CreateHeaderTableL()
 *
 * The function creates the header table in the message store.
 */
void CMsvServerSession::CreateHeaderTableL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityWriteDeviceData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CreateHeaderTableL")));
		}

	// Capability check is fine. Proceed.
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	ReadBufferL(aMessage, 1, iBuffer);
	
	// Read the header structure from the buffer.
	RPointerArray<CFieldPair> fieldDetails;
	CleanupClosePushL(fieldDetails);
	TMsvPackedHeaderStructure headerStruct(iBuffer);
			
	headerStruct.UnpackL(fieldDetails);
	
	// Create the header table and handle error.
	TRAPD(err, iMsvServer.MessageDBAdapter().CreateHeaderTableL(mtmTypeUid, fieldDetails, iLastDBErrorMessage));
	fieldDetails.ResetAndDestroy();
	CleanupStack::PopAndDestroy();		  // fieldDetails
	aMessage.Complete(err);
	}





/*
 * DoesStoreExistsL()
 *
 * The function checks if a header table for a MTM Id already exists in message store.
 */
void CMsvServerSession::DoesStoreExistsL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityReadDeviceData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityReadDeviceData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DoesStoreExistsL")));
		}

	// Capability check is fine. Proceed.
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	
	TBool isStoreExists = iMsvServer.MessageDBAdapter().IsHeaderTableExistsL(mtmTypeUid);

	TPckgBuf<TBool> storeExists;
	storeExists() = isStoreExists;
	WriteL(aMessage, 1, storeExists);
	aMessage.Complete(KErrNone);
	}
	
	



/*
 * LastErrorMessageL()
 *
 * This function is mainly used after the header table creation.
 * The function returns the error message text, if the header table
 * creation fails.
 */ 
void CMsvServerSession::LastErrorMessageL(const RMessage2 &aMessage)
	{
	TInt desLen = iLastDBErrorMessage.Size();   
	if (Align4(desLen+4) > aMessage.GetDesMaxLength(0))
		{
		User::Leave(KErrOverflow);
		}
	
	iBuffer->Des().SetLength(desLen+4);
	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	*ptr++ = desLen;
	Mem::Copy((void*)ptr, iLastDBErrorMessage.Ptr(), desLen);

	WriteBufferL(aMessage, 0);
	aMessage.Complete(KErrNone);
	}






/*
 * CreateHeaderEntryL()
 *
 * This function creates a new header entry in the database.
 */ 
void CMsvServerSession::CreateHeaderEntryL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityWriteUserData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityWriteUserData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::CreateHeaderEntryL")));
		}

	// Capability check is fine. Proceed.
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	TMsvId entryId = aMessage.Int1();
	
	ReadBufferL(aMessage, 2, iBuffer);
		
	RPointerArray<CHeaderFields> fieldPairList;
	CleanupClosePushL(fieldPairList);
	
	TMsvPackedHeaderData headerData(iBuffer);
	headerData.UnpackL(fieldPairList);
	
	TRAPD(err, iMsvServer.MessageDBAdapter().CreateHeaderEntryL(mtmTypeUid, entryId, fieldPairList));
	fieldPairList.ResetAndDestroy();
	CleanupStack::PopAndDestroy();
	aMessage.Complete(err); 
	}
	
	
	
	


/*
 * LoadHeaderEntryL()
 *
 * This function loads the header entry from the database
 * and returns to the client.
 */ 
void CMsvServerSession::LoadHeaderEntryL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityReadUserData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityReadUserData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::LoadHeaderEntryL")));
		}

	// Capability check is fine. Proceed.
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	TMsvId entryId = aMessage.Int1();
	
	RPointerArray<CHeaderFields> fieldPairList;
	CleanupClosePushL(fieldPairList);
	
	TRAPD(err, iMsvServer.MessageDBAdapter().LoadHeaderEntryL(mtmTypeUid, entryId, fieldPairList));
	if(KErrNone == err)
		{
		TMsvPackedHeaderData headerData(iBuffer);
		err = headerData.Pack(fieldPairList);
		while (err != KErrNone)
			{
			// increase the size of the buffer and try again
			iBuffer->Des().SetLength(0); // to avoid copying contents
			iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
			err = headerData.Pack(fieldPairList);
			}
		WriteBufferL(aMessage, 2);
		}
	fieldPairList.ResetAndDestroy();
	CleanupStack::PopAndDestroy();

	aMessage.Complete(err);
	}
	
	
	
	
	
	
/*
 * DeleteHeaderEntryL()
 *
 * This function deletes the header entry from the database.
 */	 
void CMsvServerSession::DeleteHeaderEntryL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityWriteUserData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityWriteUserData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::DeleteHeaderEntryL")));
		}

	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	TMsvId entryId = aMessage.Int1();
	
	TRAPD(err, iMsvServer.MessageDBAdapter().DeleteHeaderEntryL(mtmTypeUid, entryId));
	aMessage.Complete(err);
	}



	
	
	
/*
 * UpdateHeaderEntryL()
 *
 * This function udpates the header entry in the database.
 */ 
void CMsvServerSession::UpdateHeaderEntryL(const RMessage2 &aMessage)
	{
	// Check for capability.
	if(!aMessage.HasCapability(ECapabilityWriteUserData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityWriteUserData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::UpdateHeaderEntryL")));
		}

	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	TMsvId entryId = aMessage.Int1();
	
	ReadBufferL(aMessage, 2, iBuffer);
		
	RPointerArray<CHeaderFields> fieldPairList;
	CleanupClosePushL(fieldPairList);
	
	TMsvPackedHeaderData headerData(iBuffer);
	headerData.UnpackL(fieldPairList);
	
	TRAPD(err, iMsvServer.MessageDBAdapter().UpdateHeaderEntryL(mtmTypeUid, entryId, fieldPairList));
	fieldPairList.ResetAndDestroy();
	CleanupStack::PopAndDestroy();
	aMessage.Complete(err);
	}






/*
 * DoesAnyStoreExists()
 *
 * This function checks if the store (header/body) exists 
 * for a give metadata entry.
 */ 
void CMsvServerSession::DoesAnyStoreExists(const RMessage2 &aMessage)
	{
	TMsvId entryId = aMessage.Int0();
	TUid mtmTypeUid = TUid::Uid(aMessage.Int1());
	
	TBool isStoreExists = EFalse;
	TRAPD(err, isStoreExists = iMsvServer.MessageDBAdapter().DoesAnyStoreExistsL(entryId, mtmTypeUid));
	if(err)
		{
		aMessage.Complete(EFalse);
		}
	else
		{
		aMessage.Complete(isStoreExists);
		}
	}

void CMsvServerSession::DoesHeaderTableExist(const RMessage2 aMessage)
	{
	TUid mtmTypeUid = TUid::Uid(aMessage.Int0());
	
	TBool doesHeaderExists = EFalse;
	TRAPD(err, doesHeaderExists = iMsvServer.MessageDBAdapter().IsHeaderTableExistsL(mtmTypeUid));
	if(err)
		{
		aMessage.Complete(EFalse);
		}
	else
		{
		aMessage.Complete(doesHeaderExists);
		}
	}

/***************************************Converter API's***************************/
/*
 GetConvertibleDriveListL()
 Fetches a list of drives with message store unsupported	
 
 @param aMessage: RMessage2 representing client request and containing request data.
 @return None
 */
void CMsvServerSession::GetConvertibleDriveListL(const RMessage2 &aMessage)
	{
	// capability check
	if(!aMessage.HasCapability(ECapabilityReadDeviceData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityReadDeviceData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::GetConvertibleDriveListL")));
		}
	
	
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	RArray<TDriveNumber> driveNumList;
	CleanupClosePushL(driveNumList);
	
	for(TInt index=0; index<driveList->Count(); index++)
		{
		if(EMsvMessageStoreNotSupportedStatus == (*driveList)[index].status)
			{
			driveNumList.AppendL((*driveList)[index].driveNum);	 
			}
		}
	
	TMsvPackedDriveIdOperation driveOp(iBuffer);
	User::LeaveIfError(driveOp.Pack(driveNumList));
	WriteBufferL(aMessage, 0);
	
	CleanupStack::PopAndDestroy();	  // driveNumList
	aMessage.Complete(KErrNone);
	}

/*
 ConvertMessageStoreL()
 Initiates message store conversion on a drive. if the conversion is active on a drive
 than this request is queued and completed later.

 @param aMessage: RMessage2 representing client request and containing request data.
 @return None
 */
void CMsvServerSession::ConvertMessageStoreL(const RMessage2 &aMessage)
	{
	// capability check
	if(!aMessage.HasCapability(ECapabilityWriteDeviceData))
		{
		// Client missing capabilities - emit diagnostics and leave...
		User::LeaveIfError(PlatSec::CapabilityCheckFail(aMessage, ECapabilityWriteDeviceData, __PLATSEC_DIAGNOSTIC_STRING("Checked by CMsvServerSession::ConvertMessageStoreL")));
		}
	
	/*Instantiate the converter waiter thread. If it is already active, than the existing instance is returned
	  Else a new instance is created and returned. Singleton Pattern   */
	  
	iConverterWaiter=CMsvConverterWaiter::InstanceL(&iMsvServer);
		
	if(!iConverterWaiter->isRunningMessageStoreConverter()) 
		{
		iConverterWaiter->StartMessageStoreConversionL(aMessage,EFalse);
		}
	else 
		{
		// Queue the request if the drive is already not present
		iConverterWaiter->QueueConversionRequestL(aMessage);
		}
	}

/*
 CancelConversionRequestL()
 Cancels conversion request for a drive.

 @param aMessage: RMessage2 representing client request and containing request data.
 @return None
 */
void CMsvServerSession::CancelConversionRequestL(const RMessage2 &aMessage)
	{
	if(iConverterWaiter)
		{
		iConverterWaiter->CancelConversionL(aMessage);
		}
	else
		{
		aMessage.Complete(KErrNotFound);
		}
	}

/*
 GetConversionStatus()
 Gets the conversion status for a drive.
 
 @param aMessage: RMessage2 representing client request and containing request data.
 @return None
 */ 
void CMsvServerSession::GetConversionStatus(const RMessage2 &aMessage)
	{
	if(iConverterWaiter)
		{
		iConverterWaiter->GetConversionStatus(aMessage);
		}
	else
		{
		aMessage.Complete(KErrNotFound);
		}
	}

TBool CMsvServerSession::FoundUnSupportedDrives()
	{
	CMsvPreferredDriveList *driveList = CMsvPreferredDriveList::GetDriveList();
	for(TInt index=0; index<driveList->Count(); index++)
		{
		if(EMsvMessageStoreNotSupportedStatus == (*driveList)[index].status)
			{
			return ETrue;
			}
		}
	return EFalse;
	
	}
#endif	  // #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)






