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
// CMtfTestActionInstallMtm.h
// 
//

#ifndef __CMTFTESTACTIONDEINSTALLMTMGROUP_H__
#define __CMTFTESTACTIONDEINSTALLMTMGROUP_H__

#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionDeInstallMtmGroup,"DeInstallMtmGroup");
/*@}*/

/**
Implements the Test Action to deinstall a group of MTMs

@internalTechnology
*/
class CMtfTestActionDeInstallMtmGroup : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionDeInstallMtmGroup(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionDeInstallMtmGroup(CMtfTestCase& aTestCase);
	};
#endif //__CMTFTESTACTIONDEINSTALLMTMGROUP_H__
