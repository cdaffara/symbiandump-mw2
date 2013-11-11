// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "MSVSERV.H"
#include "MSVAPI.H"
#include "CCopyFiles.h"
#include "CCopyOneFile.h"
#include "cmsvdeletestoreoperation.h"



/* Constructor  */
CMsvDeleteStoreOperation::CMsvDeleteStoreOperation(const RMessage2& aMessage, CMsvServer& aServer)
: CMsvServerOperation(aMessage, aMessage.Int0(), KUidMsvLocalServiceMtm, KMsvLocalServiceIndexEntryId, -1), iDrive(TDriveUnit (aMessage.Int1())), iServer(aServer)
	{
	CActiveScheduler::Add(this);
	}

/* Destructor */
CMsvDeleteStoreOperation::~CMsvDeleteStoreOperation()
	{
	delete iFileMan;
	Cancel();
	}
	

/* Start */	
void CMsvDeleteStoreOperation::StartL()
	{
	__ASSERT_DEBUG(!IsActive() , PanicServer(EMsvDeleteStoreReset));
	
	iProgress().iState=TMsvDeleteProgress::ENotYetStarted;
	iFileMan = CFileMan::NewL(iServer.FileSession());

	TRequestStatus *status=&iStatus;
	iStatus=KRequestPending;
	SetActive();
	User::RequestComplete(status,KErrNone);
	}



/* Function to retrieve progress of the Copy operation 
**/
const TDesC8& CMsvDeleteStoreOperation::Progress()
	{
	return iProgress;
	}


void CMsvDeleteStoreOperation::DoCancel()
	{
	iProgress().iError = KErrCancel;
	Completed(KErrNone);
	}


/* Run different operations like DeleteStore
   and completed */
void CMsvDeleteStoreOperation::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	switch(iProgress().iState)
		{
		case TMsvDeleteProgress::ENotYetStarted:
			DeleteMailStoreL();
			iProgress().iState=TMsvDeleteProgress::EDeleteStore;
			break;
		case TMsvDeleteProgress::EDeleteStore:
			Completed(KErrNone);
			iProgress().iState=TMsvDeleteProgress::ECompleted;
			break;
		default:
			__ASSERT_DEBUG(EFalse,PanicServer(EMsvDeleteStoreReset));
			Completed(KErrUnknown);
			iProgress().iState=TMsvDeleteProgress::ECompleted;
			break;
		}
	}

void CMsvDeleteStoreOperation::Completed(TInt aError)
	{	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	if(KErrNone == aError)
		{
		TRAP(aError, iServer.UpdateDriveStatusL(iDrive, EMsvMessageStoreUnavailableStatus));
		}
#endif

	iProgress().iError=aError;
	SetState(EMsvOperationCompleted);
	iMessage.Complete(KErrNone);
	}


/* Delete the Mailstore on the specified drive
**/
void CMsvDeleteStoreOperation::DeleteMailStoreL()
{

	// Cant delete from the current drive
	if(iDrive==MessageServer::CurrentDriveL(iServer.FileSession()))
	{		
		User::Leave(KErrInUse);
	}
	else
	{	
#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
		iServer.PrepareDriveForDeletionL((TDriveNumber)TInt(iDrive));
#endif
		iToDelete.Zero();
		iToDelete = iDrive.Name();

		// Delete the DB
		iToDelete.Append(KMsvDbFile);
		User::LeaveIfError(RSqlDatabase::Delete(iToDelete));
		iToDelete = iDrive.Name();
		iToDelete.Append(iServer.Context().MessageFolder());
		// Delete the 2 drive letters that come with message folder path
		iToDelete.Delete(2,2); 
		User::LeaveIfError( iFileMan->RmDir(iToDelete,iStatus));
		SetActive();	
	}
}

/* RunError  */
TInt CMsvDeleteStoreOperation::RunError(TInt aError)
	{
	Completed(aError);
	return(KErrNone);
	}
