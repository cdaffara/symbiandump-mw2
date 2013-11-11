/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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



#ifndef __DATABUFFERQUEUEITEM_H__
#define __DATABUFFERQUEUEITEM_H__

//  INCLUDES
#include <e32base.h>
#include <mmf/server/mmfdatabuffer.h>
#include <mmfcontrollerframework.h>


/**
*  Class to encapsulate a queue item.
*
*  @lib S60StreamingSource.lib
*  @since 3.0
*/
class CDataBufferQueueItem : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CDataBufferQueueItem* NewL( CMMFDataBuffer* aDataBuffer, TMMFMessage& aMessage );

        /**
        * Destructor.
        */
        virtual ~CDataBufferQueueItem();

        /**
        * Used to set the Message Status when completing the Message.
        * @since Series 60 3.1
        * @param status ErrorCode to be sent back when completing the message
        */
		void SetMessageStatus(TInt status);

        /**
        * Used to Get the DataBuffer associated with the Item.
        * @since Series 60 3.1
        * @return Pointer to the CMMFDataBuffer owned by the Queue Item
        */
		CMMFDataBuffer* GetDataBuffer();

    private:

        /**
        * C++ default constructor.
        */
        CDataBufferQueueItem();

       /**
        * Construct a queue item object and initialize it with
        * @param aDataBuffer Buffer containing data
        */
        void ConstructL( CMMFDataBuffer* aDataBuffer, TMMFMessage& aMessage );

    public:
    	// next item
        TSglQueLink* iLink;

    private:
        // Data buffer
        CMMFDataBuffer* iBuffer;
        // Message to complete when this buffer item is processed
        TMMFMessage* iMessage;
        TInt iMessageStatus;
    };


#endif      // __DATABUFFERQUEUEITEM_H__

// End of File
