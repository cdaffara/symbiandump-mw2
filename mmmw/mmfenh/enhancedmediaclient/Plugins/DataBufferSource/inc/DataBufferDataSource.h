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
* Description:  Definition of the S60 Audio Stream Source custom command msg cods.
*
*/


#ifndef DATABUFFERDATASOURCE_H
#define DATABUFFERDATASOURCE_H

#include <e32base.h>
#include <DRMConfigTypes.h>

// Public Media Server includes
//#include <mmfdatasink.h>
#include <mmfdatasource.h>
//#include <mmfdatapath.h>
#include <mmfdatasourcesink.hrh>
#include <mmfaudioiointerfaceuids.hrh>
#include <mmffourcc.h>
#include <MultimediaDataSource.h>
#include <DataBufferSource.h>
#include "DataCopyEngineObserver.h"
#include "ParentChildIntfc.h"

// FORWARD DECLARATIONS
class CSourceQueueItem;
class CSinkQueueItem;
class CDataCopyEngineAO;
using multimedia::TBufferingConfig;
class CDataBufferMultimediaSource;
// CLASS DECLARATION
class CDataBufferSource : public MDataSource
    {
    public:
        static CDataBufferSource* NewL( TUid aType );
        ~CDataBufferSource(void);
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
        
        void SetMultimediaSource(CDataBufferMultimediaSource& aMultimediaSource);
        
    private:
        CDataBufferSource(TUid aType);
        
        void ConstructL (void);
    private:
        //Data
        CDataBufferMultimediaSource* iMultiMediaSource;        
 
    };
    
class CDataBufferMultimediaSource : public CMultimediaDataSource,
                                    public MDataCopyEngineObserver,
                                    public CParentIntfc
    {
    public:
        IMPORT_C static CDataBufferMultimediaSource* NewL(MDataSource& aDataSource);
        
        // From CMultimediaDataSource begins
        TInt SetObserver( MMultimediaDataSourceObserver& aObserver );
        TInt GetObserver( MMultimediaDataSourceObserver*& aObserver );
        void Event( TUid aEvent );
        TInt SetDataTypeCode(TFourCC aSourceFourCC );
        TInt GetDataTypeCode(TFourCC& aSourceFourCC );
        TInt GetSize( TUint& aSize );
        TInt Open();
        TInt Close();
        TInt Prime();
        TInt Play();
        TInt Stop();
        TInt FillBuffer( CMMFBuffer* aBuffer);
        TInt GetInterface(
                TUid aInterfaceId,
                TVersion& aVer,
                TAny*& aInterfaceImpl );
        TInt EvaluateIntent( ContentAccess::TIntent aIntent );
        TInt ExecuteIntent( ContentAccess::TIntent aIntent );
        // From CMultimediaDataSource ends
        
        // From MCustomInterface
        TAny* CustomInterface( TUid aInterfaceUid );
        
        
        // From MDataCopyEngineObserver begins
        // Called by iDataCopyEngineAO when item on source queue is processed
        void SourceQueueItemProcessed();
        // Called by iDataCopyEngineAO when item on sink queue is processed
        void SinkQueueItemProcessed();
        // From MDataCopyEngineObserver ends

        void SourceCustomCommand(TMMFMessage& aMessage);
        
        // A common function that implements FillBuffer requests
        TInt ServiceFillBuffer( CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aObserver,
            MDataSink* aConsumer );

        // To be called by DRMConfig Interface begins
        TInt GetDRMType( TDRMType& aDRMType );
        TInt GetAllowedOutputDeviceCount( TInt& aCount );
        TInt GetAllowedOutputDevice(
                        TInt aIndex,
                        TDRMAllowedOutputDevice& aOutputDevice );
        // To be called by DRMConfig Interface ends
        
    private:
        CDataBufferMultimediaSource(MDataSource& aDataSource);
        ~CDataBufferMultimediaSource(void);
        void ConstructL (void);
         // A common function that implements FillBuffer requests
        TInt AppendBufferToSinkQueue( CMMFBuffer* aBuffer,
            MMultimediaDataSourceObserver* aObserver,
            MDataSink* aConsumer );
        // Reset queues
        TInt EmptySourceQueue();
        TInt EmptySinkQueue();
        
        // Calculate BufferedDataSize
        void CalculateBufferdDataSize();
        
        // Handles SetDRMConfig custom command
        TInt HandleSetDRMConfig(  TMMFMessage& aMessage );
        // Implements SetDRMConfig custom command
        void DoSetDRMConfigL( TMMFMessage& aMessage );
        TInt DoHandleCancel( TMMFMessage& aMessage );
        
    private: // Data
        
        // Queue of data buffers from client
        TSglQue<CSourceQueueItem>* iSourceQueue;
        // Queue of buffers from controller
        TSglQue<CSinkQueueItem>* iSinkQueue;
        // Pointer to DataCopyEngineAO owned by this
        CDataCopyEngineAO* iDataCopyEngineAO;
        // Source item Counter
        TInt iSrcItemsCount;
        // Sink item Counter
        TInt iSnkItemsCount;
        // Number of bytes received from client so far
        TUint iSrcBytes;
        // Number of bytes copied to controller so far
        TUint iSnkBytes;
        // Size of source (Set by the application)
        TUint iSizeBytes;
       // FourCC
        TFourCC iSourceFourCC;
       // Reference to MMultimedia data source
        MMultimediaDataSourceObserver* iObserver;
        // Observer bit rate in bps
        TUint   iObserverBitRate;
        // Transfer bit rate in bps
        TUint   iTransferRate;
        // Buffering size in bytes
        TUint   iBufferedDataSize;
        // Holds true if last buffer is received or not
        TBool   iLastBufferReceived;
        
        TBufferingConfig iBufferingConfig;
        
        TMMFMessage* iMessage;
        
        /* Attributes to hold DRM Info */
        // Holds DRM Type set by client
        TDRMType iDRMType;
        // Holds list of allowed output devices set by client
        RArray<TDRMAllowedOutputDevice> iAllowedOutputDeviceList;
        // Parent MDataSource
        MDataSource* iParentDataSource;

    };

#endif // DATABUFFERDATASOURCE_H

    //  End of File
