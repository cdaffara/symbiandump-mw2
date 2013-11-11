// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "msvtestutils.h"
#include <msvstd.h>
#include "msvtestutils.h"
#include "t_schsendutils.h"



class CSchSendExeTest : public CSchSendTestUtils
	{
	public:
		static CSchSendExeTest* NewL(RTest& aTest);
		~CSchSendExeTest();

	protected:

		CSchSendExeTest(RTest& aTest);

		void ShowMenuL();
		void RunAutoL();

		void TestSessionFailureL();
		void TestOperationFailureL();
	};
