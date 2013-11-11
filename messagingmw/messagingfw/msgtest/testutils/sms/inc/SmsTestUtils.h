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

#ifndef SMS_TEST_H_
#define SMS_TEST_H_

#include "msvtestutils.h"
#include <smutset.h>
#include <etelmm.h>

// System Agent header files
#include <e32property.h>


_LIT(KSmsServerMtmFileName, "z:\\system\\libs\\smss.dll");
_LIT(KSmsClientMtmFileName, "z:\\system\\libs\\smsc.dll");
// The ui data mtms are not instantiated in practice
_LIT(KSmsUiMtmFileFileName, "z:\\system\\libs\\smum.dll");

_LIT(KSmsDataComponentFileName, "c:\\system\\mtm\\sms.dat");

_LIT(KSmsServiceCenterNumber, "+447785016005");  //  Not used, CJCW!  

//SMS Scripting

_LIT(KSmsComponent, "sms");

//SMS Script Valid Script Items

_LIT(KScriptItemMessage, "Message");

_LIT(KScriptItemBioUid, "BioUid");
const TInt KScriptItemBioUidDef		= 0;

_LIT(KScriptItemSC, "SC");
_LIT(KScriptItemSCDef, "+447785016005");

_LIT(KScriptItemSCUseDefault, "Default");

_LIT(KScriptItemMessageType, "Type");
_LIT(KScriptItemSubmit, "Submit");
_LIT(KScriptItemDeliver, "Deliver");
_LIT(KScriptItemStatusReport, "SR");
_LIT(KScriptItemCommand, "Command");

_LIT(KScriptItemMessageClass, "Class");
_LIT(KScriptItemClassDefault, "DEF");
_LIT(KScriptItemClass0, "0");
_LIT(KScriptItemClass1, "1");
_LIT(KScriptItemClass2, "2");
_LIT(KScriptItemClass3, "3");

_LIT(KScriptItemRecipients, "Recipients");
_LIT(KScriptItemRecipientsDef, "+447747065xxx");
_LIT(KScriptItemRecipients2Def, "+447747065xxx");

_LIT(KScriptItemBearer, "Bearer");
_LIT(KScriptItemBearerDef, "NBS");

_LIT(KScriptItemEncoding, "Encoding");
const TInt KScriptItemEncodingDef	= 7;

_LIT(KScriptItemDeliveryReport, "DeliveryReport");
_LIT(KScriptItemDeliveryReportDef, "DEF");

_LIT(KScriptItemFile, "File");

const TUid KVCardBioUid				= {0x10005534};
const TUid KVCalBioUid				= {0x10005533};

enum TSmsTestUtilsPanic
	{
	KSmsTestUtilsPanicNotOnClientSide,
	KSmsTestUtilsPanicSettingsNotConstructed,
	KSmsTestUtilsPanicSmsTestUtilsCreationFlagCombination,
	KSmsTestUtilsPanicSmsServiceIdNotSet,
	KSmsTestUtilsPanicCannotCompareSameIds
	};

class CSmsHeader;
class CSmsClientMtm;
class CSmsSettings;
class CParaFormatLayer;
class CCharFormatLayer;
class CRichText;
class CScriptFile;

class CSmsTestUtils : public CMsvTestUtils
	{
public:
	IMPORT_C static CSmsTestUtils* NewL(RTest& aRTest, TUint aCreationFlags = ETuMinimalClientSide);
	IMPORT_C static CSmsTestUtils* NewLC(RTest& aRTest, TUint aCreationFlags = ETuMinimalClientSide);
	
	IMPORT_C void InstallSmsMtmGroupL();
	IMPORT_C void InstantiateSmsClientMtmL();
	IMPORT_C void Reset();
	IMPORT_C ~CSmsTestUtils();

	IMPORT_C void CreateSmsServerMtmRegL();
	IMPORT_C TMsvId CreateSmsServiceL();
	IMPORT_C void InstantiateSmsServerMtmL();
	IMPORT_C void DeleteSmsServiceL();
	IMPORT_C TMsvId FindExistingSmsServiceL();

	IMPORT_C TBool ReadScriptL(const TDesC& aFile, TMsvId aDestFolder, CMsvEntrySelection& aSelection, const TTime& aDate);
	IMPORT_C void CreateMessageL(TMsvId aDestFolder, TMsvEntry& aEntry, CSmsHeader& aHeader, CRichText& aBody, const TTime& aDate);

	IMPORT_C TMsvId CreateDummyMessageToSendL();
	IMPORT_C TMsvId CreateDummyMessageToReceiveL();

	IMPORT_C void AddRecipientL(TMsvId aId, const TDesC& aNumber);
	IMPORT_C void AddSystemAgentConditionsL();

	IMPORT_C void NotifySaPhoneOnL();
	IMPORT_C void NotifySaPhoneOffL();
	IMPORT_C void WaitForInitializeL();

	IMPORT_C void DeleteSmsMessagesL(TMsvId aFolder);

	inline void SetProgressL(CMsvOperation& iOperation);

	//From CMsvTestUtils
	void InstantiateClientMtmsL()	{InstantiateSmsClientMtmL();}
	void InstantiateServerMtmsL()	{InstantiateSmsServerMtmL();}
	void DeleteServicesL()			{DeleteSmsServiceL();}
	void CreateServicesL()			{CreateSmsServiceL();}
	IMPORT_C void FindExistingServicesL();
	void InstallMtmGroupsL()		{InstallSmsMtmGroupL();}
	void CreateServerMtmRegsL()		{CreateSmsServerMtmRegL();}

	IMPORT_C TBool AreMessagesSameTextL(TMsvId aSentId, TMsvId aRecvId, TBool aPrintIfMismatch = EFalse);

	IMPORT_C void DisplaySendingStatesL(const CMsvEntrySelection& aSelection);
	IMPORT_C TBool SendingCompleteL(const CMsvEntrySelection& aSelection, TInt& rError);

	IMPORT_C void SetCommDbServiceCenterFromScriptL(const TDesC& aScriptFile);

	IMPORT_C CSmsHeader* GetHeaderLC(TMsvId aId);

	IMPORT_C void SaveBodyToFileL(TMsvId aId);
	IMPORT_C void DisplayMessagesL(const CMsvEntrySelection& aSelection);
	IMPORT_C void DisplayMessageL(TMsvId aId);

protected:

	IMPORT_C CSmsTestUtils(RTest& aRTest);
	IMPORT_C void ConstructL(TUint aCreationFlags);
	IMPORT_C void Panic(TInt aPanic);
	IMPORT_C void Panic (TSmsTestUtilsPanic aPanic);

private:

	TBool CreateMessagesL(TMsvId aDestFolder, CScriptFile& rSections, CMsvEntrySelection& rSelection, const TTime& aDate);

	void DisplayLogEventData(const CLogEvent& aEvent);

	TBool DoReadScriptL(const TDesC& aFile, TMsvId aDestFolder, CMsvEntrySelection& aSelection, const TTime& aDate);
	void DisplayChildDetailsL(const TMsvEntry& aEntry);
	CSmsHeader* DoGetHeaderLC(CMsvEntry& aEntry);

public:

	CSmsClientMtm*		iSmsClientMtm;			
	TMsvId				iSmsServiceId;
	CSmsSettings*		iServiceSettings;
	TSmsProgress		iProgress;

	CParaFormatLayer*	iParaFormat;
	CCharFormatLayer*	iCharFormat;
	CRichText*			iRichText;

	TBool iSmokeTest; //if ETrue, then smaller number of tests will be run
	};

#define SMSTEST_PANIC _L("Sms_Test")

#include <smstestutils.inl>

#endif
