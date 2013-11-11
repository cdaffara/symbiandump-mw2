// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//
/**
@test
@internalComponent

This contains CT_ConverterServer
*/

#ifndef __C_TEF_INTEGRATION_TEST_SERVER__
#define __C_TEF_INTEGRATION_TEST_SERVER__

#include <test\testblockcontroller.h>
#include <test\testserver2.h>

#include "t_converterwrapper.h"


_LIT(KConverterWrapper, "ConverterWrapper");
/*_LIT(KOomTestWrapper, "OomTestWrapper");
_LIT(KCenRepTestWrapper, "CenRepTestWrapper");
_LIT(KSetSubstTestWrapper, "SetSubstTestWrapper");*/


class CT_ConverterTestBlock : public CTestBlockController
	{
public:
	CT_ConverterTestBlock() : CTestBlockController() {}
	~CT_ConverterTestBlock() {}

	CDataWrapper* CreateDataL(const TDesC& aData)
		{
		CDataWrapper* wrapper = NULL;
		if (KConverterWrapper() == aData)
			{
			wrapper = CT_ConverterWrapper::NewL();
			}
		return wrapper;
		}
	};

class CT_ConverterServer : public CTestServer2
	{
public:
	CT_ConverterServer() {}
	~CT_ConverterServer() {}

	static CT_ConverterServer* NewL();

	CTestBlockController*	CreateTestBlock()
		{
		CTestBlockController* controller = new (ELeave) CT_ConverterTestBlock();
		return controller;
		}
	};

#endif // __C_TEF_INTEGRATION_TEST_SERVER__
