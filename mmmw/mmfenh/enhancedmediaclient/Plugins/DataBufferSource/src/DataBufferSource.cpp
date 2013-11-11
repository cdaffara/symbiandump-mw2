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
* Description:  S60 DataBuffer Data Source Plugin implementation
*  Version     : %version: bh1mmcf#14.1.4 %
*
*/

#include "DataBufferDataSource.h"
#include <mmf/server/mmfdatabuffer.h>
#include "DataBufferSourceCustomCommands.h"
#include "SourceQueueItem.h"
#include "SinkQueueItem.h"
#include "DataCopyEngineAO.h"
#include "DataBufferSourceUid.h"
#include <MultimediaDataSourceEvents.h>
#include "DRMConfigIntfcImpl.h"
#include <mmfdatasink.h>
#include <mmfdatapath.h>
#include "tracemacros.h"

const TUint KMaxHeapForBuffering = 0x40000; // 256 KB (1 MB = default controller heap size).

CDataBufferSource* CDataBufferSource::NewL(TUid aType )
    {
    EMC_TRACE1(_L("CDataBufferSource::NewL"));
    CDataBufferSource* self = new (ELeave) CDataBufferSource(aType);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CDataBufferSource::CDataBufferSource(TUid aType)
: MDataSource(aType)
    {
    }

CDataBufferSource::~CDataBufferSource(void)
    {
    }

void CDataBufferSource::ConstructL (void)
    {
    }

// From MDataSource begins
TUid CDataBufferSource::DataSourceType() const
    {
    return KMmfDataBufferSource;
    }


TFourCC CDataBufferSource::SourceDataTypeCode(TMediaId /*aMediaId*/ )
    {
    TFourCC fourCC;        
    iMultiMediaSource->GetDataTypeCode(fourCC);
    return fourCC;
    }

TInt CDataBufferSource::SetSourceDataTypeCode(TFourCC aSourceFourCC,
                                                  TMediaId /*aMediaId*/ )
    {
    iMultiMediaSource->SetDataTypeCode(aSourceFourCC);
    return KErrNone;
    }

void CDataBufferSource::FillBufferL(CMMFBuffer* aBuffer,
                                        MDataSink* aConsumer,
                                        TMediaId /*aMediaId*/ )
    {
    User::LeaveIfError( iMultiMediaSource->ServiceFillBuffer( aBuffer, NULL, aConsumer ) );
    }

void CDataBufferSource::BufferEmptiedL(CMMFBuffer* /*aBuffer*/ )
    {
    User::Leave(KErrUnknown);
    }

TBool CDataBufferSource::CanCreateSourceBuffer()
    {
    return EFalse;
    }

CMMFBuffer* CDataBufferSource::CreateSourceBufferL(TMediaId /*aMediaId*/,
                                                       TBool &/*aReference*/ )
    {
    /*    CMMFBuffer* newBuffer = 0; //dummy
    return newBuffer;
    */
    return NULL;
    }

TInt CDataBufferSource::SourceThreadLogon( MAsyncEventHandler& /*aEventHandler*/ )
    {
    return iMultiMediaSource->Open();
    }

void CDataBufferSource::SourceThreadLogoff()
    {
    iMultiMediaSource->Close();
    }

void CDataBufferSource::SourcePrimeL()
    {
    EMC_TRACE1(_L("CDataBufferSource::SourcePrimeL"));
    User::LeaveIfError(iMultiMediaSource->Prime());
    }

void CDataBufferSource::SourcePlayL()
    {
    EMC_TRACE1(_L("CDataBufferSource::SourcePlayL"));
    User::LeaveIfError(iMultiMediaSource->Play());
    }

void CDataBufferSource::SourceStopL()
    {
    EMC_TRACE1(_L("CDataBufferSource::SourceStopL"));
    User::LeaveIfError(iMultiMediaSource->Stop());
    }


void CDataBufferSource::ConstructSourceL(const TDesC8& /*aInitData*/ )
    {
    }

void CDataBufferSource::SourceCustomCommand(TMMFMessage& aMessage)
    {
    iMultiMediaSource->SourceCustomCommand(aMessage);        
    }

void CDataBufferSource::SetMultimediaSource(CDataBufferMultimediaSource& aMultimediaSource)
    {
    iMultiMediaSource = &aMultimediaSource;   
    }
// From MDataSource begins

// From CMultimediaDataSource begins

EXPORT_C CDataBufferMultimediaSource* CDataBufferMultimediaSource::NewL(MDataSource& aDataSource)
    {
    EMC_TRACE1(_L("CDataBufferMultimediaSource::NewL"));
    CDataBufferMultimediaSource* self = new (ELeave) CDataBufferMultimediaSource(aDataSource);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


CDataBufferMultimediaSource::CDataBufferMultimediaSource(MDataSource& aDataSource)
: iSrcItemsCount(0),
iSnkItemsCount(0),
iSrcBytes(0),
iSnkBytes(0),
iSizeBytes(0),
iTransferRate(0),
iBufferedDataSize(0),
iLastBufferReceived(EFalse),
iParentDataSource(&aDataSource)
    {
    // iState from Base
    iState = ECLOSED;
    iBufferingConfig.iType = TBufferingConfig::BUFFERINGNONE;
    iBufferingConfig.iAmount = 0;
    iMessage = NULL;
    }

CDataBufferMultimediaSource::~CDataBufferMultimediaSource(void)
    {
    EMC_TRACE1(_L("CDataBufferMultimediaSource::~CDataBufferMultimediaSource"));
    
    // Cancel data copying
    if(iDataCopyEngineAO)
    	{
    	iDataCopyEngineAO->Cancel();
    	delete iDataCopyEngineAO;
    	}
    
    iState = ECLOSED;
    // Empty source queue
    EmptySourceQueue();
    delete iSourceQueue;
    EmptySinkQueue();
    // Empty sink queue
    delete iSinkQueue;
    iAllowedOutputDeviceList.Close();
    }

void CDataBufferMultimediaSource::ConstructL (void)
    {
    static_cast<CDataBufferSource*>(iParentDataSource)->SetMultimediaSource(*this);
    iSourceQueue = new(ELeave) TSglQue<CSourceQueueItem>(_FOFF(CSourceQueueItem, iLink));
    iSinkQueue = new(ELeave) TSglQue<CSinkQueueItem>(_FOFF(CSinkQueueItem, iLink));
    iDataCopyEngineAO = CDataCopyEngineAO::NewL(iSourceQueue, iSinkQueue, *this);
    }


TInt CDataBufferMultimediaSource::SetObserver( MMultimediaDataSourceObserver& aObserver )
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
            EMC_TRACE2(_L("CDataBufferMultimediaSource::SetObserver[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::GetObserver( MMultimediaDataSourceObserver*& aObserver )
    {
    aObserver = iObserver;
    return KErrNone;
    }

void CDataBufferMultimediaSource::Event( TUid aEvent )
    {
    if ( aEvent == KMultimediaDataSourceEventBitRateChanged )
        {
        if(iMessage)
            {
            if(!iMessage->IsCompleted())
                {
                iMessage->Complete(KErrNone);
                delete iMessage;
                iMessage = NULL;
                }
            }
        }
    }

TInt CDataBufferMultimediaSource::SetDataTypeCode(TFourCC aSourceFourCC )
    {
    TInt status(KErrNone);
    iSourceFourCC = aSourceFourCC;
    return status;
    }

TInt CDataBufferMultimediaSource::GetDataTypeCode(TFourCC& aSourceFourCC )
    {
    TInt status(KErrNone);
    aSourceFourCC = iSourceFourCC;
    return status;
    }

TInt CDataBufferMultimediaSource::GetSize( TUint& aSize )
    {
    TInt status(KErrNone);
    aSize = iSizeBytes;
    return status;
    }

TInt CDataBufferMultimediaSource::Open()
    {
    TInt status(KErrNotReady);
    EMC_TRACE2(_L("CDataBufferSource::Open iState[%d]"),iState);
    switch ( iState )
        {
        case ECLOSED:
            iState = ESTOPPED;
            status = KErrNone;
            break;
        case ESTOPPED:
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            status = KErrNone;
            EMC_TRACE2(_L("CDataBufferMultimediaSource::Open[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::Close()
    {
    TInt status(KErrNone);
    EMC_TRACE2(_L("CDataBufferSource::Close iState[%d]"),iState);
    iState = ECLOSED;
    // Clear app buffers
    EmptySinkQueue();
    iSnkBytes = 0;
    // Clear observer buffers
    EmptySourceQueue();
    iSrcBytes = 0;
    return status;
    }

TInt CDataBufferMultimediaSource::Prime()
    {
    TInt status(KErrNotReady);
    EMC_TRACE2(_L("CDataBufferSource::Prime iState[%d]"),iState);
    switch ( iState )
        {
        case ESTOPPED:
            iState = EPRIMED;
            status = KErrNone;
            break;
        case EPRIMED:
            // Clear observer buffers
            EmptySinkQueue();
            iSnkBytes = 0;
            status = KErrNone;
            break;
        case ECLOSED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::Prime[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::Play()
    {
    TInt status(KErrNotReady);
    EMC_TRACE2(_L("CDataBufferSource::Play iState[%d]"),iState); 
    switch ( iState )
        {
        case EPRIMED:
            iState = EEXECUTING;
            iDataCopyEngineAO->Start();
            status = KErrNone;
            break;
        case EEXECUTING:
        case EBUFFERING:
            // No op
            status = KErrNone;
            break;
        case ECLOSED:
        case ESTOPPED:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::Play[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::Stop()
    {
    TInt status(KErrNotReady);
    EMC_TRACE2(_L("CDataBufferSource::Stop iState[%d]"),iState); 
    switch ( iState )
        {
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
            iState = ESTOPPED;
            
            // Stop data copy engine
            iDataCopyEngineAO->Stop();

            // Clear observer buffers
            status = EmptySourceQueue();
            iSrcBytes = 0;
            iLastBufferReceived = EFalse;
            
            // Clear app buffers
            status = EmptySinkQueue();
            iSnkBytes = 0;
            
            status = KErrNone;
            break;
        case ESTOPPED:

            // Stop data copy engine
            iDataCopyEngineAO->Stop();

            // Clear user buffers
            status = EmptySourceQueue();
            iSrcBytes = 0;
            iLastBufferReceived = EFalse;
            break;
        case ECLOSED:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::Stop[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::FillBuffer( CMMFBuffer* aBuffer )
    {
    return ServiceFillBuffer( aBuffer, iObserver, NULL );
    }

TInt CDataBufferMultimediaSource::GetInterface(
        TUid aInterfaceId,
        TVersion& aVer,
        TAny*& aInterfaceImpl )
    {
    TInt status(KErrNotFound);
    aInterfaceImpl = NULL;
    if ( ( aInterfaceId == KDRMConfigIntfc ) &&
         ( ( aVer.iMajor == KDRMConfigIntfcMajorVer1 ) &&
           ( aVer.iMinor == KDRMConfigIntfcMinorVer1 ) &&
           ( aVer.iBuild == KDRMConfigIntfcBuildVer1 ) ) )
            {
            CDRMConfigIntcfImpl* temp(NULL);
            TRAP(status, temp = CDRMConfigIntcfImpl::NewL(*this));
            if ( status == KErrNone )
                {
                this->SetChild(*((CChildIntfc*)temp));
                temp->SetParent(*(CParentIntfc*)this);
                aInterfaceImpl = (CDRMConfigIntfc*)temp;
                }
            }
    return status;
    }

TAny* CDataBufferMultimediaSource::CustomInterface( TUid /*aInterfaceUid*/ )
    {
    return NULL;
    }


// From CMultimediaDataSource ends

// From MDataCopyEngineObserver begins
void CDataBufferMultimediaSource::SourceQueueItemProcessed()
    {
    iSrcItemsCount--;
    EMC_TRACE2(_L("CDataBufferMultimediaSource::SourceQueueItemProcessed[SrcItemsCount(%d)]"), iSrcItemsCount);
    
    CSourceQueueItem* srcItem = iSourceQueue->First();
    iSourceQueue->Remove(*srcItem);
    srcItem->CompleteMessage(KErrNone);
    delete srcItem;
    }

void CDataBufferMultimediaSource::SinkQueueItemProcessed()
    {
    iSnkItemsCount--;
    
    CSinkQueueItem* item = iSinkQueue->First();
    iSinkQueue->Remove(*item);
    iSnkBytes += item->Buffer()->Data().Length();
    EMC_TRACE3(_L("CDataBufferMultimediaSource::SinkQueueItemProcessed[Src[%d]B. Snk[%d]B]"), iSrcBytes, iSnkBytes);
    
    // If this is the last buffer being sent to the controller
    // empty the rest of the items in the sink queue
    if ( item->Buffer()->LastBuffer() )
        {
        EMC_TRACE1(_L("CDataBufferMultimediaSource::SinkQueueItemProcessed[LASTBUFFER]"));
        CSinkQueueItem* tempItem;
        while ( !iSinkQueue->IsEmpty() )
            {
            tempItem = iSinkQueue->First();
            iSinkQueue->Remove(*tempItem);
            delete tempItem;
            iSnkItemsCount--;
            }
        iSnkBytes = 0;
        
        iState = ESTOPPED;
        iSrcBytes = 0;
        }
    // Signal the controller, based on which FillBuffer API was called,
    // make the appriopriate callback.
    if ( item->Observer() )
        {
        item->Observer()->BufferFilled(item->Buffer());
        }
    else
        {
        TRAP_IGNORE(item->Consumer()->BufferFilledL(item->Buffer()));
        }
    delete item;
    }
// From MDataCopyEngineObserver ends

// To be called by DRMConfig Interface begins
TInt CDataBufferMultimediaSource::GetDRMType( TDRMType& aDRMType )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
            aDRMType = iDRMType;
            status = KErrNone;
            break;
        case ECLOSED:
        case ESTOPPED:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::GetDRMType[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::GetAllowedOutputDeviceCount( TInt& aCount )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
            aCount = iAllowedOutputDeviceList.Count();
            status = KErrNone;
            break;
        case ECLOSED:
        case ESTOPPED:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::GetDRMType[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::GetAllowedOutputDevice(
                TInt aIndex,
                TDRMAllowedOutputDevice& aOutputDevice )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
            status = KErrArgument;
            if ( (0 <= aIndex ) && ( aIndex < iAllowedOutputDeviceList.Count() ) )
                {
                aOutputDevice = iAllowedOutputDeviceList[aIndex];
                status = KErrNone;
                }
            break;
        case ECLOSED:
        case ESTOPPED:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::GetDRMType[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }
// To be called by DRMConfig Interface ends

void CDataBufferMultimediaSource::SourceCustomCommand(TMMFMessage& aMessage)
    {
    TInt err(KErrNone);
    EMC_TRACE1(_L("CDataBufferMultimediaSource::SourceCustomCommand "));
    switch ( aMessage.Function() )
        {
        case EProcessBuffer:
            {
            CSourceQueueItem* item = NULL;
            TRAPD(err, item = CSourceQueueItem::NewL(aMessage));
            if (err == KErrNone)
                {
                iSourceQueue->AddLast(*item);
                iSrcItemsCount++;
                iSrcBytes += item->DataSize();
                EMC_TRACE6(_L("EProcessBuffer>State[%d] Src[%d]B Snk[%d]B] LB[%d] SeqNum[%d]"), \
                    iState, iSrcBytes, iSnkBytes, item->IsLastBuffer(), item->GetBufferSequenceNumber());
                
                iLastBufferReceived = item->IsLastBuffer();
                
                // If we are in EBUFFERING state, check if source can transition
                // to EEXECUTING state
                if ( ( iState == EBUFFERING ) && 
                    ( ( ( iSrcBytes - iSnkBytes ) >= iBufferedDataSize ) || ( iLastBufferReceived ) ) )
                    {
                    iState = EEXECUTING;
                    // Signal to start copy AO
                    iDataCopyEngineAO->Start();
                    }
                else
                    {
                    iDataCopyEngineAO->SourceQueueChanged();
                    }

                }
            }
            break;
            
        case ECancel:
            {
            EMC_TRACE1(_L("EProcessBuffer>Cancel"));
            err = DoHandleCancel( aMessage );
            }
            break;
            
        case ESetSize:
            {
            // Read size info
            TPckgBuf<TInt> sizePckg;
            err = aMessage.ReadData1FromClient(sizePckg);
            if ( err == KErrNone )
                {
                iSizeBytes = sizePckg();
                EMC_TRACE2(_L("ESetSize>SizeSet[%d] Bytes"), iSizeBytes);
                // The message is handled completely.
                aMessage.Complete(KErrNone);
                
                // Signal the observer that there is a new size changed event.
                if ( iObserver )
                    {
                    iObserver->Event(KMultimediaDataSourceObserverEventSourceSizeChanged);
                    }
                }
            // If we are in EBUFFERING state, re-calculate iBufferedDataSize,
            // check if source can transition to EEXECUTING state
            }
            break;
        
        case EGetCurrentBufferingConfig :
            {
            TPckgBuf<TBufferingConfig> sizePckg;	 
            EMC_TRACE3(_L("EGetCurrentBufferingConfig>BufType[%d] BufAmt[%d]Bytes"), \
                    iBufferingConfig.iType, iBufferingConfig.iAmount);
            sizePckg() = iBufferingConfig ;	
            aMessage.WriteDataToClient(sizePckg);
            aMessage.Complete(KErrNone);
            }
            break;
        
        case ESetBufferingConfig:
            {
            // Read size info
            TPckgBuf<TBufferingConfig> sizePckg;
            err = aMessage.ReadData1FromClient(sizePckg);
            if ( err == KErrNone )
                {
                iBufferingConfig = sizePckg();
                EMC_TRACE3(_L("ESetBufferingConfig>BufType[%d] BufAmt[%d]Bytes"), \
                        iBufferingConfig.iType, iBufferingConfig.iAmount);
                // The message is handled completely.
                aMessage.Complete(KErrNone);
                }
            }
            break;
        
        case EGetBitRate:
            {
            TPckgBuf<TUint> bitRatePckg;
            EMC_TRACE2(_L("EGetBitRate>[%d]"), iObserverBitRate);
            bitRatePckg() = iObserverBitRate ;	
            aMessage.WriteDataToClient(bitRatePckg);
            aMessage.Complete(KErrNone);
            }
            break;         

        case EGetBufferingConfigSupported:
            {
			if(iObserverBitRate)
				{
				aMessage.Complete(KErrNone);
				}
			else
				{
				iMessage = new(ELeave) TMMFMessage(aMessage);
				}
            }
            break;    

        case ESetDRMConfig:
            // Need to complete the message in this function
            // and return KErrNone
            err = HandleSetDRMConfig(aMessage);
            break;
        default:
            err = KErrArgument;
            break;
        };
    
    // If any error
    if (err != KErrNone)
        {
        EMC_TRACE2(_L("CDataBufferMultimediaSource::SourceCustomCommand:Completing message with error[%d]"), err);
        aMessage.Complete(err);
        }
    }


TInt CDataBufferMultimediaSource::ServiceFillBuffer( CMMFBuffer* aBuffer,
                                              MMultimediaDataSourceObserver* aObserver,
                                              MDataSink* aConsumer )
    {
    TInt status(KErrNotReady);
    switch ( iState )
        {
        case EPRIMED:
        case EBUFFERING:
            {
            status = AppendBufferToSinkQueue( aBuffer, aObserver, aConsumer );
            }
            break;
        case EEXECUTING:
            {
            status = AppendBufferToSinkQueue( aBuffer, aObserver, aConsumer );
            
            if ( status == KErrNone )
                {
                // If not enough data, determine buffered data size, pause
                // data copy engine AO and move to EBUFFERING state.
                if ( !iLastBufferReceived && ( ( iSrcBytes - iSnkBytes ) < ( aBuffer->RequestSize() ) ) )
                    {
                    CalculateBufferdDataSize();
                    
                    if(iBufferedDataSize > 0)
                        {
                        iDataCopyEngineAO->Pause();
                        iState = EBUFFERING;
                        }
                    else
                        {
                        iDataCopyEngineAO->SinkQueueChanged();
                        }          
                    }
                else
                    {
                    iDataCopyEngineAO->SinkQueueChanged();
                    }
                }
            }
            break;
            
        case ECLOSED:
        case ESTOPPED:
            status = KErrNone;
            break;
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::ServiceFillBuffer[Illegal cmd on state[%d]]"), iState );
            break;
        };
    return status;
    }

TInt CDataBufferMultimediaSource::AppendBufferToSinkQueue( CMMFBuffer* aBuffer,
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
        
        EMC_TRACE5(_L("CDataBufferMultimediaSource::AppendBufferToQueue[ReqSize[%d]SrcItems[%d]SnkItems[%d]]Avail[%d]B"), \
            aBuffer->RequestSize(), iSrcItemsCount, iSnkItemsCount, (iSrcBytes - iSnkBytes) );
        }
    return status;
    }

TInt CDataBufferMultimediaSource::EmptySourceQueue()
    {
    TInt status(KErrNone);
    // Complete and empty source queue
    EMC_TRACE2(_L("CDataBufferSource::EmptySourceQueue() iState[%d]"),iState); 
    CSourceQueueItem* srcItem;
    while ( !iSourceQueue->IsEmpty() )
        {
        srcItem = iSourceQueue->First();
        iSourceQueue->Remove(*srcItem);
        srcItem->CompleteMessage(KErrCancel);
        delete srcItem;
        }
    iSrcItemsCount = 0;
    return status;
    }

TInt CDataBufferMultimediaSource::EmptySinkQueue()
    {
    TInt status(KErrNone);
    // Empty sink queue
    EMC_TRACE2(_L("CDataBufferSource::EmptySinkQueue() iState[%d]"),iState); 
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

void CDataBufferMultimediaSource::CalculateBufferdDataSize()
    {
    iBufferedDataSize = 0;
    EMC_TRACE2(_L("CDataBufferSource::CalculateBufferdDataSize() iState[%d]"),iState);     
    // Get the bit rate from observer
    iObserver->GetBitRate( iObserverBitRate );
    
    // If we don't know observer bit rate
    if ((iObserverBitRate == 0 ) && (!iSinkQueue->First()) )
        {// Just buffer until the observer buffer can be filled
        iBufferedDataSize = iSinkQueue->First()->Buffer()->RequestSize();
        }
    else
        {
        if(iBufferingConfig.iType == TBufferingConfig::BUFFERINGNONE)
            {
                iBufferedDataSize = 0;
            }
        else if ( iBufferingConfig.iType == TBufferingConfig::FIXEDSIZE)
            {
                iBufferedDataSize = iBufferingConfig.iAmount;
            }
        else
            {
                iBufferedDataSize = (iBufferingConfig.iAmount * iObserverBitRate) / 8;
            }            
        }
    
    // In case of streaming make sure this doesn't exceed the heap memory
    iBufferedDataSize = iBufferedDataSize > KMaxHeapForBuffering ? KMaxHeapForBuffering : iBufferedDataSize;
    EMC_TRACE2(_L("CDataBufferMultimediaSource::CalculateBufferdDataSize[%d]"), iBufferedDataSize );
    }

TInt CDataBufferMultimediaSource::HandleSetDRMConfig(  TMMFMessage& aMessage )
    {
    TInt status(KErrNone);
    TRAP(status, DoSetDRMConfigL( aMessage ));
    EMC_TRACE2(_L("CDataBufferMultimediaSource::HandleSetDRMConfig[%d]"), status );
    aMessage.Complete(status);
    return status;
    }

void CDataBufferMultimediaSource::DoSetDRMConfigL( TMMFMessage& aMessage )
    {
    switch ( iState )
        {
        case ECLOSED:
        case ESTOPPED:
            {
            // Create buffer to hold data
            HBufC8* buf = HBufC8::NewLC( aMessage.SizeOfData1FromClient() );
            TPtr8 ptr = buf->Des();

            // Read data from client.
            aMessage.ReadData1FromClientL( ptr );
        
           	RDesReadStream stream(ptr);
            CleanupClosePushL(stream);
            
            // Get DRM Type
            iDRMType = (TDRMType)stream.ReadInt32L();
            // Get Allowed Output Device Count
            TInt count = stream.ReadInt32L();
            // Get Allowed Output Device items
            iAllowedOutputDeviceList.Reset(); // If any
           	for (TInt i=0; i<count; i++)
              	{
           		iAllowedOutputDeviceList.Append((TDRMAllowedOutputDevice)stream.ReadInt32L());
           		}
           	CleanupStack::PopAndDestroy(&stream);
           	CleanupStack::PopAndDestroy(buf);
            }
            break;

        case EPRIMED:
        case EEXECUTING:
        case EBUFFERING:
        default:
            EMC_TRACE2(_L("CDataBufferMultimediaSource::DoSetDRMConfigL[Illegal cmd on state[%d]]"), iState );
            User::Leave(KErrNotReady);
            break;
        };
    }

TInt CDataBufferMultimediaSource::DoHandleCancel( TMMFMessage& aMessage )
    {
    TInt status(KErrNone);
    TPckgBuf<TUint> bufSeqPckg;
    status = aMessage.ReadData1FromClient(bufSeqPckg);            
    if ( status == KErrNone )
        {
        CSourceQueueItem* item (NULL);
        TSglQueIter<CSourceQueueItem> itemIter(*iSourceQueue);
        while ( (item = itemIter++) != NULL )
            {
            if ( item->GetBufferSequenceNumber() == bufSeqPckg() )
                { // We found what we are looking for
                break; // from while ( item != NULL )
                }
            }
        // If we have the item we are looking for
        if ( item != NULL )
            {
            aMessage.Complete( KErrNone );
            iSourceQueue->Remove(*item);
            item->CompleteMessage(KErrCancel);
            delete item;
            }
        else
            {
            aMessage.Complete( KErrNotFound );
            }
        }
    return status;
    }

TInt CDataBufferMultimediaSource::EvaluateIntent( ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNone;
    }

TInt CDataBufferMultimediaSource::ExecuteIntent( ContentAccess::TIntent /*aIntent*/ )
    {
    return KErrNone;
    }

// End of file
