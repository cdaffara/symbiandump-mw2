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

#ifndef __IMAPTESTPARSERS_H
#define __IMAPTESTPARSERS_H

#include "emailsmokeparse.h"
#include "emailsmokecommands.h"
#include "imaptests.h"

//
//
// CNewImapTestCommandParser
//

class CNewImapTestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewImapTestCommandParser* NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest);
private:
	void ConstructL();
	CNewImapTestCommandParser(CTestScript& aScript, CEmailClientTest& CEmailClientTest);
	CEmailClientTest& iEmailClientTest;
	CTestScript& iScript;
	};



//
//
// CImapClientSectionParser
//

class CImapClientSectionParser : public CEmailClientSectionParser
	{
public:
	static CImapClientSectionParser* NewL(CImapClientTest& aImapTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void ConstructL(const TDesC& aNewSectionName);
	CImapClientSectionParser(CImapClientTest& aImapTest, CTestScript& aScript);

private:
	CImapClientTest& iImapTest;
	};



//
//
// CImapServiceCommandParser
//

class CImapServiceCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CImapServiceCommandParser* NewL(CTestScript& aScript, CImapClientTest& aImapClientTest);
private:
	void ConstructL();
	CImapServiceCommandParser(CTestScript& aScript, CImapClientTest& aImapClientTest);
	CImapClientTest& iImapClientTest;
	CTestScript& iScript;
	};



//
//
// CImapServiceSectionParser
//

class CImImapSettings;

class CImapServiceSectionParser : public CEmailServiceSectionParser
	{
public:
	static CImapServiceSectionParser* NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName);
	~CImapServiceSectionParser();

protected:
	void ConstructL(const TDesC& aSectionName);
	CImapServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness);
	};



//
//
// CImapServiceCommands
//

class CCreateImapService;

class CImapServiceCommands : public CBaseCommandParser
	{
public:
	static CImapServiceCommands* NewL(CCreateImapService& aTestState);
	void ProcessL();

private:
	CImapServiceCommands(CCreateImapService& aTestState);
	void ConstructL();

private:
	CCreateImapService& iTestState;
	};




//
//
// CImapOperationParser
//

class CImapOperationParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CImapOperationParser* NewL(CImapClientTest& iImapClientTest);
private:
	void ConstructL();
	CImapOperationParser(CImapClientTest& aImapClientTest);
	CImapClientTest& iImapClientTest;
	};





//
//
// CImapCheckStatusParser
//

class CImapCheckStatusParser : public CBaseCommandParser
	{
public:
	static CImapCheckStatusParser* NewL(CImapClientTest& aImapTestHarness);
	void ProcessL();

private:
	CImapCheckStatusParser(CImapClientTest& aImapTestHarness);
	void ConstructL();

private:
	CImapClientTest& iTestHarness;
	};

#endif

