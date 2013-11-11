// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CWAPPUSHFAILMESSAGETEST_H__
#define __CWAPPUSHFAILMESSAGETEST_H__

#include "pushtests.h"

class CWapPushFailMessageTest : public CWapPushBaseMessageTest
	{
	virtual const TDesC& TestName();
	
	virtual void NumberTestCasesToRun();
	virtual void PrepareTestMessageL(TInt aCurrentTestCase);
	// new to validate expected test messages(s) stored ok...
	virtual TBool ConfirmMessagesSavedL();
	};

#endif
