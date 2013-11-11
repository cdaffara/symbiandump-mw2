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
* Description:  Class definition for the DataSourceAdapter functions.
*
*/


//  INCLUDES
#include "3gpDataSourceAdapter.h"
#include "DebugMacros.h"
#include <mmfclip.h>
//#include <mmfdatabuffer.h>
#include <MultimediaDataSourceFactory.h>
#include <mmffile.h>
#include <caf/caf.h>
#include <mmfcontroller.h>


#include <oma2dcf.h> // EPeek
const TInt KDataSourceReadBufferSize = 16*1024; // read from source
const TInt KParserReadBufferSize = 2000; // read from parser - it only gives 1 frame
const TInt KMaxFrameSize = 1536;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::C3gpDataSourceAdapter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
C3gpDataSourceAdapter::C3gpDataSourceAdapter() :
    	MDataSink(KUidMmfFormatDecode)

    {
    }

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::ConstructL()
    {
	// should end up doing this for all sources
	iMediaId = KUidMediaTypeAudio;
	iParserBuf = CreateSourceBufferOfSizeL(KParserReadBufferSize);
	iSrcBuf = CreateSourceBufferOfSizeL(KDataSourceReadBufferSize);
	iZeroBuffer = CMMFDataBuffer::NewL(0); // this is just for test to allow source seek
	iHdrBuffer = CMMFDataBuffer::NewL(20); // just enough to cover the header lengths create by recorder
    iAsyncProxyFillBuffer = new(ELeave) CAsyncProxyFillBuffer(*this);

	}

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C C3gpDataSourceAdapter* C3gpDataSourceAdapter::NewL()
    {
    DP0(_L("C3gpDataSourceAdapter::NewL"));
    C3gpDataSourceAdapter* self = new(ELeave) C3gpDataSourceAdapter();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C C3gpDataSourceAdapter::~C3gpDataSourceAdapter()
    {
    DP0(_L("C3gpDataSourceAdapter::~C3gpDataSourceAdapter"));
	TRAP_IGNORE(SourceStopL());
	MP4ParseClose(iMP4Handle);
	delete iParserBuf;
	delete iSrcBuf;
    delete iCafHandle;
    iQueuedAsyncBuffers.Close(); // this is different from one in base class
	delete iAsyncProxyFillBuffer; // this is different from one in base class
    }

// -----------------------------------------------------------------------------
// From MDataSource
// -----------------------------------------------------------------------------
//
EXPORT_C void C3gpDataSourceAdapter::FillBufferL(CMMFBuffer* aBuffer, MDataSink* aDataSink,TMediaId /*aMediaId*/)
	{
	TInt status = KErrNone;
    CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(aBuffer);
		
	DP1(_L("C3gpDataSourceAdapter::FillBufferL buffer[%x]"), buffer->Data().Ptr());
	if (aDataSink != this)
	    { // called from the controller
        iControllerMDataSink = aDataSink;
        iQueuedAsyncBuffers.Append(aBuffer);
	    if (iQueuedAsyncBuffers.Count() > 1)
	        {
			DP1(_L("C3gpDataSourceAdapter::FillBufferL buffer queued count[%d]"), iQueuedAsyncBuffers.Count());
    	    return;
	        }
	    }
        
    if (iParserBuf->Data().Length())
        {
		// Audio Frames previously read is appended first
        buffer->Data().Append(iParserBuf->Data().Ptr(), iParserBuf->Data().Length());
		iParserBuf->Data().SetLength(0);
        iParserBuf->SetPosition(0);
        }
        
    iFillingBuffer = aBuffer;
    if (iReadMp4LibSync)
    	{
	    iAsyncProxyFillBuffer->ReadSync();
    	}
    else
    	{
	    status = ReadAsync();
    	}	

    if (status == KErrNone)
        {
        // do nothing
        }
    else
        { // define the use case for this...
        DP1(_L("C3gpDataSourceAdapter::FillSharedBufferL, ReadAsync status[%d] calling frames available"), status);
        M3GPMP4LibAudioFramesAvailable(status, 0, 0,0,0);
        }
	}

EXPORT_C void C3gpDataSourceAdapter::SourcePrimeL()
	{
	DP0(_L("C3gpDataSourceAdapter::SourcePrimeL"));
	if(iDataSource)
		{
		iDataSource->SourcePrimeL();
		if (!iMP4Handle)
			{
			PrepareMP4ParserL();
			}
		}	
	}
	
EXPORT_C void C3gpDataSourceAdapter::SourceStopL()
	{
	DP0(_L("C3gpDataSourceAdapter::SourceStopL"));
	ResetVariables();
	MP4ParseClose(iMP4Handle);
	iMP4Handle = NULL;
	if(iDataSource)
		{
		iDataSource->SourceStopL();
		}
	// Clear previously read Audio Frames  
	iParserBuf->Data().SetLength(0);
	iParserBuf->SetPosition(0);	
	}

// -----------------------------------------------------------------------------
// From MMultimediaDataSourceObserver
// -----------------------------------------------------------------------------
//
EXPORT_C void C3gpDataSourceAdapter::BufferFilled(CMMFBuffer* aBuffer)
	{
	TRAPD(err,BufferFilledL(aBuffer));
	if (err != KErrNone)
		{//send event to client to notify error in source adapter
	//	SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, err));		
		}
	}

EXPORT_C void C3gpDataSourceAdapter::Event(TUid aEvent)
	{
	iControllerMMultimediaDataSourceObserver->Event(aEvent);
	}
	
EXPORT_C TInt C3gpDataSourceAdapter::GetBitRate(TUint& aBitRate)
	{
	return iControllerMMultimediaDataSourceObserver->GetBitRate(aBitRate);
	}
	
// -----------------------------------------------------------------------------
// From CDataSourceAdapter
// -----------------------------------------------------------------------------
//
EXPORT_C TInt C3gpDataSourceAdapter::SeekToTime(TUint aTimeMs, TUint& aFoundTimeMs)
	{
	DP0(_L("C3gpDataSourceAdapter::SeekToTime"));
	TInt status = KErrNone;
    mp4_u32 audioTimeStamp, videoTimeStamp;
	MP4Err err = MP4ParseSeek(iMP4Handle, aTimeMs, &audioTimeStamp, &videoTimeStamp, EFalse);
	
	aFoundTimeMs = audioTimeStamp;

	DP3(_L("C3gpDataSourceAdapter::SeekToTime stat[%d] timeMs[%d] foundtimeMs[%d]"), err, aTimeMs,aFoundTimeMs);

	status = TranslateMP4Err(err);	
	return status;
	}

EXPORT_C void C3gpDataSourceAdapter::SetDataSourceL(MDataSource* aDataSource,
								 MMultimediaDataSourceObserver* aMMultimediaDataSourceObserver,
								 MAsyncEventHandler* aAsyncEventHandler)
	{
	DP0(_L("C3gpDataSourceAdapter::SetDataSource"));
	iHeaderOnly = EFalse;
	iControllerMMultimediaDataSourceObserver = aMMultimediaDataSourceObserver;
	iAsyncEventHandler = aAsyncEventHandler;
	ResetVariables();
   	if (iDataSource)
    	{
	    return;
   		}

    if (iCafHandle)
        {
        delete iCafHandle;
        iCafHandle = NULL;
        }

  	iDataSource = aDataSource;
    iSourceType = iDataSource->DataSourceType();
	DP1(_L("C3gpDataSourceAdapter::SetDataSource DataSourceType[0x%x]"), iSourceType);
    
    if ((iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) ||
        (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmdsDescriptorSourceUid) ||
        (iSourceType == KUidMmfDescriptorSource) || (iSourceType == KMmdsFileSourceUid))
		{ // data is streamed in buffers to parser and we read sync from parser - parser does not support seeking
		iReadMp4LibSync = ETrue;
		}
	else
		{ // parser has file handle and we read async from parser - parser supports seeking
		iReadMp4LibSync = EFalse;
    	iTimeSeekable = ETrue;
		}	

    if ((iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) ||
        (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmdsDescriptorSourceUid) ||
        (iSourceType == KMmdsFileSourceUid))
        { // data is written to parser
        TInt err = CMultimediaDataSourceFactory::CreateDataSource(*iDataSource, iMMDataSource);

        User::LeaveIfError(err);

//        iMMDataSource->SetObserver(*this);
        User::LeaveIfError(iMMDataSource->SetObserver(*this));
        User::LeaveIfError(iMMDataSource->Open());
        }
	if ((iSourceType == KUidMmfFileSource) || (iSourceType == KOldProgDLSourceUid))
    	{ // parser has file handle
	   	if (iSourceType == KUidMmfFileSource) 
			{
		    	iIsLocalPlayback = ETrue;
			}
		// read async from lib when it has a file handle
		iClip = static_cast<CMMFClip*>(iDataSource);
        iDataSource->SourcePrimeL();
   		iIsProtected = static_cast<CMMFFile*>(iDataSource)->IsProtectedL();
		iClip->SourceThreadLogon(*aAsyncEventHandler);
		iClip->ReadBufferL(iHdrBuffer,0);
		iClip->ReadBufferL(iZeroBuffer, 0); // seek back to 0 position
		const TUint8* ptr = iHdrBuffer->Data().Ptr();
    	if (iClip->Size() == 11)
    		{ // check for just the header from recorder
    		TUint32* w1p = (TUint32*)ptr;
    		TUint32* w2p = (TUint32*)(ptr+4);
    		TUint32* w3p = (TUint32*)(ptr+8);
    		if (((*w1p & 0x1C000000)==0x1C000000) && 
    			((*w2p & 0x70797466)==0x70797466) && 
    			((*w3p & 0x0034706D)==0x0034706D))
    			{
	    		iClip->SourceStopL();
    			iHeaderOnly = ETrue;
    			return;
    			}
    		}
    		
        if (iSourceType == KUidMmfFileSource)
            {
            CMMFFile* File = static_cast<CMMFFile*>(iDataSource);                
            RFile rFile = File->FileL();
            iCafHandle = ContentAccess::CData::NewL(rFile, File->UniqueId(), EPeek);                
            }
    	}
	}

// -----------------------------------------------------------------------------
// From MDataSink
// -----------------------------------------------------------------------------
//
EXPORT_C void C3gpDataSourceAdapter::BufferFilledL(CMMFBuffer* aBuffer)
	{ // only talks to MMDataSource asynchronously using a single iSrcBuf
	// aBuffer here is just the one iSrcBuf
    CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(aBuffer);
    DP2(_L("C3gpDataSourceAdapter::BufferFilledL[%x], LastBuffer[%d]"), buffer->Data().Ptr(), aBuffer->LastBuffer());

	TUint bufferSize = buffer->Data().Length();

    DP1(_L("C3gpDataSourceAdapter::BufferFilledL, Length[%d]"), bufferSize);

	TUint8* bufPtr = const_cast<TUint8*>(buffer->Data().Ptr());
    TInt err = KErrNone;
/*	
	if ( bufferSize > 5 )
	{
	DP5(_L("C3gpDataSourceAdapter::BufferFilledL data[0x%x][0x%x][0x%x][0x%x][0x%x]"), \
	*bufPtr,*(bufPtr+1),*(bufPtr+2),*(bufPtr+3),*(bufPtr+4));
    }
*/	
    if (bufferSize > 0)
   	    {
   	    err = TranslateMP4Err(MP4ParseWriteData(iMP4Handle, bufPtr, bufferSize));
		//mp4_u32 audioLength, audioType, timeScale, averateBitRate;
		mp4_u32 audioLength, timeScale, averateBitRate;
		mp4_u8 framesPerSample;
		if (!iMp4HeaderAvail)
			{
			if (MP4_OK == MP4ParseRequestAudioDescription(iMP4Handle, &audioLength, &iAudioType,	&framesPerSample, &timeScale, &averateBitRate))
				{ // header is required to do anything.
				iMp4HeaderAvail = ETrue;
				}
			}

        DP3(_L("C3gpDataSourceAdapter::BufferFilledL, MP4ParseWriteData err[%d], bufferSize[%d] d0[%x]"), err, bufferSize, bufPtr[0]);
        if (err == MP4_OUT_OF_MEMORY)
        	{
	        DP0(_L("C3gpDataSourceAdapter::BufferFilledL, out of memory abort"));
	        User::Leave(KErrAbort);
        	}
    	if (err == MP4_OK)
    		{
	  	    if (!aBuffer->LastBuffer())
   		        {
	   	        // continue getting next buffer
		        DP1(_L("C3gpDataSourceAdapter::BufferFilledL, iMMDataSource->FillBuffer buffer[%x]"), iSrcBuf->Data().Ptr());
	       	    buffer->SetPosition(0);
			    buffer->Data().SetLength(0);
				iMMDataSource->FillBuffer(iSrcBuf);
	   	    	}
		    else
		        {
	    	    // do nothing, since the file has reach the end
            	DP0(_L("C3gpDataSourceAdapter::BufferFilledL, reach EOF"));
	            iLastBufferWrittenToMp4Lib = ETrue;
		        }
    		}
       	} // buffersize > 0
   	else
   	    { // buffersize <= 0
   	    // should never reach here, since no callback should occur if no data available for streaming
        DP0(_L("C3gpDataSourceAdapter::BufferFilledL, unexpected 0 length buffer"));
        User::Leave(KErrAbort);
   	    }
	if (iRestartSyncRead)
		{
		iRestartSyncRead = EFalse;
        DP0(_L("C3gpDataSourceAdapter::BufferFilledL, restarting parser read ReadSync()"));
	    iAsyncProxyFillBuffer->ReadSync();
		}
	}

EXPORT_C void C3gpDataSourceAdapter::EmptyBufferL(
	CMMFBuffer* /*aBuffer*/,
	MDataSource* /*aSupplier*/,
	TMediaId /*aMediaId*/ )
	{}

EXPORT_C TFourCC C3gpDataSourceAdapter::SinkDataTypeCode(
	TMediaId /*aMediaId*/ )
	{
	return KFourCCNULL;
	}

EXPORT_C TBool C3gpDataSourceAdapter::CanCreateSinkBuffer()
	{
	return EFalse;
	}

EXPORT_C CMMFBuffer* C3gpDataSourceAdapter::CreateSinkBufferL(
	TMediaId /*aMediaId*/,
	TBool& /*aReference*/ )
	{
	return NULL;
	}

EXPORT_C void C3gpDataSourceAdapter::ConstructSinkL(
	const TDesC8& /*aInitData*/ )
	{}

EXPORT_C TBool C3gpDataSourceAdapter::IsPositonSeekable()
    {
    DP1(_L("C3gpDataSourceAdapter::IsPositonSeekable DataSourceType[0x%x]"), iSourceType);
    if ((iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) ||
        (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmdsDescriptorSourceUid) ||
        (iSourceType == KUidMmfDescriptorSource) || (iSourceType == KMmdsFileSourceUid) ||
        (iSourceType == KOldProgDLSourceUid))
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }   
    }
// -----------------------------------------------------------------------------
// From M3GPMP4LibAsyncObserver
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable(MP4Err aError,
                                                                mp4_u32 aAudioSize,
                                                                mp4_u32 /*aTimeStamp*/,
                                                                mp4_u32 /*aReturnedFrames*/,
                                                                mp4_u32 /*aTimestamp2*/)
    {
    DP2(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable, aError = %d, aAudioSize = %d"), aError, aAudioSize);
    DP2(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable, iParserBuf[%x] d0[%x]"), static_cast<CMMFDataBuffer*>(iParserBuf)->Data().Ptr(), static_cast<CMMFDataBuffer*>(iParserBuf)->Data().Ptr()[0]);

    TInt err = aError;
    if (err == MP4_OK)
        {
        TUint8* bufPtr = const_cast<TUint8*>(iParserBuf->Data().Ptr());
    	TInt requestSize = static_cast<CMMFDataBuffer*>(iFillingBuffer)->RequestSize();
    	TInt bufferlength = static_cast<CMMFDataBuffer*>(iFillingBuffer)->Data().Length()+aAudioSize;
	
        if (bufferlength <= requestSize)
            {
/*            TUint32 timePositionInMilliSecs = I64INT(iPlayWindowEndPosition.Int64())/1000;
            
            if ((iPlayWindowEndPosition != TTimeIntervalMicroSeconds(0)) && (aTimeStamp >= timePositionInMilliSecs))
                {
                iFillingBuffer->SetLastBuffer(ETrue);
                iFillingBuffer->SetStatus(EFull);
                }
            else // Not reached end of playwindow or no playwindow at all
*/                {
                static_cast<CMMFDataBuffer*>(iFillingBuffer)->Data().Append(iParserBuf->Data().Ptr(), aAudioSize);
                iParserBuf->Data().SetLength(0);
                iParserBuf->SetPosition(0);

                err = ReadAsync();
                // this error return value is handled below
                    
                }
            }
        else
            {
            // Save this read for the next source buffer
            DP2(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable, iFillingBuffer[%x] - FULL, Length[%d]"),
            		static_cast<CMMFDataBuffer*>(iFillingBuffer)->Data().Ptr(), static_cast<CMMFDataBuffer*>(iFillingBuffer)->Data().Length());
            iParserBuf->Data().SetLength(aAudioSize);
            iQueValid = ETrue;
            TRAP(err,iControllerMDataSink->BufferFilledL(iFillingBuffer)); // setpos could come in here and clear the queue here and add another read
            // this error return value is handled below
            if (iQueValid)
                { // this detects if the que has been cleared and refilled
                // we don't want to do this with the new buffers in the queue
                iQueuedAsyncBuffers.Remove(0);
                if (iQueuedAsyncBuffers.Count() > 0)
                    {
                    DP0(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable calls FillBufferL"));
                    TRAP(err,FillBufferL(iQueuedAsyncBuffers[0],this,iMediaId));
                    // this error return value is handled below
                    }
                }
            }
        }
    if (err == MP4_NO_FRAME)
	    {
        DP0(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable sending last buffer"));
	    iFillingBuffer->SetLastBuffer(ETrue);
        TRAP(err,iControllerMDataSink->BufferFilledL(iFillingBuffer));
       	if (err != KErrNone)
           	{
           	DP1(_L("C3gpDataSourceAdapter::M3GPMP4LibAudioFramesAvailable, BufferFilledL err[%d]"), err);

//            	SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, err));
           	}          
	    }
    else if (err != MP4_OK)
        {
        DP1(_L("error %d..."), TranslateMP4Err(err));
        
        //send event to client to notify error in source adapter
        if(iAsyncEventHandler)
        	{
         	iAsyncEventHandler->SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrSourceAdapter));
        	}
        }
    }

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::M3GPMP4LibVideoFrameAvailable
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::M3GPMP4LibVideoFrameAvailable(MP4Err /*aError*/,
                                                          mp4_u32 /*aFrameSize*/,
                                                          mp4_u32 /*aTimeStamp*/,
                                                          mp4_bool /*aKeyFrame*/,
                                                          mp4_u32 /*aTimestamp2*/)
    {
    // do nothing, not used
    }

// -----------------------------------------------------------------------------
// New
// -----------------------------------------------------------------------------
//
EXPORT_C TInt C3gpDataSourceAdapter::ReadHeader(TUint32& aAudioLength, TUint32& aAudioType, 
            TUint8& aFramesPerSample, TUint32& aTimeScale, TUint32& aAverateBitRate)
	{
	TInt err = MP4ParseRequestAudioDescription(iMP4Handle, &aAudioLength, &aAudioType, &aFramesPerSample, &aTimeScale, &aAverateBitRate);
    DP6(_L("C3gpDataSourceAdapter::ReadHeader stat[%d] aAudioLength[%d], aAudioType[%d], aFramesPerSample[%d], aTimeScale[%d], aAverateBitRate[%d]"), 
    	err, aAudioLength, aAudioType, aFramesPerSample, aTimeScale, aAverateBitRate);
	iAudioType = aAudioType;    	
	return(TranslateMP4Err(err));
	}
    
EXPORT_C TInt C3gpDataSourceAdapter::ReadAudioSpecificConfig(TUint8* aDecSpecInfo, TUint32 aDecSpecInfoSizeIn, TUint32* aDecSpecInfoSizeOut)
	{
	TInt err = KErrNone;
	MP4Err stat = MP4ParseReadAudioDecoderSpecificInfo(iMP4Handle, aDecSpecInfo, aDecSpecInfoSizeIn, aDecSpecInfoSizeOut);
	if ((stat == MP4_BUFFER_TOO_SMALL) && (aDecSpecInfo == NULL)) // This is valid Error as we are just getting the buffersize
		{
		err = KErrNone;
		}
	else
		{
		err = TranslateMP4Err(stat);
		}
    DP2(_L("C3gpDataSourceAdapter::ReadAudioSpecificConfig info[0x%x] err[%d]"), aDecSpecInfo, err);
	return err;
	}
    
// -----------------------------------------------------------------------------
// Local
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::PrepareMP4ParserL()
	{
    DP0(_L("C3gpDataSourceAdapter::PrepareMP4ParserL"));

    MP4Err err = MP4_ERROR;
	
    if (iCafHandle)
        {
        // SourceStopL might terminate the access to the File, SourcePrimeL to regain access
        err = MP4ParseOpenCAF(&iMP4Handle, iCafHandle);
        DP1(_L("C3gpDataSourceAdapter::PrepareMP4ParserL, MP4ParseOpenCAF err = %d"), err);
        User::LeaveIfError(TranslateMP4Err(err));
        }
	else if (iSourceType == KUidMmfDescriptorSource)	// KUidMmfDescriptorSource
		{
		err = MP4ParseOpen(&iMP4Handle, NULL);
		User::LeaveIfError(TranslateMP4Err(err));

		CMMFClip* clip = static_cast<CMMFClip*>(iDataSource);

		TBool endOfFile = EFalse;
		iSourceReadPosition = 0;
		TUint8* buffer = const_cast<TUint8*>(iSrcBuf->Data().Ptr());
		while (!endOfFile)
			{
			clip->ReadBufferL(iSrcBuf, iSourceReadPosition);
			TUint buffersize = iSrcBuf->Data().Length();
			
			if (buffersize > 0)
				{
				err = MP4ParseWriteData(iMP4Handle, buffer, buffersize);
		        DP1(_L("C3gpDataSourceAdapter::PrepareMP4ParserL, write desc buffer to parser stat[%d]"), err);
				User::LeaveIfError(TranslateMP4Err(err));
				iSourceReadPosition += buffersize;
				}
			else
				{
				endOfFile = ETrue;
				// This call with empty buffer indicates to the 3GP library that
				// this is the last buffer.
				iLastBufferWrittenToMp4Lib = ETrue;
				err = MP4ParseWriteData(iMP4Handle, buffer, 0);
		        DP1(_L("C3gpDataSourceAdapter::PrepareMP4ParserL, write last desc buffer to parser stat[%d]"), err);
				User::LeaveIfError(TranslateMP4Err(err));
				}
			}

		clip->SourceStopL();
		}
    else if (iReadMp4LibSync && (iSourceType != KOldProgDLSourceUid))
//		    (iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) || 
//    		 (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmdsFileSourceUid) || 
//    		 (iSourceType == KMmdsDescriptorSourceUid))
        {
        err = MP4ParseOpen(&iMP4Handle, NULL);
        User::LeaveIfError(TranslateMP4Err(err));
        DP1(_L("C3gpDataSourceAdapter::PrepareMP4ParserL, iMMDataSource->FillBuffer buffer[%x]"), iSrcBuf->Data().Ptr());
   	    iSrcBuf->SetPosition(0);
	    iSrcBuf->Data().SetLength(0);
		iMMDataSource->FillBuffer(iSrcBuf);
        }
	else if (iSourceType == KOldProgDLSourceUid)
		{
		err = MP4ParseOpen(&iMP4Handle, NULL);
		User::LeaveIfError(TranslateMP4Err(err));

		iSourceReadPosition = 0;
		StartReadingSourceL();
		}
	else
		{
		User::Leave(KErrNotSupported);
		}
		
	// MP4ParseRequestAudioDescription() is used here to initialize the parser internally
	// before other parser functions can be used.
	// The data gathered here is not used and the function may return an error in some cases.
	// mp4_u32 audioLength, audioType, timeScale, averateBitRate;
	mp4_u32 audioLength, timeScale, averateBitRate;	
	mp4_u8 framesPerSample;
	err = MP4ParseRequestAudioDescription(iMP4Handle, &audioLength, &iAudioType,	&framesPerSample, &timeScale, &averateBitRate);
	if (err == MP4_OK)
		{ // mmf descriptor source or a short file would put complete content to the parser
		// so this needs to be set here.
		iMp4HeaderAvail = ETrue;
		}
	if /*( ((iSourceType == KOldProgDLSourceUid) || (iSourceType == KMmdsFileSourceUid) ||
    	  (iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) ||
    	  (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmdsDescriptorSourceUid)) &&*/
    	 ((err == MP4_NOT_AVAILABLE) || (err == MP4_FILE_ERROR)) //)
        {
        // do nothing
        }
    else
        {
        DP1(_L("C3gpDataSourceAdapter::PrepareMP4ParserL Leave, err = %d"), err);
        User::LeaveIfError(TranslateMP4Err(err));
        }
	}


// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::TranslateMP4Err
// -----------------------------------------------------------------------------
//
TInt C3gpDataSourceAdapter::TranslateMP4Err(
	MP4Err aError)
	{
    DP1(_L("C3gpDataSourceAdapter::TranslateMP4Err err = %d"), aError);
	TInt err;
	switch (aError)
		{
		case MP4_OK:
			err = KErrNone;
			break;
		case MP4_OUT_OF_MEMORY:
			err = KErrNoMemory;
			break;
		case MP4_NOT_AVAILABLE:     // valid error for progressive download and streaming source
			err = KErrNotReady;
			break;
		case MP4_FILE_ERROR:
			err = KErrBadHandle;
			break;
		case MP4_INVALID_TYPE:
			err = KErrNotSupported;
			break;
		case MP4_TIMESCALE_NOT_SET:
			err = KErrNotReady;
			break;
		case MP4_NOT_STREAMABLE:
		case MP4_NO_REQUESTED_FRAME:
		case MP4_CANT_SEEK:
		case MP4_INVALID_INPUT_STREAM:
		case MP4_NO_FRAME:
			err = KErrArgument;
			break;
		case MP4_ERROR:
		case MP4_FILE_MODE:
		case MP4_BUFFER_TOO_SMALL:
		case MP4_END_OF_VIDEO:
		case MP4_METADATA_ERROR:
		case MP4_NO_VIDEO:
		case MP4_NO_AUDIO:
			err = KErrGeneral;
			break;
		default:
			if (aError > 0)     //For Any 3GPLib Errors
                {
                err = KErrGeneral;
                }
            else                // For CAF Errors 
                {
                err = aError;
                }

			break;
		}

	return err;
	}


CMMFDataBuffer* C3gpDataSourceAdapter::CreateSourceBufferOfSizeL(
	TUint aSize )
    {
    CMMFDataBuffer* buffer = CMMFDataBuffer::NewL(aSize);
    CleanupStack::PushL(buffer);
    buffer->SetRequestSizeL(aSize);
    buffer->SetPosition(0);
	buffer->Data().SetLength(0);
	CleanupStack::Pop(buffer);	
    return buffer;
    }
    
    
// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::StartReadingSourceL
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::StartReadingSourceL()
	{
    DP1(_L("C3gpDataSourceAdapter::StartReadingSourceL, iSourceReadPosition = %d"), iSourceReadPosition);

	// kick off the asynchronous read from source
//	iClip->SourcePrimeL();
//	iClip->SourceThreadLogon(*this);
	iSrcBuf->SetPosition(0);
	iSrcBuf->Data().SetLength(0);
	iClip->ReadBufferL(iSrcBuf, iSourceReadPosition, this);
	iContinueReading = ETrue;
	}
	
// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::ReadAsync
// -----------------------------------------------------------------------------
//
TInt C3gpDataSourceAdapter::ReadAsync()
    {
    TUint8* bufPtr = const_cast<TUint8*>(iParserBuf->Data().Ptr());
    mp4_u32 buffersize = iParserBuf->RequestSize();
    DP2(_L("C3gpDataSourceAdapter::ReadAsync ptr[%x] size[%d]"), bufPtr, buffersize);
    MP4Err err = MP4_ERROR;
    
    if (iMP4Handle)
        {
	    err = MP4ParseReadAudioFramesAsync(iMP4Handle, this, bufPtr, &buffersize);
        }
    return err;
    }

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::ReadSyncL
// -----------------------------------------------------------------------------
//
TInt C3gpDataSourceAdapter::ReadSyncL()
    {
    TInt err = MP4_ERROR;
    CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(iFillingBuffer);
    TUint8* bufPtr = const_cast<TUint8*>(buffer->Data().Ptr()) + iFillingBuffer->Position();
	TInt buffersize = iFillingBuffer->RequestSize() - iFillingBuffer->Position();
    DP4(_L("C3gpDataSourceAdapter::ReadSyncL, buffer[%x] length[%d] buffersize[%d] position[%d]"),
		     buffer->Data().Ptr(), buffer->Data().Length(), buffersize, iFillingBuffer->Position());

	TInt bytesAdded = 0;
	mp4_u32 framesize;

	while (ETrue)
		{
		if (iMp4HeaderAvail)
			{ // know audio type after header read
			err = MP4ParseNextFrameSize(iMP4Handle, iAudioType, &framesize);
			if (err != MP4_OK)
				{ // default in case not enough data
				framesize = KMaxFrameSize;
				err = MP4_OK;	
				}
			}	
		else
			{
			framesize = KMaxFrameSize;		
			}
		if (buffersize >= framesize)
	        {
    	    mp4_u32 audiosize, timestamp, returnedframes, timestamp2;
        	if (iMp4HeaderAvail)
	        	{
		  	    err = MP4ParseReadAudioFrames(iMP4Handle, bufPtr, buffersize, &audiosize, &timestamp, &returnedframes, &timestamp2);
	   		    DP4(_L("C3gpDataSourceAdapter::ReadSyncL, MP4ParseReadAudioFrames err[%d] timestamp[%d] size[%d] d0[%x]"), 
	   		    	err, timestamp, audiosize, bufPtr[0]);
				mp4_u32 bytes;	   		    	
	   		    TInt s = MP4ParseGetBufferedBytes(iMP4Handle, &bytes);
			    DP2(_L("C3gpDataSourceAdapter::ReadSyncL bufferedbytes[%x] stat[%d]"),bytes,s);
/*
	    TInt block,aNumber =2;
        RHeap &heap = User::Heap();
        RDebug::Print(_L("### SB-Cont%d"), aNumber);
        RDebug::Print(_L("### SB-Cont%d: Heap Base Address: [0x%x]"),aNumber, heap.Base());
        RDebug::Print(_L("### SB-Cont%d: Available Heap Memory [%6d]"),aNumber, heap.Available(block));
        RDebug::Print(_L("### SB-Cont%d: Largest block: [%6d]"),aNumber, block);
        RDebug::Print(_L("### SB-Cont%d: Size: [%8d]"),aNumber, heap.Size());
        RDebug::Print(_L("### SB-Cont%d: Max length: [%8d]"),aNumber, heap.MaxLength());
*/

        		}
	        else
    	    	{
        		err = MP4_NOT_AVAILABLE;
	   	    	DP0(_L("C3gpDataSourceAdapter::ReadSyncL no read - header not available yet"));
	        	}	
				                            
	        if (err == MP4_OK)
    	        {
/*            if (!((iSourceType == KMmfAudioStreamingSourceUid) || (iSourceType == KMmfAudioProgDLSourceUid) 
                || (iSourceType == KS60AudioStreamingSourceUid) || (iSourceType == KMmfFileSourceUid) || (iSourceType == KMmfDescriptorSourceUid)))   // pay attention to the ! (not condition)
                {
                TUint32 timePositionInMilliSecs = I64LOW(iPlayWindowEndPosition.Int64()/1000);
                    
                if ((iPlayWindowEndPosition != TTimeIntervalMicroSeconds(0)) &&
                    (timestamp >= timePositionInMilliSecs))
                    {
                    aBuffer->SetLastBuffer(ETrue);
                    break;
                    }
                }
*/                
				bufPtr += audiosize;
				buffersize -= audiosize;
				bytesAdded += audiosize;
				}
	        else if ((err == MP4_NO_FRAME) ||
    	   			((err == MP4_NOT_AVAILABLE) && iLastBufferWrittenToMp4Lib))
  			    { // if the file is cut short, it might return not_available - so check for last buffer
   		    	iFillingBuffer->SetLastBuffer(ETrue);
	            break;
   			    }
       		else if (err == MP4_NOT_AVAILABLE)
	  		    { // in case parser is totally out of data
  			    TInt pos = buffer->Data().Length()+bytesAdded;
  			    buffer->Data().SetLength(pos); // must set length first to accomodate position
  		    	iFillingBuffer->SetPosition(pos);
  			    iRestartSyncRead = ETrue;
			    DP3(_L("C3gpDataSourceAdapter::ReadSyncL, MP4_NOT_AVAILABLE bytesAdded[%d] pos[%d] length[%d]"), 
		    		bytesAdded, iFillingBuffer->Position(), buffer->Data().Length());
	            break;
   			    }
    		} // if buffer large enough for read
    	else
    		{ // if buffer too small for read
		    DP3(_L("C3gpDataSourceAdapter::ReadSyncL, buffersize[%d] too small for next frame[%d] err[%d] should be 0"),
		    		buffersize, framesize, err); 
    		break;
    		}
		} // while ETrue keep filling buffer
	if ((err == MP4_OK) || (iFillingBuffer->LastBuffer()))
		{
	    iQueValid = ETrue;
	    buffer->Data().SetLength(buffer->Data().Length() + bytesAdded);
	    DP2(_L("C3gpDataSourceAdapter::ReadSyncL, sending buffer to controller iFillingBuffer[%x], Length[%d]"),
	    		buffer->Data().Ptr(), buffer->Data().Length());
    	iControllerMDataSink->BufferFilledL(iFillingBuffer); // setpos could come in here and clear the queue here and add another read
    	if (iQueValid)
        	{ // this detects if the que has been cleared and refilled
		      // we don't want to do this with the new buffers in the queue
    	    iQueuedAsyncBuffers.Remove(0);
        	if (iQueuedAsyncBuffers.Count() > 0)
	            {
    	        DP0(_L("C3gpDataSourceAdapter::ReadSyncL calls FillBufferL"));
        	    FillBufferL(iQueuedAsyncBuffers[0],this,iMediaId);
	            }
    	    }
		}
    TInt status = TranslateMP4Err(err);
	return status;
    }

void C3gpDataSourceAdapter::ResetVariables()
	{
	iRestartSyncRead = EFalse;
    iLastBufferWrittenToMp4Lib = EFalse;
    iMp4HeaderAvail = EFalse;
	iQueuedAsyncBuffers.Reset();
	iQueValid = EFalse;
	}
	
// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------
//
C3gpDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer(C3gpDataSourceAdapter& a3gpDataSourceAdapter) :
    CActive(EPriorityHigh),
    i3gpDataSourceAdapter(a3gpDataSourceAdapter)
	{
    DP0(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer"));
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------
//
C3gpDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer()
	{
    DP0(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer"));
	}

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::GenerateCallback
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::CAsyncProxyFillBuffer::ReadSync()
	{
    DP0(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::ReadSync"));
    iStatus = KRequestPending; // service request would be made here and pending set by service provider
	SetActive();
	iRequestStatus = &iStatus;
	User::RequestComplete(iRequestStatus, KErrNone);
	}
    
// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel
// -----------------------------------------------------------------------------
//
void  C3gpDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel()
	{
    DP0(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel"));
	}

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunL
// -----------------------------------------------------------------------------
//
void C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunL()
	{
    DP0(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunL"));
    i3gpDataSourceAdapter.ReadSyncL();
	}

// -----------------------------------------------------------------------------
// C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunError
// -----------------------------------------------------------------------------
//
TInt C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunError(TInt aError)
	{
	if (aError)
	    {
	    DP1(_L("C3gpDataSourceAdapter::CAsyncProxyFillBuffer::RunError, aError = %d"), aError);
	    }
	return KErrNone;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// End of File

