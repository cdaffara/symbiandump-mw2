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
* Description:  Definition of the DataBufferQueue Item
*
*/


#ifndef C_CDATABUFFERQUEUEITEM_H
#define C_CDATABUFFERQUEUEITEM_H

//  INCLUDES
#include <e32base.h>

namespace multimedia
    {
    class MDataBuffer;
    class CDataBufferQueueItem;
    
    // Observer
    /**
     *  Observer that monitors the BufferProcessed Event from the Source Side.
     *  @lib EnhancedMediaClient.lib
     *  @since S60 v3.2
     */    
     class MBufferObserver
        {
        public:
            virtual void BufferProcessed( CDataBufferQueueItem* aItem ) = 0;
            virtual void HandleCancel( CDataBufferQueueItem& aItem ) = 0;
        };
    
    /**
     *  DataBufferQueueItem which encapsulates the Client Buffers.
     *  @lib EnhancedMediaClient.lib
     *  @since S60 v3.2
     */    
    class CDataBufferQueueItem : public CActive
        {
        public:  // Constructors and destructor
            /**
            * Function to Create the Object.
            * @param aObserver Observer to callback the Event.
            */
            static CDataBufferQueueItem* NewL( MBufferObserver& aObserver, 
                                                MDataBuffer& aBuffer,
                                                TBool aLastBuffer,
                                                TUint aBufSeqNum );
            
            /**
            * Destructor.
            */
            virtual ~CDataBufferQueueItem();
            
            /**
            * From CActive.
            * Indicates that this active object has issued a
            * request and that it is now outstanding 
            *
            * @since S60 v3.2
            */    
            void SetActive();
            
            /**
            * From CDataBufferQueueItem.
            * Returns the DataBuffer Stored on in the Queue Item
            *
            * @since S60 v3.2
            */            
            MDataBuffer& DataBuffer();
            
            /**
            * From CDataBufferQueueItem.
            * Returns the Error Stored on Completion of the Request
            *
            * @since S60 v3.2
            */            
            TInt Error();
            
            /**
            * From CDataBufferQueueItem.
            * Returns the Last Buffer status of the Buffer
            *
            * @since S60 v3.2
            */            
            TDesC8& GetDataBufferAttributesDesc();
            
            /**
            * From CDataBufferQueueItem.
            * Returns the buffer sequence number
            *
            * @since S60 v5.0
            */            
            TUint GetBufferSequenceNumber();
            
            /**
             * 
             */
            void CompleteSelf( TInt aError );

        protected:
            // From CActive
            void RunL();
            void DoCancel();
            TInt RunError( TInt aError );
            
        private:
            CDataBufferQueueItem( MBufferObserver& aObserver, 
                                    MDataBuffer& aBuffer,
                                    TBool aLastBuffer,
                                    TUint aBufSeqNum );
            void ConstructL();
            
        public:
            // next item
            TSglQueLink* iLink;
            
        private:
            /**
            * Observer stored to Callback
            */
            MBufferObserver& iObserver;
            /**
            * Buffer Stored in the Queue item
            */
            MDataBuffer& iBuffer;
            /**
            * Error Stored when the Request Completes
            */
            TInt iError;

            struct DataBufferAttributesStruct
                {
                TBool iLastBuffer; // streaming data last buffer indicator
                TUint iBufferSeqNum; // buffer sequence number
                };
            /**
             * 
             */
            TPckgBuf<DataBufferAttributesStruct> iDataBufferAttributesStructPckg;
        };
    
    } // namespace multimedia

#endif // C_CDATABUFFERQUEUEITEM_H

// End of File
