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
// commands.h
//

#ifndef __COMMANDS_H
#define __COMMANDS_H
#include "harness.h"
#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS 
#include "tmsvbioinfo.h"
#endif
#include <commsdattypesv1_1.h>
using namespace CommsDat;

extern RFs g_rfs;


_LIT(KBioTMsgDirectory, "C:\\msgtest\\biomsg\\data\\");

const TChar KCommaSeparator = ',';
_LIT(KTrue, "true");
_LIT(KFalse, "false");
_LIT(KText, "text");
_LIT(KBool, "bool");
_LIT(KUint, "uint");

//
// CDoCmdGeneral
// Handles the running of the genral commands
class CDoCmdGeneral : public CBase, public MBaseTestState 
{
public:
	enum TCmdGeneral
	{
		ECmdGeneralClean,
		ECmdGeneralDumpMsg,
		ECmdGeneralPrettifyLog,
		ECmdGeneralComment,
		ECmdGeneralGenerateFromFile,
		ECmdGeneralGenerateFromSection,
		ECmdGeneralCheckUsedBif,
		ECmdGeneralCheckFileWithExtExists,
		ECmdGeneralCleanAllBioMsgs
	};

	static CDoCmdGeneral* NewL(CClientTestHarness& aTestUtils, const HBufC*& aBuffer, const TBIOMessageType& aMsgType, TCmdGeneral aCmd);
	static CDoCmdGeneral* NewL(CClientTestHarness& aTestHarness, const CDesCArrayFlat& aComment, TCmdGeneral aCmd);
	static CDoCmdGeneral* NewL(CClientTestHarness& aTestUtils, TCmdGeneral aCmd);
	void StartL(TRequestStatus& aStatus);
	~CDoCmdGeneral();

private:
	inline CDoCmdGeneral(CClientTestHarness& aTestHarness, TCmdGeneral aCmd) : iTestHarness(aTestHarness) , iCmd(aCmd) {}
	void ConstructL(const CDesCArrayFlat& aComment);
	void ConstructL(const HBufC*& aBuffer, const TBIOMessageType& aMsgType);
	void GenerateBIOMessageFromFileL(const TDesC& aFilename);
	void GenerateBIOMessageFromSectionL();
	void CheckDataFileCreatedL();
	void CheckBifFileUsedL(TDesC& aExpectedName);
	void DumpCurrentMessageL();
	void PrettifyLogL();
	void CleanAllBioMsgsL();
	
#ifdef SYMBIAN_BOOKMARK_DATABASE
	void CheckBookmarksDbL() const;
#endif // SYMBIAN_BOOKMARK_DATABASE

protected:
	CClientTestHarness& iTestHarness;
	HBufC* iComment;
	TCmdGeneral iCmd;
	HBufC* iBuffer;
	TBIOMessageType iMsgType;

friend class CGenericScriptParser;
};

//
// CDoCmdBioDB
// Handles the running of the BIODB commands

class CDoCmdBioDB : public CBase, public MBaseTestState
{
public:
	enum TCmdBioDB
	{
		ECmdBioDBDump,				// bio_db_dump
		ECmdBioDBTestAPI,			// bio_db_test_api
		ECmdBioDBDumpWapBif,		// bio_db_dump_wap
		ECmdBioDBCheckBioness,		// bio_db_check_bioness
		ECmdBioDBDefaultSendBearer, // bio_db_default_send_bearer
	};

	static CDoCmdBioDB* NewL(CClientTestHarness& aTestHarness, TCmdBioDB aCmd);
	void StartL(TRequestStatus& aStatus);
	~CDoCmdBioDB();

private:
	inline CDoCmdBioDB(CClientTestHarness& aTestHarness, TCmdBioDB aCmd) : iTestHarness(aTestHarness), iCmd(aCmd) {}
	void DoBioDBDumpL();
	void DoBioDBTestAPIL();
	void DoBioDBDumpWapBifsL();
	void DoBioDBCheckBionessL();
	void DoBioDBDefaultSendBearerL();
	void GetBearerText(TInt aBearer, TBuf<100>& rBearerString);

protected:
	CClientTestHarness& iTestHarness;
	TCmdBioDB iCmd;
};

//
// CDoCmdCommDB
// Handles the running of the COMMDB commands
class CDoCmdCommDB : public CBase, public MBaseTestState
{
public:
	enum TCmdCommDB
	{
		ECmdCommDBReadText,
		ECmdCommDBReadLongText,
		ECmdCommDBReadUint,
		ECmdCommDBReadBool,
		ECmdCommDBCheck
	};

	static CDoCmdCommDB* NewL(const CDesCArrayFlat& aArgs,CClientTestHarness& aTestHarness, TCmdCommDB aCmd);
	void StartL(TRequestStatus& aStatus);
	~CDoCmdCommDB();

private:
	TBool DoFindRecordExists(const TDesC& aTableNam,const TDesC& aData);
	TBool CheckBool(const TDesC& aValue);
	TInt GetIntValue(const TDesC& aValue);	

	inline CDoCmdCommDB(CClientTestHarness& aTestHarness, TCmdCommDB aCmd) : iTestHarness(aTestHarness), iCmd(aCmd) {}
	void ConstructL(const CDesCArrayFlat& aArgs);
	void DoCmdCommDBReadTextL();
	
	void DoCmdCommDBCheckL();
private:
	CDesCArrayFlat* iArgs;
	CClientTestHarness& iTestHarness;
	TCmdCommDB iCmd;
	CMDBSession* iDBSession; //commsdat session
};

//
// CDoCmdClient
// Handles the running of the Client commands
class CDoCmdClient : public CBase, public MBaseTestState
{
public:
	enum TCmdClient
	{
		ECmdClientParse,		// bio_client_parse
		ECmdClientProcess,		// bio_client_process
		ECmdClientFindBody,		// bio_client_find_in_body
		ECmdClientFindSubject	// bio_client_find_in_subject
	};

	~CDoCmdClient();
	static CDoCmdClient* NewL(CDesCArrayFlat& aArgs, CClientTestHarness& aTestHarness, const TCmdClient& aCmd);
	void StartL(TRequestStatus& aStatus);

private:
	inline CDoCmdClient(CClientTestHarness& aTestHarness, const TCmdClient& aCmd) : iTestHarness(aTestHarness), iCmd(aCmd) {}
	void DoCmdClientParseL();
	void DoCmdClientProcessL();
	void DoCmdClientFindInSubjectL();
	void DoCmdClientFindInBodyL();
	void ConstructL(const CDesCArrayFlat& aArgs);

private:
	CDesCArrayFlat* iArgs;
	CClientTestHarness& iTestHarness;
	TCmdClient iCmd;
};


//
// CDoCmdServer
// Handles the running of the Server commands
class CDoCmdServer : public CBase, public MBaseTestState
{
public:
	enum TCmdServer
	{
		ECmdServerParse,		// bio_server_parse
		ECmdServerProcess,		// bio_server_process
	};

	~CDoCmdServer();
	static CDoCmdServer* NewL(CDesCArrayFlat& aArgs, CClientTestHarness& aTestHarness, const TCmdServer& aCmd);
	void StartL(TRequestStatus& aStatus);

private:
	inline CDoCmdServer(CClientTestHarness& aTestHarness, const TCmdServer& aCmd) : iTestHarness(aTestHarness), iCmd(aCmd) {}
	void DoCmdServerParseL();
	void DoCmdServerProcessL();
	void ConstructL(const CDesCArrayFlat& aArgs);

private:
	CDesCArrayFlat* iArgs;
	CClientTestHarness& iTestHarness;
	TCmdServer iCmd;
};


//
// CDoCmdMsv
// Handles the running of the COMMDB commands
class CDoCmdMsv : public CBase, public MBaseTestState
{
public:
	enum TCmdMsv
	{
		//Pop3 Settings
		ECmdChkPop3Apop,ECmdChkPop3AutoSendOnConnect,ECmdChkPop3LoginName,
		ECmdChkPop3Password,ECmdChkPop3Port,ECmdChkPop3Restore,ECmdChkPop3ServerAddress,
		ECmdChkPop3Store,ECmdChkPop3UserAddress,ECmdChkPop3Version,ECmdChkDeleteMsgsFromServer,
		ECmdInstantDefaultPop3,ECmdInstantDefaultSmtp,ECmdInstantDefaultImap,
		ECmdInstantNamedPop3,ECmdInstantNamedImap,ECmdInstantNamedSmtp,
		//Smtp Settings
		ECmdChkSmtpEmailAlias,ECmdChkSmtpEmailAddress,ECmdChkSmtpReplyToAddress,
		ECmdChkSmtpReceiptAddress,ECmdChkSmtpBodyEncoding,
		ECmdChkSmtpDefaultMsgCharSet,ECmdChkSmtpAddVCardToEmail,
		ECmdChkSmtpAddSignatureToEmail,ECmdChkSmtpRequestReceipts,
		ECmdChkSmtpSendCopyToSelf,ECmdChkSmtpSendMessageOption,ECmdChkSmtpServerAddress,
		ECmdChkSmtpUserAddress,ECmdChkSmtpPort,
		//Imap Settings
		ECmdChkImapLoginName,ECmdChkImapPassword,ECmdChkImapFolderPath,ECmdChkImapPathSeparator,
		ECmdChkImapDisconnectedUserMode,ECmdChkImapSynchronise,ECmdChkImapSubscribe,
		ECmdChkImapAutoSendOnConnect,ECmdChkImapMaxEmailSize,ECmdChkImapDeleteEmailsWhenDisconnecting,
		ECmdChkImapAcknowledgeReceipts,ECmdChkImapGetMailOptions,ECmdChkImapInboxSynchronisationLimit,
		ECmdChkImapMailboxSynchronisationLimit,ECmdChkImapServerAddress,ECmdChkImapPort,
		//Sms Settings
		ECmdChkSmsMsgCentreNumber,ECmdDumpSmsMsgCentres,ECmdChkDefaultSmsMsgCentre
		
	};

	static CDoCmdMsv* NewL(const CDesCArrayFlat& aArgs,CClientTestHarness& aTestHarness, TCmdMsv aCmd);
	static CDoCmdMsv* NewL(const TDesC& aSettingsName,CClientTestHarness& aTestHarness, TCmdMsv aCmd);
	void StartL(TRequestStatus& aStatus);
	~CDoCmdMsv();

private:
	inline CDoCmdMsv(CClientTestHarness& aTestHarness, TCmdMsv aCmd) : iTestHarness(aTestHarness), iCmd(aCmd) {}
	inline CDoCmdMsv(const TDesC& aName,CClientTestHarness& aTestHarness, TCmdMsv aCmd) : iName(aName), iTestHarness(aTestHarness), iCmd(aCmd) {}
	void ConstructL(const CDesCArrayFlat& aArgs);

private:
	TBuf<50> iName;
	CDesCArrayFlat* iArgs;
	CClientTestHarness& iTestHarness;
	TCmdMsv iCmd;
};

#endif

