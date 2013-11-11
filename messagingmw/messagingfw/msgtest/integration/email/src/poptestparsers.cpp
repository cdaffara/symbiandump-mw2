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

#include "poptestparsers.h"
#include "poptests.h"

_LIT(KCommandPop3ClientTest, "pop3_client_test");
_LIT(KCommandPop3Service, "pop3_service");
_LIT(KCommandPop3UseService,		"use_service");			// 0 parameters, uses the currently selected service

// POP3 settings strings
_LIT(KCommandPop3SetServer, "set_server");
_LIT(KCommandPop3SetUserName, "set_user");
_LIT(KCommandPop3SetPassword, "set_pass");
_LIT(KCommandPop3SetName, "set_name");
_LIT(KCommandPop3SetTLS,  "set_tls");				// 1 parameter, sets the secure connection for POP service.  value should be 1 or 0
_LIT(KCommandPop3SetWrappedTLS,  "set_wrapped_tls");	// 1 parameter, sets the secure connection for POP service.  value should be 1 or 0
_LIT(KCommandPop3SetPort,  "set_port");	// 1 parameter, sets the port no for POP service.

// POP3 Operations
_LIT(KCommandPop3Connect, "connect");
_LIT(KCommandPop3ConnectAndMonitor,			"connect_and_monitor");	// 0 parameters, connects to the Pop3 service and monitors connection
_LIT(KCommandPop3Disconnect, "disconnect");

_LIT(KCommandPop3CheckStatus,   "check_pop3_error");

_LIT(KCommandPop3SetSyncLimits,	"set_sync_limits");		// 2 parameters, resets the sync limits for currently selected pop service


//
//
// CNewPop3TestCommandParser
//

// A command parser
// Parses the command which generates a new Pop3 test harness
// This command would usually be found in an email test section

CNewPop3TestCommandParser* CNewPop3TestCommandParser::NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest)
	{
	CNewPop3TestCommandParser* self = new (ELeave) CNewPop3TestCommandParser(aScript, aEmailClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	};

void CNewPop3TestCommandParser::ProcessL()
	{
	// Create an smtp test, give it the test utilities
	CPop3ClientTest* pop3Test = CPop3ClientTest::NewL(iEmailClientTest.EmailTestUtils(), iEmailClientTest.TestConsole());
	CleanupStack::PushL(pop3Test);

	// Attach the email test to the main test
	iEmailClientTest.AddStateL(pop3Test, iDebugInfo);
	CleanupStack::Pop(pop3Test); // emailTest is now owned by the iMainTest

	// Check that there is one argument, the email test section name
	CheckNumberOfArgumentsL(1);

	// Create an email section parser
	CPop3ClientSectionParser* sectionParser = CPop3ClientSectionParser::NewL(*pop3Test, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewPop3TestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandPop3ClientTest);
	}

CNewPop3TestCommandParser::CNewPop3TestCommandParser(CTestScript& aScript, CEmailClientTest& aEmailClientTest) : iEmailClientTest(aEmailClientTest), iScript(aScript)
	{
	}




//
//
// CPop3ClientSectionParser
//

// A section parser
// Parses an POP3 test section

CPop3ClientSectionParser* CPop3ClientSectionParser::NewL(CPop3ClientTest& aPop3ClientTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CPop3ClientSectionParser* self = new (ELeave) CPop3ClientSectionParser(aPop3ClientTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CPop3ClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CEmailClientSectionParser::ConstructL(aNewSectionName);
	AddCommandParserL(CPop3ServiceCommandParser::NewL(iScript, iPop3Test));
	AddCommandParserL(CPop3OperationParser::NewL(iPop3Test));	
	AddCommandParserL(CPop3CheckStatusParser::NewL(iPop3Test));	
	}

CPop3ClientSectionParser::CPop3ClientSectionParser(CPop3ClientTest& aPop3Test, CTestScript& aScript) : CEmailClientSectionParser(aPop3Test, aScript), iPop3Test(aPop3Test)
	{
	}



//
//
// CPop3ServiceCommandParser
//

void CPop3ServiceCommandParser::ProcessL()
	{
	CheckNumberOfArgumentsL(1);
	// Create the settings object which we will give to the settings section parser
	CPop3ServiceSectionParser* serviceParser = CPop3ServiceSectionParser::NewL(iScript, iPop3ClientTest, (*iArgumentList)[0]);
	CleanupStack::PushL(serviceParser);

	serviceParser->ParseL();
	CleanupStack::PopAndDestroy(serviceParser);
	}

CPop3ServiceCommandParser* CPop3ServiceCommandParser::NewL(CTestScript& aScript, CPop3ClientTest& aPop3ClientTest)
	{
	CPop3ServiceCommandParser* self = new (ELeave) CPop3ServiceCommandParser(aScript, aPop3ClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPop3ServiceCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandPop3Service);
	}

CPop3ServiceCommandParser::CPop3ServiceCommandParser(CTestScript& aScript, CPop3ClientTest& aPop3ClientTest) : iPop3ClientTest(aPop3ClientTest), iScript(aScript)
	{
	}




//
//
// CPop3ServiceSectionParser
//

// A section parser
// Parses a POP3 section to create a POP3 service

CPop3ServiceSectionParser* CPop3ServiceSectionParser::NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName)
	{
	CPop3ServiceSectionParser* self = new (ELeave) CPop3ServiceSectionParser(aScript, aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL(aSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CPop3ServiceSectionParser::ConstructL(const TDesC& aSectionName)
	{
	CBaseSectionParser::ConstructL(aSectionName);

	// Add the state to generate the pop3 service
	CCreatePop3Service* testState = CCreatePop3Service::NewL(iTestHarness);
	TTestDebugInfo debugInfo(iScript, iSection->SectionPosition(), 0);
	iTestHarness.AddStateL(testState, debugInfo);

	// The parsers will fill in the entry and service details before the state that creates the entry is run.
	AddCommandParserL(CPop3ServiceCommands::NewL(*testState));	
	
	CEmailServiceSectionParser::ConstructL(aSectionName, *testState);
	}

CPop3ServiceSectionParser::CPop3ServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness) : CEmailServiceSectionParser(aScript, aTestHarness)
	{
	}

CPop3ServiceSectionParser::~CPop3ServiceSectionParser()
	{
	}



//
//
// CPop3ServiceCommands
//

// A command parser
// Parses the commands that generate the POP3 service

CPop3ServiceCommands* CPop3ServiceCommands::NewL(CCreatePop3Service& aTestState)
	{
	CPop3ServiceCommands* self = new (ELeave) CPop3ServiceCommands(aTestState);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPop3ServiceCommands::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandPop3SetServer)
		{
		CheckNumberOfArgumentsL(1);
		iTestState.Settings().SetServerAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetUserName)
		{
		CheckNumberOfArgumentsL(1);
		TBuf8<128> loginName;
		loginName.Copy((*iArgumentList)[0]);
		iTestState.Settings().SetLoginNameL(loginName);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetPassword)
		{
		CheckNumberOfArgumentsL(1);
		TBuf8<128> password;
		password.Copy((*iArgumentList)[0]);
		iTestState.Settings().SetPasswordL(password);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetName)
		{
		iTestState.SetDetailsStringL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setTls=0;
		User::LeaveIfError(lex.Val(setTls));
		iTestState.Settings().SetSecureSockets(setTls);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetPort)
		{
		
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setPort=0;
		User::LeaveIfError(lex.Val(setPort));
		iTestState.Settings().SetPort(setPort);
		
		}
	else if ((*iCurrentCommand) == KCommandPop3SetWrappedTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setWrappedTls=0;
		User::LeaveIfError(lex.Val(setWrappedTls));
		iTestState.Settings().SetSSLWrapper(setWrappedTls);
		}

	}

CPop3ServiceCommands::CPop3ServiceCommands(CCreatePop3Service& aTestState) : iTestState(aTestState)
	{
	}

void CPop3ServiceCommands::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandPop3SetServer);
	AddCommandL(KCommandPop3SetUserName);
	AddCommandL(KCommandPop3SetPassword);
	AddCommandL(KCommandPop3SetName);
	AddCommandL(KCommandPop3SetTLS);
	AddCommandL(KCommandPop3SetPort);
	AddCommandL(KCommandPop3SetWrappedTLS);
	}







//
//
// CPop3OperationParser
//

CPop3OperationParser* CPop3OperationParser::NewL(CPop3ClientTest& aPop3ClientTest)
	{
	CPop3OperationParser* self = new (ELeave) CPop3OperationParser(aPop3ClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPop3OperationParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandPop3Connect)
		{
		CheckNumberOfArgumentsL(0);
		iPop3ClientTest.AddStateL(new (ELeave) CPop3Connect(iPop3ClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandPop3ConnectAndMonitor)
		{
		CheckNumberOfArgumentsL(0);
		
		iPop3ClientTest.AddStateL(new (ELeave) CPop3ConnectAndMonitor(iPop3ClientTest, EFalse), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandPop3Disconnect)
		{
		CheckNumberOfArgumentsL(0);
		iPop3ClientTest.AddStateL(new (ELeave) CPop3Disconnect(iPop3ClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandPop3UseService)
		{
		CheckNumberOfArgumentsL(0);
		iPop3ClientTest.AddStateL(new (ELeave) CPop3UseService(iPop3ClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandPop3SetSyncLimits)
		{
		CheckNumberOfArgumentsL(1);

		TInt inboxSyncLimit;
		TLex inboxLex((*iArgumentList)[0]);
		User::LeaveIfError(inboxLex.Val(inboxSyncLimit));

		iPop3ClientTest.AddStateL(new (ELeave) CPop3SetSyncLimits(inboxSyncLimit,iPop3ClientTest), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CPop3OperationParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandPop3Connect);
	AddCommandL(KCommandPop3ConnectAndMonitor);
	AddCommandL(KCommandPop3Disconnect);
	AddCommandL(KCommandPop3UseService);
	AddCommandL(KCommandPop3SetSyncLimits);
	}

CPop3OperationParser::CPop3OperationParser(CPop3ClientTest& aPop3ClientTest) : iPop3ClientTest(aPop3ClientTest)
	{
	}




//
//
// CPop3CheckStatusParser
//


CPop3CheckStatusParser* CPop3CheckStatusParser::NewL(CPop3ClientTest& aPop3TestHarness)
	{
	CPop3CheckStatusParser* self = new (ELeave) CPop3CheckStatusParser(aPop3TestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CPop3CheckStatusParser::ProcessL()
	{
	TLex lex((*iArgumentList)[0]);
	TInt expectedStatus;
	User::LeaveIfError(lex.Val(expectedStatus));
	iTestHarness.AddStateL(new (ELeave) CCheckPop3Operation(expectedStatus, iTestHarness), iDebugInfo);
	}

CPop3CheckStatusParser::CPop3CheckStatusParser(CPop3ClientTest& aPop3TestHarness) : iTestHarness(aPop3TestHarness)
	{
	}

void CPop3CheckStatusParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandPop3CheckStatus);
	}

