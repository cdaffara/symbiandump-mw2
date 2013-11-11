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
// MsgSyncStepCommon
// References Common shared data for use by derived test steps.
// 
//

/* User includes */
#include "T_MsgSyncStepCommon.h"


/**
  Function : CT_MsgSyncStepCommon
  Description : Constructor
  @return : N/A
*/
CT_MsgSyncStepCommon::CT_MsgSyncStepCommon(CT_MsgSharedDataCommon& aSharedDataCommon)
:	CT_MsgStep()
,	iSharedDataCommon(aSharedDataCommon)
	{
	}
