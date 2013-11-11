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

#ifndef __TMSSERVERSHUTDOWN_H
#define __TMSSERVERSHUTDOWN_H

//  INCLUDES
#include "tmsserver.h"

namespace TMS {

// CLASS DECLARATION

/**
 *  Shutdown timer after the last client has disconnected.
 *
 *  @lib TMSServer.exe
 */

/**
 * A Timer utility class used by the Server for shutdown purpose.
 */
class TMSServerShutDown : public CActive
    {
public:
    // Construct/destruct
    static TMSServerShutDown* NewL();
    ~TMSServerShutDown();

public:
    // Request a timeout after aDelay
    void SetDelay(TTimeIntervalMicroSeconds32 aDelay);

protected:
    // From CActive
    void RunL();
    void DoCancel();

private:
    // Construct/destruct
    TMSServerShutDown();
    void ConstructL();

private:
    RTimer iShutDownTimer; // Has
    };

} //namespace TMS

#endif //__TMSSERVERSHUTDOWN_H

// End of File
