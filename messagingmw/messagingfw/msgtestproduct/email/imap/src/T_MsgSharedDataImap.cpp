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
// This file contains the shared data class for the IMAP server
// 
//

// User include 
#include "T_MsgSharedDataImap.h"


// Epoc includes 
#include <miutset.h>

/**
  Function			: NewL
  Description		: Creates an object of CT_MsgSharedDataImap 
  @return			: N/A
  @leave			: KErrNoMemory	If there is no sufficient memory
*/
CT_MsgSharedDataImap* CT_MsgSharedDataImap::NewL()
	{
	CT_MsgSharedDataImap* self = new(ELeave) CT_MsgSharedDataImap();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

/**
  Function 			: ConstructL
  Description 		: Second phage constructor
  @return 			: void
*/
void CT_MsgSharedDataImap::ConstructL()
{
	CT_MsgSharedDataBase::ConstructL();
	iObserver		= CT_MsgEventObserver::NewL();
	iPeriodic 		= CPeriodic::New(EPriorityHigh);
 	iMaxRamValue 	= 0;
 	iMinRamValue 	= 0;
 	iStartRamValue 	= 0;
 	iEndRamValue 	= 0;
 	iMark			= ETrue;
}

/**
  Function			: NewMtmL
  Description		: Creates an IMAP MTM
  @return			: Pointer of type CBaseMtm
  @leave			: KErrNoMemory	If there is no sufficient memory
*/
CBaseMtm* CT_MsgSharedDataImap::NewMtmL()
	{
	return iRegistry->NewMtmL(KUidMsgTypeIMAP4);	
	}

/**
  Function			: CT_MsgSharedDataImap
  Description		: Constructor
  @return			: N/A
*/
CT_MsgSharedDataImap::CT_MsgSharedDataImap()
:CT_MsgSharedDataBase(), iObserver(NULL)
	{}

/**
  Function			: CT_MsgSharedDataImap
  Description		: Destructor
  @return			: N/A
*/
CT_MsgSharedDataImap::~CT_MsgSharedDataImap()
{
  	delete iPeriodic;
	if(iObserver!=NULL)
	{
	delete iObserver;
	iObserver=NULL;
	}
}
	
	
		



	
