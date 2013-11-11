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
// Test code for CBioDatabase
// Author	Julia Blumin 
// Date		Aug 1999
// 
//

#include <e32test.h>
#include <e32hal.h>
#include <f32fsys.h>
#include <s32file.h>
#include <barsc.h>
#include "biotestutils.h"

#include <e32uid.h>

#include "BIODB.H"	
#include <biouids.h>	

//----------------------------------------------------------------------------------------

// Id array
const TBioMsgIdType										KId0Type=EBioMsgIdIana;
const CApaDataRecognizerType::TRecognitionConfidence	KId0Confidence=CApaDataRecognizerType::EPossible;
const TBioMsgIdText										KId0Text=_L("text/x-bio");
const TInt16											KId0Port=0;
const TUid												KId0CharacterSet={0x10003b10};
const TInt16											KId0GeneralIdData=0x0000;

const TBioMsgIdType						KId1Type=EBioMsgIdWap;
const TBioMsgIdText										KId1Text=_L("");

const TBioMsgIdText										KId2Text=_L("");

const TBioMsgIdType										KId3Type=EBioMsgIdIana;
const CApaDataRecognizerType::TRecognitionConfidence	KId3Confidence=CApaDataRecognizerType::EPossible;
const TBioMsgIdText										KId3Text=_L("");
const TInt16											KId3Port=0;
const TUid												KId3CharacterSet={0x1000aaaa};
const TInt16											KId3GeneralIdData=0;



const TUid KUidBioMsgTypeEmailNotification = {0x10005530};
const TUid KUidBioMsgTypeVCal = {0x10005533};

LOCAL_D CActiveScheduler scheduler;

// end of test data.
//----------------------------------------------------------------------------------------

#include "CMSTD.H"


//----------------------------------------------------------------------------------------
RTest gTest(_L("BIODB.dll Test Harness"));
LOCAL_D RFs				gFs;	
LOCAL_D CBioTestUtils*	testUtils;
//----------------------------------------------------------------------------------------
#define KBifDir		_L("c:\\resource\\messaging\\Bif\\")

#ifdef __WINS__
	#define KRscPath	_L("z:\\system\\data\\")
#else
	#define KRscPath	_L("c:\\system\\data\\")
#endif



//----------------------------------------------------------------------------------------
void TestScheduler::ErrorL( TInt anError ) const
//----------------------------------------------------------------------------------------
	{
	User::Leave( anError );
	}


//----------------------------------------------------------------------------------------
LOCAL_C void InitTestUtils()
//----------------------------------------------------------------------------------------
	{
	testUtils = CBioTestUtils::NewLC(gTest,ETuCleanMessageFolder);
	gTest.Start(_L("CBIODatabase"));
	gTest.Console()->ClearScreen();
	}

//----------------------------------------------------------------------------------------
LOCAL_C void CloseTestUtils()
//----------------------------------------------------------------------------------------
	{
	testUtils->TestHarnessCompleted();
	CleanupStack::PopAndDestroy(testUtils);
	//gTest.Console()->SetPos(0, 13);
	gTest.End();
	gTest.Close();
	}

//----------------------------------------------------------------------------------------
LOCAL_C void OpenFileSession() 
//----------------------------------------------------------------------------------------
	{
	gFs.Connect();
	gFs.MkDir(KBifDir);
	gFs.SetSessionPath(KBifDir);

	CActiveScheduler::Install( &scheduler );
	}


//----------------------------------------------------------------------------------------
LOCAL_C void CloseFileSession()
//----------------------------------------------------------------------------------------
	{
	gFs.Close( );
	}

