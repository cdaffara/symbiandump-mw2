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


#ifndef __CMTF_TEST_ACTION_INSERT_MEDIA_H__
#define __CMTF_TEST_ACTION_INSERT_MEDIA_H__


#include "CMtfSynchronousTestAction.h"

// Forward declarations
class CSessionMediaObserver;
class CMsvSession;


_LIT(KTestActionInsertMedia,"InsertMedia");


class CMtfTestActionInsertMedia : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionInsertMedia(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionInsertMedia(CMtfTestCase& aTestCase);

private:
	CSessionMediaObserver* iObserver;
	CMsvSession* iSession;
	};


#endif
