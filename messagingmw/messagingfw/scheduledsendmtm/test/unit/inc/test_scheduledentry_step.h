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

#ifndef __TEST_SCHEDULEDENTRY_STEP_H__
#define __TEST_SCHEDULEDENTRY_STEP_H__

#include <test/testexecutestepbase.h>
#include <msvscheduledentry.h>
#include <msvapi.h>

/**
Dummy class inherits from CMsvScheduledEntry

@internalTechnology
*/
class CDummyMsvScheduledEntry : public CMsvScheduledEntry
	{
public:	
	static CDummyMsvScheduledEntry*	NewL(const TMsvEntry& aMsvEntry);
	
	//methods from CMsvScheduledEntry
	TBool CanSendToAnyRecipients(const CMsvSendErrorActions&, TMsvSendErrorAction&) {return ETrue;};
	TBool CanSendToAnyRecipients(const TMsvSendErrorAction& ) {return ETrue;};
	void RecipientsResetRetries() {};
	void RecipientsIncreaseRetries() {};
	void RecipientsSetFailed() {};
	TBool RecipientsAllSent() const {return ETrue;};
	void RecipientsStoreL(CMsvStore& ) const {};
	void RecipientsRestoreL(CMsvStore& ) {};
	
private:
	CDummyMsvScheduledEntry(const TMsvEntry& aMsvEntry);
	};

_LIT(KMsvScheduledEntryStep, "MsvScheduledEntryStep");

/**
Testing the class CMsvScheduledEntry

@internalTechnology
*/	
class CMsvScheduledEntryStep : public CTestStep, public MMsvSessionObserver
	{
public:
	CMsvScheduledEntryStep();
	
	//methods from CTestStep
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
	//methods from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny*, TAny*);

private:
	void InitializeEntryL();
	void TestGetValues();
	void TestSetValues();
	void TestStoreRestoreL();
	void TestEntry();
		
private:
	CMsvScheduledEntry*		iScheduledEntry;
	CActiveScheduler* 	iScheduler;
	CMsvSession* 		iSession;
	TMsvEntry			iEntry;
	};


#endif
