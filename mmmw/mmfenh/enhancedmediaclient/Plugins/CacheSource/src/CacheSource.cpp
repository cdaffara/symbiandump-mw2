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
* Description:  S60 Cache Data Source Plugin implementation
*
*/

#include "CacheSource.h"
#include "SinkQueueItem.h"
#include "CacheSourceUid.h"
#include <MultimediaDataSourceEvents.h>
#include "ReadWriteRequestAO.h"
#include <mmfdatabuffer.h>

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


const TUint KTransferBufferSize = 8192;

EXPORT_C CCacheSource* CCacheSource::NewL(CMultimediaDataSource* aDataSource, CDataSourceConfigIntfc* aDataSourceConfig )
    {
    DEBPRN1(_L("CCacheSource::NewL"));
    CCacheSource* self = new (ELeave) CCacheSource(aDataSource,aDataSourceConfig);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CCacheSource::CCacheSource(CMultimediaDataSource* aDataSource,CDataSourceConfigIntfc* aDataSourceConfig )
: iDataSource(aDataSource),iDataSourceConfig(aDataSourceConfig)
    {
    // iState from Base
    iState = ECLOSED;
    iFileSize = 0;
    iLastBufferWritten = false;
    iAbsBufferStart = 0;
    iAbsBufferEnd = 0;
    iBufferReadPos = 0;
    iSeekStart = 0;
    iSeekEnd = 0;
    iSnkBytes = 0;
    iDataBuffer = NULL;
    iCacheLocation = NULL;
    }

CCacheSource::~CCacheSource()
    {
    DEBPRN1(_L("CCacheSource::~CCacheSource"));
    iState = ECLOSED;
    EmptySinkQueue();
    // Empty sink queue
    delete iSinkQueue;
    delete iDataSourceObserver;
    delete iTransferBuffer;
    delete iReadRequest;
    delete iWriteRequest;
    delete iDataSource;
    
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
        if(iCacheLocation)
            {
            delete iCacheLocation;    
            }
        iFile.Close();
        iFs.Delete(iFileName);
        iFs.Close();
        }
    else
        {
        delete iDataBuffer;
        }        
    }

void CCacheSource::ConstructL (void)
    {
    DEBPRN1(_L("CCacheSource::ConstructL ENTER"));
    iSinkQueue = new(ELeave) TSglQue<CSinkQueueItem>(_FOFF(CSinkQueueItem, iLink));
    iDataSourceObserver = CMDataSourceObserver::NewL(iDataSource,this);
    iTransferBuffer = CMMFDataBuffer::NewL(KTransferBufferSize);
    iTransferBuffer->SetRequestSizeL(KTransferBufferSize);
    iWriteRequest = CReadWriteRequestAO::NewL(*this,CReadWriteRequestAO::EWriteRequest);
    iWriteRequest->SetBuffer(iTransferBuffer);
    iReadRequest = CReadWriteRequestAO::NewL(*this,CReadWriteRequestAO::EReadRequest);
    iReadRequest->SetBuffer(iTransferBuffer);
    iDataSourceConfig->GetCacheType(iCacheType);
    DEBPRN1(_L("CCacheSource::ConstructL EXIT"));        
    }



// From CMultimediaDataSource begins
TInt CCacheSource::SetObserver( MMultimediaDataSourceObserver& aObserver )
    {
    TInt status(KErrNone);    
    iObserver = &aObserver;
    return status;
    }

TInt CCacheSource::GetObserver( MMultimediaDataSourceObserver*& aObserver )
    {
    aObserver = iObserver;
    return KErrNone;
    }

void CCacheSource::Event( TUid aEvent )
    {
    // CAll the Parent DataSource
    iDataSource->Event(aEvent);
    }

TInt CCacheSource::SetDataTypeCode(TFourCC aSourceFourCC )
    {
    TInt status(KErrNone);
    status = iDataSource->SetDataTypeCode(aSourceFourCC);
    return status;
    }

