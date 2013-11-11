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


#ifndef __CMTFTESTACTIONSENDASADDLINKEDATTACHMENT_H__
#define __CMTFTESTACTIONSENDASADDLINKEDATTACHMENT_H__


#include "CMtfTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsAddLinkedAttachment,"SendAsAddLinkedAttachment");
/*@}*/

/**
SendAsAddLinkedAttachment Test Action is intended to add linked attachment to the created message
@internalTechnology
*/
class CMtfTestActionSendAsAddLinkedAttachment : public CMtfTestAction
	{
public:
	virtual ~CMtfTestActionSendAsAddLinkedAttachment(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

protected:	
	void DoCancel();
	void RunL();
	
private:
	CMtfTestActionSendAsAddLinkedAttachment(CMtfTestCase& aTestCase);

private:
	RSendAsMessage iSendAsMessage;
	};


#endif //__CMTFTESTACTIONSENDASADDLINKEDATTACHMENT_H__