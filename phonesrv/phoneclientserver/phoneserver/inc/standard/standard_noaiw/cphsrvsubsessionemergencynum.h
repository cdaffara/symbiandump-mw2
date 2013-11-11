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
* Description:  Emergency Number Subsession.
*
*/

#ifndef CPHSRVSUBSESSIONEMERGENCYNUM_H
#define CPHSRVSUBSESSIONEMERGENCYNUM_H


// INCLUDES
#include "cphsrvsubsessionbase.h" 


// CLASS DECLARATION

/**
*  Emergency Number subsession.
*
*  @since 1.0
*/
class CPhSrvSubSessionEmergencyNum : public CPhSrvSubSessionBase
    {
    public:  // Constructors and destructor

        /**
        * Constructor
        *
        * @param aSession The session where this subsession belongs. 
        */
        CPhSrvSubSessionEmergencyNum( CPhSrvSession& aSession );

        /**
        * @see CPhSrvSubSessionBase
        */
        void ConstructL();


    private:
        /**
        * @see MPhSrvMessageDecoder
        */
        TBool PhSrvMessageDecoderCanProcessMessage( TInt aFunction );

        /**
        * @see MPhSrvMessageProcessor
        */
        void PhSrvMessageProcessorHandleMessageL( const RMessage2& aMessage );

    private: // SubSession function handlers

        void CmdSubSessionCloseL( const RMessage2& aMessage );

        void CmdSubSessionIsEmergencyNumberL( const RMessage2& aMessage );

    };

#endif // CPHSRVSUBSESSIONEMERGENCYNUM_H


// End of File
