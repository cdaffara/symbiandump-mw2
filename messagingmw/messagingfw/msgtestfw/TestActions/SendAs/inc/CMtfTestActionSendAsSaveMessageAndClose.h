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


#ifndef __CMTFTESTACTIONSENDASSAVEMESSAGEANDCLOSE_H__
#define __CMTFTESTACTIONSENDASSAVEMESSAGEANDCLOSE_H__


//#include "CMtfTestActionSendAsCloseAndDeleteMessage.h"
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsSaveMessageAndClose,"SendAsSaveMessageAndClose");
/*@}*/

/**
SendAsSaveMessageAndClose Test Action is intended to close the RSendAsMessage object and delete the message from Drafts folder.
@internalTechnology
*/
class CMtfTestActionSendAsSaveMessageAndClose : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsSaveMessageAndClose(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsSaveMessageAndClose(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASSAVEMESSAGEANDCLOSE_H__