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


#ifndef __CMTF_TEST_ACTION_SENDAS_CLOSE_MESSAGE_H__
#define __CMTF_TEST_ACTION_SENDAS_CLOSE_MESSAGE_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsCloseMessage,"SendAsCloseMessage");
/*@}*/

/**
  Implements the Test Action to close the RSendAsMessage object
  @internalTechnology
*/
class CMtfTestActionSendAsCloseMessage : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);
								
	virtual ~CMtfTestActionSendAsCloseMessage(); 							
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsCloseMessage(CMtfTestCase& aTestCase);
	};

#endif //__CMTF_TEST_ACTION_SENDAS_CLOSE_MESSAGE_H__