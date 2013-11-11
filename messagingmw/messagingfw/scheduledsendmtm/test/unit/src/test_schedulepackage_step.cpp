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

#include "test_schedulepackage_step.h"

CMsvSchedulePackageStep::CMsvSchedulePackageStep()
	{
	SetTestStepName(KMsvSchedulePackageStep);
	}	
	
TVerdict CMsvSchedulePackageStep::doTestStepL()	
	{
	TestConstructor();
	TestPackUnpackL();
	return TestStepResult();
	}

void CMsvSchedulePackageStep::TestConstructor()
	{
	TMsvSchedulePackage schedulePackage;
	if(schedulePackage.iId != 0 ||
		schedulePackage.iCommandId != 0 ||
		schedulePackage.iPollProgress.Int() != 0)
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("TMsvSchedulePackage: constructor wrong!"));
		}
		
	}	
	
void CMsvSchedulePackageStep::TestPackUnpackL()
	{
	TMsvSchedulePackage package1, package2;
	package1.iCommandId = 1000;
	package1.iId = 123456;
	package1.iPollProgress = 1000000;
	
	TTaskInfo task;
	HBufC* des;
	package1.PackLC(task, des);
	
	package2.UnpackL(task, *des);
	
	if(!TestPackages(package1, package2))
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("TMsvSchedulePackage: pack/unpack wrong!"));		
		}
	
	CleanupStack::PopAndDestroy(des);
		
	package1.iCommandId = 20000;
	package1.iId = 1000;
	package1.iParameter.Append(_L("Hello"));
	
	package1.PackLC(task, des);
	package2.UnpackL(task,*des);
	if(!TestPackages(package1, package2))
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("TMsvSchedulePackage: pack/unpack wrong!"));		
		}
	CleanupStack::PopAndDestroy(des);

	package1.iParameter.Fill('a');
	package1.PackLC(task, des);
	package2.UnpackL(task, *des);
	if(!TestPackages(package1, package2))
		{
		SetTestStepResult(EFail);
		INFO_PRINTF1(_L("TMsvSchedulePackage: pack/unpack wrong!"));		
		}
	CleanupStack::PopAndDestroy(des);	
	}	

TBool CMsvSchedulePackageStep::TestPackages(const TMsvSchedulePackage& aSource, const TMsvSchedulePackage& aDest)	
	{
	if(aSource.iCommandId == aDest.iCommandId &&
		aSource.iId == aDest.iId &&
		aSource.iPollProgress == aDest.iPollProgress &&
		aSource.iParameter == aDest.iParameter)
		{
		return ETrue;
		}
	return EFalse;	
	}

