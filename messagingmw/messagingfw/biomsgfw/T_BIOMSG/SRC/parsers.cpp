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

#include "parsers.h"
#include "commands.h"
#include "biotestutils.h"

//
// The [main] section
_LIT(KMainSectionName, "main");

//
// The bio_client_test command in the [main] section
_LIT(KBioClientCmd,						"bio_test");

//
// The commands in the [client] section (general)
_LIT(KCommandCleanMessageFolder,		"clean_message_folder");        // 0 parameters, deletes message store
_LIT(KCommandPrettifyLog,				"prettify_log");				// 0 parameters, deletes message store
_LIT(KCommandDumpParsedMsg,				"dump_parsed_message");         // 0 parameters, deletes message store
_LIT(KCommandAddComment,				"add_comment");					// n parameters, adds a comment to the log, concatenates parameters
_LIT(KCommandGenerateMsgFromFile,		"gen_msg_from_file");			// 1 parameter - FileName, create a BIO msg from a file, 
_LIT(KCommandGenerateMsgFromSection,	"gen_msg_from_section");		// 1 parameter - FileName, create a BIO msg from a file, 
_LIT(KCommandCheckUsedBif,				"check_used_bif");				// 1 parameter - the expected bif name, 
_LIT(KCommandChkFileInMsgDirWithExt,    "chk_file_in_msg_dir_with_ext");// 0 parameters
_LIT(KCommandCleanAllBioMsgs,			"clean_all_bio_msgs");			// 0 parameters, deletes all the BIO msgs in the message store

//
// The commands in the [client] section (TestBIODB)
_LIT(KCommandDumpBif,					"bio_db_dump");					// 0 parameters, dumps biffs
_LIT(KCommandBifAPITest,				"bio_db_api_test");				// 0 parameters, tests bif API
_LIT(KCommandDumpWapBif,				"bio_db_dump_wap");
//_LIT(KCommandCheckBioNess,				"bio_db_check_bioness");
_LIT(KCommandDefaultSendBearer,			"bio_db_default_send_bearer");


//
// The commands in the [client] section (COMMDB)
_LIT(KCommandClientCommDB,				"bio_commdb");					// 2 parameters, table name and section name
//
// The commands in the [client] section (MSV)
_LIT(KCommandPop3SettingsCheck,						"check_pop3_defs");			//1 parameter - a section name.
_LIT(KCommandPop3NamedSettingsCheck,				"check_pop3_named");	    //2 parameters - a section name and a SettingsName
_LIT(KCommandSmtpSettingsCheck,						"check_smtp_defs");			//1 parameter - a section name.
_LIT(KCommandSmtpNamedSettingsCheck,				"check_smtp_named");	    //2 parameters - a section name and a SettingsName
_LIT(KCommandImapSettingsCheck,						"check_imap_defs");			//1 parameter - a section name.
_LIT(KCommandImapNamedSettingsCheck,				"check_imap_named");	    //2 parameters - a section name and a SettingsName
_LIT(KCommandSmsSettingsCheck,						"check_sms");				//1 parameter - a section name.

//
// The commands in the [client] section (TestClientSide)
_LIT(KCommandClientParse,							"bio_client_parse");			// 1 optional parameter.
_LIT(KCommandClientProcess,							"bio_client_process");
_LIT(KCommandClientFindBody,						"bio_client_find_in_body");		// 1 parameter, finds text in message body
_LIT(KCommandClientFindSubject,						"bio_client_find_in_subject");	// 1 parameter, finds text in message subject

//
// The commands in the [client] section (TestServerSide)
_LIT(KCommandServerParse,							"bio_server_parse");
_LIT(KCommandServerProcess,							"bio_server_process");


//
// The commands in the [check_sms_defs] section 

_LIT(KCommandSmsChkMessageCentreNumber,				"chk_messagecentrenumber");		//2 parameters - the message centre name and expectred number
_LIT(KCommandSmsChkDefaultMessageCentreNumber,		"chk_defaultmsgcentrenumber");  //1 parameters - the message centre name and expectred number
_LIT(KCommandSmsDumpMessageCentres,					"dump_messagecentres");			//0 parameters


//
// The commands in the [check_pop3_defs] section 
// NB: Those commented out are not in _current_ BIO spec.
_LIT(KCommandPop3ChkLoginName,						"chk_pop3_loginname");				//1 parameter - the expected LogonName
_LIT(KCommandPop3ChkPassword,						"chk_pop3_password");				//1 parameter - the expected Password
_LIT(KCommandPop3ChkServerAddress,					"chk_pop3_serveraddress");			//1 parameter - the expected server address
//_LIT(KCommandPop3ChkAPop,							"chk_pop3_apop");					//1 parameter - true or false
//_LIT(KCommandPop3ChkAutoSendOnConnect,			"chk_pop3_autosendonconnect");		//1 parameter - true or false
//_LIT(KCommandPop3ChkPort,							"chk_pop3_port");					//1 parameter - a number
_LIT(KCommandPop3ChkUserAddress,					"chk_pop3_useraddress");				//1 parameter - the expected user@address
//_LIT(KCommandPop3ChkVersion,						"chk_pop3_version");					//1 parameter - a number

