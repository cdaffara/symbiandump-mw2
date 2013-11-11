// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_IMAPMAILSTORE_H
#define __T_IMAPMAILSTORE_H

#include <test/tefunit.h>

#include "emailtestutils.h"
#include "MSVSERV.H"
#include "cactivewaiter.h"

class CMsvPlainBodyText;
class CActiveWaiter ;

//constants shared by the tests
const TInt KTestStringLength=10;
const TInt KMaxFilePathLength=50;
const TInt KBufferGranularity = 256;

_LIT(KEmail40kBodyFile,"c:\\msgtest\\testdata\\T_OnlinePerformance40KBody.txt");

_LIT(KCDrive,"c:\\");
_LIT(KPlainBodyTextTest,"t_plaintextstore");
_LIT(KBodyDetails,"testbody");

// Note that all test suites must begin with "CTest"
class CTestPlainTextStore : public  CActiveTestFixture
	{
public:
	CTestPlainTextStore();
	// SetUp and TearDown code (optional)
	virtual void SetupL();
	virtual void TearDownL();
	static CTestSuite* CreateSuiteL(const TDesC& aName);

	void TestStoreRestore8BitChunkAsyncL();
	void TestStoreRestore8BitChunkSynchL();
	void TestStoreRestore16BitChunkAsyncL();
	void TestStoreRestore16BitChunkSynchL();
	void TestStoreRestore16BitChunkAsyncWith8BitL();	
	void TestStoreRestore16BitChunkSynchWith8BitL();
	void TestStoreRestore16BitChunkAsynchStoreWith8BitL();
	void TestStoreRichTextAsPlainTextL();
	void TestStoreRestoreRichTextL();
private:
	CMsvPlainBodyText* iPlainBodyTextStore;
	TMsvId iMsvMessageEntry;
	TMsvId iBodyMessageEntry;
	TMsvId iFolderId;
	CMsvServerEntry* iServerEntry;
	CMsvServer*	iMsvServer;
	CActiveWaiter* iActiveWaiter;
	TUint iDefaultCharset;
	HBufC8* iFilebuf1;
	HBufC8* iFilebuf2;
	HBufC8* iFilebuf3;
	HBufC16* iFilebuf4;
	TInt iSize;
	};

#endif //__T_IMAPMAILSTORE_H
