/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_INITIALISE_SMS_DEFAULT_SETTINGS_H__
#define __CMTF_TEST_ACTION_INITIALISE_SMS_DEFAULT_SETTINGS_H__

//#include <centralrepository.h>
//#include "csmsaccount.h"

#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionInitialiseSmsDefaultSettings,"InitialiseSmsDefaultSettings");


class CMtfTestActionInitialiseSmsDefaultSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);
	virtual ~CMtfTestActionInitialiseSmsDefaultSettings(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionInitialiseSmsDefaultSettings(CMtfTestCase& aTestCase);

private:
	
	};



#endif
