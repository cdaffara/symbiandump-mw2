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
* Description:  Implementation of Cache Source Reader active object.
*
*/



// INCLUDE FILES
#include <MultimediaDataSource.h>
#include "SinkQueueItem.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSinkQueueItem::CSinkQueueItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSinkQueueItem::CSinkQueueItem(
                               CMMFBuffer* aBuffer )
                               :iBuffer(aBuffer)
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
                                     CMMFBuffer* aBuffer )
    {
    CSinkQueueItem* self = new(ELeave) CSinkQueueItem( aBuffer );
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

// End of file
