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

#ifndef EMAIL_TEST_H_
#define EMAIL_TEST_H_

#include "msvtestutils.h"
#include <e32test.h>
#include <e32uid.h>
#include <msvruids.h>
#include <mtsr.h>
#include <msvapi.h>
#include <mtclreg.h> // CClientMtmRegistry
#include <miutset.h>
#include <smtpset.h>
#include <pop3set.h>
#include <imapset.h>
#include <imcvrecv.h>
#include <smtcmtm.h>
#include <popcmtm.h>
#include <impcmtm.h>

#define KMailtestPath			_L("c:\\mailtest\\")
#define KRfc822Dir				_L("c:\\mailtest\\rfc822\\")
#define KEmailDataDir			_L("c:\\logs\\Email\\")

#define KImcmResourceLocation	_L("z:\\system\\data\\imcm.r01")
#define pop3DatFile				_L("c:\\system\\mtm\\pops.dat")
#define smtpDatFile				_L("c:\\system\\mtm\\smts.dat")
#define imap4DatFile			_L("c:\\system\\mtm\\imps.dat")

// used for writing to logs
#define KFileNameHeaders		_L("Entry_822Headers.txt")
#define KFileNameMimeHeaders	_L("Entry_MimeHeaders.txt")

#define KPeriod					10000	// period of timer

const TUid KUidMsvTechnologyGroupEmail	= {0x10001671}; 
_LIT(KPop3ServerMtmFileName, "z:\\system\\libs\\pops.dll");
_LIT(KSmtpServerMtmFileName, "z:\\system\\libs\\smts.dll");
_LIT(KImap4ServerMtmFileName, "z:\\system\\libs\\imps.dll");
_LIT(KPop3ClientMtmFileName, "z:\\system\\libs\\imcm.dll");
_LIT(KSmtpClientMtmFileName, "z:\\system\\libs\\imcm.dll");
_LIT(KImap4ClientMtmFileName, "z:\\system\\libs\\imcm.dll");

// The following are dummy ui mtm filenames - they are never instantiated.
_LIT(KPop3DataLayerFileName, "z:\\system\\libs\\dummypop3uimtm.dll");
_LIT(KSmtpDataLayerFileName, "z:\\system\\libs\\dummysmtpuimtm.dll");
_LIT(KImap4DataLayerFileName, "z:\\system\\libs\\dummyimap4uimtm.dll");

enum TEmailTestUtilsPanic
	{
	KErrNotOnClientSide
	};

class CEmailTestUtils : public CMsvTestUtils
	{
public:
	IMPORT_C static CEmailTestUtils* NewL(RTest& aRTest, TUint aCreationFlags = ETuNone);
	IMPORT_C static CEmailTestUtils* NewLC(RTest& aRTest, TUint aCreationFlags = ETuNone);

	IMPORT_C void CreateAllTestDirectories();

	IMPORT_C void InstallSmtpMtmGroupL();
	IMPORT_C void InstallPopMtmGroupL();
	IMPORT_C void InstallImapMtmGroupL();

	IMPORT_C void InstantiateImapClientMtmL();
	IMPORT_C void InstantiatePopClientMtmL();
	IMPORT_C void InstantiateSmtpClientMtmL();

	IMPORT_C void InstantiateImapServerMtmL();
	IMPORT_C void InstantiatePopServerMtmL();
	IMPORT_C void InstantiateSmtpServerMtmL();

	IMPORT_C void CreateMessageFilesL(TMsvId aFinalServiceId, TMsvId aCreateInFolderId);
	IMPORT_C void CreateMessageFilesL(TMsvId aFinalServiceId, TMsvId aCreateInFolderId,const TDesC& aPath);
	IMPORT_C void CreateMessageL(const TDesC& filename, TMsvId aFinalServiceId, TMsvId aCreateInFolderId);

	IMPORT_C void CreateImapServerMtmRegL();
	IMPORT_C void CreateSmtpServerMtmRegL();
	IMPORT_C void CreatePopServerMtmRegL();

	IMPORT_C TMsvId CreateImapServiceL();
	IMPORT_C TMsvId CreateSmtpServiceL();
	IMPORT_C TMsvId CreatePopServiceL();

	IMPORT_C void DeleteImapServiceL();
	IMPORT_C void DeleteSmtpServiceL();
	IMPORT_C void DeletePopServiceL();
	IMPORT_C void DeleteScriptFilesFromLogsDirL();

	IMPORT_C TMsvId FindImapServiceL();
	IMPORT_C TMsvId FindSmtpServiceL();
	IMPORT_C TMsvId FindPopServiceL();

	IMPORT_C void CopyScriptFileL(const TDesC& aSourceFile, const TDesC& aDestDirectory, const TDesC& aPortNumber);
	IMPORT_C void CopyScriptFileL(const TDesC& aSourceFile, const TDesC& aPortNumber);

	IMPORT_C void StartConnectionL(TUint32 aIAP);

	IMPORT_C void Reset();
	IMPORT_C void ClearEmailAccountsL();
	IMPORT_C const TDesC8& MachineName();
	IMPORT_C ~CEmailTestUtils();

	//From CMsvTestUtils
	IMPORT_C void InstantiateClientMtmsL();
	IMPORT_C void InstantiateServerMtmsL();
	IMPORT_C void DeleteServicesL();
	IMPORT_C void CreateServicesL();
	IMPORT_C void FindExistingServicesL();
	IMPORT_C void InstallMtmGroupsL();
	IMPORT_C void CreateServerMtmRegsL();
	IMPORT_C void ListChildrenL(RFile& aFile, CDir& rfcFileList, TInt& aCount, TInt& aEntryCount, TBool aReplace = ETrue, TBool aOtherFiles = ETrue);
	void SetFolderType(TMsvEntry& aMsvEntry, TPtrC& type);

private:
	CEmailTestUtils(RTest& aRTest);
	void DisplayLogEventData(const CLogEvent& /*aEvent*/, TBool /*aToFile*/, RFile& /*aFile*/) {};
	void ConstructL(TUint aCreationFlags);
	void DeleteCurrentMtm();

	void DisplayMimeHeaderL(TMsvId aId, CMsvStore& fileStore, const TFileName& aFilepath, TBool aReplace);
	void DisplayHeaderL(TMsvId aId, CMsvStore& fileStore, const TFileName& aFilepath, TBool aReplace);
	void PrintRecipients(CDesC8Array& list, const TDesC& label, RFile file);
	void PrintRecipientsL(CDesC16Array& list, const TDesC& label, RFile file);
	void Panic(TInt aPanic);
	void Panic(TEmailTestUtilsPanic aPanic);

	void ReadDataL(CImRecvConvert* recvConvert, TFileName fileName);

public:
	CImap4ClientMtm*		iImapClientMtm;
	CPop3ClientMtm*			iPopClientMtm;
	CSmtpClientMtm*			iSmtpClientMtm;

	CBaseServerMtm*			iImapServerMtm;
	CBaseServerMtm*			iPopServerMtm;
	CBaseServerMtm*			iSmtpServerMtm;

	TMsvId					iImapServiceId;
	TMsvId					iSmtpServiceId;
	TMsvId					iPopServiceId;
	
private:
	HBufC8*                 iMachineName;
	};

#define EmailTEST_PANIC _L("Email_Test")

#endif