//----------------------------------------------------------------------------------------
LOCAL_C void GetBearerText(TInt aBearer, TBuf<100>& rBearerString)
//----------------------------------------------------------------------------------------
{
	switch (aBearer)
	{
	case EBioMsgIdIana:
		rBearerString.Copy(_L("Iana"));
		break;
	case EBioMsgIdNbs:
		rBearerString.Copy(_L("Nbs"));
		break;
	case EBioMsgIdWap:
		rBearerString.Copy(_L("Wap"));
		break;
	case EBioMsgIdWapSecure:
		rBearerString.Copy(_L("WapSecure"));
		break;
	case EBioMsgIdUnknown:
	default:
		rBearerString.Copy(_L("Unknown"));
		break;
	}
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
LOCAL_C void DumpBifFiles(TInt aTestNumber)
//----------------------------------------------------------------------------------------
{
	HBufC* commentText = HBufC::NewLC(100);
	
	gTest.Printf(_L("Opening & Searching DB\n"));
	testUtils->TestStart( aTestNumber, _L("Opening & Searching DB"));

	CBIODatabase* bioDB = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB );
	gTest.Printf(_L("Opened DB Successfully!\n"));
	
	(commentText->Des()).Format(_L("<%D> Bif files read\n"), bioDB->BIOCount());
	testUtils->WriteComment(commentText->Des());
	gTest.Printf(commentText->Des());

	const CArrayFix<TBioMsgId>* ent = NULL;

	for (TInt i=0; i < bioDB->BIOCount(); i++)
	{
		const CBioInfoFileReader& bifReader = bioDB->BifReader(i);

		TPtrC desc;
		desc.Set(bifReader.Description());
		gTest.Printf(_L("File: %d: "), i);
		gTest.Printf(_L("Desc: %S "), &desc);
	
		(commentText->Des()).Format(_L("%D: '%S'"), i, &desc);
		testUtils->WriteComment(commentText->Des());
		
		ent = bioDB->BIOEntryLC(i);
		gTest.Printf(_L("Has %d identifiers \n"), ent->Count());
		CleanupStack::PopAndDestroy();	// ent
		
	}
	//
	gTest.Printf(_L("Looking for all Wap Ports to Watch\n"));
	testUtils->WriteComment(_L("Looking for all Wap Ports to Watch\n"));

	TPtrC desc;
	TInt pos;
	ent = bioDB->BioEntryByTypeLC(CBIODatabase::EStart, EBioMsgIdWap, pos);
	
	while(pos < bioDB->BIOCount())
	{
		desc.Set(bioDB->BifReader(pos).Description());
		gTest.Printf(_L("Desc: %S \n"), &desc);

		for (TInt i = 0; ent && i < ent->Count(); i++)
		{
			if ((*ent)[i].iType == EBioMsgIdWap)
			{
				gTest.Printf(_L("Wap Port number %D\n"), (*ent)[i].iPort);
				(commentText->Des()).Format(_L("%D: '%S' Port#:%D"), i, &desc, (*ent)[i].iPort);
				testUtils->WriteComment(commentText->Des());
			}
			
		}
		if (ent)
			CleanupStack::PopAndDestroy();	// ent
		ent = bioDB->BioEntryByTypeLC(CBIODatabase::ENext, EBioMsgIdWap, pos);
	}

	if (ent)
		CleanupStack::PopAndDestroy();	// ent
	//

	gTest.Printf(_L("Looking for all NBS Ports to Watch\n"));
	testUtils->WriteComment(_L("Looking for all NBS Ports to Watch\n"));

	ent = bioDB->BioEntryByTypeLC(CBIODatabase::EStart, EBioMsgIdNbs, pos);
	
	while(pos < bioDB->BIOCount())
	{
		desc.Set(bioDB->BifReader(pos).Description());
		gTest.Printf(_L("Desc: %S \n"), &desc);

		for (TInt i = 0; ent && i < ent->Count(); i++)
		{
			if ((*ent)[i].iType == EBioMsgIdNbs)
			{
				gTest.Printf(_L("Wap Port number %D\n"), (*ent)[i].iPort);
				(commentText->Des()).Format(_L("%D: '%S' String:%S"), i, &desc, &((*ent)[i].iText));
				testUtils->WriteComment(commentText->Des());
			}
			
		}
		if (ent)
			CleanupStack::PopAndDestroy();	// ent
		ent = bioDB->BioEntryByTypeLC(CBIODatabase::ENext, EBioMsgIdNbs, pos);
	}

	if (ent)
		CleanupStack::PopAndDestroy();	// ent

	CleanupStack::PopAndDestroy();	// bioDB
	CleanupStack::PopAndDestroy();  // commentText
	
//	
	//To improve coverage.
	
  	CBIODatabase* bioDB1 = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB1 );
	gTest.Printf(_L("Opened DB1 Successfully!\n"));
  
  	const CBioInfoFileReader& bifReader1 = bioDB1->BifReader(0);
  	
  	
  	bifReader1.MessageAppCtrlName();
  	bifReader1.IconsFilename();
  	// TRAP_IGNORE(bifReader1.ZoomLevelsLC()); // Depricated
  	bifReader1.MessageAppUid();
  //bifReader1.MessageAppCtrlUid(); //Depricated
  	bifReader1.GeneralData1();
  	bifReader1.GeneralData2();
  	bifReader1.GeneralData3();
  	bifReader1.ZoomLevelsCount();
  	bifReader1.BifEntry();
  	
	CleanupStack::PopAndDestroy();	// bioDB
	
		
	testUtils->TestFinish( aTestNumber,0 );
}

