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


#ifndef __CMTFTESTACTIONVERIFYBIOTYPE_H__
#define __CMTFTESTACTIONVERIFYBIOTYPE_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionVerifyBioType,"VerifyBioType");
/*@}*/

/**
VerifyBioType Test Action is intended verify the Bio type set for the message with the specified value.
@internalTechnology
*/
class CMtfTestActionVerifyBioType : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionVerifyBioType(); 
	
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
								CMtfTestActionParameters* ActionParameters);
	
	virtual void ExecuteActionL();

private:
	CMtfTestActionVerifyBioType(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONVERIFYBIOTYPE_H__