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

#ifndef __POPTESTPARSERS_H
#define __POPTESTPARSERS_H

#include "emailsmokeparse.h"
#include "poptests.h"

//
//
// CNewPop3TestCommandParser
//

class CNewPop3TestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewPop3TestCommandParser* NewL(CTestScript& aScript, CEmailClientTest& aEmailClientTest);
private:
	void ConstructL();
	CNewPop3TestCommandParser(CTestScript& aScript, CEmailClientTest& CEmailClientTest);
	CEmailClientTest& iEmailClientTest;
	CTestScript& iScript;
	};



//
//
// CPop3ClientSectionParser
//

class CPop3ClientSectionParser : public CEmailClientSectionParser
	{
public:
	static CPop3ClientSectionParser* NewL(CPop3ClientTest& aPop3Test, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void ConstructL(const TDesC& aNewSectionName);
	CPop3ClientSectionParser(CPop3ClientTest& aPop3Test, CTestScript& aScript);

private:
	CPop3ClientTest& iPop3Test;
	};



//
//
// CPop3ServiceCommandParser
//

class CPop3ServiceCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CPop3ServiceCommandParser* NewL(CTestScript& aScript, CPop3ClientTest& aPop3ClientTest);
private:
	void ConstructL();
	CPop3ServiceCommandParser(CTestScript& aScript, CPop3ClientTest& aPop3ClientTest);
	CPop3ClientTest& iPop3ClientTest;
	CTestScript& iScript;
	};



//
//
// CPop3ServiceSectionParser
//

class CImPop3Settings;

class CPop3ServiceSectionParser : public CEmailServiceSectionParser
	{
public:
	static CPop3ServiceSectionParser* NewL(CTestScript& aScript, CEmailClientTest& aTestHarness, const TDesC& aSectionName);
	~CPop3ServiceSectionParser();

protected:
	void ConstructL(const TDesC& aSectionName);
	CPop3ServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness);
	};



//
//
// CPop3ServiceCommands
//

class CCreatePop3Service;

class CPop3ServiceCommands : public CBaseCommandParser
	{
public:
	static CPop3ServiceCommands* NewL(CCreatePop3Service& aTestState);
	void ProcessL();

private:
	CPop3ServiceCommands(CCreatePop3Service& aTestState);
	void ConstructL();

private:
	CCreatePop3Service& iTestState;
	};




//
//
// CPop3OperationParser
//

class CPop3OperationParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CPop3OperationParser* NewL(CPop3ClientTest& iPop3ClientTest);
private:
	void ConstructL();
	CPop3OperationParser(CPop3ClientTest& aPop3ClientTest);
	CPop3ClientTest& iPop3ClientTest;
	};





//
//
// CPop3CheckStatusParser
//

class CPop3CheckStatusParser : public CBaseCommandParser
	{
public:
	static CPop3CheckStatusParser* NewL(CPop3ClientTest& aPop3TestHarness);
	void ProcessL();

private:
	CPop3CheckStatusParser(CPop3ClientTest& aPop3TestHarness);
	void ConstructL();

private:
	CPop3ClientTest& iTestHarness;
	};




#endif