TInt CCacheSource::GetDataTypeCode(TFourCC& aSourceFourCC )
    {
    TInt status(KErrNone);
    status = iDataSource->GetDataTypeCode(aSourceFourCC);
    return status;
    }

TInt CCacheSource::GetSize( TUint& aSize )
    {
    TInt status(KErrNone);
    status = iDataSource->GetSize(aSize);
    return status;
    }

TInt CCacheSource::Open()
    {
    TInt status(KErrNone);
    DEBPRN1(_L("CCacheSource::Open"));                 
    status = iDataSource->Open();
    return status;
    }

TInt CCacheSource::Close()
    {
    TInt status(KErrNone);
    DEBPRN1(_L("CCacheSource::Close"));                 
    status = iDataSource->Close();
    return status;
    }

TInt CCacheSource::Prime()
    {
    TInt status(KErrNone);
    DEBPRN1(_L("CCacheSource::Prime"));             
    status = iDataSource->Prime();

    iDataSourceConfig->GetCacheType(iCacheType);
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
	    status = iFs.Connect();
	    if(status == KErrNone)
	        {
	        	if(iCacheLocation)
	            {
	            delete iCacheLocation;
	            iCacheLocation = NULL;                
	            }
	        iCacheLocation = HBufC16::NewL(KMaxFileName);
	        TPtr des = iCacheLocation->Des();
	        iDataSourceConfig->GetCacheLocation(des);
	
	        TInt err = iFile.Temp(iFs,des,iFileName,EFileWrite);
			status = err;
			}
        }
    else
        {
        TUint cacheSize = 0;
        iDataSourceConfig->GetCacheSize(cacheSize);            
        
        if(iDataBuffer)
            {
            delete iDataBuffer;
            iDataBuffer = NULL;            
            }
        TRAP(status,iDataBuffer = CMMFDataBuffer::NewL( cacheSize ));
        if(status == KErrNone)
        	{
        	iDataBuffer->Data().SetLength(cacheSize);
        	}
        }        

    //status = iDataSource->FillBuffer(iTransferBuffer);
    return status;
    }

TInt CCacheSource::Play()
    {
     TInt status(KErrNone);
    DEBPRN1(_L("CCacheSource::Play"));         
    status = iDataSource->Play();
    return status;
    }

TInt CCacheSource::Stop()
    {
    TInt status(KErrNone);
    DEBPRN1(_L("CCacheSource::Stop"));    
    status = iDataSource->Stop();
    iFileSize = 0;
    iLastBufferWritten = false;
    iAbsBufferStart = 0;
    iAbsBufferEnd = 0;
    iBufferReadPos = 0;
    iSeekStart = 0;
    iSeekEnd = 0;
    iSnkBytes = 0;
    iTransferBuffer->SetLastBuffer(EFalse);
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
        iFile.Close();
        iFs.Delete(iFileName);
        }
    return status;
    }

TInt CCacheSource::FillBuffer( CMMFBuffer* aBuffer )
    {
    TInt status(KErrNone);
    ASSERT(aBuffer);
    CMMFDataBuffer* dest = static_cast<CMMFDataBuffer*>( aBuffer );
    TDes8& destBufferDes = dest->Data();
    
    DEBPRN4(_L("CCacheSource::FillBuffer RequestSize[%d] AbsBufEnd[%d] SnkBytes[%d]"),aBuffer->RequestSize(),iAbsBufferEnd,iSnkBytes); 
    
    AppendBufferToSinkQueue(dest);
    
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
        if(((iSnkBytes + aBuffer->RequestSize())  < iFileSize) || iLastBufferWritten)
            {
            status = ServiceFillBuffer();
            }
        else
            {
            if(!iLastBufferWritten)
                {
				iTransferBuffer->Data().FillZ();
				iTransferBuffer->Data().SetLength(0);
				iDataSource->FillBuffer(iTransferBuffer);                
				}
                
            status = KErrUnderflow;                                
            }        
        }
    else
        {
        if((aBuffer->RequestSize() < (iAbsBufferEnd - iAbsBufferReadPos)) || iLastBufferWritten)
            {
            status = ServiceFillBuffer();                
            }
        else
            {
            if(!iLastBufferWritten)                
                {
				iTransferBuffer->Data().FillZ();
				iTransferBuffer->Data().SetLength(0);
				iDataSource->FillBuffer(iTransferBuffer);                
				}
            
            status = KErrUnderflow;                
            }                                
        }        
    return status;
    }

