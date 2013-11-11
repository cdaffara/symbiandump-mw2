// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

/**
 @file
 @internalTechnology 
*/



#ifndef __CMTF_TEST_ACTION_CREATE_MAIL_SELECTION_H__
#define __CMTF_TEST_ACTION_CREATE_MAIL_SELECTION_H__


// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCreateMailSelection,"CreateMailSelection");
/*@}*/


/**
  Implements the action to create two seperate message selections from a 
  selection of all IMAP4 messages in a parent folder.  From the selection of all 
  IMAP4 type children messages, the message Ids are copied into seperate selections
  based on odd or even index in the children selection.
  @internalTechnology
*/
class CMtfTestActionCreateMailSelection : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* ActionParameters
							   );
	
public:
	// From CMtfSynchronousTestAction, override the base class' virtual function
	void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionCreateMailSelection(CMtfTestCase& aTestCase); 	
	};
#endif //__CMTF_TEST_ACTION_CREATE_MAIL_SELECTION_H__
