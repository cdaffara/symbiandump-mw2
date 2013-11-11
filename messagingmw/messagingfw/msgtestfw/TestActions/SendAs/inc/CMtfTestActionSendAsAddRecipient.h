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

#ifndef __CMTFTESTACTIONSENDASADDRECIPIENT_H__
#define __CMTFTESTACTIONSENDASADDRECIPIENT_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsAddRecipient,"SendAsAddRecipient");
/*@}*/

/**
SendAsAddRecipient Test Action is intended to add a recipient of type 'To' or 'Cc' or 'Bcc' address to the created message.
@internalTechnology
*/
class CMtfTestActionSendAsAddRecipient : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsAddRecipient(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsAddRecipient(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASADDRECIPIENT_H__