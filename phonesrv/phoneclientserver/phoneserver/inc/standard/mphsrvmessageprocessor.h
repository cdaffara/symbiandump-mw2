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
* Description:  Message Processor interface class.
*
*/


#ifndef MPHSRVMESSAGEPROCESSOR_H
#define MPHSRVMESSAGEPROCESSOR_H


// INCLUDES
#include <e32base.h>


// CLASS DECLARATION

/**
*  Abstract message decoder class.
*
*  @since 1.0 
*/
class MPhSrvMessageDecoder
    {
    public: // New functions

        /**
        * Returns ETrue if this object can understand the specified
        * message function.
        *
        * @return Whether this object can understand the specified 
        *         client/server operation.
        */
        virtual TBool PhSrvMessageDecoderCanProcessMessage( 
            TInt aFunction ) = 0;
    };



// CLASS DECLARATION

/**
*  Abstract message processor class.
*
*  @since 1.0 
*/
class MPhSrvMessageProcessor : public MPhSrvMessageDecoder
    {
    public: // New functions

        /**
        * Process the specified client/server message.
        *
        * @param aMessage Message to be processed.
        */
        virtual void PhSrvMessageProcessorHandleMessageL( 
            const RMessage2& aMessage ) = 0;
    };


#endif // MPHSRVMESSAGEPROCESSOR

       
// End of File
