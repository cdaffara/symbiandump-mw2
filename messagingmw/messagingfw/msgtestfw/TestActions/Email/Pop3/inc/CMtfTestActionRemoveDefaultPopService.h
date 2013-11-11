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



#ifndef __CMTFTESTACTIONREMOVEDEFAULTPOPSERVICE_H__
#define __CMTFTESTACTIONREMOVEDEFAULTPOPSERVICE_H__


#include "CMtfSynchronousTestAction.h"


_LIT( KTestActionRemoveDefaultPopService,"RemoveDefaultPopService");

class CMtfTestActionRemoveDefaultPopService : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionRemoveDefaultPopService();
	
private:
	CMtfTestActionRemoveDefaultPopService(CMtfTestCase& aTestCase);
	};


#endif  // __CMTFTESTACTIONREMOVEDEFAULTPOPSERVICE_H__
