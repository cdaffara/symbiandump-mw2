// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the Cpp file which contains the utility functions to delete all children in the
// message store
// 
//

/**
 @file
*/


// User includes
#include <t_utilsdeleteallchildren.h>
	
// Epoc includes
#include <msvapi.h>
#include <msvuids.h>

/**
CT_MsgUtilsDeleteAllChildren()
Constructor

@param aSession
An object of CMsvSession

@param aParentId
Entry ID of whose children would be deleted
*/
EXPORT_C CT_MsgUtilsDeleteAllChildren::CT_MsgUtilsDeleteAllChildren(CMsvSession* aSession,TMsvId aParentId) 
: CActive(EPriorityNormal), iSession(aSession), iParentId(aParentId) 
	{
	CActiveScheduler::Add(this);
	}


/**
~CT_MsgUtilsDeleteAllChildren()
Destructor
*/
CT_MsgUtilsDeleteAllChildren::~CT_MsgUtilsDeleteAllChildren()
	{
	delete iEntry;
	delete iSelection;
	}


/**
StartL()
Deletes the children entries of the specified parent

@param aStatus			
*/
EXPORT_C void CT_MsgUtilsDeleteAllChildren::StartL(TRequestStatus& aStatus)
	{
	StartL(EFalse, aStatus);
	}

/**
StartL()
Deletes the children entries of the specified parent

@param aBlockDelete Delete all in one go
@param aStatus			
*/
EXPORT_C void CT_MsgUtilsDeleteAllChildren::StartL(TBool aBlockDelete, TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	aStatus = KRequestPending;

	// Creates a new CMsvEntry for the specified parent entry ID nad sets the context
	// to that entry.
	if(iEntry==NULL)
	{
	iEntry = CMsvEntry::NewL(*iSession,iParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	}
	iEntry->SetEntryL(iParentId);
	
	// Gets a selection containing the IDs of children
	iSelection = iEntry->ChildrenL();
	iTotalChildren = iSelection->Count();
	
	// If the entry has no children entries, then signals the thread that the asynchronous request 
	// is complete.
	if (iTotalChildren == 0)
		{
		User::RequestComplete(iRequestStatus,KErrNone);
		}
		
	
	// If the entry has children entries then,delete child entries of the context
	// recursively through all the descendants
	else
		{
		if (aBlockDelete)
			{
			iOperation = iEntry->DeleteL(*iSelection ,iStatus);
			iState = EBlockDelete;
			}
		else
			{
			iOperation = iEntry->DeleteL((*iSelection)[iCurrentChild],iStatus);
			iState = EStateThisLevel;
			}
		
		SetActive();
		}
	}


/**
DoCancel()
Implements cancellation of an outstanding request

@see CActive::Cancel()
*/
void CT_MsgUtilsDeleteAllChildren::DoCancel()
	{
	switch (iState)
		{
		case EStateThisLevel:
		case EBlockDelete:
			iOperation->Cancel();
			break;

		case EStateNextLevel:
			iNextLevel->Cancel();
			break;
		}
	}


/**
RunL()
Handles an active object's request completion event

@see CActive::RunL()
*/
void CT_MsgUtilsDeleteAllChildren::RunL()
	{
	TInt err;

	switch (iState)
		{
		case EStateThisLevel:
			
			err = CT_MsgUtilsDeleteAllChildren::FinalProgressStatus(*iOperation,iStatus);
			delete iOperation;
			iOperation = NULL;

			switch (err)
				{
				case KErrAccessDenied:
					iUndeletedChildren++;
					if(iNextLevel==NULL)
					{
					iNextLevel = new (ELeave) CT_MsgUtilsDeleteAllChildren(iSession,(*iSelection)[iCurrentChild]);
					}
					iNextLevel->StartL(iStatus);
					SetActive();
					iState = EStateNextLevel;
					break;
				
				case KErrNone:
					if (++iCurrentChild == iTotalChildren)
						{
						User::RequestComplete(iRequestStatus,KErrNone);
						}
					else
						{
						iOperation = iEntry->DeleteL((*iSelection)[iCurrentChild],iStatus);
						SetActive();
						}
					break;
					
				default:
					User::RequestComplete(iRequestStatus,err);
					break;			
				}
				break;

		case EStateNextLevel:
			err = iStatus.Int();
			iUndeletedChildren += iNextLevel->Undeleted();
			delete iNextLevel;
			iNextLevel = NULL;

			switch (err)
				{
				case KErrNone:
					if (++iCurrentChild == iTotalChildren)
						{
						User::RequestComplete(iRequestStatus,KErrNone);
						}
					else
						{
						iOperation = iEntry->DeleteL((*iSelection)[iCurrentChild],iStatus);
						SetActive();
						iState = EStateThisLevel;
						}
					break;
				default:
					User::RequestComplete(iRequestStatus,err);
					break;
				}
			break;

		case EBlockDelete:
			{
			err = CT_MsgUtilsDeleteAllChildren::FinalProgressStatus(*iOperation,iStatus);
			delete iOperation;
			iOperation = NULL;

			User::RequestComplete(iRequestStatus,err);
			break;
			}
		}
	}

	
/**
FinalProgressStatus()
This function gets the information about a completed operation for the local entries..

@param aOperation	
Used to get progress information about the operation

@param aStatus	
@return
The error code
*/
EXPORT_C TInt CT_MsgUtilsDeleteAllChildren::FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus)
	{
	TInt err = KErrNone;

	if (aOperation.Mtm() == KUidMsvLocalServiceMtm)
		{
		TPckgBuf<TMsvLocalOperationProgress> progress;
		progress.Copy(aOperation.FinalProgress());
		err = progress().iError;
		}
	else
		{
		if (iState == EBlockDelete)
			{
			TRAP_IGNORE(err = McliUtils::GetProgressErrorL(aOperation));
			}
		else
			{
			err = aStatus.Int();
			}
		}
	return (err);
	}

