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

#if !defined(__MTUDPAN_H__)
#define __MTUDPAN_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

/**
@internalComponent
@released
*/
enum TMtudPanic
	{
	EMtudBadLibrary,
	EMtudIncorrectBitmapNumbers,
	EMtudWrongMtmType,
	EMtudSelectionOfMoreThanOneType,
	EMtudMoreThanOneLocationInFuntionFlags,
	EMturThreadStartPanic1,
	EMturThreadStartPanic2,
	EMturThreadStartPanic3
	};

GLREF_C void Panic(TMtudPanic aPanic);

#endif
