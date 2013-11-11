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
* Description:  S60 Audio Streaming Data Source Plugin implementation
*
*/

#include <mmf/server/mmfdatabuffer.h>
#include "S60StreamingSourceCustomCommands.h"
#include "S60StreamingSource.h"
#include "DataBufferQueueItem.h"
#include "ReadRequest.h"



EXPORT_C CS60StreamingSource* CS60StreamingSource::NewL(
	TUid aType )
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60StreamingSource::NewL"));
#endif

	CS60StreamingSource* self = CS60StreamingSource::NewLC(aType);
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CS60StreamingSource* CS60StreamingSource::NewLC(
	TUid aType )
	{
	CS60StreamingSource* self = new (ELeave) CS60StreamingSource(aType);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}


CS60StreamingSource::CS60StreamingSource(
	TUid aType)
	: MDataSource(aType),
	  iHandlingRequest(EFalse),
	  iCurrentRequest(NULL)
	{
	}

CS60StreamingSource::~CS60StreamingSource(void)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60StreamingSource::~CS60StreamingSource"));
#endif

   if ( iDataBufferQueue )
        {
        CDataBufferQueueItem* item;
        while ( !iDataBufferQueue->IsEmpty() )
            {
            item = iDataBufferQueue->First();
            iDataBufferQueue->Remove(*item);
            item->SetMessageStatus(KErrNone);
            delete item;
            }
        delete iDataBufferQueue;
        }

   if ( iReadRequestQueue )
        {
        CReadRequest* item;
        while ( !iReadRequestQueue->IsEmpty() )
            {
            item = iReadRequestQueue->First();
            iReadRequestQueue->Remove(*item);
            delete item;
            }
        delete iReadRequestQueue;
        }

    delete iCurrentRequest;
	}

void CS60StreamingSource::ConstructL (void)
	{
	iDataBufferQueue = new(ELeave) TSglQue<CDataBufferQueueItem>(_FOFF(CDataBufferQueueItem, iLink));
	iReadRequestQueue = new(ELeave) TSglQue<CReadRequest>(_FOFF(CReadRequest, iLink));
	}

void CS60StreamingSource::ConstructSourceL(
	const TDesC8& /*aInitData*/ )
	{
	}

TUid CS60StreamingSource::DataSourceType() const
	{
	const TUid KMmfStreamingSource = {KMmfS60StreamingSourceUid};
	return KMmfStreamingSource;
	}

TFourCC CS60StreamingSource::SourceDataTypeCode(
	TMediaId /*aMediaId*/ )
	{
	return KMMFFourCCCodePCM16; // dummy
	}

TInt CS60StreamingSource::SetSourceDataTypeCode(
	TFourCC /*aSourceFourCC*/,
	TMediaId /*aMediaId*/ )
	{
	return KErrNotSupported;
	}

void CS60StreamingSource::FillBufferL(
	CMMFBuffer* aBuffer,
	MDataSink* aConsumer,
	TMediaId /*aMediaId*/ )
	{

	CMMFDataBuffer* dest = STATIC_CAST( CMMFDataBuffer*, aBuffer );
	TDes8& destBufferDes = dest->Data();
	destBufferDes.Zero();

	CReadRequest* request = CReadRequest::NewL(*this, aBuffer, aConsumer);
	iReadRequestQueue->AddLast(*request);

	aBuffer->SetPosition(0);

	if( !iHandlingRequest && !iReadRequestQueue->IsEmpty())
		{
		iHandlingRequest = ETrue;
		iCurrentRequest = iReadRequestQueue->First();
		iReadRequestQueue->Remove(*iCurrentRequest);
		iCurrentRequest->HandleRequest();
		}

	}