//
// The commands in the [check_smtp_defs] section 
// NB: Those commented out are not in _current_ BIO spec.
//_LIT(KCommandSmtpEmailAlias,						"chk_smtp_emailalias");			//1 parameter - the expected value
_LIT(KCommandSmtpEmailAddress,						"chk_smtp_emailaddress");		//1 parameter - the expected value
//_LIT(KCommandSmtpReplyToAddress,					"chk_smtp_replytoaddress");		//1 parameter - the expected value
//_LIT(KCommandSmtpReceiptAddress,					"chk_smtp_receiptaddress");		//1 parameter - the expected value
//_LIT(KCommandSmtpBodyEncoding,					"chk_smtp_bodyencoding");		//1 parameter - the expected value
//_LIT(KCommandSmtpDefaultMsgCharSet,				"chk_smtp_defaultmsgcharset");	//1 parameter - the expected value
//_LIT(KCommandSmtpAddVCardToEmail,					"chk_smtp_addvcardtoemail");	//1 parameter - the expected value
//_LIT(KCommandSmtpAddSignatureToEmail,				"chk_smtp_addsignaturetoemail");//1 parameter - the expected value
//_LIT(KCommandSmtpRequestReceipts,					"chk_smtp_requestreceipts");	//1 parameter - the expected value
//_LIT(KCommandSmtpSendCopyToSelf,					"chk_smtp_sendcopytoself");		//1 parameter - the expected value
//_LIT(KCommandSmtpSendMessageOption,				"chk_smtp_sendmessageoption");	//1 parameter - the expected value
_LIT(KCommandSmtpServerAddress,						"chk_smtp_serveraddress");		//1 parameter - the expected value
//_LIT(KCommandSmtpUserAddress,						"chk_smtp_useraddress");		//1 parameter - the expected value


//
// The commands in the [check_imap_defs] section 
// NB: Those commented out are not in _current_ BIO spec.
_LIT(KCommandImapServerAddress,						"chk_imap_serveraddress");		//1 parameter - the expected value
//_LIT(KCommandImapPort,							"chk_imap_port");				//1 parameter - the expected value
_LIT(KCommandImapLoginName,							"chk_imap_loginname");			//1 parameter - the expected value
_LIT(KCommandImapPassword,							"chk_imap_password");			//1 parameter - the expected value
_LIT(KCommandImapFolderPath,						"chk_imap_folderpath");			//1 parameter - the expected value
//_LIT(KCommandImapPathSeparator,					"chk_imap_pathseparator");		//1 parameter - the expected value
//_LIT(KCommandImapDisconnectedUserMode,			"chk_imap_disconnectedusermode");//1 parameter - the expected value
//_LIT(KCommandImapSynchronise,						"chk_imap_synchronise");		//1 parameter - the expected value
//_LIT(KCommandImapSubscribe,						"chk_imap_subscribe");			//1 parameter - the expected value
//_LIT(KCommandImapAutoSendOnConnect,				"chk_imap_autosendonconnect");	//1 parameter - the expected value
//_LIT(KCommandImapMaxEmailSize,					"chk_imap_maxemailsize");		//1 parameter - the expected value
//_LIT(KCommandImapDeleteEmailsWhenDisconnecting,	"chk_imap_deleteemailswhendisconnecting");	//1 parameter - the expected value
//_LIT(KCommandImapAcknowledgeReceipts,				"chk_imap_acknowledgereceipts");//1 parameter - the expected value
//_LIT(KCommandImapGetMailOptions,					"chk_imap_getmailoptions");		//1 parameter - the expected value
//_LIT(KCommandImapInboxSynchronisationLimit,		"chk_imap_inboxsynchronisationlimit");//1 parameter - the expected value

//
// The commands in the [bio_commdb] section 
_LIT(KCommandCommDbReadText,			"commdb_read_text");		// 2 parameters, reads a setting from CommDB
_LIT(KCommandCommDbReadLongText,		"commdb_read_longtext");	// 2 parameters, reads a setting from CommDB
_LIT(KCommandCommDbReadUint,			"commdb_read_uint");		// 2 parameters, reads a setting from CommDB
_LIT(KCommandCommDbReadBool,			"commdb_read_bool");		// 2 parameters, reads a setting from CommDB
_LIT(KCommandCommDbCheck,				"commdb_check");		    // 3 parameters, checks that a setting in CommDB has the specified value

//
// The commands in the [gen_msg_from_section] section 
_LIT(KCommandGenMsgAddLine,				"msg_add_line");					// 1 parameters, the text line to add - MUST BE "'ed!
_LIT(KCommandGenMsgType,				"msg_set_type");					// 1 parameters, the message type - MUST BE "'ed!


