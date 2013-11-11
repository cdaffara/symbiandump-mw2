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

#ifndef __CMTF_TEST_ACTION_WAIT_H__
#define __CMTF_TEST_ACTION_WAIT_H__

#include "CMtfTestAction.h"
class CMtfTestActionParameters;
class CMtfTestCase;

/** Used by the framework to handle wait actions. */
class CMtfTestActionWait: public CMtfTestAction
{
public:
	virtual ~CMtfTestActionWait();
	static CMtfTestActionWait* NewL(CMtfTestCase& aTestCase);		
	
	/** Creates a new ait action. Takes ownership of aActionIds at the end. */
	static CMtfTestActionWait* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionIds);		
	
	virtual void ExecuteActionL();
	virtual void RunL();
	virtual void DoCancel();
	virtual TBool WaitAction() const;
		
private:
	CMtfTestActionWait(CMtfTestCase& aTestCase, const TBool& aWaitForAll);
	
private:
	TBool	iWaitForAll;	// flag is true if this is a WaitForAll action
};

#endif

