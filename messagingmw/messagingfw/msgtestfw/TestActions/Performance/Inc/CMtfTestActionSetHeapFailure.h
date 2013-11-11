/**
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __CMTF_TEST_ACTION_HEAPFAILURE_H__
#define __CMTF_TEST_ACTION_HEAPFAILURE_H__


#include "CMtfSynchronousTestAction.h"


_LIT(KTestActionSetHeapFailure,"SetHeapFailure");

//////////////////////////////////////////////////////////////////////////////
class CMtfTestActionSetHeapFailure : public CMtfSynchronousTestAction
{
public:
	virtual ~CMtfTestActionSetHeapFailure(); 
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,
		CMtfTestActionParameters* ActionParameters);
	virtual void ExecuteActionL();
	
	enum TMsvFailure // as defined in msvserv.h
		{
		EHeapFailure,
		EDiskFailure
		};
private:
	CMtfTestActionSetHeapFailure(CMtfTestCase& aTestCase);
};



#endif
