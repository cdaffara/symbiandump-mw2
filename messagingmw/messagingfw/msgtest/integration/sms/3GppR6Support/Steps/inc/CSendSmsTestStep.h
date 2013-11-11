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
 
#ifndef __CSENDSMSTESTSTEP_H__
#define __CSENDSMSTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;
class CMsvOperation;

_LIT(KSendSmsTestStep,"SendSms");
namespace SharedData
{
class CSendSmsTestStep : public CBasicTestStep
{
public:
	CSendSmsTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CSendSmsTestStep();
private:
	TInt FinalProgressStatus(CMsvOperation& aOperation,const TRequestStatus& aStatus);
};
}
#endif //__CSENDSMSTESTSTEP_H__