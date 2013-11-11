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
* Description:  location triggering server client interface
*
*/



#ifndef LBTTRIGGERFIRINGEVENTNOTIFIER_H
#define LBTTRIGGERFIRINGEVENTNOTIFIER_H

#include <lbt.h>
#include <lbttriggerfiringeventobserver.h>

/**
 * Helper class to receive trigger firing event 
 * through observer interface. 
 *
 * @see RLbt::NotifyTriggerFired
 *
 * @lib lbt.lib
 *
 * @since S60 5.1
 */

class CLbtTriggerFiringEventNotifier : public CActive
    {
public:
    /**
     * Construct a trigger firing event notifier object. 
     *
     * When the object is constructed. The trigger firing event 
     * notification request is not issued to the
     * Location Triggering Server. Client shall call 
     * CLbtTriggerFiringEventNotifier::Start() to start
     * notification.
     *
     * @param[in] aLbt A referece to RLbt object. The subsession 
     * must be opened. Otherwise a panic is raised.
     * @param[in] aObserver A referece to the 
     * observer object to receive trigger firing event.
     * @param[in] aPriority An integer specifying the 
     * priority of this active object. CActive::TPriority 
     * defines a standard set of priorities. 
     *
     * @panic LocTriggering ELbtServerBadHandle If the 
     * subsession of aLbt is not opened.
     */
    IMPORT_C static CLbtTriggerFiringEventNotifier* NewL(
        RLbt& aLbt,
        MLbtTriggerFiringEventObserver& aObserver,
        TInt aPriority = CActive::EPriorityStandard);

    /**
     * Destructor. 
     *
     * If the notification is started, the 
     * destructor will cancel the notification.
     */
    IMPORT_C ~CLbtTriggerFiringEventNotifier();

    /**
     * Start trigger firing event notification. 
     *
     * After this function is called, when a 
     * trigger is fired, the
     * client will be notified from the observer interface.
     * 
     * Client shall call Cancel() function to stop the
     * trigger firing event notification.
     */
    IMPORT_C void Start();

private:
    //Derived from CActive
    void RunL();
    TInt RunError( TInt aError );
    void DoCancel();

private:
    /**
     * Default constructor
     */
    CLbtTriggerFiringEventNotifier(
        RLbt& aLbt,
        MLbtTriggerFiringEventObserver& aObserver,
        TInt aPriority = CActive::EPriorityStandard );
    
    /**
     * Second phase constructor
     */
    void ConstructL();

private:
    /** 
     * Trigger fire info
     */
    TLbtTriggerFireInfo iFiringEvent;
    
    /**
     * Reference to Lbt subsession.
     */
    RLbt& iLbt;
    
    /**
     * Reference to trigger fire event observer
     */
    MLbtTriggerFiringEventObserver& iObserver;
    };

#endif //LBTTRIGGERFIRINGEVENTNOTIFIER_H
