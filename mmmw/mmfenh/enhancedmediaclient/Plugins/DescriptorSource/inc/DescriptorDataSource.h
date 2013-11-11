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
* Description:  Definition of the S60 Descriptor Data Source.
*
*/


#ifndef DESCRIPTORDATASOURCE_H
#define DESCRIPTORDATASOURCE_H

#include <e32base.h>

// Public Media Server includes
#include <mmfdatasource.h>
#include <MultimediaDataSource.h>
#include "DescriptorDataSourceCommon.h"

// FORWARD DECLARATIONS
class CSinkQueueItem;
class CDescriptorDataMultimediaSource;
// CLASS DECLARATION
class CDescriptorDataSource : public MDataSource
    {
    public:
        static CDescriptorDataSource* NewL( TUid aType );
        ~CDescriptorDataSource(void);
        
        // From MDataSource begins
        TUid DataSourceType() const;
        TFourCC SourceDataTypeCode( TMediaId aMediaId );
        TInt SetSourceDataTypeCode( TFourCC aSourceFourCC, TMediaId aMediaId );
        void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId );
        void BufferEmptiedL( CMMFBuffer* aBuffer );
        TBool CanCreateSourceBuffer();
        CMMFBuffer* CreateSourceBufferL( TMediaId aMediaId, TBool& aReference );
        TInt SourceThreadLogon(MAsyncEventHandler& aEventHandler);
        void SourceThreadLogoff();
        void SourcePrimeL();
        void SourcePlayL();
        void SourceStopL();
        void SourceCustomCommand(TMMFMessage& aMessage);
        void ConstructSourceL(  const TDesC8& aInitData );
        // From MDataSource ends
        
        void SetMultimediaSource(CDescriptorDataMultimediaSource& aMultimediaSource);
        
    private:
        CDescriptorDataSource(TUid aType);
        void ConstructL (void);
    private:
        //Data
        CDescriptorDataMultimediaSource* iMultiMediaSource;        
 
    };
    
    
class CDescriptorDataMultimediaSource : public CMultimediaDataSource
    {
    public:
        IMPORT_C static CDescriptorDataMultimediaSource* NewL(MDataSource& aDataSource);
        
        // From CMultimediaDataSource begins
        TInt SetObserver( MMultimediaDataSourceObserver& aObserver );
        TInt GetObserver( MMultimediaDataSourceObserver*& aObserver );
        void Event( TUid aEvent );
        TInt SetDataTypeCode( TFourCC aSourceFourCC );
        TInt GetDataTypeCode( TFourCC& aSourceFourCC );
        TInt GetSize( TUint& aSize );
        TInt Open();
        TInt Close();
        TInt Prime();
        TInt Play();
        TInt Stop();
        TInt FillBuffer( CMMFBuffer* aBuffer );
        TInt GetSeekingSupport( TBool& aSeekSupport );
        TInt GetRandomSeekingSupport( TBool& aSeekSupport );
        TInt Seek( TUint aPosInBytes );
        TInt EvaluateIntent( ContentAccess::TIntent aIntent );
        TInt ExecuteIntent( ContentAccess::TIntent aIntent );
        TInt SetAgentProperty( ContentAccess::TAgentProperty aProperty, TInt aValue);

        // From CMultimediaDataSource ends
        
        // From MCustomInterface
        TAny* CustomInterface( TUid aInterfaceUid );

        void SourceCustomCommand(TMMFMessage& aMessage);
         // A common function that implements FillBuffer requests
        TInt ServiceFillBuffer( CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aObserver,
            MDataSink* aConsumer );
       
    private:
        CDescriptorDataMultimediaSource(MDataSource& aDataSource);
        ~CDescriptorDataMultimediaSource();
        void ConstructL();
        // A common function that implements FillBuffer requests
        TInt AppendBufferToSinkQueue( CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aObserver,
            MDataSink* aConsumer );
            
        void AppendEventToQueue( TDescriptorDataSourceEvent& aEvent );
        void SendEventToClient();

        // Reset queues
        TInt EmptyEventQueue();
        TInt EmptySinkQueue();
        
        TInt DoSetConfig( TMMFMessage& aMessage );
        TInt DoGetSourceEvent( TMMFMessage& aMessage );
        TInt DoFillDataResponse( TMMFMessage& aMessage );
        
    private: // Data
        // True if Source position can be set to zero
        TBool iSeekingSupported;
        // True if Source position can be set to anywhere between zero and size of clip
        TBool iRandomSeekingSupported;
        // Size of source (Set by the application)
        TUint iSizeBytes;
        // Queue of buffers from controller
        TSglQue<CSinkQueueItem>* iSinkQueue;
        // Sink item Counter
        TInt iSnkItemsCount;
        // FourCC
        TFourCC iSourceFourCC;
        // Reference to MMultimedia data source
        MMultimediaDataSourceObserver* iObserver;
        // Observer bit rate in bps
        TUint iObserverBitRate;
        // Array to hold events to client side object
        RPointerArray<TDescriptorDataSourceEvent> iEventsQueue;
        // Asnyc message for reading source events
        TMMFMessage* iMessage;
        // Parent MDataSource
        MDataSource* iParentDataSource;        
    };

#endif // DESCRIPTORDATASOURCE_H

//  End of File
