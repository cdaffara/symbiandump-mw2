// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Entry point functions for the BIUT  project
// 
//

#include <e32std.h>
#include "BIUTPAN.H"

// global function declarations



GLDEF_C void Panic(TBiutPanic aPanic)
	{
	User::Panic(_L("BIUT"), aPanic);
	}
