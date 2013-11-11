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



#ifndef CNWNETWORKVIAGREFRESH_H
#define CNWNETWORKVIAGREFRESH_H

//  INCLUDES
#include "CNWNetworkViagBaseEngine.h"
#include <etelmm.h>
#include <msatrefreshobserver.h>



// FORWARD DECLARATIONS
//class CNWGsmMessageHandler;
class CNWNetworkViagBaseEngine;
class RSatSession;
class RSatRefresh;
class RSystemAgent;
class CNWNetworkViagSimReader;

// CLASS DECLARATION

/**
*  Monitors network connection failure.
*
*  @lib Networkhandling.lib
*  @since Series 60_2.8
*/
class CNWNetworkViagRefresh : public CBase, MSatRefreshObserver
    {
    public:  // Constructors and destructor
        
                                  
       
        /**
        * Two-phased constructor.
        */
        static CNWNetworkViagRefresh* NewL(
                        CNWNetworkViagBaseEngine& aBase,
                        RMmCustomAPI& aCustomAPI,
                        CNWNetworkViagSimReader& aViagSimReader,
                        TBool& iReadingOnGoing
                        );
        
        /**
        * Destructor.
        */
        virtual ~CNWNetworkViagRefresh();

         /**
        * Notifies SAT server when all Elemenatary Files are read. 
        * @since 3.0
        * @param None.
        * return None.
        */
        void RefreshEFRead();
              
    private:

        /**
        * Executes the NotifyNetworkConnectionFailure function.
        * 
        */
        void IssueRequest();
        
          /**
        * C++ default constructor.
        */
        CNWNetworkViagRefresh( CNWNetworkViagBaseEngine& aBase,
                                  RMmCustomAPI& aCustomAPI,
                                  CNWNetworkViagSimReader& aViagSimReader,
                                  TBool& iReadingOnGoing
                                   );
                                  
                                  
          /**
        * Refresh query. Client should determine whether it allow the
        * refresh to happen. In this method the client should not close or
        * cancel the RSatRefresh subsession.
        * @since 2.6
        * @param aType Refresh type.
        * @param aFiles Elementary files which are to be changed. 
        * May be zero length. 
        * @return ETrue to allow refresh, EFalse to decline refresh.
        */
        TBool AllowRefresh( TSatRefreshType aType, 
                const TSatRefreshFiles& aFiles );

         /**
        * Notification of refresh. In this method the client should not close 
        * or cancel the RSatRefresh subsession.
        * @since 2.6
        * @param aType Type of refresh which has happened.
        * @param aFiles List of elementary files which has been changed. 
        * May be zero length.
        */
        void Refresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );
        
        
         /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private:    // Data
        // Reference to base
        CNWNetworkViagBaseEngine& iBase;
        // Reference to CustomAPI
        RMmCustomAPI& iCustomAPI;
        // Reference to sim viag reader
        CNWNetworkViagSimReader& iViagSimReader;
        // customapi file info packet
        RMmCustomAPI::TSimFileInfoPckg iSimFileInfoPckg;
        // Customapi sim file info
        RMmCustomAPI::TSimFileInfo iSimFileInfo;
        // Sat session connection.
        RSatSession* iSatSession;
        // Sat refresh connection
        RSatRefresh* iSatRefresh;
        // System agent connection
        RSystemAgent* iSystemAgent;
        // reference to Reading OnGoing boolean
        TBool& iReadingOnGoing;
    };

#endif      // CNWNETWORKVIAGREFRESH_H   



// End of File
