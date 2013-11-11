// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32base.h>
#include "MSVPANIC.H"
#include "CMsvChangeBuffer.h"


//**********************************
//CMsvChangeRecord
//**********************************

CMsvChangeRecord* CMsvChangeRecord::NewL(const TDesC8& aChange, TUint32 aNotifySequence)
	{
	CMsvChangeRecord* self = new(ELeave)CMsvChangeRecord(aNotifySequence);
	CleanupStack::PushL(self);
	self->ConstructL(aChange);
	CleanupStack::Pop();
	return self;
	}

void CMsvChangeRecord::ConstructL(const TDesC8& aChange)
	{
	iChange = HBufC8::NewL(aChange.Length());
	iChange->Des() = aChange;
	}

CMsvChangeRecord::~CMsvChangeRecord()
	{
	delete iChange;
	}

CMsvChangeRecord::CMsvChangeRecord(TUint32 aNotifySequence)
: iNotifySequence(aNotifySequence)
	{
	}

//**********************************
//CMsvChangeBuffer
//**********************************

CMsvChangeBuffer* CMsvChangeBuffer::NewL()
	{
	CMsvChangeBuffer* self = new(ELeave) CMsvChangeBuffer();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CMsvChangeBuffer::CMsvChangeBuffer()
	{
	__DECLARE_NAME(_S("CMsvChangeBuffer"));
	}

CMsvChangeBuffer::~CMsvChangeBuffer()
	{
	if (iArray)
		{
		iArray->ResetAndDestroy();
		delete iArray;
		}
	}

void CMsvChangeBuffer::ConstructL()
	{
	iArray = new (ELeave) CArrayPtrFlat<CMsvChangeRecord>(KMsvChangeBufferGranularity);
	CMsvChangeRecord* ptr = NULL;
	iArray->ResizeL(KMsvChangeBufferSize, ptr);
	}

void CMsvChangeBuffer::InL(const TDesC8& aChange)
//
// Inserts an entry into the FIFO buffer.
// 
	{
	iNotifySequence++;
	CMsvChangeRecord* record = CMsvChangeRecord::NewL(aChange, iNotifySequence);
	iArray->At(iInPos++) = record;
	
	// Check if we've run past the end of the array.  If so, wrap around.
	if (iInPos==iArray->Count())
		iInPos=0;
	
	// Check if we've run out of space in the array
	if (iInPos==iOutPos)
		{
		// Attempt to grow the array
		TRAPD(ret, GrowArrayL());
		if (ret != KErrNone)
			// Error - possibly out of memory.
			// Delete the last entry and rollback the iInPos variable to ensure
			//  class is in a valid state.
			{
			delete record;
			// If iInPos is not 0, subtract 1 from it,
			//  else set it back to Count-1
			iInPos = (iInPos ? iInPos-1 : iArray->Count()-1);
			User::Leave(ret);
			}
		}
	}

const TDesC8& CMsvChangeBuffer::Next(TUint32& aNotifySequence)
//
// Returns the next entry from the FIFO buffer
//
	{
	__ASSERT_ALWAYS(iOutPos!=iInPos,PanicServer(EMsvChangeBufferEmpty));
	CMsvChangeRecord* record = iArray->At(iOutPos);
	aNotifySequence = record->NotifySequence();
	return record->Change();
	}


void CMsvChangeBuffer::Out()
//
// Removes the next entry from the queue
//
	{
	__ASSERT_ALWAYS(iOutPos!=iInPos,PanicServer(EMsvChangeBufferEmpty));
	delete iArray->At(iOutPos);
	iArray->At(iOutPos++) = NULL;
	if (iOutPos==iArray->Count())
		{
		iOutPos=0;
		ShrinkArray();
		}
	}

void CMsvChangeBuffer::ShrinkArray()
//
// Shrinks the array if possible
//
	{
	while ((iArray->Count()-iInPos)>KMsvChangeBufferGranularity)
		iArray->Delete(iInPos,KMsvChangeBufferGranularity);
	}
	
void CMsvChangeBuffer::GrowArrayL()
//
// Grows the array
// Can leave with OOM
//
	{
	CMsvChangeRecord* ptr=NULL;
	iArray->InsertL(iOutPos, ptr, KMsvChangeBufferGranularity);
	iOutPos+=KMsvChangeBufferGranularity;
	}

