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

#ifndef __CMTFTESTACTIONSENDASSETSUBJECT_H__
#define __CMTFTESTACTIONSENDASSETSUBJECT_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsSetSubject,"SendAsSetSubject");
/*@}*/

/**
 SendAsSetSubject Test Action is intended to set subject field for a message created
 using Send-As API.
@internalTechnology
*/
class CMtfTestActionSendAsSetSubject : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsSetSubject(); 

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);

	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsSetSubject(CMtfTestCase& aTestCase);
	};

#endif //__CMTFTESTACTIONSENDASSETSUBJECT_H__