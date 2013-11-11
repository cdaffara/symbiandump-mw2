/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/



/**
 @file
*/
 
#ifndef __CCREATESMSMESSAGETESTSTEP_H__
#define __CCREATESMSMESSAGETESTSTEP_H__

 
#include "CBasicTestStep.h"
class MSharedData;
class CSmsHeader;

_LIT(KCreateSmsMessageTestStep,"CreateSmsMessage");

namespace SharedData
{
class CCreateSmsMessageTestStep : public CBasicTestStep
{
public:
	CCreateSmsMessageTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CCreateSmsMessageTestStep();
private:
	void SetRecipientsL(CSmsHeader& aHeader);
};
}
#endif //__CCREATESMSMESSAGETESTSTEP_H__