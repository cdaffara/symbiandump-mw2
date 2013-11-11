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
#include <s32mem.h>


#include "MSVIPC.H"
#include "MSERVER.H"
#include "MSVREGIP.H"

#include "MCLIENT.H"
#include "MCLENTRY.H"
#include "MSVPANIC.H"
#include "MSVAPI.H"
#include "MsvSecurityCapabilitySet.h"
#include <mmsvstoremanager.h>
#include <tmsvsystemprogress.h>
#include <tnonoperationmtmdata.h>

_LIT(KMsvServerExe, "msexe.exe");

#if defined _UNICODE
const TUid KMsvMsexExeUid={0x1000484B};
#else
const TUid KMsvMsexExeUid={0x1000484A};
#endif

const TInt KMsvRetryTimeout=100000;
const TInt KMsvRetryCount=10;

static TInt StartServer()
	{
	const TUidType serverUid(KNullUid, KNullUid, KMsvMsexExeUid);
	RProcess server;
	TInt r = server.Create(KMsvServerExe, KNullDesC, serverUid);

	if( r != KErrNone )
		return r;
	
	TRequestStatus status;
	server.Rendezvous(status);
	if( status != KRequestPending )
		server.Kill(0); // abort start-up
	else
		server.Resume();	// wait for server start-up.
	User::WaitForRequest(status);
	
	// If the server panics on start-up, then exit reason may still be zero,
	// which is not distinguishable from KErrNone.		
	r = (server.ExitType() == EExitPanic ) ? KErrGeneral : status.Int();
	server.Close();
	return r;	
	}

//********************************
//RMsvServerSession
//**********************************

/**
Constructor.

@capability None
*/
EXPORT_C RMsvServerSession::RMsvServerSession()
: iBuffer(NULL)
	{}


/**
Destructor.

@capability None
*/
EXPORT_C RMsvServerSession::~RMsvServerSession()
	{
	delete iBuffer;
	}

#if defined (__EPOC32__)
/**
Connect to the messaging server, and starts it if is not already running.
 
The default number of message slots is 14.

@param aFs File server handle
@return System wide error codes

@capability None
*/
EXPORT_C TInt RMsvServerSession::Connect(RFs& aFs)
#else
EXPORT_C TInt RMsvServerSession::Connect(RFs& /*aFs*/)
#endif
	{
	if (iBuffer==NULL)
		{
		iBuffer = HBufC8::New(KMsvSessionBufferLength);
		if (iBuffer==NULL)
			return KErrNoMemory;
		}


	TInt retry = KMsvRetryCount;
	FOREVER
		{
		TInt r = CreateSession(KMsvServerName, Version(), KMsvNumberOfSlots);
	 	if( r != KErrNotFound && r != KErrServerTerminated && r != KErrServerBusy )
			return r;
		if( --retry == 0 )
			return r;
		if( r == KErrServerBusy )
			User::After(KMsvRetryTimeout);
		r = StartServer();
		if( r != KErrNone && r != KErrAlreadyExists )
			return r;		
		}

	}

	
/**
Gets the version number of the server.

@return Version number

@capability None
*/
EXPORT_C TVersion RMsvServerSession::Version(void) const
	{
	return(TVersion(KMsvServerMajorVersionNumber,KMsvServerMinorVersionNumber,KMsvServerBuildVersionNumber));
	}


/** 
Makes a request for session event notifications.

@param aChange Packaged notification information (a TMsvNotifBuffer object)
@param aSequence notification sequence number (a packaged TUint) 
@param aRequestStatus Asynchronous request status

@capability None
*/
EXPORT_C void RMsvServerSession::QueueSessionEventRequest(TDes8& aChange, TDes8& aSequence, TRequestStatus& aRequestStatus)
	{
	SendReceive(EMsvNotifySessionEvent, TIpcArgs(&aChange,&aSequence), aRequestStatus);
	}

/**
Cancels a request for session event notifications.

@return System wide error codes

@capability None
*/
EXPORT_C TInt RMsvServerSession::CancelSessionEventRequest()
	{
	return SendReceive(EMsvCancelSessionEventNotification);
	}


/**
Set a session as an observer only.

@return System wide error codes

@capability None
*/
TInt RMsvServerSession::SetSessionAsObserver()
	{
	return SendReceive(EMsvSetSessionAsObserver);
	}

void RMsvServerSession::SendEntryDataL(TMsvOp aOperationId, const TMsvEntry& aEntry)
//
// Sends the entry data in a synchronous call
//
	{
	// package up the entry into a packed entry
	TMsvPackedEntry packedEntry(iBuffer);
	TInt error = packedEntry.PackEntry(aEntry);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = packedEntry.PackEntry(aEntry);
		}

	// pass data to the server
	User::LeaveIfError(SendReceive(EMsvOperationData, TIpcArgs(aOperationId,iBuffer)));
	}

void RMsvServerSession::SendOperationDataL(TMsvOp aOperationId, const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2)
//
// Sends the operation data in a synchronous call
//
	{
	PackOperationDataL(aSelection, aParameter1, aParameter2);

	// package up the entry into a packed entry
	User::LeaveIfError(SendReceive(EMsvOperationData, TIpcArgs(aOperationId,iBuffer)));
	}

void RMsvServerSession::PackOperationDataL(const CMsvEntrySelection& aSelection, TInt aParameter1, TInt aParameter2)
	{
	TMsvPackedOperation packedOperation(iBuffer);
	TInt error = packedOperation.Pack(aSelection, aParameter1, aParameter2);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		error = packedOperation.Pack(aSelection, aParameter1, aParameter2);
		}
	}

void RMsvServerSession::SendCommandDataL(TMsvOp aOperationId, const CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter)
//
// Sends the operation data in a synchronous call
//
	{
	TMsvPackedOperation packedOperation(iBuffer);
	TInt error = packedOperation.Pack(aSelection, aCommand);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		error = packedOperation.Pack(aSelection, aCommand);
		}

	// package up the entry into a packed entry
	User::LeaveIfError(SendReceive(EMsvCommandData, TIpcArgs(aOperationId,iBuffer,&aParameter)));
	}



/**
Creates an entry in the message server index (asynchronously) with the supplied owner ID.

@param aEntry Entry to create
@param aOperationId Operation identifier
@param aOwnerId The ID of the owning process
@param aRequestStatus Asynchronous request status

@capability None A client with no capabilities can create an entry only if the entry is    
under a local service, is in an unprotected folder, is not a service entry, and, if part of 
an existing message, if that message is owned by the client.  

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to create an entry that is not a service vary depending on the 
following conditions:  

-# Entry to be created is part of an existing message: 
	-# Entry is under the local service:
		-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
		-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
		-# Entry is in a protected folder: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder and the existing entry is not owned by the client: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder and the existing entry is owned by the client: no capabilities
	-# Entry is under a remote service:
		-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
		-# The Outbox is unprotected and the existing entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
		-# The Outbox is unprotected and the existing entry is not owned by the client: ReadUserData + WriteUserData + 
		MTM-specified capabilities (of the MTM for the remote service)
-# Entry to be created is not part of an existing message: 
	-# Entry is under the local service:
		-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
		-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
		-# Entry is in a protected folder: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder: no capabilities
	-# Entry is under a remote service:
		-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
		-# The Outbox is unprotected: MTM-specified capabilities (of the MTM for the remote service) 

@internalComponent
*/
EXPORT_C void RMsvServerSession::CreateEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus)
//
// Creates an entry in the index for aEntry.
//
	{
	TestSlotAvailableL();
	SendEntryDataL(aOperationId, aEntry);
	aRequestStatus=KRequestPending;
	SendReceive(EMsvCreateEntry, TIpcArgs(aOperationId, aOwnerId), aRequestStatus);
	}



/**
Creates an entry in the message server index (synchronously).

@param aEntry Entry to create
@param aOperationId Operation identifier
@param aOwnerId The ID of the owning process

@capability None A client with no capabilities can create an entry only if the entry is    
under a local service, is in an unprotected folder, is not a service entry, and, if part of 
an existing message, if that message is owned by the client.  

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to create an entry that is not a service vary depending on the 
following conditions:  

-# Entry to be created is part of an existing message: 
	-# Entry is under the local service:
		-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
		-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
		-# Entry is in a protected folder: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder and the existing entry is not owned by the client: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder and the existing entry is owned by the client: no capabilities
	-# Entry is under a remote service:
		-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
		-# The Outbox is unprotected and the existing entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
		-# The Outbox is unprotected and the existing entry is not owned by the client: ReadUserData + WriteUserData + 
		MTM-specified capabilities (of the MTM for the remote service)
-# Entry to be created is not part of an existing message: 
	-# Entry is under the local service:
		-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
		-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
		-# Entry is in a protected folder: ReadUserData + WriteUserData
		-# Entry is in an unprotected folder: no capabilities
	-# Entry is under a remote service:
		-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
		-# The Outbox is unprotected: MTM-specified capabilities (of the MTM for the remote service) 

@internalComponent
*/
EXPORT_C void RMsvServerSession::CreateEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId)
//
// Creates an entry in the index for aEntry.
//
	{
	TestSlotAvailableL();
	SendEntryDataL(aOperationId, aEntry);
	User::LeaveIfError(SendReceive(EMsvCreateEntry,TIpcArgs(aOperationId, aOwnerId)));
	}


	
/**
Changes the contents of a message server entry (asynchronously).

@param aEntry Entry to change
@param aOperationId Operation identifier
@param aOwnerId The ID of the owning process
@param aRequestStatus Asynchronous request status

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)

@internalComponent
*/
EXPORT_C void RMsvServerSession::ChangeEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus)
//
// Changes the content of an entry
//
	{
	TestSlotAvailableL();
	SendEntryDataL(aOperationId, aEntry);
	aRequestStatus=KRequestPending;
	SendReceive(EMsvChangeEntry, TIpcArgs(aOperationId, aOwnerId), aRequestStatus);
	}




/**
Changes the contents of a message server entry (synchronously).

@param aEntry Entry to create
@param aOwnerId The ID of the owning process
@param aOperationId Operation identifier

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)

@internalComponent
*/
EXPORT_C void RMsvServerSession::ChangeEntryL(const TMsvEntry& aEntry, TMsvOp aOperationId, TSecureId aOwnerId)
//
// Changes the content of an entry
//
	{
	TestSlotAvailableL();
	SendEntryDataL(aOperationId, aEntry);
	User::LeaveIfError(SendReceive(EMsvChangeEntry, TIpcArgs(aOperationId, aOwnerId)));
	}

/**
Update the specified Changes to selection of TMsvId (asynchronously).

@param aSelection The IDs of the entry to Change. 
@param aMark The read/Unread value.  
@param aOperationId Operation identifier
@param aOwnerId The ID of the owning process
@param aRequestStatus Asynchronous request status

*/

void RMsvServerSession::ChangeEntriesL(const CMsvEntrySelection& aSelection, TBool aMark ,TMsvOp aOperationId, TSecureId aOwnerId, TRequestStatus& aRequestStatus)
    {
     TestSlotAvailableL();
     TInt markvalue = (TInt)aMark;
     SendOperationDataL(aOperationId, aSelection, markvalue);
     aRequestStatus=KRequestPending;
     SendReceive(EMsvChangeEntries, TIpcArgs(aOperationId, aOwnerId), aRequestStatus);
    }

/**
Gets the index entry with the specified unique id.
@param aId ID of the entry to get
@param aService On return, the ID of the service to which the entry belongs
@param aEntry On return, the index entry with the specified ID
@return System wide error code

@capability None A client with no capabilities can access an entry only if owns that entry. 

@capability ReadUserData This is required to access any entry where the condition 
described for no capabilities does not apply.
*/
EXPORT_C TInt RMsvServerSession::GetEntry(TMsvId aId, TMsvId& aService, TMsvEntry& aEntry)
	{
    // pass the buffer and its max length to receive the data through
    TPtr8 ptr=iBuffer->Des();
    TInt maxLength = iBuffer->Des().MaxLength();
    
	// signal the server
    TInt error = SendReceive(EMsvGetEntry,TIpcArgs(aId, &ptr, maxLength));
    while(error == KErrOverflow)
        {
        // increase the size of the buffer and try again
        iBuffer->Des().SetLength(0);
        // TRAP here and return if leaving with error
        TRAP(error, iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength));
        if( error != KErrNone)
			{
            break;
			}
        // pass the buffer to receive the data through
        TPtr8 ptr=iBuffer->Des();
        maxLength = iBuffer->Des().MaxLength();
        error = SendReceive(EMsvGetEntry,TIpcArgs(aId,&ptr,maxLength));
        }

    if( error == KErrNone)
        {
        TMsvPackedEntry packedEntry(iBuffer);
        // Unpack the entry and servie id from same buffer
        packedEntry.UnpackEntryAndService(aEntry, aService);
        }

    return error;
	}

/**
Gets the children of the index entry with the specified unique id.

@param aId ID of the entry to get
@param aEntries On return, the entry's children. If an error is returned, aEntries is unchanged.
@param aOrdering Sorting and grouping rules for the returned entries
@return System wide error code

@capability None Only children that the client owns are returned. 

@capability ReadUserData All children of the entry are returned
*/
EXPORT_C TInt RMsvServerSession::GetChildren(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering)
	{
	TInt origCount=aEntries.Count();
	TRAPD(leave, DoGetChildrenL(aId, aEntries, aOrdering));
	if (leave!=KErrNone)
		{
		TInt count=aEntries.Count();
		while (count>origCount)
			{
			delete aEntries.At(--count);
			aEntries.Delete(count);
			}
		}
	return leave;
	}


void RMsvServerSession::DoGetChildrenL(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering)
//
// Gets the children of the index entry with the Unique id aId.
//
	{
	// package up the Id into a children details class
	TPckgBuf<TMsvChildrenDetails> details;
	details().iParentId = aId;
	// pass the sort order
	TPckgC<TMsvSelectionOrdering> package2(aOrdering);
	// pass the buffer to receive the data through
	TPtr8 ptr=iBuffer->Des();
	// signal the server
	TInt error = SendReceive(EMsvGetChildren, TIpcArgs(&details,&package2,&ptr));
		
	if (error!=KErrNone && error!=KErrOverflow)
		User::Leave(error);

	// unpack the entries from the buffer
	TMsvPackedEntryArray packedEntryArray(iBuffer);
	for (TInt count=0; count<details().iNumberChildrenInArray; count++)
		{
		TMsvEntry entry;
		User::LeaveIfError(packedEntryArray.UnpackEntry(count, entry));
		CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(entry, EMsvClientChild);
		aEntries.AppendL(cEntry);
		CleanupStack::Pop(); // cEntry
		}
	
	// if there are more entries - get them
	if (error==KErrOverflow)
		DoGetRemainingChildrenL(details, aEntries);
	}

void RMsvServerSession::DoGetRemainingChildrenL(TPckgBuf<TMsvChildrenDetails>& aDetails, CArrayPtrFlat<CMsvClientEntry>& aEntries)
//
// Gets the remaining children 
//
	{
	TInt error=KErrOverflow;
	while (error==KErrOverflow)
		{
		// package up the Id into a children details class & pass the buffer to receive the data through
		TPtr8 ptr=iBuffer->Des();
		// get some more entries
		error = SendReceive(EMsvGetRemainingChildren,TIpcArgs(&aDetails,NULL,&ptr));
		if (error!=KErrNone && error!=KErrOverflow)
			User::Leave(error);

		if (aDetails().iNumberChildrenInArray == 0)
			{
			if (error == KErrOverflow)
				{
				// The buffer isn't big enough for the current entry
				// we must resize the buffer or we're in an infinite loop
				iBuffer->Des().SetLength(0);
				iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
				}
			}
		else
			{
			// unpack the entries from the buffer
			TMsvPackedEntryArray packedEntryArray(iBuffer);
			for (TInt count=0; count<aDetails().iNumberChildrenInArray; count++)
				{
				TMsvEntry entry;
				User::LeaveIfError(packedEntryArray.UnpackEntry(count, entry));
				CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(entry, EMsvClientChild);
				aEntries.AppendL(cEntry);
				CleanupStack::Pop(); // cEntry
				}
			}
		}
	}



/**
Delete the specified message server entries (asynchronously).

@param aSelection Entries to delete
@param aOperationId Operation identifier
@param aRequestStatus Asynchronous request status

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::DeleteEntriesL(const CMsvEntrySelection& aSelection, TMsvOp aOperationId, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection);
	// start the operation
	aRequestStatus=KRequestPending;
	SendReceive(EMsvDeleteEntries, TIpcArgs(aOperationId), aRequestStatus);
	}


/**
Delete the specified message server entries (synchronously).

@param aSelection Entries to delete
@param aOperationId Operation identifier

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::DeleteEntriesL(const CMsvEntrySelection& aSelection, TMsvOp aOperationId)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection);
	User::LeaveIfError(SendReceive(EMsvDeleteEntries, TIpcArgs(aOperationId)));
	}

/**
Locks a message server entry.

This prevents it from being accessed by other clients. 

This function is intended only for test purposes.

@param aId ID of the entry to lock
@return System wide error code
@see RMsvServerSession::ReleaseEntry

@capability Note When the __REMOVE_MESSAGING_API_V1__ macro is defined, the
function is policed against the capabilities of the message server (Read/Write Device Data, 
Protected Server, Network Control, Network Services, Local Services and Read 
User Data). When the macro is not defined, the function is not policed. 
*/
EXPORT_C TInt RMsvServerSession::LockEntry(TMsvId aId)
	{
	return SendReceive(EMsvLockEntry, TIpcArgs(aId));
	}


/**
Releases the lock on an entry.

This function is intended only for test purposes.

@param aId ID of the locked entry to release
@return System wide error code
@see RMsvServerSession::LockEntry

@capability Note When the __REMOVE_MESSAGING_API_V1__ macro is defined, the
function is policed against the capabilities of the message server (Read/Write Device Data, 
Protected Server, Network Control, Network Services, Local Services and Read 
User Data). When the macro is not defined, the function is not policed. 
*/
EXPORT_C TInt RMsvServerSession::ReleaseEntry(TMsvId aId)
	{
	return SendReceive(EMsvReleaseEntry, TIpcArgs(aId));
	}


/**
Tests if a client can read from a specified message store.
A locked store cannot be read from.

@param aId ID of the entry that owns the store
@return KErrNone if the store can be read from; another system wide error code if not.

@capability None A client with no capabilities can access an entry only if owns that entry. 

@capability ReadUserData This is required to access any entry where the condition 
described for no capabilities does not apply.
*/
EXPORT_C TInt RMsvServerSession::ReadStore(TMsvId aId)
	{
	// package an TInt to get the id back
	return SendReceive(EMsvReadStore, TIpcArgs(aId));
	}


/**
Locks a store for writing.

@param aId ID of the entry that owns the store
@return System wide error code
@see RMsvServerSession::ReleaseStore

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C TInt RMsvServerSession::LockStore(TMsvId aId)
	{
	// package an TInt to get the id back
	return SendReceive(EMsvLockStore, TIpcArgs(aId));
	}

/**
Releases the lock on a store.

@param aId ID of the entry that owns the store
@return System wide error code
@see RMsvServerSession::LockStore

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C TInt RMsvServerSession::ReleaseStore(TMsvId aId)
	{
	return SendReceive(EMsvReleaseStore, TIpcArgs(aId));
	}


/**
Decrements the number of readers on a store.

@param aId ID of the entry that owns the store
@return System wide error code

@capability None A client with no capabilities can access an entry only if owns that entry. 

@capability ReadUserData This is required to access any entry where the condition 
described for no capabilities does not apply.
*/
TInt RMsvServerSession::DecStoreReaderCount(TMsvId aId)
	{
	return SendReceive(EMsvDecStoreReaderCount,TIpcArgs(aId));
	}


/**
Closes the message server.

The function results in the session sending a shutdown session notification 
(TMsvSessionEvent::EMsvCloseSession) to all current sessions. The Message 
Server closes when all sessions have been closed.
 
@capability WriteDeviceData
*/
EXPORT_C void RMsvServerSession::CloseMessageServer()
	{
	SendReceive(EMsvCloseServer);
	}


/**
Gets registry information for the MTMs for a specified MTM DLL type (client, server, UI, UI data).  

@param aMtmDllTypeUid UID of the MTM DLL type
@param aRegisteredMtmDllArray On return, an array of registry information for the MTMs of the specified type.
@param aTimeoutMicroSeconds32 
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::FillRegisteredMtmDllArray(TUid aMtmDllTypeUid,CRegisteredMtmDllArray& aRegisteredMtmDllArray,TTimeIntervalMicroSeconds32 aTimeoutMicroSeconds32)
	{
	// pass the buffer to receive the data through
	TPtr8 ptr=iBuffer->Des();
	// signal the server
	TInt error = SendReceive(EMsvFillRegisteredMtmDllArray,TIpcArgs(aMtmDllTypeUid.iUid, &ptr));
	if (error==KErrNone)
		{
		RDesReadStream readStream(*iBuffer);
		TRAP(error, TMsvPackedRegisteredMtmDllArray::UnpackRegisteredMtmDllArrayL(readStream, aRegisteredMtmDllArray, aTimeoutMicroSeconds32, *this));
		}

	return error;
	}

/**
Installs a new MTM group.
 
@param aFullName The full path name of the MTM group file
@return System wide error code

@capability WriteDeviceData
*/
EXPORT_C TInt RMsvServerSession::InstallMtmGroup(const TDesC& aFullName)
	{
	return SendReceive(EMsvInstallMtmGroup,TIpcArgs(&aFullName));
	}

/**
Uninstalls an MTM group.
 
@param aFullName The full path name of the MTM group file
@return System wide error code

@capability WriteDeviceData
*/
EXPORT_C TInt RMsvServerSession::DeInstallMtmGroup(const TDesC& aFullName)
	{
	return SendReceive(EMsvDeInstallMtmGroup,TIpcArgs(&aFullName));
	}

/**
Increments the usage reference count for a specified MTM group. 

@param aMtmTypeUid MTM group UID
@return System wide error code
@see RMsvServerSession::ReleaseMtmGroup

@capability None
*/
EXPORT_C TInt RMsvServerSession::UseMtmGroup(TUid aMtmTypeUid)
	{
	// package up the Id of the entry required
	return SendReceive(EMsvUseMtmGroup,TIpcArgs(aMtmTypeUid.iUid));
	}

/**
Decrements the usage reference count for a specified MTM group.

@param aMtmTypeUid MTM group UID
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::ReleaseMtmGroup(TUid aMtmTypeUid)
	{
	// package up the Id of the entry required
	return SendReceive(EMsvReleaseMtmGroup,TIpcArgs(aMtmTypeUid.iUid));
	}

/**
Gets the registration data for an MTM group.

@param aMtmTypeUid MTM group UID
@return Registration data for the specified MTM group
@leave System wide error code

@capability None
*/
EXPORT_C CMtmGroupData* RMsvServerSession::GetMtmGroupDataL(TUid aMtmTypeUid)
	{
	TPtr8 ptr=iBuffer->Des();
	// package up the Id of the entry required and pass the buffer to receive the data through
	User::LeaveIfError(SendReceive(EMsvGetMtmGroupData,TIpcArgs(aMtmTypeUid.iUid,&ptr)));
	RDesReadStream readStream(*iBuffer);
	return CMtmGroupData::NewL(readStream);
	}


/**
Gets capabilities required to use the specified MTM.

@param aMtmTypeUid MTM UID
@param aCapSet On return, capabilities required to use the specified MTM.

@capability None
*/
void RMsvServerSession::GetMtmRequiredCapabilitiesL(TUid aMtmTypeUid, TCapabilitySet& aCapSet) const
	{
	TPtr8 ptr=iBuffer->Des();
	User::LeaveIfError(SendReceive(EMsvGetMtmRequiredCapabilities, TIpcArgs(aMtmTypeUid.iUid, &ptr)));
	RDesReadStream readStream(*iBuffer);
	MsvSecurityCapabilitySetUtils::InternalizeL(readStream,aCapSet);
	}
	

/**
Cancels a specified operation.

@param aId Operation identifier
@param aProgress On return, progress information for the operation
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::CancelOperation(TMsvOp aId, TDes8& aProgress)
	{
	TInt ret = SendReceive(EMsvCancelOperation, TIpcArgs(aId,&aProgress));
	if (ret>0)
		{
		aProgress.SetLength(ret);
		ret=KErrNone;
		}
	else
		{
		aProgress.SetLength(0);
		if (ret==KErrNotReady)
			ret=KErrNone;
		}
	return ret;
	}


/**
Gets progress information for an operation.

@param aId Operation identifier
@param aProgress On return, progress information for the operation
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::OperationProgress(TMsvOp aId, TDes8& aProgress)
	{
	TInt ret = SendReceive(EMsvOperationProgress, TIpcArgs(aId,&aProgress));
	if (ret>0)
		{
		aProgress.SetLength(ret);
		ret=KErrNone;
		}
	else
		aProgress.SetLength(0);
	return ret;
	}

/**
Obtains the progress information that is defined by and understood by the system as well as client MTM's

@param aId Operation identifier
@param aProgress On return, progress information for the operation
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::OperationSystemProgress(TMsvOp aId, TMsvSystemProgress& aProgress)
	{
	TPckg<TMsvSystemProgress> sysProgressPkg(aProgress);
	return SendReceive(EMsvOperationSystemProgress, TIpcArgs(aId,&sysProgressPkg));
	}

/**
Obtains the final progress information for an operation.
The associated server side objects are then deleted.

@param aId Operation identifier
@param aProgress On return, progress information for the operation
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::OperationCompletion(TMsvOp aId, TDes8& aProgress)
	{
	TInt ret = SendReceive(EMsvOperationCompletion, TIpcArgs(aId,&aProgress));
	if (ret>0)
		{
		aProgress.SetLength(ret);
		ret=KErrNone;
		}
	else
		aProgress.SetLength(0);
	return ret;
	}


/**
Moves the specified message server entries (asynchronously).

@param aSelection The IDs of the entry to move. On return, contains the children that could not be fully moved. 
@param aTarget The ID of the new parent 
@param aOperationId Operation identifier
@param aRequestStatus Asynchronous request status

@capability None A client with no capabilities can move an entry only if both source and target entries
are under a local service, are owned by the client, and are in an unprotected folder.

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The function tests that the caller has capabilities to modify the 
the source entry (see RMsvServerSession::ChangeEntryL) and to create under the target
entry (see RMsvServerSession::CreateEntryL).
*/
EXPORT_C void RMsvServerSession::MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection, aTarget);
	// start the operation
	aRequestStatus=KRequestPending;
	SendReceive(EMsvMoveEntries, TIpcArgs(aOperationId), aRequestStatus);
	}

/**
Moves the specified message server entries (synchronously).

@param aSelection The IDs of the entry to move. On return, contains the children that could not be fully moved. 
@param aTarget The ID of the new parent entry
@param aOperationId Operation identifier

@capability None A client with no capabilities can move an entry only if both source and target entries
are under a local service, are owned by the client, and are in an unprotected folder.

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The function tests that the caller has capabilities to modify the 
the source entry (see RMsvServerSession::ChangeEntryL) and to create under the target
entry (see RMsvServerSession::CreateEntryL).
*/
EXPORT_C void RMsvServerSession::MoveEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection, aTarget);
	// start the operation
	User::LeaveIfError(SendReceive(EMsvMoveEntries, TIpcArgs(aOperationId)));
	}

/**
Copies the specified message server entries (synchronously).

@param aSelection The IDs of the source entries to copy. On return, contains the children that could not be fully copied. 
@param aTarget The ID of the destination parent entry
@param aOperationId Operation identifier

@capability None A client with no capabilities can copy an entry only if both source and target entries
are under a local service, are owned by the client, and are in an unprotected folder.

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The function tests that the caller has capabilities to create under the 
target entry (see RMsvServerSession::CreateEntryL).
*/
EXPORT_C void RMsvServerSession::CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection, aTarget);
	// start the operation
	User::LeaveIfError(SendReceive(EMsvCopyEntries, TIpcArgs(aOperationId)));
	}


/**
Copies the specified message server entries (asynchronously).

@param aSelection The IDs of the source entries to copy. On return, contains the children that could not be fully copied. 
@param aTarget The ID of the destination parent entry
@param aOperationId Operation identifier
@param aRequestStatus Asynchronous request status

@capability None A client with no capabilities can copy an entry only if both source and target entries
are under a local service, are owned by the client, and are in an unprotected folder.

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The function tests that the caller has capabilities to create under the 
target entry (see RMsvServerSession::CreateEntryL).
*/
EXPORT_C void RMsvServerSession::CopyEntriesL(const CMsvEntrySelection& aSelection, TMsvId aTarget, TMsvOp aOperationId, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	// send the data to the server
	SendOperationDataL(aOperationId, aSelection, aTarget);
	// start the operation
	aRequestStatus=KRequestPending;
	SendReceive(EMsvCopyEntries, TIpcArgs(aOperationId), aRequestStatus);
	}


/**
Passes an MTM-specific operation to the associated server-side MTM by means of the Message Server (asynchronously). 

@param aSelection A selection of entries that may be relevant to the operation 
@param aCommandId The command ID. The interpretation of the command is MTM-specific. 
@param aParameter A descriptor containing operation-specific parameters 
@param aOperationId Operation identifier
@param aRequestStatus Asynchronous request status

@capability ReadUserData    
@capability WriteUserData
@capability Note A client application needs to have any additional capabilities that are specified by 
the MTM for the message type, as well as ReadUserData and WriteUserData. 

@see RMsvServerSession::GetMTMRequiredCapabilitiesL
*/
EXPORT_C void RMsvServerSession::TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TMsvOp aOperationId, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	// send the operation data to the server
	SendCommandDataL(aOperationId, aSelection, aCommandId, aParameter);
	// start the operation
	aRequestStatus=KRequestPending;
	SendReceive(EMsvMtmCommand, TIpcArgs(aOperationId), aRequestStatus);
	}

/**
Passes an MTM-specific operation to the associated server-side MTM by means of the Message Server (synchronously). 

@param aSelection A selection of entries that may be relevant to the operation 
@param aCommandId The command ID. The interpretation of the command is MTM-specific. 
@param aParameter A descriptor containing operation-specific parameters 
@param aOperationId Operation identifier

@capability ReadUserData    
@capability WriteUserData
@capability Note A client application needs to have any additional capabilities that are specified by 
the MTM for the message type, as well as ReadUserData and WriteUserData. 

@see RMsvServerSession::GetMTMRequiredCapabilitiesL
*/
EXPORT_C TInt RMsvServerSession::TransferCommandL(const CMsvEntrySelection& aSelection, TInt aCommandId, const TDesC8& aParameter, TMsvOp aOperationId)
	{
	TestSlotAvailableL();
	// send the operation data to the server
	SendCommandDataL(aOperationId, aSelection, aCommandId, aParameter);
	// start the operation
	return SendReceive(EMsvMtmCommand, TIpcArgs(aOperationId));
	}


/**
Gets the MTM that would perform an operation using two specified entries.

@param aId1 ID of first entry
@param aId2 ID of second entry
@param aMtm On return, the UID of the MTM that would be used 
@param aService On return, the ID of the service that would be used 
@return System wide error code

@capability None
*/
EXPORT_C TInt RMsvServerSession::OperationMtmL(TMsvId aId1, TMsvId aId2, TUid& aMtm, TMsvId& aService)
	{
	TPckg<TUid> uid(aMtm);
	TPckg<TMsvId> service(aService);
	return SendReceive(EMsvOperationMtm, TIpcArgs(aId1,aId2,&uid,&service));
	}


/**
Stops any operations that a Server-side MTM for the specified service is running, 
and then unloads the Server-side MTM.

The current operation and any queued operations are cancelled.
 
@param aServiceId ID of the service
@return System wide error code

@capability Note A client application needs to have any capabilities that are specified by 
the MTM.

@see RMsvServerSession::GetMTMRequiredCapabilitiesL
*/
EXPORT_C TInt RMsvServerSession::StopService(TMsvId aServiceId)
	{
	return SendReceive(EMsvStopService, TIpcArgs(aServiceId));
	}


/**
Tests if a Server-side MTM for a particular service is loaded by the Message 
Server.

The Server-side MTM does not have to be executing a command: it may be 
waiting for another command.

@param aServiceId The ID of the relevant service 
@return ETrue if the Server-side MTM for the service is loaded, otherwise EFalse 

@capability None
*/
EXPORT_C TBool RMsvServerSession::ServiceActive(TMsvId aServiceId)
	{
	return SendReceive(EMsvServiceActive, TIpcArgs(aServiceId)); 
	}

/** 
Gets the current progress information from the Server-side MTM for the specified 
service.

It is typically used by User Interface MTMs. The format of the progress information 
returned in the aProgress buffer is MTM-specific.

Calling this function results in the Message Server calling CBaseServerMtm::Progress() 
on the relevant Server-side MTM. 

Note that the progress information is independent of which message client 
application started the current operation. 

@param aServiceId The ID of the service from which to get the progress information 
@param aProgress On return, a descriptor holding progress information. It is 
the caller's responsibility to ensure the descriptor is large enough for this information. 
@return KErrNone: success; KErrNotFound: The service is not active (the relevant Server-side MTM is not loaded 
by the Message Server); KErrOverflow: The descriptor was too small for the progress information

@capability None
*/
EXPORT_C TInt RMsvServerSession::ServiceProgress(TMsvId aServiceId, TDes8& aProgress)
	{
	TInt ret = SendReceive(EMsvServiceProgress, TIpcArgs(aServiceId,&aProgress));
	if (ret>0)
		{
		aProgress.SetLength(ret);
		ret=KErrNone;
		}
	else
		aProgress.SetLength(0);
	return ret;
	}

/** 
Deletes the specified entry from the Message Server. 

The call is guaranteed not to fail. If the entry cannot be deleted immediately, 
it will be deleted later.

@param aId The ID of the entry to remove 

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::RemoveEntry(TMsvId aId)
	{
	SendReceive(EMsvRemoveEntry, TIpcArgs(aId));
	}

/**
Gets the path of the folder in which the Message Server data is stored.

@param aDirectory Descriptor to hold returned path 
@return System wide error code
*/
EXPORT_C TInt RMsvServerSession::GetMessageDirectory(TDes& aDirectory)
	{
	return SendReceive(EMsvGetMessageDirectory, TIpcArgs(&aDirectory));
	}

/**
Gets the drive on which the Message Server data is stored.

@return Drive, as a TDriveNumber value

@capability None
*/
TInt RMsvServerSession::GetMessageDrive()
	{
	return SendReceive(EMsvGetMessageDrive);
	}


void RMsvServerSession::TestSlotAvailableL()
//
//
//
	{
	User::LeaveIfError(SendReceive(EMsvSlotAvailable));
	}


/**
Set failure condition to simulate (for test purposes).
@param aType 
@param aArg1 
@param aArg2 
@param aArg3 

@capability Note When the __REMOVE_MESSAGING_API_V1__ macro is defined, the
function is policed against the capabilities of the message server (Read/Write Device Data, 
Protected Server, Network Control, Network Services, Local Services and Read 
User Data). When the macro is not defined, the function is not policed. 
*/
EXPORT_C void RMsvServerSession::SetFailure(TInt aType, TInt aArg1, TInt aArg2, TInt aArg3)
	{
	SendReceive(EMsvSetFailure,TIpcArgs(aType,aArg1,aArg2,aArg3));
	}

/** Sets or clears multiple fields in a selection of entries.

Fields to change are specified using a bitmask of TMsvAttribute values. 
@param aSelection The entries to change
@param aSetAttributes A bitmask of the fields to set
@param aClearAttributes A bitmask of the fields to clear
@see TMsvAttribute 

@capability WriteUserData 

@capability Note Clients with no capabilities that own entries in an unprotected folder cannot use this IPC call. 
Instead, they must change attributes by using ChangeEntryL on each entry.
*/
EXPORT_C void RMsvServerSession::ChangeAttributesL(const CMsvEntrySelection& aSelection, TUint aSetAttributes, TUint aClearAttributes)
	{
	PackOperationDataL(aSelection, aSetAttributes, aClearAttributes);
	User::LeaveIfError(SendReceive(EMsvChangeAttributes,TIpcArgs(iBuffer)));
	}

/**
Gets a filtered list of children of a specified message entry.
@param aId Message entry of which to get children
@param aFilter Filter by which various message entries can be excluded
@param aSelection On return, a list of message entry IDs

@capability None Only children that the client owns are returned. 

@capability ReadUserData All children of the entry are returned
*/
EXPORT_C void RMsvServerSession::GetChildIdsL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
	{
	TMsvPackedEntryFilter package(iBuffer);
	User::LeaveIfError(package.PackFilter(aFilter));

	TPtr8 ptr=iBuffer->Des();

	TInt error = SendReceive(EMsvGetChildIds,TIpcArgs(&ptr,aId));

	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		TMsvPackedEntryFilter package1(iBuffer);
		User::LeaveIfError(package1.PackFilter(aFilter));
		TPtr8 ptr2=iBuffer->Des();
		error = SendReceive(EMsvGetChildIds,TIpcArgs(&ptr2,aId));
		}	
	User::LeaveIfError(error);
	TInt temp1(0); 
	TInt temp2(0);
	TMsvPackedOperation op(iBuffer);
	op.UnpackL(aSelection, temp1, temp2);
	}




/** 
Load the Message Server index from the specified drive.

Progress information is provided by a TMsvIndexLoadProgress object.

If an error occurs, the index is unchanged.

@param aDrive The drive holding the Message Server index, specified 
by a TDriveNumber value
@param aOperationId Operation identifier
@param aRequestStatus Asynchronous completion status 
@leave KErrServerBusy Cannot change drive because there are outstanding Message 
Server operations.

@capability WriteDeviceData
*/
void RMsvServerSession::ChangeDriveL(TInt aDrive, TMsvOp aOperationId, TRequestStatus& aRequestStatus)
	{
	__ASSERT_ALWAYS(RFs::IsValidDrive(aDrive), PanicServer(EMsvInvalidDrive));
	TestSlotAvailableL();

	// start the operation
	aRequestStatus = KRequestPending;
	SendReceive(EMsvChangeDrive, TIpcArgs(aOperationId,TInt(aDrive)), aRequestStatus);
	}
	
	
/** 
Copy the Message Store to the specified drive.

Progress information is provided by a TMsvCopyProgress object.

If an error occurs, copying is stopped.

@param aDrive The drive holding the Message Server index, specified 
by a TDriveUnit value
@param aOperationId Operation identifier
@param aStatus Asynchronous completion status 
@leave KErrServerBusy Cannot copy store because there are outstanding Message 
Server operations.

@capability WriteDeviceData*/

void RMsvServerSession::CopyStoreL(const TDriveUnit& aDrive, TMsvOp aOperationId, TRequestStatus& aStatus)                     
	{
	__ASSERT_ALWAYS(RFs::IsValidDrive(aDrive), PanicServer(EMsvInvalidDrive));
	TestSlotAvailableL();

	// start the operation
	SendReceive(EMsvCopyStore, TIpcArgs(aOperationId,TInt(aDrive)), aStatus);
	}

	
/** 
Delete the Message Store to the specified drive.

Progress information is provided by a TMsvDeleteProgress object.

If an error occurs, deleting is stopped.

@param aDrive The drive holding the Message Server index, specified 
by a TDriveUnit value
@param aOperationId Operation identifier
@param aStatus Asynchronous completion status 
@leave KErrServerBusy Cannot delete store because there are outstanding Message 
Server operations.

@capability WriteDeviceData
*/
void RMsvServerSession::DeleteStoreL(const TDriveUnit& aDrive, TMsvOp aOperationId, TRequestStatus& aStatus)                     
	{
	__ASSERT_ALWAYS(RFs::IsValidDrive(aDrive), PanicServer(EMsvInvalidDrive));
	TestSlotAvailableL();

	// start the operation
	SendReceive(EMsvDeleteStore, TIpcArgs(aOperationId,TInt(aDrive)), aStatus);
	}
	
	

/**
Gets the number of outstanding operations.

@return The number of outstanding operations 

@capability None
*/
TInt RMsvServerSession::OutstandingOperationsL()
	{
	TInt count;
	TPckg<TBool> package(count);

	User::LeaveIfError(SendReceive(EMsvOutstandingOperations, TIpcArgs(&package)));
	return count;
	}

/**
Get the notification sequence number.

@return Notification sequence number

@capability None
*/
TUint32 RMsvServerSession::NotifySequenceL()
	{
	TUint32 sequence;
	TPckg<TUint32> package(sequence);

	User::LeaveIfError(SendReceive(EMsvGetNotifySequence, TIpcArgs(&package)));
	return sequence;
	}


/**
Sets whether the session's observer should be notified of events related to 
changes in message server entries.

If the flag is set to true, the session observer will be notified of all events types. 
If it is set to false, the observer will not be notified of events of types EMsvEntriesCreated, 
EMsvEntriesChanged, EMsvEntriesDeleted, or EMsvEntriesMoved. (Event types are enumerated in  
MMsvSessionObserver::TMsvSessionEvent.) 

@param aReceive Event request flag
@return System wide error code
@see MMsvSessionObserver::TMsvSessionEvent

@capability None
*/
EXPORT_C TInt RMsvServerSession::SetReceiveEntryEvents(TBool aReceive)
	{
	// Send the uid type in message and Make request
	return SendReceive(EMsvSetReceiveEntyEvents, TIpcArgs(aReceive));
	}

// from MMsvStoreManager
/** 
Creates a new attachment file in the message store for the message entry specified.
This function allows clients to create a new empty file and stream data to it. 
The empty attachment file is passed back to the client using an opened file handle 
to the file in the message store.

@param aEntryId Entry to create the attachment for
@param aFileName The attachment filename
@param aFile On return, open file handle to the new attachment file . Ownership is transferred. The caller must close the file handle .
@return True if the file created was named as specified, otherwise false 

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C TBool RMsvServerSession::CreateAttachmentForWriteL(TMsvId aEntryId, TDes& aFileName, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);  // intitialise
	TPckg<TBool> pckgFileNameChanged(EFalse);  // intitialise
	// session handle (RFs)received from TransferToClient() in server			
	TInt sessionHandle = User::LeaveIfError(SendReceive(EMsvCreateAttachmentForWrite,TIpcArgs(aEntryId,&aFileName,&pckgSubSessionHandle,&pckgFileNameChanged)));
	
	// Adopt the file handle from server
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));

	return pckgFileNameChanged();
	}

/** 
Allows clients to replace an existing attachment file and allows clients to stream data to it.

Replaces an existing attachment file in the message store for the message entry specified by the aEntryId 
parameter. The attachment filename is set to the filename indicated by the aFilename parameter. 
If an attachment with the supplied name does not exist, a new file is created. The empty attachment
file is passed back to the client using an opened file handle to the file in the message store.

@param aEntryId The entry for which an attachment needs to be created in the message store
@param aFilename The name of the attachment file to be created
@param aFile On return, The created file handle for the attachment file.Ownership is transferred . The caller must close the file handle .

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/	
EXPORT_C void RMsvServerSession::ReplaceAttachmentForWriteL(TMsvId aEntryId, TDes& aFileName, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);  // intitialise

	// session handle (RFs)received from TransferToClient() in server			
	TInt sessionHandle = User::LeaveIfError(SendReceive(EMsvReplaceAttachmentForWrite,TIpcArgs(aEntryId,&aFileName,&pckgSubSessionHandle)));
	
	// Adopt the file handle from server
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));
	}

/** 
Opens an existing attachment file in the message store for the message entry specified.
This function allows clients to view attachment files.

@param aEntryId Entry that has the attachment
@param aFilePath The attachment filename
@param aFile On return, open file handle to the attachment file. Ownership is transferred . The caller must close the file handle .

@capability None A client with no capabilities can access an entry only if owns that entry. 

@capability ReadUserData This is required to access any entry where the condition 
described for no capabilities does not apply.
*/
EXPORT_C void RMsvServerSession::OpenAttachmentL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);   // initialise
	// session handle (RFs)received from TransferToClient() in server			
	TInt sessionHandle = SendReceive(EMsvOpenAttachment,TIpcArgs(aEntryId,&aFilePath,&pckgSubSessionHandle));
	
	// Adopt the file handle from server
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));
	}

	
