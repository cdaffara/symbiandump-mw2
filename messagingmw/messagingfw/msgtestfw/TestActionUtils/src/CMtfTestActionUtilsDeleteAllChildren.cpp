// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/


#include "MtfTestActionUtilsUser.h"
#include "CMtfTestActionUtilsDeleteAllChildren.h"


CMtfTestActionUtilsDeleteAllChildren::CMtfTestActionUtilsDeleteAllChildren(CMsvSession* aSession,TMsvId aParentId) 
	: CActive(EPriorityNormal), iSession(aSession), iParentId(aParentId) 
	{
	CActiveScheduler::Add(this);
	}


CMtfTestActionUtilsDeleteAllChildren::~CMtfTestActionUtilsDeleteAllChildren()
	{
	delete iEntry;
	delete iSelection;
	}


void CMtfTestActionUtilsDeleteAllChildren::StartL(TRequestStatus& aStatus)
	{
	iRequestStatus = &aStatus;
	aStatus = KRequestPending;

	iEntry = CMsvEntry::NewL(*iSession,iParentId,TMsvSelectionOrdering(KMsvNoGrouping,EMsvSortByNone,ETrue));
	iEntry->SetEntryL(iParentId);
	iSelection = iEntry->ChildrenL();
	iTotalChildren = iSelection->Count();
	if (iTotalChildren == 0)
		User::RequestComplete(iRequestStatus,KErrNone);
	else
		{
		iOperation = iEntry->DeleteL((*iSelection)[iCurrentChild],iStatus);
		SetActive();
		iState = EStateThisLevel;
		}
	}


void CMtfTestActionUtilsDeleteAllChildren::DoCancel()
	{
	switch (iState)
		{
		case EStateThisLevel:
			iOperation->Cancel();
			break;

		case EStateNextLevel:
			iNextLevel->Cancel();
			break;
		}
	}


void CMtfTestActionUtilsDeleteAllChildren::RunL()
	{
	TInt err;

	switch (iState)
		{
		case EStateThisLevel:
			err = MtfTestActionUtilsUser::FinalProgressStatus(*iOperation,iStatus);

			delete iOperation;
			iOperation = NULL;

			switch (err)
				{
				case KErrAccessDenied:
					iUndeletedChildren++;
					iNextLevel = new (ELeave) CMtfTestActionUtilsDeleteAllChildren(iSession,(*iSelection)[iCurrentChild]);
					iNextLevel->StartL(iStatus);
					SetActive();
					iState = EStateNextLevel;
					break;

				case KErrNone:
					if (++iCurrentChild == iTotalChildren)
						User::RequestComplete(iRequestStatus,KErrNone);
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
						User::RequestComplete(iRequestStatus,KErrNone);
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
		}
	}

