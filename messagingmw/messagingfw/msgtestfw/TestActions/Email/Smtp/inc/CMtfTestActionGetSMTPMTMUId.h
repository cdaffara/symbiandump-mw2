
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

#ifndef __CMTFTESTACTIONGETSMTPMTMUID_H__
#define __CMTFTESTACTIONGETSMTPMTMUID_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionGetSMTPMTMUId,"GetSMTPMTMUId");


class CMtfTestActionGetSMTPMTMUId : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetSMTPMTMUId(); 

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionGetSMTPMTMUId(CMtfTestCase& aTestCase);
	};


#endif	//__CMTFTESTACTIONGETSMTPMTMUID_H__
