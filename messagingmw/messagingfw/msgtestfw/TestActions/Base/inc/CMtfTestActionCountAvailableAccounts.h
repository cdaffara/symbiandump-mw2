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

#ifndef __CMTFTESTACTIONCOUNTAVAILABLEACCOUNTS_H_
#define __CMTFTESTACTIONCOUNTAVAILABLEACCOUNTS_H_

#include "CMtfSynchronousTestAction.h"

#include <msvapi.h>

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionCountAvailableAccounts,"CountAvailableAccounts");
/*@}*/

/**
Implements the Test Action to Count Number of Available SendAs Accounts

@internalTechnology
*/
class CMtfTestActionCountAvailableAccounts : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCountAvailableAccounts(); 

public:
	virtual void ExecuteActionL();


private:
	CMtfTestActionCountAvailableAccounts(CMtfTestCase& aTestCase);
	};

#endif //__CMTFTESTACTIONCOUNTAVAILABLEACCOUNTS_H_