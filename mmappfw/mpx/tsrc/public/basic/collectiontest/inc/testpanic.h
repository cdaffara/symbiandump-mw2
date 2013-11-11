/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  test panic code
*
*/

#ifndef TESTPANIC_H
#define TESTPANIC_H

// INCLUDE
#include <e32std.h>

/** Test panic codes */
enum TTestCommonPanics
    {
    ETestCommonState = 1,
    ETestCommonActiveState,
    ETestCommonActiveRunError,
    ETestCommonOutOfBound,
    ETestCommonCancelError,
    ETestCommonTestNotSupport,
    // add further panics here
    };

inline void TestPanic(TTestCommonPanics aReason)
    {
    _LIT(name, "TestCommon");
    User::Panic(name, aReason);
    }

#endif // TESTPANIC_H
