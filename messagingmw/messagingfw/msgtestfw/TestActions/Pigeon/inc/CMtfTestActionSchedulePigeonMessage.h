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

#ifndef __CMTFTESTACTIONSCHEDULEPIGEONMESSAGE_H__
#define __CMTFTESTACTIONSCHEDULEPIGEONMESSAGE_H__

// User Includes
#include "CMtfTestAction.h"

// System Includes
#include <msvstd.h>
#include <msvapi.h>

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionSchedulePigeonMessage,"SchedulePigeonMessage");
/*@}*/

/**
This Test action issues the command the Pigeon server MTM to schedule/reschdule
the messages.
@internalTechnology
*/
class CMtfTestActionSchedulePigeonMessage : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters);
	virtual ~CMtfTestActionSchedulePigeonMessage();
	virtual void ExecuteActionL();

protected:
	void RunL();
	void DoCancel();

private:
	CMtfTestActionSchedulePigeonMessage(CMtfTestCase& aTestCase);

private:
	CMsvOperation*			iOperation;
	CMsvEntrySelection *	iSelection;
	};

#endif // __CMTFTESTACTIONSCHEDULEPIGEONMESSAGE_H__
