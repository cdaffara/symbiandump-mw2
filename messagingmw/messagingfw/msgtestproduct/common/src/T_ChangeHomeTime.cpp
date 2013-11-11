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
// [TestStep Name]
// ChangeHomeTime
// [Paramaters]
// NewTime		: New time to be set as the hometime.
// NOTE** Value of this parameter is treated as a string and must of the format 
// YYYYMMDD:HHMMSS.MMMMMM where, "YYYY" represents the year, "MM" represents month,
// "DD" represents day, "HH" represents hour, "MM" represents minutes, "SS" represents seconds,
// "MMMMMM" represents microseconds.
// Modify the current home time to a specified time.
// [APIs Used]
// User::SetHomeTime
// 
//

/**
 @file  
*/

/* User Includes */
#include "t_changehometime.h"

// Literals used 
_LIT(KTime, "NewTime");
	
/**
 Function : CT_MsgChangeHomeTime
 Description : Constructor
 @param : aSharedDataCommon	    A reference to CT_MsgSharedDataCommon
 @return : N/A
 */
CT_MsgChangeHomeTime::CT_MsgChangeHomeTime(CT_MsgSharedDataCommon& aSharedDataCommon) 
: CT_MsgSyncStepCommon(aSharedDataCommon)
	{
	SetTestStepName (KChangeHomeTime);
	}


/**
 Function : doTestStepL
 Description : Modifies the current HomeTime to a specified time.
 @return : TVerdict - Test step result
 */
TVerdict CT_MsgChangeHomeTime::doTestStepL()
	{
	INFO_PRINTF1(_L("Test Step : ChangeHomeTime"));
	
	TPtrC time;
	// Read new Date and Time from the INI file
	if ( !GetStringFromConfig( ConfigSection(), KTime, time))
		{
		ERR_PRINTF1(_L("Number of Serch Criteria is not specified"));
		SetTestStepResult (EFail);
		}
	TTime		currentHomeTime;
	currentHomeTime.HomeTime();
	TDateTime	curDateTime=currentHomeTime.DateTime();
    // Print the home time before modification
    INFO_PRINTF6(
		_L("The current DateTime  %d:%d on %d/%d/%d"),
		curDateTime.Hour(), curDateTime.Minute(),
		curDateTime.Day()+1, curDateTime.Month()+1, curDateTime.Year());
	
	TTime newHomeTime(time);
	// Modify home time
	TInt error = User::SetHomeTime(newHomeTime);
	User::LeaveIfError(error);
    // Print modified home time
	TDateTime	modifiedDateTime = newHomeTime.DateTime();
    INFO_PRINTF6(
		_L("New DateTime  %d:%d on %d/%d/%d"),
		modifiedDateTime.Hour(), modifiedDateTime.Minute(),
		modifiedDateTime.Day()+1, modifiedDateTime.Month()+1, modifiedDateTime.Year());	
	return TestStepResult();	
	}
