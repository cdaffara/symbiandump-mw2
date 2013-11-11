/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Class definition for the 3gpDataSourceAdapter functions.
*
*/



#ifndef C3GPDATASOURCEADAPTER_H
#define C3GPDATASOURCEADAPTER_H

//  INCLUDES
#include "DataSourceAdapter.h"
//#include <mmf/server/mmfdatasource.h>
//#include <caf/caf.h>
#include <MultimediaDataSource.h>
#include <mmf/server/mmfdatasink.h>
#include <mp4lib.h>

// FORWARD DECLARATIONS

// CLASS DEFINITIONS

/**
*  This class provides C3gpDataSourceAdapter functions.
*
*  @lib C3gpDataSourceAdapter.lib
*  @since 3.2
*/
class C3gpDataSourceAdapter : 
	public CDataSourceAdapter,
	public MDataSink,
	public M3GPMP4LibAsyncObserver,
	public MMultimediaDataSourceObserver
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static C3gpDataSourceAdapter* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~C3gpDataSourceAdapter();

		// From MMultimediaDataSourceObserver
        IMPORT_C virtual void BufferFilled(CMMFBuffer* aBuffer);
        IMPORT_C virtual void Event(TUid aEvent);
        IMPORT_C virtual TInt GetBitRate(TUint& aBitRate);
		
		
		// From CDataSourceAdapter
		IMPORT_C virtual void FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId);
		IMPORT_C virtual void SourcePrimeL();
		IMPORT_C virtual void SourceStopL();
        IMPORT_C virtual TInt SeekToTime(TUint aTimeMs, TUint& aFoundTimeMs);
		IMPORT_C virtual void SetDataSourceL(MDataSource* aDataSource,
											MMultimediaDataSourceObserver* aMMultimediaDataSourceObserver,
											MAsyncEventHandler* aMAsyncEventHandler);

		// From MDataSink
		// The following functions are declared as pure virtual in MDataSink, therefore
		// declared here.  However, they are not implemented.
		IMPORT_C virtual void BufferFilledL ( CMMFBuffer* aBuffer );
		IMPORT_C virtual void EmptyBufferL( CMMFBuffer* aBuffer, MDataSource* aSupplier, TMediaId aMediaId );
		IMPORT_C virtual TFourCC SinkDataTypeCode( TMediaId aMediaId );
		IMPORT_C virtual TBool CanCreateSinkBuffer();
		IMPORT_C virtual CMMFBuffer* CreateSinkBufferL( TMediaId aMediaId, TBool& aReference );
		IMPORT_C virtual void ConstructSinkL( const TDesC8& aInitData );
		
		// New
        IMPORT_C virtual TInt ReadHeader(TUint32& aAudioLength, TUint32& aAudioType, 
                                         TUint8& aFramesPerSample, TUint32& aTimeScale, TUint32& aAverateBitRate);
                                         
		IMPORT_C virtual TInt ReadAudioSpecificConfig(TUint8* aDecSpecInfo, TUint32 aDecSpecInfoSizeIn, TUint32* aDecSpecInfoSizeOut);

        IMPORT_C virtual TBool IsPositonSeekable();		
		
		
		// From M3GPMP4LibAsyncObserver
	    void M3GPMP4LibAudioFramesAvailable(MP4Err aError,
        								    mp4_u32 aAudioSize, 
        									mp4_u32 aTimeStamp, 
        									mp4_u32 aReturnedFrames,
 					                        mp4_u32 aTimestamp2);

        void M3GPMP4LibVideoFrameAvailable(MP4Err aError,
        								   mp4_u32 aFrameSize, 
        								   mp4_u32 aTimeStamp, 
        								   mp4_bool aKeyFrame, 
        								   mp4_u32 aTimestamp2);


	private:

        class CAsyncProxyFillBuffer : public CActive
            {
            public: // may want to export constructor and fillbuffer for 3gpdatasourceadapter
                CAsyncProxyFillBuffer(C3gpDataSourceAdapter& a3gpDataSourceAdapter);
            	~CAsyncProxyFillBuffer();
            	void ReadSync();
            
            protected:	
                void RunL();
                TInt RunError(TInt aError);
                void DoCancel();

            private:
                TRequestStatus* iRequestStatus;
                C3gpDataSourceAdapter& i3gpDataSourceAdapter;
            };
	
        /**
        * C++ default constructor.
        */
		C3gpDataSourceAdapter();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

        void PrepareMP4ParserL();
        TInt TranslateMP4Err(MP4Err aError);
        CMMFDataBuffer* CreateSourceBufferOfSizeL(TUint aSize);
        void StartReadingSourceL();
        TInt ReadAsync();
        TInt ReadSyncL();
		void ResetVariables();

        // currently filling buffer
        CMMFBuffer* iFillingBuffer;

        MP4Handle iMP4Handle;

		// for getting data out from MP4Lib and keeping the extra data that cannot fit into shared buffers
        CMMFDataBuffer* iParserBuf;
        
        TUint iSourceReadPosition;

		ContentAccess::CData* iCafHandle;
		CMMFDataBuffer* iSrcBuf;
		TBool iContinueReading;
		CMMFClip* iClip;
        MDataSink* iControllerMDataSink;
		RPointerArray<CMMFBuffer> iQueuedAsyncBuffers;
		TInt iBufferIndex;
		TMediaId iMediaId;
		TBool iQueValid;
		CAsyncProxyFillBuffer*	iAsyncProxyFillBuffer;

		TBool iReadMp4LibSync;
		TBool iRestartSyncRead;
	    TBool iLastBufferWrittenToMp4Lib;
		TBool iMp4HeaderAvail;
		mp4_u32 iAudioType;
		MMultimediaDataSourceObserver* iControllerMMultimediaDataSourceObserver;
};

#endif 		// C3GPDATASOURCEADAPTER_H

// End of File
