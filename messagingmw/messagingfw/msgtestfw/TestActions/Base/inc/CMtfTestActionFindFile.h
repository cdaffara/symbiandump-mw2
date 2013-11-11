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


#ifndef __CMTF_TEST_ACTION_FIND_FILE_H__
#define __CMTF_TEST_ACTION_FIND_FILE_H__


// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
// Literal defined for the name of the Test Action
_LIT(KTestActionFindFile,"FindFile");
/*@}*/


/**
  Implements the test action to check if the specified file is present at 
  specified path.
  @internalTechnology
*/
class CMtfTestActionFindFile : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters
							   );
	// Destructor
	virtual ~CMtfTestActionFindFile(); 

public:
	virtual void ExecuteActionL();

private:
	// Constructor
	CMtfTestActionFindFile(CMtfTestCase& aTestCase);
	};



#endif
