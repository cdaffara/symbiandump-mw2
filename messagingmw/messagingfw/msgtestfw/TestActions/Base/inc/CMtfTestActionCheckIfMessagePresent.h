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


#ifndef __CMTF_TEST_ACTION_CHECK_IF_MESSAGE_PRESENT_H__
#define __CMTF_TEST_ACTION_CHECK_IF_MESSAGE_PRESENT_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCheckIfMessagePresent,"CheckIfMessagePresent");
/*@}*/


/**
  Implements the test action to checks a specified folder for a
  specified message id.
  @internalTechnology
*/
class CMtfTestActionCheckIfMessagePresent : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );

	// Destructor
	virtual ~CMtfTestActionCheckIfMessagePresent(); 

public:
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionCheckIfMessagePresent(CMtfTestCase& aTestCase);
	};



#endif
