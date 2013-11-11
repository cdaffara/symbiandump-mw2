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
// MSVAPI.H
//
/**
 * @file 
 * @publishedAll
 * @released
 */

#ifdef _DEBUG
#undef _NO_SESSION_LOGGING_
#endif

#ifndef __MSVAPI_H__
#define __MSVAPI_H__

#include <e32base.h>
#include <s32file.h>

#ifdef _MSVAPI_DONT_INCLUDE_FLOGGER_
	#ifndef _NO_SESSION_LOGGING_
		#define _NO_SESSION_LOGGING_
	#endif
#else
	#include <flogger.h>
#endif

#include <msvstd.h>
#include <msvstore.h>
#include <msvipc.h>
#include <msvarray.h>
#include <mclient.h>
#include <tmsvsystemprogress.h>

// Forward declarations
class CMsvSession;
class CMsvEntry;
class CBaseMtm;
class CMsvClientEntry;
class CMsvEntryArray;
class CMsvServer;
class CMsvServerEntry;
class CMsvSearchSortOperation;



// Remove these to remove client logging
//#define _NO_SESSION_LOGGING_
#define _NO_SESSION_LOGGING_SERIAL_

// Single export used to start the message server
IMPORT_C TInt StartMessageServer(TAny*);

// Create Message Server and return a server entry owned by caller
// This is all that is required to test server Mtm's
IMPORT_C CServer2* CreateMessageServerL(CMsvServerEntry*& aServerEntry);

//**********************************
// CMsvOperation
//**********************************
//
// Abstract base class for operations controlling asynchronous functions
//

class CMsvOperation : public CActive
/** Defines an interface for use by objects which control asynchronous commands 
in the messaging system. 

Such objects are returned by CMsvEntry and User Interface MTM functions that 
complete asynchronously. The interface allows clients to:

1. get progress information about the operation

2. cancel the operation

3. be signalled asynchronously when the operation completes; a client passes 
in a TRequestStatus of a suitable active object for this purpose

The client only needs to understand the CMsvOperation interface, not the concrete 
implementation used. 

Writing derived classes:

As the base class interface for User Interface MTMs, CBaseUiMtm, defines functions 
that return CMsvOperation objects for control of asynchronous operations, 
implementers of these MTM components are required to provide suitable derived 
classes. For example, if CBaseUiMtm::EditL() is implemented to provide message 
editing, a CMsvOperation -derived class would be provided that completes when 
the editing operation is complete.

Concrete derived classes must provide implementations of the pure virtual 
DoCancel() and RunL() functions defined by CActive. DoCancel() should be provided 
in the normal way to cancel the operation. RunL() should, in addition to any 
other required functionality, always end by signalling the client that the 
operation is complete with a suitable completion code. 

@publishedAll
@released
*/
	{
public:
	IMPORT_C CMsvOperation(CMsvSession& aMsvSession, TInt aPriority, TRequestStatus& aObserverRequestStatus);
	IMPORT_C ~CMsvOperation();
	/** Gets information on the progress of the operation. 
	
	All operations on local entries share the same progress information format, 
	which defined by TMsvLocalOperationProgress. 
	
	For MTM-specific operations, progress information can be extracted by calling 
	CBaseUiMtm::GetProgress() or displayed by CBaseUiMtm::DisplayProgressSummary() 
	on the appropriate User Interface MTM.
	
	Requirements:
	
	Derived classes should implement this function so that the client can check 
	on the progress of the operation. The function should share an understanding 
	of the format of the buffer with the implementations of CBaseUiMtm::GetProgress() 
	and CBaseUiMtm::DisplayProgressSummary() in the User Interface MTM. The arguments 
	of CBaseUiMtm::GetProgress() show some information that should be included 
	where appropriate.
	
	@leave KErrNotReady The operation has not yet been started: it has been queued 
	for execution in the Message Server 
	@return Descriptor holding progress information. The maximum buffer size should 
	be KMsvProgressBufferLength (defined in msvipc.h). */
	virtual const TDesC8& ProgressL()=0;
	IMPORT_C virtual const TDesC8& FinalProgress();
	IMPORT_C TInt SystemProgress(TMsvSystemProgress& aOutSysProg);
	IMPORT_C virtual TUid Mtm() const;
	//
	inline TMsvOp Id() const;
	inline TMsvId Service() const;
	//
protected:
	IMPORT_C TInt Extension_(TUint aExtensionId, TAny *&a0, TAny *a1);

protected:
	/** The ID of the service that is associated with this operation. 
	
	Usually, the derived class constructor will contain a suitable argument to 
	allow the caller to set this.
	
	@see Service() */
	TMsvId iService;
	/** The UID of the MTM associated with the operation. 
	
	The appropriate value should be set by the derived class constructor.
	
	@see Mtm() */
	TUid iMtm;
	/** Request status of the operation observer. 
	
	This is the aObserverRequestStatus passed in the constructor. */
	TRequestStatus& iObserverRequestStatus;
	/** Message Server session used by object. This is set by the constructor. */
	CMsvSession& iMsvSession;
	
private:
	TMsvOp iId;
	};


//**********************************
// CMsvOperationWait
//**********************************
//
// Allows a synchronous wait on a operation
//

