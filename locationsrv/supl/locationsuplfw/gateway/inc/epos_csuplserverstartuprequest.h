/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*    Active object and base class for subclasses implementing a SUPL session 
*    request cycle.
*
*/



#ifndef __CSuplServerStartupRequest_H__
#define __CSuplServerStartupRequest_H__

// INCLUDE FILES
#include <e32base.h>


#include "epos_msuplecomeventobserver.h"

// CONSTANT DECLARATIONS

// FORWARD DECLARATIONS
class MConnectObserver;
// CLASS DECLARATION

/**
*    Active object implementing a position request cycle. Handles
*    activities such as controlling logging and verifying privacy.
*
*/
class CSuplServerStartupRequest : public CActive
    {
    public:  // Constructors and destructor
   
        /**
        * Two-phased constructor.
        */
        static CSuplServerStartupRequest* NewL(MConnectObserver* aObserver,CSuplSessionManager& aSessionManager);

        /**
        * Destructor.
        */
        virtual ~CSuplServerStartupRequest();

    public:  // New functions

        /**
        * Starts a SUPL session request cycle. 
        * @param aMessage the request message from the client
        */

        void MakeSuplServerStartupRequestL(CSuplSessionManager* aSessionManager,TUid aPluginUid,MSuplEcomEventObserver& aSuplEcomEventObserver);
        /**
        * Called when the server class is shutting down.
        */
        

		
		
    protected:  // From CActive

        void RunL();
        TInt RunError(TInt aError);
        void DoCancel();

    private:

        CSuplServerStartupRequest(MConnectObserver* aObserver,CSuplSessionManager& aSessionManager);

        void ConstructL();

        // By default, prohibit copy constructor
        CSuplServerStartupRequest( const CSuplServerStartupRequest& );
        // Prohibit assigment operator
        CSuplServerStartupRequest& operator= ( const CSuplServerStartupRequest& );
 		
	
    private:  // Data

        enum TSuplServerStartupRequestStage
            {
            ESuplReqInactive = 0,
            ESuplServerStartup,
            };

     
	 TSuplServerStartupRequestStage  iReqPhase;
	 MConnectObserver *iObserver;
     CSuplSessionManager&        iSessionManager;
	
    };  

#endif  // __CSuplServerStartupRequest_H__

// End of File
