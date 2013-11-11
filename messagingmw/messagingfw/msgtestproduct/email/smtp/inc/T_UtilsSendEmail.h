/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
* This is the header file for CT_MsgUtilsSendEmail
* 
*
*/



/**
 @file
*/


#ifndef __T_UTILS_SMTP_SEND_EMAIL_H__
#define __T_UTILS_SMTP_SEND_EMAIL_H__


// User  Includes
#include <test/testexecutestepbase.h>


// Epoc includes
#include <imsk.h>

// Forward Declaration
class CImIAPPreferences;


/**
Implements a utility that performs the sending of the email
*/
class CT_MsgUtilsSendEmail : public CActive
	{
public:
	static CT_MsgUtilsSendEmail* NewL(CTestStep& aTestStep);
	~CT_MsgUtilsSendEmail();
public:	
	void StartL(TPtrC aSender,TPtrC aReceipient,TPtrC aMailFile,TPtrC aServerIP,
											TRequestStatus& aStatus);
protected:
	void DoCancel();
	void RunL();
private:
	CT_MsgUtilsSendEmail(CTestStep& aTestStep);
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
public:
	TPtrC iSender;
	TPtrC iReceipient;
private:
	TRequestStatus* iRequestStatus;
	CImTextServerSession* iImSocket;
	TInt iSendState;
	TInt iRecvState;
	RFs iFs;
	RFile iFile;
	CImIAPPreferences* iIapPrefs;
	CTestStep& iTestStep;
	};
#endif //__T_UTILS_SMTP_SEND_EMAIL_H__
