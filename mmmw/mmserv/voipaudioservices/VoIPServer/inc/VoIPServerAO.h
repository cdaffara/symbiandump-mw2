/*
 * Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Definition of active object used for ITC.
 *
 */

#ifndef VOIPSERVERAO_H
#define VOIPSERVERAO_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS

// CLASS DEFINITIONS

// -----------------------------------------------------------------------------
//  Class Name:  MThreadComObserver
//
//  A/O observer class
// -----------------------------------------------------------------------------
class MThreadComObserver
    {
public:
    virtual void DoHandleCmd(TExcType aExc, TInt aError) = 0;
    virtual void DoHandleError(TInt aError) = 0;
    };

// -----------------------------------------------------------------------------
//  Class Name:  CVoIPServerAO
//
//  A/O class used for ITC between VoIP server threads.
// -----------------------------------------------------------------------------
class CVoIPServerAO : public CActive
    {
public:

    void Request();
    CVoIPServerAO(MThreadComObserver* aObserver, TThreadId aThreadID);

private:
    void DoCancel();
    void RunL();
    TInt RunError(TInt aError);

    MThreadComObserver* iObserver;
    TThreadId iID;
    };

#endif // VOIPSERVERAO_H

// End of file
