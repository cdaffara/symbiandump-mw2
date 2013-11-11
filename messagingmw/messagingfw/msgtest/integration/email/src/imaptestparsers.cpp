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

#include "imaptestparsers.h"
#include "imaptests.h"

_LIT(KCommandImapClientTest,		"imap_client_test");
_LIT(KCommandImapService,			"imap_service");		// 1 parameter, creates an imap service
_LIT(KCommandImapUseService,		"use_service");			// 0 parameters, uses the currently selected service

// IMAP settings strings
_LIT(KCommandImapSetServer,			"set_server");			// 1 parameter, sets the IMAP server
_LIT(KCommandImapSetUserName,		"set_user");			// 1 parameter, sets the username for the IMAP service
_LIT(KCommandImapSetPassword,		"set_pass");			// 1 parameter, sets the password for the IMAP service
_LIT(KCommandImapSetName,			"set_name");			// 1 parameter, sets the name of the IMAP service
_LIT(KCommandImapSetFolderPath,		"set_folder_path");		// 1 parameter, sets the folder path of the IMAP service
_LIT(KCommandImapSetTLS,			"set_tls");				// 1 parameter, sets the secure connection for IMAP service.  Value should be 0 or 1
_LIT(KCommandImapSetUpdatingSeenFlags,	"set_updating_seen_flag");	// 1 parameter, sets the updating seen flag value to the parameter value
_LIT(KCommandImapSetWrappedTLS,  "set_wrapped_tls");	    // 1 parameter, sets the secure connection for IMAP service.  value should be 1 or 0
_LIT(KCommandImapSetPort,  "set_port");	                    // 1 parameter, sets the port no for IMAP service.

// IMAP Operations
_LIT(KCommandImapConnect,			"connect");				// 0 parameters, connects to the IMAP service
_LIT(KCommandImapConnectAndCancel,		"connect_and_cancel");	// 1 parameters, connects to the IMAP service, starts the sync process then cancels after the specified interval
_LIT(KCommandImapConnectAndMonitor,			"connect_and_monitor");			// 0 parameters, connects to the IMAP service and monitors connection
_LIT(KCommandImapConnectMonitorAndSync,		"connect_monitor_and_sync");	// 0 parameters, connects to the IMAP service and monitors connection, then syncs
_LIT(KCommandImapConnectAndSync,	"connect_and_sync");	// 0 parameters, connects to the IMAP service and automatically synchronises
_LIT(KCommandImapConnectAndPeriodicSync,	"connect_and_periodic_sync");	// 0 parameters, connects to the IMAP service and automatically synchronises
_LIT(KCommandImapDisconnect,		"disconnect");			// 0 parameters, disconnects from the IMAP service

_LIT(KCommandImapSetSyncRate,		"set_sync_rate");	// 1 parameters, sets the sync rate - in seconds
_LIT(KCommandImapGetSyncRate,		"get_sync_rate");	// 1 parameters, gets the sync rate and validates with given rate param

_LIT(KCommandImapCheckStatus,		"check_imap_error");	// 1 parameter, checks the last error code and compares it to param 1
															//  Note: must only be used to check commands that have been set using
															//  CMsvClientTest::SetCurrentOperation

_LIT(KCommandImapSyncTree,			"sync_tree");			// 0 parameters, synchronises the local IMAP structure with the remote
_LIT(KCommandImapCheckRemoteInbox,	"check_remote_inbox");	// 0 parameters, checks the remote inbox for new mail and retrieves headers
_LIT(KCommandImapSyncFolder,		"sync_folder");			// 0 parameters, synchonises the currently selected IMAP folder with the server
_LIT(KCommandImapSubscribeFolder,	"subscribe_folder");	// 0 parameters, subscribes to the currently selected folder
_LIT(KCommandImapUnsubscribeFolder,	"unsubscribe_folder");	// 0 parameters, unsubscribes from the currently selected folder
_LIT(KCommandImapPopulateFolder,	"populate_folder");		// 0 parameters, populates the bodies of the currently selected messages
_LIT(KCommandImapPerformFullSync,	"perform_full_sync");	// 0 parameters, performs a full synchronisation with the selected remote service
_LIT(KCommandImapSetSyncLimits,		"set_sync_limits");		// 2 parameters, resets the sync limits for currently selected imap service

//Commands for IMAP read status testing - defect INC016632
//mark_selection_unread
//check_selection

_LIT(KCommandImapMarkSelection,	"mark_selection_unread");	// 1 parameter, marks all the messages in mailbox as specified in parameter
_LIT(KCommandImapCheckSelectionUnread, "check_selection_unread");	// 1 parameter, checks the unread flag all the messages in mailbox as specified in parameter
_LIT(KCommandImapCheckSelectionSeen, "check_selection_seen");	// 1 parameter, checks the seen flag all the messages in mailbox as specified in parameter
_LIT(KCommandImapMarkSelectedMail, "mark_selected_mail");	// 0 parameters, marks selected messages in mailbox as read
_LIT(KCommandImapCheckSelectedMail, "check_selected_mail");	// 0 parameters, checks selected messages in mailbox as read

