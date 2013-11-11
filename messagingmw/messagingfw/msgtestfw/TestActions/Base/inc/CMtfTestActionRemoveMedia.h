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


#ifndef __CMTF_TEST_ACTION_REMOVE_MEDIA_H__
#define __CMTF_TEST_ACTION_REMOVE_MEDIA_H__


#include "CMtfSynchronousTestAction.h"

#include <msvapi.h>


_LIT(KTestActionRemoveMedia,"RemoveMedia");


class CMtfTestActionRemoveMedia : public CMtfSynchronousTestAction
	{
public:
	static CMtfTestAction* NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* ActionParameters);
	virtual ~CMtfTestActionRemoveMedia(); 

public:
	virtual void ExecuteActionL();

private:
	CMtfTestActionRemoveMedia(CMtfTestCase& aTestCase);

	enum TMsvFailure // as defined in msvserv.h
		{
		EHeapFailure,
		EDiskFailure
		};
	};


#endif
