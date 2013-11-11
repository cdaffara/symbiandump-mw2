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
* Description:   SendUi Watcher signals service resolver, if service list
*                is in need of an update. Update events are implicit update
                (via Publish and Subcribe interface),
                 and mtm installation/removal.
*
*/




#ifndef C_CSENDUIWATCHER_H
#define C_CSENDUIWATCHER_H


#include <msvapi.h>
#include <msvstd.h>
#include <e32base.h>


class CSendUiPropertyObserver;
class CSendUiEcomObserver;
class CSendUiWatcher;

const TInt KSendUiServiceListUpdateDelay = 5000000;
const TInt KSendUiWatcherUid = 0x102823B6;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSendUiWatcher :  public CBase,
                        public MMsvSessionObserver
    {

public:

    static CSendUiWatcher* NewL( TAny* aWatcherParams );

    virtual ~CSendUiWatcher();

    /**
     * Invokes SendUi Service Resolver
     *
     * @since S60 v3.2
     */

    static TInt DoTaskL( TAny* aTask );

    
    void HandleUpdateServicesL(  );


// from base class MMsvSessionObserver

    /**
     * From MMsvSessionObserver.
     * HandleSessionEventL
     *
     * @since S60 v.3.2
     * @param aEvent Indicates the event type. 
     * @param aArg1 Not used
     * @param aArg2 Not used
     * @param aArg3 Not used
     */
     void HandleSessionEventL(
        TMsvSessionEvent aEvent,
        TAny* /*aArg1*/,
        TAny* /*aArg2*/,
        TAny* /*aArg3*/ );

private:

    CSendUiWatcher();

    void ConstructL();
 
    /**
     * Requests a timer event after short delay.
     *
     * @since S60 v3.2
     */    
    void SetTimerL();
    
    /**
     * Gets feature status from FeatureManager and updates an internal feature flag.
     *
     * This is used for observing feature status changes and keeping sending service list up-to-date.
     *
     * @since S60 v5.0
     */
    void FeatureStatus( TInt aFeature, TInt &aNewFeatures, TInt aFeatureFlag );

    /**
     * Checks if Mtm count or language has changed and immediate update is needed.
     *
     * @since S60 v3.2
     */
    void CheckIfUpdateNeededL();

private: // data

    /**
     * A channel of communication between a client thread and the Message 
     * Server thread.
     * Own.
     */
    CMsvSession* iSession;

    /**
     * Publish and Subcribe API observer
     * Own.
     */
    CSendUiPropertyObserver* iPropertyObserver;
 
    /**
     * Observes Ecom events. Requests update, if SendUI Ecom service amount
     * changes. 
     * Own.
     */
    CSendUiEcomObserver* iEcomObserver;

    /**
     * Own.
     */    
    CPeriodic* iTimer;
    
    };


#endif // C_CSENDUIWATCHER_H
