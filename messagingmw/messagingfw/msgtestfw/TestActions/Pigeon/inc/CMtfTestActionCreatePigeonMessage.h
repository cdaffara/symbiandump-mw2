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

#ifndef __CMTFTESTACTIONCREATEPIGEONMESSAGE_H__
#define __CMTFTESTACTIONCREATEPIGEONMESSAGE_H__

#include "CMtfSynchronousTestAction.h"

class CMsvEntry;

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionCreatePigeonMessage,"CreatePigeonMessage");
/*@}*/

/**
Implements the Test Action to create a simple Pigeon message.

@internalTechnology
*/
class CMtfTestActionCreatePigeonMessage : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionCreatePigeonMessage(CMtfTestCase& aTestCase);
	
	void RoundUpToSecond(TTime& aTime);
	};

#endif //__CMTFTESTACTIONCREATEPIGEONMESSAGE_H__
