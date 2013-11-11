/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Network registration status monitor.
*
*/



#ifndef CSATNETWORKREGSTATUSMONITOR_H
#define CSATNETWORKREGSTATUSMONITOR_H

#include <e32base.h>
#include <e32svr.h>
#include <satcs.h>
#include "msatmultimodeapi.h"

/**
 *  Network registration status monitor
 *
 *  Initiates asynchronous operation to get current network registration
 *  status. After it has been got, initiates monitor change of its value.
 *
 *  @lib SystemState
 *  @since S60 v3.1
 */
class CSatNetworkRegStatusMonitor : public CActive
    {

public:

    static CSatNetworkRegStatusMonitor* NewL( MSatMultiModeApi& aPhone );

    virtual ~CSatNetworkRegStatusMonitor();

    /**
     * Activate network registration status monitor.
     */
    void Start();

    /**
     * Fetch current network registration status.
     * On error returns ERegistrationUnknown.
     *
     * @return Current network registration status.
     */
    RMobilePhone::TMobilePhoneRegistrationStatus CurrentValue();

protected:

    /**
     * From CActive
     * Implements cancellation of an outstanding request.
     */
    void DoCancel();

    /**
     * From CActive
     * Handles an active object's request completion event.
     */
    void RunL();

private:

    CSatNetworkRegStatusMonitor( MSatMultiModeApi& aPhone );

private: // data

    /**
     * Current value of network registration status.
     */
    RMobilePhone::TMobilePhoneRegistrationStatus iRegistrationStatus;

    /**
     * iRegistrationStatus first value has not yet been got
     * after starting this monitor.
     */
    TBool iWaitingRegistrationStatus;

    /**
     * Blocker for synchronous requests.
     */
    CActiveSchedulerWait iWait;

    /**
     * Phone instance.
     * Not own.
     */
    MSatMultiModeApi& iPhone;

    };

#endif // CSATNETWORKREGSTATUSMONITOR_H
