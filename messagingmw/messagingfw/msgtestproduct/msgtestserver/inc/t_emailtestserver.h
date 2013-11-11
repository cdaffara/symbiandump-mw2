// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// @file
// This contains CT_EmailTestServer
//
//

/**
 @test
 @internalComponent
*/

#ifndef __C_EMAIL_TEST_SERVER__
#define __C_EMAIL_TEST_SERVER__

#include <test/testblockcontroller.h>
#include <test/testserver2.h>

#include "t_imaptestwrapper.h"


_LIT(KImapTestWrapper, "ImapTestWrapper");


class CT_EmailTestBlock : public CTestBlockController
	{
public:
	CT_EmailTestBlock() : CTestBlockController() {}
	~CT_EmailTestBlock() {}

	CDataWrapper* CreateDataL(const TDesC& aData)
		{
		CDataWrapper* wrapper = NULL;
		if (KImapTestWrapper() == aData)
			{
			wrapper = CT_ImapTestWrapper::NewL();
			}
		return wrapper;
		}
	};

class CT_EmailTestServer : public CTestServer2
	{
public:
	CT_EmailTestServer() {}
	~CT_EmailTestServer() {}

	static CT_EmailTestServer* NewL();

	CTestBlockController*	CreateTestBlock()
		{
		CTestBlockController* controller = new (ELeave) CT_EmailTestBlock();
		return controller;
		}
	};

#endif // __C_EMAIL_TEST_SERVER__
