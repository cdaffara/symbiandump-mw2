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


#ifndef __CMTF_TEST_ACTION_SECTION_COMPLETE_H__
#define __CMTF_TEST_ACTION_SECTION_COMPLETE_H__

#include "CMtfTestAction.h"
class CMtfTestCase;

/** Test action used by the framework to handle "end of section" commands from a script. */
class CMtfTestActionSectionComplete : public CMtfTestAction
{
public:
	virtual ~CMtfTestActionSectionComplete();
	
	static CMtfTestActionSectionComplete* NewL(CMtfTestCase& aTestCase);
			
	virtual void ExecuteActionL();
	virtual void RunL();
	virtual void DoCancel();
	virtual TBool SectionCompleteAction() const;
		
private:
	CMtfTestActionSectionComplete(CMtfTestCase& aTestCase);
};


#endif

