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

#ifndef __T_PERFORMANCE426STEP_H__
#define __T_PERFORMANCE426STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance426Server.h" 
#include "T_Performance426Client.h"
#include "T_PerformanceInitialisationClient.h"

class CPerformance426Step : public CPerformanceStepBase
	{
public:
	CPerformance426Step();
	~CPerformance426Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	CPerformance426Client* iImapClient;
	};

 		
_LIT(KPerformance426,"Performance426");
	
#endif  //__T_PERFORMANCE426STEP_H__