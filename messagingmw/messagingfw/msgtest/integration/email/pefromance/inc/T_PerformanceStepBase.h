// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_PERFORMANCESTEPBASE_H__
#define __T_PERFORMANCESTEPBASE_H__

#include <test/testexecutestepbase.h>
#include "emailtestutils.h"
#include <iapprefs.h>
#include <cemailaccounts.h>

_LIT(KEmailBodyFile1,"emailbodyfile1");
_LIT(KEmailBodyFile2,"emailbodyfile2");
_LIT(KAttachmentFile1,"attachment1");
_LIT(KAttachmentFile2,"attachment2");
_LIT(KSubject,"subject");
_LIT(KEmailAddress,"emailaddress");
_LIT(KNullFileName,"");

const TInt KMaxFileNameSize=80;
const TInt KMaxSubjectSize=80;

class TDummySessionObserver : public CBase , public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent aEvent, TAny* aArg1, TAny* aArg2, TAny* aArg3);
	};


class CPerformanceStepBase : public CTestStep
	{
public:
	CPerformanceStepBase();
	~CPerformanceStepBase();
	virtual TVerdict doTestStepPreambleL();
	void InitialisationComplete(TInt aErrorCode);
	void StartTimer();
	void StopTimer();
	void TestComplete(TInt aErrorCode);
	
protected:
	HBufC8* CopyToBuf8LC(const TDesC& aDesC);
	void LoadProfiler();
	void CreateAccountsL();
	void GetProfileDetailsFromIniFileL();
	TInt GetEntryCount();
	void WriteStoreL();
private:
	CConsoleBase* iConsole;
	CActiveScheduler* iScheduler;
	CEmailTestUtils* iTestUtils;
	TInt iMessageEntryCount;
	TPtrC16 iScriptFile;
	TImapAccount iImapAccount;
protected:
	CMsvSession* iSession;
	TSmtpAccount iSmtpAccount;
	TBool iProfile;
	TUint32 iStopCount;
	TUint32 iStartCount;
	TReal64 iDuration;
	TDummySessionObserver* iSessionObserver;
	TPtrC iPtrEmailaddress;
	TBuf<KMaxFileNameSize>iEmailBodyFile1;
	TBuf<KMaxFileNameSize>iEmailBodyFile2;
	TBuf<KMaxFileNameSize>iAttachmentFile1;
	TBuf<KMaxFileNameSize>iAttachmentFile2;
	TBuf<KMaxSubjectSize>iSubject;
	TBuf<KMaxSubjectSize>iNullFilePath;
	};

	
#endif  //__T_PERFORMANCESTEPBASE_H__