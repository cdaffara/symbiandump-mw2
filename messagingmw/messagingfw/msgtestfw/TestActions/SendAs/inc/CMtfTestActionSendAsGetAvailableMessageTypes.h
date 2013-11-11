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


#ifndef __CMTF_TEST_ACTION_SENDAS_GET_AVAILABLE_MESSAGE_TYPES_H__
#define __CMTF_TEST_ACTION_SENDAS_GET_AVAILABLE_MESSAGE_TYPES_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsGetAvailableMessageTypes,"SendAsGetAvailableMessageTypes");
/*@}*/

/**
  Implements the Test Action to get the list of available message types
  from the list of filtered MTMs.
  @internalTechnology
*/
class CMtfTestActionSendAsGetAvailableMessageTypes : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);
								
	virtual ~CMtfTestActionSendAsGetAvailableMessageTypes(); 							
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsGetAvailableMessageTypes(CMtfTestCase& aTestCase);
	};

#endif // __CMTF_TEST_ACTION_SENDAS_GET_AVAILABLE_MESSAGE_TYPES_H__