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



#ifndef C_RADIOMONITOR_H
#define C_RADIOMONITOR_H

#include <e32base.h>

/**  Radio Monitor Event */
enum TRadioMonitorEvent
    {
	/** Radio state is off */
    ERadioStateOff,
	/** Radio state is on */
    ERadioStateOn
    };


/**
 *  This class defines the interface to be implemented by object wishing to receive
 *  notification from the radio monitor.
 *
 *  @since S60 3.2
 */
class MRadioMonitorObserver
    {
public:

    /**
     * Radio monitor event.
     *
     * @since S60 3.2
     * @aEvent a radio event
     */
    virtual void MrmEvent( TRadioMonitorEvent aEvent ) = 0;

    };

/**
 *  This class encapsulates a light-weight radio monitoring service that enable clients
 *  to monitor the operational status of the radio.
 *
 *  @lib RadioUtility.lib
 *  @since S60 3.2
 */
class CRadioMonitor : public CBase
    {
public:

    /**
     * Factory function to create a new instance of the radio monitor.
     *
     * @since S60 3.2
     * @param aObserver The observer object
     * @return A new radio monitor object.
     */
    IMPORT_C static CRadioMonitor* NewL( MRadioMonitorObserver& aObserver );

    IMPORT_C virtual ~CRadioMonitor();

    /**
     * Check whether the radio is on.
     *
     * @since S60 3.2
     * @return Radio state. ETrue = ON, EFalse = OFF.
     */
    IMPORT_C TBool IsRadioOn() const;

private:

    CRadioMonitor( MRadioMonitorObserver& aObserver );

    void ConstructL();

private: // data

    class CBody;

    /**
     * Radio monitor implementation body
     */
    CBody* iBody;
    };

#endif // C_RADIOMONITOR_H
