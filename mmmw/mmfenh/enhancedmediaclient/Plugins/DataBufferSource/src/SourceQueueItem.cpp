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
* Description:  This file contains the implementation of Data Buffer Queue Item.
*
*/


// INCLUDE FILES

#include "SourceQueueItem.h"
#include <mmfcontrollerframework.h>
#include "tracemacros.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSourceQueueItem::CSourceQueueItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSourceQueueItem::CSourceQueueItem()
: iMessage(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CSourceQueueItem::~CSourceQueueItem
// Destructor
// -----------------------------------------------------------------------------
//
CSourceQueueItem::~CSourceQueueItem()
    {
    if(iMessage && !iMessage->IsCompleted())
        {
        iMessage->Complete(KErrCancel);
        delete iMessage;
        }
    }

// -----------------------------------------------------------------------------
// CSourceQueueItem::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSourceQueueItem::ConstructL(
                                  TMMFMessage& aMessage )
    {
    iMessage = new(ELeave) TMMFMessage(aMessage);
    // Read buffer attributes here
    TPckgBuf<DataBufferAttributesStruct> dataBufferAttributesStructPckg;
    TInt err = iMessage->ReadData2FromClient(dataBufferAttributesStructPckg);
    if ( err == KErrNone )
        {
        iLastBuffer = dataBufferAttributesStructPckg().iLastBuffer;
        iBufferSeqNum = dataBufferAttributesStructPckg().iBufferSeqNum;
        }
    EMC_TRACE3(_L("CSourceQueueItem::ConstructL LB[%d] err[%d]"), iLastBuffer, err);
    }

// -----------------------------------------------------------------------------
// CSourceQueueItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSourceQueueItem* CSourceQueueItem::NewL(
                                         TMMFMessage& aMessage )
    {
    CSourceQueueItem* self = new (ELeave) CSourceQueueItem();
    CleanupStack::PushL(self);
    self->ConstructL(aMessage);
    CleanupStack::Pop(self);
    return self;
    }

void CSourceQueueItem::CompleteMessage(TInt status)
    {
    EMC_TRACE3(_L("CSourceQueueItem::CompleteMessage:BufSeqNum[%d]Status[%d]"), \
            iBufferSeqNum, status);
    if ( iMessage && !iMessage->IsCompleted())
        {
        iMessage->Complete(status);
        }
    delete iMessage;
    iMessage = NULL;
    }

TInt CSourceQueueItem::DataSize()
    {
    TInt val(0);
    if (iMessage)
        val = iMessage->SizeOfData1FromClient();
    return val;
    }

TInt CSourceQueueItem::ReadData(TDes8 &aDes)
    {
    TInt err(KErrNotReady);
    if (iMessage)
        {
        err = iMessage->ReadData1FromClient(aDes);
        }
    return err;
    }

TBool CSourceQueueItem::IsLastBuffer()
    {
    return iLastBuffer;
    }

TUint CSourceQueueItem::GetBufferSequenceNumber()
    {
    return iBufferSeqNum;
    }

// End of file
