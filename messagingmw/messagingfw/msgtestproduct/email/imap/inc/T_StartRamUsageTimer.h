/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file 
* This is the header file for CT_StartRamUsageTimer
* 
*
*/





#ifndef _T_START_RAM_USAGE_TIMER_
#define _T_START_RAM_USAGE_TIMER_


// User includes 
#include "T_MsgAsyncStepIMAP.h"

// Literals used 
_LIT(KStartRamUsageTimer,"StartRamUsageTimer");

/**
Implement a wrapper class over HAL APIs and 
setting the periodic Timer to monitor the RAM usage
 */
class CT_StartRamUsageTimer : public CT_MsgAsyncStepIMAP
	{
public:
	CT_StartRamUsageTimer(CT_MsgSharedDataImap& aSharedDataIMAP);	
	~CT_StartRamUsageTimer();

	static TInt RamUsage(TAny* aObject);
	void DoRamUsage();

private:
     void ProgressL(TBool aFinal);
     void CancelStep();
     
	// CTestStep implementation 
	virtual TVerdict	doTestStepL();	
	};


#endif // _T_START_RAM_USAGE_TIMER_ 