class CMsvOperationWait: public CActive
/** Utility class used to wait until an asynchronous messaging operation
has completed. 

Note that CMsvOperationActiveSchedulerWait is simpler to use, and should be used in 
preference to this class.

To use the class:

1. Create a new CMsvOperationWait object

2. Call the messaging function that returns the asynchronous operation. 
The operation observer parameter should be the iStatus word of the CMsvOperationWait 
object. This means that the CMsvOperationWait object will be signalled when
the operation completes.

3. Call the CMsvOperationWait object's Start() function. This sets the object
to be active.

4. Call CActiveScheduler::Start(). This starts a nested active scheduler. The 
program will then wait until this active scheduler is stopped. The CMsvOperationWait 
object stops the scheduler when the operation completes, allowing the program to 
continue. 

These steps cause the program to wait until the operation completes.

@code
     CMsvOperationWait* waiter=CMsvOperationWait::NewLC();
     CMsvOperation* op = function_returning_opLC(waiter->iStatus);
     waiter->Start();
     CActiveScheduler::Start();
     CleanupStack::PopAndDestroy(2); // op, waiter
@endcode

@see CActiveScheduler
@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvOperationWait* NewLC(TInt aPriority=EPriorityStandard);
	IMPORT_C ~CMsvOperationWait();
	IMPORT_C void Start();
protected:
	CMsvOperationWait(TInt aPriority);
	void RunL();
	void DoCancel();
	};


//**********************************
// CMsvOperationActiveSchedulerWait
//**********************************
//
// Allows a synchronous wait on a operation using CActiveSchedulerWait class
// This class should be used in preference to CMsvOperationWait
// DOES NOT require an explicit call to CActiveScheduler::Start()--CMsvOperationActiveSchedulerWait::Start()
// effectively encapsulates this functionality
//

class CMsvOperationActiveSchedulerWait: public CActive
/** Utility class used to wait until an asynchronous messaging operation
has completed. 

To use the class:

1. Create a new CMsvOperationActiveSchedulerWait object

2. Call the messaging function that returns the asynchronous operation. 
The operation observer parameter should be the iStatus word of the CMsvOperationActiveSchedulerWait 
object. This means that the CMsvOperationActiveSchedulerWait object will be signalled when
the operation completes.

3. Call the CMsvOperationActiveSchedulerWait object's Start() function. This sets the object
to be active.

These steps cause the program to wait until the operation completes.

@code
     CMsvOperationActiveSchedulerWait* waiter=CMsvOperationActiveSchedulerWait::NewLC();
     CMsvOperation* op = function_returning_opLC(waiter->iStatus);
     waiter->Start();

     CleanupStack::PopAndDestroy(2); // op, waiter
@endcode

@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvOperationActiveSchedulerWait* NewLC(TInt aPriority=EPriorityStandard);
	IMPORT_C ~CMsvOperationActiveSchedulerWait();
	IMPORT_C void Start();
private:
	CMsvOperationActiveSchedulerWait(TInt aPriority);
	void RunL();
	void DoCancel();
private:
	CActiveSchedulerWait iActiveSchedulerWait;
	};

//**********************************
// CMsvCompletedOperation
//**********************************
//
// An operation which is already completed on construction
//

class CMsvCompletedOperation : public CMsvOperation
/** Utility to create a messaging operation object for an operation that has already 
completed. 

This utility is useful, for example, for writers of MTMs, where the API requires that an 
asynchronous operation is returned, but where in reality, the operation has performed
synchronously. It allows the program to construct an operation object for which 
the operation is already completed, with the progress information and error code set.   

@publishedAll
@released
*/
	{
public:
	IMPORT_C static CMsvCompletedOperation* NewL(CMsvSession& aMsvSession, TUid aMtm, const TDesC8& aProgress, TMsvId aService, TRequestStatus& aObserverRequestStatus, TInt aErr=KErrNone);
	IMPORT_C ~CMsvCompletedOperation();
	//
	const TDesC8& ProgressL();
	const TDesC8& FinalProgress();
private:
	CMsvCompletedOperation(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus);
	void ConstructL(TUid aMtm, TInt aError, const TDesC8& aProgress, TMsvId aService);
	// from CActive
	void DoCancel();
	void RunL();
	//
private:
	HBufC8* iProgress;
	};

//**********************************
// CMsvSendOperation
//**********************************
//
// A send operation which returns a standard progress (used by SendAs2)
//

