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


#ifndef __CMTFTESTACTIONSENDASCLOSEANDSENDMESSAGE_H__
#define __CMTFTESTACTIONSENDASCLOSEANDSENDMESSAGE_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCloseAndSendMessage,"SendAsCloseAndSendMessage");
/*@}*/

/**
Test Action is intended to request sending the message and closing the RSendAsMessage object.
@internalTechnology
*/
class CMtfTestActionSendAsCloseAndSendMessage : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsCloseAndSendMessage(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCloseAndSendMessage(CMtfTestCase& aTestCase);
	};

#endif //__CMTFTESTACTIONSENDASCLOSEANDSENDMESSAGE_H__