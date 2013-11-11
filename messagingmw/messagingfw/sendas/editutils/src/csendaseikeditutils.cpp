// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "csendaseikeditutils.h"

#include <mturutils.h>
#include <ecom/implementationproxy.h>

const TImplementationProxy ImplementationTable[] = 
	{
		IMPLEMENTATION_PROXY_ENTRY(0x1020806C, CSendAsEikEditUtils::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}

CSendAsEikEditUtils* CSendAsEikEditUtils::NewL()
	{
	CSendAsEikEditUtils* self = new (ELeave) CSendAsEikEditUtils();	
	return self;
	}
	
CSendAsEikEditUtils::CSendAsEikEditUtils()
: CSendAsEditUtils()
	{
	}
	
CSendAsEikEditUtils::~CSendAsEikEditUtils()
	{
	iThread.Close();
	}
	
// methods from CSendAsEditUtils

void CSendAsEikEditUtils::LaunchEditorL(TMsvId aId, TRequestStatus& aStatus)
	{
	MturUtils::LaunchEditorL(aId, iThread, aStatus);
	}
	
void CSendAsEikEditUtils::LaunchEditorAndWaitL(TMsvId aId)
	{
	MturUtils::LaunchEditorAndWaitL(aId);
	}
	
void CSendAsEikEditUtils::Cancel()
	{
	iThread.Kill(KErrServerTerminated);
	}
