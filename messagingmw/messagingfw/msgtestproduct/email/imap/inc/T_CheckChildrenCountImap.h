/**
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* This is the header file for CT_MsgCheckChildrenCountImap
* 
*
*/



/**
 @file 
*/


#ifndef __T_CHECK_CHILDREN_COUNT_IMAP_H__
#define __T_CHECK_CHILDREN_COUNT_IMAP_H__


// User includes 
#include"T_MsgSyncStepIMAP.h"


// Literals Used 
_LIT(KCheckChildrenCountImap,"CheckChildrenCountImap");


/*
 * Implements the test step which obtains the children count of the entries under the remote 
 * IMAP folder and verifies if it matches with the expected count
 */
class CT_MsgCheckChildrenCountImap : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckChildrenCountImap(CT_MsgSharedDataImap& aSharedDataIMAP);

	// CTestStep implementation 
	virtual TVerdict doTestStepL();
	};
#endif //__T_CHECK_CHILDREN_COUNT_IMAP_H__ 
