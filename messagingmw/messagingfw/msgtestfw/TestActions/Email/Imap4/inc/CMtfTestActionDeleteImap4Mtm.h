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


#ifndef __CMTF_TEST_ACTION_DELETE_IMAP4_MTM_H__
#define __CMTF_TEST_ACTION_DELETE_IMAP4_MTM_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionDeleteImap4Mtm,"DeleteImap4Mtm");


class CMtfTestActionDeleteImap4Mtm : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionDeleteImap4Mtm(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionDeleteImap4Mtm(CMtfTestCase& aTestCase);
	};


#endif
