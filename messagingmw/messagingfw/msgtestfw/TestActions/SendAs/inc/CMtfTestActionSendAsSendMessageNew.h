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


#ifndef __CMtfTestActionSendAsSendMessageNew_H__
#define __CMtfTestActionSendAsSendMessageNew_H__


#include "CMtfTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsSendMessageNew,"SendAsSendMessageNew");
/*@}*/

/**
SendAsSendMessage Test Action is intended to request the Send-As server to send the message.
@internalTechnology
*/
class CMtfTestActionSendAsSendMessageNew : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionSendAsSendMessageNew(); 

public:
	virtual void ExecuteActionL();

protected:
	void DoCancel();
	void RunL();	
	
private:
	CMtfTestActionSendAsSendMessageNew(CMtfTestCase& aTestCase);

private:
	RSendAsMessage 		iSendAsMessage;
	};


#endif// __CMtfTestActionSendAsSendMessageNew_H__