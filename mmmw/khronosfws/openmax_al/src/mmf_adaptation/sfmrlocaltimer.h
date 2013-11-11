/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: 
 *
 */

#ifndef SFMRLOCALTIMER_H
#define SFMRLOCALTIMER_H

#include <e32base.h>
class CMMFBackendEngine;

NONSHARABLE_CLASS(LocalTimer) : public CActive
    {
public:
    // Construct/destruct
    LocalTimer(CMMFBackendEngine* parent, void* adaptContext);
    TInt PostInit();
    virtual ~LocalTimer();

public:
    // Starts ticking after every aDelay microsecs
    void Start(TUint64 aDelay);
    void Stop();
    void ResetTime();
    TUint64 GetTime();

protected:
    // From CActive
    void RunL();
    void DoCancel();

private:
    RTimer iTimer; // Has
    TUint64 iTime;
    TUint64 iDelay;
    CMMFBackendEngine* iParent;
    void* iAdaptContext;
    };

#endif /* SFMRLOCALTIMER_H */