void CS60StreamingSource::HandleFillBuffer(
	CMMFBuffer* aBuffer,
	MDataSink* aConsumer )
	{

	if (!iDataBufferQueue->IsEmpty())
		{
		iSrcBuffer = iDataBufferQueue->First();

		CMMFDataBuffer* src = iSrcBuffer->GetDataBuffer();
		CMMFDataBuffer* dest = STATIC_CAST( CMMFDataBuffer*, aBuffer );

		TDes8& srcBufferDes = src->Data();
		TDes8& destBufferDes = dest->Data();

		TUint srcLength = srcBufferDes.MaxLength() - src->Position();
		TUint destLength = destBufferDes.MaxLength() - dest->Position();

		RDebug::Print(_L("Src Length [%d] Dest Length [%d] [%x]"),srcLength,destLength,aBuffer);
		RDebug::Print(_L("Src Position [%d] Dest Position [%d]"),src->Position(),dest->Position());

//      if the source data length is bigger than the destination buffer size. Enough for data to be copied
		if ( srcLength > destLength)
			{
			destBufferDes.Append(srcBufferDes.Ptr() + src->Position(), destLength);
			RDebug::Print(_L("dest->Data().Length() = %d"),dest->Data().Length());
			src->SetPosition(src->Position() + destLength);
			aConsumer->BufferFilledL(aBuffer);
			}
		else
			{
			// At this point source data length is equal or less than destination buffer size.
			// Copy all data to destination. If src is less then save the dest position.
			RDebug::Print(_L("Before dest->Data().Length() = %d"),dest->Data().Length());

			destBufferDes.Append(srcBufferDes.Ptr()+ src->Position(), srcLength);
			RDebug::Print(_L("Before Dest Position [%d]"),dest->Position());
			RDebug::Print(_L("After dest->Data().Length() = %d"),dest->Data().Length());

			dest->SetPosition(dest->Position() + srcLength);

			if ( src->LastBuffer() )
				{
				dest->SetLastBuffer(ETrue);
				// Dequeue and Delete the source buffer
				iDataBufferQueue->Remove(*iSrcBuffer);
				iSrcBuffer->SetMessageStatus(KErrNone);
				delete iSrcBuffer;
				iSrcBuffer = NULL;
				aConsumer->BufferFilledL(aBuffer);

				// Since we already have the last buffer we will
				// delete all the outstanding read requests
				while(!iReadRequestQueue->IsEmpty())
					{
						CReadRequest* outStandingReq = iReadRequestQueue->First();
						iReadRequestQueue->Remove(*outStandingReq);
						delete outStandingReq;
					}

				}
			else
				{
				iDataBufferQueue->Remove(*iSrcBuffer);
				iSrcBuffer->SetMessageStatus(KErrNone);
				delete iSrcBuffer;
				iSrcBuffer = NULL;

				if(dest->Position() == dest->Data().MaxLength())
					aConsumer->BufferFilledL(aBuffer);
				else
					{
						CReadRequest* request = CReadRequest::NewL(*this, aBuffer, aConsumer);
						iReadRequestQueue->AddFirst(*request);
					}
				}
			}
		}
	else
		{
		RDebug::Print(_L("HandleFillBuffer but iDataBufferQueue->IsEmpty [%d] [%x]"),iDataBufferQueue->IsEmpty(),aBuffer);
		CReadRequest* request = CReadRequest::NewL(*this, aBuffer, aConsumer);
		iReadRequestQueue->AddFirst(*request);
		}

	delete iCurrentRequest;
	iCurrentRequest = NULL;
	iHandlingRequest = EFalse;

	if( !iHandlingRequest && !iReadRequestQueue->IsEmpty() && !iDataBufferQueue->IsEmpty())
		{
		iHandlingRequest = ETrue;
		iCurrentRequest = iReadRequestQueue->First();
		iReadRequestQueue->Remove(*iCurrentRequest);
		iCurrentRequest->HandleRequest();
		}
	}

void CS60StreamingSource::BufferEmptiedL(
	CMMFBuffer* /*aBuffer*/ )
	{
	}//called by MDataSink to pass back emptied buffer to the source

TBool CS60StreamingSource::CanCreateSourceBuffer()
	{
	return EFalse;
	}

CMMFBuffer* CS60StreamingSource::CreateSourceBufferL(
	TMediaId /*aMediaId*/,
	TBool &/*aReference*/ )
	{
	CMMFBuffer* newBuffer = 0; //dummy
	return newBuffer;
	}

CMMFBuffer* CS60StreamingSource::CreateSourceBufferL(
	TMediaId aMediaId,
	CMMFBuffer& /*aSinkBuffer*/,
	TBool &aReference)
	{
	return CreateSourceBufferL(aMediaId, aReference);
	}
/*
void CS60StreamingSource::SourcePrimeL()
	{
	}
*/
void CS60StreamingSource::SourceStopL()
	{
    if ( iDataBufferQueue )
        {
        CDataBufferQueueItem* item;
        while ( !iDataBufferQueue->IsEmpty() )
            {
            item = iDataBufferQueue->First();
            iDataBufferQueue->Remove(*item);
            item->SetMessageStatus(KErrCancel);
            delete item;
            }
        }
   if ( iReadRequestQueue )
        {
        CReadRequest* item;
        while ( !iReadRequestQueue->IsEmpty() )
            {
            item = iReadRequestQueue->First();
            iReadRequestQueue->Remove(*item);
            delete item;
            }
        }

	}


void CS60StreamingSource::SourceCustomCommand(TMMFMessage& aMessage)
	{
#if _DEBUG
	RDebug::Print(_L("CS60StreamingSource::SourceCustomCommand\n"));
#endif



	switch(aMessage.Function())
		{
		case ECancel:
			  	break;
		case EProcessBuffer:
			{
			TPckgBuf<TInt> lastBufferFlagPkg;
			User::LeaveIfError(aMessage.ReadData2FromClient(lastBufferFlagPkg));
			TInt lastBuffer = lastBufferFlagPkg();
			CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(aMessage.SizeOfData1FromClient());
			User::LeaveIfError(aMessage.ReadData1FromClient(buffer->Data()));

			if ( lastBuffer )
				buffer->SetLastBuffer(ETrue);

			TInt length = buffer->Data().Length();
			TInt maxLength = buffer->Data().MaxLength();
			TDes8& data = buffer->Data();

			CDataBufferQueueItem* item = NULL;
			TRAPD(err, item = CDataBufferQueueItem::NewL(buffer, aMessage));
			if (err != KErrNone)
				aMessage.Complete(err);
			else
				iDataBufferQueue->AddLast(*item);

			if( !iHandlingRequest && !iReadRequestQueue->IsEmpty())
				{
				iHandlingRequest = ETrue;
				iCurrentRequest = iReadRequestQueue->First();
				iReadRequestQueue->Remove(*iCurrentRequest);
				iCurrentRequest->HandleRequest();
				}

			  break;
			}
		 default:
			  break;
		  }
	}

EXPORT_C TBool CS60StreamingSource::IsSeekingSupported()
	{
		return EFalse;
	}


EXPORT_C TBool CS60StreamingSource::IsRandomSeekingSupported()
	{
		return EFalse;
	}

// __________________________________________________________________________
// Exported proxy for instantiation method resolution
// Define the interface UIDs


const TImplementationProxy ImplementationTable[] =
	{
		IMPLEMENTATION_PROXY_ENTRY(KMmfS60StreamingSourceUid, CS60StreamingSource::NewL)
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);

	return ImplementationTable;
	}



