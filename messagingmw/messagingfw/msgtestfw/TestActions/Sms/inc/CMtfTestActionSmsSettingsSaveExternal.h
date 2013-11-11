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


#ifndef __CMTF_TEST_ACTION_SMS_SETTINGS_SAVE_EXTERNAL_H__
#define __CMTF_TEST_ACTION_SMS_SETTINGS_SAVE_EXTERNAL_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionSmsSettingsSaveExternal,"SmsSettingsSaveExternal");

class CMtfTestActionSmsSettingsSaveExternal : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSmsSettingsSaveExternal(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSmsSettingsSaveExternal(CMtfTestCase& aTestCase);

	};


#endif
