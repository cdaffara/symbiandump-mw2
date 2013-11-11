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
 
#ifndef __CDELETECHILDRENTESTSTEP_H__
#define __CDELETECHILDRENTESTSTEP_H__

#include "CBasicTestStep.h"
class MSharedData;

_LIT(KDeleteChildrenTestStep,"DeleteChildren");

namespace SharedData
{
class CDeleteChildrenTestStep: public CBasicTestStep
{
public:
	CDeleteChildrenTestStep(MSharedData& aObj); 
	virtual TVerdict doTestStepL();
	~CDeleteChildrenTestStep();
};
}
#endif //__CDELETECHILDRENTESTSTEP_H_