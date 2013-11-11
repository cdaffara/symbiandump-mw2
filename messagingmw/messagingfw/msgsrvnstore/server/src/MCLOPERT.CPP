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

#include <s32std.h>

#include "MSVAPI.H"
#include "MSVCOP.H"
#include "MSVPANIC.H"
#include <tmsvsystemprogress.h>

//**********************************
// CMsvOperation
//**********************************

EXPORT_C CMsvOperation::CMsvOperation(CMsvSession& aMsvSession, TInt aPriority, TRequestStatus& aObserverRequestStatus)
: CActive(aPriority), iObserverRequestStatus(aObserverRequestStatus), iMsvSession(aMsvSession)
//
//
//
/** Constructor. This is called by MTM implementers from the derived class constructor. 

CMsvOperation-derived objects are not created by message client applications. The constructor uses the 
CMsvSession object to obtain a ID for the operation. This ID can be accessed by clients through Id(). 

aPriority is the priority used by the active object. Typically, this is EPriorityStandard.

aObserverRequestStatus is the asynchronous status object of an active object provided by the caller. 
The derived class is required to signal the caller through this when the operation completes. 
It is stored by the constructor in the iObserverRequestStatus member.
@param aMsvSession Session object 
@param aPriority Active object priority 
@param aObserverRequestStatus  Request status of operation observer
 */
	{
	__DECLARE_NAME(_S("CMsvOperation"));
	iId = iMsvSession.OperationId();
	}


EXPORT_C CMsvOperation::~CMsvOperation()
/** Destructor.

This base class function takes no action. Derived classes should implement 
a destructor to do the clean up tasks that they require. Minimally, they should 
call Cancel() to cancel any outstanding request. */
	{}

EXPORT_C const TDesC8& CMsvOperation::FinalProgress()
/** Gets information about a completed operation. 

Unlike ProgressL(), this is guaranteed to provide status information reflecting 
the outcome of a completed operation.

Requirements:

The default implementation of this function returns the result from ProgressL(). 
If the operation is active or ProgressL() leaves, a panic occurs. 

A derived class should implement the function to meet the requirement to provide 
appropriate final status information. Typically, on active object completion, 
it will store progress information in a private member, and return that stored 
information from this function.

@return Descriptor holding progress information
@panic MSGS 285 Operation is active
@panic MSGS 286 Could not get final progress information, as ProgressL() left.
*/
	{	
	__ASSERT_ALWAYS(!IsActive(), PanicServer(EMsvActiveInFinalProgress));
	const TDesC8* progress = &KNullDesC8();
	TRAPD(leave, progress = &ProgressL());
	__ASSERT_ALWAYS(leave == KErrNone, PanicServer(EMsvFinalProgressFailed));
	return *progress;
	}

EXPORT_C TUid CMsvOperation::Mtm() const
/** Gets the UID of the MTM associated with the operation.

This UID can be used to determine the appropriate method of getting information 
from a progress descriptor. 

The default implementation returns iMtm.

@return The UID of the MTM associated with the operation 
@see ProgressL() */
 	{
 	return iMtm;
 	} 

/** This call leads to calling SystemProgress() in the server to populate the
TMsvSystemProgress structure.
@param aOutSysProg The TMsvSystemProgress structure to be populated by the server
@return the error of the Extension_ method call
*/
EXPORT_C TInt CMsvOperation::SystemProgress(TMsvSystemProgress& aOutSysProg)
	{
	TAny* ptrNull = NULL;
	return Extension_(KUIDMsgMsvSystemProgress, ptrNull, &aOutSysProg);
	}

/** The extension method provides a polymorphic behaviour to call the correct
SystemProgress function.
@param aExtensionId The Uid passed in as KUIDMsgMsvSystemProgress to obtain the
System Progress.
@return KErrExtensionNotSupported retuned if no Extension_ function is overriddden
by the derived class.
*/
EXPORT_C TInt CMsvOperation::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)	
	{
	TInt ret = KErrNone;
	switch(aExtensionId)
		{
		case KUIDMsgMsvSystemProgress:
			{
			ret = KErrExtensionNotSupported;
			}
			break;
		default:
			{
		// Chain to base class
			ret = CActive::Extension_(aExtensionId, a0, a1);
			}
			break;
		}
	return ret;
	}

//**********************************
// CMsvEntryOperation
//**********************************

CMsvEntryOperation* CMsvEntryOperation::NewL(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus)
//
//
//
	{
	CMsvEntryOperation* self = CMsvEntryOperation::NewLC(aMsvSession, aObserverRequestStatus);
	CleanupStack::Pop();
	return self;
	}