/** 
Opens an existing attachment file in the message store for the message entry specified.
This function allows clients to edit attachment files.

@param aEntryId Entry that has the attachment
@param aFilePath The attachment filename
@param aFile On return, open file handle to the attachment file .Ownership is transferred . The caller must close the file handle .

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry.

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::OpenAttachmentForWriteL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);   // initialise
	// session handle (RFs)received from TransferToClient() in server			
	TInt sessionHandle = SendReceive(EMsvOpenAttachmentForWrite,TIpcArgs(aEntryId,&aFilePath,&pckgSubSessionHandle));
	
	// Adopt the file handle from server
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));
	}

/** 
Deletes an existing attachment file in the message store.

@param aEntryId Entry that has the attachment
@param aFilePath The attachment filename

@return System wide error code 

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C TInt RMsvServerSession::DeleteAttachment(TMsvId aEntryId, const TDesC& aFilePath)
	{
	return SendReceive(EMsvDeleteAttachment,TIpcArgs(aEntryId,&aFilePath));
	}

/** 
Renames an existing attachment file.

Renames an existing attachment file in the message store associated with the
message entry specified by the aEntryId parameter. The attachment is 
identified by the file path specified by the aOldFilePath parameter and renamed to
the name supplied in the aNewName paramter.

@param aEntryId The entry whose attachment needs to be renamed from the message store
@param aOldFilePath The name and path of the attachment file to be renamed.
@param aNewName The new name of the attachment file.
@return KErrNone if successful, otherwise any of the system wide error codes.

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/	
EXPORT_C TInt RMsvServerSession::RenameAttachment(TMsvId aEntryId, const TDesC& aOldFilePath, const TDesC& aNewName)
	{
	return SendReceive(EMsvRenameAttachment,TIpcArgs(aEntryId, &aOldFilePath, &aNewName));
	}

/** 
Checks if the attachment filename specified exists in the store.

@param aFilePath The complete attachment file path specification

@capability None

@return ETrue if the attachment exists, otherwise EFalse
*/
EXPORT_C TBool RMsvServerSession::FileExistsL(const TDesC& aFilePath)
	{
	TInt err = SendReceive(EMsvFileExists, TIpcArgs(&aFilePath));
	if( err == KErrNotFound )
		{
		return EFalse;
		}
		
	User::LeaveIfError(err);
	return ETrue;
	}

/** 
Get the file path where the attachments for a particular entry are stored.

@param aEntryId The message entry Id.
@param aFilePath On return this will contain the filepath for the attachments.

@capability None
*/
EXPORT_C void RMsvServerSession::AttachmentFilePathL(TMsvId aEntryId, TDes& aFilePath)
	{
	User::LeaveIfError(SendReceive(EMsvGetAttachmentFilePath, TIpcArgs(aEntryId, &aFilePath)));
	}

/** 
Opens for reading a message store file for the message entry specified.

@param aEntryId Entry that has the store
@param aFile On return, open file handle to the store.Ownership is transferred . The caller must close the file handle .

@capability None A client with no capabilities can access an entry only if owns that entry. 

@capability ReadUserData This is required to access any entry where the condition 
described for no capabilities does not apply.
*/
EXPORT_C TInt RMsvServerSession::OpenFileStoreForRead(TMsvId aEntryId, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);  // initialise

	// session handle (RFs) received from TransferToClient() in server	
	TInt sessionHandle = SendReceive(EMsvOpenFileStoreForRead,TIpcArgs(aEntryId,&pckgSubSessionHandle));
	if( sessionHandle < KErrNone )
		return sessionHandle;
	
	// Adopt the file handle from server
	return aFile.AdoptFromServer(sessionHandle ,pckgSubSessionHandle());
	}

/** 
Opens a temporary store file for the message entry specified.

The function returns an open read-write file handle with an exclusive share to the temporary store file. 
This method, along with ReplaceFileStoreL, allows the message store classes to write data to the store 
file for a particular message entry.

@param aEntryId Entry with which to associate the store
@param aFile On return, open file handle to the store. Ownership is transferred . The caller must close the file handle .

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::OpenTempStoreFileL(TMsvId aEntryId, RFile& aFile)
	{
	TPckg<TInt> pckgSubSessionHandle(0);  // intitialise
	
	// session handle (RFs) received from TransferToClient() in server
	TInt sessionHandle = SendReceive(EMsvOpenTempStoreFile,TIpcArgs(aEntryId,&pckgSubSessionHandle));
	
	// Adopt the file handle from server
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle ,pckgSubSessionHandle()));
	}
	
/** 
Replaces the current store file with the temporary store file, created from OpenTempStoreFileL, for the specified message entry.

@param aEntryId Entry associated with the store

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::ReplaceFileStoreL(TMsvId aEntryId)
	{
	User::LeaveIfError(SendReceive(EMsvReplaceFileStore,TIpcArgs(aEntryId)));
	}

/** 
Deletes the store file associated with the specified message entry.

@capability None A client with no capabilities can access an entry only if the entry is    
under a local service, is owned by the client, is in an unprotected folder,  
and is not a service entry. 

@capability WriteDeviceData Required if the entry is a service. 

@capability ReadUserData Required in some circumstances: see the note below.   

@capability WriteUserData Required in some circumstances: see the note below.

@capability Note The capabilities required to access an entry that is not a service vary depending on the 
following conditions:  

- Entry is under the local service:
	-# Entry is in the Outbox and the Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities
	-# Entry is in the Outbox and the Outbox is unprotected: MTM-specified capabilities
	-# Entry is in a protected folder: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is not owned by the client: ReadUserData + WriteUserData
	-# Entry is in an unprotected folder and the entry is owned by the client: no capabilities

- Entry is under a remote service:
	-# The Outbox is protected: ReadUserData + WriteUserData + MTM-specified capabilities (of the MTM for the remote service)
	-# The Outbox is unprotected and the entry is owned by the client: MTM-specified capabilities (of the MTM for the remote service) 
	-# The Outbox is unprotected and the entry is not owned by the client: ReadUserData + WriteUserData + 
	MTM-specified capabilities (of the MTM for the remote service)
*/
EXPORT_C void RMsvServerSession::DeleteFileStoreL(TMsvId aEntryId)
	{
	User::LeaveIfError(SendReceive(EMsvDeleteFileStore,TIpcArgs(aEntryId)));
	}

/** 
Tests if a store file exists for the specified message entry.

@param aEntryId Entry associated with the store

@return ETrue if the store file exists, otherwise EFalse.

@capability None
*/
EXPORT_C TBool RMsvServerSession::FileStoreExistsL(TMsvId aEntryId) const
	{
	return SendReceive(EMsvFileStoreExists,TIpcArgs(aEntryId));
	}

/** 
Checks a flag in the message server that is set when the server
deletes a corrupt index file. Then clears the flag.
	
It is intended to allow the message centre UI to check whether
a corrupt index has been deleted on startup. If it has the UI
may inform the user about the loss of their messages.
    
@return ETrue if a corrupt index flag was present indicating that
a corrupt index file has been deleted since the last time this
function was called.
 
@capability None
*/
TBool RMsvServerSession::GetAndClearIndexCorruptFlagL()
	{
	TInt result=User::LeaveIfError(SendReceive(EMsvGetAndClearIndexCorruptFlag));
	return result==0 ? EFalse : ETrue;
	}
	
/** Checks if the specified drive has a Message Server index on it.

@param aDrive Drive to check
@return True if aDrive has a Message Server index, otherwise false
@capability None
*/
TBool RMsvServerSession::DriveContainsStoreL(TDriveUnit aDrive)
	{
	TInt result=User::LeaveIfError(SendReceive(EMsvDriveContainsStore,TIpcArgs(aDrive)));
	return result==0 ? EFalse : ETrue;
	}

/** Checks to see if the currently selected drive contains the correct mail store.

If the message store is present on an external drive, we have to check if the same drive is 
mounted or not before storing the mail.

@return ETrue if the same drive is mounted. otherwise returns EFalse
@capability None
*/
EXPORT_C TBool RMsvServerSession::MessageStoreDrivePresentL()
	{
	TInt result = User::LeaveIfError(SendReceive(EMsvMessageStoreDrivePresent));	
	return result== 0 ? EFalse : ETrue;
	}

EXPORT_C TBool RMsvServerSession::CreateShareProtectedAttachmentForWriteL(TMsvId /*aEntryId*/, TDes& /*aFilename*/, RFile& /*aFile*/)
	{
	User::Leave(KErrNotSupported);
	//Avoid complier errors and warnings...
	return EFalse;
	}

/** 
Get the file path where the plain body text for a particular entry will be stored.
@param aBodyTextId 	The TMsvId of the body text entry.
@param aFilePath 	On return this will contain the filepath for the plainbody text.
@capability None
*/
EXPORT_C void RMsvServerSession::BodyTextFilePathL(TMsvId aBodyTextId, TDes& aFilePath)
 	{
 	User::LeaveIfError(SendReceive(EMsvGetBodyTextFilePath, TIpcArgs(aBodyTextId, &aFilePath)));
	}

/** 
Open the bodytext file whose path is given in aFileName.
@param aFile	 	 The RFile reference that is to be opened.
@param aBodyTextId	 The TMsvId of the body text entry.
@capability ReadUserData
*/
EXPORT_C void RMsvServerSession::OpenBodyTextFileForReadL(RFile& aFile, TMsvId aBodyTextId, const TDesC& aFilePath)
	{
	TPckg<TInt> pckgSubSessionHandle(0);
	TInt sessionHandle = User::LeaveIfError(SendReceive(EMsvOpenTextFileForRead, TIpcArgs(aBodyTextId,  &aFilePath, &pckgSubSessionHandle)));
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));
	}

