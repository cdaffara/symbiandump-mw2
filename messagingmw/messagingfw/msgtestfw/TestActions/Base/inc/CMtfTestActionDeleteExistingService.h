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

#ifndef __CMTFTESTACTIONDELETEEXISTINGSERVICE_H__
#define __CMTFTESTACTIONDELETEEXISTINGSERVICE_H__

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionDeleteExistingService,"DeleteExistingService");
/*@}*/


/**
	Implements the action to delete the existing services for the specified MTM.
  @internalTechnology
*/
class CMtfTestActionDeleteExistingService : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionDeleteExistingService(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionDeleteExistingService(CMtfTestCase& aTestCase);
	};


#endif // __CMTFTESTACTIONDELETEEXISTINGSERVICE_H__
