/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Container of event monitors
*
*/



#ifndef CSATEVENTMONITORCONTAINER_H
#define CSATEVENTMONITORCONTAINER_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include "MSatEventMonitorUtils.h"
#include "MSatEventMonitorContainer.h"
#include "MSatEventObserver.h"


// FORWARD DECLARATIONS
class MSatEventMonitor;
class TUSatAPI;
class MSatUtils;
class MSatBIPEventNotifier;

/**
*  Base class for SAT commands.
*
*  @lib SatServer
*  @since Series 60 3.0
*/
class CSatEventMonitorContainer : public CActive,
                                  public MSatEventMonitorContainer,
                                  public MSatEventMonitorUtils,
                                  public MSatEventObserver
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aUsatAPI Interface for EventDownload commands
        * @param aSatUtils Interface for SAT Utils class.
        */
        static CSatEventMonitorContainer* NewL(
            TUSatAPI& aUsatAPI,
            MSatUtils& aSatUtils );

        /**
        * Destructor.
        */
        virtual ~CSatEventMonitorContainer();

    public: // Functions from parent class

        /**
        * From MSatEventObserver
        */
        void Event( TInt aEvent );

        /**
        * From MSatEventMonitorContainer Updates monitors state.
        * @param aEvents Events that should be monitored.
        * @return KErrNone if all goes OK
        */
        TInt SetUpEventMonitors( TUint aEvents );

        /**
        * From MSatEventMonitorContainer Cancels all monitors
        * @return KErrNone if all monitros were cancelled
        */
        TInt CancelAllMonitors();

        /**
        * From MSatEventMonitorContainer Solves language code.
        * @param aLanguageId Resolved language ID
        */
        void LanguageSettingL( TInt& aLanguageId );

        /**
        * From MSatEventMonitorUtils Sends notify to SIM about some event
        * using EventDownload command.
        * @param aSingleEvent Single event reported to SIM.
        * @param aFirstArg First argument interpreted according to type of event
        * @param aSecArg Second argument interpreted according to type of event
        */
        void EventDownload( RSat::TEventList aSingleEvent,
                            TInt aFirstArg,
                            TInt aSecArg );

        /**
        * From MSatEventMonitorUtils Returns reference to
        * BIP Event notifier class.
        * @return Interface for BIP Utils class.
        */
        MSatBIPEventNotifier& BipNotifier();

        /**
        * From MSatEventMonitorContainer Called when SIM is removed.
        * This causes all monitors to be cancelled.
        */
        void SimRemoved();

        /**
        * From MSatEventMonitorContainer Gets the ISO639 language code.
        * @param aId Language ID
        * @param aText Language code can be read from this after function call.
        */
        void GetLanguageString( const TInt aId, TDes& aText );

        /**
        * From CActive Checks that EventDownload were succesful
        */
        void RunL();

        /**
        * From CActive Cancels EventDownload
        */
        void DoCancel();
        
        /**
        * From MSatEventMonitorUtils 
        * Writes time between language selection and reboot.
        * @param aTime Time before reboot.
        * @return KErrNone if language selection time is set.
        */
        TInt WriteLanguageSelectionTime( TInt aTime );

    private:

        /**
        * C++ default constructor.
        * @param aUsatAPI Interface for EventDownload commands
        * @param aSatUtils Interface for SAT Utils class.
        */
        CSatEventMonitorContainer( TUSatAPI& aUsatAPI, MSatUtils& aSatUtils );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:

        /**
        * Handles Language Selection event and sends event download
        * @param aLanguageId ID read from the P&S. Comes from monitor.
        */
        void LanguageSelectionEvent( TInt aLanguageId );

        /**
        * Handles the browser termination event and sends event download.
        * @param aCause The cause of the browser termination
        */
        void BrowserTerminationEvent( TInt aCause );

        /**
        * Handles Data avaliable event and sends event download
        * @param aChannelId Data channel identifier. Comes from monitor.
        */
        void DataAvailableEvent( const TInt aChannelId, const TInt aLength );

        /**
        * Handles Channel status event and sends event download
        * @param aChannelId Data channel identifier. Comes from monitor.
        * @param aStatus Status of the data channel.
        */
        void ChannelStatusEvent( const TInt aChannelId, const TInt aStatus );

        /**
        * Checks if aEvents contains only events that
        * are supported.
        * @param aEvents Events from SIM.
        * @return ETrue if aEvents are supported.
        */
        TBool EventsSupported( TUint aEvents ) const;

        /**
        * Removes single event from event list
        * @param aSingleEvent Event to remove
        */
        void RemoveSingleEvent( RSat::TEventList aSingleEvent );

    private:    // Data

        // Ponter array containing event monitors
        CArrayPtrFlat<MSatEventMonitor>* iEventMonitors;

        // Interface to EventDownload command sending
        TUSatAPI& iSat;

        // Indecates is there event pending
        TBool iEventPending;

        // Pending event
        RSat::TEventList iPendingEvent;

        // Pending first argument
        TInt iPendingFirstArg;

        // Pending second argument
        TInt iPendingSecondArg;

        // Utils class for BIP event monitors
        MSatUtils& iSatUtils;

        // Indicates whether there is BIP command executing or not
        TBool iBipExecuting;

        // Removable event after event download
        TInt iRemovableSingleEvent;

    };

#endif      // CSATEVENTMONITORCONTAINER_H

// End of File
