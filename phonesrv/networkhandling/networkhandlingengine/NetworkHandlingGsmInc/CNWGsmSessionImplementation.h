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
* Description:  Gsm specific interface class for network handling
*
*/



#ifndef CNWGSMSESSIONIMPLEMENTATION_H
#define CNWGSMSESSIONIMPLEMENTATION_H

//  INCLUDES
#include <cnwsession.h>
#include <nwhandlingengine.h>


// FORWARD DECLARATIONS
class CNWGsmMessageHandler;

// CLASS DECLARATION


/**
*  This file contains the header file of the class CNWGsmSessionImplementation, 
*  which is a Gsm specific interface class to network properties of the system
*
*  @lib NetworkHandling.lib
*  @since Series 60_2.8
*/
class CNWGsmSessionImplementation : public CNWSession
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CNWGsmSessionImplementation* NewL( 
                MNWMessageObserver& aMessageObserver,
                TNWInfo& aNWInfo );
                
        /**
        * Destructor.
        */
        IMPORT_C virtual ~CNWGsmSessionImplementation();

    private:

        /**
        * C++ default constructor.
        */
        CNWGsmSessionImplementation( 
                MNWMessageObserver& aMessageObserver,
                TNWInfo& aNWInfo );


        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

    private:
        // CNWGsmMessageHandler object which handles messages
        CNWGsmMessageHandler* iGsmMessageHandler;
        //Reference to MNWMessageObserver
        MNWMessageObserver& iMessageObserver;
        //Reference to network info struct
        TNWInfo& iNWInfo;
    };

#endif      // CNWGsmSessionImplementation_H

// End of File
