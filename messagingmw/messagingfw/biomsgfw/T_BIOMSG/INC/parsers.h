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

#ifndef __PARSERS_H
#define __PARSERS_H

#include "testframeparse.h"
#include "harness.h"

//
// This class parses the main section
class CBioMainSectionParser : public CBaseSectionParser
	{
public:
	static CBioMainSectionParser* NewL(CMainTestHarness& aMainTest, CTestScript& aScript);

protected:
	void ConstructL();
	inline CBioMainSectionParser(CMainTestHarness& aMainTest, CTestScript& aScript) : CBaseSectionParser(aScript), iMainTest(aMainTest) {}
	void LogComment(const TDesC& aComment);

private:
	CMainTestHarness& iMainTest;
};

//
// This class parses the bio_client_test command in the main section
class CBioMainCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioMainCommandsParser* NewL(CTestScript& aScript, CMainTestHarness& aMainTest);

private:
	void ConstructL();
	inline CBioMainCommandsParser(CTestScript& aScript, CMainTestHarness& aMainTest) : iMainTest(aMainTest), iScript(aScript) {}
	CMainTestHarness& iMainTest;
	CTestScript& iScript;
};

//
// This class parses the client secion specified in the bio_client_test cmd
class CBioClientSectionParser : public CBaseSectionParser
{
public:
	static CBioClientSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioClientSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class parses the commands in the client secion
class CBioClientCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioClientCommandsParser* NewL(CClientTestHarness& aMainTest);

private:
	void ConstructL();
	inline CBioClientCommandsParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	CClientTestHarness& iClientTest;
};

//
// This class parses the SmsSettings section
class CBioSmsSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioSmsSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioSmsSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class handles the Sms commands
class CBioSmsSettingsCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioSmsSettingsCommandsParser* NewL(CClientTestHarness& aMainTest);
private:
	void ConstructL();
	inline CBioSmsSettingsCommandsParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	CClientTestHarness& iClientTest;	
};

//
// This class parses the Pop3Settings section
class CBioPop3SettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioPop3SettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioPop3SettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class parses the Pop3Settings section
class CBioPop3NamedSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioPop3NamedSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName, const TDesC& aSettingsName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName, const TDesC& aSettingsName);
	inline CBioPop3NamedSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class handles the POP3 commands
class CBioPop3SettingsCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioPop3SettingsCommandsParser* NewL(CClientTestHarness& aMainTest);
private:
	void ConstructL();
	inline CBioPop3SettingsCommandsParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	CClientTestHarness& iClientTest;	
};

//
// This class parses the SMTPSettings section
class CBioSmtpSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioSmtpSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioSmtpSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class parses the SmtpSettings section
class CBioSmtpNamedSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioSmtpNamedSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName, const TDesC& aSettingsName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName, const TDesC& aSettingsName);
	inline CBioSmtpNamedSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};


//
// This class handles the SMTP commands
class CBioSmtpSettingsCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioSmtpSettingsCommandsParser* NewL(CClientTestHarness& aMainTest);
private:
	void ConstructL();
	inline CBioSmtpSettingsCommandsParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	CClientTestHarness& iClientTest;	
};


//
// This class parses the ImapSettings section
class CBioImapSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioImapSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioImapSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};



//
// This class parses the ImapSettings section
class CBioImapNamedSettingsSectionParser : public CBaseSectionParser
{
public:
	static CBioImapNamedSettingsSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName, const TDesC& aSettingsName);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName, const TDesC& aSettingsName);
	inline CBioImapNamedSettingsSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};


//
// This class handles the Imap commands
class CBioImapSettingsCommandsParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioImapSettingsCommandsParser* NewL(CClientTestHarness& aMainTest);
private:
	void ConstructL();
	inline CBioImapSettingsCommandsParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	CClientTestHarness& iClientTest;	
};

//
// This class parses the CommDB section
class CBioCommDBSectionParser : public CBaseSectionParser
{
public:
	static CBioCommDBSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aTableName, const TDesC& aNewSectionName);
	void SectionFinishedL(TTestDebugInfo& aDebugInfo);

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aTableName, const TDesC& aNewSectionName);
	inline CBioCommDBSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
};

//
// This class handles the CommDB commands
class CBioCommDBCommandParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioCommDBCommandParser* NewL(CClientTestHarness& aMainTest, const TDesC& aTableName);
	void SectionFinishedL(TTestDebugInfo& aDebugInfo);

private:
	void ConstructL();
	inline CBioCommDBCommandParser(CClientTestHarness& aClientTest, const TDesC& aTableName) : iClientTest(aClientTest), iTableName(aTableName) {}

private:
	CClientTestHarness& iClientTest;
	TBuf<50> iTableName;
};

//
// This class parses the gen_msg_from_section section
class CBioGenMsgCommandParser;
class CBioGenMsgSectionParser : public CBaseSectionParser
{
public:
	static CBioGenMsgSectionParser* NewL(CClientTestHarness& aTest, CTestScript& aScript, const TDesC& aNewSectionName);
	void SectionFinishedL();

protected:
	void LogComment(const TDesC& aComment);
	void ConstructL(const TDesC& aNewSectionName);
	inline CBioGenMsgSectionParser(CClientTestHarness& aTest, CTestScript& aScript) : CBaseSectionParser(aScript), iClientTest(aTest), iScript(aScript) {}

private: 
	CClientTestHarness& iClientTest;
	CTestScript& iScript;
	CBioGenMsgCommandParser* iCmdParser;	// Created, but NOT owned by us.
};

//
// This class parses the commands in the gen_msg_from_section section
class CBioGenMsgCommandParser : public CBaseCommandParser
{
public:
	void ProcessL();
	static CBioGenMsgCommandParser* NewL(CClientTestHarness& aMainTest);
	void SectionFinishedL();

private:
	void ConstructL();
	inline CBioGenMsgCommandParser(CClientTestHarness& aClientTest) : iClientTest(aClientTest) {}
	inline ~CBioGenMsgCommandParser() { delete iBuffer; }

private:
	CClientTestHarness& iClientTest;
	HBufC* iBuffer;
	TBIOMessageType iMsgType;
};

#endif