//
//
// CNewImapTestCommandParser
//
// A command parser
// Parses the command which generates a new Imap test harness
// This command would usually be found in an email test section

CNewImapTestCommandParser* CNewImapTestCommandParser::NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest)
	{
	CNewImapTestCommandParser* self = new (ELeave) CNewImapTestCommandParser(aScript, aEmailClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	};

void CNewImapTestCommandParser::ProcessL()
	{
	// Create an smtp test, give it the test utilities
	CImapClientTest* imapTest = CImapClientTest::NewL(iEmailClientTest.EmailTestUtils(), iEmailClientTest.TestConsole());
	CleanupStack::PushL(imapTest);

	// Attach the email test to the main test
	iEmailClientTest.AddStateL(imapTest, iDebugInfo);
	CleanupStack::Pop(imapTest); // emailTest is now owned by the iMainTest

	// Check that there is one argument, the email test section name
	CheckNumberOfArgumentsL(1);

	// Create an email section parser
	CImapClientSectionParser* sectionParser = CImapClientSectionParser::NewL(*imapTest, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewImapTestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandImapClientTest);
	}

CNewImapTestCommandParser::CNewImapTestCommandParser(CTestScript& aScript, CEmailClientTest& aEmailClientTest) : iEmailClientTest(aEmailClientTest), iScript(aScript)
	{
	}




//
//
// CImapClientSectionParser
//
// A section parser
// Parses an IMAP test section

CImapClientSectionParser* CImapClientSectionParser::NewL(CImapClientTest& aImapClientTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CImapClientSectionParser* self = new (ELeave) CImapClientSectionParser(aImapClientTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CImapClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CEmailClientSectionParser::ConstructL(aNewSectionName);
	AddCommandParserL(CImapServiceCommandParser::NewL(iScript, iImapTest));
	AddCommandParserL(CImapOperationParser::NewL(iImapTest));	
	AddCommandParserL(CImapCheckStatusParser::NewL(iImapTest));	
	}

CImapClientSectionParser::CImapClientSectionParser(CImapClientTest& aImapTest, CTestScript& aScript) : CEmailClientSectionParser(aImapTest, aScript), iImapTest(aImapTest)
	{
	}



//
//
// CImapServiceCommandParser
//

void CImapServiceCommandParser::ProcessL()
	{
	CheckNumberOfArgumentsL(1);
	// Create the settings object which we will give to the settings section parser
	CImapServiceSectionParser* serviceParser = CImapServiceSectionParser::NewL(iScript, iImapClientTest, (*iArgumentList)[0]);
	CleanupStack::PushL(serviceParser);

	serviceParser->ParseL();
	CleanupStack::PopAndDestroy(serviceParser);
	}

CImapServiceCommandParser* CImapServiceCommandParser::NewL(CTestScript& aScript, CImapClientTest& aImapClientTest)
	{
	CImapServiceCommandParser* self = new (ELeave) CImapServiceCommandParser(aScript, aImapClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CImapServiceCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandImapService);
	}

CImapServiceCommandParser::CImapServiceCommandParser(CTestScript& aScript, CImapClientTest& aImapClientTest) : iImapClientTest(aImapClientTest), iScript(aScript)
	{
	}




//
//
// CImapServiceSectionParser
//

// A section parser
// Parses a IMAP section to create a IMAP service

CImapServiceSectionParser* CImapServiceSectionParser::NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName)
	{
	CImapServiceSectionParser* self = new (ELeave) CImapServiceSectionParser(aScript, aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL(aSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CImapServiceSectionParser::ConstructL(const TDesC& aSectionName)
	{
	CBaseSectionParser::ConstructL(aSectionName);

	// Add the state to generate the imap service
	CCreateImapService* testState = CCreateImapService::NewL(iTestHarness);

	TTestDebugInfo debugInfo(iScript, iSection->SectionPosition(), 0);
	iTestHarness.AddStateL(testState, debugInfo);

	// The parsers will fill in the entry and service details before the state that creates the entry is run.
	AddCommandParserL(CImapServiceCommands::NewL(*testState));	
	
	CEmailServiceSectionParser::ConstructL(aSectionName, *testState);
	}

CImapServiceSectionParser::CImapServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness) : CEmailServiceSectionParser(aScript, aTestHarness)
	{
	}

CImapServiceSectionParser::~CImapServiceSectionParser()
	{
	}



//
//
// CImapServiceCommands
//
// A command parser
// Parses the commands that generate the IMAP service

CImapServiceCommands* CImapServiceCommands::NewL(CCreateImapService& aTestState)
	{
	CImapServiceCommands* self = new (ELeave) CImapServiceCommands(aTestState);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CImapServiceCommands::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandImapSetServer)
		{
		CheckNumberOfArgumentsL(1);
		iTestState.Settings().SetServerAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandImapSetUserName)
		 {
		CheckNumberOfArgumentsL(1);
		TBuf8<128> loginName;
		loginName.Copy((*iArgumentList)[0]);
		iTestState.Settings().SetLoginNameL(loginName);
		}
	else if ((*iCurrentCommand) == KCommandImapSetPassword)
		{
		CheckNumberOfArgumentsL(1);
		TBuf8<128> password;
		password.Copy((*iArgumentList)[0]);
		iTestState.Settings().SetPasswordL(password);
		}
	else if ((*iCurrentCommand) == KCommandImapSetName)
		{
		iTestState.SetDetailsStringL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandImapSetFolderPath)
		{
		CheckNumberOfArgumentsL(1);
		TBuf8<128> folderName;
		folderName.Copy((*iArgumentList)[0]);
		iTestState.Settings().SetFolderPathL(folderName);
		}
	else if ((*iCurrentCommand) == KCommandImapSetTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setTls=0;
		User::LeaveIfError(lex.Val(setTls));
		iTestState.Settings().SetSecureSockets(setTls);
		}
	else if ((*iCurrentCommand) == KCommandImapSetUpdatingSeenFlags)
		{

		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt updateseenflag;
		User::LeaveIfError(lex.Val(updateseenflag));
		iTestState.Settings().SetUpdatingSeenFlags(updateseenflag);
		}
	else if ((*iCurrentCommand) == KCommandImapSetPort)
		{
		
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setPort=0;
		User::LeaveIfError(lex.Val(setPort));
		iTestState.Settings().SetPort(setPort);
		}
	else if ((*iCurrentCommand) == KCommandImapSetWrappedTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setWrappedTls=0;
		User::LeaveIfError(lex.Val(setWrappedTls));
		iTestState.Settings().SetSSLWrapper(setWrappedTls);
		}
	}

CImapServiceCommands::CImapServiceCommands(CCreateImapService& aTestState) : iTestState(aTestState)
	{
	}

void CImapServiceCommands::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandImapSetServer);
	AddCommandL(KCommandImapSetUserName);
	AddCommandL(KCommandImapSetPassword);
	AddCommandL(KCommandImapSetName);
	AddCommandL(KCommandImapSetFolderPath);
	AddCommandL(KCommandImapSetTLS);
	AddCommandL(KCommandImapSetUpdatingSeenFlags);
	AddCommandL(KCommandImapSetPort);
	AddCommandL(KCommandImapSetWrappedTLS);
	}


//
//
// CImapOperationParser
//

CImapOperationParser* CImapOperationParser::NewL(CImapClientTest& aImapClientTest)
	{
	CImapOperationParser* self = new (ELeave) CImapOperationParser(aImapClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CImapOperationParser::ProcessL()
	{
	
	if ((*iCurrentCommand) == KCommandImapConnect)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnect(iImapClientTest, CImapConnect::ENoSync), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapConnectAndCancel)
		{
		CheckNumberOfArgumentsL(1);
		TUint cancelDelay;
		TLex cancelDelayLex((*iArgumentList)[0]);
		User::LeaveIfError(cancelDelayLex.Val(cancelDelay));

		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnectAndCancel(iImapClientTest, cancelDelay), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapConnectAndMonitor)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnectAndMonitor(iImapClientTest, EFalse), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapConnectMonitorAndSync)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnectAndMonitor(iImapClientTest, ETrue), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapConnectAndSync)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnect(iImapClientTest, CImapConnect::EConnectAndSync), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapConnectAndPeriodicSync)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CCheckService(iImapClientTest), iDebugInfo);
		iImapClientTest.AddStateL(new (ELeave) CImapConnect(iImapClientTest, CImapConnect::EConnectAndPeriodicSync), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapDisconnect)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapDisconnect(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapUseService)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapUseService(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapCheckRemoteInbox)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapInboxNewSync(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapSyncTree)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapSyncTree(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapSyncFolder)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapFolderSync(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapSubscribeFolder)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapSubscribeFolder(ETrue,iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapUnsubscribeFolder)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapSubscribeFolder(EFalse,iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapPopulateFolder)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapPopulateFolder(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapPerformFullSync)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapPerformFullSync(iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapSetSyncLimits)
		{
		CheckNumberOfArgumentsL(2);

		TInt inboxSyncLimit,folderSyncLimit;
		TLex inboxLex((*iArgumentList)[0]);
		TLex folderLex((*iArgumentList)[1]);
		User::LeaveIfError(inboxLex.Val(inboxSyncLimit));
		User::LeaveIfError(folderLex.Val(folderSyncLimit));

		iImapClientTest.AddStateL(new (ELeave) CImapSetSyncLimits(inboxSyncLimit,folderSyncLimit,iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapMarkSelection)
		{
		
		CheckNumberOfArgumentsL(1);

		TBool Unreadflag;
		TLex unreadflaglex((*iArgumentList)[0]);
		User::LeaveIfError(unreadflaglex.Val(Unreadflag));

		iImapClientTest.AddStateL(new (ELeave) CImapMarkSelection(Unreadflag,*(iImapClientTest.iCurrentSelection), iImapClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapCheckSelectionUnread)
		{
		
		CheckNumberOfArgumentsL(1);

		TInt TestType;
		TLex TestTypelex((*iArgumentList)[0]);
		User::LeaveIfError(TestTypelex.Val(TestType));
		iImapClientTest.AddStateL(new (ELeave) CImapCheckSelectionUnread(TestType, *(iImapClientTest.iCurrentSelection), iImapClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapCheckSelectionSeen)
		{
		
		CheckNumberOfArgumentsL(1);

		TInt Seenflag;
		TLex Seenflaglex((*iArgumentList)[0]);
		User::LeaveIfError(Seenflaglex.Val(Seenflag));
		iImapClientTest.AddStateL(new (ELeave) CImapCheckSelectionSeen(Seenflag, *(iImapClientTest.iCurrentSelection), iImapClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapMarkSelectedMail)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapMarkSelectedMail(*(iImapClientTest.iCurrentSelection), iImapClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapCheckSelectedMail)
		{
		CheckNumberOfArgumentsL(0);
		iImapClientTest.AddStateL(new (ELeave) CImapCheckSelectedMail(*(iImapClientTest.iCurrentSelection), iImapClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapSetSyncRate)
		{
		CheckNumberOfArgumentsL(1);
		TUint syncRate;
		TLex syncRateLex((*iArgumentList)[0]);
		User::LeaveIfError(syncRateLex.Val(syncRate));

		iImapClientTest.AddStateL(new (ELeave) CImapSetSyncRate(syncRate,iImapClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandImapGetSyncRate)
		{
		CheckNumberOfArgumentsL(1);
		TUint syncRate;
		TLex syncRateLex((*iArgumentList)[0]);
		User::LeaveIfError(syncRateLex.Val(syncRate));

		iImapClientTest.AddStateL(new (ELeave) CImapGetSyncRate(syncRate,iImapClientTest), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CImapOperationParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandImapConnect);
	AddCommandL(KCommandImapConnectAndCancel);
	AddCommandL(KCommandImapConnectAndMonitor);
	AddCommandL(KCommandImapConnectMonitorAndSync);
	AddCommandL(KCommandImapConnectAndSync);
	AddCommandL(KCommandImapConnectAndPeriodicSync);
	AddCommandL(KCommandImapDisconnect);
	AddCommandL(KCommandImapUseService);
	AddCommandL(KCommandImapCheckRemoteInbox);
	AddCommandL(KCommandImapSyncTree);
	AddCommandL(KCommandImapSyncFolder);
	AddCommandL(KCommandImapSubscribeFolder);
	AddCommandL(KCommandImapUnsubscribeFolder);
	AddCommandL(KCommandImapPopulateFolder);
	AddCommandL(KCommandImapPerformFullSync);
	AddCommandL(KCommandImapSetSyncLimits);
	AddCommandL(KCommandImapMarkSelection);
	AddCommandL(KCommandImapCheckSelectionUnread);
	AddCommandL(KCommandImapCheckSelectionSeen);
	AddCommandL(KCommandImapMarkSelectedMail);
	AddCommandL(KCommandImapCheckSelectedMail);
	AddCommandL(KCommandImapSetSyncRate);
	AddCommandL(KCommandImapGetSyncRate);
	}

CImapOperationParser::CImapOperationParser(CImapClientTest& aImapClientTest) : iImapClientTest(aImapClientTest)
	{
	}




//
//
// CImapCheckStatusParser
//


CImapCheckStatusParser* CImapCheckStatusParser::NewL(CImapClientTest& aImapTestHarness)
	{
	CImapCheckStatusParser* self = new (ELeave) CImapCheckStatusParser(aImapTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CImapCheckStatusParser::ProcessL()
	{
	TLex lex((*iArgumentList)[0]);
	TInt expectedStatus;
	User::LeaveIfError(lex.Val(expectedStatus));
	iTestHarness.AddStateL(new (ELeave) CCheckImapOperation(expectedStatus, iTestHarness), iDebugInfo);
	}

CImapCheckStatusParser::CImapCheckStatusParser(CImapClientTest& aImapTestHarness) : iTestHarness(aImapTestHarness)
	{
	}

void CImapCheckStatusParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandImapCheckStatus);
	}

