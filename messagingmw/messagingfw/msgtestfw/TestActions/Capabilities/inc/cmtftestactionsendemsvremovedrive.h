// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef __CMTFTESTACTIONSENDEMSVREMOVEDRIVE_H__
#define __CMTFTESTACTIONSENDEMSVREMOVEDRIVE_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvRemoveDrive,"SendEMsvRemoveDrive");

class CMtfTestActionSendEMsvRemoveDrive : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvRemoveDrive();

private:
	CMtfTestActionSendEMsvRemoveDrive(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVREMOVEDRIVE_H__
