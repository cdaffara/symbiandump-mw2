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

#ifndef __T_PERFORMANCE424STEP_H__
#define __T_PERFORMANCE424STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance424Server.h" 
#include "T_Performance424Client.h"
#include "T_PerformanceInitialisationClient.h"

class CPerformance424Step : public CPerformanceStepBase
	{
public:
	CPerformance424Step();
	~CPerformance424Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	CPerformance424Client* iImapClient;
	};

 		
_LIT(KPerformance424,"Performance424");
	
#endif  //__T_PERFORMANCE424STEP_H__