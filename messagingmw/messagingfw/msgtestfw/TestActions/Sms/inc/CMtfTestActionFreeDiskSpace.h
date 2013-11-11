// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CMTFTESTACTIONFREEDISKSPACE_H__
#define __CMTFTESTACTIONFREEDISKSPACE_H__

#include "CMtfSynchronousTestAction.h"

_LIT( KTestActionFreeDiskSpace,"FreeDiskSpace");

class CMtfTestActionFreeDiskSpace : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionFreeDiskSpace();
	
private:
	CMtfTestActionFreeDiskSpace(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONFREEDISKSPACE_H__
