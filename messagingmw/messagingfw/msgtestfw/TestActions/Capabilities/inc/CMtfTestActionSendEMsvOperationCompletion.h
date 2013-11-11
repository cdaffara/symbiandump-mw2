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



#ifndef __CMTFTESTACTIONSENDEMSVOPERATIONCOMPLETION_H__
#define __CMTFTESTACTIONSENDEMSVOPERATIONCOMPLETION_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvOperationCompletion,"SendEMsvOperationCompletion");

class CMtfTestActionSendEMsvOperationCompletion : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvOperationCompletion();
	
private:
	CMtfTestActionSendEMsvOperationCompletion(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVOPERATIONCOMPLETION_H__
