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
* Description:  Monitoring application for servers restarting & IAD
*
*/

#ifndef MDSWATCHDOG_H
#define MDSWATCHDOG_H

#include <e32base.h>

#include "wdshutdownobserver.h"
#include "wdselfshutdownobserver.h"

class CWatchdog : public CActive, public MWDShutdownObserver, public MWDSelfShutdownObserver
    {
    public:
    
        enum TWatchdogState
            {
            EIdle,
            EWaitingRendezvous,
            ERunning,
            EShuttingDown,
            ERestarting
        };

        static CWatchdog* NewL();
	    virtual ~CWatchdog();

        // from base class CActive
	    void RunL();
        TInt RunError( TInt aError );
	    void DoCancel();

	    // From MShutdownObserver
        void ShutdownNotification();
        void RestartNotification();
        
        // From MWDSelfShutdownObserver
        void SelfShutdownNotification();

    private:

        CWatchdog();
        
        void ConstructL();
        
        void Start();

    private: // data

        TWatchdogState iState;

        CWDShutdownObserver* iShutdownObserver;        
        
        CWDSelfShutdownObserver* iSelfShutdownObserver;
        
        RProcess iProcess;
  
    };


#endif