/** Utility to create an operation containing a sending operation which may be used
to retrieve a standard progress structure.

This class is intended to be derived from. The derived class must define the TranslateProgress()
member function. This function is responsible for translating the native progress of the given
mtm into the standard progress structure, CMsvSendOperation::TSendOperationProgress.

@publishedAll
@released
*/
class CMsvSendOperation : public CMsvOperation
	{
public:
	/** Send progress state.

	@see TSendOperationProgress
	*/
	enum TSendOperationState
		{
		ESendStateInPreparation,
		ESendStateWaitingToSend,
		ESendStateConnecting,
		ESendStateSending,
		ESendStateDone,
		ESendStateFailed
		};
	/** Send progress.
	
	Structure holding send progress. For mtms unable to provide detailed progress 
	information, iProgressMax and iProgress must be zeroed. This will then allow
	the client to check for this case and display a busy status dialog as opposed to
	a progress dialog. If the mtm is capable of detailed progress, iProgressMax 
	should be set to the total number of units to be sent and iProgress the number
	of units sent.

	@see TSendOperationState
	*/
	class TSendOperationProgress
		{
	public:
		TSendOperationState iState;
		TInt 				iError;
		TInt 				iProgressMax;
		TInt 				iProgress;	
		};

public:
	IMPORT_C virtual ~CMsvSendOperation();
	// methods from CMsvOperation
	IMPORT_C virtual const TDesC8& ProgressL();
	IMPORT_C virtual const TDesC8& FinalProgress();
	IMPORT_C void Start(CMsvOperation* aOperation);
protected:
	IMPORT_C CMsvSendOperation(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus);
private:	// methods from CActive
	IMPORT_C virtual void DoCancel();
	IMPORT_C virtual void RunL();
private:
	/** Translates the progress from sending operation's progress into TSendOperationProgress. 
	
	This member function must be defined by derived classes. Must not leave.
	@see TSendOperationProgress 
	@return Descriptor holding progress information.
	*/
	virtual const TDesC8& TranslateProgress(const TDesC8& aProgress)=0;
	void Complete(TInt anError);
protected:
	/** The progress that is associated with this operation. 

	The TranslateProgress defined by derived classes must populate and return
	this progress structure.
	@see TranslateProgress() 
	@see TSendOperationProgress */
	TPckgBuf<TSendOperationProgress> iProgress;
	/** The sending operation. 
	
	Assigned when ConstructL is called
	*/
	CMsvOperation* iOperation;
	};
	
/** Package buffer for a TSendOperationProgress
*/
typedef TPckgBuf<CMsvSendOperation::TSendOperationProgress> TMsvSendOperationProgress;


//**********************************
// MMsvSessionObserver
//**********************************
//
// 
//

class MMsvSessionObserver
/** Provides the interface for notification of events from a Message Server session. 


The types of event are given in the enumeration TMsvSessionEvent. Clients 
must provide an object that implements the interface, and set it to be notified 
through CMsvSession::OpenSyncL() or CMsvSession::OpenASyncL(). Additional 
observers can also be added and removed through CMsvSession.

@see CMsvSession::AddObserverL()
@see CMsvSession::RemoveObserver() 
@publishedAll
@released
*/
	{
public:
	/** Session event type.

	@see EMsvMediaUnavailable
	@see TDriveNumber 
	*/
	enum TMsvSessionEvent 
							{ 
	/** One or more entries have been created.
	
	aArg1 is a CMsvEntrySelection of the new entries. aArg2 is the TMsvId of the 
	parent entry. */
							EMsvEntriesCreated,
	/** One or more index entries have been changed. 
	
	aArg1 is a CMsvEntrySelection of the index entries. aArg2 is the TMsvId of 
	the parent entry. */
							EMsvEntriesChanged,
	/** One or more entries have been deleted.
	
	aArg1 is a CMsvEntrySelection containing the IDs of the deleted entries. aArg2 
	is the TMsvId of the parent entry. */
							EMsvEntriesDeleted,
	/** One or more entries have been moved.
	
	aArg1 is a CMsvEntrySelection containing the IDs of the moved entries. aArg2 
	is the TMsvId of the new parent. aArg3 is the TMsvId of the old parent entry. */
							EMsvEntriesMoved,
	/** A new MTM has been installed. 
	
	aArg2 points to a TUid for the new MTM. */
							EMsvMtmGroupInstalled,
	/** A MTM has been uninstalled. 
	
	aArg2 points to a TUid of the removed MTM. */
							EMsvMtmGroupDeInstalled,
	/** Something has happening in the server, but this client was unable to retrieve 
	the information.
	
	aArg1 points to the error code. */
							EMsvGeneralError,
	/** The client should immediately close the session with the Message Server. */
							EMsvCloseSession,
	/** Received after a client has used CMsvSession::OpenAsyncL() to create a session. 
	
	
	The session can now be used. */
							EMsvServerReady,
	/** Received after a client has used CMsvSession::OpenAsyncL() to create a session. 
	
	
	The server could not be started, and aArg1 points to the error code. */
							EMsvServerFailedToStart,
	/** The Message Server index had been corrupted and had to be rebuilt. 
	
	All local entries are recovered, but all remote entries have been lost. */
							EMsvCorruptedIndexRebuilt,
	/** The Message Server has been terminated. 
	
	All clients must close their sessions immediately. */
							EMsvServerTerminated,
	/** This notification is sent whenever Message Server current drive is changed.
	Upon receiving this event, the client should delete the current context of operations 
	and refresh its view of messaging.
	
	aArg1 is a TDriveNumber value that identifies the drive used by the Message 
	Server to hold the index prior to the change. 
	
	aArg2 is also a TDriveNumber value; it identifies the new drive that the Message 
	Server is using.
	
	CMsvEntry contexts either refresh themselves or mark themselves invalid.
	
	@see EMsvMediaUnavailable
	@see TDriveNumber */
							EMsvMediaChanged, // I assume the following four are in sequential order
	/** The media (disk) containing the Message Server index has been removed. 
	
	aArg1 is a TDriveNumber value that identifies the drive that is no longer 
	available.
	
	Future requests may fail with KMsvMediaUnavailable. A EMsvMediaChanged event 
	may be received in the future, as the Message Server switches back to the 
	internal drive. */
							EMsvMediaUnavailable,
	/** The disk containing the Message Store is available again.
	
	aArg1 is a TDriveNumber value that identifies the drive that is being used.
	
	The Message Server can now operate as normal. No client action is necessary. */
							EMsvMediaAvailable,
	/** An incorrect disk is inserted. 
	
	aArg1 is a TDriveNumber value that identifies the drive in which the incorrect 
	disk has been inserted.
	
	Some requests may fail with KMsvMediaIncorrect. Clients may get an EMsvMediaChanged 
	event in the future telling them that the Message Server has switched back 
	to the internal drive. */
							EMsvMediaIncorrect,
	/** The Message Server has started to rebuild its index after it has been corrupted.
	
	@see EMsvCorruptedIndexRebuilt */
							EMsvCorruptedIndexRebuilding
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	/** A media with pre-SQLite message store index has been inserted into a drive in the
	priority drive list. */
							,EMsvMessageStoreNotSupported,
	/** A media with corrupt message store has been inserted into a drive in the priority
	drive list and will be deleted. */
							EMsvMessageStoreCorrupt,
	/** The client needs to refresh its view of messaging either due to a drive with a 
	valid message store being added or removed from the priority drive list, or a disk with
	a valid message store being inserted or removed from a drive with priority less than
	the current drive. */
							EMsvRefreshMessageView,
	/** Media is not available in the drive that has been added to the priority drive list. */
							EMsvDiskNotAvailable,
	/** A media insertion/removal event is received by server which it is unable to handle. */
							EMsvUnableToProcessDiskNotification
#endif
							};
public: 
	/** Indicates an event has occurred. 
	
	The type of event is indicated by the value of aEvent. The interpretation 
	of the TAny arguments depends on this type. 
	
	For most event types, the action that is taken, for example, updating the 
	display, is client-specific. All clients though should respond to EMsvCloseSession 
	and EMsvServerTerminated events. 
	
	@param aEvent Indicates the event type. 
	@param aArg1 Event type-specific argument value 
	@param aArg2 Event type-specific argument value 
	@param aArg3 Event type-specific argument value */
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)=0;
	};