//
// CBioMainSectionParser
// Parses the [main] section of the script
CBioMainSectionParser* CBioMainSectionParser::NewL(CMainTestHarness& aMainTest, CTestScript& aScript)
{
	CBioMainSectionParser* self = new (ELeave) CBioMainSectionParser(aMainTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioMainSectionParser::LogComment(const TDesC& aComment)
{
	iMainTest.TestUtils().WriteComment(aComment);
}

void CBioMainSectionParser::ConstructL()
{
	CBaseSectionParser::ConstructL(KMainSectionName);

	//
	// Add the command parser to parse the bio_client_test command
	iCommandParsers->AppendL(CBioMainCommandsParser::NewL(iScript, iMainTest));
}

//
// CBioMainCommandsParser
// Handles the commands that appear in the [main] section
CBioMainCommandsParser* CBioMainCommandsParser::NewL(CTestScript& aScript, CMainTestHarness& aMainTest)
{
	CBioMainCommandsParser* self = new (ELeave) CBioMainCommandsParser(aScript, aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioMainCommandsParser::ProcessL()
{
	//
	// Check there are 1 argument
	CheckNumberOfArgumentsL(1);

	//
	// Create the client test harness that will actually do the work
	// It shares the same TestUtils instance to update the same log file
	CClientTestHarness* clientTestH = CClientTestHarness::NewL(iMainTest.TestUtils(), iMainTest.TestConsole());
	CleanupStack::PushL(clientTestH);

	//
	// Attach it to the main state machine, this transfers ownership!
	iMainTest.AddStateL(clientTestH, iDebugInfo);
	CleanupStack::Pop(clientTestH);

	//
	// Create the client secion parser
	// Note we pass in the client test harness not the main one!
	CBioClientSectionParser* sectionP = CBioClientSectionParser::NewL(*clientTestH, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionP);
	sectionP->ParseL();
	CleanupStack::PopAndDestroy(sectionP);
	
}

void CBioMainCommandsParser::ConstructL()
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands we can handle (bio_client_test)
	AddCommandL(KBioClientCmd);
}

//
// CBioClientSectionParser
// Handles the [client] section of the script
CBioClientSectionParser* CBioClientSectionParser::NewL(CClientTestHarness& aTestH, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioClientSectionParser* self = new (ELeave) CBioClientSectionParser(aTestH, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioClientSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioClientSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iCommandParsers->AppendL(CBioClientCommandsParser::NewL(iClientTest));
}

//
// CBioClientCommandsParser
// Handles the bio specific commands in the
// [client] section of the script
CBioClientCommandsParser* CBioClientCommandsParser::NewL(CClientTestHarness& aClientTest)
{
	CBioClientCommandsParser* self = new (ELeave) CBioClientCommandsParser(aClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioClientCommandsParser::ProcessL()
{
	//
	// General
	if((*iCurrentCommand) == KCommandCleanMessageFolder)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, CDoCmdGeneral::ECmdGeneralClean), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandPrettifyLog)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, CDoCmdGeneral::ECmdGeneralPrettifyLog), iDebugInfo);
	}
	else  
	if((*iCurrentCommand) == KCommandDumpParsedMsg)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, CDoCmdGeneral::ECmdGeneralDumpMsg), iDebugInfo);
	}
	else 
	if((*iCurrentCommand) == KCommandAddComment)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, *iArgumentList, CDoCmdGeneral::ECmdGeneralComment) ,iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandGenerateMsgFromFile)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, *iArgumentList, CDoCmdGeneral::ECmdGeneralGenerateFromFile) ,iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandGenerateMsgFromSection)
	{
		//
		// Create a new section parser for the msg
		CheckNumberOfArgumentsL(1);
		CBioGenMsgSectionParser* section = CBioGenMsgSectionParser::NewL(iClientTest, *(iDebugInfo.ScriptFile()), (*iArgumentList)[0]);
		CleanupStack::PushL(section);
		section->ParseL();
		section->SectionFinishedL();
		CleanupStack::PopAndDestroy(section);
	}
	else
	if((*iCurrentCommand) == KCommandCheckUsedBif)
	{
		CheckNumberOfArgumentsL(1);
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, *iArgumentList, CDoCmdGeneral::ECmdGeneralCheckUsedBif) ,iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandChkFileInMsgDirWithExt)
	{
		//CheckNumberOfArgumentsL(1);
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, CDoCmdGeneral::ECmdGeneralCheckFileWithExtExists) ,iDebugInfo);
	}
	else
	if ((*iCurrentCommand) == KCommandCleanAllBioMsgs)
	{
		iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, CDoCmdGeneral::ECmdGeneralCleanAllBioMsgs) ,iDebugInfo);
	}
	else

	//
	// BIO DB 
	if((*iCurrentCommand) == KCommandDumpBif)
	{
		iClientTest.AddStateL(CDoCmdBioDB::NewL(iClientTest, CDoCmdBioDB::ECmdBioDBDump), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandBifAPITest)
	{
		iClientTest.AddStateL(CDoCmdBioDB::NewL(iClientTest, CDoCmdBioDB::ECmdBioDBTestAPI), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandDumpWapBif)
	{
		iClientTest.AddStateL(CDoCmdBioDB::NewL(iClientTest, CDoCmdBioDB::ECmdBioDBDumpWapBif), iDebugInfo);
	}
	else
	/*if((*iCurrentCommand) == KCommandCheckBioNess)
	{
		iClientTest.AddStateL(CDoCmdBioDB::NewL(iClientTest, CDoCmdBioDB::ECmdBioDBCheckBioness), iDebugInfo);
	}
	else*/
	if((*iCurrentCommand) == KCommandDefaultSendBearer)
	{
		iClientTest.AddStateL(CDoCmdBioDB::NewL(iClientTest, CDoCmdBioDB::ECmdBioDBDefaultSendBearer), iDebugInfo);
	}
	else

	//
	// CommDB
	if((*iCurrentCommand) == KCommandClientCommDB)
	{
		//
		// Check there are 2 arguments (TableName, SectionName)
		CheckNumberOfArgumentsL(2);

		//
		// Create the COMMDB section parser
		// Note we pass in the client test harness not the main one!
		CBioCommDBSectionParser* sectionP = CBioCommDBSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0], (*iArgumentList)[1]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();

		//
		// Note how we pass our own debug info to the section.  This insures if this
		// test fails, the line we are on now is said to have failed - rather than
		// one of the lines in the section.  This is because the operations in a COMMDB
		// section are not individual commands - they simply build up an SQL statement
		// I.E they cant individually fail - the whole section failes instead
		sectionP->SectionFinishedL(iDebugInfo);
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	
	//
	// Msv
	if((*iCurrentCommand) == KCommandSmsSettingsCheck)
	{
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(1);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioSmsSettingsSectionParser* sectionP = CBioSmsSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandPop3SettingsCheck)
	{
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(1);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioPop3SettingsSectionParser* sectionP = CBioPop3SettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandPop3NamedSettingsCheck)
	{
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(2);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioPop3NamedSettingsSectionParser* sectionP = CBioPop3NamedSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0],(*iArgumentList)[1]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandSmtpNamedSettingsCheck)
	{
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(2);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioSmtpNamedSettingsSectionParser* sectionP = CBioSmtpNamedSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0],(*iArgumentList)[1]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandImapNamedSettingsCheck)
	{
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(2);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioImapNamedSettingsSectionParser* sectionP = CBioImapNamedSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0],(*iArgumentList)[1]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandSmtpSettingsCheck)
	{
		//This creates a new section parser
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(1);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioSmtpSettingsSectionParser* sectionP = CBioSmtpSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	if((*iCurrentCommand) == KCommandImapSettingsCheck)
	{
		//This creates a new section parser
		//
		// Check there are 1 argument
		CheckNumberOfArgumentsL(1);

		//
		// Create the client secion parser
		// Note we pass in the client test harness not the main one!
		CBioImapSettingsSectionParser* sectionP = CBioImapSettingsSectionParser::NewL(iClientTest, (*iDebugInfo.ScriptFile()), (*iArgumentList)[0]);
		CleanupStack::PushL(sectionP);
		sectionP->ParseL();
		CleanupStack::PopAndDestroy(sectionP);
	}
	else
	//
	// Client
	if((*iCurrentCommand) == KCommandClientParse)
	{
		iClientTest.AddStateL(CDoCmdClient::NewL(*iArgumentList, iClientTest, CDoCmdClient::ECmdClientParse), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandClientProcess)
	{
		iClientTest.AddStateL(CDoCmdClient::NewL(*iArgumentList, iClientTest, CDoCmdClient::ECmdClientProcess), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandClientFindBody)
	{
		CheckNumberOfArgumentsL(1);
		iClientTest.AddStateL(CDoCmdClient::NewL(*iArgumentList, iClientTest, CDoCmdClient::ECmdClientFindBody), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandClientFindSubject)
	{
		CheckNumberOfArgumentsL(1);
		iClientTest.AddStateL(CDoCmdClient::NewL(*iArgumentList, iClientTest, CDoCmdClient::ECmdClientFindSubject), iDebugInfo);
	}
	else

	//
	// Server
	if((*iCurrentCommand) == KCommandServerProcess)
	{
		iClientTest.AddStateL(CDoCmdServer::NewL(*iArgumentList, iClientTest, CDoCmdServer::ECmdServerProcess), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandServerParse)
	{
		iClientTest.AddStateL(CDoCmdServer::NewL(*iArgumentList, iClientTest, CDoCmdServer::ECmdServerParse), iDebugInfo);
	}
	
	else
	{
		User::Leave(KErrNotFound);
	}
}

void CBioClientCommandsParser::ConstructL()
{
	CBaseCommandParser::ConstructL();

	//
	// Add the general commands
	AddCommandL(KCommandCleanMessageFolder);
	AddCommandL(KCommandPrettifyLog);
	AddCommandL(KCommandDumpParsedMsg);
	AddCommandL(KCommandAddComment);
	AddCommandL(KCommandGenerateMsgFromFile);
	AddCommandL(KCommandGenerateMsgFromSection);
	AddCommandL(KCommandCheckUsedBif);
	AddCommandL(KCommandChkFileInMsgDirWithExt);
	AddCommandL(KCommandCleanAllBioMsgs);

	//
	// Add the BioDB Commands
	AddCommandL(KCommandDumpBif);
	AddCommandL(KCommandBifAPITest);
	AddCommandL(KCommandDumpWapBif);
	//AddCommandL(KCommandCheckBioNess);
	AddCommandL(KCommandDefaultSendBearer);

	//
	// Add the CommDB related Commands
	AddCommandL(KCommandClientCommDB);

	//
	// Add the client side commands
	AddCommandL(KCommandClientParse);
	AddCommandL(KCommandClientProcess);
	AddCommandL(KCommandClientFindBody);
	AddCommandL(KCommandClientFindSubject);

	//
	// Add the server side commands
	AddCommandL(KCommandServerParse);
	AddCommandL(KCommandServerProcess);

	//Msv
	//AddCommandL(KCommandReadDefaultPOP3LoginName);
	AddCommandL(KCommandPop3NamedSettingsCheck);
	AddCommandL(KCommandSmtpNamedSettingsCheck);
	AddCommandL(KCommandImapNamedSettingsCheck);
	AddCommandL(KCommandPop3SettingsCheck);
	AddCommandL(KCommandSmtpSettingsCheck);
	AddCommandL(KCommandImapSettingsCheck);
	AddCommandL(KCommandSmsSettingsCheck);

}

//
// CBioSmsSettingsSectionParser
// Handles the [check_Sms_defs] section of the script
CBioSmsSettingsSectionParser* CBioSmsSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioSmsSettingsSectionParser* self = new (ELeave) CBioSmsSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioSmsSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioSmsSettingsSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iCommandParsers->AppendL(CBioSmsSettingsCommandsParser::NewL(iClientTest));
}

//
// CBioSmsSettingsCommandsParser
// Handles the commands in the
// [check_Sms_defs] section of the script
void CBioSmsSettingsCommandsParser::ProcessL() 
{
	if((*iCurrentCommand) == KCommandSmsChkMessageCentreNumber)
	{
		CheckNumberOfArgumentsL(2);
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkSmsMsgCentreNumber), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandSmsDumpMessageCentres)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdDumpSmsMsgCentres), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandSmsChkDefaultMessageCentreNumber)
	{
		CheckNumberOfArgumentsL(1);
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkDefaultSmsMsgCentre), iDebugInfo);
	}
	else
	{
		User::Leave(KErrNotFound);
	}
}

