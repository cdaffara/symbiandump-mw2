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


#ifndef __CMTF_SYNCHRONOUS_TEST_ACTION_H__
#define __CMTF_SYNCHRONOUS_TEST_ACTION_H__

#include "CMtfTestAction.h"

/** Abstract base class for synchronous test actions. Test actions derived from this
class do not need to override RunL and DoCancel, only ExecuteActionL needs to be overriden. */
class CMtfSynchronousTestAction : public CMtfTestAction
{
public:
	virtual void RunL();
	virtual void DoCancel();
	
protected:
	CMtfSynchronousTestAction(CMtfTestCase& aTestCase);
};

#endif

