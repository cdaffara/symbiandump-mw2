/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This class represents the base class for all server-side subsession
*                classes.
*
*/


#ifndef     CCBSOBJECT_H
#define     CCBSOBJECT_H

//  INCLUDES

#include <e32base.h>
#include "CbsCommon.h"

//  FORWARD DECLARATIONS
class CCbsSession;

//  CLASS DECLARATION 

/**
*   CCbsSession is the base class for the classes that represent
*   server-side subsessions. 
*
*   It provides some convience functions is remove redundancy of
*   subsession classes.
*/
class CCbsObject : public CObject
    {
    public:     // New functions
        /**
        *   Constructor.
        *   
        *   @param aSession                 Pointer to the session.
        */  
        CCbsObject( CCbsSession& aSession );

        /**
        *   Destructor.
        */
        ~CCbsObject();

        /**
        *   Handles the requests for the object.
        *    
        *   Implemented in subsession classes.
        *
        *   @param  aMessage            Request to be handled.
        *   @return                     Boolean value indicating whether 
        *                               the request was handled.
        */
        virtual TBool HandleRequestsL( const RMessage2& aMessage ) = 0;

        /**
        *   Returns the message of current client request.
        *
        *   @return                     The message object of the current
        *                               client request.
        */
        const RMessage2& Message();

        /** 
        *   Returns a reference to the session.
        *
        *   @return                     Main session object.
        */
        CCbsSession& Session();

        /**
        *   Panics the client.
        *
        *   @param aPanic                Reason for panic.
        */
        void PanicClient( TCbsSessionPanic aPanic ) const;

    private:    // Data

        // The session to which this object belongs.
        CCbsSession& iSession;

    };

#endif      //  CCBSOBJECT_H   
            
// End of File


