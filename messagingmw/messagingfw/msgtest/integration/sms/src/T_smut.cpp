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

#include "T_smcm.h"
#include <commsdattypesv1_1.h>
#include <commsdat.h>
#include <etelmm.h>
#include <smutset.h>

using namespace CommsDat;

	#include <csmsaccount.h>

CSmutTest::CSmutTest(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
: CSmsTestBase(aSmsTest, aScriptFile, aNextTest)
	{
	}

void CSmutTest::RunAutoL()
	{
	// Testing settings (CSmsSettings)
	iSmsTest.TestStart(++iNextTest, _L("CSmsSettings"));  
	TestSettingsL();
	iSmsTest.TestFinish(iNextTest);

	// Testing number class (CSmsNumber)
	iSmsTest.TestStart(++iNextTest, _L("CSmsNumber"));  
	TestNumberL();
	iSmsTest.TestFinish(iNextTest);

	// Testing with header (CSmsHeader)
	iSmsTest.TestStart(++iNextTest, _L("CSmsHeader"));
	TestHeaderL();
	iSmsTest.TestFinish(iNextTest);

	// Testing with utility class (TSmsUtilities)
	iSmsTest.TestStart(++iNextTest, _L("TSmsUtilities"));
	TestSmsUtilitiesL();
	iSmsTest.TestFinish(iNextTest);

	}

CSmutTest::~CSmutTest()
	{
	delete iSmsHeader;
	delete iRichText;
	delete iParaLayer;
	delete iCharLayer;
	}

void CSmutTest::ConstructL()
	{
	SetTestNameL(KSmsUtilitiesTestName);
	iSmsTest.SetLogToFile();

	iSelection = new (ELeave) CMsvEntrySelection();
	iTimer = CTestTimer::NewL();

	iParaLayer =CParaFormatLayer::NewL();
	iCharLayer = CCharFormatLayer::NewL();
	iRichText = CRichText::NewL(iParaLayer, iCharLayer, CEditableText::EFlatStorage,256);
	CActiveScheduler::Add(this);
	}

CSmutTest* CSmutTest::NewLC(CSmsTestUtils& aSmsTest, const TDesC& aScriptFile, TInt& aNextTest)
	{
	CSmutTest* self = new (ELeave) CSmutTest(aSmsTest, aScriptFile, aNextTest);
	CleanupStack::PushL(self);

	self->ConstructL();

	return self;
	}

void CSmutTest::ShowMenuL()
	{
	iSmsTest.ResetMenu();

	iSmsTest.AppendToMenuL(_L("Start"));

	TInt result = iSmsTest.DisplayMenu(_L("SMS Client MTM Test"));

	if (result <= 0)
		return;

	switch (result)
		{
		case 1:
			RunAutoL();
			break;
		default:
			User::Leave(KErrArgument);
			break;
		}

	if (!iSmsTest.RunAuto())
		{
		iSmsTest.Printf(_L("Press any key to continue...\n"));
		iSmsTest.Test().Getch();
		}

	ShowMenuL();
	}

void CSmutTest::TestSettingsL()
	{
	iSmsTest.Printf(_L("Testing Create Settings...\n"));

	iSmsTest.SetEntryL(iSmsTest.iSmsServiceId);

// First we test all the features of the class CSmsSettings
// We give them different values and compare if it works

	CSmsSettings* settings = CSmsSettings::NewL();
	CleanupStack::PushL(settings);
	iSmsTest(CompareSettings(*settings, *settings));
	TestSettingsMembersL(*settings);
	TestMessageSettingsMembersL(*settings);

// Lets copy the values to a different instance
	CSmsSettings* smsSettings=CSmsSettings::NewL();
	CleanupStack::PushL(smsSettings);
	smsSettings->CopyL(*settings);
	iSmsTest(CompareSettings(*smsSettings, *settings));
	CleanupStack::PopAndDestroy(smsSettings);
	smsSettings = NULL;
	iSmsTest.Printf(_L("Test CSmsSettings::CopyL() passed\n"));

//	Lets create a sms service in the file system with these values
	StoreSettingsL(*settings);

	iSmsTest.Printf(_L("Stored CSmsSettings against the service\n"));

// Lets read the service and see it the values are still ok
	CSmsSettings* set1=CSmsSettings::NewL();
	CleanupStack::PushL(set1);

	RestoreSettingsL(*set1);

	iSmsTest.Printf(_L("Restored CSmsSettings\n"));

	iSmsTest(CompareSettings(*set1, *settings));

	CleanupStack::PopAndDestroy(set1);

// Lets make the previous test again; Lets give a settings instance some
// values, store it, restore it and see that the values are still the same
	CSmsSettings* set2=CSmsSettings::NewL();
	CleanupStack::PushL(set2);
	set2->AddServiceCenterL(_L("Radiolinja0"),_L("+358508771010"));
	set2->AddServiceCenterL(_L("Radiolinja1"),_L("+358508771010"));
	set2->AddServiceCenterL(_L("Radiolinja2"),_L("+358508771010"));
	set2->AddServiceCenterL(_L("Radiolinja3"),_L("+358508771010"));
	set2->AddServiceCenterL(_L("Radiolinja4"),_L("+358508771010"));
	set2->SetDefaultServiceCenter(1);
	iSmsTest(set2->DefaultServiceCenter()==1);
// Validity Period	
	set2->SetValidityPeriod(ESmsVPSixHours);
	iSmsTest(set2->ValidityPeriod().Int()==ESmsVPSixHours);
// Reply Quoted
	set2->SetReplyQuoted(EFalse);
	iSmsTest(!set2->ReplyQuoted());
// Reject Duplicate 
	set2->SetRejectDuplicate(ETrue);
	iSmsTest(set2->RejectDuplicate());
// Delivery Method
	set2->SetDelivery(ESmsDeliveryUponRequest);
	iSmsTest(set2->Delivery()==ESmsDeliveryUponRequest);
// Delivery Report
	set2->SetDeliveryReport(ETrue);
	iSmsTest(set2->DeliveryReport());
// Reply Path
	set2->SetReplyPath(EFalse);
	iSmsTest(!set2->ReplyPath());
// Message Conversion
	set2->SetMessageConversion(ESmsConvPIDNone);
	iSmsTest(set2->MessageConversion()==ESmsConvPIDNone);
// Can Concatenate
	set2->SetCanConcatenate(ETrue);
	iSmsTest(set2->CanConcatenate());
// Character Set
	set2->SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabet8Bit);
	iSmsTest(set2->CharacterSet()==TSmsDataCodingScheme::ESmsAlphabet8Bit);
// Validity period format
	set2->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFSemiOctet);
	iSmsTest(set2->ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFSemiOctet);
// Status Report Handling
	set2->SetStatusReportHandling(CSmsSettings::EMoveReportToInboxVisible);
	iSmsTest(set2->StatusReportHandling()==CSmsSettings::EMoveReportToInboxVisible);
// Special Message Handling
	set2->SetSpecialMessageHandling(CSmsSettings::EMoveReportToInboxVisible);
	iSmsTest(set2->SpecialMessageHandling()==CSmsSettings::EMoveReportToInboxVisible);

	StoreSettingsL(*set2);

	iSmsTest.Printf(_L("Stored CSmsSettings 2\n"));


	CSmsSettings* set3=CSmsSettings::NewL();
	CleanupStack::PushL(set3);

	RestoreSettingsL(*set3);

	iSmsTest.Printf(_L("Restored CSmsSettings 2\n"));

	iSmsTest(CompareSettings(*set2, *set3));

	CleanupStack::PopAndDestroy(set3);
	CleanupStack::PopAndDestroy(set2);

	iSmsTest.iServiceSettings->CopyL(*settings);  // Lets copy the current setting to the service settings

	iSmsTest(CompareSettings(*iSmsTest.iServiceSettings, *settings));

	iSmsTest.Printf(_L("CSmsSettings::CopyL() 2\n"));

	CleanupStack::PopAndDestroy(settings);

	iSmsTest.Printf(_L("Test Create Settings Passed\n"));

	TestSettingsStoreToCommDbL();

	iSmsTest.Printf(_L("TestSettingsStoreToCommDbL Passed\n"));

	TestSettingsRestoreDefectL();

	iSmsTest.Printf(_L("TestSettingsRestoreDefectL Passed\n"));
	}

void CSmutTest::TestSettingsMembersL(CSmsSettings& aSettings)
	{
// Status Report Handling (what should be done to report messages)
	aSettings.SetStatusReportHandling(CSmsSettings::EMoveReportToInboxInvisible);
	iSmsTest(aSettings.StatusReportHandling()==CSmsSettings::EMoveReportToInboxInvisible);
	aSettings.SetStatusReportHandling(CSmsSettings::EMoveReportToInboxVisible);
	iSmsTest(aSettings.StatusReportHandling()==CSmsSettings::EMoveReportToInboxVisible);
	aSettings.SetStatusReportHandling(CSmsSettings::EDiscardReport);
	iSmsTest(aSettings.StatusReportHandling()==CSmsSettings::EDiscardReport);
	iSmsTest.Printf(_L("Test Status Report Handling Passed\n"));

// SC handling
	aSettings.AddServiceCenterL(_L("Radiolinja0"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja1"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja2"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja3"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja4"),_L("+358508771010"));
	iSmsTest(aSettings.GetServiceCenter(0).Name()==_L("Radiolinja0"));
	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-3);
	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-2);
	iSmsTest(aSettings.GetServiceCenter(1).Name()==_L("Radiolinja1"));
	iSmsTest(aSettings.GetServiceCenter(2).Name()==_L("Radiolinja4"));
	iSmsTest(aSettings.GetServiceCenter(2).Address()==_L("+358508771010"));
	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-1);
	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-1);
	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-1);
//	aSettings.RemoveServiceCenter(aSettings.ServiceCenterCount()-1); // this would be -1
	aSettings.AddServiceCenterL(_L("Radiolinja0"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja1"),_L("+358508771010"));
	aSettings.AddServiceCenterL(_L("Radiolinja2"),_L("+358508771010"));
	aSettings.SetDefaultServiceCenter(1);
	iSmsTest(aSettings.DefaultServiceCenter()==1);
	aSettings.SetDefaultServiceCenter(2);
	iSmsTest(aSettings.DefaultServiceCenter()==2);
	iSmsTest.Printf(_L("Test Default SC Passed\n"));


// Special Message Handling (what should be done to special messages)
	aSettings.SetSpecialMessageHandling(CSmsSettings::EMoveReportToInboxInvisible);
	iSmsTest(aSettings.SpecialMessageHandling()==CSmsSettings::EMoveReportToInboxInvisible);
	aSettings.SetSpecialMessageHandling(CSmsSettings::EMoveReportToInboxVisible);
	iSmsTest(aSettings.SpecialMessageHandling()==CSmsSettings::EMoveReportToInboxVisible);
	aSettings.SetSpecialMessageHandling(CSmsSettings::EDiscardReport);
	iSmsTest(aSettings.SpecialMessageHandling()==CSmsSettings::EDiscardReport);
	iSmsTest.Printf(_L("Test Special Message Handling Passed\n"));

//CommDbAction
	aSettings.SetCommDbAction(CSmsSettings::ENone);
	iSmsTest(aSettings.CommDbAction() == CSmsSettings::ENone);
	aSettings.SetCommDbAction(CSmsSettings::EStoreToCommDb);
	iSmsTest(aSettings.CommDbAction() == CSmsSettings::EStoreToCommDb);
	iSmsTest.Printf(_L("Test CommDb Action Passed\n"));

//SmsBearerAction
	aSettings.SetSmsBearerAction(CSmsSettings::ENone);
	iSmsTest(aSettings.SmsBearerAction() == CSmsSettings::ENone);
	aSettings.SetSmsBearerAction(CSmsSettings::EStoreToCommDb);
	iSmsTest(aSettings.SmsBearerAction() == CSmsSettings::EStoreToCommDb);
	iSmsTest.Printf(_L("Test SMS Bearer Action Passed\n"));

//SmsBearer
	aSettings.SetSmsBearer(CSmsSettings::ESmsBearerPacketOnly);
	iSmsTest(aSettings.SmsBearer() == CSmsSettings::ESmsBearerPacketOnly);
	aSettings.SetSmsBearer(CSmsSettings::ESmsBearerPacketOnly);
	iSmsTest(aSettings.SmsBearer() == CSmsSettings::ESmsBearerPacketOnly);
	aSettings.SetSmsBearer(CSmsSettings::ESmsBearerCircuitOnly);
	iSmsTest(aSettings.SmsBearer() == CSmsSettings::ESmsBearerCircuitOnly);
	aSettings.SetSmsBearer(CSmsSettings::ESmsBearerPacketPreferred);
	iSmsTest(aSettings.SmsBearer() == CSmsSettings::ESmsBearerPacketPreferred);
	aSettings.SetSmsBearer(CSmsSettings::ESmsBearerCircuitPreferred);
	iSmsTest(aSettings.SmsBearer() == CSmsSettings::ESmsBearerCircuitPreferred);
	iSmsTest.Printf(_L("Test SMS Bearer Passed\n"));

// Reply Quoted
	aSettings.SetReplyQuoted(EFalse);
	iSmsTest(!aSettings.ReplyQuoted());
	aSettings.SetReplyQuoted(ETrue);
	iSmsTest(aSettings.ReplyQuoted());
	iSmsTest.Printf(_L("Test Reply Quoted Passed\n"));

// Delivery Method
	aSettings.SetDelivery(ESmsDeliveryUponRequest);
	iSmsTest(aSettings.Delivery()==ESmsDeliveryUponRequest);
	aSettings.SetDelivery(ESmsDeliveryScheduled);
	iSmsTest(aSettings.Delivery()==ESmsDeliveryScheduled);
	aSettings.SetDelivery(ESmsDeliveryImmediately);
	iSmsTest(aSettings.Delivery()==ESmsDeliveryImmediately);
	iSmsTest.Printf(_L("Test Delivery Passed\n"));
	}

void CSmutTest::TestMessageSettingsMembersL(CSmsMessageSettings& aSettings)
	{

// Reject Duplicate 
	aSettings.SetRejectDuplicate(ETrue);
	iSmsTest(aSettings.RejectDuplicate());
	aSettings.SetRejectDuplicate(EFalse);
	iSmsTest(!aSettings.RejectDuplicate());
	iSmsTest.Printf(_L("Test Reject Duplicate Passed\n"));

// Delivery Report
	aSettings.SetDeliveryReport(ETrue);
	iSmsTest(aSettings.DeliveryReport());
	aSettings.SetDeliveryReport(EFalse);
	iSmsTest(!aSettings.DeliveryReport());
	iSmsTest.Printf(_L("Test Delivery Report Passed\n"));

// Reply Path
	aSettings.SetReplyPath(EFalse);
	iSmsTest(aSettings.ReplyPath()==EFalse);
	aSettings.SetReplyPath(ETrue);
	iSmsTest(aSettings.ReplyPath());
	iSmsTest.Printf(_L("Test Reply Path Passed\n"));

// Validity period format
	aSettings.SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFNone);
	iSmsTest(aSettings.ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFNone);
	aSettings.SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFEnhanced);
	iSmsTest(aSettings.ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFEnhanced);
	aSettings.SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFInteger);
	iSmsTest(aSettings.ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFInteger);
	aSettings.SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFSemiOctet);
	iSmsTest(aSettings.ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFSemiOctet);
	iSmsTest.Printf(_L("Test Validity Period Format Passed\n"));

