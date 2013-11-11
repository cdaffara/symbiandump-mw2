/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: dialog waiter
*
*
*/

#include "dialogwaiter.h"
#include "vmbxlogger.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// DialogWaiter::wait
// 
// ----------------------------------------------------------------------------
//
HbAction* DialogWaiter::wait()
{
    VMBLOGSTRING("DialogWaiter::wait=>")
    mLoop.exec();
    HbAction* a=mResult;
    mResult=0;
    VMBLOGSTRING2("VMBX: DialogWaiter::wait action a %d <=",a)
    return a;
}

// ----------------------------------------------------------------------------
// DialogWaiter::done
// 
// ----------------------------------------------------------------------------
//
void DialogWaiter::done(HbAction* result)
{
    VMBLOGSTRING("DialogWaiter::done=>")
    mResult=result;
    mLoop.quit();
    VMBLOGSTRING("DialogWaiter::done<=")
}

//End of file
