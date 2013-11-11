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
* This is the header file for CT_MsgIsImap4Connected
* 
*
*/




#ifndef __T_ISIMAP4CONNECTED_H__
#define __T_ISIMAP4CONNECTED_H__


/* User include */
#include "T_MsgSyncStepIMAP.h"
#include "T_MsgSharedDataImap.h"


/* Literals Used */
_LIT(KIsImap4Connected,"IsImap4Connected");


/* 
 * Implements a test step to check whether the MTM is connected or not 
 * connected to an external server
 */
class CT_MsgIsImap4Connected : public CT_MsgSyncStepIMAP
	{
public:
	CT_MsgIsImap4Connected(CT_MsgSharedDataImap& aSharedDataIMAP);

	/*	CTestStep implementation */
	virtual TVerdict doTestStepL();
	};
#endif /*__T_ISIMAP4CONNECTED_H__*/