//**********************************
// CMsvSession
//**********************************
//
// 
//

class TCapabilitySet;

const TInt KMsvSessionObserverGranularity=4;  //???

class CMsvSession: public CActive
/** Represents a channel of communication between a client thread (Client-side 
MTM, User Interface MTM, or message client application) and the Message Server 
thread. 

The class provides the means by which clients are notified when important 
Message Server events occur.

Note the following significant groups of functions:

Creation functions: a message client application must use OpenSyncL() or OpenASyncL() 
to create a session object, before it can instantiate any MTM or CMsvEntry 
object. Only a single session should be created within a thread. As Client-side 
MTM, User Interface MTM, and CMsvEntry objects are created in the client thread, 
these use the client session, and do not create their own. Note that to close 
a session, delete all objects relying on that session, and then the session 
object itself.

Cleanup functions: CMsvSession provides the ability to handle the cleanup 
of entries in the event of a leave occurring, in a very similar manner to 
the standard cleanup stack. The difference is that, on a leave, any entries 
that are on the entry cleanup stack are removed from the Message Server. The 
implementation uses the standard cleanup stack, so entry push and pop functions 
should be used in the same order as all other types of push and pop. The functions 
can be used both by MTM implementations and message client applications. 
@publishedAll
@released
*/
	{
public: 
	IMPORT_C static CMsvSession* OpenSyncL(MMsvSessionObserver& aObserver);
	IMPORT_C static CMsvSession* OpenAsyncL(MMsvSessionObserver& aObserver);
	IMPORT_C static CMsvSession* OpenAsObserverL(MMsvSessionObserver& aObserver);
	
	IMPORT_C static CMsvSession* OpenSyncL(MMsvSessionObserver& aObserver, RFs& aFs);
	IMPORT_C static CMsvSession* OpenAsyncL(MMsvSessionObserver& aObserver, RFs& aFs);
	IMPORT_C static CMsvSession* OpenAsObserverL(MMsvSessionObserver& aObserver, RFs& aFs);
	
	IMPORT_C static CMsvSession* OpenSyncL(MMsvSessionObserver& aObserver, TInt aPriority);
	IMPORT_C static CMsvSession* OpenAsyncL(MMsvSessionObserver& aObserver, TInt aPriority);
	IMPORT_C static CMsvSession* OpenAsObserverL(MMsvSessionObserver& aObserver, TInt aPriority);
	
	IMPORT_C static CMsvSession* OpenSyncL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority);
	IMPORT_C static CMsvSession* OpenAsyncL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority);
	IMPORT_C static CMsvSession* OpenAsObserverL(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority);	
	
	~CMsvSession();
	// --- Observer functions ---
	IMPORT_C void AddObserverL(MMsvSessionObserver& aObserver);
	IMPORT_C void RemoveObserver(MMsvSessionObserver& aObserver);
	IMPORT_C TInt SetReceiveEntryEvents(TBool aReceive);
	// --- Utility functions ---
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    IMPORT_C CMsvEntry* GetEntryL(TMsvId aEntId, TBool aChildrenOfAvailableDrives);
#endif
	IMPORT_C CMsvEntry* GetEntryL(TMsvId aEntId);
	IMPORT_C TInt GetEntry(TMsvId aId, TMsvId& aService, TMsvEntry& aEntry);
	
	IMPORT_C CMsvOperation* TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TRequestStatus& aStatus);
	IMPORT_C void TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TDes8& aProgress);
	IMPORT_C void IncPcSyncCountL(const CMsvEntrySelection& aSelection);
	IMPORT_C void DecPcSyncCountL(const CMsvEntrySelection& aSelection);
	IMPORT_C void GetChildIdsL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);
	IMPORT_C void ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	IMPORT_C CMsvOperation* ChangeDriveL(TInt aDrive, TRequestStatus& aStatus);
	IMPORT_C TInt OutstandingOperationsL();
	IMPORT_C CMsvOperation* CopyStoreL(const TDriveUnit& aDrive, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* DeleteStoreL(const TDriveUnit& aDrive, TRequestStatus& aStatus);
	// --- cleanup functions
	IMPORT_C static void CleanupEntry(TAny* aPtr);
	IMPORT_C void CleanupEntryPushL(TMsvId aId);
	IMPORT_C void CleanupEntryPop(TInt aCount=1);

	IMPORT_C void RemoveEntry(TMsvId aId);
	// MTM functions
	IMPORT_C TInt InstallMtmGroup(const TDesC& aFullName);
	IMPORT_C TInt DeInstallMtmGroup(const TDesC& aFullName); 
	
	IMPORT_C TInt StopService(TMsvId aServiceId);
	IMPORT_C TBool ServiceActive(TMsvId aServiceId);
	IMPORT_C TInt ServiceProgress(TMsvId aServiceId, TDes8& aProgress);
	
	IMPORT_C void CloseMessageServer();
	IMPORT_C RFs& FileSession();
	
	IMPORT_C void GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCapSet) const;
	MMsvStoreManager& StoreManager();

	IMPORT_C TBool GetAndClearIndexCorruptFlagL();
	IMPORT_C TDriveUnit CurrentDriveL();
	IMPORT_C TBool DriveContainsStoreL(TDriveUnit aDrive);
	IMPORT_C TBool MessageStoreDrivePresentL();

	IMPORT_C TInt ServiceAccessPointId(TMsvId aServiceId, TUint32& aAccessPointId);
	
	// Changed for PREQ 557.
	// ---- Preferred list related APIs.
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	IMPORT_C void CurrentDriveInfoL(TDriveNumber& aDriveNumber, TUint& aPriority);
	IMPORT_C void DriveListL(RArray<TDriveNumber>& aDriveList);
	IMPORT_C void AvailableDriveListL(RArray<TDriveNumber>& aDriveList);
	IMPORT_C void AddDriveL(TDriveNumber aDriveNumber, TUint& aPriority);
	IMPORT_C void RemoveDriveL(TDriveNumber aDriveNumber);
	IMPORT_C void UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aPriority);		
	IMPORT_C void GetChildIdsAllL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection);

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	IMPORT_C void ResetRepositoryL();
	IMPORT_C void PrintCache();
