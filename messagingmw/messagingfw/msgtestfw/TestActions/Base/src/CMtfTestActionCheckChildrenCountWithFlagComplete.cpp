// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// CheckChildrenCountWithFlagComplete
// [Action Parameters]
// Session  <input>: Reference to the session.
// ParentId <input>: Value of the parent id.
// Count    <input>: Value of expected count of children with "Complete" flag set.
// [Action Description]
// Checks if count of children with "Complete" flag set is as expected.
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
// __ACTION_INFO_END__
// 
//

/**
 @file
*/


#include "CMtfTestActionCheckChildrenCountWithFlagComplete.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"

#include <msvapi.h>

CMtfTestAction* CMtfTestActionCheckChildrenCountWithFlagComplete::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionCheckChildrenCountWithFlagComplete* self = new (ELeave) CMtfTestActionCheckChildrenCountWithFlagComplete(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionCheckChildrenCountWithFlagComplete::CMtfTestActionCheckChildrenCountWithFlagComplete(CMtfTestCase& aTestCase)
	: CMtfTestActionCheckChildrenCountWithFlagBase(aTestCase)
	{
	}


CMtfTestActionCheckChildrenCountWithFlagComplete::~CMtfTestActionCheckChildrenCountWithFlagComplete()
	{
	}


TBool CMtfTestActionCheckChildrenCountWithFlagComplete::FlagIsSet(TMsvEntry& entry)
	{
	return entry.Complete();
	}

const TDesC* CMtfTestActionCheckChildrenCountWithFlagComplete::FlagName()
   {
   return &KFlagNameComplete;
   }