// Message Conversion (following conversions are supported)
	aSettings.SetMessageConversion(ESmsConvFax);
	iSmsTest(aSettings.MessageConversion()==ESmsConvFax);
	aSettings.SetMessageConversion(ESmsConvX400);
	iSmsTest(aSettings.MessageConversion()==ESmsConvX400);
	aSettings.SetMessageConversion(ESmsConvPaging);
	iSmsTest(aSettings.MessageConversion()==ESmsConvPaging);
	aSettings.SetMessageConversion(ESmsConvMail);
	iSmsTest(aSettings.MessageConversion()==ESmsConvMail);
	aSettings.SetMessageConversion(ESmsConvErmes);
	iSmsTest(aSettings.MessageConversion()==ESmsConvErmes);
	aSettings.SetMessageConversion(ESmsConvSpeech);
	iSmsTest(aSettings.MessageConversion()==ESmsConvSpeech);
	aSettings.SetMessageConversion(ESmsConvPIDNone);
	iSmsTest(aSettings.MessageConversion()==ESmsConvPIDNone);
	iSmsTest.Printf(_L("Test Message Conversion Passed\n"));

// Validity Period	
	aSettings.SetValidityPeriod(ESmsVPHour);
	iSmsTest(aSettings.ValidityPeriod().Int()==ESmsVPHour);
	aSettings.SetValidityPeriod(ESmsVPSixHours);
	iSmsTest(aSettings.ValidityPeriod().Int()==ESmsVPSixHours);
	aSettings.SetValidityPeriod(ESmsVP24Hours);
	iSmsTest(aSettings.ValidityPeriod().Int()==ESmsVP24Hours);
	aSettings.SetValidityPeriod(ESmsVPMaximum);
	iSmsTest(aSettings.ValidityPeriod().Int()==ESmsVPMaximum);
	aSettings.SetValidityPeriod(ESmsVPWeek);
	iSmsTest(aSettings.ValidityPeriod().Int()==ESmsVPWeek);
	iSmsTest.Printf(_L("Test Validity Period Passed\n"));

// Can Concatenate (write messages with more than 160 characters)
	aSettings.SetCanConcatenate(ETrue);
	iSmsTest(aSettings.CanConcatenate());
	aSettings.SetCanConcatenate(EFalse);
	iSmsTest(aSettings.CanConcatenate()==EFalse);
	iSmsTest.Printf(_L("Test Can Concatenate Passed\n"));

// Character Set (following character sets are supported)
	aSettings.SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabet7Bit);
	iSmsTest(aSettings.CharacterSet()==TSmsDataCodingScheme::ESmsAlphabet7Bit);
	aSettings.SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabetUCS2);
	iSmsTest(aSettings.CharacterSet()==TSmsDataCodingScheme::ESmsAlphabetUCS2);
	aSettings.SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabet8Bit);
	iSmsTest(aSettings.CharacterSet()==TSmsDataCodingScheme::ESmsAlphabet8Bit);
	iSmsTest.Printf(_L("Test Character Set Passed\n"));
	}

TBool CSmutTest::CompareSettings(const CSmsSettings& aLeft, const CSmsSettings& aRight, TBool aTestSC)
	{
	TCompare compare(CompareMessageSettings(aLeft, aRight));

	compare(aLeft.CommDbAction() == aRight.CommDbAction());
	compare(aLeft.SmsBearerAction() == aRight.SmsBearerAction());
	compare(aLeft.SmsBearer() == aRight.SmsBearer());
	compare(aLeft.Delivery() == aRight.Delivery());
	compare(CompareBools(aLeft.ReplyQuoted(), aRight.ReplyQuoted()));
	compare(aLeft.SpecialMessageHandling() == aRight.SpecialMessageHandling());
	compare(aLeft.StatusReportHandling() == aRight.StatusReportHandling());

	if (aTestSC)
		{
		compare(aLeft.DefaultServiceCenter() == aRight.DefaultServiceCenter());
		compare(aLeft.ServiceCenterCount() == aRight.ServiceCenterCount());

		TInt count = aLeft.ServiceCenterCount();

		while (count--)
			{
			compare(CompareServiceCenters(aLeft.GetServiceCenter(count), aRight.GetServiceCenter(count)));
			}
		}		

	return compare;
	}

TBool CSmutTest::CompareMessageSettings(const CSmsMessageSettings& aLeft, const CSmsMessageSettings& aRight)
	{
	TCompare compare(CompareBools(aLeft.CanConcatenate(), aRight.CanConcatenate()));
	compare(aLeft.CharacterSet() == aRight.CharacterSet());
	compare(CompareBools(aLeft.DeliveryReport(), aRight.DeliveryReport()));
	compare(aLeft.MessageConversion() == aRight.MessageConversion());
	compare(CompareBools(aLeft.RejectDuplicate(), aRight.RejectDuplicate()));
	compare(CompareBools(aLeft.ReplyPath(), aRight.ReplyPath()));
	compare(aLeft.ValidityPeriod() == aRight.ValidityPeriod());
	compare(aLeft.ValidityPeriodFormat() == aRight.ValidityPeriodFormat());
	return compare;
	}

void CSmutTest::TestSettingsRestoreDefectL()
	{
	//Test fix for defects EDNAALR-4KTKTD (CSmsSettings::RestoreL() should leave if stream not present)
	//and EDNAALR-4KTKLP (CSmsSettings::InternalizeL() causes memory leak)

	CSmsSettings* settings = CSmsSettings::NewL();
	CleanupStack::PushL(settings);

	//Test defect fix for EDNAALR-4KTKTD (CSmsSettings::RestoreL() should leave if stream not present)
	const TMsvId id = iSmsTest.CreateDummyMessageToSendL();
	Session().CleanupEntryPushL(id);
	iSmsTest.SetEntryL(id);

	iSmsTest.Printf(_L("TestSettingsRestoreDefectL Created dummy message\n"));

	TRAPD(err, RestoreSettingsL(*settings));

	iSmsTest.Printf(_L("TestSettingsRestoreDefectL RestoreL 1 with error %d\n"), err);
	iSmsTest.Printf(_L("Expected %d\n"), KErrNotFound);
	iSmsTest(err == KErrNotFound);

	//Test defect fix for EDNAALR-4KTKLP (CSmsSettings::InternalizeL() causes memory leak)
//	iSmsTest.SetEntryL(id);
	StoreSettingsL(*settings);

	iSmsTest.Printf(_L("TestSettingsRestoreDefectL StoreL 1\n"));

	settings->AddServiceCenterL(_L("Unknown"), _L("447785016005"));

	RestoreSettingsL(*settings);

	CleanupStack::PopAndDestroy(); //id
	CleanupStack::PopAndDestroy(settings);
	iSmsTest.Printf(_L("TestSettingsRestoreDefectL RestoreL 2\n"));
	}

void CSmutTest::TestSettingsStoreToCommDbL()
	{
	iSmsTest.Test().Next(_L("Storing CSmsSettings to CommDb\n"));
	iSmsTest.Printf(_L("Test Store Settings to CommDb\n"));

	CSmsSettings* settings1 = CSmsSettings::NewL();
	CleanupStack::PushL(settings1);

	CSmsSettings* settings2 = CSmsSettings::NewL();
	CleanupStack::PushL(settings2);

	iSmsTest(CompareSettings(*settings1, *settings2));

	settings1->SetCommDbAction(CSmsSettings::EStoreToCommDb);
	settings1->AddServiceCenterL(_L("Vodafone"), _L("+447785016005"));
	settings1->SetDeliveryReport(ETrue);
	settings1->SetValidityPeriod(1000);

	iSmsTest.SetEntryL(iSmsTest.iSmsServiceId);

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tNew Settings...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));

	settings1->GetServiceCenter(settings1->DefaultServiceCenter()).SetAddressL(_L("+3854354433"));

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tSC Address Changed...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));
	
	settings1->SetDeliveryReport(EFalse);

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tDelivery Report Changed...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));

	settings1->SetValidityPeriod(2000);

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tValidity Period Changed...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));

	settings1->GetServiceCenter(settings1->DefaultServiceCenter()).SetAddressL(_L("+48996655888"));
	settings1->SetDeliveryReport(ETrue);
	settings1->SetValidityPeriod(3000);

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tAll Settings Changed...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));

	settings1->AddServiceCenterL(_L("Radio"), _L("+385669988"));
	settings1->SetDefaultServiceCenter(settings1->ServiceCenterCount()-1);

	StoreSettingsL(*settings1);
	iSmsTest.Printf(_L("\tDefaultSC Changed...\n"));
	iSmsTest(CompareSettingsAndCommDbL(*settings1));

	settings1->SetCommDbAction(CSmsSettings::ENone);
	settings1->GetServiceCenter(settings1->DefaultServiceCenter()).SetAddressL(_L("+123456789"));
	settings1->SetDeliveryReport(EFalse);
	settings1->SetValidityPeriod(4000);

	iSmsTest.Printf(_L("\tTesting Setting Different from CommDb...\n"));
	iSmsTest(!CompareSettingsAndCommDbL(*settings1));

	TInt count = settings1->ServiceCenterCount();

	while (count--)
		{
		settings1->RemoveServiceCenter(count);
		}

	iSmsTest.Printf(_L("\tTesting Setting Different from CommDb...\n"));
	iSmsTest(!CompareSettingsAndCommDbL(*settings1));

	iSmsTest.Printf(_L("\tRestore settings from store...\n"));
	RestoreSettingsL(*settings2);
	iSmsTest(CompareSettingsAndCommDbL(*settings2));

//Test SMS Bearer

	iSmsTest.Printf(_L("\tTest Store SMS Bearer...\n"));

//Test 1

	settings1->SetSmsBearerAction(CSmsSettings::EStoreToCommDb);
	settings1->SetSmsBearer(CSmsSettings::ESmsBearerPacketOnly);

	StoreSettingsL(*settings1);
	iSmsTest(CompareSettingsAndSmsBearerL(*settings1));
	
//Test 2

	settings1->SetSmsBearer(CSmsSettings::ESmsBearerCircuitOnly);
	iSmsTest(!CompareSettingsAndSmsBearerL(*settings1));

//Test 3

	StoreSettingsL(*settings1);
	iSmsTest(CompareSettingsAndSmsBearerL(*settings1));

	RestoreSettingsL(*settings2);
	iSmsTest(CompareSettingsAndSmsBearerL(*settings2));

//Test 4

	settings1->SetSmsBearerAction(CSmsSettings::ENone);
	StoreSettingsL(*settings1);
	iSmsTest(CompareSettingsAndSmsBearerL(*settings1));

//Test 5

	settings1->SetSmsBearer(CSmsSettings::ESmsBearerCircuitPreferred);
	iSmsTest(!CompareSettingsAndSmsBearerL(*settings1));
	StoreSettingsL(*settings1);
	iSmsTest(!CompareSettingsAndSmsBearerL(*settings1));

	CleanupStack::PopAndDestroy(2); //settings1, settings2
	}

TBool CSmutTest::CompareSettingsAndCommDbL(const CSmsSettings& aSettings) const
	{
	TCompare compare = ETrue;

	TBuf<256> scAddress;
	CMDBSession* iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
	TRAPD(err, globalSettingsRecord.LoadL(*iDBSession));
	if(err != KErrNone)
		{
		User::Leave(KErrNotFound);				
		}
	
	CCDModemBearerRecord *modemBearerRecord = static_cast<CCDModemBearerRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdModemBearerRecord));
	CleanupStack::PushL(modemBearerRecord);
	modemBearerRecord->SetRecordId(((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iModemForPhoneServicesAndSMS); 
	modemBearerRecord->LoadL(*iDBSession);
	scAddress = modemBearerRecord->iMessageCentreNumber;

	if (aSettings.DefaultServiceCenter() < 0 || !aSettings.ServiceCenterCount())
		{
		compare(!scAddress.Length());
		}
	else
		{
		compare(scAddress == aSettings.GetServiceCenter(aSettings.DefaultServiceCenter()).Address());
		}

	TUint32 valPeriod = 0;
	valPeriod = modemBearerRecord->iMessageValidityPeriod;
	compare(valPeriod == (TUint32) aSettings.ValidityPeriod().Int());

	TBool delReport = EFalse;
	delReport = modemBearerRecord->iMessageDeliveryReport;
	compare(delReport ? aSettings.DeliveryReport() : !aSettings.DeliveryReport());
	CleanupStack::PopAndDestroy(modemBearerRecord);	
	delete iDBSession;	
	iDBSession = NULL;
	return compare;
	}

TBool CSmutTest::CompareSettingsAndSmsBearerL(const CSmsSettings& aSettings) const
	{
	TUint32 smsBearer;
	CMDBSession* iDBSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CMDBRecordSet<CCDGlobalSettingsRecord> globalSettingsRecord(KCDTIdGlobalSettingsRecord);
	TRAPD(err, globalSettingsRecord.LoadL(*iDBSession));
	if(err != KErrNone)
		{
		User::Leave(KErrNotFound);				
		}
	smsBearer = ((CCDGlobalSettingsRecord*)globalSettingsRecord.iRecords[0])->iSMSBearer;	
	delete iDBSession;	
	iDBSession = NULL;
	TCompare compare(smsBearer == (TUint32) aSettings.SmsBearer());
	return compare;
	}

void CSmutTest::StoreNumberL(const CSmsNumber& aNumber, TUid aStreamId)
	{
	CMsvStore* store = iSmsTest.EditStoreL(); // lets edit the entry
	CleanupStack::PushL(store);

	RMsvWriteStream out;
	out.AssignLC(*store, aStreamId);		// file stream id ok?
	aNumber.ExternalizeL(out);				// lets extrenalise the instance
	out.CommitL();
	store->CommitL();

	CleanupStack::PopAndDestroy(2);			// out, store
	}

void CSmutTest::RestoreNumberL(CSmsNumber& rNumber, TUid aStreamId)
	{
	CMsvStore* store = iSmsTest.ReadStoreL(); // prepare the store for reading
	CleanupStack::PushL(store);

	RMsvReadStream in;
	in.OpenLC(*store, aStreamId);		// lets open the store for 
	rNumber.InternalizeL(in);				// internalising the address/name

	CleanupStack::PopAndDestroy(2);			// in, store
	}

void CSmutTest::StoreHeaderL(const CSmsHeader& aHeader)
	{
	CMsvStore* store = iSmsTest.EditStoreL();
	CleanupStack::PushL(store);

	aHeader.StoreL(*store);
	store->CommitL();

	CleanupStack::PopAndDestroy(store);
	}

void CSmutTest::RestoreHeaderL(CSmsHeader& aHeader)
	{
	CMsvStore* store = iSmsTest.ReadStoreL();
	CleanupStack::PushL(store);

	aHeader.RestoreL(*store);

	CleanupStack::PopAndDestroy(store);
	}

void CSmutTest::StoreSettingsL(const CSmsSettings& aSettings)
	{
	
	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	smsAccount->SaveSettingsL(aSettings);
	CleanupStack::PopAndDestroy(smsAccount);
	
	}

void CSmutTest::RestoreSettingsL(CSmsSettings& aSettings)
	{

	CSmsAccount* smsAccount = CSmsAccount::NewLC();
	smsAccount->LoadSettingsL(aSettings);
	CleanupStack::PopAndDestroy(smsAccount);

	}

TBool CSmutTest::CompareServiceCenters(const CSmsServiceCenter& aLeft, const CSmsServiceCenter& aRight)
	{
	TCompare compare(aLeft.Name() == aRight.Name());
	compare(aLeft.Address() == aRight.Address());

	return compare;
	}

TBool CSmutTest::CompareNumbers(const CSmsNumber& aLeft, const CSmsNumber& aRight)
	{
	TCompare compare(aLeft.Name() == aRight.Name());
	compare(aLeft.Address() == aRight.Address());
	compare(aLeft.LogId() == aRight.LogId());

	return compare;
	}

void CSmutTest::TestNumberL()
	{
	iSmsTest.Test().Next(_L("Testing CSmsNumber"));
	iSmsTest.Printf(_L("Preparing test CSmsNumber"));

	TMsvId id=iSmsTest.CreateDummyMessageToSendL();
	Session().CleanupEntryPushL(id);
	iSmsTest.SetEntryL(id);

	CSmsNumber* number0=CSmsNumber::NewL();  
	CleanupStack::PushL(number0);

	CSmsNumber* number=CSmsNumber::NewL();  // create the instance
	CleanupStack::PushL(number);

	// Testing the features of CSmsNumber
	// The max length for a name is 256 characters
	// The max length for a number is 20 characters


// Lets take an ordinary number, test it, then store and restore it
// and see if the values are still ok.

	number->SetNameL(_L("Radiolinja1"));	// set the name
	number->SetAddressL(_L("+358508771010")); // set the address

	iSmsTest(number->Name()==_L("Radiolinja1")); // name ok?
	iSmsTest(number->Address()==_L("+358508771010")); // number ok?

	StoreNumberL(*number);
	RestoreNumberL(*number0);
	iSmsTest(CompareNumbers(*number0, *number));
	
// Lets try the max length of the telephone address, store and
// restore it and see if it works

	number->SetNameL(_L("Whatsthemaximumlengthforthis?Whatsthemaximumlengthforthis?Whatsthemaximumlengthforthis?"));
	number->SetAddressL(_L("+0123456789012345678"));
	iSmsTest(number->Name()==_L("Whatsthemaximumlengthforthis?Whatsthemaximumlengthforthis?Whatsthemaximumlengthforthis?"));
	iSmsTest(number->Address()==_L("+0123456789012345678"));

	StoreNumberL(*number);
	RestoreNumberL(*number0);
	iSmsTest(CompareNumbers(*number, *number0));

// Lets use special Scandinavian letters both in the name and
// address, store and restore and see if they are ok
	number->SetNameL(_L("ÖÄÅåöä"));
	number->SetAddressL(_L("åäöÅÄÖ"));
	iSmsTest(number->Name()==_L("ÖÄÅåöä"));
	iSmsTest(number->Address()==_L("åäöÅÄÖ"));

	StoreNumberL(*number);
	RestoreNumberL(*number0);
	iSmsTest(CompareNumbers(*number, *number0));

//  Lets try if both the number and the name can be empty,
//	store and restore it and see if they still match
	number->SetNameL(_L(""));
	number->SetAddressL(_L(""));
	iSmsTest(number->Name()==_L(""));
	iSmsTest(number->Address()==_L(""));

	StoreNumberL(*number);
	RestoreNumberL(*number0);
	iSmsTest(CompareNumbers(*number, *number0));

//  Lets try [alt+255] as both the name and number, store and	
//  restore it and see if it works
	number->SetNameL(_L(" ")); //alt+255
	number->SetAddressL(_L(" "));
	iSmsTest(number->Name()==_L(" "));
	iSmsTest(number->Address()==_L(" "));

	StoreNumberL(*number);
	RestoreNumberL(*number0);
	iSmsTest(CompareNumbers(*number, *number0));

	CleanupStack::PopAndDestroy(2); //number, number0

	//Test for defect EDNMOHN-4LDDK8
	number=CSmsNumber::NewL();  // create the instance
	CleanupStack::PushL(number);

	StoreNumberL(*number);
	RestoreNumberL(*number);

	number->SetAddressL(_L("+4477676545"));
	//End Test for defect  EDNMOHN-4LDDK8

	CleanupStack::PopAndDestroy(2); //number, id
	}

void CSmutTest::TestHeaderL()
	{

// Testing class CSmsHeader
	iSmsTest.Test().Next(_L("Testing CSmsHeader"));
	
// A CSmsHeader can be of 6 different types
// First we test constructing ESmsDeliver
	delete iSmsHeader; // deleting the previous one
	iSmsHeader = NULL;
	iSmsHeader=CSmsHeader::NewL(CSmsPDU::ESmsDeliver,*iRichText); // constructing
	iSmsTest(iSmsHeader->Type()==CSmsPDU::ESmsDeliver);	// type ok?

	CSmsDeliver& deliver = iSmsHeader->Deliver();  // lets get the CSmsPDU of the CSmsHeader
	const CSmsDeliver& deliver2 = iSmsHeader->Deliver();

	iSmsTest(deliver.Type() == CSmsPDU::ESmsDeliver); // Are the types ok?
	iSmsTest(deliver2.Type() == CSmsPDU::ESmsDeliver);

// Type ESmsCommand
	delete iSmsHeader; // deleting previous
	iSmsHeader=CSmsHeader::NewL(CSmsPDU::ESmsCommand,*iRichText);
	iSmsTest(iSmsHeader->Type()==CSmsPDU::ESmsCommand);

	CSmsCommand& command=iSmsHeader->Command();
	const CSmsCommand& command2=iSmsHeader->Command();

	iSmsTest(command.Type() == CSmsPDU::ESmsCommand);
	iSmsTest(command2.Type() == CSmsPDU::ESmsCommand);

// Type ESmsStatusReport
	delete iSmsHeader; // deleting the previous one
	iSmsHeader=CSmsHeader::NewL(CSmsPDU::ESmsStatusReport,*iRichText);
	iSmsTest(iSmsHeader->Type()==CSmsPDU::ESmsStatusReport);

	CSmsStatusReport& statusreport=iSmsHeader->StatusReport();
	const CSmsStatusReport& statusreport2=iSmsHeader->StatusReport();

	iSmsTest(statusreport.Type() == CSmsPDU::ESmsStatusReport);
	iSmsTest(statusreport2.Type() == CSmsPDU::ESmsStatusReport);

// Type ESmsSubmit
	delete iSmsHeader; // deleting the previous one
	iSmsHeader=CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*iRichText);
	iSmsTest(iSmsHeader->Type()==CSmsPDU::ESmsSubmit);

	CSmsSubmit& submit=iSmsHeader->Submit();
	const CSmsSubmit& submit2=iSmsHeader->Submit();

	iSmsTest(submit.Type() == CSmsPDU::ESmsSubmit);
	iSmsTest(submit2.Type() == CSmsPDU::ESmsSubmit);

// Lets access the CSmsMessage of our header
	CSmsMessage& message=iSmsHeader->Message();
	const CSmsMessage& message2=iSmsHeader->Message();

	iSmsTest(message.Type() == CSmsPDU::ESmsSubmit);
	iSmsTest(message2.Type() == CSmsPDU::ESmsSubmit);

// Lets test the features of CSmsHeader with different values
	iSmsHeader->SetReplyPathProvided(ETrue);
	iSmsTest(iSmsHeader->ReplyPathProvided());
	iSmsHeader->SetReplyPathProvided(EFalse);
	iSmsTest(!iSmsHeader->ReplyPathProvided());

	iSmsHeader->SetBioMsgIdType(EBioMsgIdUnknown);
	iSmsTest(iSmsHeader->BioMsgIdType()==EBioMsgIdUnknown);
	iSmsHeader->SetBioMsgIdType(EBioMsgIdIana);
	iSmsTest(iSmsHeader->BioMsgIdType()==EBioMsgIdIana);
	iSmsHeader->SetBioMsgIdType(EBioMsgIdNbs);
	iSmsTest(iSmsHeader->BioMsgIdType()==EBioMsgIdNbs);
	iSmsHeader->SetBioMsgIdType(EBioMsgIdWap);
	iSmsTest(iSmsHeader->BioMsgIdType()==EBioMsgIdWap);
	iSmsHeader->SetBioMsgIdType(EBioMsgIdWapSecure);
	iSmsTest(iSmsHeader->BioMsgIdType()==EBioMsgIdWapSecure);

// Lets set the service center number
	iSmsHeader->SetServiceCenterAddressL(_L("+350508771010"));
	iSmsTest(iSmsHeader->ServiceCenterAddress()==_L("+350508771010"));

// Lets set one recipient for the message
	CArrayPtrFlat<CSmsNumber>& recipients = iSmsHeader->Recipients();
	CSmsNumber* number=CSmsNumber::NewL();
	CleanupStack::PushL(number);
	number->SetAddressL(_L("+358503367709"));
	recipients.AppendL(number);
	CleanupStack::Pop();

// Setting settings of the message
	CSmsSettings* smsset = CSmsSettings::NewL();
	CleanupStack::PushL(smsset);
	smsset->AddServiceCenterL(_L("Hippo"), _L("+358508771010"));
	smsset->SetCharacterSet(TSmsDataCodingScheme::ESmsAlphabet7Bit);
	smsset->SetDelivery(ESmsDeliveryImmediately);
	smsset->SetDeliveryReport(EFalse);
	smsset->SetMessageConversion(ESmsConvPIDNone);
	smsset->SetRejectDuplicate(ETrue);
	smsset->SetReplyPath(EFalse);
	smsset->SetValidityPeriod(ESmsVP24Hours);
	smsset->SetValidityPeriodFormat(TSmsFirstOctet::ESmsVPFSemiOctet);
// And copying them to the CSmsHeader
	iSmsHeader->SetSmsSettingsL(*smsset);
	CleanupStack::PopAndDestroy(smsset);

	CSmsSettings* smsset2 = CSmsSettings::NewL();
	CleanupStack::PushL(smsset2);
// Lets get the same setting from the CSmsHeader
	iSmsHeader->GetSmsSettingsL(*smsset2);
// And compare that they are ok!
	iSmsTest(smsset2->CharacterSet()==TSmsDataCodingScheme::ESmsAlphabet7Bit);
	iSmsTest(!smsset2->DeliveryReport());
	iSmsTest(smsset2->MessageConversion()==ESmsConvPIDNone);
	iSmsTest(smsset2->RejectDuplicate());
	iSmsTest(!smsset2->ReplyPath());
	iSmsTest(smsset2->ValidityPeriod().Int()==ESmsVP24Hours);
	iSmsTest(smsset2->ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFSemiOctet);
	CleanupStack::PopAndDestroy(smsset2);

//Test Set/FromAddress()
	iSmsHeader->SetFromAddressL(_L("+358503367709"));
	iSmsTest(iSmsHeader->FromAddress()==_L("+358503367709"));

//Lets store and restore the CSmsHeader and test that the values are right
	TMsvId id = iSmsTest.CreateDummyMessageToSendL();
	Session().CleanupEntryPushL(id);
	iSmsTest.SetEntryL(id);

	StoreHeaderL(*iSmsHeader);

	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*iRichText);
	CleanupStack::PushL(header);

	RestoreHeaderL(*header);

