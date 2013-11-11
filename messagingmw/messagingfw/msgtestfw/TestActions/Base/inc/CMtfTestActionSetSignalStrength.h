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



#ifndef __CMTFTESTACTIONSETSIGNALSTRENGTH_H__
#define __CMTFTESTACTIONSETSIGNALSTRENGTH_H__

#include <e32property.h>
#include "CMtfSynchronousTestAction.h"

_LIT(KTestActionSetSignalStrength,"SetSignalStrength");

/**
This test action sets the signal strength through RProperty.

@internalTechnology
*/
class CMtfTestActionSetSignalStrength : public CMtfSynchronousTestAction
	{
	public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase, CMtfTestActionParameters* aActionParameters);
	CMtfTestActionSetSignalStrength(CMtfTestCase& aTestCase);
	virtual void ExecuteActionL();	
	};

#endif //__CMTFTESTACTIONSETSIGNALSTRENGTH_H__