//----------------------------------------------------------------------------------------
LOCAL_C void TestAPIs(TInt aTestNumber)
//----------------------------------------------------------------------------------------
{
	CBIODatabase* bioDB = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB );

	testUtils->TestStart( aTestNumber, _L("Testing APIs"));

	testUtils->WriteComment(_L("RemoveBifL"));
	TUid msgID;
	TInt error = 0;
	while (bioDB->BIOCount())
	{
		bioDB->GetBioMsgID(0,msgID);
		TRAP(error, bioDB->RemoveBifL(msgID));
		if (error)
			testUtils->TestFinish( aTestNumber,error );
			break;
	}
	CleanupStack::PopAndDestroy();	// bioDB
	testUtils->TestFinish( aTestNumber,error );
}


//----------------------------------------------------------------------------------------
LOCAL_C void DumpWapBifFiles(TInt aTestNumber)
//----------------------------------------------------------------------------------------
{
	// gets a list of port numbers for WAP Port type
	TInt pos;

	HBufC* commentText = HBufC::NewLC(100);
	CBIODatabase* bioDB = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB );
	
	gTest.Printf(_L("Dump WAP Bif Files...\n"));
	testUtils->TestStart( aTestNumber, _L("Dump WAP Bif Files..."));

	const CArrayFix<TBioMsgId>* bioMsgIDs = bioDB->BioEntryByTypeLC(
							CBIODatabase::EStart,
							EBioMsgIdWap, pos);
	while (bioMsgIDs)
	{
		TUid msgUID;
		bioDB->GetBioMsgID(pos, msgUID);
		gTest.Printf(_L("Message %D "),msgUID );
		
		TPtrC parserName;
		parserName.Set(bioDB->GetBioParserNameL(msgUID));
		gTest.Printf(_L("Parser %S\n"),&parserName );

		TPtrC ext;
		ext.Set(bioDB->GetFileExtL(msgUID));
		gTest.Printf(_L("File Extension '%S'\n"),&ext );
	
		TPtrC desc;
		desc.Set(bioDB->BifReader(pos).Description());
		(commentText->Des()).Format(_L("<%S> BioUID:%D\tParserName:%S\tFileExt:%S"), &desc, msgUID, &parserName, &ext);
		testUtils->WriteComment(commentText->Des());

		for (TInt i = 0; i < bioMsgIDs->Count(); i++)
		{
			// Really should make a copy contructor & = operator
			gTest.Printf(_L("Type\t: %D\n"),bioMsgIDs->At(i).iType );
			gTest.Printf(_L("Confidence\t:%D\n"),bioMsgIDs->At(i).iConfidence );
			gTest.Printf(_L("IANA\t:%S\n"),&(bioMsgIDs->At(i).iText) );
			gTest.Printf(_L("Wap Port\t: %D\n"),bioMsgIDs->At(i).iPort );
			gTest.Printf(_L("CharSet\t: %D\n"),bioMsgIDs->At(i).iCharacterSet );
		
			TBuf<100> bearerString;
			GetBearerText(bioMsgIDs->At(i).iType, bearerString);
			(commentText->Des()).Format(_L("\tType:%S\tConf:%D\tIANA:%S\tPort:%D\t"),
					&bearerString,
					bioMsgIDs->At(i).iConfidence,
					&(bioMsgIDs->At(i).iText),
					bioMsgIDs->At(i).iPort);
			
			testUtils->WriteComment(commentText->Des());
		}
		CleanupStack::PopAndDestroy();	// bioMsgID
		
		bioMsgIDs = bioDB->BioEntryByTypeLC(
							CBIODatabase::ENext,
							EBioMsgIdWap, pos);
	}


	TInt portNumber = 0;
	TRAPD(leaveValue, bioDB->GetPortNumberL(KUidBioMsgTypeEmailNotification, 
							EBioMsgIdWap, 
							portNumber));
	
	gTest.Printf(_L("Email Notify is Wap Port %d \n"), portNumber);
	(commentText->Des()).Format(_L("Email Notify is Wap Port %d"),portNumber);
	testUtils->WriteComment(commentText->Des());

	TBioMsgIdText ianaString;
	TRAPD(leaveValue2, bioDB->GetIdentifierTextL(KUidBioMsgTypeEmailNotification, 
							EBioMsgIdIana, 
							ianaString));
	gTest.Printf(_L("Email Notify is IANA String %S \n"), &ianaString);
	(commentText->Des()).Format(_L("Email Notify is IANA String %d"), &ianaString);
	testUtils->WriteComment(commentText->Des());
		
	
	CleanupStack::PopAndDestroy();	// bioDB
	CleanupStack::PopAndDestroy();	// commentText
	
	testUtils->TestFinish( aTestNumber,0 );
}

