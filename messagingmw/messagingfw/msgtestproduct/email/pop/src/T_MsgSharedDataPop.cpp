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
//

/**
 @file
*/

// user include
#include "T_MsgSharedDataPop.h"


// epoc includes
#include <miutset.h>


/**
NewL()
Allocates and creates a new CT_MsgSharedDataPop object

@leave KErrNoMemory
@return
Returns pointer to an object of CT_MsgComparePopEmailMsgs
*/
CT_MsgSharedDataPop* CT_MsgSharedDataPop::NewL()
	{
	CT_MsgSharedDataPop* self = new(ELeave) CT_MsgSharedDataPop();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
  ConstructL()
  Second phage constructor
*/
void CT_MsgSharedDataPop::ConstructL()
{
	CT_MsgSharedDataBase::ConstructL();
	iPeriodic 		= CPeriodic::New(EPriorityHigh);
 	iMaxRamValue 	= 0;
 	iMinRamValue 	= 0;
 	iStartRamValue 	= 0;
 	iEndRamValue 	= 0;
 	iMark			= ETrue;
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
CT_MsgSharedDataPop::CT_MsgSharedDataPop()
	{
	}

/**
  ~CT_MsgSharedDataPop()
  Destructor
*/
CT_MsgSharedDataPop::~CT_MsgSharedDataPop()
	{
	delete iPeriodic;
	}

