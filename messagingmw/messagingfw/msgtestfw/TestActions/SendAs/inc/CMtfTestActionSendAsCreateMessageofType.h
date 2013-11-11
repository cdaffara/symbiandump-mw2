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

#ifndef __CMTF_TEST_ACTION_SENDAS_CREATE_MESSAGE_OF_TYPE_H__
#define __CMTF_TEST_ACTION_SENDAS_CREATE_MESSAGE_OF_TYPE_H__

#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCreateMessageofType,"SendAsCreateMessageofType");
/*@}*/

/**
  Implements the Test Action to create a message of specified message type.
  @internalTechnology
*/
class CMtfTestActionSendAsCreateMessageofType : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	
	virtual ~CMtfTestActionSendAsCreateMessageofType(); 	
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCreateMessageofType(CMtfTestCase& aTestCase);
	};

#endif //__CMTF_TEST_ACTION_SENDAS_CREATE_MESSAGE_OF_TYPE_H__