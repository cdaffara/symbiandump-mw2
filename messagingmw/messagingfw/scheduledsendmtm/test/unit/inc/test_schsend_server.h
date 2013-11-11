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

#ifndef __TEST_SCHSEND_SERVER_H__
#define __TEST_SCHSEND_SERVER_H__

#include <test/testexecuteserverbase.h>

/**
Schedule Send Test Server

@internalTechnology
*/
class CTest_SchSend_Server : public CTestServer
	{
public:
	static CTest_SchSend_Server* NewL();
	
	//method from CTestServer
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
	};
#endif