TInt CCacheSource::ServiceFillBuffer()
    {
    TInt status(KErrNone);
    if ( iSinkQueue->IsEmpty() )
        return status;
    
    CSinkQueueItem* snkItem = iSinkQueue->First();
    iSinkQueue->Remove(*snkItem);
    iSnkItemsCount--;
    CMMFBuffer* buffer = snkItem->Buffer();
    delete snkItem;
    
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
        iReadRequest->SetBuffer(buffer);
        iReadRequest->SetActive();
        if((iSnkBytes + iReadRequest->Buffer()->RequestSize()) > iFileSize)
            {
            iReadRequest->Buffer()->SetLastBuffer(ETrue);    
            }

        iFile.Read(iSnkBytes,iReadRequest->Buffer()->Data(),iReadRequest->iStatus);
        iSnkBytes += iReadRequest->Buffer()->RequestSize();
        }
    else
        {
        iReadRequest->SetBuffer(buffer);
        
        DEBPRN4(_L("CCacheSource::ServiceFillBuffer DataSize[%d] iBufferReadPos[%d] SnkBytes[%d]"),iDataBuffer->Data().Size(),iBufferReadPos,iSnkBytes); 
        if((iBufferReadPos + iReadRequest->Buffer()->RequestSize()) > iAbsBufferEnd)
            {
            iReadRequest->Buffer()->SetLastBuffer(ETrue);    
            }
        
        iDataBuffer->SetPosition(iBufferReadPos);
        
        if((iBufferReadPos + iReadRequest->Buffer()->RequestSize()) <= iDataBuffer->Data().Size())
            {
            iReadRequest->Buffer()->Data().Copy(iDataBuffer->Data().MidTPtr(iBufferReadPos,iReadRequest->Buffer()->RequestSize()));
            iBufferReadPos += iReadRequest->Buffer()->RequestSize();
            }
        else
            {
            TInt dataToCopy = iDataBuffer->Data().Size() - iBufferReadPos;
            iReadRequest->Buffer()->Data().Append(iDataBuffer->Data().MidTPtr(iBufferReadPos,dataToCopy));
            iBufferReadPos = 0;
            TInt dataLeftToCopy = iReadRequest->Buffer()->RequestSize() - dataToCopy;
            iReadRequest->Buffer()->Data().Append(iDataBuffer->Data().MidTPtr(iBufferReadPos,dataLeftToCopy));            
            iBufferReadPos +=  dataLeftToCopy;                   
            DEBPRN4(_L("CCacheSource::ServiceFillBuffer dataToCopy[%d] dataLeftToCopy[%d] SnkBytes[%d]"),dataToCopy,dataLeftToCopy,iBufferReadPos);         
            }            

        iSnkBytes += iReadRequest->Buffer()->Data().Length();     
        DEBPRN4(_L("CCacheSource::ServiceFillBuffer RequestSize[%d] iBufferReadPos[%d] SnkBytes[%d]"),iReadRequest->Buffer()->RequestSize(),iBufferReadPos,iSnkBytes);                 
        iReadRequest->SetActive();
        TRequestStatus* status = &(iReadRequest->iStatus);
        User::RequestComplete(status,KErrNone);
        }        
    return status;        
    }

TAny* CCacheSource::CustomInterface( TUid /*aInterfaceUid*/ )
    {
    return NULL;    
    }

