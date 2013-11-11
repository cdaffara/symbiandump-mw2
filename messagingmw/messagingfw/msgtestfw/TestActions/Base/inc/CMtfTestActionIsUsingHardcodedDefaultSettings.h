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
 


#ifndef __CMTFTESTACTIONISUSINGHARDCODEDDEFAULTSETTINGS_H__
#define __CMTFTESTACTIONISUSINGHARDCODEDDEFAULTSETTINGS_H__

#include <f32file.h>

#include "CMtfSynchronousTestAction.h"


_LIT( KTestActionIsUsingHardcodedDefaultSettings,"IsUsingHardcodedDefaultSettings");

class CMtfTestActionIsUsingHardcodedDefaultSettings : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionIsUsingHardcodedDefaultSettings(); 

private:
	CMtfTestActionIsUsingHardcodedDefaultSettings(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONISUSINGHARDCODEDDEFAULTSETTINGS_H__