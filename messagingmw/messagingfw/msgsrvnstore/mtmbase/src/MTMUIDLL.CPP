// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32std.h>
#include "MTUIPAN.H"
#include "MTUDPAN.H"

// global function declarations

GLDEF_C void Panic(TMtuiPanic aPanic)
	{
	User::Panic(_L("MTUI"), aPanic);
	}

GLREF_C void Panic(TMtudPanic aPanic)
// Panic function
	{
	User::Panic(_L("MTMUI Data"), aPanic);
	}
