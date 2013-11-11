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

#ifndef __TEST_UTILITY_SERVER_OBSERVER_H__
#define __TEST_UTILITY_SERVER_OBSERVER_H__

// epoc include
#include <msvapi.h>


/**
An observer class for 
*/
class CTestUtilityServerObserver : public CBase, public MMsvSessionObserver
{
public:
	static CTestUtilityServerObserver* NewL();
	static CTestUtilityServerObserver* NewLC();
	~CTestUtilityServerObserver();	
	virtual void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	
private:
	CTestUtilityServerObserver();
	void ConstructL();
};
#endif // ! defined __TEST_UTILITY_SERVER_OBSERVER_H__
