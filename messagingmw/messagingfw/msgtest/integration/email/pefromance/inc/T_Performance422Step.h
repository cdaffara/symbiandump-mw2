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

#ifndef __T_PERFORMANCE422STEP_H__
#define __T_PERFORMANCE422STEP_H__
 
#include <test/testexecutestepbase.h>
#include "T_PerformanceStepBase.h"
#include "T_Performance422Server.h" 
#include "T_Performance422Client.h"
#include "T_PerformanceInitialisationClient.h"

class CPerformance422Step : public CPerformanceStepBase
	{
public:
	CPerformance422Step();
	~CPerformance422Step();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepPostambleL();
	virtual TVerdict doTestStepL();
private:
	void StoreMessageSettingsL(CImMimeHeader* aimMimeHeader);

private:
	CPerformance422Client* iImapClient;
	};

 		
_LIT(KPerformance422,"Performance422");
	
#endif  //__T_PERFORMANCE422STEP_H__