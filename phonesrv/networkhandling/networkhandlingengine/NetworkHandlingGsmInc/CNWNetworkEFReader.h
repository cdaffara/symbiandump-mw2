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
* Description:  This file contains the header file of the cnwcbhzcachereader class.
*
*/



#ifndef CNWNETWORKHEFREADER_H
#define CNWNETWORKHEFREADER_H

//  INCLUDES
#include <rmmcustomapi.h>
#include <e32property.h>
#include "MNWNetworkEFReaderObserver.h"
// CONSTANTS

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Reads Elementary File from TSY
*  
*
*  @lib networkhandling.lib
*  @since Series 60_3.0
*/

NONSHARABLE_CLASS( CNWNetworkEFReader ) : public CActive
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CNWNetworkEFReader* NewL( RMmCustomAPI& aCustomAPI, 
            MNWNetworkEFReaderObserver * aObserver );        
        
        /**
        * Destructor.
        */
        ~CNWNetworkEFReader( );

    private:
        /**
        * C++ consrtructor
        */
        CNWNetworkEFReader( RMmCustomAPI& aCustomAPI, 
            MNWNetworkEFReaderObserver * aObserver );
        
    public:

        TInt StartMonitoringSimCache( MNWNetworkEFReaderObserver * aObserver );

    public: // From CActive
        void RunL();
        void DoCancel();
    
    private:
        void ConstructL();

    private:
         // Custom API for SIM activities.
        RMmCustomAPI &   iCustomAPI;   // Uses only
        // Observe not notify about compelted operation
        MNWNetworkEFReaderObserver * iObserver;
        // cache content
        RMmCustomAPI::TViagCacheRecordContent iRecord;
        RMmCustomAPI::TViagCacheRecordId iCache;
    };

#endif      // CNWNETWORKHEFREADER_H
            
// End of File
