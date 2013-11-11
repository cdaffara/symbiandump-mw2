// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// @file
// [Test Step] :
// AsyncStepIMAP
// References IMAP shared data for use by derived test steps.
// 
//

/* User includes */
#include "T_MsgAsyncStepIMAP.h"


/**
  Function : CT_MsgAsyncStepIMAP
  Description : Constructor
  @return : N/A
*/
CT_MsgAsyncStepIMAP::CT_MsgAsyncStepIMAP(CT_MsgSharedDataImap& aSharedDataIMAP)
:	CT_MsgAsyncStep()
,	iSharedDataIMAP(aSharedDataIMAP)
	{
	}
