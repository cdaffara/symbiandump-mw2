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
* Description:  Definition of the databuffer source reader active object class
*
*/


// INCLUDES
#include "DataCopyEngineAO.h"
#include "DataBufferDataSource.h"
#include "SourceQueueItem.h"
#include "SinkQueueItem.h"
#include <e32cmn.h>

#ifdef _DEBUG
#define DEBPRN1(str) \
    RDebug::Print(str);
#define DEBPRN2(str, val1) \
    RDebug::Print(str, val1);
#define DEBPRN3(str, val1, val2) \
    RDebug::Print(str, val1, val2);
#define DEBPRN4(str, val1, val2, val3) \
    RDebug::Print(str, val1, val2, val3);
#define DEBPRN5(str, val1, val2, val3, val4) \
    RDebug::Print(str, val1, val2, val3, val4);
#else
#define DEBPRN1(str)
#define DEBPRN2(str, val1)
#define DEBPRN3(str, val1, val2)
#define DEBPRN4(str, val1, val2, val3)
#define DEBPRN5(str, val1, val2, val3, val4)
#endif // _DEBUG


CDataCopyEngineAO::CDataCopyEngineAO( TSglQue<CSourceQueueItem>* aSourceQueue,
                                     TSglQue<CSinkQueueItem>* aSinkQueue,
                                     MDataCopyEngineObserver& aObserver )
                                     : CActive( CActive::EPriorityStandard ),
                                     iSourceQueue( aSourceQueue ),
                                     iSinkQueue( aSinkQueue ),
                                     iObserver( &aObserver )
    {
    CActiveScheduler::Add( this );
    }

CDataCopyEngineAO::~CDataCopyEngineAO()
    {
    Cancel();
    delete iSrcDataDes;
    }

