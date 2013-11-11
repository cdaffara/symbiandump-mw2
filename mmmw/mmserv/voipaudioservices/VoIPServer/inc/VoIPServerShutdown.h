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
 * Description:  Definition of A/O for VoIP server shutdown timer.
 *
 */

#ifndef VOIPSERVERSHUTDOWN_H
#define VOIPSERVERSHUTDOWN_H

//  INCLUDES
//#include <e32base.h>
#include "VoIPServer.h"

// CLASS DECLARATION
class CVoIPAudioServer;

/**
 *  Shutdown timer after the last client has disconnected.
 *
 *  @lib CVoIPAudioServer.exe
 */

/**
 * A Timer utility class used by the Server for shutdown purpose.
 */
class CDelayVoIPServerShutDown : public CActive
    {
public:
    // Construct/destruct
    static CDelayVoIPServerShutDown* NewL();
    ~CDelayVoIPServerShutDown();

public:
    // Request a timeout after aDelay
    void SetDelay(TTimeIntervalMicroSeconds32 aDelay);

protected:
    // From CActive
    void RunL();
    void DoCancel();

private:
    // Construct/destruct
    CDelayVoIPServerShutDown();
    void ConstructL();

private:
    RTimer iShutDownTimer; // Has
    };

#endif //VOIPSERVERSHUTDOWN_H

// End of File
