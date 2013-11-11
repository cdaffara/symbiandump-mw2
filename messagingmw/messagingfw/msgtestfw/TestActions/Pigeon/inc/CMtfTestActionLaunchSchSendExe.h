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

#ifndef __CMTFTESTACTIONLAUNCHSCHSENDEXE_H__
#define __CMTFTESTACTIONLAUNCHSCHSENDEXE_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionLaunchSchSendExe,"LaunchSchSendExe");
/*@}*/

/**
  Implements the test action that launches SchSendExe exe.
  @internalTechnology
*/
class CMtfTestActionLaunchSchSendExe : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );
	// Destructor
	virtual ~CMtfTestActionLaunchSchSendExe(); 

public:
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionLaunchSchSendExe(CMtfTestCase& aTestCase);
	};

#endif	// __CMTF_TEST_ACTION_LAUNCH_SCHSEND_EXE_H__
