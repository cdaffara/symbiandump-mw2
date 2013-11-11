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

#ifndef __CADDINDICATIONTESTSTEP_H__
#define __CADDINDICATIONTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;
class CSmsHeader;

_LIT(KAddIndicationTestStep,"AddIndication");
namespace SharedData
{
class CAddIndicationTestStep : public CBasicTestStep
{
public:
	CAddIndicationTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CAddIndicationTestStep();
private:
	void ClearAllSpecialMsgIndicationsL(CSmsHeader& aHeader);
	void AddVideoIndicationL(CSmsHeader& aHeader, TInt aMsgCount);
	void AddVoiceMailIndicationL(CSmsHeader& aHeader, TInt aMsgCount);
	void AddReplyToAddressL(CSmsHeader& aHeader,const TDesC& aReplyAddress);
	void AddFaxIndicationL(CSmsHeader& aHeader);
};
}
#endif //__CADDINDICATIONTESTSTEP_H__