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
#ifndef __CVERIFYRECIPIENTTESTSTEP_H__
#define __CVERIFYRECIPIENTTESTSTEP_H__


#include "CBasicTestStep.h"
#include "mtclreg.h"

_LIT(KVerifyRecipientTestStep,"VerifyRecipient");

namespace SharedData
{
class MSharedData;
class CVerifyRecipientTestStep : public CBasicTestStep
{
public:
	CVerifyRecipientTestStep(MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CVerifyRecipientTestStep();

};
}
#endif //__CVERIFYRECIPIENTTESTSTEP_H__