//Values ok?
	iSmsTest(iSmsHeader->Type()==header->Type());
	iSmsTest(!header->ReplyPathProvided());
	iSmsTest(header->ServiceCenterAddress()==_L("+350508771010"));
	iSmsTest(iSmsHeader->Recipients().Count()==header->Recipients().Count());
	iSmsTest(iSmsHeader->BioMsgIdType()==header->BioMsgIdType());
	iSmsTest(iSmsHeader->FromAddress()==header->FromAddress());

// Lets get the setting of the message
	CSmsSettings* smsset3 = CSmsSettings::NewL();
	CleanupStack::PushL(smsset3);
	header->GetSmsSettingsL(*smsset3);

// And compare that they are the original values
	iSmsTest(smsset3->CharacterSet()==TSmsDataCodingScheme::ESmsAlphabet7Bit);
	iSmsTest(!smsset3->DeliveryReport());
	iSmsTest(smsset3->MessageConversion()==ESmsConvPIDNone);
	iSmsTest(smsset3->RejectDuplicate());
	iSmsTest(!smsset3->ReplyPath());
	iSmsTest(smsset3->ValidityPeriod().Int()==ESmsVP24Hours);
	iSmsTest(smsset3->ValidityPeriodFormat()==TSmsFirstOctet::ESmsVPFSemiOctet);

	CleanupStack::PopAndDestroy(); // smsset3
	CleanupStack::PopAndDestroy(); // header
	CleanupStack::PopAndDestroy(); // id
	}