CBioSmsSettingsCommandsParser* CBioSmsSettingsCommandsParser::NewL(CClientTestHarness& aMainTest)
{
	CBioSmsSettingsCommandsParser* self = new (ELeave) CBioSmsSettingsCommandsParser(aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioSmsSettingsCommandsParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	AddCommandL(KCommandSmsChkMessageCentreNumber);
	AddCommandL(KCommandSmsChkDefaultMessageCentreNumber);
	AddCommandL(KCommandSmsDumpMessageCentres);
	
}

//
// CBioPop3NamedSettingsSectionParser
// Handles the [check_pop3_defs] section of the script
CBioPop3NamedSettingsSectionParser* CBioPop3NamedSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBioPop3NamedSettingsSectionParser* self = new (ELeave) CBioPop3NamedSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName,aSettingsName);
	CleanupStack::Pop(self);
	return self;
}

void CBioPop3NamedSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioPop3NamedSettingsSectionParser::ConstructL(const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);

	iClientTest.AddStateL(CDoCmdMsv::NewL(aSettingsName,iClientTest, CDoCmdMsv::ECmdInstantNamedPop3), debugInfo);

	iCommandParsers->AppendL(CBioPop3SettingsCommandsParser::NewL(iClientTest));
}

//
// CBioSmtpNamedSettingsSectionParser
// Handles the [check_Smtp_defs] section of the script
CBioSmtpNamedSettingsSectionParser* CBioSmtpNamedSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBioSmtpNamedSettingsSectionParser* self = new (ELeave) CBioSmtpNamedSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName,aSettingsName);
	CleanupStack::Pop(self);
	return self;
}

void CBioSmtpNamedSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioSmtpNamedSettingsSectionParser::ConstructL(const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);

	iClientTest.AddStateL(CDoCmdMsv::NewL(aSettingsName,iClientTest, CDoCmdMsv::ECmdInstantNamedSmtp), debugInfo);

	iCommandParsers->AppendL(CBioSmtpSettingsCommandsParser::NewL(iClientTest));
}

//
// CBioImapNamedSettingsSectionParser
// Handles the [check_Imap_defs] section of the script
CBioImapNamedSettingsSectionParser* CBioImapNamedSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBioImapNamedSettingsSectionParser* self = new (ELeave) CBioImapNamedSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName,aSettingsName);
	CleanupStack::Pop(self);
	return self;
}

void CBioImapNamedSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioImapNamedSettingsSectionParser::ConstructL(const TDesC& aNewSectionName,const TDesC& aSettingsName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);

	iClientTest.AddStateL(CDoCmdMsv::NewL(aSettingsName,iClientTest, CDoCmdMsv::ECmdInstantNamedImap), debugInfo);

	iCommandParsers->AppendL(CBioImapSettingsCommandsParser::NewL(iClientTest));
}

//
// CBioPop3SettingsSectionParser
// Handles the [check_pop3_defs] section of the script
CBioPop3SettingsSectionParser* CBioPop3SettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioPop3SettingsSectionParser* self = new (ELeave) CBioPop3SettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioPop3SettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioPop3SettingsSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);

	iClientTest.AddStateL(CDoCmdMsv::NewL(_L("Null"),iClientTest, CDoCmdMsv::ECmdInstantDefaultPop3), debugInfo);

	iCommandParsers->AppendL(CBioPop3SettingsCommandsParser::NewL(iClientTest));
}

//
// CBioPop3SettingsCommandsParser
// Handles the commands in the
// [check_pop3_defs] section of the script
void CBioPop3SettingsCommandsParser::ProcessL() 
{

	if((*iCurrentCommand) == KCommandPop3ChkLoginName)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3LoginName), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandPop3ChkPassword)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3Password), iDebugInfo);
	}
	else
	if((*iCurrentCommand) == KCommandPop3ChkServerAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3ServerAddress), iDebugInfo);
	}
	else
	/*if((*iCurrentCommand) == KCommandPop3ChkAPop)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3Apop), iDebugInfo);
	}
	else*/
	/*if((*iCurrentCommand) == KCommandPop3ChkAutoSendOnConnect)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3AutoSendOnConnect), iDebugInfo);
	}
	else*/
	/*if((*iCurrentCommand) == KCommandPop3ChkPort)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3Port), iDebugInfo);
	}
	else*/
	if((*iCurrentCommand) == KCommandPop3ChkUserAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3UserAddress), iDebugInfo);
	}
	else
	/*if((*iCurrentCommand) == KCommandPop3ChkVersion)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest, CDoCmdMsv::ECmdChkPop3Version), iDebugInfo);
	}
	else*/
	{
		User::Leave(KErrNotFound);
	}
}

