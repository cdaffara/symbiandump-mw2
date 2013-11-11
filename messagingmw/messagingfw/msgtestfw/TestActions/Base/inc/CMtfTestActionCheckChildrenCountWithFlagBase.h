/**
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/



/**
 @file
*/


#ifndef __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_WITH_FLAG_BASE_H__
#define __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_WITH_FLAG_BASE_H__


#include "CMtfSynchronousTestAction.h"
#include <msvapi.h>


/**
  Abstrast base class for Test Actions that check the number of Message Entries with a 
  particular flag set.
  
  Derived classes need to override FlagIsSetL() which tests the specific TMsvEntry flag. 

  @internalTechnology
*/


class CMtfTestActionCheckChildrenCountWithFlagBase : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionCheckChildrenCountWithFlagBase(); 

public:
	virtual void ExecuteActionL();

protected:
	CMtfTestActionCheckChildrenCountWithFlagBase(CMtfTestCase& aTestCase);

	virtual TBool FlagIsSet(TMsvEntry& entry);
	virtual const TDesC* FlagName() = 0;

private:
	TInt CountChildrenL(TMsvId aParent, CMsvSession& aSession);
	};


#endif
