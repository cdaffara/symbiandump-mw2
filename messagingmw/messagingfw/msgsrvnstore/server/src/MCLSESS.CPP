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
#undef _NO_SESSION_LOGGING_
#endif

#include <s32std.h>

#include "MSVIPC.H"
#include "MSVUIDS.H"
#include "MSVIDS.H"
#include "MSVAPI.H"
#include "MSVCOP.H"
#include "MSVPANIC.H"

#include <mmsvstoremanager.h>
#include <tmsvsystemprogress.h>
#include <tnonoperationmtmdata.h>

const TInt KMsvSessionObserverArrayGranularity=4;  

//**********************************
// CMsvSession
//**********************************

EXPORT_C CMsvSession* CMsvSession::OpenSyncL(MMsvSessionObserver& aObserver)
//
//
//
/** Creates a session synchronously. 

The session can be used once the function returns. 

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message 
Server. 
@leave KErrNoMemory Not enough memory to create object. 
@return New session object */
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(ETrue);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CMsvSession* CMsvSession::OpenAsyncL(MMsvSessionObserver& aObserver)
//
//
//
/** Creates a session asynchronously. 

The session cannot be used until the passed MMsvSessionObserver observer has 
been informed that the message server is ready with TMsvSessionEvent::EMsvServerReady. 
If any functions are called before this, they will fail with KErrNotReady.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver);
	CleanupStack::PushL(self);
	self->ConstructL(EFalse);
	CleanupStack::Pop(self);
	return self;
	}


EXPORT_C CMsvSession* CMsvSession::OpenAsObserverL(MMsvSessionObserver& aObserver)
//
//
//
/** Creates a session that is used only as an observer. 

The session created with this function should only be used to observe events, and 
not actively use the server. It used for applications such as system monitoring utilities.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver);
	CleanupStack::PushL(self);
	self->ConstructAsObserverL();
	CleanupStack::Pop(self);
	return self;
	}


/** 
Creates a session synchronously. 

The session can be used once the function returns. 

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message 
Server. 
@param aFs A connected file server session 
@leave KErrNoMemory Not enough memory to create object. 
@return New session object 
*/
EXPORT_C CMsvSession* CMsvSession::OpenSyncL(MMsvSessionObserver& aObserver, RFs& aFs)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs);
	CleanupStack::PushL(self);
	self->ConstructL(ETrue);
	CleanupStack::Pop(self);
	return self;
	}

/** 
Creates a session asynchronously. 

The session cannot be used until the passed MMsvSessionObserver observer has 
been informed that the message server is ready with TMsvSessionEvent::EMsvServerReady. 
If any functions are called before this, they will fail with KErrNotReady.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aFs A connected file server session 
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
EXPORT_C CMsvSession* CMsvSession::OpenAsyncL(MMsvSessionObserver& aObserver, RFs& aFs)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs);
	CleanupStack::PushL(self);
	self->ConstructL(EFalse);
	CleanupStack::Pop(self);
	return self;
	}


/**
Creates a session that is used only as an observer. 

The session created with this function should only be used to observe events, and 
not actively use the server. It used for applications such as system monitoring utilities.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aFs A connected file server session 
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
EXPORT_C CMsvSession* CMsvSession::OpenAsObserverL(MMsvSessionObserver& aObserver, RFs& aFs)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs);
	CleanupStack::PushL(self);
	self->ConstructAsObserverL();
	CleanupStack::Pop(self);
	return self;
	}

CMsvSession::CMsvSession(MMsvSessionObserver& aObserver)
: CActive(EPriorityStandard), iMainObserver(aObserver), iReceiveEntryEvents(ETrue)
	{
	__DECLARE_NAME(_S("CMsvSession"));
	}

CMsvSession::CMsvSession(MMsvSessionObserver& aObserver, RFs& aFs)
: CActive(EPriorityStandard), iFs(aFs), iMainObserver(aObserver), iReceiveEntryEvents(ETrue), iUseSharedFs(ETrue)
	{
	}

EXPORT_C CMsvSession* CMsvSession::OpenSyncL(MMsvSessionObserver& aObserver, TInt aPriority)
//
//
//
/** Creates a session synchronously. 

The session can be used once the function returns.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message 
Server.
@param aPriority Active object priority 
@leave KErrNoMemory Not enough memory to create object. 
@return New session object */
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL(ETrue);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CMsvSession* CMsvSession::OpenAsyncL(MMsvSessionObserver& aObserver, TInt aPriority)
//
//
//
/** Creates a session asynchronously. 

The session cannot be used until the passed MMsvSessionObserver observer has 
been informed that the message server is ready with TMsvSessionEvent::EMsvServerReady. 
If any functions are called before this, they will fail with KErrNotReady.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aPriority Active object priority
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL(EFalse);
	CleanupStack::Pop(self);
	return self;
	}


EXPORT_C CMsvSession* CMsvSession::OpenAsObserverL(MMsvSessionObserver& aObserver, TInt aPriority)
//
//
//
/** Creates a session that is used only as an observer. 

The session created with this function should only be used to observe events, and 
not actively use the server. It used for applications such as system monitoring utilities.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aPriority Active object priority
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aPriority);
	CleanupStack::PushL(self);
	self->ConstructAsObserverL();
	CleanupStack::Pop(self);
	return self;
	}


/** 
Creates a session synchronously. 

The session can be used once the function returns.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message 
Server. 
@param aFs A connected file server session 
@param aPriority Active object priority
@leave KErrNoMemory Not enough memory to create object. 
@return New session object 
*/
EXPORT_C CMsvSession* CMsvSession::OpenSyncL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL(ETrue);
	CleanupStack::Pop(self);
	return self;
	}

/** 
Creates a session asynchronously. 

The session cannot be used until the passed MMsvSessionObserver observer has 
been informed that the message server is ready with TMsvSessionEvent::EMsvServerReady. 
If any functions are called before this, they will fail with KErrNotReady.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aFs A connected file server session 
@param aPriority Active object priority
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
EXPORT_C CMsvSession* CMsvSession::OpenAsyncL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL(EFalse);
	CleanupStack::Pop(self);
	return self;
	}


/**
Creates a session that is used only as an observer. 

The session created with this function should only be used to observe events, and 
not actively use the server. It used for applications such as system monitoring utilities.
Clients to specify a priority that is tuned to their scheduler.

@param aObserver A reference to a session observer, through which the program 
can be notified of important events regarding entries, MTMs, and the Message Server. 
@param aFs A connected file server session 
@param aPriority Active object priority
@return New session object 
@leave KErrNoMemory Not enough memory to create object.
*/ 
EXPORT_C CMsvSession* CMsvSession::OpenAsObserverL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority)
	{
	CMsvSession* self = new(ELeave) CMsvSession(aObserver, aFs, aPriority);
	CleanupStack::PushL(self);
	self->ConstructAsObserverL();
	CleanupStack::Pop(self);
	return self;
	}

