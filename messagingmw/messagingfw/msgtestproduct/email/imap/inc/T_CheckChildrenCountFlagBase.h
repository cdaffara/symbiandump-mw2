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
* This is the header file for CT_MsgCheckChildrenCountFlagBase
* 
*
*/



/**
 @file 
*/

#ifndef __T_CHECK_CHILDREN_COUNT_FLAG_BASE_H__
#define __T_CHECK_CHILDREN_COUNT_FLAG_BASE_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Epoc include */
#include <msvstd.h>
#include <miuthdr.h> 


/* Literals Used */
_LIT(KCheckChildrenCountFlagBase,"CheckChildrenCountFlagBase");


/* 
 * Implements the test step which obtains the count of the children,for a given
 * flag being set, and verifies if it matches with the expected count
 */
class CT_MsgCheckChildrenCountFlagBase : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgCheckChildrenCountFlagBase(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation*/
	virtual TVerdict doTestStepL();
private:
	/*
	 * Obtains the count of the number of children under the given folder, 
	 * for a given flag being set
	 */
	TInt CountChildrenL(TMsvId aFolderId, CMsvSession& aSession, TPtrC aFlagName);	

	/* Checks if the specified flag is set and returns ETrue if set 
	 * and EFalse otherwise
	 */
	TBool CheckFlag(TPtrC aFlagName, TMsvEmailEntry& aEntry);	
	};

#endif /*__T_CHECK_CHILDREN_COUNT_FLAG_BASE_H__*/
