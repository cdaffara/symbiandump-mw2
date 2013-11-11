/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This contains the header file for server
* 
*
*/



/**
 @file
*/

#ifndef __C3GPPSUPPORTTESTSERVER_H__
#define  __C3GPPSUPPORTTESTSERVER_H__


//for accessing TEF classes

#include <test/testexecuteserverbase.h>
#include "MSharedData.h"

_LIT(KServerName,"3GppSupportTestServer");

class CEntryCmdHandler;

namespace SharedData
{
class C3GppSupportTestServer : public CTestServer, public MSharedData
{
public:
	static C3GppSupportTestServer*	NewL();
	virtual CTestStep*	CreateTestStep(const TDesC& aStepName);
	C3GppSupportTestServer();
	~C3GppSupportTestServer();

	virtual MMsvEntryObserver* GetObserverHandle(); //MSharedData
	
protected:
	void ConstructL();
	
private:
	CTestStep* CreateTestL(const TDesC& aStepName);

private:
	TInt	iSharedIntData;
	CEntryCmdHandler* iEntryObserver;
	RProcess iWatcher;
	TBool iCreateFlag;
};
}
#endif //__C3GPPSUPPORTTESTSERVER_H__