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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef DELAYTIMER_H
#define DELAYTIMER_H

#include <e32base.h>

namespace TMS {

/**
 * A Timer utility class used by the Server for shutdown purpose.
 */
class TMSCallDelayTimer : public CActive
    {
public:
    // Construct/destruct
    static TMSCallDelayTimer* NewL();
    ~TMSCallDelayTimer();

public:
    // Request a timeout after aDelay
    void SetDelay(TTimeIntervalMicroSeconds32 aDelay);

protected:
    // From CActive
    void RunL();
    void DoCancel();

private:
    // Construct/destruct
    TMSCallDelayTimer();
    void ConstructL();

private:
    RTimer iShutDownTimer; // Has
    };

} //namespace TMS

#endif //DELAYTIMER_H

// End of File
