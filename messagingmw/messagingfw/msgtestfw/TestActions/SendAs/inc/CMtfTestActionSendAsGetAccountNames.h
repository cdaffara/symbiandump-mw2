/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTFTESTACTIONSENDASGETACCOUNTNAMES_H__
#define __CMTFTESTACTIONSENDASGETACCOUNTNAMES_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsGetAccountNames,"SendAsGetAccountNames");
/*@}*/

/**
SendAsGetAccount Test Action is intended get the account from the CSendAsAccounts object which is present at the specified index.
@internalTechnology
*/
class CMtfTestActionSendAsGetAccountNames : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsGetAccountNames(); 
	
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
				CMtfTestActionParameters* ActionParameters);
	
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsGetAccountNames(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASGETACCOUNTNAMES_H__