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


#ifndef __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_H__
#define __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionCheckChildrenCount,"CheckChildrenCount");


class CMtfTestActionCheckChildrenCount : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCheckChildrenCount(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionCheckChildrenCount(CMtfTestCase& aTestCase);
	};


#endif
