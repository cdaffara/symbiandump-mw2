/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitors browser window session termination.
*
*/


#ifndef CSATBROWSERWSMONITOR_H
#define CSATBROWSERWSMONITOR_H

// INCLUDES
#include <w32std.h>
#include "CSatEventMonitorHandler.h"
#include "msatbrowserthreadobserver.h"
#include "msatbrowserwsobserver.h"

// FORWARD DECLARATION
class CSatBrowserThreadMonitor;
class MSatBrowserWSObserver;

// CLASS DECLARATION

/**
 * Monitors browser termination.
 *
 * @lib EventMonitors.lib
 * @since Series 60 3.1
 */
class CSatBrowserWSMonitor : public CActive,
                             public MSatBrowserThreadObserver
    {
public:

    /**
     * Two-phased constructor.
     */
    static CSatBrowserWSMonitor* NewL();

    /**
     * Destructor.
     */
    virtual ~CSatBrowserWSMonitor();

    /**
     * From CSatEventMonitorHandler Starts event monitoring
     * @param Browser termination observer.
     * @return Error code indicating the status of starting the monitor
     */
    TInt StartMonitor(
        MSatBrowserWSObserver* aObserver );

    /**
     * This function is called when browser's thread has actually died.
     * @param aCause The cause for the browser termination.
     */
    void NotifyBrowserTerminated(
        const RSat::TBrowserTerminationCause aCause );

// From base class CActive

    /**
     * From CSatEventMonitorHandler Cancels event monitoring
     */
    void DoCancelMonitor();

protected:

// From base class CActive

    /**
     * From CActive.
     */
    void RunL();

    /**
     * From CActive, implements the cancel protocol.
     */
    void DoCancel();

private:

    /**
     * C++ default constructor.
     */
    CSatBrowserWSMonitor();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

    /**
     * Start to monitor Browser termination.
     */
    void Start();

    /**
     * Creates a window group and hides it from the UI.
     */
    void CreateWGL();

private: // Data

    /**
     * Thread death monitor
     */
    CSatBrowserThreadMonitor* iThreadMonitor;

    /**
     * Indicates is window group created or not
     */
    TBool iWGCreated;

    /**
     * Browser termination observer.
     */
    MSatBrowserWSObserver* iObserver;

    /**
     * Window group
     */
    RWindowGroup iWg;

    /**
     * Window session
     */
    RWsSession iWsSession;

    };

#endif // CSATBROWSERWSMONITOR_H
