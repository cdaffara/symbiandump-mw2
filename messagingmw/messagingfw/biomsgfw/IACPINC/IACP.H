// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Internet Access Configuration Parser
// 
//

 
#ifndef __IACP_H__
#define __IACP_H__

#include <bsp.h>

_LIT(KIACP, "IACP-DLL");
// Names for the different kinds of settings
_LIT(KINTERNET, "Iname");
_LIT(KMAIL, "Mname");
_LIT(KHOTLIST, "Hname");
_LIT(KSCRIPT, "Pname");
_LIT(KSMS, "Sname");
_LIT(KTELNET, "Tname");
_LIT(KTERMINAL, "Rname");
_LIT(KFTP, "Fname");
_LIT(KTELEPHONE, "TBox"); // voice mailbox number
_LIT(KGPRS, "Gname");
_LIT(KWWWHotlist, "Hname");

_LIT(KIacpIfName,		"ppp");
_LIT(KIacpIfNetworks,	"ip,ip6");

#define KBIO_MSG_ENTRY_PARSED		1 // Set entry .iMtmData3 to 1 to indicate that the entry has store i.e parsed,externalised
#define KBIO_MSG_ENTRY_PROCESSED	2 // Set TMsvEntry iMtmData3 = 2 to indicate that a message has been processed

//forward declarations
class CBaseScriptParser2;
class CRegisteredParserDll;
class CMailParser;
class CIspParser;
class CScriptParser;
class CSmsParser;
class CGprsParser;
class CTelVoiceMailboxParser;
class CWWWHotlistParser;
class CRichText;
class CParsedFieldCollection;


class CIacSettingsParser : public CBaseScriptParser2
/**
@internalComponent
@released
*/
	{
public:
	IMPORT_C static CIacSettingsParser* NewL(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
	~CIacSettingsParser();

	void ParseL(TRequestStatus& aStatus,const TDesC& aSms);
	void ProcessL(TRequestStatus& aStatus);
	//Utility functions
	CArrayPtrSeg<CParsedField>& ParsedFieldArray() const;
	void SetFieldValueL(const TDesC& aFieldName, const TDesC& aFieldValue);
	TBool GetMandatoryField(const TDesC& aFieldName) const;
	
	enum TSmsType
		{
		ESmsTypeUndefined,
		EBasicIAP,    //Supported
		EBasicMail,		//Supported
		EBasicMailIAP, // combined basic Email and Internet settings
		EExtendedIAP, //Supported
		EExtendedMail,
		EExtWWWHostListItem, // Supported
		EExtScriptSettings, //Supported
		EExtSmsSettings,	//Supported
		EExtGprsSettings,		//Supported
		EExtTelnetSettings,
		EExtTerminalSettings,
		EExtWWWSettings,		
		EExtTTMLSettings,
		EExtFTPSettings,
		EExtInternetSettings,
		EExtTelephoneSettings, //Supported
		EExtWWWAutofetchSettings,
		EExtGprsIAP,				// combined IAP and GPRS
		EExtGprsMail,				// combined mail & GPRS
		EExtGprsIAPMail,			// combined GPRS, IAP and mail
		ESmsMixedContent
		};

	enum TParseSession
		{
		EParseSms=1,
		ECheckData,
		ESetParsersData,
		EProcessSms
		};

	// flags indicating whether to attempt to create
	// a connection preference for GPRS or ISP settings
	enum TIacpConnectionPreferenceFlag
		{
		EIacpDoNotCreatePreference,
		EIacpAttempCreateAsFirstRank,
		EIacpAttempCreateAsSecondRank
		};

private:
	CIacSettingsParser(CRegisteredParserDll& aRegisteredParserDll, CMsvEntry& aEntry, RFs& aFs);
	void ConstructL();
	void ChangeStateL(TParseSession aState);

	void ParseSmsL();		//Parsers sms data into an array of CParsedField
	void DoParseL();
	void DoProcessL();

	void GetPtrToScriptDataL(HBufC*& aPtrScriptData);
	TBool SetBioMsgTypeL(const TDesC& aLeftToken, TSmsType& aType);
	TBool IsSmsExtendedIAP();
	TBool isMandatoryData(const TDesC& aFieldName);
	void CheckMandatoryDataL();

	// set & store/restore data
	void SetDataL();
	void SetParsersDataL();
	void StoreParsedDataL();
	void RestoreParsedDataL();

	void SetDataFieldsL(CParsedField& aParsedField, const TDesC& aName, const TDesC& aValue, TBool aBool);
	TBool IsValidToken(const TDesC& aToken);

	void RequestComplete(TRequestStatus& aStatus,TInt aCompletion);

	void PopulateFieldCollectionsL();
	TInt CompleteMessage();

	//CActive
	void DoCancel();
	void RunL();
	TInt RunError(TInt aError);

private:
	TBool									iSmsParsed;
	TInt									iState;
	TSmsType								iSmsType;
	TSmsType								iCurrentlyProcessing;

	HBufC*									iSmsBuf;
	CMailParser*							iMailParser;  //Mail Parser i.e IMAP, POP3, SMTP
	CIspParser*								iIspParser;   //Internet Service Provided Parser i.e.Dial Out DB
	CScriptParser*							iScriptParser;
	CSmsParser*								iSmsParser;   //SMS settings parser
	CTelVoiceMailboxParser* 				iTelVoiceMailboxParser;
	CGprsParser*							iGprsParser;
	CWWWHotlistParser*						iWWWHotlistParser;
	TRequestStatus*							iReport;

	CArrayPtrSeg<CParsedFieldCollection>* 	iParsedFieldCollections;
	TInt									iCurrentSettingsCtr;
	TIacpConnectionPreferenceFlag			iConnectionPreference;

	TUint32									iDefaultIAP;
	TUint32									iDefaultMail;
	};

class CParsedFieldCollection : public CArrayPtrSeg<CParsedField>
/**
@internalComponent
@released
*/
	{
public:
	CParsedFieldCollection(CIacSettingsParser::TSmsType aSmsType);
	CIacSettingsParser::TSmsType MessageType() { return iSmsType; };

	TPtrC GetFieldValue(const TDesC& aFieldName) const;
	TInt GetFieldValueAndLength(const TDesC& aFieldName, TPtrC& aValue) const;

	void GetTBoolL(const TDesC& aDes, TBool& aBool) const;
	void GetTUint32NumL(const TDesC& aDes, TUint32& aTUint32Value) const;

	TBool FieldNameExists(const TDesC& aFieldName) const;

	// Used in runl to associate iap and mail settings

public:
	TBool									iCanCommit;
private:
	CIacSettingsParser::TSmsType			iSmsType;
	};

#endif