/** 
Create a plainbody text file whose path is given in aFilePath.
@param aFile 	  	The RFile reference that is to be opened.
@param aBodyTextId  The TMsvId of the body text entry.
@capability WriteUserData
*/
EXPORT_C void RMsvServerSession::CreatePlainTextFileL(RFile& aFile, TMsvId aBodyTextId)
	{
	TPckg<TInt> pckgSubSessionHandle(0);
	TInt sessionHandle = User::LeaveIfError(SendReceive(EMsvCreatePlainTextFile, TIpcArgs(aBodyTextId, &pckgSubSessionHandle)));
	User::LeaveIfError(aFile.AdoptFromServer(sessionHandle,pckgSubSessionHandle()));
	}

/** 
Deletes  a plainbody text file whose path is given in aFilePath.
@param aBodyTextId	 The TMsvId of the body text entry.
@capability WriteUserData
*/
EXPORT_C void RMsvServerSession::DeletePlainTextFileL(TMsvId aBodyTextId)
	{
	User::LeaveIfError(SendReceive(EMsvDeletePlainTextFile, TIpcArgs(aBodyTextId)));
	}

/** 
Replaces  a plainbody text file whose Id is given in aBodyTextId.
@param aBodyTextId 	  The TMsvId of the body text entry.
@capability WriteUserData
*/	
EXPORT_C void RMsvServerSession::ReplacePlainTextFileL(TMsvId aBodyTextId)
	{
	User::LeaveIfError(SendReceive(EMsvReplacePlainTextFile,TIpcArgs(aBodyTextId)));
	}



#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
/**
 * GetChildrenAll()
 *
 * @param TMsvId:                ID of the parent entry.
 * @param TMsvSelectionOrdering: Sorting and grouping rules for the returned entries.
 * @param CArrayPtrFlat<CMsvClientEntry>: On return, the entry's children. If an error is returned, aEntries is unchanged.
 * @return TInt:                 System wide error code
 *
 * @capability None Only children that the client owns are returned. 
 * @capability ReadUserData All children of the entry are returned
 *
 * Gets the children of a parent entry specified as first argument.
 * If the passed parent is a standard folder the function will fetch 
 * entries from all drives currently present in the preferred drive list.
 */
EXPORT_C TInt RMsvServerSession::GetChildrenAll(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering)
	{
	TInt origCount = aEntries.Count();
	TInt leave = KErrNone;
	
	TRAP(leave, DoGetChildrenAllL(aId, aEntries, aOrdering));
	if(KErrNone != leave)
		{
		TInt count=aEntries.Count();
		while(count > origCount)
			{
			delete aEntries.At(--count);
			aEntries.Delete(count);
			}
		}
	return leave;
	}


void RMsvServerSession::DoGetChildrenAllL(TMsvId aId, CArrayPtrFlat<CMsvClientEntry>& aEntries, const TMsvSelectionOrdering& aOrdering)
//
// Gets the children of the index entry with the Unique id aId.
//
	{
	// package up the Id into a children details class
	TPckgBuf<TMsvChildrenDetails> details;
	details().iParentId = aId;
	// pass the sort order
	TPckgC<TMsvSelectionOrdering> package2(aOrdering);
	// pass the buffer to receive the data through
	TPtr8 ptr=iBuffer->Des();
	
	// signal the server
	TInt error = SendReceive(EMsvGetChildrenALL, TIpcArgs(&details,&package2,&ptr));

	if (error!=KErrNone && error!=KErrOverflow)
		User::Leave(error);

	// unpack the entries from the buffer
	TMsvPackedEntryArray packedEntryArray(iBuffer);
	for (TInt count=0; count<details().iNumberChildrenInArray; count++)
		{
		TMsvEntry entry;
		User::LeaveIfError(packedEntryArray.UnpackEntry(count, entry));
		CMsvClientEntry* cEntry = CMsvClientEntry::NewLC(entry, EMsvClientChild);
		aEntries.AppendL(cEntry);
		CleanupStack::Pop(); // cEntry
		}
	
	// if there are more entries - get them
	if (error==KErrOverflow)
		DoGetRemainingChildrenL(details, aEntries);
	}
	
	
	

/** 
 * GetChildIdsAllL()
 *
 * @param TMsvId:          ID of the parent entry.
 * @param CMsvEntryFilter: Filter by which various message entries can be excluded.
 * @return aSelection:     A list of message entry IDs
 *
 * @capability None Only children that the client owns are returned. 
 * @capability ReadUserData All children of the entry are returned
 *
 * Gets the children of a parent entry specified as first argument.
 * If the passed parent is a standard folder the function will fetch 
 * entries from all drives currently present in the preferred drive list.
 */
EXPORT_C void RMsvServerSession::GetChildIdsAllL(TMsvId aId, const CMsvEntryFilter& aFilter, CMsvEntrySelection& aSelection)
	{
	TMsvPackedEntryFilter package(iBuffer);
	User::LeaveIfError(package.PackFilter(aFilter));

	TPtr8 ptr=iBuffer->Des();

	TInt error = SendReceive(EMsvGetChildIdsALL, TIpcArgs(&ptr, aId));
	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		TMsvPackedEntryFilter package1(iBuffer);
		User::LeaveIfError(package1.PackFilter(aFilter));
		TPtr8 ptr2=iBuffer->Des();
		error = SendReceive(EMsvGetChildIdsALL,TIpcArgs(&ptr2,aId));
		}
		
	User::LeaveIfError(error);
	TInt temp1(0); 
	TInt temp2(0);
	TMsvPackedOperation op(iBuffer);
	op.UnpackL(aSelection, temp1, temp2);
	}




/**
 * CurrentDriveInfo()
 *
 * @return TDriveNumber: Drive number of the current drive
 *						 in the preferred drive list.
 * @return TUint	   : Priority of the drive.
 *
 * Code changes for PREQ 557.
 * The function returns the current drive of message server.
 */ 
EXPORT_C void RMsvServerSession::CurrentDriveInfoL(TDriveNumber& aDriveNumber, TUint& aPriority)
	{
	TPckg<TDriveNumber> driveNum(aDriveNumber);
	TPckg<TUint> priority(aPriority);
		
	User::LeaveIfError(SendReceive(EMsvGetCurrentDriveInfo, TIpcArgs(&driveNum, &priority)));
	}



/**
 * DriveList()
 *
 * @return RArray<TDriveNumber>: List of drive number present in 
 *         the preferred drive list arranged in decrementing order
 *         of their priority.
 *
 * Code changes for PREQ 557.
 */
EXPORT_C void RMsvServerSession::DriveListL(RArray<TDriveNumber>& aDriveList)
	{
	TPtr8 ptr = iBuffer->Des();	
	User::LeaveIfError(SendReceive(EMsvGetDriveList, TIpcArgs(&ptr)));

	// Unpack the entries from the buffer.
	TMsvPackedDriveIdOperation packedDriveIdOperation(iBuffer);
	packedDriveIdOperation.UnpackL(aDriveList);	
	}




/**
 * AvailableDriveListL()
 *
 * @return RArray<TDriveNumber>: List of drive number of available
 *         drives present in the preferred drive list arranged in 
 *         decrementing order of their priority.
 *
 * Code changes for PREQ 557.
 * NOTE: A drive number is available only if it has a valid 
 * (readable and of correct version) message store in it.
 */
EXPORT_C void RMsvServerSession::AvailableDriveListL(RArray<TDriveNumber>& aDriveList)
	{
	TPtr8 ptr = iBuffer->Des();	
	User::LeaveIfError(SendReceive(EMsvGetAvailableDriveList, TIpcArgs(&ptr)));
	
	// Unpack the entries from the buffer.
	TMsvPackedDriveIdOperation packedDriveIdOperation(iBuffer);
	packedDriveIdOperation.UnpackL(aDriveList);	
	}




/**
 * AddDriveL()
 *
 * @param  TDriveNumber: Drive number of the drive
 *						 to be added.
 * @param TUint        : Priority of the drive.
 * @return TUint       : New priority of the added drive.
 *
 * Code changes for PREQ 557.
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
 */ 
EXPORT_C void RMsvServerSession::AddDriveL(TDriveNumber aDriveNumber, TUint& aPriority)
	{
	TUint newPriority = aPriority;
	TPckg<TUint> priorityPkg(newPriority);
	if(!RFs::IsValidDrive(aDriveNumber))
		{
		User::Leave(KErrArgument);
		}
	User::LeaveIfError(SendReceive(EMsvAddDriveToDriveList, TIpcArgs(aDriveNumber, aPriority, &priorityPkg)));
	aPriority = newPriority;
	}



/**
 * RemoveDriveL()
 *
 * @param  TDriveNumber: Drive number of the drive
 *						 to be removed.
 *
 * Code changes for PREQ 557.
 * The function removes the drive from the preferred
 * drive list. If the current drive is being removed, 
 * the next available drive in the preferred drive list 
 * becomes the current drive. The device internal drive 
 * cannot be removed from the preferred drive list. 
 * Appropriate notification will be sent to the client 
 * process if there is a change in the current drive. 
 * The function will also update the central repository 
 * accordingly.
 */ 
 EXPORT_C void RMsvServerSession::RemoveDriveL(TDriveNumber aDriveNumber)
 	{
	if(!RFs::IsValidDrive(aDriveNumber))
		{
		User::Leave(KErrArgument);
		}
	User::LeaveIfError(SendReceive(EMsvRemoveDriveFromDriveList, TIpcArgs(aDriveNumber)));
 	}
 	
 	
 	
 /**
 * UpdateDrivePriorityL()
 *
 * @param  TDriveNumber: Drive number of the drive
 *						 to be updated.
 * @param  TUint       : Priority of the drive.
 * @return TUint       : New priority of the drive.
 *
 * Code changes for PREQ 557.
 * The function updates the priority of the drive 
 * already present in the preferred drive list. If 
 * the priority being mentioned is greater than the 
 * number of drive present in the list, the drive 
 * will be added at the end of the preferred drive list. 
 * The function will then return the new priority of 
 * the drive. If updating a priority makes a non-current 
 * drive the current drive or vice-versa, the function 
 * will implicitly perform changeDrive() and send the 
 * notification to all registered client. It will return
 * error if the said drive is not already present in the 
 * preferred drive list.
 */ 
EXPORT_C void RMsvServerSession::UpdateDrivePriorityL(TDriveNumber aDriveNumber, TUint& aPriority)
	{
	TUint newPriority = aPriority;
	TPckg<TUint> priorityPkg(newPriority);
	if(!RFs::IsValidDrive(aDriveNumber))
		{
		User::Leave(KErrArgument);
		}
	User::LeaveIfError(SendReceive(EMsvUpdateDrivePriority, TIpcArgs(aDriveNumber, aPriority, &priorityPkg)));
	aPriority = newPriority;
	}
	

#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
void RMsvServerSession::ResetRepositoryL()
	{
	User::LeaveIfError(SendReceive(EMsvResetRepository));
	}
