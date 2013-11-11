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
//

#ifndef __T_PERFORMANCE427STEP_H__
#define __T_PERFORMANCE427STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance427Server.h" 
#include "T_Performance427Client.h"
#include "T_PerformanceInitialisationClient.h"

class CPerformance427Step : public CPerformanceStepBase
	{
public:
	CPerformance427Step();
	~CPerformance427Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();

private:
	CPerformance427Client* iImapClient;
	};

 		
_LIT(KPerformance427,"Performance427");
	
#endif  //__T_PERFORMANCE427STEP_H__