TInt CCacheSource::GetSeekingSupport( TBool& aSeekSupport )
    {
    aSeekSupport = ETrue;
    return KErrNone;
    };

TInt CCacheSource::GetRandomSeekingSupport( TBool& aSeekSupport )
    {
    aSeekSupport = EFalse;
    return KErrNone;
    };

// Seek Implementation

TInt CCacheSource::Seek(TUint aPosInBytes)
    {
    TInt status(KErrNone);
    DEBPRN2(_L("CCacheSource::Seek[%d]"),aPosInBytes);    
    
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
		// This is File Seek so the SeekPosition has to be less
		// then the File Size otherwise we leave        	
        if(aPosInBytes <= iFileSize)
            {
            iSnkBytes = aPosInBytes;
            }
        else
            {
            status = KErrNotReady;        
            }
        }
    else
        {
        DEBPRN3(_L("CCacheSource::Seek AbsBufStart[%d] AbsBufEnd[%d]"),iAbsBufferStart,iAbsBufferEnd);            
        // Here we seek in the Buffer, so we can only seek from the begining of the
        // buffer window. which is iAbsBufferStart
        if(aPosInBytes >= iAbsBufferStart)
            {
            iAbsBufferReadPos = aPosInBytes;
            TInt seekDelta = aPosInBytes - iAbsBufferStart;
            if(seekDelta <= (iDataBuffer->Data().MaxLength() - iSeekStart))
                {
                iBufferReadPos = iSeekStart + seekDelta;    
                }
            else
                {
                iBufferReadPos = seekDelta - (iDataBuffer->Data().MaxLength() - iSeekStart);    
                }                    
            }
        else
            {
            status = KErrNotReady;    
            }                
        }        
    return status;        
    }

// From CMultimediaDataSource ends

TInt CCacheSource::ServiceBufferFilled( CMMFBuffer* aBuffer )
    {
    TInt status(KErrNone);
    CMMFDataBuffer* dest = static_cast<CMMFDataBuffer*>( aBuffer );
    TDes8& destBufferDes = dest->Data();
    
    if(iCacheType == CDataSourceConfigIntfc::EFILE)
        {
        iWriteRequest->SetActive();
        iFile.Write(iFileSize,destBufferDes,iWriteRequest->iStatus);
        iFileSize = iFileSize + dest->BufferSize();    
        DEBPRN3(_L("CCacheSource::ServiceBufferFilled FileSize[%d] LastBuffer[%d]"),iFileSize,dest->LastBuffer());
        if(dest->LastBuffer())
            {
            iLastBufferWritten = ETrue;
            dest->SetLastBuffer(EFalse);
            }
        }
    else
        {
        iWriteRequest->SetActive();

        iDataBuffer->SetPosition(iSeekEnd);
        
        if((iSeekEnd + dest->BufferSize()) <= iDataBuffer->Data().MaxLength())
            {
            iDataBuffer->Data().Replace(iSeekEnd,dest->Data().Size(),dest->Data());
            iSeekEnd += dest->Data().Size();
            }
        else
            {
            TInt copyEndData = iDataBuffer->Data().MaxLength() - iSeekEnd;    
            iDataBuffer->Data().Replace(iSeekEnd,copyEndData,dest->Data().MidTPtr(0,copyEndData));
            TInt dataLeftToCopy = dest->Data().Size() - copyEndData;
            iSeekEnd = 0;
            iDataBuffer->Data().Replace(iSeekEnd,dataLeftToCopy,dest->Data().MidTPtr(copyEndData,dataLeftToCopy));
            iSeekEnd = dataLeftToCopy;
            }            
        
        iAbsBufferEnd += dest->BufferSize();    
        if(iAbsBufferEnd < iDataBuffer->Data().MaxLength())
            {
            iAbsBufferStart = 0;    
            }
        else
            {
            iAbsBufferStart =  iAbsBufferEnd - iDataBuffer->Data().MaxLength();   
            }            
        
        if(iAbsBufferEnd < iDataBuffer->Data().MaxLength())
            {
            iSeekStart = 0;    
            }
        else
            {
            iSeekStart = iSeekEnd;    
            }            
        
        DEBPRN4(_L("CCacheSource::ServiceBufferFilled DataSize[%d] AbsBufferEnd[%d] LastBuffer[%d]"),iDataBuffer->Data().Size(),iAbsBufferEnd,dest->LastBuffer());
        if(dest->LastBuffer())
            iLastBufferWritten = ETrue;            
            
        TRequestStatus* status = &(iWriteRequest->iStatus);
        User::RequestComplete(status,KErrNone);
        }        
    return status;
    }

