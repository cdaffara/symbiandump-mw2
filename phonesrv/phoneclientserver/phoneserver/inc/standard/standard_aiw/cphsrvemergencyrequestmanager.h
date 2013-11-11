/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Emergency call request manager.
*
*/


#ifndef CPHSRVEMERGENCYREQUESTMANAGER_H
#define CPHSRVEMERGENCYREQUESTMANAGER_H


// INCLUDES
#include <e32base.h>
#include "mphsrvemergencynegotiators.h" 
#include "mphsrvemergencyinitiators.h" 


// FORWARD DECLARATIONS
class CPhSrvCallRequest;


// CLASS DECLARATION

/**
*  Call request manager.
*
*  @since 1.0
*/
class CPhSrvEmergencyRequestManager : 
    public CBase, 
    public MPhSrvEmergencyNegotiatorCall
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        */
        CPhSrvEmergencyRequestManager();

        /**
        * Destructor.
        */
        ~CPhSrvEmergencyRequestManager();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();


    public: // Functions from base classes

        /**
        * @see MPhSrvEmergencyCallBase
        */
        void EmergencyDialL( const RMessage2& aMessage );
           
        /**
        * @see MPhSrvEmergencyCallBase
        */
        void EmergencyDialCancel();

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorReadyCall( MPhSrvEmergencyCall& aInitiator );

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorReadyCallCancel( MPhSrvEmergencyCall& aInitiator );

        /**
        * @see MPhSrvNegotiatorCall
        */
        void SetNegotiatorAttemptedCallResponseValue( 
            TPhCltPhoneResults aResult );

    private:
        
        /**
        * Does emergency call request
        */
        void DoMakeEmergencyDial( const RMessage2& aMessage );    
                    
    private:    // Data

        // The object responsible for processing external client call requests. 
        // This is essentially an interface to the phone app engine.
        MPhSrvEmergencyCall* iEmergencyInitiator;
        
        // Pending request message pointer 
        RMessagePtr2 iPendingRequestPointer;
        
        // Pending request message 
        RMessage2 iPendingRequest;
        
        // Pending request flag 
        TBool iIsRequestPending;
        
    };


#endif // CPHSRVEMERGENCYREQUESTMANAGER_H


// End of File
