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
// This contains CT_MediaTestServer
// 
//

/**
 @test
 @internalComponent
*/

#ifndef __C_TEF_INTEGRATION_TEST_SERVER__
#define __C_TEF_INTEGRATION_TEST_SERVER__

#include <test\testblockcontroller.h>
#include <test\testserver2.h>

#include "t_mediatestwrapper.h"
#include "t_oomtestwrapper.h"
#include "t_cenreptestwrapper.h"
#include "t_setsubsttestwrapper.h"


_LIT(KMediaTestWrapper, "MediaTestWrapper");
_LIT(KOomTestWrapper, "OomTestWrapper");
_LIT(KCenRepTestWrapper, "CenRepTestWrapper");
_LIT(KSetSubstTestWrapper, "SetSubstTestWrapper");


class CT_MediaTestBlock : public CTestBlockController
	{
public:
	CT_MediaTestBlock() : CTestBlockController() {}
	~CT_MediaTestBlock() {}

	CDataWrapper* CreateDataL(const TDesC& aData)
		{
		CDataWrapper* wrapper = NULL;
		if (KMediaTestWrapper() == aData)
			{
			wrapper = CT_MediaTestWrapper::NewL();
			}
		else if (KOomTestWrapper() == aData)
			{
			wrapper = CT_OomTestWrapper::NewL();
			}
		else if (KCenRepTestWrapper() == aData)
			{
			wrapper = CT_CenRepTestWrapper::NewL();
			}
		else if (KSetSubstTestWrapper() == aData)
			{
			wrapper = CT_SetSubstTestWrapper::NewL();
			}
		return wrapper;
		}
	};

class CT_MediaTestServer : public CTestServer2
	{
public:
	CT_MediaTestServer() {}
	~CT_MediaTestServer() {}

	static CT_MediaTestServer* NewL();

	CTestBlockController*	CreateTestBlock()
		{
		CTestBlockController* controller = new (ELeave) CT_MediaTestBlock();
		return controller;
		}
	};

#endif // __C_TEF_INTEGRATION_TEST_SERVER__
