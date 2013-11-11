/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CMTFTESTACTIONGETMESSAGEATINDEX_H__
#define __CMTFTESTACTIONGETMESSAGEATINDEX_H__

#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionGetMessageAtIndex,"GetMessageAtIndex");
/*@}*/

/**
Implements the Test Action to check if the schedule time of the message has expired

@internalTechnology
*/
class CMtfTestActionGetMessageAtIndex : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionGetMessageAtIndex(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionGetMessageAtIndex(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONGETMESSAGEATINDEX_H__