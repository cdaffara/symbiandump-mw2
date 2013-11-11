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

#ifndef __CMTFTESTACTIONLAUNCHAUTOSEND_H__
#define __CMTFTESTACTIONLAUNCHAUTOSEND_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionLaunchAutoSend,"LaunchAutoSend");
/*@}*/

/**
  Implements the test action that launches AutoSend exe.
  @internalTechnology
*/
class CMtfTestActionLaunchAutoSend : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );

	// Destructor
	virtual ~CMtfTestActionLaunchAutoSend(); 

public:
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionLaunchAutoSend(CMtfTestCase& aTestCase);
	};

#endif	// __CMTFTESTACTIONLAUNCHAUTOSEND_H__
