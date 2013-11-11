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

#include <miutset.h>
/* User include */
#include "T_MsgSharedDataCommon.h"

/* Epoc includes */
#include <msvsearchsortoperation.h>

/**
  Function : NewL
  Description : Constructor
  @return : A pointer to CT_MsgSharedDataCommon
  @leave  : KErrNoMemory if	sufficient memory is not available
*/
CT_MsgSharedDataCommon* CT_MsgSharedDataCommon::NewL()
	{
	CT_MsgSharedDataCommon* self = new(ELeave) CT_MsgSharedDataCommon();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CT_MsgSharedDataCommon::ConstructL()
	{
	CT_MsgSharedDataBase::ConstructL();
	User::LeaveIfError(iUtilityServer.Connect());
	iSearchSortOperation = NULL;
	iSearchSortQuery = NULL;
	}

CT_MsgSharedDataCommon::CT_MsgSharedDataCommon()//:iSearchSortQuery()
	{
	}

CT_MsgSharedDataCommon::~CT_MsgSharedDataCommon()
	{
	iUtilityServer.Close();
	if (iSearchSortOperation != NULL)
		{
		delete iSearchSortOperation;
		}
	iSearchSortResultArray.Close();
	}
