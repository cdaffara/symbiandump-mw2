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
* Description:  This file contains the implementation of Data Buffer Queue Item.
*
*/


// INCLUDE FILES

#include "DataBufferQueueItem.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDataBufferQueueItem::CDataBufferQueueItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CDataBufferQueueItem::CDataBufferQueueItem()
	:	iBuffer(NULL),
		iMessage(NULL)
	{
		iMessageStatus = KErrNone;
	}

// -----------------------------------------------------------------------------
// CDataBufferQueueItem::~CDataBufferQueueItem
// Destructor
// -----------------------------------------------------------------------------
//
CDataBufferQueueItem::~CDataBufferQueueItem()
	{
	delete iBuffer;
	if(!iMessage->IsCompleted())
		{
		iMessage->Complete(iMessageStatus);
		delete iMessage;
		}
	}

// -----------------------------------------------------------------------------
// CDataBufferQueueItem::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDataBufferQueueItem::ConstructL(
	CMMFDataBuffer* aDataBuffer,
	TMMFMessage& aMessage )
	{
	iBuffer = aDataBuffer;
	iMessage = new(ELeave) TMMFMessage(aMessage);
	}

// -----------------------------------------------------------------------------
// CDataBufferQueueItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CDataBufferQueueItem* CDataBufferQueueItem::NewL(
	CMMFDataBuffer* aDataBuffer,
	TMMFMessage& aMessage )
	{
	CDataBufferQueueItem* self = new (ELeave) CDataBufferQueueItem();
	CleanupStack::PushL(self);
	self->ConstructL(aDataBuffer, aMessage);
	CleanupStack::Pop(self);
	return self;
	}

void CDataBufferQueueItem::SetMessageStatus(TInt status)
{
	iMessageStatus = status;
}

CMMFDataBuffer* CDataBufferQueueItem::GetDataBuffer()
{
	return iBuffer;
}