#endif
#endif

    // change for PREQ 2073 Converter
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	IMPORT_C void GetConvertibleDriveListL(RArray<TDriveNumber>& aDriveList);
	IMPORT_C void ConvertMessageStore(TDriveNumber aDrive,TRequestStatus& aStatus);
	IMPORT_C void GetConversionStatusL(TDriveNumber aDrive);
	IMPORT_C TInt CancelConversion(TDriveNumber aDrive);
#endif			// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
protected:
	CMsvSession(MMsvSessionObserver& aObserver);
	CMsvSession(MMsvSessionObserver& aObserver, RFs& aFs);
	CMsvSession(MMsvSessionObserver& aObserver, TInt aPriority);
	CMsvSession(MMsvSessionObserver& aObserver, RFs& aFs, TInt aPriority);
	
	void ConstructL(TBool aSyncOpening);
	void ConstructAsObserverL();
	//
	IMPORT_C TInt OperationId();
	IMPORT_C RMsvServerSession& Session();

	void CheckDrive();
	//
	// from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	//
private:
	void NotifyAllObserversL(MMsvSessionObserver::TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void CleanupEntryDelete();
  	void DoRunL(TMsvNotifBuffer& aBuffer);
 	void HandleNotifyL();
 	void DoHandleNotifyL(TMsvNotifBuffer& aBuffer);
	void GetMessageFolderL();

	//	
private:
	TInt iOperationId;
	RFs	iFs;
	RMsvServerSession* iSession;
	TInt dummy1;
	TInt dummy2;
	TInt dummy3;
	TMsvNotifBuffer iChange;	
	MMsvSessionObserver&	iMainObserver;
	CArrayPtrFlat<MMsvSessionObserver>* iObservers;
	CMsvEntrySelection* iCleanupList;
	TInt iSyncStart;
	HBufC* iMessageFolder;
	TDriveNumber iDrive;
	CMsvEntrySelection* iNotifSelection;
	TPckgBuf<TUint32> iSequenceBuf;
	TUint32 iNotifySequence;
	TBool iReceiveEntryEvents;
	/** Specifies whether to use the shared file server session */	
	TBool iUseSharedFs;
	//
#ifndef _NO_SESSION_LOGGING_
	void CreateSessionLogL();
	void Log(TRefByValue<const TDesC> aFmt, ...);

	// must be the last data member in the class to preserve BC.
	mutable RFileLogger iLog;
#endif
	//
friend class CSendAs;
friend class CMsvEntry;
friend class CMsvOperation;
friend class CMsvEntryOperation;
friend class CObserverRegistry;
friend class CMsvSearchSortOperation;
friend class CMsvSearchsortOpOnHeaderBody;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
friend class CMsvHeaderStore;
#endif
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
	friend class CTestMsvSession;
#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
	friend class CTestHeaderEntry;
#endif
#endif	
	};




//**********************************
// MMsvEntryObserver
//**********************************
//
// 
//

class MMsvEntryObserver
/** Provides the interface for notification of events associated with an entry. 

The types of event are given in the enumeration TMsvEntryEvent. Clients can 
provide an object that implements the interface, and set it to be notified 
through CMsvEntry::AddObserverL(). 
@publishedAll
@released
*/
	{
public:
	/** Defines entry event types. 
@publishedAll
@released
*/
	enum TMsvEntryEvent 
							{	
	/** The entry has been changed, either as a result of a CMsvEntry::ChangeL() or 
	by another client. */
							EMsvEntryChanged,
	/** New children have been created. aArg1 points to a CMsvEntrySelection contain 
	the ID of the new children. */
							EMsvNewChildren,
	/** Children have been deleted. aArg1 points to a CMsvEntrySelection contain the 
	ID of the deleted children. */
							EMsvDeletedChildren,
	/** One or more of the children have been changed. aArg1 points to a CMsvEntrySelection 
	containing the IDs of the changed children. */
							EMsvChildrenChanged,
	/** The entry has been deleted by another client. The context is now invalid. */
							EMsvEntryDeleted,
	/** The context has become invalid. The entry has been changed, but the CMsvEntry 
	was unable to update the context. The context will only become valid by a 
	successful CMsvEntry::SetEntryL() call. aArg1 points to a TInt containing 
	the error code for the invalid context. */
							EMsvContextInvalid,
	/** Some new children have been created, but CMsvEntry was unable to retrieve the 
	data from the Message Server. The children will be correct only after a successful 
	call to CMsvEntry::SetEntryL(). */
							EMsvChildrenMissing,
	/** An error has occurred such that the status of the children is unknown and probably 
	invalid. aArg1 points to a TInt containing the error code for the invalid 
	context */
							EMsvChildrenInvalid,
	/** The current entry has been moved by another client. The CMsvEntry has already 
	been updated to reflect the new parent. */
							EMsvEntryMoved};
public: 
	/** Indicates when called by a CMsvEntry object that an event has occurred.
	
	The type of event is indicated by the value of aEvent. The interpretation of the aArg1-3 values depends on this type. 
	For most event types, the action that is taken, for example, updating the display, is client-specific. Most clients will 
	need to handle events that make the current context invalid: EMsvContextInvalid and EMsvEntryDeleted.

	An implementation can leave if an error occurs. The leave is not trapped by the framework, so 
	the error code may be displayed to the user.

	@param aEvent Indicates the event type.
	@param aArg1 Event-specific argument value
	@param aArg2 Event-specific argument value
	@param aArg3 Event-specific argument value
	*/
	virtual void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3)=0;
	};



//**********************************
// CMsvEntry
//**********************************
//
// 
//

