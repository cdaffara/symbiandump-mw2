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
* Description:  Monitors network connection failure.
*
*/



#ifndef CNWNETWORKVIAGSIMREADER_H
#define CNWNETWORKVIAGSIMREADER_H

#include "CNWNetworkViagBaseEngine.h"
#include <etelmm.h>


// FORWARD DECLARATIONS
// None.

// CLASS DECLARATION
// None.

/**
*  Read Elementary files from SIM.
*
*  @lib Networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkViagSimReader : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagSimReader();


    protected:  // Functions from CActive
        
        /**
        * Cancels asyncronous request(s).
        * 
        */
        void DoCancel();

        /**
        * Informs object that asyncronous request is ready.
        * 
        */
        void RunL();
        
       
        
    private:

        /**
        * Executes the NotifyNetworkConnectionFailure function.
        * 
        */

        
          /**
        * C++ default constructor.
        */
        CNWNetworkViagSimReader( CNWNetworkViagBaseEngine& aOwner,
                                  RMmCustomAPI& aCustomAPI );

    public:

      
                                   
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagSimReader* NewL(
                        CNWNetworkViagBaseEngine& aMessageHandler,
                        RMmCustomAPI& aCustomAPI
                        );
         /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
         /**
        * Reads sim file based on elementary files . 
        * @since 2.8
        * @param None.
        * return None.
        */
        void  ReadSimFile( );
         
         /**
        * Set elementary file to array. 
        * @since 2.8
        * @param aElementFile  Element file for reading. 
        * return None.
        */
        void SetElementFile( TNWNetworkElementFile aElementFile );

    private:    // Data
        // Reference to Owner of the component
        CNWNetworkViagBaseEngine& iOwner;
        // Reference to CustomApi 
        RMmCustomAPI& iCustomAPI;
        // Customapi file info packet
        RMmCustomAPI::TSimFileInfoPckg iSimFileInfoPckg;
        // Customapi sim file info 
        RMmCustomAPI::TSimFileInfo iSimFileInfo;
        // Readed data from SIM Elementary file. 
        TBuf8<700>    iResponse;    
        // Array of the element file to refresh 
        RArray<TNWNetworkElementFile> iElementFiles;
    
    };

#endif      // CNWNETWORKVIAGSIMREADER_H   
            
// End of File
