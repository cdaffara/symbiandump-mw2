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


#ifndef CACHESOURCE_H
#define CACHESOURCE_H

#include <e32base.h>
#include <DataSourceConfigIntfc.h>
#include <MultimediaDataSource.h>

// FORWARD DECLARATIONS
class MDataSource;
class CSinkQueueItem;
class CReadWriteRequestAO;
class CDataSourceConfigIntfc;
class CMMFDataBuffer;
// CLASS DECLARATION

class CMDataSourceObserver : public MMultimediaDataSourceObserver
    {
    public:
      static CMDataSourceObserver* NewL(
            CMultimediaDataSource* aDataSource
            ,CMultimediaDataSource* aParent );
        
        virtual ~CMDataSourceObserver();

    private:
        CMDataSourceObserver(CMultimediaDataSource* aDataSource ,CMultimediaDataSource* aParent);
        void ConstructL();
        
        void BufferFilled( CMMFBuffer* aBuffer );
        
        // KStreamControlEventSeekingSupportChanged
        // KStreamControlEventRandomSeekingSupportChanged
        // KMultimediaDataSourceObserverEventSourceSizeChanged
        void Event( TUid aEvent );
        // Returns BITRATE. This is used for calculation how much of data need to be
        // buffered by the source before filling observer buffers.
        TInt GetBitRate( TUint& aBitRate );
        
        CMultimediaDataSource* iDataSource;
        CMultimediaDataSource* iParent;
    };


class CCacheSource : public CMultimediaDataSource
                            //,public MCacheCopyEngineObserver
    {
    public:
        IMPORT_C static CCacheSource* NewL( CMultimediaDataSource* aDataSource, CDataSourceConfigIntfc* aDataSourceConfig );
        
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
        

        // From MCustomInterface
        TAny* CustomInterface( TUid aInterfaceUid );

       /* Seeking Interface */
        // Returns true if byte position can be reset to zero.
        // Valid in all states
        TInt GetSeekingSupport( TBool& aSeekSupport );
        // Returns true if byte position can be set anywhere between zero and
        // GetSize().
        // Valid in all states
        TInt GetRandomSeekingSupport( TBool& aSeekSupport );
        // Moves read position pointer to byte location requested by the observer
        // between zero and GetSize(). If the size specified is out of range, this
        // function returns KErrArgument.
        TInt Seek( TUint aPosInBytes );
        // From CMultimediaDataSource ends
        
        TInt ServiceBufferFilled( CMMFBuffer* aBuffer );
        void ReadRequestComplete(CReadWriteRequestAO* aRequest,TRequestStatus& aStatus);
        ~CCacheSource();
    private:
 
        CCacheSource(CMultimediaDataSource* aDataSource, CDataSourceConfigIntfc* aDataSourceConfig);
        
        void ConstructL (void);
        
        TInt EmptySinkQueue();
        TInt AppendBufferToSinkQueue( CMMFBuffer* aBuffer );
        TInt ServiceFillBuffer();
        
    private: // Data
        
        // Queue of data buffers from client
        TSglQue<CSinkQueueItem>* iSinkQueue;
        // Reference to MMultimedia data source
        MMultimediaDataSourceObserver* iObserver;
        
        CMDataSourceObserver* iDataSourceObserver;
        // Parent DataSource to Talk to
        CMultimediaDataSource* iDataSource;
        // Sink item Counter
        TInt iSnkItemsCount;
        //Temp File to Store Data
        RFile iFile;
        //Session to File server.
        RFs iFs;
        //File Name and Path for the Temp file
        TFileName iFileName;
        //Last Buffer Flag
        TBool iLastBufferWritten;
        //Size of the Temp File
        TInt iFileSize;
        // Number of bytes copied to controller so far
        TUint iSnkBytes;

        CMMFDataBuffer* iTransferBuffer;
        
        CDataSourceConfigIntfc* iDataSourceConfig;
        
        CReadWriteRequestAO* iReadRequest;
        CReadWriteRequestAO* iWriteRequest;
        
        CDataSourceConfigIntfc::TCacheType iCacheType;
        CMMFDataBuffer* iDataBuffer;
        HBufC*          iCacheLocation;
        TInt            iAbsBufferStart;
        TInt            iAbsBufferEnd;
        TInt            iBufferReadPos;
        TInt            iSeekStart;
        TInt            iSeekEnd;
        TInt            iAbsBufferReadPos;

    };

#endif // CACHESOURCE_H

    //  End of File
