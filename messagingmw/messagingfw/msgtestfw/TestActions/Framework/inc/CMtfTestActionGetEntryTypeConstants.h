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

#ifndef __CMTF_TEST_ACTION_GET_ENTRY_TYPE_CONSTANTS_H__
#define __CMTF_TEST_ACTION_GET_ENTRY_TYPE_CONSTANTS_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionGetEntryTypeConstants,"GetEntryTypeConstants");

class CMtfTestActionGetEntryTypeConstants : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetEntryTypeConstants(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionGetEntryTypeConstants(CMtfTestCase& aTestCase);
	};


#endif // __CMTF_TEST_ACTION_GET_ENTRY_TYPE_CONSTANTS_H__
