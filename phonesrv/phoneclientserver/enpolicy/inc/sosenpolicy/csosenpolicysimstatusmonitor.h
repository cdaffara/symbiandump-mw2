/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitoring status of sim for emergency numbers.
*
*/



#ifndef CSOSENPOLICYSIMSTATUSMONITOR_H
#define CSOSENPOLICYSIMSTATUSMONITOR_H


// INCLUDES
#include "csosemergencynumberpolicyhandler.h" 

#include <e32base.h>            // CActive
#include <e32property.h>        // RProperty


// FORWARD DECLARATIONS
class CSosEmergencyNumberPolicyHandler;
class RSystemAgent;
class TSysAgentEvent;


// CLASS DECLARATION

/**
*  It is the SIM monitor class for Emergency number manager.
*
*  @since Series60_2.6 
*/
NONSHARABLE_CLASS( CSosEnPolicySimStatusMonitor ) : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Creates a new monitor and starts it.
        *
        * @param aObserver The observer class of the SIM status changes.
        * @return Pointer to created CSosEnPolicySimStatusMonitor instance.
        */
        static CSosEnPolicySimStatusMonitor* NewL( 
            CSosEmergencyNumberPolicyHandler& aObserver );
       
        /**
        * Destructor.
        */
        ~CSosEnPolicySimStatusMonitor();

    public: // New functions
        /** 
        * Is SIM reading allowed?
        *
        * @return Returns ETrue, if SIM reading is allowed.
        */
        TBool IsReadingAllowed();


    private: // Functions from base classes

        /** 
        * @see CActive::RunL() 
        */
        void DoCancel();

        /** 
        * @see CActive::RunL() 
        */
        void RunL();
        

    private: // New functions

        /** 
        * Requests notifications from system agent.
        */
        void IssueRequest();
        
        /** 
        * Translates system agent's state information.
        * 
        * @param aState This is the state from SA.
        */
        TBool TranslateState( TInt aState );

    private:
        /**
        * C++ constructor.
        * @param aObserver The observer class of the SIM status changes.
        */
        CSosEnPolicySimStatusMonitor( CSosEmergencyNumberPolicyHandler& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();


    private:  // Data

        // The Observer for events.
        CSosEmergencyNumberPolicyHandler& iObserver;

        // The RProperty to replace System agent.
        RProperty                         iProperty;

    };

#endif  // CSOSENPOLICYSIMSTATUSMONITOR_H


// End of File
