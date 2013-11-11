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

#ifndef BIO_TEST_H_
#define BIO_TEST_H_

#include "msvtestutils.h"

#include <biocmtm.h>
#include "biosmtm.h"
#include <bitsids.h>

#include <bsp.h>

const TUid KUidMsgTypeSmartMessage  = {0x10001262};
_LIT(KBioServerMtmFileName, "z:\\system\\libs\\bios.dll");
_LIT(KBioClientMtmFileName, "z:\\system\\libs\\bioc.dll");
_LIT(KBioUiMtmFileName, "z:\\system\\libs\\bium.dll");

const TInt KTelephoneNumberMaxLength	= 32;

#define KDataComponentFileName _L("z:\\system\\mtm\\cbiomtmu.dat")


_LIT(KBIOTxtFilePath, "C:\\test\\bio\\tbiogen\\");  

// Wildcard Format for text filenames used to create messages
_LIT(KBIOIapPrefix, "iacp*"); //IACP messages-mail&internet settings, 
								//Inet scripts and additions,& ?? 
_LIT(KBIOEnpPrefix, "enp*"); // Smart Message email notification
_LIT(KBIORingTonePrefix, "rtone*"); // ringing tones message
_LIT(KBIOOpLogoPrefix, "oplogo*"); // operator logo message
_LIT(KBIOcBusinessCardPrefix, "cbc*");	// compact business card
_LIT(KBIOvCardPrefix, "vcard*"); // vCard message
_LIT(KBIOvCalenderPrefix, "vcal*"); // vCalender
_LIT(KBIOWappPrefix, "wapp*"); //Wapp message


enum TBioTestUtilsPanic
	{
	KErrBIONotOnClientSide,
	KErrBIOSettingsNotConstructed
	};

class CBIODatabase;
class CBioTestUtils : public CMsvTestUtils
	{
public:
	IMPORT_C static CBioTestUtils* NewL(RTest& aRTest);
	IMPORT_C static CBioTestUtils* NewLC(RTest& aRTest);
	// additional overloaded constructors
	IMPORT_C static CBioTestUtils* NewL(RTest& aRTest, TInt aCreationFlags);
	IMPORT_C static CBioTestUtils* NewLC(RTest& aRTest, TInt aCreationFlags);
	IMPORT_C void CreateMessageFilesL(TMsvId aFinalServiceId, TMsvId aCreateInFolderId);

	IMPORT_C void Reset();
	IMPORT_C ~CBioTestUtils();

	IMPORT_C void CreateBioServerMtmRegL();

	inline void SetProgressL(CMsvOperation& iOperation);
	IMPORT_C void EmptyInboxMessagesL();
	//Set the BIO service.  Creates a default service if one not available. Returns value of new entry
	IMPORT_C TMsvId SetBIOServiceIdL();
	// Inline function which returns the id of message services
	inline TMsvId BIOServiceId();
	// Inline function - sets  Bio to value passed as a parameter
	inline void SetBIOServiceIdL(TMsvId aServiceId);
	// Set the Session path for the file session
	IMPORT_C void SetSessionPath(const TDesC& aSessionpath);
	// Creates a BIO entry of the Message Type, from the descriptor passed as a parameter 
	IMPORT_C TMsvId CreateBIOEntryL(TDesC& aText, TBIOMessageType aMessageType);
	// Reads text from file, and creates a BIO message entry
	IMPORT_C TMsvId CreateBIOEntryFromFileL(const TDesC& aFilename, TBIOMessageType aMessageType);
	IMPORT_C void LogCurrentMessageTypeL(TMsvId aMsvId);

	IMPORT_C CMsvEntrySelection*	GenerateMessagesL();
	IMPORT_C CMsvEntrySelection*	GenerateMessagesL(const TDesC& aFileDirectory, TBool aLogCreationDetails = EFalse);

	// create  a parser
	IMPORT_C CBaseScriptParser2*	CreateParserTypeL(TInt32 aBioMsgType);
	IMPORT_C CBaseScriptParser2*	CreateParserL(TMsvId aBioMsgId);

	// return message body contents
	IMPORT_C TPtrC					MessageBodyL(TMsvId aBioMsg);

	// log extract parsed fields
	IMPORT_C void					LogExtractedFieldsL(TMsvId aMessage);
	IMPORT_C CArrayPtrSeg<CParsedField>&			ParsedFieldArray();
	
	IMPORT_C TBIOMessageType SetMessageType(const TDesC& aFileName);
	IMPORT_C static void DoAppendVariantName(TDes& aFileName);



	void InstantiateClientMtmsL();
	void InstantiateServerMtmsL();
	void DeleteServicesL();
	void CreateServicesL();
	void FindExistingServicesL();
	void InstallMtmGroupsL();
	void CreateServerMtmRegsL();
private:
	CBioTestUtils(RTest& aRTest);
	void DisplayLogEventData(const CLogEvent& /*aEvent*/, TBool /*aToFile*/, RFile& /*aFile*/) {};
	void ConstructL();
	void ConstructL(TInt aCreationFlags);
	void Panic(TInt aPanic);
	void Panic (TBioTestUtilsPanic aPanic);

	HBufC* ReadFromFileL(const TDesC& aFile);
	HBufC* CreateDummyPhoneNumberL();

	void SetForMtmTypeL(TMsvEntry& aEntry, TBIOMessageType aType);

	TMsvId DefaultServiceForMTML(TUid aMtmUid, TBool aFindFirstServiceIfNoDefault);
	TMsvId DefaultServiceServerL(TUid aMtmUid, TBool aFindFirstServiceIfNoDefault);
	TMsvId DefaultServiceClientL(TUid aMtmUid, TBool aFindFirstServiceIfNoDefault);

	CMsvEntrySelection* GetListOfAccountsWithMTMServerL(TUid aMtmUid);
	CMsvEntrySelection* GetListOfAccountsWithMTMClientL(TUid aMtmUid);
	TMsvId CreateDefaultBIOServiceL();

	void CreateBioEntryServerSideL(TMsvEntry& aEntry, CRichText& aBody);
	void CreateBioEntryClientSideL(TMsvEntry& aEntry, CRichText& aBody);
	void DeleteLocalEntrySynchronouslyL(CMsvEntry& aParentEntry, TMsvId anId);

	void SetBioServiceServerSideL();
	void SetBioServiceClientSideL();

	void InternalizeL(RMsvReadStream& aReadStream);

public:
	CBIOClientMtm*			iBioClientMtm;
	CBIOServerMtm*			iBioServerMtm;
	TMsvId					iBioServiceId;
	CBIODatabase*			iBioDb;

	TInt					iNumFiles;
	TInt					iFilesProcessed;
	TInt					iMessagesCreated;

	CBaseScriptParser*		iParser;
	CRegisteredParserDll*	iRegisteredParserDll;
	CDir*					iDir;
	HBufC*					iMessageBody;
	CArrayPtrSeg<CParsedField>*	iTestParsedFieldArray;
	};

#define BIOTEST_PANIC _L("Bio_Test")

#include <biotestutils.inl>

#endif