CBioPop3SettingsCommandsParser* CBioPop3SettingsCommandsParser::NewL(CClientTestHarness& aMainTest)
{
	CBioPop3SettingsCommandsParser* self = new (ELeave) CBioPop3SettingsCommandsParser(aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioPop3SettingsCommandsParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	AddCommandL(KCommandPop3ChkLoginName);
	AddCommandL(KCommandPop3ChkPassword);
	AddCommandL(KCommandPop3ChkServerAddress);
	//AddCommandL(KCommandPop3ChkAPop);
	//AddCommandL(KCommandPop3ChkAutoSendOnConnect);
	//AddCommandL(KCommandPop3ChkPort);
	AddCommandL(KCommandPop3ChkUserAddress);
	//AddCommandL(KCommandPop3ChkVersion);
}

//
// CBioSmtpSettingsSectionParser
// Handles the [check_smtp_defs] section of the script
CBioSmtpSettingsSectionParser* CBioSmtpSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioSmtpSettingsSectionParser* self = new (ELeave) CBioSmtpSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioSmtpSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioSmtpSettingsSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iClientTest.AddStateL(CDoCmdMsv::NewL(_L("Null"),iClientTest, CDoCmdMsv::ECmdInstantDefaultSmtp), debugInfo);
	iCommandParsers->AppendL(CBioSmtpSettingsCommandsParser::NewL(iClientTest));
}

//
// CBioSmtpSettingsCommandsParser
// Handles the commands in the
// [check_Smtp_defs] section of the script
void CBioSmtpSettingsCommandsParser::ProcessL() 
{
	/*if((*iCurrentCommand) == KCommandSmtpEmailAlias)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpEmailAlias), iDebugInfo);
	}
	else */
	if((*iCurrentCommand) == KCommandSmtpEmailAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpEmailAddress), iDebugInfo);
	}
	else
	/*if((*iCurrentCommand) == KCommandSmtpReplyToAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpReplyToAddress), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpReceiptAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpReceiptAddress), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpBodyEncoding)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpBodyEncoding), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpDefaultMsgCharSet)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpDefaultMsgCharSet), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpAddVCardToEmail)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpAddVCardToEmail), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpAddSignatureToEmail)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpAddSignatureToEmail), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpRequestReceipts)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpRequestReceipts), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpSendCopyToSelf)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpSendCopyToSelf), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandSmtpSendMessageOption)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpSendMessageOption), iDebugInfo);
	}
	else */
	if((*iCurrentCommand) == KCommandSmtpServerAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpServerAddress), iDebugInfo);
	}
	else 
	/*if((*iCurrentCommand) == KCommandSmtpUserAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkSmtpUserAddress), iDebugInfo);
	}
	else*/
	{
		User::Leave(KErrNotFound);
	}
}

CBioSmtpSettingsCommandsParser* CBioSmtpSettingsCommandsParser::NewL(CClientTestHarness& aMainTest)
{
	CBioSmtpSettingsCommandsParser* self = new (ELeave) CBioSmtpSettingsCommandsParser(aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioSmtpSettingsCommandsParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	//AddCommandL(KCommandSmtpEmailAlias);
	AddCommandL(KCommandSmtpEmailAddress);
	//AddCommandL(KCommandSmtpReplyToAddress);
	//AddCommandL(KCommandSmtpReceiptAddress);
	//AddCommandL(KCommandSmtpBodyEncoding);
	//AddCommandL(KCommandSmtpDefaultMsgCharSet);
	//AddCommandL(KCommandSmtpAddVCardToEmail);
	//AddCommandL(KCommandSmtpAddSignatureToEmail);
	//AddCommandL(KCommandSmtpRequestReceipts);
	//AddCommandL(KCommandSmtpSendCopyToSelf);
	//AddCommandL(KCommandSmtpSendMessageOption);
	AddCommandL(KCommandSmtpServerAddress);
	//AddCommandL(KCommandSmtpUserAddress);

}

//
// CBioImapSettingsSectionParser
// Handles the [check_Imap_defs] section of the script
CBioImapSettingsSectionParser* CBioImapSettingsSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioImapSettingsSectionParser* self = new (ELeave) CBioImapSettingsSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioImapSettingsSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioImapSettingsSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iClientTest.AddStateL(CDoCmdMsv::NewL(_L("Null"),iClientTest, CDoCmdMsv::ECmdInstantDefaultImap), debugInfo);
	iCommandParsers->AppendL(CBioImapSettingsCommandsParser::NewL(iClientTest));
}

