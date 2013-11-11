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

#include "smstestutils.h"

#include <msvapi.h>
#include <popcmtm.h>

class TestUiTimer;
class CInternetAccessTester : public CBase
	{
public:
	static CInternetAccessTester* NewL(CSmsTestUtils& aTestUtils);
	void StartL(TMsvId aEntryId);
	~CInternetAccessTester();
private:
	CInternetAccessTester(CSmsTestUtils& aTestUtils);
	
	virtual void ConstructL();

	CMsvSession& Session()	{return *iSmsTest.iMsvSession;}
	CMsvEntry& MsvEntry()	{return *iSmsTest.iMsvEntry;}
	CSmsClientMtm& Client()	{return *iSmsTest.iSmsClientMtm;}

	void GetParsedFieldStoreL();
	void InternalizeL(RMsvReadStream& aReadStream);

	void GetIspAndMailServiceL();
	void GetMailServiceL(const TDesC& aServiceName);
	void GetIAPL(const TDesC& aIspName);

	void TestRefreshL();
	void TestQuitL();

private:
	enum TIapTestState
		{
		EIapTestGetIapAndService,
		EIapTestRefresh,
		EIapTestQuit
		} 
		iState;

	CSmsTestUtils&				iSmsTest;
	TMsvId						iEntryId;
	CArrayPtrSeg<CParsedField>* iParsedFieldArray;
	CMsvEntrySelection*			iSelection;

	TMsvId						iServiceId;
	TUint32						iIap;

	CClientMtmRegistry*			iClientMtmRegistry;
	CPop3ClientMtm*				iPop3ClientMtm;
	TestUiTimer*				iTimer;
	};
