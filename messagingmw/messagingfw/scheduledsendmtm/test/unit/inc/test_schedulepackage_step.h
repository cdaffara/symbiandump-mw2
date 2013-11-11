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

#ifndef __TEST_SCHEDULEPACKAGE_STEP_H__
#define __TEST_SCHEDULEPACKAGE_STEP_H__

#include <test/testexecutestepbase.h>
#include <msvschedulesend.h>
#include <msvschedulepackage.h>

_LIT(KMsvSchedulePackageStep, "MsvSchedulePackageStep");
	
/**
Testing the class TMsvSchedulePackage

@internalTechnology
*/	
class CMsvSchedulePackageStep : public CTestStep
	{
public:
	CMsvSchedulePackageStep();
	
	//methods from CTestStep
	virtual TVerdict doTestStepL();
private:
	void TestConstructor();	
	void TestPackUnpackL();
	TBool TestPackages(const TMsvSchedulePackage& aSource, const TMsvSchedulePackage& aDest);
	};

#endif
