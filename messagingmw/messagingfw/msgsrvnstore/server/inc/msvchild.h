// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined (__MENTACT_H__)
#include "MENTACT.H"
#endif

#if !defined(__MSVCHILD_H__)
#define __MSVCHILD_H__

//**********************************
// CMsvChildMessages
//**********************************

class CMsvChildMessages : public CMsgActive
/**
@internalComponent
@released
*/
	{
public:
	static CMsvChildMessages* NewL(CMsvSession& aSession, TInt aPriority);
	~CMsvChildMessages();
	void StartL(TMsvId aParentId, CMsvEntrySelection& aChildren, TRequestStatus& aStatus);
	//
private:
	CMsvChildMessages(CMsvSession& aSession, TInt aPriority);
	void ConstructL();
	void DoRunL();
	//
private:
	CMsvSession& iSession;
	CMsvEntrySelection* iFolders;
	CMsvEntrySelection* iSelection;
	CMsvEntrySelection* iChildren;
	CMsvEntryFilter* iFilter;
	};

#endif
