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
* Description:  Global services for LBT server
*
*/


#ifndef LBTGLOBAL_H
#define LBTGLOBAL_H

//  INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Global functions for the server
*  
*/
class LbtGlobal
	{
	public:
	
	   /**
        * Writes to the message and panics the client if any error
        * If the returned error code is negative, the RMessage must 
        * not be completed.
        *
        * @param aMessage Message instance
        * @param aPtr Pointer to write to
        * @param aDes Descriptor to write from
        * @param aOffset offset to start write
        * @return Error code
        */
        
        static TInt Write(
        /* IN  */		const RMessage2& aMessage,
        /* IN  */		const TInt aParam,
        /* IN  */		const TDesC8& aDes,
        /* IN  */		TInt aOffset=0
            );

		/**
         * Read and kill the client if it leaves. If the returned error code 
         * is negative, the RMessage must not be completed. Complete after 
         * client is paniced crashes the server with USER 70.
         *
         * @param aMessage Message instance
         * @param aPtr Pointer to read from
         * @param aDes Descriptor to read to
         * @param aOffset offset to start read
         * @return Error code
         */
        static TInt Read(
        /* IN  */       const RMessage2& aMessage,
        /* IN  */		const TInt aParam,
        /* OUT */       TDes8& aDes,
        /* IN  */       TInt aOffset=0
            );
        
        /**
        * Copies an 8-bit buffer from the address space of the client and puts 
        * the result in the returned argument. If the method leaves it the 
        * client may have been paniced and in that case it is forbidden to
        * complete the RMessage. If a client is paniced can be verified by the
        * call chain:
        * aMessage.Client().ExitType()
        *
        * @param aMessage the message from the client.
        * @param aClientBuffer a pointer to an 8-bit descriptor located in the 
        *        address space of the client.
        * @return a copy of the client buffer.
        */
        static HBufC8* CopyClientBuffer8LC(
        /* IN  */   					  const RMessage2& aMessage,
        /* IN  */   					  const TInt aClientBufferParam
            							  );
            							  
		static void RequestComplete(
		/* IN  */   				const RMessage2& aMessage,
        /* IN  */   				const TInt aError
            		 	    	   );
	};


#endif // LBTGLOBAL_H

// end of file
