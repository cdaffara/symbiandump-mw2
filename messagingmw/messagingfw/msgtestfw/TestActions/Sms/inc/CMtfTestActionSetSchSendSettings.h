// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_SET_SCHSEND_SETTINGS_H__
#define __CMTF_TEST_ACTION_SET_SCHSEND_SETTINGS_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSetSchSendSettings,"SetSchSendSettings");

/**
The test action sets the schedule send related settings from a configuration file.

The action first get the settings from the configuration file.  It then
creates and sets the two object CMsvScheduleSettings and CMsvSysAgentActions.  Finally,
it stores the objects to the SMS service entry store.

@internalTechnology
*/
class CMtfTestActionSetSchSendSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionSetSchSendSettings(CMtfTestCase& aTestCase);
	};

#endif