_LIT(KGetDescriptionTestMessage, "Finland to win the ice hockey world champs");

void CSmutTest::TestSmsUtilitiesL()
	{
	iSmsTest.Test().Next(_L("Testing TSmsUtilities"));

	iRichText->Reset();
	iRichText->InsertL(0, KGetDescriptionTestMessage);
	CSmsHeader* header = CSmsHeader::NewL(CSmsPDU::ESmsSubmit,*iRichText);
	CleanupStack::PushL(header);

	TestSmsUtilities1L(*header);
	TestSmsUtilities2L(*header);
	TestSmsUtilities3L(*header);

	CleanupStack::PopAndDestroy(); //header
	}

void CSmutTest::TestSmsUtilities1L(CSmsHeader& aHeader)
	{
	iSmsTest.Printf(_L("Testing TSmsUtilities::GetDescription()"));

//Test GetDescription()

	TPtrC buf(KGetDescriptionTestMessage);
	TBuf<KSmsDescriptionLength> desc;
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	iSmsTest(desc.Length());
	iSmsTest(desc == buf.Left( Min(buf.Length(), desc.Length()) ) ); //this won't work if buf contains CEditableText::EParagraphDelimiter

//Test Special Message Indication

	CSmsPDU& pdu = aHeader.Message().SmsPDU();
	CSmsUserData& userData = pdu.UserData();
	TBuf8<2> msgInd;
	msgInd.Copy(_L("AA"));

	iSmsTest.Printf(_L("\tAdding 1st information element (Fax) to UD\n"));
	msgInd[0] = TSmsUtilities::EFaxMessageWaiting;
	msgInd[1] = 0x00000001; //1 message
	userData.AddInformationElementL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication, msgInd);
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	iSmsTest.Printf(_L("\tGetDescription() returned: %S\n"), &desc);
	iSmsTest(desc.Length());

	//Add another information element
	msgInd[0] = TSmsUtilities::EEmailMessageWaiting;
	msgInd[1] = 0x00000001; //1 message

	iSmsTest.Printf(_L("\tAdding 2nd information element (Email) to UD\n"));
	userData.AddInformationElementL(CSmsInformationElement::ESmsIEISpecialSMSMessageIndication, msgInd);
	TSmsUtilities::GetDescription(aHeader.Message(), desc);
	iSmsTest.Printf(_L("\tGetDescription() returned: %S\n"), &desc);
	iSmsTest(desc.Length());

	iSmsTest.Printf(_L("Passed\n"));
	}

_LIT(KGetDetailsResult, "\t\tNum In: \"%S\" Name Out: \"%S\"\n");

