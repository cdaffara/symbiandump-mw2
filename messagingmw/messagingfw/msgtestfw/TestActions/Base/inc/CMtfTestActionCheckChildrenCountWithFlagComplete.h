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


#ifndef __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_WITH_FLAG_COMPLETE_H__
#define __CMTF_TEST_ACTION_CHECK_CHILDREN_COUNT_WITH_FLAG_COMPLETE_H__


#include "CMtfTestActionCheckChildrenCountWithFlagBase.h"

////////////////////////////////////////////////////////////////////////////// 
//
// __ACTION_INFO_BEGIN__ 
//
// [Action Name]
// CheckChildrenCountWithFlagComplete
//
// [Action Parameters]
// Session  <input>: Reference to the session.
// ParentId <input>: Value of the parent id.
// Count    <input>: Value of expected count of children with "Complete" flag set.
//
// [Action Description]
// Checks if count of children with "Complete" flag set is as expected.
//
// [APIs Used]
// CMsvEntry::SetEntryL
// CMsvEntry::ChildrenL
// CMsvEntry::SetSortTypeL
// TMsvSelectionOrdering::SetShowInvisibleEntries
//
// __ACTION_INFO_END__
//
////////////////////////////////////////////////////////////////////////////// 

_LIT(KTestActionCheckChildrenCountWithFlagComplete,"CheckChildrenCountWithFlagComplete");
_LIT(KFlagNameComplete,"Complete");


class CMtfTestActionCheckChildrenCountWithFlagComplete : public CMtfTestActionCheckChildrenCountWithFlagBase
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionCheckChildrenCountWithFlagComplete(); 

public:
	virtual TBool FlagIsSet(TMsvEntry& entry);
	virtual const TDesC* FlagName();

private:
	CMtfTestActionCheckChildrenCountWithFlagComplete(CMtfTestCase& aTestCase);

	};


#endif
