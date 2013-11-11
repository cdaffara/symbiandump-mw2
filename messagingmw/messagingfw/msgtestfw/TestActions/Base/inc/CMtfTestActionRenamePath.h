
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

#ifndef __CMTFTESTACTIONRENAMEPATH_H__
#define __CMTFTESTACTIONRENAMEPATH_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionRenamePath,"RenamePath");


class CMtfTestActionRenamePath : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionRenamePath(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionRenamePath(CMtfTestCase& aTestCase);
	};


#endif	// __CMTFTESTACTIONRENAMEPATH_H__
