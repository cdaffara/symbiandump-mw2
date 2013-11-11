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

#ifndef __T_SCHSEND_UTC_FAILED_2_STEP_H__
#define __T_SCHSEND_UTC_FAILED_2_STEP_H__
 
#include <test/testexecutestepbase.h>
#include "emailtestutils.h"
#include "smstestutils.h"
#include <stdlib.h>
#include <msvapi.h>
#include <smuthdr.h> 
#include "t_schedulesend_utils.h"
#include "t_schsend_utc_server.h"
#include "t_schsend_utc_teststep_base.h"

class CFailed2Step : public CSchSendStepBase
	{
public:
	CFailed2Step();
	virtual TVerdict doTestStepL();
	};
 	
_LIT(KFailed2Step,"Failed2Step");
	
#endif  //__T_SCHSEND_UTC_FAILED_2_STEP_H__