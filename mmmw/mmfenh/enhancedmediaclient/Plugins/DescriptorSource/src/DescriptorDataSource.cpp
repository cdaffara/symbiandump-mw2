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
* Description:  Implementation of the S60 Descriptor Source.
*
*/


#include "DescriptorDataSource.h"
#include "SinkQueueItem.h"
#include <MultimediaDataSourceEvents.h>
#include <mmfdatasink.h>

#ifdef _DEBUG
#define DEBPRN1(str)        RDebug::Print(str);
#define DEBPRN2(str, val1)   RDebug::Print(str, val1);
#define DEBPRN3(str, val1, val2)   RDebug::Print(str, val1, val2);
#define DEBPRN4(str, val1, val2, val3)   RDebug::Print(str, val1, val2, val3);
#define DEBPRN5(str, val1, val2, val3, val4)   RDebug::Print(str, val1, val2, val3, val4);
#else
#define DEBPRN1(str)
#define DEBPRN2(str, val1)
#define DEBPRN3(str, val1, val2)
#define DEBPRN4(str, val1, val2, val3)
#define DEBPRN5(str, val1, val2, val3, val4)
#endif // _DEBUG



// From MDataSource begins
CDescriptorDataSource* CDescriptorDataSource::NewL(TUid aType )
    {
    DEBPRN1(_L("CDataBufferSource::NewL"));
    CDescriptorDataSource* self = new (ELeave) CDescriptorDataSource(aType);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CDescriptorDataSource::CDescriptorDataSource(TUid aType)
: MDataSource(aType)
    {
    }

CDescriptorDataSource::~CDescriptorDataSource(void)
    {
    }

void CDescriptorDataSource::ConstructL (void)
    {
    }

TUid CDescriptorDataSource::DataSourceType() const
    {
    return KDescriptorDataSourcePlugin;
    }

TFourCC CDescriptorDataSource::SourceDataTypeCode( TMediaId /* aMediaId */ )
    {
    TFourCC fourCC;        
    iMultiMediaSource->GetDataTypeCode(fourCC);
    return fourCC;
    }

TInt CDescriptorDataSource::SetSourceDataTypeCode(TFourCC aSourceFourCC,
                                                  TMediaId /*aMediaId*/ )
    {
    iMultiMediaSource->SetDataTypeCode(aSourceFourCC);
    return KErrNone;
    }

void CDescriptorDataSource::FillBufferL(CMMFBuffer* aBuffer,
                                        MDataSink* aConsumer,
                                        TMediaId /*aMediaId*/ )
    {
    User::LeaveIfError( iMultiMediaSource->ServiceFillBuffer( aBuffer, NULL, aConsumer ) );
    }

void CDescriptorDataSource::BufferEmptiedL(CMMFBuffer* /*aBuffer*/ )
    {
    User::Leave(KErrUnknown);
    }

TBool CDescriptorDataSource::CanCreateSourceBuffer()
    {
    return EFalse;
    }

CMMFBuffer* CDescriptorDataSource::CreateSourceBufferL( TMediaId /* aMediaId */, TBool& /* aReference */ )
    {
    return NULL;
    }

TInt CDescriptorDataSource::SourceThreadLogon( MAsyncEventHandler& /* aEventHandler */ )
    {
    return iMultiMediaSource->Open();
    }

void CDescriptorDataSource::SourceThreadLogoff()
    {
    iMultiMediaSource->Close();
    }

void CDescriptorDataSource::SourcePrimeL()
    {
    DEBPRN1(_L("CDescriptorDataSource::SourcePrimeL"));
    User::LeaveIfError(iMultiMediaSource->Prime());
    }

void CDescriptorDataSource::SourcePlayL()
    {
    DEBPRN1(_L("CDescriptorDataSource::SourcePlayL"));
    User::LeaveIfError(iMultiMediaSource->Play());
    }

void CDescriptorDataSource::SourceStopL()
    {
    DEBPRN1(_L("CDescriptorDataSource::SourceStopL"));
    User::LeaveIfError(iMultiMediaSource->Stop());
    }

void CDescriptorDataSource::SourceCustomCommand( TMMFMessage& aMessage )
    {
    iMultiMediaSource->SourceCustomCommand(aMessage);        
    }

void CDescriptorDataSource::ConstructSourceL(  const TDesC8& /* aInitData */ )
    {
    // No Implementation
    }

void CDescriptorDataSource::SetMultimediaSource(CDescriptorDataMultimediaSource& aMultimediaSource)
    {
    iMultiMediaSource = &aMultimediaSource;   
    }

// From MDataSource ends

CDescriptorDataMultimediaSource::CDescriptorDataMultimediaSource(MDataSource& aDataSource):
iSizeBytes(0),
iSnkItemsCount(0),
iObserver(NULL),
iObserverBitRate(0),
iMessage(NULL),
iParentDataSource(&aDataSource)
    {
     // iState from Base
    iState = ECLOSED;
    }

CDescriptorDataMultimediaSource::~CDescriptorDataMultimediaSource()
    {
    DEBPRN1(_L("CDescriptorDataMultimediaSource::~CDescriptorDataMultimediaSource"));

    if (iMessage)
        {
        iMessage->Complete(KErrCancel);
        delete iMessage;
        }

    // Empty event queue
    EmptyEventQueue();
    iEventsQueue.Close();
    
    // Empty sink queue
    EmptySinkQueue();
    delete iSinkQueue;
    }

EXPORT_C CDescriptorDataMultimediaSource* CDescriptorDataMultimediaSource::NewL(MDataSource& aDataSource)
    {
    DEBPRN1(_L("CDescriptorDataMultimediaSource::NewL"));
    CDescriptorDataMultimediaSource* self = new (ELeave) CDescriptorDataMultimediaSource(aDataSource);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

void CDescriptorDataMultimediaSource::ConstructL()
    {
    static_cast<CDescriptorDataSource*>(iParentDataSource)->SetMultimediaSource(*this);
    iSinkQueue = new(ELeave) TSglQue<CSinkQueueItem>(_FOFF(CSinkQueueItem, iLink));
    }

// From CMultimediaDataSource begins

TInt CDescriptorDataMultimediaSource::SetObserver( MMultimediaDataSourceObserver& aObserver )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case ECLOSED:
            iObserver = &aObserver;
            status = KErrNone;
            break;
        case ESTOPPED:
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::SetObserver[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDescriptorDataMultimediaSource::GetObserver( MMultimediaDataSourceObserver*& aObserver )
    {
    aObserver = iObserver;
    return KErrNone;
    }

void CDescriptorDataMultimediaSource::Event( TUid aEvent )
    {
    if ( aEvent == KMultimediaDataSourceEventBitRateChanged && iObserver )
        {
        TDescriptorDataSourceEvent event;
        event.iType = EBitRateChanged;
        if ( iObserver->GetBitRate( event.iBitRate ) == KErrNone )
            {
            AppendEventToQueue( event );
            }
        }
    }

TInt CDescriptorDataMultimediaSource::SetDataTypeCode( TFourCC aSourceFourCC )
    {
    TInt status(KErrNone);
    iSourceFourCC = aSourceFourCC;
    return status;
    }

TInt CDescriptorDataMultimediaSource::GetDataTypeCode( TFourCC& aSourceFourCC )
    {
    TInt status(KErrNone);
    aSourceFourCC = iSourceFourCC;
    return status;
    }

TInt CDescriptorDataMultimediaSource::GetSize( TUint& aSize )
    {
    TInt status(KErrNone);
    aSize = iSizeBytes;
    return status;
    }

TInt CDescriptorDataMultimediaSource::Open()
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case ECLOSED:
            iState = ESTOPPED;
            status = KErrNone;
            break;
        case ESTOPPED:
            status = KErrNone;
            break;
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::Open[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDescriptorDataMultimediaSource::Close()
    {
    TInt status(KErrNone);
    iState = ECLOSED;

    if (iMessage)
        {
        iMessage->Complete(KErrCancel);
        delete iMessage;
        iMessage = NULL;
        }

    // Empty event queue
    EmptyEventQueue();

    // Clear observer buffers
    EmptySinkQueue();

    return status;
    }

TInt CDescriptorDataMultimediaSource::Prime()
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case ESTOPPED:
            iState = EPRIMED;
            status = KErrNone;
            break;
        case EPRIMED:
            // Clear observer buffers
            EmptySinkQueue();
            break;
        case ECLOSED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::Prime[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDescriptorDataMultimediaSource::Play()
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
            {
            iState = EEXECUTING;
            // Loop through sink buffers and post notification events

        	TSglQueIter<CSinkQueueItem> it(*iSinkQueue);
            it.SetToFirst();
        	CSinkQueueItem* entry;
        	while((entry = it++) != NULL)
        		{
                TDescriptorDataSourceEvent event;
                event.iType = EFillData;
                event.iFillDataSize = entry->Buffer()->RequestSize();
                AppendEventToQueue( event );
        		}
            status = KErrNone;
            break;
            }
        case EEXECUTING:
        case EBUFFERING:
            // No op
            status = KErrNone;
            break;
        case ECLOSED:
        case ESTOPPED:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::Play[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;

    }

TInt CDescriptorDataMultimediaSource::Stop()
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
            iState = ESTOPPED;
            // Clear observer buffers
            TDescriptorDataSourceEvent event;
            event.iType = ESeek;
            event.iSeekPos = 0;
            AppendEventToQueue( event );

            status = EmptyEventQueue();
            
            // Clear app buffers
            status = EmptySinkQueue();
            
            status = KErrNone;
            break;
        case ESTOPPED:
            status = KErrNone;
            break;
        case ECLOSED:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::Stop[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;

    }

TInt CDescriptorDataMultimediaSource::FillBuffer( CMMFBuffer* aBuffer )
    {
    return ServiceFillBuffer( aBuffer, iObserver, NULL );
    }

TInt CDescriptorDataMultimediaSource::GetSeekingSupport( TBool& aSeekSupport )
    {
    aSeekSupport = iSeekingSupported;
    return KErrNone;
    }

TInt CDescriptorDataMultimediaSource::GetRandomSeekingSupport( TBool& aSeekSupport )
    {
    aSeekSupport = iRandomSeekingSupported;
    return KErrNone;
    }

TInt CDescriptorDataMultimediaSource::Seek( TUint /* aPosInBytes */ )
    {
    return KErrNone;
    }

// From CMultimediaDataSource ends

void CDescriptorDataMultimediaSource::SourceCustomCommand( TMMFMessage& aMessage )
    {
    TInt status(KErrNone);
    switch ( aMessage.Function() )
        {
        case ESetConfig:
            status = DoSetConfig( aMessage );
            aMessage.Complete(status);
            break;
        case EGetSourceEvent:
        	// We dont complete this message since this is a Async req
        	// and we only send the source event when its available
            status = DoGetSourceEvent( aMessage );
            break;
        case EFillDataResponse:
            status = DoFillDataResponse( aMessage );
            aMessage.Complete(status);
            break;
        case EGetSourceBitRate:
            {
            TUint bitRate = 0;
            iObserver->GetBitRate(bitRate);
            TPckgBuf<TDescriptorDataSourceEvent> statePckg;	
            aMessage.ReadData1FromClient(statePckg); 
            statePckg().iBitRate = bitRate;
            aMessage.Complete(status);
            break;
            }
        default:
            status = KErrNotSupported;
            break;
        };
    }
    
// From MCustomInterface
TAny* CDescriptorDataMultimediaSource::CustomInterface( TUid /* aInterfaceUid */ )
    {
    return NULL;
    }

// A common function that implements FillBuffer requests
TInt CDescriptorDataMultimediaSource::ServiceFillBuffer( CMMFBuffer* aBuffer,
    MMultimediaDataSourceObserver* aObserver,
    MDataSink* aConsumer )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
            {
            status = AppendBufferToSinkQueue( aBuffer, aObserver, aConsumer );
            }
            break;
        case EEXECUTING:
            {
            status = AppendBufferToSinkQueue( aBuffer, aObserver, aConsumer );
            
            if ( status == KErrNone )
                {
                TDescriptorDataSourceEvent event;
                event.iType = EFillData;
                event.iFillDataSize = aBuffer->RequestSize();
                AppendEventToQueue( event );
                }
            }
            break;
            
        case EBUFFERING:
        case ECLOSED:
        case ESTOPPED:
        default:
            DEBPRN2(_L("CDataBufferSource[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

// A common function that implements FillBuffer requests
TInt CDescriptorDataMultimediaSource::AppendBufferToSinkQueue( CMMFBuffer* aBuffer,
    MMultimediaDataSourceObserver* aObserver,
    MDataSink* aConsumer )
    {
    TInt status(KErrNone);
    // Add observer buffer to queue
    CMMFDataBuffer* dest = static_cast<CMMFDataBuffer*>( aBuffer );
    TDes8& destBufferDes = dest->Data();
    
    CSinkQueueItem* request(NULL);
    
    TRAP( status, request = CSinkQueueItem::NewL( aBuffer, aObserver, aConsumer ) );
    if ( status == KErrNone )
        {
        iSinkQueue->AddLast(*request);
        iSnkItemsCount++;
        
        DEBPRN3(_L("CDescriptorDataMultimediaSource::AppendBufferToQueue[ReqSize[%d]SnkItems[%d]]"), \
            aBuffer->RequestSize(), iSnkItemsCount );
        }
    return status;
    }

void CDescriptorDataMultimediaSource::AppendEventToQueue( TDescriptorDataSourceEvent& aEvent )
    {
    TDescriptorDataSourceEvent* eventPtr = new TDescriptorDataSourceEvent;
    eventPtr->iType = aEvent.iType;
    eventPtr->iFillDataSize = aEvent.iFillDataSize;
    eventPtr->iSeekPos = aEvent.iSeekPos;
    eventPtr->iBitRate = aEvent.iBitRate;
    if ( iEventsQueue.Append(eventPtr) != KErrNone )
        {
        delete eventPtr;
        }
    // See if the event can be processed
    SendEventToClient();
    }

void CDescriptorDataMultimediaSource::SendEventToClient()
    {
    // If there is are already events in the queue, append new one to queue  
    if ( ( iEventsQueue.Count() > 0 ) && (iMessage) )
        {
        // Point to the first element in the array
        TDescriptorDataSourceEvent* eventPtr = iEventsQueue[0];

        TDescriptorDataSourceEventPckgBuf pckg;
        pckg().iType = eventPtr->iType;
        pckg().iFillDataSize = eventPtr->iFillDataSize;
        pckg().iSeekPos = eventPtr->iSeekPos;
        pckg().iBitRate = eventPtr->iBitRate;

        iMessage->WriteDataToClient( pckg );
        iMessage->Complete(KErrNone);
        delete iMessage;
        iMessage = NULL;
        
        // Remove item from queue and delete the object
        iEventsQueue.Remove(0);
        delete eventPtr;
        }
    }

// Reset queues
TInt CDescriptorDataMultimediaSource::EmptyEventQueue()
    {
    iEventsQueue.ResetAndDestroy();
    return KErrNone;
    }

TInt CDescriptorDataMultimediaSource::EmptySinkQueue()
    {
    TInt status(KErrNone);
    // Empty sink queue
    CSinkQueueItem* snkItem;
    while ( !iSinkQueue->IsEmpty() )
        {
        snkItem = iSinkQueue->First();
        iSinkQueue->Remove(*snkItem);
        delete snkItem;
        }
    iSnkItemsCount = 0;
    return status;
    }

TInt CDescriptorDataMultimediaSource::DoSetConfig( TMMFMessage& aMessage )
    {
    TInt status(KErrNotReady);
    // Can be called only when closed or stopped state
    switch ( iState )
        {
        case ECLOSED:
        case ESTOPPED:
            {
            TCCAttributesPckgBuf pckg;
            status = aMessage.ReadData1FromClient(pckg);
            if ( status == KErrNone )
                {
                iSeekingSupported = pckg().iSeeking;
                iRandomSeekingSupported = pckg().iRandomSeeking;
                iSizeBytes = pckg().iSourceSizeInBytes;
                }
            }
            break;
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::DoSetConfig[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDescriptorDataMultimediaSource::DoGetSourceEvent( TMMFMessage& aMessage )
    {
    // Can be called in any state
    TInt status(KErrNone);
    // Cancel any pending request
    if (iMessage)
        {
        iMessage->Complete(KErrCancel);
        delete iMessage;
        iMessage = NULL;
        }

    // Create a new copy of message
    iMessage = new TMMFMessage(aMessage);
   
    // Check to see if we can send any events to client
    SendEventToClient();
    return status;
    }

TInt CDescriptorDataMultimediaSource::DoFillDataResponse( TMMFMessage& aMessage )
    {
    TInt status(KErrNone);
    // Can be called only when executing state
    switch ( iState )
        {
        case EEXECUTING:
            {
            TCCAttributesPckgBuf pckg;
            status = aMessage.ReadData1FromClient(pckg);
            if ( status != KErrNone )
                {
                return status;
                }
            iSnkItemsCount--;
            
            CSinkQueueItem* item = iSinkQueue->First();
            iSinkQueue->Remove(*item);
            
            // Copy data
            status = aMessage.ReadData2FromClient(item->Buffer()->Data());
            if ( status != KErrNone )
                {
                DEBPRN2(_L("CDataBufferSource::DoFillDataResponse[ErrorReadingData[%d]"), status);
                return status;
                }
            
            item->Buffer()->SetLastBuffer( pckg().iLastBuffer );
            
            MMultimediaDataSourceObserver* mmdsObsrvr(item->Observer());
            MDataSink* mdatasink(item->Consumer());
                        
            // Signal the controller, based on which FillBuffer API was called,
            // make the appriopriate callback.
            if ( mmdsObsrvr )
                {
                mmdsObsrvr->BufferFilled(item->Buffer());
                }
            else
                {
                // This is a callback. Ignore leave from it.
                TRAP_IGNORE(mdatasink->BufferFilledL(item->Buffer()));
                }
            
            delete item;
            }
            break;
        case ECLOSED:
        case ESTOPPED:
        case EPRIMED:
        case EBUFFERING:
        default:
            DEBPRN2(_L("CDescriptorDataMultimediaSource::DoFillDataResponse[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDescriptorDataMultimediaSource::EvaluateIntent( ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNone;
    }

TInt CDescriptorDataMultimediaSource::ExecuteIntent( ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNone;
    }

TInt CDescriptorDataMultimediaSource::SetAgentProperty( ContentAccess::TAgentProperty /*aProperty*/, TInt /*aValue*/)
    {
    return KErrNone;
    }

//  End of File
