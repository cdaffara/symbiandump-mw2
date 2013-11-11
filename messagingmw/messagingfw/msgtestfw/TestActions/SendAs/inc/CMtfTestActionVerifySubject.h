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

#ifndef __CMTFTESTACTIONVERIFYSUBJECT_H__
#define __CMTFTESTACTIONVERIFYSUBJECT_H__

// System include
#include <msvstore.h>

// User include
#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifySubject,"VerifySubject");
/*@}*/


/**
VerifySubject Test Action is intended compare the subject field of the message with the string 
provided as input to the Test Action. If the subject field of the message does not match with the
specified string, the Test Action fails.
@internalTechnology
*/
class CMtfTestActionVerifySubject : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionVerifySubject(); 

	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);

	virtual void ExecuteActionL();

private:
	CMtfTestActionVerifySubject(CMtfTestCase& aTestCase);
	
	TInt VerifySubjectL(CMsvEntry& aMsvEntry,HBufC* aSubject);
	};

#endif //__CMTFTESTACTIONVERIFYSUBJECT_H__