CMsvSession::CMsvSession(MMsvSessionObserver& aObserver, TInt aPriority)
: CActive(aPriority), iMainObserver(aObserver), iReceiveEntryEvents(ETrue)
	{
	__DECLARE_NAME(_S("CMsvSession"));
	}

CMsvSession::CMsvSession(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority)
: CActive(aPriority), iFs(aFs), iMainObserver(aObserver), iReceiveEntryEvents(ETrue), iUseSharedFs(ETrue)
	{
	}
	
CMsvSession::~CMsvSession()
	{
	__ASSERT_ALWAYS(iCleanupList==NULL || iCleanupList->Count()==0, PanicServer(EMsvEntriesStillOnCleanupList));
	delete iCleanupList;
	delete iNotifSelection;
	Cancel();
	if (!iUseSharedFs)
		{
		iFs.Close();
		}
	iSession->Close();
	delete iSession;
	delete iObservers;
	delete iMessageFolder;

#ifndef _NO_SESSION_LOGGING_
	if (iLog.LogValid())
		iLog.CloseLog();
	iLog.Close();
#endif
	}

void CMsvSession::ConstructL(TBool aSyncOpening)
	{
	if (!iUseSharedFs)
		{
		// Not using the connected file session, so need to connect	
		User::LeaveIfError(iFs.Connect());
		}
	iSession = new(ELeave) RMsvServerSession;
	
	TInt error = iSession->Connect(iFs);

#ifndef _NO_SESSION_LOGGING_
	CreateSessionLogL();
	Log(_L("CMsvSession::ConstructL - iSession->Connect: %d"),error);
#endif
	User::LeaveIfError(error);

	iCleanupList = new(ELeave) CMsvEntrySelection;
	iCleanupList->SetReserveL(1);

	iNotifSelection = new(ELeave) CMsvEntrySelection;
	CActiveScheduler::Add(this);
	iSession->QueueSessionEventRequest(iChange, iSequenceBuf, iStatus);
	SetActive();
	if (aSyncOpening)
		{
		iSyncStart=KRequestPending;
 		while(iSyncStart == KRequestPending)
 			{
 			User::WaitForRequest(iStatus);
 			HandleNotifyL();
 			}
		GetMessageFolderL();
		}
	}

#ifndef _NO_SESSION_LOGGING_
void CMsvSession::CreateSessionLogL()
	{
	// Connect to flogger
	if (iLog.Connect() == KErrNone)
		{
		// Log name same as the process which creates the session
		TFileName name(RProcess().FileName());
		TInt pos = name.LocateReverse('\\');
		if (pos > 0)
			name = name.Mid(pos + 1);
		pos = name.LocateReverse('.');
		if (pos > 0)
			name = name.Left(pos);
		name += _L(".txt");

		iLog.CreateLog(_L("msgs"), name, EFileLoggingModeOverwrite);
		iLog.SetDateAndTime(EFalse, ETrue); 

		Log(_L("Created new session"));
		}
	}

void CMsvSession::Log(TRefByValue<const TDesC> aFmt, ...)
	{
	if (!iLog.LogValid())
		return;

	VA_LIST list;
	VA_START(list, aFmt);

	// Generate the text
	TBuf<256> buf;
	buf.FormatList(aFmt, list);

	// Write to file
	_LIT(KFormatFile, "CMsvSession %x: %S");
	iLog.WriteFormat(KFormatFile, this, &buf);

#ifndef _NO_SESSION_LOGGING_SERIAL_
	// Write to serial
	_LIT(KFormatSerial, "MSGS: CMsvSession %x: %S");
	RDebug::Print(KFormatSerial, this, &buf);
#endif
	}
#endif

void CMsvSession::ConstructAsObserverL()
	{
	ConstructL(ETrue);
	iSession->SetSessionAsObserver();

#ifndef _NO_SESSION_LOGGING_
	Log(_L("The session is an observer"));
#endif
	}

/**
	Checks that the current drive matches the one on the server
	updates it if it doesn't
	used because sync create might cause the drive to change, which we
	will be notified about, but the client might try and do something
	before that notification comes in.
  **/
void CMsvSession::CheckDrive()
	{
	const TDriveNumber drive = STATIC_CAST(TDriveNumber,iSession->GetMessageDrive());
	if (drive >= KErrNone && iDrive != drive)
		{
		iDrive = drive;
		TChar letter;
		const TInt err = RFs::DriveToChar(drive, letter);
		if (err == KErrNone)
			{
			TPtr path(iMessageFolder->Des());
			TBuf<1> buf;
			buf.Append(letter);
			path.Replace(0, 1, buf);
#ifndef _NO_SESSION_LOGGING_
			Log(_L("Check Drive changed folder to %d, %S"), iDrive,iMessageFolder);
#endif
			}
		}
	}

void CMsvSession::GetMessageFolderL()
//
//
//
	{

	TFileName dir;
	User::LeaveIfError(iSession->GetMessageDirectory(dir));
	iDrive=STATIC_CAST(TDriveNumber,User::LeaveIfError(iSession->GetMessageDrive()));
	// The folder name needs deleting on index reload
	delete iMessageFolder;
	iMessageFolder = NULL;
	iMessageFolder=dir.AllocL();

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Message Folder is %d, %S"), iDrive,&dir);
#endif
	}


void CMsvSession::RunL()
//
// Called when a change request has been completed
//
	{
	HandleNotifyL();
	}

TInt CMsvSession::RunError(TInt aError)
//
// Called on a leave in RunL
//
	{
	TRAPD(
		error, 
		NotifyAllObserversL(
			MMsvSessionObserver::EMsvGeneralError, 
			STATIC_CAST(TAny *,&aError), 
			NULL, 
			NULL
			)
		);

	return error;
	}

void CMsvSession::HandleNotifyL()
	{
	if (iStatus.Int()==KErrServerTerminated)
		{
		// the server has died, so no more changes will be notified
		NotifyAllObserversL(MMsvSessionObserver::EMsvServerTerminated, NULL, NULL, NULL);
		return;
		}
	__ASSERT_DEBUG(iStatus.Int()==KErrNone, PanicServer(EMsvErrorNotification));
	
	TMsvNotifBuffer localBuffer(iChange);
	iNotifySequence = iSequenceBuf();

	// needs to be requeued before notitication, because the main observer might delete the session
	iSession->QueueSessionEventRequest(iChange, iSequenceBuf, iStatus);
	if (iSyncStart != KRequestPending)
 		SetActive();

	DoHandleNotifyL(localBuffer);
	}


void CMsvSession::DoHandleNotifyL(TMsvNotifBuffer& aBuffer)
//
//
//
	{
	TMsvServerChangeNotificationType changeType;
	TMsvPackedChangeNotification changeBuffer(aBuffer);
	
	TInt parameter1, parameter2;
	iNotifSelection->Reset();
	changeBuffer.UnpackL(changeType, *iNotifSelection, parameter1, parameter2);
	
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Received notification %d, sequence %d"), changeType, iNotifySequence);
#endif

	switch (changeType)
		{
		case EMsvEntriesCreated:
			NotifyAllObserversL(MMsvSessionObserver::EMsvEntriesCreated, (TAny*) iNotifSelection, (TAny*) &parameter1, NULL);
			break;
		case EMsvEntriesChanged:
			NotifyAllObserversL(MMsvSessionObserver::EMsvEntriesChanged, (TAny*) iNotifSelection, (TAny*) &parameter1, NULL);
			break;
		case EMsvEntriesDeleted:
			NotifyAllObserversL(MMsvSessionObserver::EMsvEntriesDeleted, (TAny*) iNotifSelection, (TAny*) &parameter1, NULL);
			break;
		case EMsvEntriesMoved:
			NotifyAllObserversL(MMsvSessionObserver::EMsvEntriesMoved, (TAny*) iNotifSelection, (TAny*) &parameter1, (TAny*) & parameter2);
			break;
		case EMsvMtmGroupInstalled:
			NotifyAllObserversL(MMsvSessionObserver::EMsvMtmGroupInstalled, NULL, (TAny*) &parameter1, NULL);
			break;
		case EMsvMtmGroupDeInstalled:
			NotifyAllObserversL(MMsvSessionObserver::EMsvMtmGroupDeInstalled, NULL, (TAny*) &parameter1, NULL);
			break;
		case EMsvCloseSession:
			NotifyAllObserversL(MMsvSessionObserver::EMsvCloseSession, (TAny*) &iNotifSelection->At(0), (TAny*) &parameter1, NULL);
			break;
			//
		case EMsvIndexLoaded:
			if (iSyncStart==KRequestPending)
				{
				iSyncStart = KErrNone;
				}
			else
				{
				GetMessageFolderL(); // if this fails the session is usless, but they never get the server ready event
				NotifyAllObserversL(MMsvSessionObserver::EMsvServerReady, NULL, NULL, NULL);
				}
			break;
			//
		case EMsvIndexFailedToLoad:
			if (iSyncStart==KRequestPending)
				{
				iSyncStart = parameter1;
				}
			else
				NotifyAllObserversL(MMsvSessionObserver::EMsvServerFailedToStart, (TAny*) &parameter1, NULL, NULL);
			break;
			//
		case EMsvMediaChanged:
		case EMsvMediaUnavailable:
		case EMsvMediaAvailable:
		case EMsvMediaIncorrect:
			{
			MMsvSessionObserver::TMsvSessionEvent event = MMsvSessionObserver::EMsvMediaChanged;
			event = (MMsvSessionObserver::TMsvSessionEvent)(event + (changeType - EMsvMediaChanged));
			NotifyAllObserversL(event, &parameter1, &parameter2, NULL);
			break;
			}
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		case EMsvMessageStoreNotSupported:
			NotifyAllObserversL(MMsvSessionObserver::EMsvMessageStoreNotSupported, (TAny*)&parameter1, NULL, NULL);
			break;

		case EMsvMessageStoreCorrupt:
			NotifyAllObserversL(MMsvSessionObserver::EMsvMessageStoreCorrupt, (TAny*)&parameter1, NULL, NULL);
			break;

		case EMsvRefreshMessageView:
			NotifyAllObserversL(MMsvSessionObserver::EMsvRefreshMessageView, (TAny*)&parameter1, NULL, NULL);
			break;

		case EMsvDiskNotAvailable:
			NotifyAllObserversL(MMsvSessionObserver::EMsvDiskNotAvailable, (TAny*)&parameter1, NULL, NULL);
			break;
			
		case EMsvUnableToProcessDiskNotification:
			NotifyAllObserversL(MMsvSessionObserver::EMsvUnableToProcessDiskNotification, (TAny*)&parameter1, NULL, NULL);
			break;
#endif
			//
		default:
			__ASSERT_DEBUG(EFalse, PanicServer(EMsvUnknownChangeType2));
		}
	}




void CMsvSession::DoCancel()
//
//
//
	{
	iSession->CancelSessionEventRequest();
	}


void CMsvSession::NotifyAllObserversL(MMsvSessionObserver::TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)
	{
	if (iObservers)
		{
		TInt count=iObservers->Count();
		while (count--)
			iObservers->At(count)->HandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
		}
	// main observer told at end - so all entries etc can be updated
	iMainObserver.HandleSessionEventL(aEvent,aArg1,aArg2,aArg3);
	}


EXPORT_C void CMsvSession::AddObserverL(MMsvSessionObserver& aObserver)
//
// 
//
/** Registers a new session observer. 

CMsvSession objects can call back observer objects that implement the MMsvSessionObserver::HandleSessionEvent() 
when certain events occur. Any number of observers can be registered.

For details of when observers are called, see TMsvSessionEvent::TMsvEntryEvent.

@param aObserver A reference to an observer to be registered for events 
@leave KErrNoMemory Not enough memory to register the observer */
	{
	if (iObservers==NULL)
		iObservers = new (ELeave) CArrayPtrFlat<MMsvSessionObserver>(KMsvSessionObserverArrayGranularity);
	iObservers->AppendL(&aObserver);

	User::LeaveIfError(iSession->SetReceiveEntryEvents(ETrue));

#ifndef _NO_SESSION_LOGGING_
	Log(_L("Added new observer %d"), iObservers->Count());
#endif
	}

EXPORT_C void CMsvSession::RemoveObserver(MMsvSessionObserver& aObserver)	
//
// 
//
/** Deregisters a previously registered observer. 

@param aObserver A reference to an observer to be unregistered for events */
	{
	__ASSERT_DEBUG(iObservers, PanicServer(EMsvSessionNoObservers));
	if (iObservers)
		{
		TInt count=iObservers->Count();
		while (count--)
			{
			if (iObservers->At(count)==&aObserver)
				{
#ifndef _NO_SESSION_LOGGING_
				Log(_L("Removed observer %d"), count + 1);
#endif
				iObservers->Delete(count);
				if (iObservers->Count()==0)
					{
					delete iObservers;
					iObservers=0;
					if(iReceiveEntryEvents==EFalse)
						// ignoring the returned error as I have no place to report it.
						iSession->SetReceiveEntryEvents(EFalse);
					}
				return;
				}
			}
		__ASSERT_DEBUG(count>=0, PanicServer(EMsvSessionUnknownObserver));
		}
	}


/** Sets whether the session's observer should be notified of events related to 
changes in message server entries.

If the flag is set to true, the session observer will be notified of all events types. 
If it is set to false, the observer will not be notified of events of types EMsvEntriesCreated, 
EMsvEntriesChanged, EMsvEntriesDeleted, or EMsvEntriesMoved. (Event types are enumerated in  
MMsvSessionObserver::TMsvSessionEvent.) 

If a session has additional observers, as set through AddObserver(), then the value of this
flag is ignored, and the observers receive notification of all events.
@param aReceive 
@return Symbian OS error code
@see MMsvSessionObserver::TMsvSessionEvent
*/
EXPORT_C TInt CMsvSession::SetReceiveEntryEvents(TBool aReceive)
	{
	TInt error=KErrNone;
	if(aReceive!=iReceiveEntryEvents) 
		{
		iReceiveEntryEvents=aReceive;
		// don't send a turn off event if we have observers
		if(!(iReceiveEntryEvents==EFalse && iObservers!=NULL))
			error=iSession->SetReceiveEntryEvents(iReceiveEntryEvents);
		}
	return error;
	}

EXPORT_C CMsvOperation* CMsvSession::TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TRequestStatus& aStatus)
//
//
//
/** Passes MTM-specific operations asynchronously to the associated Server-side 
MTM by means of the Message Server. 

It is typically used in the implementation of CBaseMtm::InvokeSyncFunctionL()/InvokeAsyncFunctionL() 
and CBaseMtmUi::InvokeSyncFunctionL()/InvokeAsyncFunctionL(). It is not used 
by message client applications. How the passed aSelection and aParameter parameters 
are used is specific to the operation. 

Calling this function results in the Message Server calling CBaseServerMtm::StartCommandL() 
on the relevant Server-side MTM. If the Server-side MTM is not already loaded, 
then the Message Server loads it. 

The returned CMsvOperation object completes when the operation is complete.

@param aSelection A selection of entries that may be relevant to the operation 

@param aCommandId The command ID. The interpretation of the command is MTM-specific. 

@param aParameter A descriptor containing operation-specific parameters 
@param aStatus The request status to be completed when the operation has finished 

@leave KErrNoMemory Not enough memory 
@return The CMsvOperation object used to control the operation. */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Asynchronous transfer command %d"), aCommandId);
#endif

	// get the mtm
	TMsvId service;
	TMsvEntry entry;
	User::LeaveIfError(iSession->GetEntry(aSelection.At(0), service, entry));

	// create and start the operation
	CMsvEntryOperation* operation = CMsvEntryOperation::NewLC(*this, aStatus);
	operation->iMtm = entry.iMtm;	
	operation->iService = entry.iServiceId;	
	iSession->TransferCommandL(aSelection, aCommandId, aParameter, operation->Id(), operation->iStatus);
	operation->Start();
	CleanupStack::Pop(operation);
	return operation;	
	}

EXPORT_C void CMsvSession::TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TDes8& aProgress)
//
//
//
/** Passes MTM-specific operations synchronously to the associated Server-side 
MTM by means of the Message Server. 

It is typically used in the implementation of CBaseMtm::InvokeSyncFunctionL() 
and CBaseMtmUi::InvokeSyncFunctionL(). It is not used by message client applications. 
How the passed aSelection and aParameter parameters are used is specific to 
the operation. 

Calling this function results in the Message Server calling CBaseServerMtm::StartCommandL() 
on the relevant Server-side MTM. If the Server-side MTM is not already loaded, 
then the Message Server loads it. 

@param aSelection A selection of entries that may be relevant to the operation 

@param aCommandId The command ID. The interpretation of the command is MTM-specific. 

@param aParameter A descriptor containing operation-specific parameters 
@param aProgress Progress information returned from the Message Server 
@leave KErrNoMemory Not enough memory */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Synchronous transfer command %d"), aCommandId);
#endif

	TInt id = OperationId();
	TInt transErr = iSession->TransferCommandL(aSelection, aCommandId, aParameter, id);
	TInt compErr = iSession->OperationCompletion(id, aProgress);
	User::LeaveIfError(transErr);
	if (compErr && compErr != KErrNotFound)
		User::Leave(compErr);
	}

// static 
/** Cleans up an entry that has been added to the cleanup stack using CleanupEntryPushL().

This function defines the clean up function that is called when an entry on the stack
needs cleaning up. 

@param aPtr Pointer to the current message server session 
*/
EXPORT_C void CMsvSession::CleanupEntry(TAny* aPtr)
	{
	CMsvSession* session = STATIC_CAST(CMsvSession*, aPtr);
	session->CleanupEntryDelete();
	}

EXPORT_C void CMsvSession::CleanupEntryPushL(TMsvId aId)
/** Pushes the specified entry ID to the entry cleanup stack.

@param aId The ID of the entry to push onto the entry cleanup stack 
@leave KErrNoMemory The entry will have been pushed onto the entry cleanup stack 
before the leave occurs */
	{
	iCleanupList->AppendL(aId);
	CleanupStack::PushL(TCleanupItem(CleanupEntry, this));
	iCleanupList->SetReserveL(iCleanupList->Count()+1);
	}

