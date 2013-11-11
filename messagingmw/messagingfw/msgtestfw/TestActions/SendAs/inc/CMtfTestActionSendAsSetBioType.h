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


#ifndef __CMTFTESTACTIONSENDASSETBIOTYPE_H__
#define __CMTFTESTACTIONSENDASSETBIOTYPE_H__

#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsSetBioType,"SendAsSetBioType");
/*@}*/

/**
GSendAsSetBioType Test Action is intended to set BIO type for the message that is created using Send-As API.
@internalTechnology
*/
class CMtfTestActionSendAsSetBioType : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsSetBioType(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsSetBioType(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASSETBIOTYPE_H__