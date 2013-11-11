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

#ifndef __SMTPTESTPARSERS_H
#define __SMTPTESTPARSERS_H

#include "emailsmokeparse.h"
#include "smtptests.h"

//
//
// CNewSmtpTestCommandParser
//

class CNewSmtpTestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewSmtpTestCommandParser* NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest);
private:
	void ConstructL();
	CNewSmtpTestCommandParser(CTestScript& aScript, CEmailClientTest& CEmailClientTest);
	CEmailClientTest& iEmailClientTest;
	CTestScript& iScript;
	};



//
//
// CSmtpServiceCommandParser
//

class CSmtpServiceCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CSmtpServiceCommandParser* NewL(CTestScript& aScript, CSmtpClientTest& aSmtpClientTest);
private:
	void ConstructL();
	CSmtpServiceCommandParser(CTestScript& aScript, CSmtpClientTest& aSmtpClientTest);
	CSmtpClientTest& iSmtpClientTest;
	CTestScript& iScript;
	};




//
//
// CSmtpClientSectionParser
//

class CSmtpClientSectionParser : public CEmailClientSectionParser
	{
public:
	static CSmtpClientSectionParser* NewL(CSmtpClientTest& aSmtpTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void ConstructL(const TDesC& aNewSectionName);
	CSmtpClientSectionParser(CSmtpClientTest& aSmtpTest, CTestScript& aScript);

private:
	CSmtpClientTest& iSmtpTest;
	};



//
//
// CSmtpServiceSectionParser
//

class CImSmtpSettings;

class CSmtpServiceSectionParser : public CEmailServiceSectionParser
	{
public:
	static CSmtpServiceSectionParser* NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName);
	~CSmtpServiceSectionParser();

protected:
	void ConstructL(const TDesC& aSectionName);
	CSmtpServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness);
	};



//
//
// CSmtpServiceCommands
//

class CCreateSmtpService;

class CSmtpServiceCommands : public CBaseCommandParser
	{
public:
	static CSmtpServiceCommands* NewL(CCreateSmtpService& aTestState);
	void ProcessL();

private:
	CSmtpServiceCommands(CCreateSmtpService& aTestState);
	void ConstructL();

private:
	CCreateSmtpService& iTestState;
	};



//
//
// CSmtpCheckStatusParser
//


class CSmtpCheckStatusParser : public CBaseCommandParser
	{
public:
	static CSmtpCheckStatusParser* NewL(CSmtpClientTest& aSmtpTestHarness);
	void ProcessL();

private:
	CSmtpCheckStatusParser(CSmtpClientTest& aSmtpTestHarness);
	void ConstructL();

private:
	CSmtpClientTest& iTestHarness;
	};


//
//
// CSmtpOperationParser
//

class CSmtpOperationParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CSmtpOperationParser* NewL(CSmtpClientTest& iSmtpClientTest);
private:
	void ConstructL();
	CSmtpOperationParser(CSmtpClientTest& aSmtpClientTest);
	CSmtpClientTest& iSmtpClientTest;
	};

#endif

