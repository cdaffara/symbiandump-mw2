// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// PUSHSMTM.CPP
// 
//



#include "regpsdll.h"   // Parser Registry - used to load the parser
#include <msvids.h>
#include <pushentry.h>
#include "PushServerPanic.h"
#include "pushservermtm.h"



/**
 * Factory function for the WAP Push Server MTM. Sets the initial entry for the server to work with.
 * @param CRegisteredMtmDll& aRegisteredMtmDll
 *					in: Registration data for the MTM DLL 
 * @param CMsvServerEntry* aInitialEntry
 *					in: initial context that the server should operate on.
 * @return CWapPushServerMtm* - new instance of the server MTM
 */
EXPORT_C CWapPushServerMtm* CWapPushServerMtm::NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry)
	{
	CleanupStack::PushL(aInitialEntry); // Take ownership of aInitialEntry so it's cleaned up if we leave
	CWapPushServerMtm* myServerMtm =  new(ELeave) CWapPushServerMtm(aRegisteredMtmDll,aInitialEntry);
	CleanupStack::Pop(); //safely stored aInitialEntry - we can release it now
	CleanupStack::PushL(myServerMtm);
	myServerMtm->Construct();
	CleanupStack::Pop();
	return myServerMtm;
	}

/**
 * Destructor 
 */
CWapPushServerMtm::~CWapPushServerMtm()
	{
	Cancel();
	}

/**
 * C++ constructor. Passes parameters to the base constructor, and set the initial state.
 * @param CRegisteredMtmDll& aRegisteredMtmDll
 *					in: Registration data for the MTM DLL 
 * @param CMsvServerEntry* aInitialEntry
 *					in: initial context that the server should operate on
 */
CWapPushServerMtm::CWapPushServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aInitialEntry)
:CBaseServerMtm(aRegisteredMtmDll, aInitialEntry),  iError(KErrNone),iState(EWapPushServerMtmWaiting)
	{
	__DECLARE_NAME(_S("CWAPPushServerMtm"));
	}

/**
 * Second Stage Constructor. Adds the object to the Active Scheduler
 */
void CWapPushServerMtm::Construct()
	{
	CActiveScheduler::Add(this);
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::CopyToLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::CopyFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::CopyWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::MoveToLocalL(const CMsvEntrySelection& /*aSelection*/, TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);	
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::MoveFromLocalL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * MTM API Not Supported
 */
void CWapPushServerMtm::MoveWithinServiceL(const CMsvEntrySelection& /*aSelection*/,TMsvId /*aDestination*/, TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	}

/**
 * Deletes a number of entries from the Message Server index. Also deletes child entries of the selected 
 * entries. Makes a call to the Message Server to do the actual delete. 
 * @param const CMsvEntrySelection& aSelection
 *					in: Selection of entry Ids that the caller wants to delete
 * @param TRequestStatus& aStatus
 *					in: Status object of calling object to be used to return status information 
 * @leave KErrLocked Will leave if the entry is locked
 * @leave KErrAccessDenied Will leave if the entry or a descendant was locked by another client
 * @leave KErrInUse Will leave if the store or a file associated with the entry is open
 * @leave KErrNotFound Will leave if the entry is not a child of the context 
 */ 
void CWapPushServerMtm::DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus)
	{
	__ASSERT_ALWAYS(iServerEntry!=NULL,Panic(EPushEntryNotSet));
	ResetProgress();
	iProgress.iTotalMsgs = aSelection.Count();
	iState = EWapPushServerMtmDelete;

	// set the main server entry up to the first entry
	iError=iServerEntry->SetEntry(aSelection.At(0)); //Hold the error for progress
	User::LeaveIfError(iError);

	__ASSERT_ALWAYS(iServerEntry->Entry().iMtm == KUidMtmWapPush,Panic(EPushServerWrongMtm));	

	CMsvEntrySelection* entries=aSelection.CopyLC();
	iError = iServerEntry->SetEntry(iServerEntry->Entry().Parent());
	User::LeaveIfError(iError);
	
	// Call the message server to delete the entries recursively
	iError = iServerEntry->DeleteEntries(*entries);
	User::LeaveIfError(iError);
	CleanupStack::PopAndDestroy();//entries

	iProgress.iMsgsProcessed = aSelection.Count();

	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, KErrNone);
	iProgress.iMsgId = aSelection.At(0); //hold the first id for the progress
	}

/**
 * Creates a new entry with this MTM group. Makes a call to the server to do the actual 
 * create.  Holds on to the new Id so it can returned in the Progress information.
 * @param const TMsvEntry& aSelection
 *					in: entry that the user wants to create
 * @param TRequestStatus& aStatus
 *					in: Status object of calling object to be used to return status information 
 */ 
void CWapPushServerMtm::CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus)
	{
	ResetProgress();
	iProgress.iTotalMsgs = 1;
	iError=iServerEntry->SetEntry(aNewEntry.Parent());
	User::LeaveIfError(iError);
	
	__ASSERT_ALWAYS(aNewEntry.iMtm == KUidMtmWapPush,Panic(EPushServerWrongMtm));	

	iState = EWapPushServerMtmCreate;
	iError = iServerEntry->CreateEntry(aNewEntry);
	User::LeaveIfError(iError);
	
	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, KErrNone);  // No errors so far
	iProgress.iMsgsProcessed = 1;	
	iProgress.iMsgId = aNewEntry.Id();  // hold the id for the progress info
	}


