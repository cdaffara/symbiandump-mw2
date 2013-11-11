/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_SET_IMAP4_SERVICE_LONG_IDLE_PARAMETERS_H__
#define __CMTF_TEST_ACTION_SET_IMAP4_SERVICE_LONG_IDLE_PARAMETERS_H__


#include "CMtfSynchronousTestAction.h"
#include "CMtfTestActionUtilsConfigFileParser.h"

_LIT(KTestActionSetImap4ServiceLongIdleParameters,"SetImap4ServiceLongIdleParameters");


class CMtfTestActionSetImap4ServiceLongIdleParameters : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSetImap4ServiceLongIdleParameters(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionSetImap4ServiceLongIdleParameters(CMtfTestCase& aTestCase);

private:
	CMtfTestActionUtilsConfigFileParser* iParser;
	};


#endif
