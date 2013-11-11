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
// This file contains the shared data class for the POP server
// 
//

/**
 @file
*/

// user include
#include "t_shareddatapop.h"


// epoc includes
#include <miutset.h>
#include <mtclreg.h>


/**
NewL()
Allocates and creates a new CT_MsgSharedDataPop object

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgSharedDataPop
*/
CT_MsgSharedDataPop* CT_MsgSharedDataPop::NewL()
	{
	CT_MsgSharedDataPop* sharedDataPop=new(ELeave) CT_MsgSharedDataPop();
	CleanupStack::PushL(sharedDataPop);
	sharedDataPop->ConstructL();
	CleanupStack::Pop(sharedDataPop);
	return sharedDataPop;
	}

	
/**
NewMtmL()
 
@return
Returns a pointer to Client-side MTM object
*/
CBaseMtm* CT_MsgSharedDataPop::NewMtmL()
	{
	return iRegistry->NewMtmL(KUidMsgTypePOP3);
	}


/**
CT_MsgSharedDataPop()
Construstor
*/
CT_MsgSharedDataImap::CT_MsgSharedDataImap()
	{
	}
