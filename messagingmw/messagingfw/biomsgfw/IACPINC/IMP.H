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
// Internet Mail Parser
// 
//

#ifndef __IMP_H__
#define __IMP_H__

#include <imapset.h>     // TFolderSyncType, TFolderSubscribeType
#include <pop3set.h>
#include <smtpset.h> 
#include <cemailaccounts.h>

#include "IMPMACRO.H"


class CMailParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	enum TMailProtocolType //i.e POP3, IMAP, SMTP, etc...
		{
		ESmtp=1,
		EPop3,
		EImap4
		};

public:
	static CMailParser* NewLC();
	static CMailParser* NewL();
	~CMailParser();

	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CMSVENTRY& aEntry);
	void CheckMandatoryFieldsL(CParsedFieldCollection& aIacpFields);
	void AssociateIAPWithMailL(CMSVENTRY& aEntry, TUint32 aId);

private:
	CMailParser();
	void ConstructL();

	void GetServiceIdsL(TPopAccount& aPop3AccountId, TImapAccount& aImap4AccountId);
	void GetFolderSyncTypeL(const TDesC& aDes,TFolderSyncType& aFolderSyncType) const;
	void GetFolderSubscribeTypeL(const TDesC& aDes,TFolderSubscribeType& aFolderSubscribeType) const;

	void SetMailProtocolL(CParsedFieldCollection& aIacpFields);

private:
	TInt								iProtocolName;
	TBuf<KMaxSettingStringLength>		iIspName;

	CImSmtpSettings*	iSmtpSettings;
	CImPop3Settings*	iPop3Settings;
	CImImap4Settings*	iImap4Settings;

	CEmailAccounts*		iEmailAccounts;
	CImIAPPreferences* 	iEmailPreferences;
	CImIAPPreferences* 	iSmtpPreferences;
	
	TPopAccount 		iPop3AccountId;
	TImapAccount 		iImap4AccountId;
	};
	
	
#endif // __IMP_H__  