//----------------------------------------------------------------------------------------
LOCAL_C void CheckBioness(TInt aTestNumber)
//----------------------------------------------------------------------------------------
{
	HBufC* commentText = HBufC::NewLC(100);
	testUtils->TestStart( aTestNumber, _L("Check Bioness..."));

	// Check if message is bio
	CBIODatabase* bioDB = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB );
	TUid bioMsgId;
	
	gTest.Printf(_L("Searching if this data type is a BIO Message\n"));
	(commentText->Des()).Format(_L("Searching if this data type is a BIO Message"));
	testUtils->WriteComment(commentText->Des());
	
	TBioMsgId bioMessageData;
	bioMessageData.iType=			KId0Type;
	bioMessageData.iConfidence=		KId0Confidence;
	bioMessageData.iText=			KId0Text;
	bioMessageData.iPort=			KId0Port;
	bioMessageData.iCharacterSet=	KId0CharacterSet;
	bioMessageData.iGeneralIdData=	KId0GeneralIdData;
	if (bioDB->IsBioMessageL(bioMessageData, bioMsgId))
	{
		gTest.Printf(_L("This is a BIO Message\n"));
	}
	else
	{
		gTest.Printf(_L("This is not a BIO Message\n"));
	}
	
	bioMessageData.iType=			KId3Type;
	bioMessageData.iConfidence=		KId3Confidence;
	bioMessageData.iText=			KId3Text;
	bioMessageData.iPort=			KId3Port;
	bioMessageData.iCharacterSet=	KId3CharacterSet;
	bioMessageData.iGeneralIdData=	KId3GeneralIdData;
	
	if (bioDB->IsBioMessageL(bioMessageData, bioMsgId))
	{
		gTest.Printf(_L("This is a BIO Message\n"));
	}
	else
	{
		gTest.Printf(_L("This is not a BIO Message\n"));
	}
	
	bioMessageData.iText= _L("text/X-vCard");
	if (bioDB->IsBioMessageL(bioMessageData, bioMsgId))
	{
		gTest.Printf(_L("This is a BIO Message\n"));
	}
	else
	{
		gTest.Printf(_L("This is not a BIO Message\n"));
	}


	bioMessageData.iType = KId1Type;
	
	bioDB->IsBioMessageL(bioMessageData, bioMsgId);
	if (bioMsgId != KNullUid)
		gTest.Printf(_L("This is a BIO Message\n"));
	else
		gTest.Printf(_L("This is not a BIO Message\n"));

	TBioMsgIdText text  = _L("//MLAP11");
	_LIT(KIsBioFormat, "%S is a BIO Message with Uid: %D\n");
	_LIT(KIsNotBioFormat, "%S is NOT a BIO Message\n");

	bioDB->IsBioMessageL(EBioMsgIdNbs, text, 0, bioMsgId);
	if (bioMsgId != KNullUid)
		gTest.Printf(KIsBioFormat, &text, bioMsgId);
	else
		gTest.Printf(KIsNotBioFormat, &text);

	text = _L("Some Bogus Text");
	bioDB->IsBioMessageL(EBioMsgIdWapSecure, text, 9, bioMsgId);
	if (bioMsgId != KNullUid)
		gTest.Printf(KIsBioFormat, &text, bioMsgId );
	else
		gTest.Printf(KIsNotBioFormat, &text);

	TBuf<KMaxBioIdText*2> testBuf;
	testBuf.Fill('a', KMaxBioIdText*2);
	bioDB->IsBioMessageL(EBioMsgIdWapSecure, testBuf, 9, bioMsgId);
	if (bioMsgId != KNullUid)
		gTest.Printf(KIsBioFormat, &text, bioMsgId );
	else
		gTest.Printf(KIsNotBioFormat, &text);

	CleanupStack::PopAndDestroy();	// bioDB
	CleanupStack::PopAndDestroy();	// commentText
	testUtils->TestFinish( aTestNumber,0 );
}

