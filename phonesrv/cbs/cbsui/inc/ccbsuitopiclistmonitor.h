/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/


#ifndef  CCBSUITOPICLISTMONITOR_H
#define  CCBSUITOPICLISTMONITOR_H

// INCLUDES
#include <e32base.h>
#include <CbsCommon.h>

 
//  FORWARD DECLARATIONS
class MCbs;
class MCbsUiTopicMonitorObserver;

//  CLASS DEFINITIONS 


/**
*  Monitors topic related events.  
*/
class CCbsUiTopicListMonitor : public CActive
    {
    public: // New functions

        /**
        * Create an instance of the class. 
        *
        * @param aServer cbs server
        */
        static CCbsUiTopicListMonitor* NewL( MCbs& aServer );

        /**
        * Destructor.
        */
        ~CCbsUiTopicListMonitor();

        /**
        * Issues the request.
        */
        void IssueRequest();

    public: // From CActive
        
        /**
        * It is called when server notifies.
        */
        void RunL();

        /**
        * Cancels the request.
        */
        void DoCancel();

    public: // New functions

        /**
        * Adds a observer to this monitor.
        *
        * @param aObserver Observer to be added.
        */
        void AddObserverL( MCbsUiTopicMonitorObserver* aObserver );

    private:

        typedef CArrayFixFlat<MCbsUiTopicMonitorObserver*>
            TCbsUiMonitorObservers;

        /**
        * Constructor.
        *
        * @param aServer cbs server
        */
        CCbsUiTopicListMonitor( MCbs& aServer );

        /**
        * Finalizes the construction.
        */
        void ConstructL();

    private: // Data

        // ETrue if request is active.
        TBool iContinue;

        // To issue the request.
        MCbs& iServer;

        // Observers who are interested in topic changes.
        // Monitor doesn't own the observers.
        TCbsUiMonitorObservers* iObservers;

        // Which topic handle the event is conserning.
        TCbsTopicNumber iTopicNumber;

        // What kind of event has been.
        TCbsTopicListEvent iEvent;
    };

#endif      //  CCBSUITOPICLISTMONITOR_H
            
// End of File
