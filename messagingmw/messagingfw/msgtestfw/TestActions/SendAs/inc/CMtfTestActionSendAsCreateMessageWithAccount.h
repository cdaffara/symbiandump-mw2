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


#ifndef __CMTFTESTACTIONSENDASCREATEMESSAGEWITHACCOUNT_H__
#define __CMTFTESTACTIONSENDASCREATEMESSAGEWITHACCOUNT_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCreateMessageWithAccount,"SendAsCreateMessageWithAccount");
/*@}*/

/**
SendAsCreateMessageWithAccount Test Action is intended to create a message with the specified account; the type of the message depends on the type of the account specified.
@internalTechnology
*/
class CMtfTestActionSendAsCreateMessageWithAccount : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsCreateMessageWithAccount(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCreateMessageWithAccount(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASCREATEMESSAGEWITHACCOUNT_H__