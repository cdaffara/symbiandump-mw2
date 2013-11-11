/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file 
* This is the header file for CT_RetrievePlainBodyTextAndCompare
* 
*
*/





#ifndef __T_RETRIEVEPLAINBODYTEXTANDCOMPARE_H__
#define __T_RETRIEVEPLAINBODYTEXTANDCOMPARE_H__


// User includes
#include "T_MsgAsyncStepSMTP.h"
#include "T_MsgSharedDataSmtp.h"


// Literals used 
_LIT(KRetrievePlainBodyTextAndCompare,"RetrievePlainBodyTextAndCompare");


/* 
 * Implements a test step that edit a message body text 
 */
class CT_RetrievePlainBodyTextAndCompare : public CT_MsgAsyncStepSMTP
	{
public:
	static CT_RetrievePlainBodyTextAndCompare* NewL(CT_MsgSharedDataSmtp& aSharedDataSMTP);
	~CT_RetrievePlainBodyTextAndCompare();

	// CTestStep implementation 
	virtual TVerdict doTestStepL();


private:
	CT_RetrievePlainBodyTextAndCompare(CT_MsgSharedDataSmtp& aSharedDataSMTP);

	virtual void ProgressL(TBool bFinal);
	virtual void CancelStep();
	};
#endif // __T_RETRIEVEPLAINBODYTEXTANDCOMPARE_H__
