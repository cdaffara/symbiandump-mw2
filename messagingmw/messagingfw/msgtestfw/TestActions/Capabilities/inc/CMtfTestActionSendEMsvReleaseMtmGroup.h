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



#ifndef __CMTFTESTACTIONSENDEMSVRELEASEMTMGROUP_H__
#define __CMTFTESTACTIONSENDEMSVRELEASEMTMGROUP_H__


#include "CMtfSynchronousTestAction.h"



_LIT( KTestActionSendEMsvReleaseMtmGroup,"SendEMsvReleaseMtmGroup");

class CMtfTestActionSendEMsvReleaseMtmGroup : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL( CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters );
	virtual void ExecuteActionL();
	~CMtfTestActionSendEMsvReleaseMtmGroup();
	
private:
	CMtfTestActionSendEMsvReleaseMtmGroup(CMtfTestCase& aTestCase);
	};




#endif  // __CMTFTESTACTIONSENDEMSVRELEASEMTMGROUP_H__
