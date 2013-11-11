/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgUtilsDeleteAllChildren
* 
*
*/



/**
 @file
*/


#ifndef __T_UTILS_DELETE_ALL_CHILDREN_H__
#define __T_UTILS_DELETE_ALL_CHILDREN_H__


// epoc includes
#include <msvids.h>

// Forward Declaration
class CMsvSession;
class CMsvOperation;


/**
Implements a utilty that performs deletion of all the children entries
*/
class CT_MsgUtilsDeleteAllChildren : public CActive
	{
public:
	IMPORT_C CT_MsgUtilsDeleteAllChildren(CMsvSession* aSession,TMsvId aParentId = KMsvRootIndexEntryId);
   ~CT_MsgUtilsDeleteAllChildren();

	IMPORT_C void StartL(TRequestStatus& aStatus);
	IMPORT_C void StartL(TBool aBlockDelete, TRequestStatus& aStatus);
	
	// Returns the number of the children entries that are not deleted
	TInt Undeleted() {return (iUndeletedChildren);}
	
	// Gets information about a completed operation.
	IMPORT_C TInt FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus);

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
		EBlockDelete
		};
	TInt iState;

private:
	CT_MsgUtilsDeleteAllChildren* iNextLevel;
	};


#endif //__T_UTILS_DELETE_ALL_CHILDREN_H__
