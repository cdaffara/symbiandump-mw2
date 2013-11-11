/*
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
* @file 
* This is the header file for CT_MsgSetUnReadFlag
* 
*
*/




#ifndef __T_SET_UNREADFLAG_H__
#define __T_SET_UNREADFLAG_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"


/* Literals Used */
_LIT(KSetUnReadFlag,"SetUnReadFlag");


/* Implements a test step to verify the number of IMAP accounts created */
class CT_MsgSetUnReadFlag: public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgSetUnReadFlag(CT_MsgSharedDataImap& aSharedDataIMAP);

	/* CTestStep implementation*/
	virtual TVerdict doTestStepL();
	};
#endif /*__T_SET_UNREADFLAG_H__*/