class CMsvEntry: public CBase, public MMsvSessionObserver, public MMsvStoreObserver
/** Accesses and acts upon a particular Message Server entry. The current entry 
that a CMsvEntry object relates is referred to as its context. 

It may be helpful to consider CMsvEntry functions in two broad groups. The 
first provides means to access the various types of storage associated with 
an entry. The second provides a means to discover and access other entries 
that the entry owns (its children). 

Message client applications, Client-side MTMs, and User Interface MTMs all 
commonly use CMsvEntry objects. CMsvEntry objects though represent a lower 
level of access to an entry than that provided by a Client-side MTM or User 
Interface MTM. In particular, any MTM-specific functionality, such as address 
lists or subject fields, must be accessed by a message client application 
through an MTM inteface.

A CMsvEntry object is relatively expensive in RAM usage, as it caches its 
children, updating them as necessary when notified of changes. They should 
therefore be created sparingly.

Note that Server-side MTMs do not use this class, but a similar one, CMsvServerEntry.
 
@publishedAll
@released
*/
	{
public: // Public member functions
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    IMPORT_C static CMsvEntry* NewL(CMsvSession& aMsvSession, TMsvId aMsvId, const TMsvSelectionOrdering& aOrdering, TBool aChildrenOfAvailableDrives);
    IMPORT_C void SetStandardFolderEntryL(TMsvId aId);
    IMPORT_C CMsvEntrySelection* ChildrenOfAvailableDrivesL() const;
#endif
	IMPORT_C static CMsvEntry* NewL(CMsvSession& aMsvSession, TMsvId aMsvId, const TMsvSelectionOrdering& aOrdering);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	IMPORT_C void SetEntryNoCheckL(TMsvId aId, TBool aChildrenOfAvailableDrives=EFalse);
#else
	IMPORT_C void SetEntryNoCheckL(TMsvId aId);
#endif
	IMPORT_C ~CMsvEntry();
	//
	// --- Observer functions ---
	IMPORT_C void AddObserverL(MMsvEntryObserver& aObserver);
	IMPORT_C void RemoveObserver(MMsvEntryObserver& aObserver);
	//
	// --- Accessor for associated session ---
	inline CMsvSession& Session(); 
	//
	// --- Accessors the associated message store ---
	IMPORT_C CMsvStore* ReadStoreL();
	IMPORT_C CMsvStore* EditStoreL();
	//
	// --- Synchronous Current Entry functions ---
	inline TMsvId EntryId() const; 
	inline const TMsvEntry& Entry() const; 
	inline const TMsvSelectionOrdering& SortType() const;
	inline TMsvId OwningService() const;
	IMPORT_C void SetSortTypeL(const TMsvSelectionOrdering& aOrdering);
	IMPORT_C void SetMtmListL(const CArrayFix<TUid>& aMtmList);
	IMPORT_C void SetEntryL(TMsvId aId);
	IMPORT_C void ChangeL(const TMsvEntry& aEntry);
	IMPORT_C void ChangeL(const TMsvEntry& aEntry, TSecureId aOwnerId);
	

	//
	// --- Asynchronous Current Entry functions ---
	IMPORT_C CMsvOperation* ChangeL(const TMsvEntry& aEntry, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* ChangeL(const TMsvEntry& aEntry, TSecureId aOwnerId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* ChangeL(const CMsvEntrySelection& aSelection, TBool aMark,TRequestStatus& aStatus);
	//
	// --- Asynchronous Child Entry functions ---
	IMPORT_C CMsvOperation* CreateL(const TMsvEntry& aEntry, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* CreateL(const TMsvEntry& aEntry, TSecureId aOwnerId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* DeleteL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* DeleteL(TMsvId aMsvId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* CopyL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* CopyL(TMsvId aMsvId, TMsvId aTargetId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* MoveL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TRequestStatus& aStatus);
	IMPORT_C CMsvOperation* MoveL(TMsvId aMsvId, TMsvId aTargetId, TRequestStatus& aStatus);
	//
	// --- Synchronous Child Entry functions ---
	IMPORT_C CMsvEntrySelection* ChildrenL() const;
	IMPORT_C CMsvEntrySelection* ChildrenWithServiceL(TMsvId aServiceId) const;
	IMPORT_C CMsvEntrySelection* ChildrenWithMtmL(TUid aMtm) const;
	IMPORT_C CMsvEntrySelection* ChildrenWithTypeL(TUid aType) const;
	
	inline TInt Count() const;
	IMPORT_C const TMsvEntry& ChildDataL(TMsvId aId) const;
	IMPORT_C const TMsvEntry& operator[](TInt aIndex) const;
	IMPORT_C CMsvEntry* ChildEntryL(TMsvId aId) const;
	IMPORT_C void MoveL(TMsvId aMsvId, TMsvId aTargetId);
	IMPORT_C void MoveL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TMsvLocalOperationProgress& aProgress);
	IMPORT_C void CopyL(const CMsvEntrySelection& aSelection, TMsvId aTargetId, TMsvLocalOperationProgress& aProgress);
	IMPORT_C void CopyL(TMsvId aMsvId, TMsvId aTargetId);


	IMPORT_C void CreateL(TMsvEntry& aEntry);
	IMPORT_C void CreateL(TMsvEntry& aEntry, TSecureId aOwnerId);
	IMPORT_C void DeleteL(TMsvId aId);
	IMPORT_C void DeleteL(const CMsvEntrySelection& aSelection, TMsvLocalOperationProgress& aProgress);
	IMPORT_C void ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes);
	//
	// from MMsvSessionObserver
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	// From MMsvStoreObserver
	void HandleStoreEvent(TMsvStoreEvent aEvent, TMsvId aId);


	IMPORT_C TBool HasStoreL() const;
	
	//	
private: // Private members
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
    CMsvEntry(CMsvSession& aMsvSession, const TMsvSelectionOrdering& aOrdering, TBool aChildrenOfAvailableDrives);
	inline TUint GetDriveId(TMsvId aMaskedId) const;
#endif
	CMsvEntry(CMsvSession& aMsvSession, const TMsvSelectionOrdering& aOrdering);

	void ConstructL(TMsvId aMsvId);
	//
	void NotifyAllObserversL(MMsvEntryObserver::TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	void ContextChangedL(MMsvEntryObserver::TMsvEntryEvent aEvent);
	void NewChildrenL(const CMsvEntrySelection& aSelection);
	CMsvEntrySelection* DoGetNewChildrenL(const CMsvEntrySelection& aSelection);
	void DeletedChildrenL(const CMsvEntrySelection& aSelection);
	void ChildrenChangedL(const CMsvEntrySelection& aSelection);
	void CheckNewGrandchildrenL(TMsvId aId);
	void CheckDeletedGrandchildrenL(TMsvId aId);
	void NotifyChildChangedL(TMsvId aId);
	CMsvEntrySelection* DoMakeSelectionL(TMsvId aId);
	void CheckIfContextMovedL(const CMsvEntrySelection& aSelection);
	//
	TBool IsAChild(TMsvId aId) const;
	TBool AreChildren(const CMsvEntrySelection& aSelection) const;
	//
	CMsvOperation* DoDeleteL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	CMsvClientEntry* DoGetEntryLC(TMsvId aId, TMsvId& aOwningService);
	void DoGetChildrenL();
	CMsvEntryArray* GetNewSortedListL(const TMsvSelectionOrdering& aOrdering, const CArrayFix<TUid>& aMtmList);
	void DoSortTypeL(CMsvClientEntry* aContext);
	void ReplaceChildL(TInt pos, const TMsvEntry& aEntry);
	void DeleteChild(TInt aPosition);
	void HandleMediaChangeL();

	TInt MoveOneL(TMsvId aMsvId, TMsvId aTargetId);
	TInt CopyOneL(TMsvId aMsvId, TMsvId aTargetId);
	TInt DeleteOneL(TMsvId aMsvId);

	//
private:
	enum TEntryState {	EValid,
						EInvalidChangingContext,
						EInvalidDeletedContext,
						EInvalidOldContext,
						EInvalidMissingChildren};
	//
#ifndef _NO_SESSION_LOGGING_
	void Log(TRefByValue<const TDesC> aFmt, ...);
#endif
	//
private:
	TBool iOberserverAdded;
	TEntryState	iState;
	CMsvSession& iMsvSession;
	TMsvSelectionOrdering iOrdering;
	const TMsvEntry* iEntryPtr;
	CArrayPtrFlat<MMsvEntryObserver>* iObservers;
	CArrayPtrFlat<CMsvClientEntry>* iEntries;
	CMsvEntryArray* iSortedChildren;
	CMsvStore* iStore;
	CArrayFixFlat<TUid>* iMtmList;
	TMsvId iOwningService;
	TUint32 iNotifySequence;
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	/* If this flag is set, child entries from all available drives
	   in the preferred drive list will be fetched. */
	TBool iChildrenOfAvailableDrives;
#endif
	};

//**********************************
// MessageServer
//**********************************
//
// 
//

class MessageServer
/** Provides various static information functions relating to the Message Server.

@see TDriveNumber 
@publishedAll
@released
*/
	{
public:
	IMPORT_C static TVersion Version();
	IMPORT_C static TMsvId NullUidValue();
	//
	IMPORT_C static TBool DriveContainsStore(RFs& aFs, TInt aDrive);
	IMPORT_C static TInt CurrentDriveL(RFs& aFs);
	IMPORT_C static TBool IsMessageStoreDrivePresentL(RFs& aFs);
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	IMPORT_C static TBool IsMessageStoreSupported(TDriveNumber aDrive);
#endif
	};

//**********************************
// MtmClientUtils
//**********************************
//
// 
//

class McliUtils
/** Utility class to simplify getting progress information from a messaging operation object.

The functions get and unpack the progress information buffers from an operation object.

@publishedAll
@released
*/
    {
public:
    IMPORT_C static TMsvLocalOperationProgress GetLocalProgressL(CMsvOperation& aOperation);
    IMPORT_C static TMsvLocalOperationProgress GetFinalLocalProgress(CMsvOperation& aOperation);
    IMPORT_C static TInt GetProgressErrorL(CMsvOperation& aOperation);
    IMPORT_C static TMsvId GetProgressIdL(CMsvOperation& aOperation);
	};





#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
 * The class allows  client MTMs  to communicate the  header structure 
 * to the message server. It provides API to add various fields to the
 * header structure. Finally it provides API to create header store in
 * the messaging DB.   The store will always be created in the message
 * server current drive. In order to create store in non-current drive
 * the user should perform ChangeDrive() operation.

 * The header store should be created immediately after creating the
 * mail account. The server will create separate header store for each
 * MTM type. The function does not return an error if the header store
 * being created already exist.

 @publishedAll
 @released
 */
class CMsvHeaderStore : public CBase
	{
public:

	/**
	 * Common Header Fields
	 * @publishedAll
	 * @released
	 */
	enum TCommonHeaderField
		{		
		EFrom    = 0x001,
		ETo      = 0x002,
		ECC      = 0x004,
		EBCC     = 0x008,
		ESubject = 0x010
		};


	IMPORT_C static CMsvHeaderStore* NewL(const TUid& aMtmId, CMsvSession& aSession);
	IMPORT_C static CMsvHeaderStore* NewLC(const TUid& aMtmId, CMsvSession& aSession);
	~CMsvHeaderStore();

	IMPORT_C void AddFieldL(HBufC* aFieldName, EFieldType aFieldType);
	IMPORT_C void AddFieldL(TCommonHeaderField aCommonHeaderFieldName);
	IMPORT_C void CreateStoreL();
	IMPORT_C static TBool DoesStoreExistsL(const TUid& aMtmId, CMsvSession& aSession);
	IMPORT_C TPtrC LastErrorMessageL();
	
private:
	CMsvHeaderStore(TUid aMtmId, CMsvSession& aSession);
	void ConstructL();

private:

	TUid iMtmUid;
	CMsvSession& iSessionRef;
	RPointerArray<CFieldPair> iFieldDetails;
	HBufC* iLastErrorMessage;
	};


#endif    	// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
#include <msvapi.inl>

#endif // __MSVAPI_H__
