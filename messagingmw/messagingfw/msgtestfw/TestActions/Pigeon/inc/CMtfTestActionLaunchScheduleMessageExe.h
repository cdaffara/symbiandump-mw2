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

#ifndef __CMTFTESTACTIONLAUNCHSCHEDULEMESSAGEEXE_H__
#define __CMTFTESTACTIONLAUNCHSCHEDULEMESSAGEEXE_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionLaunchScheduleMessageExe,"LaunchScheduleMessageExe");
/*@}*/

/**
  Implements the test action that launches ScheduleMessage exe.
  @internalTechnology
*/
class CMtfTestActionLaunchScheduleMessageExe : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );
	// Destructor
	virtual ~CMtfTestActionLaunchScheduleMessageExe(); 

public:
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionLaunchScheduleMessageExe(CMtfTestCase& aTestCase);
	};

#endif	// __CMTFTESTACTIONLAUNCHSCHEDULEMESSAGEEXE_H__
