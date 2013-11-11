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
* Description:  This file contains definitions of the queue item class.
*
*/



#ifndef SOURCEQUEUEITEM_H
#define SOURCEQUEUEITEM_H

//  INCLUDES
#include <e32base.h>

class TMMFMessage;

/**
*  Class to encapsulate a queue item.
*
*  @lib S60StreamingSource.lib
*  @since 3.0
*/
class CSourceQueueItem : public CBase
    {
    public:  // Constructors and destructor
        
             /**
             * Two-phased constructor.
        */
        static CSourceQueueItem* NewL( TMMFMessage& aMessage );
        
        /**
        * Destructor.
        */
        virtual ~CSourceQueueItem();
        
        /**
        * Used to set the Message Status when completing the Message.
        * @since Series 60 3.1
        * @param status ErrorCode to be sent back when completing the message
        */
        void CompleteMessage( TInt status );
        
        TInt DataSize();
        TInt ReadData(TDes8 &aDes);
        TBool IsLastBuffer();
        TUint GetBufferSequenceNumber();
        
    private:
        
    /**
    * C++ default constructor.
        */
        CSourceQueueItem();
        
        /**
        * Construct a queue item object and initialize it with
        * @param aDataBuffer Buffer containing data
        */
        void ConstructL( TMMFMessage& aMessage );
        
    public:
        // next item
        TSglQueLink* iLink;
        
    private:
        // Message to complete when this buffer item is processed
        TMMFMessage* iMessage;
        TBool iLastBuffer; // streaming data last buffer indicator
        TUint iBufferSeqNum; // buffer sequence number
        
        // Definition used only in function CSourceQueueItem::ConstructL
        struct DataBufferAttributesStruct
            {
            TBool iLastBuffer; // streaming data last buffer indicator
            TUint iBufferSeqNum; // buffer sequence number
            };
       
    };

#endif      // SOURCEQUEUEITEM_H

// End of File
