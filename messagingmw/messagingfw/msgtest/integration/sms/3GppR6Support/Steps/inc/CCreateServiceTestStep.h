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
 
#ifndef __CCREATESERVICETESTSTEP_H__
#define __CCREATESERVICETESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;

_LIT(KCreateServiceTestStep,"CreateService");
namespace SharedData
{
class CCreateServiceTestStep : public CBasicTestStep
{
public:
	CCreateServiceTestStep (MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CCreateServiceTestStep();
};
}
#endif //__CCREATESERVICETESTSTEP_H__