void CSmutTest::TestSmsUtilities2L(CSmsHeader& aHeader)
	{
//Test GetDetails

	iSmsTest.Printf(_L("Test TSmsUtilities::GetDetails()\n"));

	_LIT(KMikkoTel, "+358503367709");
	_LIT(KMikkoFirst, "Mikko");
	_LIT(KMikkoLast, "Rintala");
	_LIT(KMikkoEmail, "mikko@nokia.com");

//	_LIT(KAnthonyTel, " + 4 "); //is valid
	_LIT(KAnthonyTel, "+ 7 "); //is valid
	_LIT(KAnthonyFirst, "Anthony");
	_LIT(KAnthonyLast, "Alexander");
	_LIT(KAnthonyEmail, "test@symbian.com");

	_LIT(KHarriTel, "+8"); //is valid
	_LIT(KHarriFirst, "HarriHarriHarriHarriHarriHarriHarri");
	_LIT(KHarriLast, "RäisänenRäisänenRäisänenRäisänenRäisänen");
	_LIT(KHarriEmail, "harri@nokia.com");

	_LIT(KDupTel, "+44207435676");
	_LIT(KDupFirst1, "Keval");
	_LIT(KDupLast1, "Pindoria");
	_LIT(KDupFirst2, "Jackie");
	_LIT(KDupLast2, "Chan");
	
	_LIT(KBadTel1, "This is a bad Telephone");
	_LIT(KBadTel2, " 5  ");
	_LIT(KBadTel3, "5+");
	_LIT(KBadTel4, "*5565434556");

	_LIT(KBadFirst, "BadFirstName");
	_LIT(KBadLast, "BadLastName");

	_LIT(KNoNameTel, "+449005434");

	_LIT(KDummy, "+44444");

	TRAPD(err, iSmsTest.DeleteContactsFileL());
	iSmsTest(err == KErrNotFound || err == KErrNone);

	iSmsTest.Printf(_L("\tTesting without contacts\n"));

	//Simple test
	TPtrC testTel(KMikkoTel);
	TBuf<KSmsDetailsLength> details;

	aHeader.Message().SetToFromAddressL(testTel);
	TSmsUtilities::GetDetails(iSmsTest.FileSession(), aHeader.Message(), details);
	iSmsTest.Printf(KGetDetailsResult, &testTel, &details);
	iSmsTest(testTel.FindF(details) != KErrNotFound);

	TestInvalidTelOrContact(testTel, details);

	testTel.Set(KAnthonyTel);
	aHeader.Message().SetToFromAddressL(testTel);
	err = TSmsUtilities::GetDetails(iSmsTest.FileSession(), aHeader.Message(), details);
	iSmsTest.Printf(KGetDetailsResult, &testTel, &details);
	iSmsTest(testTel.FindF(details) != KErrNotFound);
	iSmsTest(err == KErrNone);

	TestInvalidTelOrContact(testTel, details);

	//Add contacts

	iSmsTest.Printf(_L("\tAdding Contacts\n"));

		//Adding good telephone numbers

		TContactItemId contId = iSmsTest.CreateContactL(KAnthonyFirst, KAnthonyLast, KAnthonyTel, KAnthonyEmail);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KMikkoFirst, KMikkoLast, KMikkoTel, KMikkoEmail);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KHarriFirst, KHarriLast, KHarriTel, KHarriEmail);
		iSmsTest(contId != KNullContactId);

		//Adding a duplicate telephone

		contId = iSmsTest.CreateContactL(KDupFirst1, KDupLast1, KDupTel, KNullDesC);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KDupFirst2, KDupLast2, KDupTel, KNullDesC);
		iSmsTest(contId != KNullContactId);

		//Adding bad telephone numbers

		contId = iSmsTest.CreateContactL(KBadFirst, KBadLast, KBadTel1, KNullDesC);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KBadFirst, KBadLast, KBadTel2, KNullDesC);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KBadFirst, KBadLast, KBadTel3, KNullDesC);
		iSmsTest(contId != KNullContactId);

		contId = iSmsTest.CreateContactL(KBadFirst, KBadLast, KBadTel4, KNullDesC);
		iSmsTest(contId != KNullContactId);

		//Adding no name contact

		contId = iSmsTest.CreateContactL(KNullDesC, KNullDesC, KNoNameTel, KNullDesC);
		iSmsTest(contId != KNullContactId);

	iSmsTest.Printf(_L("\tTesting with valid telephone numbers\n"));

		TestValidTel(KAnthonyTel, KAnthonyFirst, KAnthonyLast, KNullDesC, details);
		TestValidTel(KMikkoTel, KMikkoFirst, KMikkoLast, KNullDesC, details);
		TestValidTel(KHarriTel, _L("Räisänen"), KNullDesC, _L("Harri"), details);

	iSmsTest.Printf(_L("\tTesting with invalid telephone numbers\n"));

		TestInvalidTelOrContact(KBadTel1, details);
		TestInvalidTelOrContact(KBadTel2, details);
		TestInvalidTelOrContact(KBadTel3, details);
		TestInvalidTelOrContact(KBadTel4, details);

	iSmsTest.Printf(_L("\tTesting with duplicate telephone numbers\n"));

//		TestInvalidTelOrContact(KDupTel, details); //Fails because of CntModel defect EDNAALR-4Q2L4D

	iSmsTest.Printf(_L("\tTesting No Names telephone number\n"));

		TestInvalidTelOrContact(KNoNameTel, details);
		TestInvalidTelOrContact(KDummy, details);

	iSmsTest.Printf(_L("Passed\n"));
	}

void CSmutTest::TestValidTel(const TDesC& aTel, const TDesC& aExpect1, const TDesC& aExpect2, const TDesC& aNotExpect, TDes& rDetails)
	{
	TInt err = TSmsUtilities::GetDetails(iSmsTest.FileSession(), aTel, rDetails);
	iSmsTest.Printf(KGetDetailsResult, &aTel, &rDetails);

	if (aExpect1.Length())
		iSmsTest(rDetails.FindF(aExpect1) != KErrNotFound);

	if (aExpect2.Length())
		iSmsTest(rDetails.FindF(aExpect2) != KErrNotFound);

	if (aNotExpect.Length())
		iSmsTest(rDetails.FindF(aNotExpect) == KErrNotFound);

	iSmsTest(err == KErrNone);
	}

void CSmutTest::TestInvalidTelOrContact(const TDesC& aTel, TDes& rDetails)
	{
	TInt err = TSmsUtilities::GetDetails(iSmsTest.FileSession(), aTel, rDetails);
	iSmsTest.Printf(KGetDetailsResult, &aTel, &rDetails);
	iSmsTest(aTel.FindF(rDetails) != KErrNotFound);
	iSmsTest(err == KErrNone);
	}

void CSmutTest::TestSmsUtilities3L(CSmsHeader& aHeader)
	{
	iSmsTest.Printf(_L("Test TSmsUtilities::ServiceIdL()\n"));

	TMsvId id2 = KMsvNullIndexEntryId;
// First tested function is ServiceIdL()
	TSmsUtilities::ServiceIdL(*iSmsTest.iMsvEntry, id2);
	iSmsTest(id2 == iSmsTest.iSmsServiceId);
	iSmsTest.Printf(_L("\tPassed\n"));

	iSmsTest.Printf(_L("Test TSmsUtilities::PopulateMsgEntry()\n"));
	TMsvEntry entry;
	TSmsUtilities::PopulateMsgEntry(entry,aHeader.Message(), iSmsTest.iSmsServiceId);
//To Do: More tests
	iSmsTest.Printf(_L("\tPassed\n"));
	}
