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

#ifndef __WATCHERDEF_H__
#define __WATCHERDEF_H__

#include <e32base.h>

_LIT(KWatcherPanicString, "Watcher");

enum TWatcherPanic
	{
	ENoWatcherExport=0,
	ENoCleanup=1,
	ELibraryLoadError=2
	};

inline void Panic(TWatcherPanic aPanic)
	{
	User::Panic(KWatcherPanicString, aPanic);
	}

#endif