//
// CBioImapSettingsCommandsParser
// Handles the commands in the
// [check_Imap_defs] section of the script
void CBioImapSettingsCommandsParser::ProcessL() 
{
	if((*iCurrentCommand) == KCommandImapServerAddress)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapServerAddress), iDebugInfo);
	}
	else
	/*if((*iCurrentCommand) == KCommandImapPort)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapPort), iDebugInfo);
	} else */
	if((*iCurrentCommand) == KCommandImapLoginName)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapLoginName), iDebugInfo);
	}
	else 
	if((*iCurrentCommand) == KCommandImapPassword)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapPassword), iDebugInfo);
	}
	else 
	if((*iCurrentCommand) == KCommandImapFolderPath)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapFolderPath), iDebugInfo);
	}
	else 
	/*if((*iCurrentCommand) == KCommandImapPathSeparator)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapPathSeparator), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapDisconnectedUserMode)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapDisconnectedUserMode), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapSynchronise)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapSynchronise), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapSubscribe)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapSubscribe), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapAutoSendOnConnect)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapAutoSendOnConnect), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapMaxEmailSize)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapMaxEmailSize), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapDeleteEmailsWhenDisconnecting)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapDeleteEmailsWhenDisconnecting), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapAcknowledgeReceipts)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapAcknowledgeReceipts), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapGetMailOptions)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapGetMailOptions), iDebugInfo);
	}
	else */
	/*if((*iCurrentCommand) == KCommandImapInboxSynchronisationLimit)
	{
		iClientTest.AddStateL(CDoCmdMsv::NewL(*iArgumentList,iClientTest,CDoCmdMsv::ECmdChkImapInboxSynchronisationLimit), iDebugInfo);
	}	
	else*/
	{
		User::Leave(KErrNotFound);
	}
}

