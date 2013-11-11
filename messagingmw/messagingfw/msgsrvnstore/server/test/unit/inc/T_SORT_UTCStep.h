// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// t_sort_utcstep.cpp
// 
//

#ifndef __T_SORT_UTCSTEP_H__
#define __T_SORT_UTCSTEP_H__
 
#include <test/testexecutestepbase.h>
#include "emailtestutils.h"
#include "smstestutils.h"
#include <stdlib.h>
#include <msvapi.h>
#include <smuthdr.h> 

#include "T_SORT_UTCServer.h"

class DummyObserver : public MMsvSessionObserver
	{
	public:
		virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	};


class CTestUTCSort : public CTestStep
	{
public:
	CTestUTCSort();
	~CTestUTCSort();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void CreateMessagesL();
	void WriteStoreL();
	TVerdict TestSortingL();
	void SetLocale(TInt aCount);
	void RemoveDefaultEntryL();
	void CreateServiceAndAccountL();
private:
	CMsvSession* iSession;
	DummyObserver* iObserver;
	CEmailTestUtils* iTestUtils;
	CActiveScheduler* iScheduler;
	CTestActive* iTestActive;
	CMsvEntryArray* iEntryArray; 
	CArrayFixFlat<TMsvId>* iTMsvIdList;
	
	CRichText*                  iRichText;
    const CParaFormatLayer*     iParaFormat;
    const CCharFormatLayer*     iCharFormat;

	};

 		
_LIT(KTestUTCSort,"TestUTCSort");
	
#endif  //__T_SORT_UTCSTEP_H__