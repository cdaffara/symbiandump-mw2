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

#ifndef __T_PERFORMANCE428STEP_H__
#define __T_PERFORMANCE428STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance428Server.h" 
#include "T_Performance428Client.h"
#include "T_PerformanceInitialisationClient.h"

class CPerformance428Step : public CPerformanceStepBase
	{
public:
	CPerformance428Step();
	~CPerformance428Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	CPerformance428Client* iImapClient;
	};

 		
_LIT(KPerformance428,"Performance428");
	
#endif  //__T_PERFORMANCE428STEP_H__