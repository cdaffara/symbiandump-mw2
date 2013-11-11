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

#ifndef __TEST_ERRORACTIONS_STEP_H__
#define __TEST_ERRORACTIONS_STEP_H__

#include <test/testexecutestepbase.h>
#include <msvapi.h>
#include <msvsenderroraction.h>

_LIT(KMsvSendErrorActionsStep, "MsvSendErrorActionsStep");
	
/**
Test class CMsvSendErrorActions

@internalTechnology
*/
class CMsvSendErrorActionsStep : public CTestStep, public MMsvSessionObserver
	{
public:
	CMsvSendErrorActionsStep();
	
	//methods from CTestStep
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();
	
	//methods from MMsvSessionObserver
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny*, TAny*);	
	
private:
	void SetConfigurationL();
	TBool CompareErrorActions(const CMsvSendErrorActions& aDest, TBool aCompareDefault = ETrue, TBool aCompareIndividual = EFalse);
	TBool CompareErrorAction(const TMsvSendErrorAction& aAction1, const TMsvSendErrorAction& aAction2);
	void TestManipulateErrorActionL();
	void TestDefaultErrorAction();
	void TestSetGetErrorsL();
	void TestRestoreFromResourceL();
private:
	CMsvSendErrorActions* iErrorActions;
	CActiveScheduler* 	iScheduler;
	CMsvSession* 		iSession;
	RFs					iFs;
	};

#endif