CMsvEntryOperation* CMsvEntryOperation::NewLC(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus)
//
//
//
	{
	CMsvEntryOperation* self = new(ELeave) CMsvEntryOperation(aMsvSession, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


CMsvEntryOperation::CMsvEntryOperation(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus)
: CMsvOperation(aMsvSession, EPriorityStandard, aObserverRequestStatus)
//
//
//
	{
	__DECLARE_NAME(_S("CMsvEntryOperation"));
	}


CMsvEntryOperation::~CMsvEntryOperation()
//
//
//
	{
	Cancel();
	delete iProgress;
	}


void CMsvEntryOperation::ConstructL()
//
//
//
	{
	iProgress = HBufC8::NewL(KMsvProgressBufferLength);
	CActiveScheduler::Add(this);
	}



void CMsvEntryOperation::DoCancel()
//
//
//
	{
	TPtr8 des = iProgress->Des();
	iMsvSession.Session().CancelOperation(Id(), des); // error ignored

	// signal the observer that the operation has completed
	TRequestStatus* status = &iObserverRequestStatus;
	User::RequestComplete(status, KErrCancel);
	}


void CMsvEntryOperation::RunL()
//
//
//
	{
	// get the final Msv system progress from the server
	iErrorGetSystemProgress = iMsvSession.Session().OperationSystemProgress(Id(), iSystemProgress);
	
	// get the final progress from the server
	TPtr8 des = iProgress->Des();
	TInt length=des.Length();
	TInt error = iMsvSession.Session().OperationCompletion(Id(), des); 
	if (error)
		des.SetLength(length);

	// signal the observer that the operation has completed
	TRequestStatus* status = &iObserverRequestStatus;

	// If there is no operation then complete with our status
	// Otherwise complete with the operation completion error

	if (error == KErrNotFound && iStatus.Int())
		User::RequestComplete(status, iStatus.Int());
	else
		User::RequestComplete(status, error ? error : iStatus.Int());
	}

const TDesC8& CMsvEntryOperation::ProgressL()
//
//
//
	{
	if (IsActive())
		{
		TPtr8 des = iProgress->Des();
		User::LeaveIfError(iMsvSession.Session().OperationProgress(Id(), des));
		}
	return *iProgress;
	}

/** The extension method provides a polymorphic behaviour to get the TMsvSystemProgress
structure in one of the Operations.
@param aExtensionId The Uid passed in as KUIDMsgMsvSystemProgress to obtain the
System Progress.
@return error from the call of DoGetSystemProgress() method.
*/
EXPORT_C TInt CMsvEntryOperation::Extension_(TUint aExtensionId, TAny *&a0, TAny *a1)
	{
	if (aExtensionId == KUIDMsgMsvSystemProgress)
		{
		TMsvSystemProgress* systemProgress = reinterpret_cast<TMsvSystemProgress*>(a1);
		return DoGetSystemProgress(*systemProgress);
		}
	
	// Chain to base class
	return CMsvOperation::Extension_(aExtensionId, a0, a1);
	}

/** This function calls the session which eventually goes to SystemProgress() in the server
to populate the TMsvSystemProgress structure.
@param aOutSysProg The TMsvSystemProgress structure to be populated by the server
@return the error from the SystemProgress() call.
*/
TInt CMsvEntryOperation::DoGetSystemProgress(TMsvSystemProgress& aOutSysProg)
	{
	TInt err = KErrNone;
	// use RMsvSession
	if (IsActive())
		{
		err = iMsvSession.Session().OperationSystemProgress(Id(), aOutSysProg);
		}
	else
		{
		aOutSysProg = iSystemProgress;
		err = iErrorGetSystemProgress;
		}
	return err;
	}

const TDesC8& CMsvEntryOperation::FinalProgress()
	{
	__ASSERT_ALWAYS(!IsActive(), PanicServer(EMsvActiveInFinalProgress));
	return *iProgress;
	}


void CMsvEntryOperation::Start()
//
//
//
	{
	iObserverRequestStatus=KRequestPending;
	SetActive();
	}

//**********************************
// CMsvOperationWait
//**********************************

// static 
/** Allocates and constructs a new CMsvOperationWait object.

The new object is added to the active scheduler and left on
the cleanup stack.

@param aPriority Active object priority. It has a default value of 
EPriorityStandard.
@return New CMsvOperationWait object
@leave KErrNoMemory
*/
EXPORT_C CMsvOperationWait* CMsvOperationWait::NewLC(TInt aPriority)
	{
	CMsvOperationWait* self = new (ELeave) CMsvOperationWait(aPriority);
	CleanupStack::PushL(self);
	return self;
	}

/** Constructor.

@param aPriority Active object priority
*/
CMsvOperationWait::CMsvOperationWait(TInt aPriority)
: CActive(aPriority)
	{
	CActiveScheduler::Add(this);
	}

/** Destructor.

Any active request on the object is cancelled.
*/
EXPORT_C CMsvOperationWait::~CMsvOperationWait()
	{
	Cancel();
	}

/** Sets the object to be active.

*/
EXPORT_C void CMsvOperationWait::Start()
	{
	iStatus = KRequestPending;
	SetActive();
	}

/** Handles a request completion event.

The function stops the current active scheduler.

@see CActive::RunL()
*/
void CMsvOperationWait::RunL()
	{
	CActiveScheduler::Stop();
	}

/** Implements cancellation of an outstanding request.

@see CActive::DoCancel()
*/
void CMsvOperationWait::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}


//**********************************
// CMsvOperationActiveSchedulerWait
//**********************************

// static 
/** Allocates and constructs a new CMsvOperationActiveSchedulerWait object.

The new object is added to the active scheduler and left on
the cleanup stack.

@param aPriority Active object priority. It has a default value of 
EPriorityStandard.
@return New CMsvOperationActiveSchedulerWait object
@leave KErrNoMemory
*/
EXPORT_C CMsvOperationActiveSchedulerWait* CMsvOperationActiveSchedulerWait::NewLC(TInt aPriority)
	{
	CMsvOperationActiveSchedulerWait* self = new (ELeave) CMsvOperationActiveSchedulerWait(aPriority);
	CleanupStack::PushL(self);
	return self;
	}

/** Constructor.

@param aPriority Active object priority
*/
CMsvOperationActiveSchedulerWait::CMsvOperationActiveSchedulerWait(TInt aPriority)
: CActive(aPriority)
	{
	CActiveScheduler::Add(this);
	}

/** Destructor.

Any active request on the object is cancelled.
*/
EXPORT_C CMsvOperationActiveSchedulerWait::~CMsvOperationActiveSchedulerWait()
	{
	Cancel();
	}

/** Sets the object to be active, and wait until the request is completed.
*/
EXPORT_C void CMsvOperationActiveSchedulerWait::Start()
	{
	SetActive();
	iActiveSchedulerWait.Start();	//Re-enter the active scheduler--execution halts here until RunL is called
	}

/** Handles a request completion event.

The function stops the object waiting, and allows the program to resume execution after the Start() function call.

@see CActive::RunL()
*/
void CMsvOperationActiveSchedulerWait::RunL()
	{
	iActiveSchedulerWait.AsyncStop();	//Exit current scheduler loop, resuming execution of the Start() function
	}

/** Implements cancellation of an outstanding request.

@see CActive::DoCancel()
*/
void CMsvOperationActiveSchedulerWait::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrCancel);
	}

