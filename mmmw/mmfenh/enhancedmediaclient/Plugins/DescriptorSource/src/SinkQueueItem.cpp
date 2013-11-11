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
* Description:  Implementation of Descriptor Source Reader active object.
*
*/



// INCLUDE FILES
#ifdef _DEBUG
#include    <e32svr.h>
#endif

#include "SinkQueueItem.h"
#include <mmfdatasource.h>
#include <mmfdatasink.h>
#include <MultimediaDataSource.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSinkQueueItem::CSinkQueueItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSinkQueueItem::CSinkQueueItem(
                               CMMFBuffer* aBuffer,
                               MMultimediaDataSourceObserver* aMMDSObserver,
                               MDataSink* aConsumer )
                               :iBuffer(aBuffer),
                               iMMDSObserver(aMMDSObserver),
                               iConsumer(aConsumer)
    {
    }

// -----------------------------------------------------------------------------
// CSinkQueueItem::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSinkQueueItem::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CSinkQueueItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSinkQueueItem* CSinkQueueItem::NewL(
                                     CMMFBuffer* aBuffer,
                                     MMultimediaDataSourceObserver* aMMDSObserver,
                                     MDataSink* aConsumer )
    {
    CSinkQueueItem* self = new(ELeave) CSinkQueueItem( aBuffer, aMMDSObserver, aConsumer );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CSinkQueueItem::~CSinkQueueItem
// Destructor
// -----------------------------------------------------------------------------
//
CSinkQueueItem::~CSinkQueueItem()
    {
    }

CMMFDataBuffer* CSinkQueueItem::Buffer()
    {    
    return static_cast<CMMFDataBuffer*>(iBuffer);
    }

MMultimediaDataSourceObserver* CSinkQueueItem::Observer()
    {
    return iMMDSObserver;
    }

MDataSink* CSinkQueueItem::Consumer()
    {
    return iConsumer;
    }

// End of file
