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


#ifndef __CMTFTESTACTIONSENDASADDATTACHMENT_H__
#define __CMTFTESTACTIONSENDASADDATTACHMENT_H__


#include "CMtfTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsAddAttachment,"SendAsAddAttachment");
/*@}*/

/**
SendAsAddAttachment Test Action is intended to add non-linked attachment to the created message
@internalTechnology
*/
class CMtfTestActionSendAsAddAttachment : public CMtfTestAction
	{
public:
	virtual ~CMtfTestActionSendAsAddAttachment(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

protected:	
	void DoCancel();
	void RunL();

private:
	CMtfTestActionSendAsAddAttachment(CMtfTestCase& aTestCase);

private:
	RSendAsMessage	iSendAsMessage;
	};


#endif //__CMTFTESTACTIONSENDASADDATTACHMENT_H__