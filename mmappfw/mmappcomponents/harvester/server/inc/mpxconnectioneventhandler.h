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
* Description:  Monitors USB events
*
*/


#ifndef CMPXCONNECTIONEVENTHANDLER_H
#define CMPXCONNECTIONEVENTHANDLER_H

#include <usbman.h>
#include <mpxpskeyobserver.h>
#include "mpxsystemeventobserver.h"

class CMPXPSKeyWatcher;

/**
 *  USB Monitoring class
 *
 *  @lib mpxharvesterserver.exe
 *  @since S60 3.0
 */
NONSHARABLE_CLASS( CMPXConnectionEventHandler ) : public CActive,
                                           public MMPXPSKeyObserver
    {
public:

    /**
    * Two-phase constructor
    * @param aObserver observer to the event handler
    */
    static CMPXConnectionEventHandler* NewL( MMPXSystemEventObserver& aObserver );

    /**
    * Two-phase constructor
    * @param aObserver observer to the event handler
    */
    static CMPXConnectionEventHandler* NewLC( MMPXSystemEventObserver& aObserver );

    /**
    * Virtual destructor
    */
    virtual ~CMPXConnectionEventHandler();

public:

    /**
    * Polls for the current monitor status
    * If an event is happening, it will callback the observer of the event
    */
    void PollStatus();

protected: // From Base Class

    /**
    * From CActive
    */
    void RunL();
    void DoCancel();

    /**
    * From MMPXPSKeyObserver
    */
    void HandlePSEvent( TUid aUid, TInt aKey );

private: // New Methods

    /**
    * Handle the PS Key event
    * @param aUid UID of the key
    * @param aKey ID of the key
    */
    void DoHandlePSEventL( TUid aUid, TInt aKey );

    void DoMSStartEventL();
    void DoMTPStartEventL();
    void DoMTPNotActiveEventL();
    
    /**
     * Connect to usbman.
     * If error, default iDeviceState to EUsbDeviceStateConfigured so this would not
     * block usb event mode change.
     */
    void ConnectUsbMan();

private:

    /**
    *  C++ constructor
    */
    CMPXConnectionEventHandler( MMPXSystemEventObserver& aObserver );

    /*
    * Second phased constructor
    */
    void ConstructL();

private: // data
    
    enum TMPXConnectionState
        {
        EMPXConnectionNone,
        EMPXConnectionMassStorage,
        EMPXConnectionMTPIdle,
        EMPXConnectionMTPActive
        };

    CMPXPSKeyWatcher*  iUSBKeyWatcher;  // PS key watcher for USB PS key
    CMPXPSKeyWatcher*  iMTPKeyWatcher; // PS key watcher for MTP PS key

    /*
    * Observer interface to callback to an observer
    */
    MMPXSystemEventObserver& iObserver;
    TInt iState;
    
    RUsb            iUsbMan;
    TUsbDeviceState iDeviceState;
    TBool           iUsbManConnected;
    };

#endif // CMPXCONNECTIONEVENTHANDLER_H
