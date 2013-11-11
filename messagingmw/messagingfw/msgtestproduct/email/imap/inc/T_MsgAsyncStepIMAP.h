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
* References IMAP shared data for use by derived asynchronous test steps.
* 
*
*/



#ifndef __T_MSGASYNCSTEPIMAP_H__
#define __T_MSGASYNCSTEPIMAP_H__


/* User includes */
#include "T_MsgAsyncStep.h"
#include "T_MsgSharedDataImap.h"


/* 
 * Implements a base test step from which other IMAP asynchronous test steps
 * which require a shared data object can be derived
 */
class CT_MsgAsyncStepIMAP : public CT_MsgAsyncStep
	{
protected:
	CT_MsgAsyncStepIMAP(CT_MsgSharedDataImap& iSharedDataIMAP);

protected:
	/* Reference to CT_MsgSharedDataImap */
	CT_MsgSharedDataImap& iSharedDataIMAP;
	};
#endif /* __T_MSGASYNCSTEPIMAP_H__ */