/**
 * Changes the specified entry to the new values. Must be of the Push Message MTM type.
 * Makes a call to the server to do the actual change.  Holds on to the Id so it 
 * can returned in the Progress information.
 * @param const TMsvEntry& aEntry
 *					in: entry with the new values 
 * @param TRequestStatus& aStatus
 *					in: Status object of calling object to be used to return status information 
 */ 

void CWapPushServerMtm::ChangeL(TMsvEntry aEntry, TRequestStatus& aStatus)
	{
	ResetProgress();
	iProgress.iTotalMsgs = 1;
	iState = EWapPushServerMtmChange;

	iError =iServerEntry->SetEntry(aEntry.Id()); //Hold the error so that the progress can return it
	User::LeaveIfError(iError);
	__ASSERT_ALWAYS(iServerEntry->Entry().iMtm == KUidMtmWapPush,Panic(EPushServerWrongMtm));	
	
	iError = iServerEntry->ChangeEntry(aEntry);
	User::LeaveIfError(iError);

	TRequestStatus* status=&aStatus;
	User::RequestComplete(status, KErrNone);

	//Update progress
	iProgress.iMsgsProcessed=1;	
	iProgress.iMsgId = aEntry.Id(); //hold the id for the progress
	}

/**
 * Function called when server specific commands are required. Case statement is used to call the  
 * appropriate function.  Currently there are no commands which are not defined in the base class.
 * If any specific commands are to be added for this MTM then the call should be made via this function.
 * @param CMsvEntrySelection& aSelection
 *						in: group of message entry Ids to be operated on.
 * @param TInt aCommand
 *						in: parameter specifying the MTM specific command to be executed 

 * @param const TDesC8& aParameter
 *						in: Command-specific parameters
 * @param TRequestStatus& aStatus
 *						in : reference to use to return asynchronous completion status information
 * @leave KErrNotSupported - currently don't support any MTM specific commands.
 *
 */
void CWapPushServerMtm::StartCommandL(CMsvEntrySelection& /*aSelection*/, TInt aCommand, const TDesC8& /*aParameter*/, TRequestStatus& /*aStatus*/)
	{
	// Add any Mtm specific requests to this section. 
	switch (aCommand)
		{
		case 0: //Keeps compiler happy
		default:
			User::Leave(KErrNotSupported);  // Currently don't support any MTM specific commands
			break;
		}
	*iReportStatus=KRequestPending;
	User::RequestComplete(iReportStatus, KErrNone);

	}


/**
 * Indicates whether any more commands are expected. This is to prevent the server unloading
 * the MTM if further commands are expected. Currently always return EFalse, so the MTM can be unloaded.
 * @return TBool - always EFalse
 */
TBool CWapPushServerMtm::CommandExpected()
	{
	return EFalse; // Currently ALWAYS EFalse  can be deleted by Msg Server
	}

/**
 * Returns a Package Buffer containing a Progress Object.  Provides the caller on the current operation
 * total number of entries, number processed, Error, and the current entry. 
 * @return const TDesC8& - package buffer containing the Progress object
 */
const TDesC8& CWapPushServerMtm::Progress()
	{
	iProgress.iPushState = iState;
	iProgress.iError=iError;
	iProgressBuf = TPushMTMProgressBuf(iProgress);
	return iProgressBuf;
	}

/**
 * Cancels the current asynchronous operation. Required by the Active Object class 
 * inherited by the base class.
 */
void CWapPushServerMtm::DoCancel()
	{
	User::RequestComplete( iReportStatus, KErrCancel );
	}


/**
 * Inherited from the the base class. Called by RunL.
 */
void CWapPushServerMtm::DoRunL()
	{
	User::RequestComplete (iReportStatus, iError);
	}

/**
 * Called by the active scheduler when RunL leaves.  Tidies things up and completes 
 * the current asynchronous request with the error code passed as a parameter. 
 * @param TInt aError 
 *					in: Error code
 * @return TInt		error code required by Active Scheduler - KErrNone prevents Scheduler panicing
 */
TInt CWapPushServerMtm::RunError(TInt aError)
	{
	iError = aError;
	Cancel();
	Complete();
	return KErrNone;
	}

/**
 * Completes the current operation with the current error level.
 */
void CWapPushServerMtm::Complete()
	{
	DoComplete(iError);
	}

/**
 * Performs the completiong of the curent operation with the specified error code.
 */
void CWapPushServerMtm::DoComplete(TInt aError)
	{
	TRequestStatus* status = iReportStatus;
	User::RequestComplete(status, aError);
	}
/**
 * Resets the progress variable.
 */
void CWapPushServerMtm::ResetProgress()
	{
	iProgress.iError=KErrNone;
	iProgress.iTotalMsgs = 0;
	iProgress.iMsgsProcessed =0;
	iProgress.iMsgId=KMsvNullIndexEntryId;

	iProgressBuf.Zero();
	}




