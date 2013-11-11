// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "PushServerPanic.h"
#include "PushClientPanic.h"
#include "pushservermtm.h"
#include "pushclientmtm.h"

#ifndef EKA2
GLDEF_C TInt E32Dll(TDllReason/* aReason*/)	
// DLL entry point
	{
	return (KErrNone);
	}
#endif

GLDEF_C void Panic(TPushServerPanic aPanic)
// Panic function
	{
	User::Panic(_L("PUSHSVR"), aPanic);
	}

GLDEF_C void Panic(TPushClientPanic aPanic)
// Panic function
	{
	User::Panic(_L("PUSHCLI"), aPanic);
	}
