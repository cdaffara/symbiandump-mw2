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

#ifndef __T_PERFORMANCE421STEP_H__
#define __T_PERFORMANCE421STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance421Server.h" 
#include "T_Performance421Client.h"

class CPerformance421Step : public CPerformanceStepBase
	{
public:
	CPerformance421Step();
	~CPerformance421Step();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	CPerformance421Client* iImapClient;
	TBool iProfile;
	};

 		
_LIT(KPerformance421,"Performance421");
	
#endif  //__T_PERFORMANCE421STEP_H__