/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/



/**
 @file
*/


#ifndef __CMTF_TEST_ACTION_SET_SMS_SERVICE_SETTINGS_H__
#define __CMTF_TEST_ACTION_SET_SMS_SERVICE_SETTINGS_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionSetSmsServiceSettings,"SetSmsServiceSettings");

class CMtfTestActionSetSmsServiceSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSetSmsServiceSettings(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSetSmsServiceSettings(CMtfTestCase& aTestCase);

	};


#endif
