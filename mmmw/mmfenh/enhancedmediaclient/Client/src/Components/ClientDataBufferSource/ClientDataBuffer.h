/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the ClientDataBuffer class.
*
*/


#ifndef C_CCLIENTDATABUFFER_H
#define C_CCLIENTDATABUFFER_H

// INCLUDES
#include <e32base.h>
#include <DataBuffer.h>

namespace multimedia
    {
    // CLASS DECLARATION
    /**
     *  Object that Implements the Client Buffers.
     *  This Class Implements the MDataBuffer interface
     *  @lib EnhancedMediaClient.lib
     *  @since S60 v3.2
     */    

    class CClientDataBuffer : public CBase,
                                public MDataBuffer
                            
        {
        public: // Constructors and destructor

            /**
             * Two-phased constructor.
             * @param aObserver Observer to callback the Event.
             */
            static CClientDataBuffer* NewL(TUint aBufferSize);

            /**
            * Destructor.
            */
            ~CClientDataBuffer();

            /**
             * From MDataBuffer.
             * Returns the Descriptor for the Client
             * @since S60 v3.2
             */            
            virtual TPtr8& GetBufferPtr();

            /**
             * From MDataBuffer.
             * Returns a boolean if it is the Last Buffer
             * @since S60 v3.2
             */            
            virtual TBool IsLastBuffer();

            /**
             * From MDataBuffer.
             * Sets the Last Buffer Flag on itself
             * @since S60 v3.2
             */            
            virtual void SetLastBuffer(TBool aStatus);
            
        private:
            CClientDataBuffer();
            void ConstructL(TUint aBufferSize);
            
        private:
            /**
            * Buffer Pointer
            */
            HBufC8* iDataBuffer;
            /**
            * Pointer Descriptor for the Buffer
            */
            TPtr8 iDataBufferDes;
            /**
            * Last Buffer Flag
            */
            TBool iLastBuffer;
        };
    }

#endif // C_CCLIENTDATABUFFER_H

//  End of File
