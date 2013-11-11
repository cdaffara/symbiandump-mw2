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
// BIUTPAN.H
// 
//

#if !defined(__BITSPAN_H__)
#define __BITSPAN_H__

#include <e32base.h>

enum TBitsPanic
	{
	// --- ---
	EBitsUnknownOperation
	};

GLREF_C void Panic(TBitsPanic aPanic);

#endif//__BITSPAN_H__
