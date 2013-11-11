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


#ifndef __CMTF_TEST_ACTION_RECEIVE_SMS_MESSAGES2_H__
#define __CMTF_TEST_ACTION_RECEIVE_SMS_MESSAGES2_H__

#include "CMtfTestAction.h"

class CMsvEntry;
class MMsvEntryObserver;

_LIT(KTestActionReceiveSmsMessages2,"ReceiveSmsMessages2");

class CMtfTestActionReceiveSmsMessages2 : public CMtfTestAction,public MMsvEntryObserver
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionReceiveSmsMessages2(); 

public:	
	virtual void ExecuteActionL();
	void StopReceiveSmsMessages2();

protected:
	void DoCancel();
	void RunL();
	
private:
	void HandleEntryEventL(TMsvEntryEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	CMtfTestActionReceiveSmsMessages2(CMtfTestCase& aTestCase);

private:
	TInt iNumberOfMessages;
	CMsvEntry* iMsventry;
	};

#endif
