// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the test server for all SUPL SMS Trigger unit tests.
// 
//

#if (!defined __TE_LBSSUPLSMSTRIGGER_SERVER_H__)
#define __TE_LBSSUPLSMSTRIGGER_SERVER_H__

/**
@file
@internalComponent
@prototype
*/

#include <testexecuteserverbase.h>

/**
This is the test server for all SUPL SMS Trigger unit tests.
*/
class CTe_LbsSuplSmsTriggerSuite : public CTestServer
	{
public:
	static CTe_LbsSuplSmsTriggerSuite* NewL();

	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	
protected:
	void ConstructL();
	
private:
	CTestStep* CreateTestStepL(const TDesC& aStepName);
	};

#endif //__TE_LBSSUPLSMSTRIGGER_SERVER_H__
