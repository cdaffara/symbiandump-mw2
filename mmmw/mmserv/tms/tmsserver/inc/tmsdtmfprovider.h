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

#ifndef TMSDTMFPROVIDER_H
#define TMSDTMFPROVIDER_H

#include <e32base.h>
#include <etelmm.h>
#include <glib.h>
#include <rmmcustomapi.h>
#include "tmsdtmfobserver.h"

namespace TMS {

// FORWARD DECLARATIONS
class TMSEtelDtmfMonitor;
class TMSEtelDtmfStopMonitor;

/**
 * TMSDTMFProvider class
 * Makes asynchronic request to ETel interface according to given request type.
 * Provides canceling via CActive::Cancel().
 */
class TMSDTMFProvider : public CActive
    {
public:

    /**
     * Two phased constructing of the DTMF provider instance.
     * @param aPhone mobile phone handle
     * @param aMmCustom custom API handle for special DTMF event
     *        monitoring.
     * @return the DTMF provider instance
     */
    static TMSDTMFProvider* NewL();

    /**
     * C++ default destructor
     */
    virtual ~TMSDTMFProvider();

    /**
     * HandleDTMFEvents.
     * @param aEvent Event type
     * @param aError Error code
     * @param aTone Character
     */
    void NotifyDTMFEvent(const TMSDTMFObserver::TCCPDtmfEvent aEvent,
            const gint aError, const TChar aTone);

    // from base class MCCPDTMFProvider
    /**
     * Cancels asynchronous DTMF string sending.
     * @return KErrNone if succesfull, otherwise another system wide error code
     */
    gint CancelDtmfStringSending();

    /**
     * Starts the transmission of a single DTMF tone across a
     * connected and active call.
     * @param aTone Tone to be played.
     * @return KErrNone if succesfull, otherwise another system wide error code
     */
    gint StartDtmfTone(const TChar aTone);

    /**
     * Stops playing current DTMF tone.
     * @return KErrNone if succesfull, otherwise another system wide error code
     */
    gint StopDtmfTone();

    /**
     * Plays DTMF string.
     * @param aString String to be played.
     * @return KErrNone if succesfull, otherwise another system wide error code
     * KErrArgument if the specified string contains illegal DTMF characters
     */
    gint SendDtmfToneString(const TDesC& aString);

    /**
     * Continue or cancel sending DTMF string which was stopped with 'w'
     * character in string.
     * @param aContinue ETrue if sending of the DTMF string should continue,
     * EFalse if the rest of the DTMF string is to be discarded.
     * @return KErrNone if succesfull, otherwise another system wide error code
     */
    gint ContinueDtmfStringSending(const gboolean aContinue);

    /**
     * Add an observer for DTMF related events.
     * Plug-in dependent feature if duplicates or more than one observers
     * are allowed or not. Currently CCE will set only one observer.
     * @param aObserver Observer
     * @leave system error if observer adding fails
     */
    void AddObserver(const TMSDTMFObserver& aObserver);

    /**
     * Remove an observer.
     * @param aObserver Observer
     * @return KErrNone if removed succesfully. KErrNotFound if observer was
     * not found. Any other system error depending on the error.
     */
    gint RemoveObserver(const TMSDTMFObserver& aObserver);

    // from base class CActive
protected:
    /**
     * From CActive
     * RunL
     */
    void RunL();

    /**
     * From CActive
     * Cancels the monitor
     */
    void DoCancel();

private:
    /**
     * Constructs the requester.
     *
     * @param aPhone handle to ETel phone
     * @param aMmCustom custom API handle
     */
    TMSDTMFProvider();

    /**
     * Constructing the provider in the second phase.
     */
    void ConstructL();

private:
    /**
     * DTMF event observer.
     */
    RPointerArray<TMSDTMFObserver> iObservers;

    /**
     * ETel phone handle for DTMF functionality.
     */
    RMobilePhone iPhone;

    /**
     * Monitor for DTMF events and changes.
     * Own.
     */
    TMSEtelDtmfMonitor* iMonitor;

    /**
     * Monitor for DTMF stopping.
     * Own.
     */
    TMSEtelDtmfStopMonitor* iStopMonitor;

    /**
     * Custom API reference.
     */
    RMmCustomAPI iMmCustom;

    RTelServer iServer;
    TBuf<25> iTsyname;
    };

} //namespace TMS

#endif //TMSDTMFPROVIDER_H
