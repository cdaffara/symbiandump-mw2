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
* This is the header file for CT_StopRamUsageTimer
* 
*
*/





#ifndef _T_STOP_RAM_USAGE_TIMER_
#define _T_STOP_RAM_USAGE_TIMER_


// User includes 
#include "T_MsgAsyncStepSMTP.h"

// Literals used 
_LIT(KStopRamUsageTimer,"StopRamUsageTimer");

// Implement a test step to stop the periodic Timer 
class CT_StopRamUsageTimer :public CT_MsgAsyncStepSMTP
	{
public:
	CT_StopRamUsageTimer(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_StopRamUsageTimer();
private:
     void ProgressL(TBool aFinal);
     void CancelStep();
     
	// CTestStep implementation 
	virtual TVerdict	doTestStepL();	
	};


#endif // _T_STOP_RAM_USAGE_TIMER_ 
