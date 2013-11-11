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

#ifndef __EMAILSMOKEPARSE_H
#define __EMAILSMOKEPARSE_H

#include "testframeparse.h"
#include "emailsmoke.h"

//
//
// CMainSectionParser
//

class CMainSectionParser : public CBaseSectionParser
	{
public:
	static CMainSectionParser* NewL(CMainTestHarness& aMainTest, CTestScript& aScript,TBool aKeepLogFile);

protected:
	void ConstructL();
	CMainSectionParser(CMainTestHarness& aMainTest, CTestScript& aScript,TBool aKeepLogFile);
	void LogComment(const TDesC& aComment);

private:
	CMainTestHarness& iMainTest;
	TBool iKeepLogFile;
	};



//
//
// CEmailClientSectionParser
//

class CEmailClientSectionParser : public CBaseSectionParser
	{
public:
	static CEmailClientSectionParser* NewL(CEmailClientTest& aEmailClientTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	CEmailClientSectionParser(CEmailClientTest& aEmailTest, CTestScript& aScript);

private:
	CEmailClientTest& iEmailTest;
	};


//
//
// CNewEmailTestCommandParser
//

class CNewEmailTestCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CNewEmailTestCommandParser* NewL(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile);
private:
	void ConstructL();
	CNewEmailTestCommandParser(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile);
	CMainTestHarness& iMainTest;
	CTestScript& iScript;
	TBool iKeepLogFile;
	};





//
//
// CMsvUtilitiesParser
//

class CMsvUtilitiesParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CMsvUtilitiesParser* NewL(CMsvClientTest& iMsvClientTest);
private:
	void ConstructL();
	CMsvUtilitiesParser(CMsvClientTest& aMsvClientTest);
	CMsvClientTest& iMsvClientTest;
	};




//
//
// CMsvSelectParser
//

class CMsvSelectParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CMsvSelectParser* NewL(CMsvClientTest& iMsvClientTest);
private:
	void ConstructL();
	CMsvSelectParser(CMsvClientTest& aMsvClientTest);
	CMsvClientTest& iMsvClientTest;
	};



//
//
// CEmailUtilitiesParser
//

class CEmailUtilitiesParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CEmailUtilitiesParser* NewL(CEmailClientTest& aEmailClientTest);
private:
	void ConstructL();
	CEmailUtilitiesParser(CEmailClientTest& aEmailClientTest);
	CEmailClientTest& iEmailClientTest;
	};



//
//
// CMsvOperationParser
//

class CMsvOperationParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CMsvOperationParser* NewL(CMsvClientTest& iMsvClientTest);
private:
	void ConstructL();
	CMsvOperationParser(CMsvClientTest& aMsvClientTest);
	CMsvClientTest& iMsvClientTest;
	};



//
//
// CEmailServiceSectionParser
//

class CCreateEmailService;

class CEmailServiceSectionParser : public CBaseSectionParser
	{
public:
	~CEmailServiceSectionParser();
	void LogComment(const TDesC& aComment);

protected:
	void ConstructL(const TDesC& aSectionName, CCreateEmailService& aCreateServiceState);
	CEmailServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness);

protected:
	TMsvEntry iEntry;
	CEmailClientTest& iTestHarness;
	};


//
//
// CEmailServiceCommands
//

class CCreateEmailService;

class CEmailServiceCommands : public CBaseCommandParser
	{
public:
	void ProcessL();
	TInt FindIAPByNameL();
	static CEmailServiceCommands* NewL(CCreateEmailService& aTestState);

private:
	void ConstructL();
	CEmailServiceCommands(CCreateEmailService& aTestState);

private:
	CCreateEmailService& iCreateEmailServiceState;
	};


//
//
// CMsvTestEntryCommandParser
//

class CMsvTestEntry;

class CMsvTestEntryCommandParser : public CBaseCommandParser
	{
public:
	void ProcessL();
	static CMsvTestEntryCommandParser* NewL(CMsvTestEntry& aTestEntry);

private:
	void ConstructL();
	CMsvTestEntryCommandParser(CMsvTestEntry& aTestEntry);

	TBool GetBooleanArgumentL(); // Leaves if arguments consist of anything apart for a single boolean

private:
	CMsvTestEntry& iTestEntry;
	};

//
//
// CMsvTestEntrySectionParser
//

class CMsvTestEntrySectionParser : public CBaseSectionParser
	{
public:
	static CMsvTestEntrySectionParser* NewL(const TDesC& aSectionName, CTestScript& aScript, CMsvTestEntry& aTestState);
	~CMsvTestEntrySectionParser();
	void LogComment(const TDesC& aComment);

protected:
	void ConstructL(const TDesC& aSectionName);
	CMsvTestEntrySectionParser(CTestScript& aScript, CMsvTestEntry& aTestState);

protected:
	CMsvTestEntry& iTestState;
	};



#endif
