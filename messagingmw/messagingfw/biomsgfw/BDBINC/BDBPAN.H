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
// BIODBPAN.H
// 
//

#if !defined(__BDBPAN_H__)
#define __BDBPAN_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

enum TBioDBPanic
	{
	EBioDBArrayNotEmpty,
	EBioDBNotFound
	};

GLREF_C void Panic(TBioDBPanic aPanic);

#endif//__SMAUPAN_H__
