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
// This contains CT_MsgActive
// 
//



/* User includes */
#include "T_MsgActive.h"


/**
  Function :~CT_MsgActive
  Description : Destructor
  @return : N/A
*/
CT_MsgActive::~CT_MsgActive()
	{
	Cancel();
	}


/**
  Function : NewL
  Description : Creates the object of CT_MsgActive
  @return : N/A
  @leave :	KErrNoMemory	There is no sufficient memory
*/
CT_MsgActive* CT_MsgActive::NewL(MT_MsgActiveCallback& aCallback, TInt aPriority)
	{
	CT_MsgActive*	self=NewLC(aCallback, aPriority);
	CleanupStack::Pop();
	return self;
	}



/**
  Function	:	NewLC
  Description : Creates the object of CT_MsgActive and pushes the object on the stack
  @return : N/A
  @leave :	KErrNoMemory	There is no sufficient memory
*/

CT_MsgActive* CT_MsgActive::NewLC(MT_MsgActiveCallback& aCallback, TInt aPriority)
	{
	CT_MsgActive*	self=new(ELeave) CT_MsgActive(aCallback, aPriority);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}



/**
  Function : CT_MsgActive
  Description : Constructor
  @return : N/A
*/
CT_MsgActive::CT_MsgActive(MT_MsgActiveCallback& aCallback, TInt aPriority)
:	CActive(aPriority)
,	iCallback(aCallback)
	{
	}


/**
  Function :ConstructL
  Description : Adds the active object to the active scheduler
  @return : N/A
*/
void CT_MsgActive::ConstructL()
	{
	CActiveScheduler::Add(this);
	}
