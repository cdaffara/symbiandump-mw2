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


 
#ifndef __CCLOSESIMTSYTESTSTEP_H__
#define __CCLOSESIMTSYTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;

_LIT(KCloseSimTsyTestStep,"CloseSimTsy");
namespace SharedData
{
class CCloseSimTsyTestStep : public CBasicTestStep
{
public:
	CCloseSimTsyTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CCloseSimTsyTestStep();
};
}
#endif //__CINITSIMTSYTESTSTEP_H__