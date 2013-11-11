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

#include "smtptestparsers.h"
#include "smtptests.h"

_LIT(KCommandSmtpClientTest, "smtp_client_test");


// SMTP commands
_LIT(KCommandSmtpService, "smtp_service");

// SMTP settings strings
_LIT(KCommandEmailSetServer, "set_server");
_LIT(KCommandEmailSetAlias, "set_alias");
_LIT(KCommandEmailSetAddress, "set_email_address");
_LIT(KCommandEmailSetReplyAddress, "set_reply_address");
_LIT(KCommandEmailSetReceiptAddress, "set_receipt_address");
_LIT(KCommandSmtpSetTLS,			"set_tls");				// 1 parameter, sets the secure connection for SMTP service

_LIT(KCommandSmtp3SetWrappedTLS,  "set_wrapped_tls");	    // 1 parameter, sets the secure connection for SMTP service.  value should be 1 or 0
_LIT(KCommandSmtpSetPort,  "set_port");	                    // 1 parameter, sets the port no for SMTP service.


_LIT(KCommandSmtpCheckStatus, "check_smtp_error");


_LIT(KCommandSmtpCopyAndMonitor,			"copy_and_monitor");	// 0 parameters, copies and monitors SMTP service

// Smtp Operations
_LIT(KCommandSmtpUseService,		"use_service");	// 0 parameters, uses the currently selected service


//
//
// CNewSmtpTestCommandParser
//

// A command parser
// Parses the command which generates a new SMTP test harness
// This command would usually be found in an email test section

CNewSmtpTestCommandParser* CNewSmtpTestCommandParser::NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest)
	{
	CNewSmtpTestCommandParser* self = new (ELeave) CNewSmtpTestCommandParser(aScript, aEmailClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	};

void CNewSmtpTestCommandParser::ProcessL()
	{
	// Create an smtp test, give it the test utilities
	CSmtpClientTest* smtpTest = CSmtpClientTest::NewL(iEmailClientTest.EmailTestUtils(), iEmailClientTest.TestConsole());
	CleanupStack::PushL(smtpTest);

	// Attach the email test to the main test
	iEmailClientTest.AddStateL(smtpTest, iDebugInfo);
	CleanupStack::Pop(smtpTest); // emailTest is now owned by the iMainTest

	// Check that there is one argument, the email test section name
	CheckNumberOfArgumentsL(1);
	// Create an email section parser
	CSmtpClientSectionParser* sectionParser = CSmtpClientSectionParser::NewL(*smtpTest, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewSmtpTestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSmtpClientTest);
	}

CNewSmtpTestCommandParser::CNewSmtpTestCommandParser(CTestScript& aScript, CEmailClientTest& aEmailClientTest) : iEmailClientTest(aEmailClientTest), iScript(aScript)
	{
	}




//
//
// CSmtpClientSectionParser
//

// A section parser
// Parses an SMTP test section

CSmtpClientSectionParser* CSmtpClientSectionParser::NewL(CSmtpClientTest& aSmtpClientTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CSmtpClientSectionParser* self = new (ELeave) CSmtpClientSectionParser(aSmtpClientTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CEmailClientSectionParser::ConstructL(aNewSectionName);
	AddCommandParserL(CSmtpServiceCommandParser::NewL(iScript, iSmtpTest));
	AddCommandParserL(CSmtpCheckStatusParser::NewL(iSmtpTest));
	AddCommandParserL(CSmtpOperationParser::NewL(iSmtpTest));	
	}

CSmtpClientSectionParser::CSmtpClientSectionParser(CSmtpClientTest& aSmtpTest, CTestScript& aScript) : CEmailClientSectionParser(aSmtpTest, aScript), iSmtpTest(aSmtpTest)
	{
	}



//
//
// CSmtpServiceCommandParser
//

void CSmtpServiceCommandParser::ProcessL()
	{
	CheckNumberOfArgumentsL(1);
	// Create the settings object which we will give to the settings section parser
	CSmtpServiceSectionParser* serviceParser = CSmtpServiceSectionParser::NewL(iScript, iSmtpClientTest, (*iArgumentList)[0]);
	CleanupStack::PushL(serviceParser);
	serviceParser->ParseL();
	CleanupStack::PopAndDestroy(serviceParser);
	}

CSmtpServiceCommandParser* CSmtpServiceCommandParser::NewL(CTestScript& aScript, CSmtpClientTest& aSmtpClientTest)
	{
	CSmtpServiceCommandParser* self = new (ELeave) CSmtpServiceCommandParser(aScript, aSmtpClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpServiceCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSmtpService);
	}

CSmtpServiceCommandParser::CSmtpServiceCommandParser(CTestScript& aScript, CSmtpClientTest& aSmtpClientTest) : iSmtpClientTest(aSmtpClientTest), iScript(aScript)
	{
	}


//
//
// CSmtpServiceSectionParser
//

// A section parser
// Parses a SMTP section to create a SMTP service

CSmtpServiceSectionParser* CSmtpServiceSectionParser::NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName)
	{
	CSmtpServiceSectionParser* self = new (ELeave) CSmtpServiceSectionParser(aScript, aTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL(aSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpServiceSectionParser::ConstructL(const TDesC& aSectionName)
	{
	CBaseSectionParser::ConstructL(aSectionName);

	// Add the state to generate the smtp service
	CCreateSmtpService* testState = CCreateSmtpService::NewL(iTestHarness);
	TTestDebugInfo debugInfo(iScript, iSection->SectionPosition(), 0);
	iTestHarness.AddStateL(testState, debugInfo);

	// The parsers will fill in the entry and service details before the state that creates the entry is run.
	AddCommandParserL(CSmtpServiceCommands::NewL(*testState));

	CEmailServiceSectionParser::ConstructL(aSectionName, *testState);
	}

CSmtpServiceSectionParser::CSmtpServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness) : CEmailServiceSectionParser(aScript, aTestHarness)
	{
	}

CSmtpServiceSectionParser::~CSmtpServiceSectionParser()
	{
	}



//
//
// CSmtpServiceCommands
//

// A command parser
// Parses the commands that generate the SMTP service

CSmtpServiceCommands* CSmtpServiceCommands::NewL(CCreateSmtpService& aTestState)
	{
	CSmtpServiceCommands* self = new (ELeave) CSmtpServiceCommands(aTestState);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpServiceCommands::ProcessL()
	{
	// eh xxxx, check args
	if ((*iCurrentCommand) == KCommandEmailSetServer)
		{
		iTestState.Settings().SetServerAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandEmailSetAlias)
		{
		iTestState.Settings().SetEmailAliasL((*iArgumentList)[0]);
		iTestState.SetDetailsStringL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandEmailSetAddress)
		{
		iTestState.Settings().SetEmailAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandEmailSetReplyAddress)
		{
		iTestState.Settings().SetReplyToAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandEmailSetReceiptAddress)
		{
		iTestState.Settings().SetReceiptAddressL((*iArgumentList)[0]);
		}
	else if ((*iCurrentCommand) == KCommandSmtpSetTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setTls=0;
		User::LeaveIfError(lex.Val(setTls));
		iTestState.Settings().SetSecureSockets(setTls);
		}
	else if ((*iCurrentCommand) == KCommandSmtp3SetWrappedTLS)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setWrappedTls=0;
		User::LeaveIfError(lex.Val(setWrappedTls));
		iTestState.Settings().SetSSLWrapper(setWrappedTls);
		}
	else if ((*iCurrentCommand) == KCommandSmtpSetPort)
		{
		
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt setPort=0;
		User::LeaveIfError(lex.Val(setPort));
		iTestState.Settings().SetPort(setPort);
		
		}
	}

CSmtpServiceCommands::CSmtpServiceCommands(CCreateSmtpService& aTestState) : iTestState(aTestState)
	{
	}

void CSmtpServiceCommands::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandEmailSetServer);
	AddCommandL(KCommandEmailSetAlias);
	AddCommandL(KCommandEmailSetAddress);
	AddCommandL(KCommandEmailSetReplyAddress);
	AddCommandL(KCommandEmailSetReceiptAddress);
	AddCommandL(KCommandSmtpSetTLS);
	AddCommandL(KCommandSmtp3SetWrappedTLS);
	AddCommandL(KCommandSmtpSetPort);
	}





//
//
// CSmtpCheckStatusParser
//


CSmtpCheckStatusParser* CSmtpCheckStatusParser::NewL(CSmtpClientTest& aSmtpTestHarness)
	{
	CSmtpCheckStatusParser* self = new (ELeave) CSmtpCheckStatusParser(aSmtpTestHarness);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpCheckStatusParser::ProcessL()
	{
	TLex lex((*iArgumentList)[0]);
	TInt expectedStatus;
	User::LeaveIfError(lex.Val(expectedStatus));
	iTestHarness.AddStateL(new (ELeave) CCheckSmtpOperation(expectedStatus, iTestHarness), iDebugInfo);
	}

CSmtpCheckStatusParser::CSmtpCheckStatusParser(CSmtpClientTest& aSmtpTestHarness) : iTestHarness(aSmtpTestHarness)
	{
	}

void CSmtpCheckStatusParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSmtpCheckStatus);
	}


//
//
// CSmtpOperationParser
//

CSmtpOperationParser* CSmtpOperationParser::NewL(CSmtpClientTest& aSmtpClientTest)
	{
	CSmtpOperationParser* self = new (ELeave) CSmtpOperationParser(aSmtpClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CSmtpOperationParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandSmtpUseService)
		{
		CheckNumberOfArgumentsL(0);
		iSmtpClientTest.AddStateL(new (ELeave) CSmtpUseService(iSmtpClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSmtpCopyAndMonitor)
		{
		CheckNumberOfArgumentsL(0);
		iSmtpClientTest.AddStateL(new (ELeave) CCopySelectionAndMonitor(iSmtpClientTest, EFalse), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CSmtpOperationParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSmtpUseService);
	AddCommandL(KCommandSmtpCopyAndMonitor);
	}

CSmtpOperationParser::CSmtpOperationParser(CSmtpClientTest& aSmtpClientTest) : iSmtpClientTest(aSmtpClientTest)
	{
	}

