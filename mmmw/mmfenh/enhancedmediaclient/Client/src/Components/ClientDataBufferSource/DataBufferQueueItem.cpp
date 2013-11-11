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
* Description:  Implementation of the DataBufferQueue Item
*
*/


#include "DataBufferQueueItem.h"
#include "tracemacros.h"

using namespace multimedia;

// ---------------------------------------------------------------------------
// Contructor
// ---------------------------------------------------------------------------
CDataBufferQueueItem::CDataBufferQueueItem( MBufferObserver& aObserver, 
                                            MDataBuffer& aBuffer,
                                            TBool aLastBuffer,
                                            TUint aBufSeqNum )
                                             : CActive(CActive::EPriorityStandard),
                                             iObserver(aObserver),
                                             iBuffer(aBuffer),
                                             iError(KErrNone)
    {
    CActiveScheduler::Add(this);
    iDataBufferAttributesStructPckg().iLastBuffer = aLastBuffer;
    iDataBufferAttributesStructPckg().iBufferSeqNum = aBufSeqNum;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CDataBufferQueueItem::~CDataBufferQueueItem()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// NewL
// ---------------------------------------------------------------------------
CDataBufferQueueItem* CDataBufferQueueItem::NewL( MBufferObserver& aObserver, 
                                                MDataBuffer& aBuffer,
                                                TBool aLastBuffer,
                                                TUint aBufSeqNum )
    {
    CDataBufferQueueItem* self = new (ELeave)CDataBufferQueueItem( aObserver, aBuffer, aLastBuffer, aBufSeqNum );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Second Phase Contructor
// ---------------------------------------------------------------------------
void CDataBufferQueueItem::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// SetActive
// ---------------------------------------------------------------------------
void CDataBufferQueueItem::SetActive()
    {
    if (!IsActive())
        {
        CActive::SetActive();
        }
    }

// ---------------------------------------------------------------------------
// Buffer
// ---------------------------------------------------------------------------
MDataBuffer& CDataBufferQueueItem::DataBuffer()
    {
    return iBuffer;
    }

// ---------------------------------------------------------------------------
// Error
// ---------------------------------------------------------------------------
TInt CDataBufferQueueItem::Error()
    {
    return iError;
    }

// From CActive
void CDataBufferQueueItem::RunL()
    {
    // Save the error code
    iError = iStatus.Int();
    // Signal the observer that this request is serviced
    iObserver.BufferProcessed( this );
    }

void CDataBufferQueueItem::DoCancel()
    {
    iObserver.HandleCancel( *this );
    }

TInt CDataBufferQueueItem::RunError( TInt /*aError*/ )
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Return buffer attributes serialized in a descriptor
// ---------------------------------------------------------------------------
TDesC8& CDataBufferQueueItem::GetDataBufferAttributesDesc()
    {
    return iDataBufferAttributesStructPckg; 
    }    

// ---------------------------------------------------------------------------
// Return buffer sequence number
// ---------------------------------------------------------------------------
TUint CDataBufferQueueItem::GetBufferSequenceNumber()
    {
    return iDataBufferAttributesStructPckg().iBufferSeqNum;
    }

void CDataBufferQueueItem::CompleteSelf( TInt aError )
    {
    EMC_TRACE3(_L("CDataBufferQueueItem::CompleteSelf:BufSeqNum[%d]aErr[%d]"), \
            iDataBufferAttributesStructPckg().iBufferSeqNum, aError);
    if ( IsActive() && (iStatus.Int() == KRequestPending) )
        {
        EMC_TRACE1(_L("CDataBufferQueueItem::CompleteSelf:User::RequestComplete"));
        TRequestStatus* status = &iStatus;
        User::RequestComplete(status, aError);
        }
    }

// End of File
