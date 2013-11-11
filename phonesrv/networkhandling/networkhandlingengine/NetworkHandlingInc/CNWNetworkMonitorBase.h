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
* Description:  This file contains the header file of the CNWNetworkMonitorBase class.
*
*/


#ifndef CNWNETWORKMONITORBASE_H
#define CNWNETWORKMONITORBASE_H

//  INCLUDES
#include <nwhandlingengine.h>


// FORWARD DECLARATIONS
class RMmCustomAPI;
class RMobilePhone;
class CNWMessageHandler;

// CLASS DECLARATION

/**
*  Base class for network monitors.
*  
*
*  @lib networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkMonitorBase : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkMonitorBase();

    public: // New functions
        
        /**
        * Is monitor initialised. Is notification completed at least once.
        * 
        * @return  ETrue if initialised.
        */
        TBool IsInitialised() const;

        /**
        * Starts initialization ( lanuches notify requests )
        * 
        */
        void Initialise();

    protected:  // New functions
    
        /**
        * C++ default constructor.
        */
        CNWNetworkMonitorBase( CNWMessageHandler& aOwner,
                               RMobilePhone& aPhone,
                               TNWInfo& aNetworkInfo,
                               RMmCustomAPI& aCustomAPI );

    protected:  // Functions from CActive
       
        /**
        * Cancels asyncronous request(s).
        * 
        */
        virtual void DoCancel() = 0;

        /**
        * Pure virtual function of request function.
        * 
        */
        virtual void IssueRequest() = 0;

        /**
        * Informs object that asyncronous request is ready.
        * 
        * @return None.
        */
        virtual void RunL() = 0;

    protected:  // Data
        // Reference to the owner of this instance.
        CNWMessageHandler&  iOwner;
        // Reference to the phone connection.
        RMobilePhone&       iPhone;
        // Reference to the network info structure.
        TNWInfo&            iNWNetworkInfo;
        //Reference to the customAPI connection
        RMmCustomAPI&       iCustomAPI;
        // Tells if monitor is initialized.
        TBool               iIsInitialised;
    };

#endif      // CNWNETWORKMONITORBASE_H
            
// End of File
