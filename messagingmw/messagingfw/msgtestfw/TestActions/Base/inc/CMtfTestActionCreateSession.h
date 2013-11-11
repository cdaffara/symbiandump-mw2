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


#ifndef __CMTF_TEST_ACTION_CREATE_SESSION_H__
#define __CMTF_TEST_ACTION_CREATE_SESSION_H__


#include "CMtfSynchronousTestAction.h"

#include <msvapi.h>


_LIT(KTestActionCreateSession,"CreateSession");


class CSessionObserver : public MMsvSessionObserver, public CBase
	{
public: 
	void HandleSessionEventL(TMsvSessionEvent,TAny*,TAny*,TAny*) {};
	};


class CMtfTestActionCreateSession : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCreateSession(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionCreateSession(CMtfTestCase& aTestCase);
	
private:
	CSessionObserver* iSessionObserver;
	};


#endif
