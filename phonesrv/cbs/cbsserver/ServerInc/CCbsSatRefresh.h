/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitors sat refresh for cbsserver
*
*/



#ifndef CCBSSATREFRESH_H
#define CCBSSATREFRESH_H

//  INCLUDES
#include <etelmm.h>
#include <msatrefreshobserver.h>


// FORWARD DECLARATIONS
//class CNWGsmMessageHandler;
class RSatSession;
class RSatRefresh;
class RSystemAgent;
class CCbsServer;

// CLASS DECLARATION

/**
*  Monitors sat refresh for cbsserver
*
*/
class CCbsSatRefresh : public CBase, public MSatRefreshObserver
    {
    public:  // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CCbsSatRefresh* NewL( CCbsServer& aCbsServer );
        
        /**
        * Destructor.
        */
        virtual ~CCbsSatRefresh();

              
    private:

        /**
        * Executes the NotifyNetworkConnectionFailure function.
        * 
        */
        void IssueRequest();
        
          /**
        * C++ default constructor.
        */
        CCbsSatRefresh( CCbsServer& aCbsServer );
                                  
                                  
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
        TBool AllowRefresh( TSatRefreshType aType, const TSatRefreshFiles& aFiles );

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
    // Reference to cbsserver
	CCbsServer& iCbsServer;
	// Sat session connection.
	RSatSession* iSatSession;
	// Sat refresh connection
	RSatRefresh* iSatRefresh;
	// System agent connection
	RSystemAgent* iSystemAgent;
   	// Is refresh allowed to do.
    TBool iAllowRefresh;	
    };

#endif      // CCbsSatRefresh_H   



// End of File
