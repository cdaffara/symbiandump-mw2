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


#ifndef __CMTFTESTACTIONSENDASSENDMESSAGE_H__
#define __CMTFTESTACTIONSENDASSENDMESSAGE_H__


#include "CMtfTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsSendMessage,"SendAsSendMessage");
/*@}*/

/**
SendAsSendMessage Test Action is intended to request the Send-As server to send the message.
@internalTechnology
*/
class CMtfTestActionSendAsSendMessage : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSendAsSendMessage(); 

public:
	virtual void ExecuteActionL();

protected:
	void DoCancel();
	void RunL();	
	
private:
	CMtfTestActionSendAsSendMessage(CMtfTestCase& aTestCase);

private:
	RSendAsMessage 		iSendAsMessage;
	};


#endif// __CMTFTESTACTIONSENDASSENDMESSAGE_H__