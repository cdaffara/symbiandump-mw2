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

#ifndef __CMTF_TEST_ACTION_DELETE_FILE_STORE_H__
#define __CMTF_TEST_ACTION_DELETE_FILE_STORE_H__

#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionDeleteFileStore,"DeleteFileStore");

class CMtfTestActionDeleteFileStore : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionDeleteFileStore(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionDeleteFileStore(CMtfTestCase& aTestCase);
	};

#endif //__CMTF_TEST_ACTION_DELETE_FILE_STORE_H__
