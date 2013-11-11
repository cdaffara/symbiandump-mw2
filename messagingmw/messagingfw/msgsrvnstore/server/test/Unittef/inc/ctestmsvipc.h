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
//

#ifndef CTESTMSVIPC_H
#define CTESTMSVIPC_H

#include <test/tefunit.h>
#include <msvsearchsortquery.h>
#include <msvsearchsortoperation.h>
#include "cactivewaiter.h"
#include <msvstd.h>
#include <e32std.h>
#include "msvdbadapter.h"

/**
@internalTechnology
@prototype
*/
class CTestMsvIpc : public  CActiveTestFixture
// WARNING: Despite being a C class, CActiveTestFixture is NOT derrived from CBase
// ... so don't expect your data members to be zero'ed
	{
public:
	void SetupL();
	void TearDownL();
	
	CTestMsvIpc(); 
	~CTestMsvIpc();

	TInt AllocateBuffer();
	void TestPackUnpackQueryL();
	void TestBufferOverflowForPackUnpackQueryL();
	void TestPackUnpackTMsvIdArrayL();
	void TestBufferOverFlowPackUnpackTMsvIdArrayL();
	
	static CTestSuite* CreateSuiteL(const TDesC& aName);
	
private:
	HBufC8* iBuffer;
	};

#endif // CTESTMSVIPC_H
