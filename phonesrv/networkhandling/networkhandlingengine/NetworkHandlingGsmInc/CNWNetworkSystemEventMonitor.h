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
* Description:  Sim monitoring class definition
*
*/



#ifndef __CNWNETWORKSYSTEMEVENTMONITOR_H
#define __CNWNETWORKSYSTEMEVENTMONITOR_H

//  INCLUDES

#include <e32base.h>

#include <networkhandlingdomainpskeys.h>
#include <PSVariables.h>

// CONSTANTS
// None.

// MACROS
// None.

// DATA TYPES
// None.

// FUNCTION PROTOTYPES
// None.

// FORWARD DECLARATIONS
// None.

// CLASS DECLARATION
class CNWNetworkViagBaseEngine;

/**
*  This class monitors Sim state changes from System Agent.
*
*  @lib ?library
*  @since ?Series60_version
*/
class CNWNetworkSystemEventMonitor : public CActive
    {

    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CNWNetworkSystemEventMonitor* NewL( 
                        CNWNetworkViagBaseEngine& aBase, 
                        TUid aUid, 
                        TInt aStatusValue );
       
      

        /**
        * Destructor.
        */
        virtual ~CNWNetworkSystemEventMonitor();

    public: // New functions

        /**
        * Starts monitoring
        * 
        */
        void Start( );

    private: // Constructors 

        /**
        * C++ default constructor.
        */
        CNWNetworkSystemEventMonitor( 
            CNWNetworkViagBaseEngine& aBase,TUid aUid, TInt aStatusValue );
        /**
        * By default EPOC constructor is private.
        */
        void ConstructL(  );


    private: // From CActive
        /**
        * Implements cancellation of an outstanding request.
        */
        void DoCancel();

        /**
        * Handles request completion event.
        */
        void RunL();

    private: // Data
        CNWNetworkViagBaseEngine& iBase;
        
        // Used in RunL to get Sim state in startup
        TBool iStartUp;
        RProperty iProperty;    
        TInt iState;  
        TUid iUid;
    
        TInt iStatusValue;    
    };

#endif      //  __CNWNetworkSystemEventMonitor_H
// End of File