CBioImapSettingsCommandsParser* CBioImapSettingsCommandsParser::NewL(CClientTestHarness& aMainTest)
{
	CBioImapSettingsCommandsParser* self = new (ELeave) CBioImapSettingsCommandsParser(aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioImapSettingsCommandsParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	//AddCommandL(KCommandImapChkLoginName);
	AddCommandL(KCommandImapServerAddress);
	//AddCommandL(KCommandImapPort);
	AddCommandL(KCommandImapLoginName);
	AddCommandL(KCommandImapPassword);
	AddCommandL(KCommandImapFolderPath);
	//AddCommandL(KCommandImapPathSeparator);
	//AddCommandL(KCommandImapDisconnectedUserMode);
	//AddCommandL(KCommandImapSynchronise);
	//AddCommandL(KCommandImapSubscribe);
	//AddCommandL(KCommandImapAutoSendOnConnect);
	//AddCommandL(KCommandImapMaxEmailSize);
	//AddCommandL(KCommandImapDeleteEmailsWhenDisconnecting);
	//AddCommandL(KCommandImapAcknowledgeReceipts);
	//AddCommandL(KCommandImapGetMailOptions);
	//AddCommandL(KCommandImapInboxSynchronisationLimit);

}

//
// CBioCommDBSectionParser
// Handles the [bio_commdb] section of the script
CBioCommDBSectionParser* CBioCommDBSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aTableName, const TDesC& aNewSectionName)
{
	CBioCommDBSectionParser* self = new (ELeave) CBioCommDBSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aTableName, aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioCommDBSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioCommDBSectionParser::ConstructL(const TDesC& aTableName, const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iCommandParsers->AppendL(CBioCommDBCommandParser::NewL(iClientTest, aTableName));
}

void CBioCommDBSectionParser::SectionFinishedL(TTestDebugInfo& aDebugInfo)
{
	((CBioCommDBCommandParser*)iCommandParsers->At(0))->SectionFinishedL(aDebugInfo);
}

//
// CBioCommDBCommandParser
// Handles the commands in the
// [check_pop3_defs] section of the script
void CBioCommDBCommandParser::ProcessL() 
{
	if((*iCurrentCommand) == KCommandCommDbReadText)
	{
		CheckNumberOfArgumentsL(2);

		//
		// Add the stored table name
		iArgumentList->AppendL(iTableName);
		iClientTest.AddStateL(CDoCmdCommDB::NewL(*iArgumentList,iClientTest, CDoCmdCommDB::ECmdCommDBReadText), iDebugInfo);
	}

	else
	if((*iCurrentCommand) == KCommandCommDbReadLongText)
	{
		CheckNumberOfArgumentsL(2);

		//
		// Add the stored table name
		iArgumentList->AppendL(iTableName);
		iClientTest.AddStateL(CDoCmdCommDB::NewL(*iArgumentList,iClientTest, CDoCmdCommDB::ECmdCommDBReadLongText), iDebugInfo);
	}

	else
	if((*iCurrentCommand) == KCommandCommDbReadUint)
	{
		CheckNumberOfArgumentsL(2);

		//
		// Add the stored table name
		iArgumentList->AppendL(iTableName);
		iClientTest.AddStateL(CDoCmdCommDB::NewL(*iArgumentList, iClientTest, CDoCmdCommDB::ECmdCommDBReadUint), iDebugInfo);
	}

	else
	if((*iCurrentCommand) == KCommandCommDbReadBool)
	{
		CheckNumberOfArgumentsL(2);

		//
		// Add the stored table name
		iArgumentList->AppendL(iTableName);
		iClientTest.AddStateL(CDoCmdCommDB::NewL(*iArgumentList, iClientTest, CDoCmdCommDB::ECmdCommDBReadBool), iDebugInfo);
	}

	else
	if((*iCurrentCommand) == KCommandCommDbCheck)
	{
		CheckNumberOfArgumentsL(3);

		//
		// Build up the SQL
		const TPtrC& ptrFieldName = (*iArgumentList)[1];
		const TPtrC& ptrExpected = (*iArgumentList)[2];
		_LIT(KEquals, "=");
		const TInt conditionPacking = 2+4; // 2x comma separators + 4 max length of type field (ie, bool, text or uint)
		TInt conditionLength = 0;
		if(iClientTest.iSQL == NULL)
			{			
			conditionLength = ptrFieldName.Length() + ptrExpected.Length() + conditionPacking;
			iClientTest.iSQL = HBufC::NewL(conditionLength);
			
			TPtr conditionDes = iClientTest.iSQL->Des();
			
			// append field name
			conditionDes.Append(ptrFieldName);
			conditionDes.Append(KEquals);
			
			iClientTest.iSQL->Des().Append(ptrExpected);
			}
			else
			{
			conditionLength = iClientTest.iSQL->Length() + ptrFieldName.Length() + ptrExpected.Length() + conditionPacking + 3;
			
			iClientTest.iSQL = iClientTest.iSQL->ReAllocL(conditionLength);
			
			iClientTest.iSQL->Des().Append(KCommaSeparator);
			TPtr conditionDes = iClientTest.iSQL->Des();
			
			// append field name
			conditionDes.Append(ptrFieldName);
			conditionDes.Append(KEquals);
			
			iClientTest.iSQL->Des().Append(ptrExpected);
			
			conditionDes = iClientTest.iSQL->Des();
			}		

	}
	else
	{
		User::Leave(KErrNotFound);
	}
}

CBioCommDBCommandParser* CBioCommDBCommandParser::NewL(CClientTestHarness& aMainTest, const TDesC& aTableName)
{
	CBioCommDBCommandParser* self = new (ELeave) CBioCommDBCommandParser(aMainTest, aTableName);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioCommDBCommandParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	AddCommandL(KCommandCommDbReadText);
	AddCommandL(KCommandCommDbReadLongText);
	AddCommandL(KCommandCommDbReadUint);
	AddCommandL(KCommandCommDbReadBool);
	AddCommandL(KCommandCommDbCheck);
}

void CBioCommDBCommandParser::SectionFinishedL(TTestDebugInfo& aDebugInfo)
{
	iArgumentList->Reset();
	iArgumentList->AppendL(*iClientTest.iSQL);
	iArgumentList->AppendL(iTableName);
	iClientTest.AddStateL(CDoCmdCommDB::NewL(*iArgumentList,iClientTest, CDoCmdCommDB::ECmdCommDBCheck), aDebugInfo);
	delete iClientTest.iSQL;
	iClientTest.iSQL = NULL;
}


//
// CBioGenMsgSectionParser
// Handles the [gen_msg_from_section] section of the script
CBioGenMsgSectionParser* CBioGenMsgSectionParser::NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName)
{
	CBioGenMsgSectionParser* self = new (ELeave) CBioGenMsgSectionParser(aTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
}

void CBioGenMsgSectionParser::LogComment(const TDesC& aComment)
{
	iClientTest.TestUtils().WriteComment(aComment);
}

void CBioGenMsgSectionParser::ConstructL(const TDesC& aNewSectionName)
{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iClientTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iClientTest.SetDebugInfo(debugInfo);
	iCmdParser = CBioGenMsgCommandParser::NewL(iClientTest);
	iCommandParsers->AppendL(iCmdParser);		// Transfer ownership
}

void CBioGenMsgSectionParser::SectionFinishedL() 
{ 
	iCmdParser->SectionFinishedL(); 
}

//
// CBioGenMsgCommandParser
// Handles the commands in the
// [gen_msg_from_section] section of the script
void CBioGenMsgCommandParser::ProcessL() 
{
	if((*iCurrentCommand) == KCommandGenMsgAddLine)
	{
		//
		// Append to buffer
		TPtrC aString((*iArgumentList)[0]);
		_LIT(KCrLf, "\r\n");

		if(iBuffer == NULL)
		{
			iBuffer = HBufC::NewL(aString.Length() + KCrLf().Length());
			iBuffer->Des().Copy(aString);
			iBuffer->Des().Append(KCrLf);
		}
		else
		{
			iBuffer = iBuffer->ReAllocL(iBuffer->Length() + aString.Length() + KCrLf().Length());
			iBuffer->Des().Append(aString);
			iBuffer->Des().Append(KCrLf);
		}

	}
	else
	if((*iCurrentCommand) == KCommandGenMsgType)
	{
		iMsgType = EBioIapSettingsMessage;
	}
	else
	{
		User::Leave(KErrNotFound);
	}
}

CBioGenMsgCommandParser* CBioGenMsgCommandParser::NewL(CClientTestHarness& aMainTest)
{
	CBioGenMsgCommandParser* self = new (ELeave) CBioGenMsgCommandParser(aMainTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
}

void CBioGenMsgCommandParser::ConstructL() 
{
	CBaseCommandParser::ConstructL();

	//
	// Add the commands
	AddCommandL(KCommandGenMsgAddLine);
	AddCommandL(KCommandGenMsgType);
}

void CBioGenMsgCommandParser::SectionFinishedL()
{
	const HBufC* buf = iBuffer;

	iClientTest.AddStateL(CDoCmdGeneral::NewL(iClientTest, buf, iMsgType, CDoCmdGeneral::ECmdGeneralGenerateFromSection), iDebugInfo);
}
