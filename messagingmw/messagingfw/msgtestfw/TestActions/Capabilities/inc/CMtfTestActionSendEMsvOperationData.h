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



#ifndef __CMTFTESTACTIONSENDEMSVOPERATIONDATA_H__
#define __CMTFTESTACTIONSENDEMSVOPERATIONDATA_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvOperationData,"SendEMsvOperationData");

class CMtfTestActionSendEMsvOperationData : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvOperationData();
	
private:
	CMtfTestActionSendEMsvOperationData(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVOPERATIONDATA_H__
