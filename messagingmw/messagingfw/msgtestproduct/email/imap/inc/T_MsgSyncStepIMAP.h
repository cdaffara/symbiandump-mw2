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
* Sync step class owning IMAP shared data for use by derived test steps.
* 
*
*/




#ifndef __T_MSG_SYNC_STEP_IMAP_H__
#define __T_MSG_SYNC_STEP_IMAP_H__


/* User includes */
#include "T_MsgStep.h"
#include "T_MsgSharedDataImap.h"


/* 
 * Implements a class from which synchrnous test steps which require a 
 * shared data object will derive
 */
class CT_MsgSyncStepIMAP : public CT_MsgStep
	{
protected:
	CT_MsgSyncStepIMAP(CT_MsgSharedDataImap& iSharedDataIMAP);

	virtual ~CT_MsgSyncStepIMAP();
private:
	CT_MsgSyncStepIMAP(); 

protected:
	/* Refernce to an CT_MsgSharedDataImap */
	CT_MsgSharedDataImap& iSharedDataIMAP;
	};
#endif /* __T_MSG_SYNC_STEP_IMAP_H__ */