void CCacheSource::ReadRequestComplete(CReadWriteRequestAO* aRequest,TRequestStatus& aStatus)
    {
    if(aStatus != KErrNone)
        {
        }
    else
        {
        if(aRequest->RequestType() == CReadWriteRequestAO::EReadRequest)
            {
            DEBPRN3(_L("CCacheSource::ReadRequestComplete [%d] [%d]"),aRequest->Buffer()->Data().Size(),aRequest->Buffer()->RequestSize());
            iObserver->BufferFilled(aRequest->Buffer());
            }
        else
            {
			CSinkQueueItem* snkItem = iSinkQueue->First();
			iSinkQueue->Remove(*snkItem);
			iSnkItemsCount--;
			CMMFBuffer* buffer = snkItem->Buffer();
			delete snkItem;
			FillBuffer(buffer);
            }
        }        
    }


TInt CCacheSource::AppendBufferToSinkQueue( CMMFBuffer* aBuffer )
    {
    TInt status(KErrNone);
    // Add observer buffer to queue
    CMMFDataBuffer* dest = static_cast<CMMFDataBuffer*>( aBuffer );
    TDes8& destBufferDes = dest->Data();
    
    CSinkQueueItem* request(NULL);
    
    TRAP( status, request = CSinkQueueItem::NewL( aBuffer ) );
    if ( status == KErrNone )
        {
        iSinkQueue->AddLast(*request);
        iSnkItemsCount++;
        }
    return status;
    }

TInt CCacheSource::EmptySinkQueue()
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

//From MMultimediaDataSourceObserver


CMDataSourceObserver::CMDataSourceObserver(CMultimediaDataSource* aDataSource ,CMultimediaDataSource* aParent)
                               :iDataSource(aDataSource)
                               ,iParent(aParent)
    {
    }

// -----------------------------------------------------------------------------
// CSinkQueueItem::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMDataSourceObserver::ConstructL()
    {
    iDataSource->SetObserver(*this);    
    }

// -----------------------------------------------------------------------------
// CSinkQueueItem::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMDataSourceObserver* CMDataSourceObserver::NewL(CMultimediaDataSource* aDataSource ,CMultimediaDataSource* aParent)
    {
    CMDataSourceObserver* self = new(ELeave) CMDataSourceObserver( aDataSource, aParent );
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
CMDataSourceObserver::~CMDataSourceObserver()
    {
    }

void CMDataSourceObserver::BufferFilled( CMMFBuffer* aBuffer )
    {
    (static_cast<CCacheSource*>(iParent))->ServiceBufferFilled(aBuffer);        
    }

void CMDataSourceObserver::Event( TUid aEvent )
    {
    MMultimediaDataSourceObserver* obsvr(NULL);    
    iParent->GetObserver(obsvr);
    obsvr->Event(aEvent);    
    }

TInt CMDataSourceObserver::GetBitRate( TUint& aBitRate )
    {
    TInt status(KErrNone);
    MMultimediaDataSourceObserver* obsvr(NULL);    
    iParent->GetObserver(obsvr);
    status = obsvr->GetBitRate(aBitRate);    
    return status;
    }

// End of file
