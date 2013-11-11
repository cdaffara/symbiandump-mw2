/**
* Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CMTFTESTACTIONSETNETREGSTATUS_H__
#define __CMTFTESTACTIONSETNETREGSTATUS_H__

#include <e32property.h>
#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSetNetRegStatus,"SetNetworkRegistrationStatus");

/**
This test action sets the network registration status through RProperty.

@internalTechnology
*/
class CMtfTestActionSetNetRegStatus : public CMtfSynchronousTestAction
	{
	public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters);
	CMtfTestActionSetNetRegStatus(CMtfTestCase& aTestCase);
	virtual void ExecuteActionL();	
	};

#endif //__CMTFTESTACTIONSETNETREGSTATUS_H__
