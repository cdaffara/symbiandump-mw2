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
 
#ifndef __CVERIFYINDICATIONTESTSTEP_H__
#define __CVERIFYINDICATIONTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;
class CSmsHeader;
class MMsvEntryObserver;

_LIT(KVerifyIndicationTestStep,"VerifyIndications");

namespace SharedData
{
class CVerifyIndicationTestStep : public CBasicTestStep
{
public:
	CVerifyIndicationTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CVerifyIndicationTestStep();
	void SetObserver(MMsvEntryObserver* aObserver);
private:
	TBool VerifyVideoIndication(CSmsHeader& aHeader, TInt aCount);
	TBool VerifyEnhancedVoiceMailIndication(CSmsHeader& aHeader, TInt aCount);
	TBool VerifyFaxIndication(CSmsHeader& aHeader, TInt aCount);
	TBool VerifyOtherIndication(CSmsHeader& aHeader, TInt aCount);
	TBool VerifyVoiceIndication(CSmsHeader& aHeader, TInt aCount);
	TBool VerifyEmailIndication(CSmsHeader& aHeader, TInt aCount);
	void VerifyReplyToAddressL(CSmsHeader& aHeader,const TDesC& aReplyAddress);

private:
	MMsvEntryObserver* iObserver;
};
}
#endif //__CVERIFYINDICATIONTESTSTEP_H__