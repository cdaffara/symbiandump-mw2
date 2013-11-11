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


#ifndef __CMTF_TEST_ACTION_MOVE_SELECTION_H__
#define __CMTF_TEST_ACTION_MOVE_SELECTION_H__


#include "CMtfTestAction.h"

#include <msvapi.h>


_LIT(KTestActionMoveSelection,"MoveSelection");


class CMtfTestActionMoveSelection : public CMtfTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionMoveSelection(); 

public:
	virtual void ExecuteActionL();

protected:
	void DoCancel();
	void RunL();

private:
	CMtfTestActionMoveSelection(CMtfTestCase& aTestCase);

private:
	CMsvOperation* iOperation;
	};


#endif
