/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     Global functions for the landmarks server
*
*/


#ifndef LMSERVERGLOBAL_H
#define LMSERVERGLOBAL_H

#include <e32std.h>

/**
*  LmServerGlobal functions for the server
*
*/
class LmServerGlobal
    {
    public: // New functions

        /**
        * Read and kill the client if not successful. If the returned error
        * code is negative, the RMessage2 must not be completed. Complete after
        * client is panicked crashes the server with KERN-EXEC 44.
        *
        * @param[in] aMessagePtr Message pointer
        * @param[in] aParam Index to read from
        * @param[out] aDes Descriptor to read to
        * @param[in] aOffset Offset to start read
        * @return Error code
        */
        static TInt Read(
            const RMessagePtr2& aMessagePtr,
            TInt aParam,
            TDes8& aDes,
            TInt aOffset = 0 );

        /**
        * Read and kill the client if not successful. If the returned error
        * code is negative, the RMessage2 must not be completed. Complete after
        * client is panicked crashes the server with KERN-EXEC 44.
        *
        * @param[in] aMessagePtr Message pointer
        * @param[in] aParam Index to read from
        * @param[out] aDes Descriptor to read to
        * @param[in] aOffset Offset to start read
        * @return Error code
        */
        static TInt Read(
            const RMessagePtr2& aMessagePtr,
            TInt aParam,
            TDes16& aDes,
            TInt aOffset = 0 );

        /**
        * Write and kill the client if not successful. If the returned error
        * code is negative, the RMessage2 must not be completed. Complete after
        * client is panicked crashes the server with KERN-EXEC 44.
        *
        * @param[in] aMessagePtr Message pointer
        * @param[in] aParam Index to write to
        * @param[in] aDes Descriptor to write from
        * @param[in] aOffset Offset to start write
        * @return Error code
        */
        static TInt Write(
            const RMessagePtr2& aMessagePtr,
            TInt aParam,
            const TDesC8& aDes,
            TInt aOffset = 0 );

        /**
        * Write and kill the client if not successful. If the returned error
        * code is negative, the RMessage2 must not be completed. Complete after
        * client is panicked crashes the server with KERN-EXEC 44.
        *
        * @param[in] aMessagePtr Message pointer
        * @param[in] aPtr Pointer to write to
        * @param[in] aDes Descriptor to write from
        * @param[in] aOffset offset to start write
        * @return Error code
        */
        static TInt Write(
            const RMessagePtr2& aMessagePtr,
            TInt aParam,
            const TDesC16& aDes,
            TInt aOffset = 0 );

        /**
        * Completes a client message with a reason.
        *
        * @param[in] aMessagePtr Message pointer
        * @param[in] aReason A reason
        */
        static void Complete(
            const RMessagePtr2& aMessagePtr,
            TInt aReason );

        /** Reads data from message 
         *  @param[in] aMessage IPC message
         *  @param[in] aClientBufferParam parameter to read
         *  @return pointer to read buffer, caller takes ownership */
        static HBufC8* CopyClientBuffer8LC(
            const RMessage2& aMessage, 
            const TInt aClientBufferParam );

        /** Reads data from message 
         *  @param[in] aMessage IPC message
         *  @param[in] aClientBufferParam parameter to read
         *  @return pointer to read buffer, caller takes ownership */
        static HBufC* CopyClientBufferLC(
            const RMessage2& aMessage, 
            const TInt aClientBufferParam);

    private:

        // By default, prohibit copy constructor
        LmServerGlobal( const LmServerGlobal& );
        // Prohibit assigment operator
        LmServerGlobal& operator= ( const LmServerGlobal& );
    };

#endif      // LMSERVERGLOBAL_H

// End of File