//**********************************
// CMsvCompletedOperation
//**********************************

// static
/** Allocates and constructs a new CMsvCompletedOperation object.

The new object is added to the active scheduler and its asynchronous request 
immediately set to complete.

@param aSession Message Server session to use
@param aMtm The UID of the MTM associated with the operation. 
@param aProgress Operation progress information buffer. This is copied by the object.
@param aService The ID of the service that is associated with this operation. 
@param aObserverRequestStatus Request status of the operation observer. This 
observer is signalled that the operation is complete. 
@param aError Error code with which to complete the request. Use KErrNone to 
indicate that the operation completed successfully. 

@return New CMsvOperationActiveSchedulerWait object
@leave KErrNoMemory
*/
EXPORT_C CMsvCompletedOperation* CMsvCompletedOperation::NewL(CMsvSession& aSession, TUid aMtm, const TDesC8& aProgress, TMsvId aService, TRequestStatus& aObserverRequestStatus, TInt aError)
	{
	CMsvCompletedOperation* self = new (ELeave) CMsvCompletedOperation(aSession, aObserverRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructL(aMtm, aError, aProgress, aService);
	CleanupStack::Pop();
	return self;
	}

CMsvCompletedOperation::CMsvCompletedOperation(CMsvSession& aSession, TRequestStatus& aObserverRequestStatus)
: CMsvOperation(aSession, EPriorityStandard, aObserverRequestStatus) 
	{
	__DECLARE_NAME(_S("CMsvCompletedOperation"));
	}

/** Destructor.

Any active request on the object is cancelled.
*/
EXPORT_C CMsvCompletedOperation::~CMsvCompletedOperation()
	{
	Cancel();
	delete iProgress;
	}

void CMsvCompletedOperation::ConstructL(TUid aMtm, TInt aError, const TDesC8& aProgress, TMsvId aService)
	{
	iProgress = HBufC8::NewL(aProgress.Length());
	*iProgress = aProgress;
	iMtm=aMtm;
	iService=aService;
	//
	CActiveScheduler::Add(this);
	iStatus = KRequestPending;
	SetActive();
	//
	TRequestStatus* pstat=&iStatus;
	User::RequestComplete(pstat, aError);
	iObserverRequestStatus = KRequestPending;
	}

/** Gets information on the progress of the operation.

For a CMsvCompletedOperation, the progress information returned by 
ProgressL() and FinalProgress() is the same, and is the progress 
information set in the NewL() function. 
 
@return Descriptor holding progress information.
@see CMsvOperation::ProgressL()
*/
const TDesC8& CMsvCompletedOperation::ProgressL()
	{
	return *iProgress;
	}

/** Gets information about a completed operation. 
@return Descriptor holding progress information.
@see CMsvOperation::FinalProgress()
*/
const TDesC8& CMsvCompletedOperation::FinalProgress()
	{
	__ASSERT_ALWAYS(!IsActive(), PanicServer(EMsvActiveInFinalProgress));
	return *iProgress;
	}

void CMsvCompletedOperation::DoCancel()
	{
	TRequestStatus* pstat=&iObserverRequestStatus;
	User::RequestComplete(pstat, iStatus.Int());
	}

void CMsvCompletedOperation::RunL()
	{
	TRequestStatus* pstat=&iObserverRequestStatus;
	User::RequestComplete(pstat, iStatus.Int());
	}


//**********************************
// CMsvSendOperation
//**********************************

/** Constructor.

@param aSession Message Server session to use
@param aObserverRequestStatus Request status of the operation observer. This 

@return New CMsvSendOperation object
@leave KErrNoMemory
*/
EXPORT_C CMsvSendOperation::CMsvSendOperation(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus) :
	CMsvOperation(aMsvSession, CActive::EPriorityStandard, aObserverRequestStatus)
	{
	iObserverRequestStatus = KRequestPending;
	CActiveScheduler::Add(this);
	}
	
/** 
Takes ownership of a CMsvOperation object that is set to complete iStatus of this object.
This implementation does not leave.

@param anOperation CMsvOperation object
*/
EXPORT_C void CMsvSendOperation::Start(CMsvOperation* aOperation)
	{
	iOperation = aOperation;
	iMtm = iOperation->Mtm();
	iService = iOperation->Service();
	SetActive();
	}
	
/** Destructor.

Any active request on the object is cancelled.
*/
EXPORT_C CMsvSendOperation::~CMsvSendOperation()
	{
	Cancel();
	delete iOperation;
	}

/** Gets progress information about the current send operation

Returns the translated progress for the current send operation. Calls the underlying
ProgressL function to obtain the native progress then calls TranslateProgressL to 
translate the native progress into standard send progress. 

@return Descriptor holding progress information
*/
EXPORT_C const TDesC8& CMsvSendOperation::ProgressL()
	{ 
	return TranslateProgress(iOperation->ProgressL()); 
	}

/** Gets information about a completed operation. 

Returns the translated progress for the current send operation. Calls the sending 
operation's FinalProgress function to obtain the native progress then calls 
TranslateProgressL to translate the native progress into standard send progress. 

@return Descriptor holding progress information
*/
EXPORT_C const TDesC8& CMsvSendOperation::FinalProgress()
	{ 
	return TranslateProgress(iOperation->FinalProgress()); 
	}

/** 
Called when this operation is cancelled and is active. It in turn cancels the
sending operation and completes iObserverRequestStatus with KErrCancel. 
*/
EXPORT_C void CMsvSendOperation::DoCancel()
	{
	iOperation->Cancel();
	Complete(KErrCancel);
	}

/**
Called on completion of sending operation. Completes iObserverRequestStatus
with the sending operation's status. This RunL does not leave. 
*/
EXPORT_C void CMsvSendOperation::RunL()
	{
	iProgress().iError = iStatus.Int();
	Complete(iStatus.Int());
	}

void CMsvSendOperation::Complete(TInt anError)
	{
	TRequestStatus* stat = &iObserverRequestStatus;
	User::RequestComplete(stat, anError);
	}

