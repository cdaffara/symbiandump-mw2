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
#include "T_MsgSharedDataSmtp.h"


// epoc includes
#include <miutset.h>

/**
NewL()
Static factory constructor. Uses two phase 
construction.

@leave KErrNoMemory
@return
A pointer to the newly created CT_MsgSharedDataSmtp object.
*/
CT_MsgSharedDataSmtp* CT_MsgSharedDataSmtp::NewL()
	{
	CT_MsgSharedDataSmtp* self = new(ELeave) CT_MsgSharedDataSmtp();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


/**
  ConstructL()
  Second phage constructor
*/
void CT_MsgSharedDataSmtp::ConstructL()
{
	CT_MsgSharedDataBase::ConstructL();
	iPeriodic 		= CPeriodic::New(1);
	iMaxRamValue 	= 0;
	iMinRamValue 	= 0;
	iStartRamValue 	= 0;
	iEndRamValue 	= 0;
	iMark			= ETrue;
}

/**
  ~CT_MsgSharedDataSmtp
  Destructor
*/
CT_MsgSharedDataSmtp::~CT_MsgSharedDataSmtp()
	{
	delete iPeriodic;
	}

/**
NewMtmL()

*/
CBaseMtm* CT_MsgSharedDataSmtp::NewMtmL()
	{
	return iRegistry->NewMtmL(KUidMsgTypeSMTP);	
	}


/**
CT_MsgSharedDataBase()
Constructor
*/
CT_MsgSharedDataSmtp::CT_MsgSharedDataSmtp()
	{
	}
