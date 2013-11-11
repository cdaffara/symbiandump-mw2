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

#ifndef __CMTFTESTACTIONCHECKIFMESSAGESCHEDULEDATEEXPIRED_H__
#define __CMTFTESTACTIONCHECKIFMESSAGESCHEDULEDATEEXPIRED_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCheckIfMessageScheduleDateExpired,"CheckIfMessageScheduleDateExpired");
/*@}*/

/**
Implements the Test Action to check if the schedule time of the message has expired

@internalTechnology
*/
class CMtfTestActionCheckIfMessageScheduleDateExpired : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionCheckIfMessageScheduleDateExpired(CMtfTestCase& aTestCase);
	};

#endif // __CMTFTESTACTIONCHECKIFMESSAGESCHEDULEDATEEXPIRED_H__
