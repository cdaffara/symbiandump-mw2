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
// Description: The header file for CMtfTestActionSendEMsvCreateHeaderEntry
//

#ifndef __CMTFTESTACTIONSENDEMSVCREATEHEADERENTRY_H__
#define __CMTFTESTACTIONSENDEMSVCREATEHEADERENTRY_H__

#include "CMtfSynchronousTestAction.h"

_LIT( KTestActionSendEMsvCreateHeaderEntry,"SendEMsvCreateHeaderEntry");

class CMtfTestActionSendEMsvCreateHeaderEntry : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvCreateHeaderEntry();

private:
	CMtfTestActionSendEMsvCreateHeaderEntry(CMtfTestCase& aTestCase);
	};

#endif  // __CMTFTESTACTIONSENDEMSVCREATEHEADERENTRY_H__
