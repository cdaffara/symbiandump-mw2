/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/


#ifndef __CMTF_TEST_ACTION_UTILS_DELETE_ALL_CHILDREN_H__
#define __CMTF_TEST_ACTION_UTILS_DELETE_ALL_CHILDREN_H__


#include <e32base.h>
#include <msvapi.h>
#include <miutset.h>


class CMtfTestActionUtilsDeleteAllChildren : public CActive
	{
public:
	CMtfTestActionUtilsDeleteAllChildren(CMsvSession* aSession,TMsvId aParentId = KMsvRootIndexEntryId);
   ~CMtfTestActionUtilsDeleteAllChildren();

	void StartL(TRequestStatus& aStatus);
	TInt Undeleted() {return (iUndeletedChildren);}

protected:
	void DoCancel();
	void RunL();

private:
	CMsvSession* iSession;
	TMsvId iParentId;
	CMsvEntry* iEntry;
	CMsvEntrySelection*	iSelection;
	TRequestStatus* iRequestStatus;
	TInt iTotalChildren;
	TInt iCurrentChild;
	TInt iUndeletedChildren;
	CMsvOperation* iOperation;

private:
	enum State
		{
		EStateThisLevel,
		EStateNextLevel,
		};
	TInt iState;

private:
	CMtfTestActionUtilsDeleteAllChildren* iNextLevel;
	};


#endif
