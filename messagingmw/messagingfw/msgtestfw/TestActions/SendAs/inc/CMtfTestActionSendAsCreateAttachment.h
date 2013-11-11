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


#ifndef __CMTFTESTACTIONSENDASCREATEATTACHMENT_H__
#define __CMTFTESTACTIONSENDASCREATEATTACHMENT_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCreateAttachment,"SendAsCreateAttachment");
/*@}*/

/**
SendAsCreateAttachment Test Action is intended to create an attachment to the message.
@internalTechnology
*/
class CMtfTestActionSendAsCreateAttachment : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsCreateAttachment(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCreateAttachment(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASCREATEATTACHMENT_H__