#endif 	// #if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
#endif  // #if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)	



/**
Gets data from a server MTM that is not related to any operation being performed on
that MTM.

@param aServiceId The ID of the service from which to get the data
@param aType The type of MTM data to get.
@param aMtmData A packaged buffer to store the data

@return KErrNone if successful or one of the system wide error codes
*/
TInt RMsvServerSession::GetNonOperationMtmData(TMsvId aServiceId, const TNonOperationMtmDataType& aMtmDataType, TDes8& aMtmData)
	{
	return SendReceive(EMsvGetNonOperationMtmData, TIpcArgs(aServiceId, aMtmDataType, &aMtmData));
	}


/** 
SearchSort Request on Header and/or Body part
@param aQuery SearchSortQuery object
@param aOperationId Opertion Id
@param aMarkQuery TBool for QueryId, whether needs to mark at server side or not
@param aRequestStatus TRequestStatus status
@capability WriteUserData
*/	
void RMsvServerSession::SearchSortRequestOnHeaderBodyL(const CMsvSearchSortQuery* aQuery, TMsvOp aOperationId, TBool aMarkQuery, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	TMsvPackQuery packedQuery(iBuffer);
	TInt error = packedQuery.PackQuery(aQuery);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = packedQuery.PackQuery(aQuery);
		}
	// pass data to the server
	SendReceive(EMsvSearchSortOperation, TIpcArgs(aOperationId, iBuffer, aMarkQuery), aRequestStatus);
	}


/** 
Get results for given QueryId
@param aQueryId queryId from client
@param aOperationId Opertion Id
@param aIterator Whether results should be send in iterator
@param aRequestStatus TRequestStatus status
@capability ReadUserData
*/
void RMsvServerSession::GetResultsForQueryId(TInt& aQueryId, TMsvOp aOperationId, TInt aIterator, TRequestStatus& aRequestStatus)
	{
	SendReceive(EMsvSearchSortQueryId, TIpcArgs(aOperationId, aQueryId, aIterator), aRequestStatus);
	}


/** 
Search Sort Request on TMsvEntry
@param aQuery SearchSortQuery object
@param aOperationId Opertion Id
@param aMarkQuery TBool for QueryId, whether needs to mark at server side or not
@param aIterator Whether results should be send in iterator
@param aRequestStatus TRequestStatus status
@capability ReadUserData
*/	
void RMsvServerSession::SearchSortRequestOnEntryL(const CMsvSearchSortQuery* aQuery, TMsvOp aOperationId, TBool aMarkQuery, TInt aIterator, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	// package up the query into a query entry
	TMsvPackQuery packedQuery(iBuffer);
	TInt error = packedQuery.PackQuery(aQuery);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = packedQuery.PackQuery(aQuery);
		}
	SendReceive(EMsvSearchSortOnIndexEntry, TIpcArgs(aOperationId, iBuffer, aMarkQuery, aIterator), aRequestStatus);
	}
	
/** 
Get search sort results as TMsvId
@param aId RArray of TMsvId's sent by server
@capability ReadUserData
*/
void RMsvServerSession::GetResultAsIdL(RArray<TMsvId>& aId)
	{
	TBool resultType=ETrue; // result as TMsvId
	
	// pass the buffer to receive the data through
	TPtr8 ptr=iBuffer->Des();
	// signal the server
	TInt error = SendReceive(EMsvGetResult,TIpcArgs(&resultType, &ptr));
	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0);
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		// pass the buffer to receive the data through
		TPtr8 ptr=iBuffer->Des();
		error = SendReceive(EMsvGetResult,TIpcArgs(&resultType, &ptr));
		}
	User::LeaveIfError(error);
	
	// unpack the entries from the buffer
	TMsvPackedIdOperation packedIdOperation(iBuffer);
	packedIdOperation.UnpackL(aId);
	}


/** 
Get search sort Request QueryId
@param aQueryId get aQueryId from server
@capability ReadUserData
*/
void RMsvServerSession::GetSearchSortRequestQueryIdL(TInt& aQueryId)
	{
	TPckg<TInt> queryId(aQueryId); 
	User::LeaveIfError(SendReceive(EMsvGetQueryId,TIpcArgs(&queryId)));
	}


/** 
Unmark the QueryId it was marked at server side
@param aQueryId Unmark the aQueryId at server
@capability WriteUserData
*/
void RMsvServerSession::UnmarkSearchSortRequestQueryIdL(TInt aQueryId)
	{
	User::LeaveIfError(SendReceive(EMsvUnmarQueryId,TIpcArgs(&aQueryId)));
	}


/** 
Sending TMsvId along with Sort filed to server, so that it will update the data in search-sort cache. 
@param aOperationId Opertion Id
@param aTMsvIdWithSortField RArray of TMsvId along with Sort filed.
@param aRequestStatus TRequestStatus status
@capability WriteUserData
*/
void RMsvServerSession::SendResultantListL(TMsvOp aOperationId, RArray<TMsvIdWithSortField> aTMsvIdWithSortField, TRequestStatus& aRequestStatus)	
	{
	TestSlotAvailableL();
	
	// need to pack TMsvId and TMessagePart (i.e TMsvId and Sort field)
	TMsvPackedIdAndMessagePart packIdAndMessagePart;
	
	// calculate size of the TTMsvIdWithMessagePart
	TInt bufferSize = packIdAndMessagePart.Size(aTMsvIdWithSortField);
	
	// if size lessthan KMsvSessionBufferLength, allocate KMsvSessionBufferLength to sync with iBuffer initial allocation.
	if(bufferSize < KMsvSessionBufferLength)
		{
		bufferSize = KMsvSessionBufferLength;
		}
		
	// allocate the buffer
	CBufFlat* flat = CBufFlat::NewL(bufferSize);
	CleanupStack::PushL(flat);
	flat->ExpandL(0, bufferSize);
	
	RBufWriteStream writeStream(*flat);
	CleanupClosePushL (writeStream);
	
	// Externalize the data availbe in TTMsvIdWithMessagePart
	packIdAndMessagePart.ExternalizeL(writeStream, aTMsvIdWithSortField);
	TPtr8 ptr8 = flat->Ptr(0);
	
	// delete memory allocated for iBuffer
	if(iBuffer != NULL)
		{
		delete iBuffer;
		iBuffer = NULL;
		}

	//Convert CBufFlat into a HBufC8
	iBuffer = ptr8.AllocL();
	
	// send to server
	SendReceive(EMsvIdWithSortFiled, TIpcArgs(aOperationId, iBuffer), aRequestStatus);
	CleanupStack::PopAndDestroy(2, flat);
	}
	
/** 
Sending result to server, this needs be sorted along with existing partial query.
@param aOperationId Opertion Id
@param aIdArray RArray of TMsvId 
@param aRequestStatus TRequestStatus status
@capability WriteUserData
*/
void RMsvServerSession::SendNewResultsToServerForSortL(TMsvOp aOperationId, RArray<TMsvId>& aIdArray, TRequestStatus& aRequestStatus)
	{
	TestSlotAvailableL();
	TMsvPackedIdOperation op(iBuffer);
	TInt error = op.Pack(aIdArray);
	while (error != KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		error = op.Pack(aIdArray);
		}
	SendReceive(EMsvUpdateAndSort, TIpcArgs(aOperationId, iBuffer), aRequestStatus);
	}

/** 
Get list TMsvId from the server.
The list of TMsvId's may be 
    case i)   Final Result
    case ii)  Partial Result
    case iii) New Query, needs search on Header and/or Body
@param aId RArray of TMsvId's (for case i & ii)
@param aResultStatus Result status, it will tell what kind of result it is..
@capability ReadUserData
*/	
void RMsvServerSession::GetResultAsIdL(RArray<TMsvId>& aId, TInt &aResultStatus)
	{
	// pass the buffer to receive the data through

	TPtr8 ptr=iBuffer->Des();
	TPckg<TInt> resultStatus(aResultStatus);
		
	// signal the server
	TInt error = SendReceive(EMsvGetIdsOrResult,TIpcArgs(&ptr, &resultStatus));
	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0);
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		
		// pass the buffer to receive the data through
		TPtr8 ptr=iBuffer->Des();
		error = SendReceive(EMsvGetIdsOrResult,TIpcArgs(&ptr, &resultStatus));
		}
	User::LeaveIfError(error);
	
	// unpack aId from the buffer
	TMsvPackedIdOperation packedIdOperation(iBuffer);
	packedIdOperation.UnpackL(aId);
	}
	
/** 
Get next TMsvEntry from server
@param aEntry next result as TMsvEntry
@capability ReadUserData
*/
void RMsvServerSession::GetNextEntryL(TMsvEntry& aEntry, TInt& aCount)
	{
	TPckg<TInt> count(aCount);
	// pass the buffer to receive the data through
	TPtr8 ptr=iBuffer->Des();
	// signal the server
	TInt error = SendReceive(EMsvGetNextEntry,TIpcArgs(&ptr, &count));
	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0);
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		// pass the buffer to receive the data through
		TPtr8 ptr=iBuffer->Des();
		error = SendReceive(EMsvGetNextEntry,TIpcArgs(&ptr, &count));
		}
	User::LeaveIfError(error);
	TMsvPackedEntry packedEntry(iBuffer);
	packedEntry.UnpackEntry(aEntry);
	}
	

/** 
Get next TMsvId from server
@param aId next result as TMsvId
@capability ReadUserData
*/
void RMsvServerSession::GetNextIdL(TMsvId& aId, TInt& aCount)
	{
	TPckg<TMsvId> tmsvId(aId);
	TPckg<TInt> count(aCount);
	
	TInt error = SendReceive(EMsvGetNextId,TIpcArgs(&tmsvId, &count));
	User::LeaveIfError(error);
	}

/** 
Get Result count from server
@param aCount Result count
@capability ReadUserData
*/
void RMsvServerSession::GetResultCountL(TInt& aCount)
	{
	TPckg<TInt> count(aCount);
	// get result count from server
	User::LeaveIfError(SendReceive(EMsvGetResultCount,TIpcArgs(&count)));
	}

/** 
Get Search sort request from server
This Query is requied in case partial result for the QueryId
@param aQuery Search Sort Query object
@param aOpId Opertion Id
@capability ReadUserData
*/
void RMsvServerSession::GetQueryFromServerL(TMsvOp aOperationId, TInt aQueryId, CMsvSearchSortQuery* aQuery)
	{
	// pass the buffer to receive the query data
	TPtr8 ptr=iBuffer->Des();
	// signal the server
	TInt error = SendReceive(EMsvQueryData,TIpcArgs(aOperationId, aQueryId, &ptr));
	while(error == KErrOverflow)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0);
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength);
		// pass the buffer to receive the data through
		TPtr8 ptr=iBuffer->Des();
		error = SendReceive(EMsvQueryData,TIpcArgs(aOperationId, aQueryId, &ptr));
		}
	User::LeaveIfError(error);
	
	TMsvPackQuery packedQuery(iBuffer);
	packedQuery.UnpackQuery(aQuery);	
	}

/** 
Get Progress information from the server for a given Request.
@param aOperationProgress Operation progress
@param aOpId Opertion Id
@capability ReadUserData
*/	
void RMsvServerSession::SearchSortOperationProgressL(TMsvOp aOperationId, TInt aOperationProgress)
	{
	TPckg<TInt> progressInfo(aOperationProgress);
	User::LeaveIfError(SendReceive(EMsvGetSearchSortProgress,TIpcArgs(aOperationId, &progressInfo)));
	}

/** 
Cancel current saerch sort Request.
@param aOpId Opertion Id
@capability ReadUserData
*/	
int RMsvServerSession::CancelSearchSortOperation(TMsvOp aOperationId)
	{
	TInt error = SendReceive(EMsvCancelSearchSortOp,TIpcArgs(aOperationId));
	return error;
	}


#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
#if (defined SYMBIAN_MESSAGESTORE_UNIT_TESTCODE)
/**
Print cache structure in a log file.
*/
EXPORT_C void RMsvServerSession::PrintCache()
	{
	SendReceive(EMsvPrintCache);
	}
#endif
#endif




#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)

/**
 * CreateStoreL()
 * 
 * A headerStore is associated with a mtm type and stores message header
 * data. Message server creates a separate store for each MTM type. A 
 * header-entry in the headerStore is essentially a list of UID-Data entries.
 * For example an email header-entry can have CImHeader, MIME header and 
 * encoded header stored as a separate UID-Data entry in the header store.
 * By default a UID-Data entry will have a "Default" field which store the
 * content of 'Data'. Moreover one such UID-Data pair can also specify mutiple
 * fields such that it can store individual portion of data in such fields 
 * separately. 
 * 
 * @param TUid: MtmId of the header store.
 * @param RPointerArray<aFieldDetails>: Structure of the Data fields in a UID-Data pair.
 * The store by default add a 'Details' field to store the content of data.
 * @return None.
 *
 * @leave: All System wide error code.
 *
 * @capability WriteDeviceData
 */
void RMsvServerSession::CreateStoreL(const TUid& aMtmId, const RPointerArray<CFieldPair>& aFieldDetails)
	{
	TMsvPackedHeaderStructure packedHeaderStruct(iBuffer);
	TInt error = packedHeaderStruct.Pack(aFieldDetails);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		error = packedHeaderStruct.Pack(aFieldDetails);
		}

	// package up the entry into a packed entry
	User::LeaveIfError(SendReceive(EMsvCreateStore, TIpcArgs(aMtmId.iUid, iBuffer)));
	}





/**
 * DoesStoreExistsL()
 * 
 * The function checks the existence of a header store
 * for a given MTM Id. The function can be called to 
 * check if the header store already exist before creating
 * the store.
 *
 * @param TUid: Mtm UID of the store.
 * @return TBool: ETrue, if the store exists, else EFalse.
 *
 * @capability ReadDeviceData 
 */
TBool RMsvServerSession::DoesStoreExistsL(const TUid& aMtmId)
	{
	TPckg<TBool> isStoreExists = EFalse;
	User::LeaveIfError(SendReceive(EMsvCheckStoreExists, TIpcArgs(aMtmId.iUid, &isStoreExists)));
	return isStoreExists();
	}






/**
 * LastErrorMessageL()
 * 
 * The function returns the detail error message 
 * received while creating the header store.
 *
 * @param HBufC*: Error message text.
 * @return None.
 * @leave: All system wide error code.
 */
void RMsvServerSession::LastErrorMessageL(HBufC*& aErrorMsg)
	{
	TPtr8 ptr8 = iBuffer->Des();
	User::LeaveIfError(SendReceive(EMsvLastErrorMessage, TIpcArgs(&ptr8)));

	TInt* ptr = (TInt*) CONST_CAST(TUint8*, iBuffer->Ptr());
	TInt textSize = *ptr++;
	
	TPtrC16 ptrBuf;
	const TText* textPtr = (TText*)ptr;
	ptrBuf.Set(textPtr, (textSize/2));	
	
	aErrorMsg = ptrBuf.AllocL();
	}







/**
 * CreateHeaderEntryL()
 * 
 * This function creates a new message header in the header-store. A message header is
 * essentially a list of UID-Data entry. At most one entry in the list must have 
 * values for all the fields created in the header store. Rest all entries should have
 * values for only default field 'Details'.
 *
 * @param TUid: MtmId to identify the header table.
 * @param TMsvId: Metadata Entry Id.
 * @param RPointerArray<CHeaderFields>: Header details.
 * @return None.
 * @leave KErrAlreadyExists, if entry already exists.
 * @leave KErrArgument, if incorrect arguments and system wide leave code.
 *
 * @capability WriteUserData
 */	
EXPORT_C void RMsvServerSession::CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TMsvPackedHeaderData packedHeaderData(iBuffer);
	TInt error = packedHeaderData.Pack(aFieldPairList);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		error = packedHeaderData.Pack(aFieldPairList);
		}

	User::LeaveIfError(SendReceive(EMsvCreateHeaderEntry, TIpcArgs(aMtmId.iUid, aEntryId, iBuffer)));
	}





/**
 * LoadHeaderEntryL()
 *
 * This function loads the header entry from the header-store
 * and returns to the client.
 *
 * @param TUid: MtmId to identify the header table.
 * @param TMsvId: Metadata Entry Id.
 * @param RPointerArray<CHeaderFields>: Header entry details.
 * @return None.
 * @leave KErrNotFound: If entry not found. 
 *
 * @capability ReadUserData
 */	
 EXPORT_C void RMsvServerSession::LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TPtr8 ptr = iBuffer->Des();	
	User::LeaveIfError(SendReceive(EMsvLoadHeaderEntry, TIpcArgs(aMtmId.iUid, aEntryId, &ptr)));
	
	TMsvPackedHeaderData packedHeaderData(iBuffer);
	TRAPD(error, packedHeaderData.UnpackL(aFieldPairList));	
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		TRAP(error, packedHeaderData.UnpackL(aFieldPairList));
		}
	}
	
	
	
	
	

/**
 * DeleteHeaderEntryL()
 *
 * This function deletes the header entry from the header-store.
 *
 * @param aMtmId: MtmId to identify the header table.
 * @param aEntryId: Entry Id.
 * @return None.
 * @leave KErrNotFound: If entry not found. 
 *
 * @capability WriteUserData
 */		
EXPORT_C void RMsvServerSession::DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId)
	{
	User::LeaveIfError(SendReceive(EMsvDeleteHeaderEntry, TIpcArgs(aMtmId.iUid, aEntryId)));
	}





/**
 * UpdateHeaderEntryL()
 *
 * This function udpates the header entry in the header-store. A header entry is 
 * essentially a list of UID-String combination which is stored as a separate 
 * element in the passed header structure. 
 *
 * If the number of UID-String element in the passed header structure is more
 * than what is present in the DB, the function inserts the new UID-String entry
 * in the header store. And if the few entries are missing from the passed header 
 * structure the function deletes the extra entry from the header store.
 *
 * @param TUid: MtmId to identify the header table.
 * @param TMsvId: Entry Id.
 * @param RPointerArray<CHeaderFields>: Header entry details.
 * @return None.
 * @leave KErrNotFound, if entry to be updated not found and all system wide leave code.
 *
 * @capability WriteUserData
 */	
EXPORT_C void RMsvServerSession::UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	TMsvPackedHeaderData packedHeaderData(iBuffer);
	TInt error = packedHeaderData.Pack(aFieldPairList);
	while(error!=KErrNone)
		{
		// increase the size of the buffer and try again
		iBuffer->Des().SetLength(0); // to avoid copying contents
		iBuffer = iBuffer->ReAllocL(iBuffer->Des().MaxSize() + KMsvSessionBufferLength); 
		error = packedHeaderData.Pack(aFieldPairList);
		}

	User::LeaveIfError(SendReceive(EMsvUpdateHeaderEntry, TIpcArgs(aMtmId.iUid, aEntryId, iBuffer)));
	}
	





/**
 * DoesAnyStoreExists()
 *
 * This function checks if the store (header/body) exists 
 * for a given metadata entry.
 */	
EXPORT_C TBool RMsvServerSession::DoesAnyStoreExists(TMsvId aId, TUid aMtmId)
	{
	return SendReceive(EMsvCheckAnyStoreExists, TIpcArgs(aId, aMtmId.iUid));
	}




/**
 * DoesHeaderTableExist()
 *
 * This function checks if the header table exists
 * for a given MTM type.
 */	
EXPORT_C TBool RMsvServerSession::DoesHeaderTableExist(const TUid& aMtmId)
	{
	return SendReceive(EMsvCheckHeaderTableExist, TIpcArgs(aMtmId.iUid));
	}
	

/********************************Converter API's***************************/
/*
 GetConvertibleDriveListL()
 Fetches a list of unsupported drives. Any drive is rendered unsupported
 if the message store version is less than current message store version.
 
 @param aDriveList: RArray containing unsupported drive numbers.
 @return None 
 */
void RMsvServerSession::GetConvertibleDriveListL(RArray<TDriveNumber>& aDriveList)
	{
	TPtr8 ptr = iBuffer->Des();	
	User::LeaveIfError(SendReceive(EMsvGetConvertibleDriveList, TIpcArgs(&ptr)));	
	// Unpack the entries from the buffer.
	TMsvPackedDriveIdOperation packedDriveIdOperation(iBuffer);
	packedDriveIdOperation.UnpackL(aDriveList);	
	}

/*
 ConvertMessageStore()
 Starts message store conversion on a drive asynchronously.
 The message store to be converted is identified by the drive number
 Message store conversion is an asynchronous operation and the client gets
 notified by TRequestStatus variable.Message store conversion starts only when
 the drive meets different validation criteria.
   
 
 @param aDrive: Drive number.
 @param aRequestStatus: Indicates the completion status of a request made to a service provider..
 @return None 
 */
void RMsvServerSession::ConvertMessageStore(TDriveNumber aDrive,TRequestStatus& aRequestStatus)
	{
	aRequestStatus=KRequestPending;
	SendReceive(EMsvConvertMessageStore, TIpcArgs(aDrive), aRequestStatus);
	}

/*
 GetConversionStatusL()
 Gets the conversion status for the specified drive
 
 @param aDrive: Drive number.
 @return None 
 */
void RMsvServerSession::GetConversionStatusL(TDriveNumber aDrive)
	{
	User::LeaveIfError(SendReceive(EMsvGetConversionStatus, TIpcArgs(aDrive)));
	}

/*
 CancelConversion()
 Cancels conversion for the specified drive.
 
 @param aDrive: Drive number.
 @return TInt: System wide error codes
 */	
TInt RMsvServerSession::CancelConversion(TDriveNumber aDrive)
	{
	return SendReceive(EMsvCancelConversionRequest, TIpcArgs(aDrive));
	}
#endif			// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
