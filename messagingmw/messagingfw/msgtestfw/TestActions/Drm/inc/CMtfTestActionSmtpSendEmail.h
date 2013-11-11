// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

//! @file
//! @internalComponent



#ifndef CMTFTESTACTIONSMTPSENDEMAIL_H
#define CMTFTESTACTIONSMTPSENDEMAIL_H


#include "CMtfSynchronousTestAction.h"
#include <imsk.h>


_LIT( KTestActionSmtpSendEmail,"SmtpSendEmail");

class CMtfTestActionSmtpSendEmail : public CMtfTestAction
{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSmtpSendEmail();

protected:
	void DoCancel();
	void RunL();

private:
	enum TSmtpSendState
	{
		ESmtpNotConnected,
		ESmtpConnecting,
		ESmtpSendHELO,
		ESmtpSendMailFrom,
		ESmtpSendRcptTo,
		ESmtpSendData,
		ESmtpSendContent,
		ESmtpSendEndData,
		ESmtpSendLogout,
		ESmtpReceiveReply
	};

	enum TSmtpReceiveState
	{
		ESmtpHeloReply,
		ESmtpFromReply,
		ESmtpRcptReply,
		ESmtpDataReply,
		ESmtpContentReply,
		ESmtpLogoutReply
	};
		
private:
	CMtfTestActionSmtpSendEmail(CMtfTestCase& aTestCase);
	
private:
	CImTextServerSession* iImSocket;
	TInt iSendState;
	TInt iRecvState;
	RFs iFs;
	RFile iFile;
	HBufC* iSender;
	HBufC* iRecipient;
	CImIAPPreferences* iIapPrefs;
};


#endif  // CMTFTESTACTIONSMTPSENDEMAIL_H
