// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// This contains CT_ImapTestWrapper
//
//


#if (!defined __IMAP_TEST_WRAPPER_H__)
#define __IMAP_TEST_WRAPPER_H__


#include <test/datawrapper.h>

#include <msvapi.h>


class CT_ImapSessionObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny*, TAny*, TAny*);
public:
	CMsvSession* iSession;
	TMsvSessionEvent iType;
	TInt iArg1;
	TInt iArg2;
	};

class CT_ImapTestWrapper : public CDataWrapper
	{
public:
	CT_ImapTestWrapper();
	~CT_ImapTestWrapper();

	static	CT_ImapTestWrapper*	NewL();

	virtual TBool	DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);
	virtual TAny*	GetObject() { return iObject; }
	inline virtual void	SetObjectL(TAny* aObject)
		{
		DestroyData();
		iObject	= static_cast<TInt*> (aObject);
		}

	inline virtual void	DisownObjectL()
		{
		iObject = NULL;
		}

	void DestroyData()
		{
		delete iObject;
		iObject=NULL;
		}

	void RunL(CActive* aActive, TInt aIndex);

	inline virtual TCleanupOperation CleanupOperation()
		{
		return CleanupOperation;
		}

protected:
	void ConstructL();
private:
	static void CleanupOperation(TAny* aAny)
		{
		TInt* number = static_cast<TInt*>(aAny);
		delete number;
		}


	inline void DoCmdNewL(const TDesC& aEntry);
	// Message commands
	inline void DoCmdCopyMessageL(const TDesC& aSection);
	inline void DoCmdMoveMessage(const TDesC& aSection);
	inline void DoCmdDeleteMessage(const TDesC& aSection);

	TBool ReadIniCopyMessage(const TDesC& aSection);
	TBool ReadIniDeleteMessage(const TDesC& aSection);

protected:
	TInt*						iObject;
	CMsvSession* 				iSession;
	CT_ImapSessionObserver*	iObserver;

	TPtrC iSubject;
	TPtrC iTrgtLookupIn;
	TPtrC iSrcLookupIn;
	TPtrC iSrcfolderName;
	TPtrC iSrcParentFolderName;
	TPtrC iImapAccountName;
	TPtrC iTrgtfolderName;
	TPtrC iTrgtParentFolderName;
	TInt iExpectedErr;
	
	TPtrC iFolderName;
	TPtrC iParentFolderName;
	TPtrC iLookUpIn;

	CMsvOperation*	iOperation;
	CActiveCallback* iActiveCallback;
	};

#endif /* __IMAP_TEST_WRAPPER_H__ */

