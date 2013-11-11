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


#ifndef CMTFTESTACTIONCREATEPOP3MAILSELECTION_H
#define CMTFTESTACTIONCREATEPOP3MAILSELECTION_H


// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionCreateMailSelection,"CreateMailSelection");
/*@}*/


/**
  Overview: 
  CMtfTestActionCreatePop3MailSelection class derived from CMtfSynchronousTestAction.
  Implements the action of creation of two seperate message selections from a 
  selection of all POP3 messages in a parent folder.  From the selection of all 
  IMPA4 type children messages, the message Ids are copied into seperate selections
  based on odd or even index in the children selection.
  @internalTechnology
*/
class CMtfTestActionCreateMailSelection : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,
								 CMtfTestActionParameters* ActionParameters
							   );
	virtual ~CMtfTestActionCreateMailSelection(); 

public:
	// From CMtfSynchronousTestAction, override the base class' virtual function
	virtual void ExecuteActionL();

private:
	CMtfTestActionCreateMailSelection(CMtfTestCase& aTestCase); 	
	};


#endif  // CMTFTESTACTIONCREATEPOP3MAILSELECTION_H