EXPORT_C void CMsvSession::CleanupEntryPop(TInt aCount)
/** Pops one or more entries from the entry cleanup stack.

@param aCount The number of entries to pop off the entry cleanup stack */
	{
	iCleanupList->Delete(iCleanupList->Count()-aCount, aCount);
	CleanupStack::Pop(aCount);
	}

void CMsvSession::CleanupEntryDelete()
	{
	TInt pos=iCleanupList->Count()-1;
	RemoveEntry(iCleanupList->At(pos));
	iCleanupList->Delete(pos);
	}

EXPORT_C void CMsvSession::IncPcSyncCountL(const CMsvEntrySelection& aSelection)
/** Increases the PC synchronisation index field for a selection of entries. 

@param aSelection Entries for which to increase the PC synchronisation index 
field */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Increment sync count"));
#endif

	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));
	iSession->ChangeAttributesL(aSelection, KMsvPcSyncCountAttribute, 0);
	}

EXPORT_C void CMsvSession::DecPcSyncCountL(const CMsvEntrySelection& aSelection)
/** Decreases the PC synchronisation index field for a selection of entries. 

If an entry has its Deleted flag set, and this call causes its PC synchronisation 
field to become zero, then the entry is permanently deleted. 

@param aSelection Entries for which to decrease the PC synchronisation index 
field */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Decrement sync count"));
#endif

	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));
	iSession->ChangeAttributesL(aSelection, 0, KMsvPcSyncCountAttribute);
	}

EXPORT_C void CMsvSession::GetChildIdsL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
/** Gets filtered list of children of a specified message entry.

@param aId Message entry of which to get children
@param aFilter Filter by which various message entries can be excluded
@param aSelection On return, a list of message entry IDs */
	{
	__ASSERT_DEBUG(aSelection.Count() == 0, PanicServer(EMsvSelectionNotEmpty));
	iSession->GetChildIdsL(aId, aFilter, aSelection);
	}

EXPORT_C void CMsvSession::ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
/** Provides a quick way to set or clear multiple fields in a selection of entries.

Fields to change are specified using a bitmask of TMsvAttribute values. Possible 
fields that can be changed using this function are:

1. PC synchronisation

2. Visibility flag

3. Read flag

4. In-preparation flag

5. Connected flag

6. New flag

@param aSelection The entries to change
@param aSetAttributes A bitmask of the fields to set
@param aClearAttributes A bitmask of the fields to clear
@see TMsvAttribute */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Change attributes, set %x, clear %x"), aSetAttributes, aClearAttributes);
#endif

	__ASSERT_DEBUG(aSelection.Count(), PanicServer(EMsvEmptySelection));
	iSession->ChangeAttributesL(aSelection, aSetAttributes, aClearAttributes);
	}



EXPORT_C CMsvOperation* CMsvSession::ChangeDriveL(TInt aDrive, TRequestStatus& aStatus)
//
// Creates a child of the context
//
/** Changes the Message Server current drive to the specified drive.

Progress information is provided by a TMsvIndexLoadProgress object.

If an error occurs, the index is unchanged.

@param aDrive The drive to which to move the Message Server index, specified 
by a TDriveNumber value
@param aStatus Asynchronous completion status 
@leave KErrServerBusy Cannot change drive because there are outstanding Message 
Server operations.
@return Asynchronous operation used to control the move. The operation's MTM 
value is set to KUidMsvServerMtm, which can be used to distinguish it from 
a local operation.
@see TDriveNumber
@see TMsvIndexLoadProgress */
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Change drive to %d"), aDrive);
#endif

	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(*this, aStatus);
	CleanupStack::PushL(operation);

	operation->iMtm = KUidMsvServerMtm; // A different MTM type (like local) to identify this type of operation
	iSession->ChangeDriveL(aDrive, operation->Id(), operation->iStatus);
	operation->Start();

	CleanupStack::Pop(operation); 
	return operation;
	}


	

/** Copies the Message Store to the specified drive.

Progress information is provided by a TMsvCopyProgress object.

If an error occurs, the Message Store is unchanged.

@param aDrive The drive to which to copy the Message Store, specified by a TDriveUnit value
@param aStatus Asynchronous completion status 
@leave KErrServerBusy Cannot copy store because there are outstanding Message Server operations.
@return Asynchronous operation used to control the move. The operation's MTM value is set to 
KUidMsvServerMtm, which can be used to distinguish it from a local operation.
@see TDriveUnit
@see TMsvCopyDeleteProgress */

EXPORT_C CMsvOperation* CMsvSession::CopyStoreL(const TDriveUnit& aDrive, TRequestStatus& aStatus)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Copy Message Store to %d"), (TInt)aDrive);
#endif

	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(*this, aStatus);
	CleanupStack::PushL(operation);

	operation->iMtm = KUidMsvServerMtm; // A different MTM type (like local) to identify this type of operation
		
	iSession->CopyStoreL(aDrive, operation->Id(), operation->iStatus);
	operation->Start();

	CleanupStack::Pop(operation);  
	return operation;
	}



/** Deletes the Message Store to the specified drive.

Progress information is provided by a TMsvDeleteProgress object.

If an error occurs, the Message Store is unchanged.

@param aDrive The drive to which to copy the Message Store, specified by a TDriveUnit value
@param aStatus Asynchronous completion status 
@leave KErrServerBusy Cannot copy store because there are outstanding Message Server operations.
@return Asynchronous operation used to control the move. The operation's MTM value is set to 
KUidMsvServerMtm, which can be used to distinguish it from a local operation.
@see TDriveUnit
@see TMsvCopyDeleteProgress */

EXPORT_C CMsvOperation* CMsvSession::DeleteStoreL(const TDriveUnit& aDrive, TRequestStatus& aStatus)
	{
#ifndef _NO_SESSION_LOGGING_
	Log(_L("Copy Message Store to %d"), (TInt)aDrive);
#endif

	CMsvEntryOperation* operation = CMsvEntryOperation::NewL(*this, aStatus);
	CleanupStack::PushL(operation);

	operation->iMtm = KUidMsvServerMtm; // A different MTM type (like local) to identify this type of operation
		
	iSession->DeleteStoreL(aDrive, operation->Id(), operation->iStatus);
	operation->Start();

	CleanupStack::Pop(operation);  
	return operation;
	}



EXPORT_C TInt CMsvSession::OutstandingOperationsL()
//
//
//
/** Gets the number of outstanding operations.

@return The number of outstanding operations */
	{
	return iSession->OutstandingOperationsL();
	}


/** Gets the additional security capabilities required by a given MTM, over and
above those required by the message server itself.

@param aMtmTypeUid The type UID of the MTM being queried
@param aCapSet A reference to a TCapabilitySet in which the security capabilities 
will be returned. Any existing capabilities will be removed. */

EXPORT_C void CMsvSession::GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCapSet) const
	{
	iSession->GetMtmRequiredCapabilitiesL(aMtmTypeUid, aCapSet);
	}

MMsvStoreManager& CMsvSession::StoreManager()
	{
	MMsvStoreManager& storeManager = *iSession;
	return storeManager;
	}

EXPORT_C RMsvServerSession& CMsvSession::Session() 
	{
	return *iSession;
	};

EXPORT_C RFs& CMsvSession::FileSession() 
/** Allows a Server-side MTM to access the file session handle created by the Message 
Server. 

This is preferable, as more efficient, to creating another handle.

@return File session handle */
	{
	return iFs;			
	}

EXPORT_C TInt CMsvSession::OperationId()
	{
	return iOperationId++;
	}

EXPORT_C TInt CMsvSession::StopService(TMsvId aServiceId)
/** Stops any operations that a Server-side MTM for the specified service is running, 
and then unloads the Server-side MTM. 

The current operation and any queued operations are cancelled.

@param aServiceId The ID of the service to stop 
@return KErrNone - success; */
	{
	return iSession->StopService(aServiceId);
	}

EXPORT_C TBool CMsvSession::ServiceActive(TMsvId aServiceId)
/** Tests whether a Server-side MTM for a particular service is loaded by the Message 
Server.

The Server-side MTM does not have to be executing a command  -  it may be 
waiting for another command.

@param aServiceId The ID of the relevant service 
@return ETrue if the Server-side MTM for the service is loaded, otherwise EFalse */
	{
	return iSession->ServiceActive(aServiceId);
	}

EXPORT_C TInt CMsvSession::ServiceProgress(TMsvId aServiceId, TDes8& aProgress)
/** Gets the current progress information from the Server-side MTM for the specified 
service.

It is typically used by User Interface MTMs. The format of the progress information 
returned in the aProgress buffer is MTM-specific.

Calling this function results in the Message Server calling CBaseServerMtm::Progress() 
on the relevant Server-side MTM. 

Note that the progress information is independent of which message client 
application started the current operation. 

@param aServiceId The ID of the service from which to get the progress information 

@param aProgress On return, a descriptor holding progress information. It is 
the caller's responsibility to ensure the descriptor is large enough for this 
information. 
@return KErrNone - success; KErrNotFound - The service is not active (the relevant Server-side MTM is not loaded 
by the Message Server); KErrOverflow - The descriptor was too small for the progress information */
	{
	return iSession->ServiceProgress(aServiceId, aProgress);
	}

EXPORT_C void CMsvSession::CloseMessageServer()
/** Closes down the Message Server.

Normal clients should not use this function, as it will affect other message 
clients.

The function results in the session sending a shutdown session notification 
(TMsvSessionEvent::EMsvCloseSession) to all current sessions. The Message 
Server closes when all sessions have been closed. */
	{
	iSession->CloseMessageServer();
	}

EXPORT_C void CMsvSession::RemoveEntry(TMsvId aId)
/** Deletes the specified entry from the Message Server. 

The call is guaranteed not to fail. If the entry cannot be deleted immediately, 
it will be deleted later. This call should only be used in preference to the 
normal deleting mechanism when no error reporting is required, typically in 
a destructor.

@param aId The ID of the entry to remove */
	{
	iSession->RemoveEntry(aId);
	}

EXPORT_C TInt CMsvSession::InstallMtmGroup(const TDesC& aFullName)
/** Installs a new group of MTMs. 

It is used by specialised MTM-installation programs. 

@param aFullName The full path name of the MTM group file 
@return KErrNone - success; KErrAlreadyExists - MTM already installed */
	{
	return iSession->InstallMtmGroup(aFullName);
	}

EXPORT_C TInt CMsvSession::DeInstallMtmGroup(const TDesC& aFullName)
/** Removes an installed MTM. 

It is used by specialised MTM-deinstallation programs. 

@param aFullName The full path name of the MTM group file 
@return KErrNone - success; KErrInUse - The MTM is currently being used */
	{
	return iSession->DeInstallMtmGroup(aFullName);
	}

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)

EXPORT_C CMsvEntry* CMsvSession::GetEntryL(TMsvId aId, TBool aChildrenOfAvailableDrives)
/** Accesses the entry with the specified ID.
Children from all available drives of the specified entry will be fetched depending
on the value of aChildrenOfAvailableDrives.

If a client is unaware of the entries that exist, it can set aId to KMsvRootIndexEntryId 
to obtain the root entry, from where all other entries can be obtained.

The CMsvEntry object must be deleted by the client when it is no longer required. 

@return A new entry object      
@param aId  The ID of the entry to access
@param aChildrenOfAvailableDrives Indicates whether children from all available drives are to
be fetched during construction of the entry. However, a value of true is valid only
if aMsvId is one among TMsvId's of Inbox, Outbox, Drafts, Sent or Deleted folders.

@leave KErrNotFound The requested entry does not exist
@leave KErrArgument aChildrenOfAvailableDrives is set to true and the requested entry is not
one of the standard folders, i.e. Inbox, Outbox, Drafts, Sent or Deleted. 
*/
    {
    return CMsvEntry::NewL(*this, aId, TMsvSelectionOrdering(), aChildrenOfAvailableDrives);
    }

#endif

EXPORT_C CMsvEntry* CMsvSession::GetEntryL(TMsvId aId)
/** Accesses the entry with the specified ID.

If a client is unaware of the entries that exist, it can set aId to KMsvRootIndexEntryId 
to obtain the root entry, from where all other entries can be obtained.

The CMsvEntry object must be deleted by the client when it is no longer required. 

@return	A new entry object 		
@param aId  The ID of the entry to access
@leave KErrNotFound The requested entry does not exist
*/
	{
	return CMsvEntry::NewL(*this, aId, TMsvSelectionOrdering());
	}
 
