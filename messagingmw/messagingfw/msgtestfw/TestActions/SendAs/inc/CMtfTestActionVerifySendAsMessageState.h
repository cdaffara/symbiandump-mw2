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


#ifndef __CMTFTESTACTIONVERIFYSENDASMESSAGESTATE_H__
#define __CMTFTESTACTIONVERIFYSENDASMESSAGESTATE_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifySendAsMessageState,"VerifySendAsMessageState");
/*@}*/

/**
Test Action is intended to get the progress information of message and the compare the state of the message with the expected state.
@internalTechnology
*/
class CMtfTestActionVerifySendAsMessageState : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionVerifySendAsMessageState(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionVerifySendAsMessageState(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONVERIFYSENDASMESSAGESTATE_H__