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

#ifndef __CMTFTESTACTIONGETPIGEONMTMCONSTANT_H__
#define __CMTFTESTACTIONGETPIGEONMTMCONSTANT_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionGetPigeonMtmConstant,"GetPigeonMtmConstant");
/*@}*/

/**
  Implements the action to return the Pigeon MTM UID
  @internalTechnology
*/
class CMtfTestActionGetPigeonMtmConstant : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionGetPigeonMtmConstant(); 

public:
	virtual void ExecuteActionL();
	
private:
	CMtfTestActionGetPigeonMtmConstant(CMtfTestCase& aTestCase);
	};

#endif  //__CMTFTESTACTIONGETPIGEONMTMCONSTANT_H__
