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
 
#ifndef __CINITSIMTSYTESTSTEP_H__
#define __CINITSIMTSYTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;

_LIT(KInitSimTsyTestStep,"InitSimTsy");
namespace SharedData
{
class CInitSimTsyTestStep : public CBasicTestStep
{
public:
	CInitSimTsyTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CInitSimTsyTestStep();
};
}
#endif //__CINITSIMTSYTESTSTEP_H__