EXPORT_C TInt CMsvSession::GetEntry(TMsvId aId, TMsvId& aService, TMsvEntry& aEntry)
/** Gets the index entry for the specified entry ID.

@param aId The ID of the entry to access 
@param aService On return, the ID of the service that owns the entry
@param aEntry On return, the index entry
@return KErrNone if successful, otherwise one of the system-wide error codes */
	{
	return iSession->GetEntry(aId, aService, aEntry);
	}

/** Checks a flag in the message server that is set when the server
	deletes a corrupt index file. Then clears the flag.
	
	It is intended to allow the message centre UI to check whether
	a corrupt index has been deleted on startup. If it has the UI
	may inform the user about the loss of their messages.
	    
    @return ETrue if a corrupt index flag was present indicating that
    a corrupt index file has been deleted since the last time this
    function was called.
    */
EXPORT_C TBool CMsvSession::GetAndClearIndexCorruptFlagL()
	{
	return iSession->GetAndClearIndexCorruptFlagL();
	}	
	
	
EXPORT_C TDriveUnit CMsvSession::CurrentDriveL()
	{
	TInt drive = iSession->GetMessageDrive();
	User::LeaveIfError(drive); // This can leave if messaging server is killed 
	return TDriveUnit(drive);
	}
	
EXPORT_C TBool CMsvSession::DriveContainsStoreL(TDriveUnit aDrive)
	{
	return iSession->DriveContainsStoreL(aDrive);
	}

/** Checks to see if the currently selected drive contains the correct mail store.

@return ETrue if the same drive is mounted. otherwise returns EFalse
@capability None
*/
EXPORT_C TBool CMsvSession::MessageStoreDrivePresentL()
	{		
	return iSession->MessageStoreDrivePresentL();
	}

/**
Get the ID of the access point currently in use by the server side MTM for the
given service ID

@param aServiceID Service ID
@param aAccessPointId If return code is KErrNone, this will store the access point ID

@return KErrNone if successful, KErrNotSupported if the server side MTM does
        not support this operation, KErrNotFound if the server side MTM is
        not connected at the time of the request, or one of the other system
        wide error codes.
*/
EXPORT_C TInt CMsvSession::ServiceAccessPointId(TMsvId aServiceId, TUint32& aAccessPointId)
	{
	TNonOperationMtmDataAccessPointIdBuffer mtmDataAccessPointIdBuffer;

	TInt err = iSession->GetNonOperationMtmData(aServiceId, EMtmDataAccessPointId, mtmDataAccessPointIdBuffer);

	if (err == KErrNone)
		{
		TNonOperationMtmDataAccessPointId mtmDataAccessPointId = mtmDataAccessPointIdBuffer();
		aAccessPointId = mtmDataAccessPointId.iAccessPointId;
		}

	return err;
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 *
 * @return TDriveNumber Drive number of the current drive
 *						 in the preferred drive list.
 * @return TUint Priority of the drive.
 *
 * The function returns the current drive of the Message Server and
 * its priority in the preferred drive list.
 *
 @publishedAll
 @released
 */ 
EXPORT_C void CMsvSession::CurrentDriveInfoL(TDriveNumber& aDriveNumber, TUint& aPriority)
	{
	iSession->CurrentDriveInfoL(aDriveNumber, aPriority);
	}



/**
 *
 * @return RArray<TDriveNumber> List of drive numbers present in 
 *         the preferred drive list arranged in descending order
 *         of their priority.
 *
 * Returns the entire preferred drive list with drive numbers arranged
 * in descending order of their priority.
 * 
 @publishedAll
 @released
 */
EXPORT_C void CMsvSession::DriveListL(RArray<TDriveNumber>& aDriveList)
	{
	iSession->DriveListL(aDriveList);
	}



/**
 *
 * @return RArray<TDriveNumber> List of drive numbers of available
 *         drives present in the preferred drive list arranged in 
 *         descending order of their priority.
 *
 * Returns a list of available drives in the preferred drive list.
 * NOTE: A drive number is available only if it has a valid 
 * (readable and of correct version) message store in it.
 *
 @publishedAll
 @released
 */
EXPORT_C void CMsvSession::AvailableDriveListL(RArray<TDriveNumber>& aDriveList)
	{
	iSession->AvailableDriveListL(aDriveList);
	}




/**
 *
 * @param  TDriveNumber Drive number of the drive
 *						 to be added.
 * @param TUint Priority of the drive.
 * @return TUint New priority of the added drive.
 *
 * The function adds the passed drive in the preferred drive list.
 * The location of the drive in the list is specified by the priority 
 * of the drive as specified in the second argument. 
 *
 * - If the priority value of the drive is more than the number of 
 * elements in the preferred drive list, the function appends the drive 
 * at the end of the list. The new priority of the drive is returned as 
 * the second argument.
 * - If the priority of the new drive is higher than the current drive 
 * of message server, and if the new drive has a media in it, the message 
 * server will perform implicit changeDrive and sends a notification to 
 * all registered client processes about the change.
 * - If the drive does not contain a valid version of message store the 
 * function updates the drive status to EMsvMessageStoreNotSupported and 
 * sends appropriate notification to the client process. The drive's content 
 * will not be visible to any of the client processes.
 * - If the message store/message index database in the media is corrupt, 
 * the server will delete the message store and create a fresh message store
 * and related files in the media.
 * - If the number of drive already present in the preferred list is eight,
 * the function will return appropriate error message. 
 * - In all cases mentioned above, if the drive is successfully added to the 
 * preferred drive list the function will also update the central repository 
 * with the new preferred drive list.
 *
 @publishedAll
 @released
 */ 
EXPORT_C void CMsvSession::AddDriveL(TDriveNumber aDriveNumber, TUint& aPriority)
	{
	iSession->AddDriveL(aDriveNumber, aPriority);
	}



/**
 *
 * @param  TDriveNumber Drive number of the drive
 *						 to be removed.
 *
 * The function removes the drive from the preferred
 * drive list. If the current drive is being removed, 
 * the next available drive in the preferred drive list 
 * becomes the current drive. The device internal drive 
 * cannot be removed from the preferred drive list. 
 * Appropriate notification will be sent to the client 
 * process if there is a change in the current drive. 
 * The function will also update the central repository 
 * accordingly.
 *
 @publishedAll
 @released
 */ 
 EXPORT_C void CMsvSession::RemoveDriveL(TDriveNumber aDriveNumber)
 	{
 	iSession->RemoveDriveL(aDriveNumber);
 	}
 	
 	
 	
 /**
 *
 * @param  TDriveNumber Drive number of the drive
 *						 to be updated.
 * @param  TUint Priority of the drive.
 * @return TUint New priority of the drive.
 *
 * The function updates the priority of the drive 
 * already present in the preferred drive list. If 
 * the priority being mentioned is greater than the 
 * number of drives present in the list, the drive 
 * will be added at the end of the preferred drive list. 
 * The function will then return the new priority of 
 * the drive. If updating a priority makes a non-current 
 * drive the current drive or vice-versa, the function 
 * will implicitly perform ChangeDrive() and send the 
 * notification to all registered clients. It will return
 * error if the said drive is not already present in the 
 * preferred drive list.
 *
 @publishedAll
 @released
 */ 
EXPORT_C void CMsvSession::UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aPriority)
	{
	iSession->UpdateDrivePriorityL(aDriveNumber, aPriority);
	}




/**
 *
 * @param TMsvId ID of the parent entry.
 * @param CMsvEntryFilter Filter by which various message entries can be excluded.
 * @return aSelection:     A list of message entry IDs
 *
 * @capability None Only children that the client owns are returned. 
 * @capability ReadUserData All children of the entry are returned
 *
 * Gets the children of a parent entry specified as first argument.
 * If the passed parent is a standard folder the function will fetch 
 * entries from all drives currently present in the preferred drive list.
 *
 @publishedAll
 @released
 */
EXPORT_C void CMsvSession::GetChildIdsAllL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
	{
	__ASSERT_DEBUG(aSelection.Count() == 0, PanicServer(EMsvSelectionNotEmpty));
	iSession->GetChildIdsAllL(aId, aFilter, aSelection);	
	}
	
	

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
EXPORT_C void CMsvSession::ResetRepositoryL()
	{
	iSession->ResetRepositoryL();
	}
	
EXPORT_C void CMsvSession::PrintCache()
	{
	iSession->PrintCache();
	}

#endif		// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)

#endif  	// #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)


#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/***************************************Converter API's***************************/

/**
Retrieves a list of drives with unsupported message store. A store is rendered 
unsupported if it has message store older than that on the phone internal memory.

@param aDriveList Reference to the list that is filled with drives having
unsupported message store.
@return None

@publishedAll
@released
*/	
EXPORT_C void CMsvSession::GetConvertibleDriveListL(RArray<TDriveNumber>& aDriveList)
	{
	iSession->GetConvertibleDriveListL(aDriveList);	
	}

/**
Converts an older version of message store to the current version as seen by message server.
This is an asynchronous API and performs conversion asynchronously so that the caller does not have
to wait for conversion is complete. This API can be used to issue conversion resquest for a single
drive. The conversion starts immediately os the drive is queued if store converter is busy performing 
another conversion.

@param aDrive drive number.
@param aRequestStatus Indicates the completion status of a request made to a service provider.
@return None

@publishedAll
@released
*/	
EXPORT_C void CMsvSession::ConvertMessageStore(TDriveNumber aDrive,TRequestStatus& aStatus)
	{
	iSession->ConvertMessageStore(aDrive,aStatus);
	}

/**

@param aDrive Drive number specifying the drive for which conversion status is required..
@return None

@publishedAll
@released
*/	
EXPORT_C void CMsvSession::GetConversionStatusL(TDriveNumber aDrive)
	{
	iSession->GetConversionStatusL(aDrive);
	}

/**
Cancels conversion on the specified drive. This API allows cancellation of only one drive
at a time.

@param aDrive drive number.
@return TInt System wide error codes
@publishedAll
@released
*/	
EXPORT_C TInt CMsvSession::CancelConversion(TDriveNumber aDrive)
	{
	return iSession->CancelConversion(aDrive);
	}

#endif			// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

//**********************************
// McliUtils
//**********************************

/** Gets the local progress information from a messaging operation object. 

@param aOperation Operation from which to get progress information
@return Local progress information for aOperation
@panic MSGS 256 aOperation is not a local operation
*/
EXPORT_C TMsvLocalOperationProgress McliUtils::GetLocalProgressL(CMsvOperation& aOperation)
    {
    __ASSERT_ALWAYS(aOperation.Mtm()==KUidMsvLocalServiceMtm, PanicServer(EMsvNotLocalOperation));
    TPckgBuf<TMsvLocalOperationProgress> progressPack;
    progressPack.Copy(aOperation.ProgressL());
    return progressPack();
    }

/** Gets the final local progress information from a messaging operation object. 

@param aOperation Operation from which to get progress information
@return Local progress information for aOperation
@panic MSGS 256 aOperation is not a local operation
*/
EXPORT_C TMsvLocalOperationProgress McliUtils::GetFinalLocalProgress(CMsvOperation& aOperation)
    {
    __ASSERT_ALWAYS(aOperation.Mtm()==KUidMsvLocalServiceMtm, PanicServer(EMsvNotLocalOperation));
    TPckgBuf<TMsvLocalOperationProgress> progressPack;
    progressPack.Copy(aOperation.FinalProgress());
    return progressPack();
    }

/** Gets the progress error from a messaging operation object. 

@param aOperation Operation from which to get progress information
@return error of the progress information
*/
EXPORT_C TInt McliUtils::GetProgressErrorL(CMsvOperation& aOperation)
	{
	TMsvSystemProgress systemProgress;
	if (aOperation.Mtm() == KUidMsvLocalServiceMtm)
		{
		TMsvLocalOperationProgress localProgress = GetLocalProgressL(aOperation);
		systemProgress.iErrorCode = localProgress.iError; 
		}
	else
		{
		User::LeaveIfError(aOperation.SystemProgress(systemProgress));
		}
	return systemProgress.iErrorCode;
	}

/** Gets the progress id from a messaging operation object. 

@param aOperation Operation from which to get progress information
@return id of the progress information
*/
EXPORT_C TMsvId McliUtils::GetProgressIdL(CMsvOperation& aOperation)
	{
	TMsvSystemProgress systemProgress;
	if (aOperation.Mtm() == KUidMsvLocalServiceMtm)
		{
		TMsvLocalOperationProgress localProgress = GetLocalProgressL(aOperation);
		systemProgress.iId = localProgress.iId; 
		}
	else
		{
		User::LeaveIfError(aOperation.SystemProgress(systemProgress));
		}
	return systemProgress.iId;
	}
