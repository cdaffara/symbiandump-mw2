/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class provides an interface for monitoring radio
*                state.
*
*
*/


#ifndef C_RADIOMONITORBODY_H
#define C_RADIOMONITORBODY_H

#include <e32base.h>
#include <e32property.h>
#include <RadioMonitor.h>

/**
 *  This class encapsulates the implementation body of CRadioMonitor interface.
 *
 *  @lib Radio_Utility.lib
 *  @since S60 3.2
 */
class CRadioMonitor::CBody : public CActive
    {

public:

    /**
     * Factory function to create a new instance of the radio monitor.
     *
     * @since S60 3.2
     * @param aObserver The observer object
     * @return A new radio monitor object.
     */
    static CBody* NewL( MRadioMonitorObserver& aObserver );

    ~CBody();

    /**
     * Check whether the radio is on.
     *
     * @since S60 3.2
     * @return Radio state. ETrue = ON, EFalse = OFF.
     */
    TBool IsRadioOn() const;

private:

// from base class CActive

    /**
     * From CActive
     * Called when P&S key changes
     *
     * @since S60 3.2
     */
    void RunL();

    /**
     * From CActive
     * Cancels outstanding requests.
     *
     * @since S60 3.2
     */
    void DoCancel();

private:

    void ConstructL();
    CBody( MRadioMonitorObserver& aObserver  );

private:

    // The monitored P&S key
    RProperty iProperty;
    // Radio monitoring client
    MRadioMonitorObserver& iRadioMonitorClient;

    };

#endif // C_RADIOMONITOR_H
