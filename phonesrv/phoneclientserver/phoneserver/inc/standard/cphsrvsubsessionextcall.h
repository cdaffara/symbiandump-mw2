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
* Description:  Sub Session Ext Call.
*
*/

#ifndef CPHSRVSUBSESSIONEXTCALL_H
#define CPHSRVSUBSESSIONEXTCALL_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 
#include <cphcltextphonedialdata.h> 

// CLASS DECLARATION

/**
*  Ext Call Subsession.
*
*  @since 1.0
*/
class CPhSrvSubSessionExtCall : public CPhSrvSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        *
        * @param aSession The session where this subsession belongs.
        */
        CPhSrvSubSessionExtCall( CPhSrvSession& aSession );


    private: // Functions from base classes

        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );

        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );


    private: // SubSession function handlers

        /**
        * Close subsession
        *
        * @param aMessage Message to be processed.
        */
        void CmdSubSessionCloseL( const RMessage2& aMessage );
        
        /**
        * Makes call
        *
        * @param aMessage Message to be processed.
        */
        void CmdSubSessionMakeCallL( const RMessage2& aMessage );
        
        /**
        * Cancels call
        *
        * @param aMessage Message to be processed.
        */
        void CmdSubSessionMakeCallCancelL( const RMessage2& aMessage );
        
        /**
        * Creates call argument object
        *
        * @param aMessage Message to be processed.
        */
        void CreateCallArgsL( const RMessage2& aMessage );
        
        /**
        * Finds chars 'p' and 'w' from input string
        *
        * @param aString Input string (telephone number).
        * @return returns true if number contains chars 'p' or 'w'
        */
        TBool IsDtmfPhoneNumber( const TDesC& aString );

    private:     // Data

        // Indicates whether we have an pending request.
        TBool       iHavePendingDialRequestPointer;
    
        // Initialized when the client initiates an dial request. This object 
        // will complete the request when the phone app has responded with 
        // the result of the dial.
        RMessage2   iPendingDialRequestPointer;

        // Call arguments
        CPhCltExtPhoneDialData* iCallArgs;
        
    };

#endif // CPHSRVSUBSESSIONEXTCALL_H


// End of File