//----------------------------------------------------------------------------------------
LOCAL_C void DefaultSendBearer(TInt aTestNumber)
//----------------------------------------------------------------------------------------
{
	HBufC* commentText = HBufC::NewLC(100);
	TBuf<100> bearerString;
	testUtils->TestStart( aTestNumber, _L("DefaultSendBearer..."));

	// Check if message is bio
	CBIODatabase* bioDB = CBIODatabase::NewL(gFs);
	CleanupStack::PushL( bioDB );
	
	gTest.Printf(_L("Getting Default Send Bearer info\n"));
	
	TBioMsgId aBioMsgIdentifier;
	TRAPD(err, bioDB->GetDefaultSendBearerL(KUidBioMsgTypeEmailNotification, aBioMsgIdentifier));
	if (!err)
	{
		GetBearerText(aBioMsgIdentifier.iType, bearerString);
		(commentText->Des()).Format(_L("Default Send Bearer for EmailNotifciation: %S"),&bearerString);
		gTest.Printf(_L("Default Send Bearer for EmailNotifciation %d \n"), aBioMsgIdentifier.iType);
	}
	else if (err == KErrNotFound)
	{
		gTest.Printf(_L("Default Send Bearer for EmailNotifciation cannot be found \n"));
		(commentText->Des()).Format(_L("Default Send Bearer for EmailNotifciation cannot be found"));
	}
	else 
	{
		gTest.Printf(_L("Unexepect Error %d"), err);
		(commentText->Des()).Format(_L("Unexepect Error %d"), err);
	}

	testUtils->WriteComment(commentText->Des());

	TBioMsgIdType aPortType = EBioMsgIdWap;
	TRAP( err, bioDB->GetDefaultSendBearerTypeL(KUidBioMsgTypeVCal, aPortType));
	if (!err)
	{
		gTest.Printf(_L("Default Send Bearer for KUidBioMsgTypeVCal %d \n"), aPortType);
		GetBearerText(aPortType, bearerString);
		(commentText->Des()).Format(_L("Default Send Bearer for vCalendar %S"),&bearerString);
	}
	else if (err == KErrNotFound)
	{
		gTest.Printf(_L("Default Send Bearer for KUidBioMsgTypeVCal cannot be found\n"));
		(commentText->Des()).Format(_L("Default Send Bearer for KUidBioMsgTypeVCal cannot be found"));
	}
	else 
	{
		gTest.Printf(_L("Unexepect Error %d"), err);
		(commentText->Des()).Format(_L("Unexepect Error %d"), err);
	}
	testUtils->WriteComment(commentText->Des());
	

	TRAP(err, bioDB->GetDefaultSendBearerByTypeL(KUidBioMsgTypeEmailNotification, EBioMsgIdWapSecure, aBioMsgIdentifier));
	if (!err) 
	{
		gTest.Printf(_L(" Send Bearer for KUidBioMsgTypeVCard, WAPSecure: Port %d \n"), aBioMsgIdentifier.iPort);
		(commentText->Des()).Format(_L("Secure WAP Bearer for vCalendar %d"),aBioMsgIdentifier.iPort);
	
	}
	else if (err == KErrNotFound)
	{
		gTest.Printf(_L("Default Send Bearer for KUidBioMsgTypeVCard cannot be found \n"));
	}
	else
	{
		gTest.Printf(_L("Unexepect Error %d"), err);
		(commentText->Des()).Format(_L("Unexepect Error %d"), err);
	}
		
	testUtils->WriteComment(commentText->Des());
	
	TInt tt = 1;
	TUid uidId = TUid::Uid(10);
	bioDB->GetBioControlName(tt);
	
	CleanupStack::PopAndDestroy();	// bioDB
	CleanupStack::PopAndDestroy();	// commentText
	testUtils->TestFinish( aTestNumber,0 );
}




//----------------------------------------------------------------------------------------
LOCAL_C void doMainL()
//----------------------------------------------------------------------------------------
	{
	TInt testNumber = 1;

	OpenFileSession();
	InitTestUtils();

	__UHEAP_MARK;
	DumpBifFiles(testNumber++);
	//gTest.Printf(_L("Hit a key to continue..."));
	//gTest.Getch();

	TestAPIs(testNumber++);
	//gTest.Printf(_L("Hit a key to continue..."));
	//gTest.Getch();

	DumpWapBifFiles(testNumber++);
	//gTest.Printf(_L("Hit a key to continue..."));
	//gTest.Getch();
	
	CheckBioness(testNumber++);
	//gTest.Printf(_L("Hit a key to continue..."));
	//gTest.Getch();

	DefaultSendBearer(testNumber++);
	//gTest.Printf(_L("Hit a key to continue..."));
	//gTest.Getch();

	__UHEAP_MARKEND;
	CloseFileSession();
	CloseTestUtils();
	}



GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup*	theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);
	delete theCleanup;	
	__UHEAP_MARKEND;
	return(KErrNone);
	}
