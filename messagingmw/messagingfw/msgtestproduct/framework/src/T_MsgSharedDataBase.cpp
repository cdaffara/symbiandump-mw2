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
// This file contains the shared data class for the base server which creates
// a session object.
// 
//

/*  user includes */
#include "T_MsgSharedDataBase.h"


/* epoc includes */
#include <miutset.h>


/**
  Function : CT_MsgSharedDataBase
  Description : Constructor	
  @return : N/A
*/
CT_MsgSharedDataBase::CT_MsgSharedDataBase()
:	CBase()
,	iSession(NULL)
,	iRegistry(NULL)
,	iMtm(NULL)
	{
	}


/**
  Function : ConstructL
  Description : Creates the object of session, registry,initialises MTM
  @return : N/A
  @leave  :	KErrNoMemory	There is no sufficient memory
*/
void CT_MsgSharedDataBase::ConstructL()
	{
	iSession	= CMsvSession::OpenSyncL(*this);
	iRegistry	= CClientMtmRegistry::NewL(*iSession);
	iMtm		= NewMtmL();
	}


/**
  Function : ~CT_MsgSharedDataBase
  Description : Destructor
  @return : N/A
*/
CT_MsgSharedDataBase::~CT_MsgSharedDataBase()
	{
	delete iMtm;
	iMtm=NULL;

	delete iRegistry;
	iRegistry=NULL;

	delete iSession;
	iSession = NULL;
	}
