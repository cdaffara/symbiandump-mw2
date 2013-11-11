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


#ifndef __CMTFTESTACTIONVERIFYBODYTEXT_H__
#define __CMTFTESTACTIONVERIFYBODYTEXT_H__

#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifyBodyText,"VerifyBodyText");
/*@}*/

/**
VerifyBodyText Test Action is intended to read the body text contents of a message and compare it with the contents of a file
@internalTechnology
*/
class CMtfTestActionVerifyBodyText : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionVerifyBodyText(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();
	void RunTestL();

private:
	CMtfTestActionVerifyBodyText(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONVERIFYBODYTEXT_H__