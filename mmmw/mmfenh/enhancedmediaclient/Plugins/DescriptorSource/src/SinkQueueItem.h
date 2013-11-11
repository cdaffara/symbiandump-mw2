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
* Description:  Definition of the Descriptor sink queue item
*
*/


#ifndef SINKQUEUEITEM_H
#define SINKQUEUEITEM_H

// INCLUDES
#include <e32base.h>

class CMMFBuffer;
class CMMFDataBuffer;
class MMultimediaDataSourceObserver;
class MDataSink;

class CSinkQueueItem : public CBase
    {
    public: // Constructors and destructor
        static CSinkQueueItem* NewL(
            CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aMMDSObserver,
            MDataSink* aConsumer );
        virtual ~CSinkQueueItem();
        
        CMMFDataBuffer* Buffer();
        MMultimediaDataSourceObserver* Observer();
        // This exists for legacy support
        MDataSink* Consumer();
        
    private:
        CSinkQueueItem(
            CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aMMDSObserver,
            MDataSink* aConsumer );
        void ConstructL();
        
    public:
        // next item
        TSglQueLink* iLink;
        
    private:
        // Buffer to be sent to server-side
        CMMFBuffer* iBuffer;
        // Reference to Multimedia data source observer
        MMultimediaDataSourceObserver* iMMDSObserver;
        // Reference to MDataSink
        MDataSink* iConsumer;
    };

#endif      // SINKQUEUEITEM_H

//  End of File
