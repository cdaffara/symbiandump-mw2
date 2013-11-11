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

//! @file
//! @internalComponent



#ifndef CMTFTESTACTIONCHECKMESSAGESTORE_H
#define CMTFTESTACTIONCHECKMESSAGESTORE_H


#include "CMtfSynchronousTestAction.h"
#include <msvapi.h>


_LIT( KTestActionCheckMessageStore,"CheckMessageStore");

class CMtfTestActionCheckMessageStore : public CMtfSynchronousTestAction
{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionCheckMessageStore();
	
private:
	CMtfTestActionCheckMessageStore(CMtfTestCase& aTestCase);
	void WalkMessageStoreL(CMsvEntry& aEntry);
	void ProcessMessageL(CMsvEntry& aEntry);
};


#endif  // CMTFTESTACTIONCHECKMESSAGESTORE_H