CDataCopyEngineAO* CDataCopyEngineAO::NewL( TSglQue<CSourceQueueItem>* aSourceQueue,
                                           TSglQue<CSinkQueueItem>* aSinkQueue,
                                           MDataCopyEngineObserver& aObserver )
    {
    CDataCopyEngineAO* self = new (ELeave) CDataCopyEngineAO( aSourceQueue, aSinkQueue, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CDataCopyEngineAO::ConstructL()
    {
    // No impl yet
    }

void CDataCopyEngineAO::SourceQueueChanged()
    {
    if (iState == EWaitingForSourceQueueSignal)
        {
        iState = EExecuting;
        KickSignal();
        }
    }

void CDataCopyEngineAO::SinkQueueChanged()
    {
    DEBPRN2(_L("CDataCopyEngineAO::SinkQueueChanged:State[%d]"), iState);
    if ( iState == EWaitingForSinkQueueSignal )
        {
        iState = EExecuting;
        KickSignal();
        }
    }

void CDataCopyEngineAO::KickSignal()
    {
    if ( ( iState != EStopped ) && ( iState != EPaused ) && ( !IsActive() ) )
        {
        TRequestStatus* s = &iStatus;
        SetActive();
        User::RequestComplete( s, KErrNone );
        }
    }

void CDataCopyEngineAO::Start()
    {
    if ( iState == EStopped )
        {
        iState = EExecuting;
        iSinkQueueItemProcessed = EFalse;
        iSourceQueueItemProcessed = EFalse;
        KickSignal();
        }
    else if ( iState == EPaused )
        {
        iState = EExecuting;
        KickSignal();
        }
    }

void CDataCopyEngineAO::Stop()
    {
    Cancel();
    iState = EStopped;
    delete iSrcDataDes;
    iSrcDataDes = NULL;
    iSrcDataPos = 0;
    }

void CDataCopyEngineAO::Pause()
    {
    Cancel();
    iState = EPaused;
    }

void CDataCopyEngineAO::RunL()
    {
    DEBPRN2(_L("CDataCopyEngineAO::RunL() State[%d]"),iState);
    
    if ( iState != EExecuting )
        return;
    
    // If we are waiting for signalling sink queue item processed event,
    // notify the observer
    if ( iSourceQueueItemProcessed )
        {
        iSourceQueueItemProcessed = EFalse;
        delete iSrcDataDes;
        iSrcDataDes = NULL;
        iSrcDataPos = 0;
        iObserver->SourceQueueItemProcessed();
        KickSignal();
        return;
        }
    
    // If we are waiting for signalling source queue item processed event,
    // notify the observer
    if ( iSinkQueueItemProcessed )
        {
        iSinkQueueItemProcessed = EFalse;
        iObserver->SinkQueueItemProcessed();
        KickSignal();
        return;
        }
    
    // No request in the source queue
    if ( iSourceQueue->IsEmpty() )
        {
        iState = EWaitingForSourceQueueSignal;
        return;
        }
    
    // No request in the sink queue
    if ( iSinkQueue->IsEmpty() )
        {
        iState = EWaitingForSinkQueueSignal;
        return;
        }
    
    CSourceQueueItem* srcItem = iSourceQueue->First();
    CSinkQueueItem* snkItem = iSinkQueue->First();
    // If there is no data copied into local buffer
    if ( !iSrcDataDes )
        {
        // If data from source item can fit into dest buffer
        // copy directly
              
        if ( srcItem->DataSize() <= ( snkItem->Buffer()->Data().MaxLength() - snkItem->Buffer()->Data().Length() )  )
            {
            // Create a temp ptr pointing to buffer in the sink queue
            TPtr8 ptr( ( const_cast<TUint8*>( ( snkItem->Buffer()->Data().Ptr() ) ) + snkItem->Buffer()->Data().Length() ),
                0,
                ( snkItem->Buffer()->Data().MaxLength() - snkItem->Buffer()->Data().Length() ) );
            // Read data into the buffer
            srcItem->ReadData( ptr );
            
            // Update descriptor attribute and position attribute
            snkItem->Buffer()->Data().SetLength( snkItem->Buffer()->Data().Length() + srcItem->DataSize() );
            
            // Now that data is copied from source queue item, signal it is done
            iSourceQueueItemProcessed = ETrue;
            // If src queue item is last buffer signal sink buffer
            if ( srcItem->IsLastBuffer() )
                {
                snkItem->Buffer()->SetLastBuffer( ETrue );
                iSinkQueueItemProcessed = ETrue;
                }
            else if ( snkItem->Buffer()->Data().Length() == snkItem->Buffer()->Data().MaxLength() )
                {                    
                snkItem->Buffer()->SetLastBuffer( EFalse );
                iSinkQueueItemProcessed = ETrue;
                }
                
/*
            if ( iSinkQueueItemProcessed )
                {
            	TUint8* bufPtr = const_cast<TUint8*>(snkItem->Buffer()->Data().Ptr());
            	if ( snkItem->Buffer()->Data().Length() > 5 )
                	{
                	RDebug::Print(_L("CDataCopyEngineAO::RunL 1 data[0x%x][0x%x][0x%x][0x%x][0x%x]"), \
                	*bufPtr,*(bufPtr+1),*(bufPtr+2),*(bufPtr+3),*(bufPtr+4));
                    }
                }
*/

            
            // Continue
            KickSignal();
            return;
            }
        else // if (srcItem->DataSize() <=
            {
            // The data in the source queue item is larger than what
            // destination can hold. So copy data to local buffer
            iSrcDataDes = HBufC8::NewL(srcItem->DataSize());
            TPtr8 ptr = iSrcDataDes->Des();
            srcItem->ReadData(ptr);
            iSrcDataPos = 0;
            }
        }
    // Now we have data copied over to temp buffer
    TInt copySize = Min( ( iSrcDataDes->Des().Length() - iSrcDataPos),
        ( snkItem->Buffer()->Data().MaxLength() - snkItem->Buffer()->Data().Length() ) );
    // Copy data from source to destination queue item
    snkItem->Buffer()->Data().Append( ( iSrcDataDes->Des().Ptr() + iSrcDataPos ),
        copySize );
    // Update the source position.
    iSrcDataPos += copySize;
    // Update the position in destination queue item.
    //snkItem->Buffer()->SetPosition( snkItem->Buffer()->Position() + copySize );
    
    if ( iSrcDataPos == iSrcDataDes->Des().Length() )
        {
        iSourceQueueItemProcessed = ETrue;
        }
    // If src queue item is last buffer signal sink buffer
    if ( srcItem->IsLastBuffer() && (iSrcDataPos == iSrcDataDes->Des().Length()) )
        {
        snkItem->Buffer()->SetLastBuffer( ETrue );
        iSinkQueueItemProcessed = ETrue;
        }
    else if ( snkItem->Buffer()->Data().Length() == snkItem->Buffer()->Data().MaxLength() )
        {
        snkItem->Buffer()->SetLastBuffer( EFalse );
        iSinkQueueItemProcessed = ETrue;
        }
    
/*
    if ( iSinkQueueItemProcessed )
        {
    	TUint8* bufPtr = const_cast<TUint8*>(snkItem->Buffer()->Data().Ptr());
    	if ( snkItem->Buffer()->Data().Length() > 5 )
        	{
        	RDebug::Print(_L("CDataCopyEngineAO::RunL 2 data[0x%x][0x%x][0x%x][0x%x][0x%x]"), \
        	*bufPtr,*(bufPtr+1),*(bufPtr+2),*(bufPtr+3),*(bufPtr+4));
            }
        }
*/

    // Continue
    KickSignal();
    return;
    
    }
    
    void CDataCopyEngineAO::DoCancel()
        {
        }
    
    TInt CDataCopyEngineAO::RunError(TInt /*aError*/)
        {
        // FIX LATER Panic client
        return KErrNone;
        }
    
    //  End of File
