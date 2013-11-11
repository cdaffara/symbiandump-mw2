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


#ifndef __CMTFTESTACTIONSENDASLAUNCHEDITORANDCLOSE_H__
#define __CMTFTESTACTIONSENDASLAUNCHEDITORANDCLOSE_H__


#include "CMtfSynchronousTestAction.h"

/*@{*/
/// Literal defined for the name of the Test Action
_LIT(KTestActionSendAsLaunchEditorAndClose,"SendAsLaunchEditorAndClose");
/*@}*/

/**
SendAsLaunchEditorAndClose Test Action is intended launch the message editor in "Fire and Forget" mode: the message created using Send-As API is closed and is opened in the corresponding message editor
@internalTechnology
*/
class CMtfTestActionSendAsLaunchEditorAndClose : public CMtfSynchronousTestAction
	{
public:
	virtual ~CMtfTestActionSendAsLaunchEditorAndClose(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();

private:
	CMtfTestActionSendAsLaunchEditorAndClose(CMtfTestCase& aTestCase);
	};


#endif //__CMTFTESTACTIONSENDASLAUNCHEDITORANDCLOSE_H__