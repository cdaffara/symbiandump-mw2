// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef T_SMCM_H_
#define T_SMCM_H_
#include <e32base.h>

#include <s32std.h>
#include <txtrich.h>    //CRichText
#include <e32test.h>
#include <msvuids.h>	// ServiceEntry, FolderEntry, etc.
#include <mtmuids.h>	// query capabilities
#include "smutset.h"	// sms settings
#include "smuthdr.h"	// sms header
#include "smsclnt.h"	// sms (a)sync commands
#include "smstestbase.h"

const TUid KFileStreamId		= {0x01010101};
const TUid KUidSmcmTestNonSense = {0x00011111};

const TMsvPartList KMsvMessagePartBody			= 0x00000001;
const TMsvPartList KMsvMessagePartRecipient		= 0x00000002;
const TMsvPartList KMsvMessagePartOriginator	= 0x00000004;
const TMsvPartList KMsvMessagePartDescription	= 0x00000008;
const TMsvPartList KMsvMessagePartDate			= 0x00000010;
const TMsvPartList KMsvMessagePartAttachments	= 0x00000020;


_LIT(KSmsClientMtmTestName, "SMS Client MTM");
_LIT(KSmsUtilitiesTestName, "SMS Utilities");

class CSmcmTest : public CSmsTestBase
	{
public:
	static CSmcmTest* NewLC(CSmsTestUtils& aTestUtils, const TDesC& aScriptFile, TInt& aNextTest);
	~CSmcmTest();

private:
	CSmcmTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	void ConstructL();

	void RunL() {};
	void DoCancel() {};

	void ShowMenuL();
	void RunAutoL();

	//Test Functions
	void TestSettingsL();

	//Client Mtm Test Functions
	void TestCapabilitiesL();
	void TestFindL();
	void TestReplyL();
	void TestForwardL();
	void TestSyncFunctionsL();

	CSmsHeader*			iSmsHeader;
	CRichText*			iRichText;
	CParaFormatLayer*	iParaLayer;
	CCharFormatLayer*	iCharLayer;
	};

class TCompare
	{
	public:
		TCompare(TBool aVal = ETrue) {iValue = aVal;}
		TCompare(const TCompare& aVal) {iValue = aVal.iValue;}
		void operator()(TBool aResult) {iValue = iValue && aResult;}
		operator TBool() const {return iValue;}
		TBool iValue;
	};

class CSmutTest : public CSmsTestBase
	{
public:
	static CSmutTest* NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	~CSmutTest();

private:
	CSmutTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest);
	void ConstructL();

	void RunL() {};
	void DoCancel() {};

	void ShowMenuL();
	void RunAutoL();

	//Test Functions
	void TestSettingsL();
	void TestHeaderL();
	void TestSmsUtilitiesL();
	void TestSmsUtilities1L(CSmsHeader& aHeader);
	void TestSmsUtilities2L(CSmsHeader& aHeader);
	void TestSmsUtilities3L(CSmsHeader& aHeader);
	void TestInvalidTelOrContact(const TDesC& aTel, TDes& rDetails);
	void TestValidTel(const TDesC& aTel, const TDesC& aExpect1, const TDesC& aExpect2, const TDesC& aNotExpect, TDes& rDetails);

	//CSmsSettings utilities/tests
	void TestSettingsStoreToCommDbL();
	TBool CompareSettingsAndCommDbL(const CSmsSettings& aSettings) const;
	TBool CompareSettingsAndSmsBearerL(const CSmsSettings& aSettings) const;
//	void OpenCommDbModemTableLCC(CCommsDatabase*& rCommDb, CCommsDbTableView*& rTable) const;
	void TestSettingsRestoreDefectL();
	TBool CompareSettings(const CSmsSettings& aLeft, const CSmsSettings& aRight, TBool aTestSC = ETrue);
	TBool CompareMessageSettings(const CSmsMessageSettings& aLeft, const CSmsMessageSettings& aRight);
	void TestSettingsMembersL(CSmsSettings& aSettings);
	void TestMessageSettingsMembersL(CSmsMessageSettings& aSettings);
	void StoreSettingsL(const CSmsSettings& aSettings);
	void RestoreSettingsL(CSmsSettings& aSettings);
	void StoreHeaderL(const CSmsHeader& aHeader);
	void RestoreHeaderL(CSmsHeader& aHeader);

	//CSmsNumber utilities
	void TestNumberL();
	void StoreNumberL(const CSmsNumber& aNumber, TUid aStreamId = KFileStreamId);
	void RestoreNumberL(CSmsNumber& rNumber, TUid aStreamId = KFileStreamId);
	TBool CompareNumbers(const CSmsNumber& aLeft, const CSmsNumber& aRight);
	TBool CompareServiceCenters(const CSmsServiceCenter& aLeft, const CSmsServiceCenter& aRight);

	CSmsHeader*			iSmsHeader;
	CRichText*			iRichText;
	CParaFormatLayer*	iParaLayer;
	CCharFormatLayer*	iCharLayer;
	};

#endif
