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
* Description:  This class provides utility functions such as parsing of header
*				 information for VBR type clips.
*
*/


#ifndef CADVANCEDAUDIOUTILITY_H
#define CADVANCEDAUDIOUTILITY_H

//  INCLUDES
#include "AdvancedAudioHelper.h"        // for MAdvancedAudioDecoderUtilityObserver
#include <e32base.h>
#include <mmfdatabuffer.h>
#include <f32file.h>

// FORWARD DECARATION
class CAdvancedAudioDecoder;
class CMMFClip;
// CLASS DECLARATION

/**
*  The TTimePos class is used to record the byte position and the corresponding
*  time position in an audio clip and the length of the frame which starts
*  from the byte position.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class TTimePos
    {
	public:
		TTimePos() : iTimePos(0), iBytePos(0), iFrameLen(0)
			{}
		TTimePos(TInt64 aTimePos, TInt aBytePos, TInt aFrameLen)
			: iTimePos(aTimePos), iBytePos(aBytePos), iFrameLen((TInt16)aFrameLen)
			{}
	public:
		TInt64 iTimePos;    // time position (microseconds)
		TInt iBytePos;     // byte position
		TInt16 iFrameLen;  // frame length
    };

/**
*  Frame info struct.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class TAudioFrameInfo
    {
	public:
		TInt iMode;            // encoding mode
		TInt iBitRate;         // bitrate (bit/s)
		TInt iSamplingRate;    // sampling frequency (Hz)
		TInt iChannels;        // number of channels
		TInt iFrameSize;       // encoded size (bytes)
		TInt iFrameSamples;    // samples per frame
		TInt iSamplingRateOut; // sampling frequency after conversion (Hz)
		TInt iChannelsOut;     // number of audio channels after conversion (1 or 2)
		TInt iFrameSamplesOut; // decoded size after conversion (samples per frame)
		TInt iPadding;         // padding flag (TRUE or FALSE, TRUE if p slot exists)
		TInt iId;              // id of algorithm (1 MPEG-1, 0 MPEG-2)
    };
    
class MAdvancedAudioUtilityObserver
    {
    public:

        /**
        * Callback function from CAdvancedAudioUtility when bitrate changed situation occurs
        * @param   aFilledBuffer  data buffer filled with converted data
        * @return  void
        */
        virtual void BitRateChanged() = 0;
        virtual void SeekPositionReached(TUint aTimeMs) = 0;
		virtual void PlayWindowEndPositionReached() = 0;
    };

/**
*  Utility class for Advanced Audio Controller Plugins.
*
*  @lib AdvancedAudioController.lib
*  @since 3.0
*/
class CAdvancedAudioUtility : public CBase,
                              public MAdvancedAudioDecoderUtilityObserver
    {
	public:  // Constructors and destructor

		/**
		* Destructor.
		*/
		IMPORT_C virtual ~CAdvancedAudioUtility();

        /**
        * C++ default constructor.
        */
		IMPORT_C CAdvancedAudioUtility();

    public: // New functions

		/**
		* Scans source buffer, frame by frame to update the frame count,
		* byte count and duration.
		* @param aClip the source clip, must be primed before calling this function
		* @param aBuf buffer containing the header data
		* @param aScanThisBufferOnly indication to scan only the contents in aBuf
		* @return TInt bytes scannes
		*/
		IMPORT_C virtual TInt ScanFramesL(CMMFClip* aClip, CMMFDataBuffer* aBuf,
			TBool aScanThisBufferOnly = EFalse, TInt aSourceReadPosition = 0);

		/**
        * Scans header information
		* @param aDataBuf
		* @return void
		*/
		IMPORT_C virtual void ScanHeaderL(CMMFDataBuffer* aDataBuf);

		/**
		* Scans source buffer, frame by frame to update the frame count,
		* byte count and duration.
		* @param aBuf buffer containing the header data
		* @param aScanThisBufferOnly indication to scan only the contents in aBuf
		* @return TInt bytes scannes
		*/
		IMPORT_C virtual TInt ScanFramesL(CMMFDataBuffer* aBuf,
			TBool aScanThisBufferOnly = EFalse, TInt aSourceReadPosition = 0);

		/**
		* Finds the byte position of the frame at given time position.
		* NOTE: This routine works accurately for time positions smaller than
		* 2^32 microseconds (~71min 34seconds) only, since the time stamps
		* use 32 bit unsigned integers. If the given time position is greater
		* than this, then this routine returns the position of the frame at
		* 2^32 microseconds.
		* @param aTimePos time position to find (microseconds)
		* @param aClip pointer to data clip
		* @return position of the frame header; KErrNotFound if not found
		*/
		IMPORT_C virtual TInt FindFramePos(const TInt64& aTimePos, CMMFClip* aClip = NULL);

		/**
		* Returns the byte position of the first frame.
		* @param aBuf Pointer to the data
		* @param aBufLen Length of the data
		* @return byte position of the first frame
		*/
		IMPORT_C virtual TInt SeekSync(CMMFDataBuffer* aBuf, TInt aBufLen) = 0;

		/**
		* Returns frame info.
		* @param aBuf Pointer to the data
		* @param aBufLen Length of the data
		* @param aInfo Contains the frame info on return
		* @return frame length
		*/
		IMPORT_C virtual TInt FrameInfo(const TUint8* aBuf, TInt aBufLen, TAudioFrameInfo& aInfo);

		/**
		* Returns the sample rate.
		* @param none
		* @return sample rate
		*/
		IMPORT_C TInt SampleRate();

		/**
		* Returns the bit rate in bits/sec.
		* @param none
		* @return bit rate
		*/
		IMPORT_C virtual TInt BitRate();

		/**
		* Returns the number of channels.
		* @param none
		* @return number of channels
		*/
		IMPORT_C TInt Channels();

		/**
		* Returns the number of channels out of decoder.
		* @param none
		* @return number of channels
		*/
		IMPORT_C TInt ChannelsOut();

		/**
		* Returns the scanned duration.
		* @param none
		* @return scanned duration
		*/
		IMPORT_C TInt64 ScanDuration();

		/**
		* Returns the duration.
		* @param none
		* @return duration
		*/
		IMPORT_C virtual TInt64 Duration();

		/**
		* Returns the header offset.
		* @param none
		* @return header offset
		*/
		IMPORT_C TInt HeaderOffset();

		/**
		* Returns the Sync offset e.g. ID3 header length
		*
		* @return  TInt
		*/
		IMPORT_C TInt SyncOffset();
		
		/**
		* Sets the codec config sample rate.
		* @param aCodecConfig codec's configuration data
		* @return void
		*/
		IMPORT_C virtual void SetCodecConfigData(RArray<TInt>& aCodecConfig);
		
        /**
		* Set the Observer for Callback
		* @param CAdvancedAudioPlayController& reference to MAdvancedAudioUtilityObserver object for callback
		* @return void
		*/
		IMPORT_C void SetObserver(MAdvancedAudioUtilityObserver& aObserver);
		
		/**
		* Set the Decoder Object
        * @param none
		* @return void
		*/
		IMPORT_C void SetDecoder(CAdvancedAudioDecoder& aDecoder);
		
		/**
		* De-reference decoder object from SetDecoderL, should always be called for safety before CAdvancedAudioOutput is deleted
        * @param none
		* @return void
		*/
		IMPORT_C void DeReferenceDecoder();

		/**
		* Returns frame header size.
		* @param none
		* @return frame header size
		*/
		IMPORT_C virtual TInt FrameHeaderSize();

        /**
        * Gets the drive number from drivename
        * @since 3.0
        * @param aDriveName
        * @return TDriveNumber
        */
        IMPORT_C TDriveNumber GetDriveNumber(const TDesC& aDriveName);

        /**
		* Set/Update the iClipSize varilable
		* @param TUint the new ClipSize
		* @return void
		*/
		IMPORT_C virtual void SetClipSizeL(TUint aSize);
		
        /**
		* Enable bitrate change callback, would trigger a BitRateChanged callback when bitrate changes occurs
		* @param TInt interval in Milliseconds of callback to happen, 0 means callback on any changes, -1 means no callback
		* @return void
		*/
		IMPORT_C virtual void EnableBitRateChangedEventL(TInt aIntervalInMilliSeconds = -1);
		
        /**
        * Sets the Source Position based on Time
        * @since 3.0
        * @param aTime
        * @param aPos
        * @return TDriveNumber
        */
        IMPORT_C virtual TInt SetSourceReference(TUint aTimeMs, TUint aPos);
        
        /**
        * Find the closest Frame's position from a given time (millisec) in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time to search from in millisec
        * @param aPos       Bytes Position found in frametable if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt FindFramePosFromTime(TUint& aTimeMs, TUint& aPos);
		
		IMPORT_C virtual TInt FindFrameTimeFromPos(TUint& aTimeMs, TUint& aPos);
		
		/**
        * Find the last record Frame's position in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aPos       Bytes Position found in frametable if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt LastFramePos(TUint& aPos);
		
		/*
        * Find the last Frame's position relative time (millisec) in frametable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time of the last Frame in millisec if return is KErrNone
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt LastFrameTime(TUint& aTimeMs);
		
		/*
        * Reset the frame table
        * added to support AAC optimization
        * @since 3.2
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt ResetTable();
		
		/**
        * Cause FrameTable to generate an event when the desire Position is recorded in FrameTable
        * added to support AAC optimization
        * @since 3.2
        * @param aTimeMs    Time to be Seek to when callback occurs in millisec
        * return TInt       standard system error
        */
		IMPORT_C virtual TInt SeekToTimeMs(TUint aTimeMs);
		IMPORT_C virtual TInt SetPlayWindowEndTimeMs(TUint aTimeMs);

	protected:      // new functions

		/**
		* Records the timestamp into iPosArr.
		* @param aFrameLength frame length of the next frame in clip
		* @return void
		*/
		IMPORT_C virtual void RecordTimeStampL(TInt aFrameLength);

		/**
		* Finds the length of the ID3 header
		* @param aBuf
		* @return TInt
		*/
        IMPORT_C TInt ID3HeaderLength(const CMMFDataBuffer* aBuf);

		/**
		* Creates a buffer
		* @param aSize
		* @return Pointer to the buffer
		*/
        IMPORT_C CMMFDataBuffer* CreateBufferOfSizeLC(TUint aSize);
        
    public:         // from MAdvancedAudioDecoderUtilityObserver
		IMPORT_C virtual void DecodeIntervalEvent();
        IMPORT_C virtual void SeekPositionReached(TUint aTimeMs);
		IMPORT_C virtual TInt GetOffsets(TInt& aOffsets);
		IMPORT_C virtual void PlayWindowEndPositionReached();

	protected:      // Data

		// Frame byte position, time position, and length
		CArrayFixSeg<TTimePos>* iPosArr;
		// Sampling rate found in header
		TInt iSamplingRate;
		// Bit rate (bps) found in header
		TInt iBitRate;
		// Number of channels found in header
		TInt iChannels;
		// Number of channels after decoding
		TInt iChannelsOut;
		// Samples per frame
		TInt iSamplesPerFrame;
		// Scanned Duration of the clip
		TInt64 iScanDuration;
		// Duration of the clip
		TInt64 iDurationUs;
		// Bytes scanned
		TInt iScanByteCount;
		// Bytes since last frame
		TInt iScanInvByteCount;
		// Saved length
		TInt iSavedLength;
		
		// clip size
		TUint iClipSize;
		
		// Header offset position
		TInt iHeaderOffset;
		// Header 2 offset position
        TInt iSyncOffset;
        // Indicates the header is read - is to be used to process buffers for calculating bitrate without corrupting the offset calculations
        TBool iHdrIsRead;
		// For Average Bitrate Calculation
		TInt 	iNumFrames;
		TInt64 	iSumBitRate;
		
		// Length of MetaData found in file
		TInt iLenMetaData;
		
		MAdvancedAudioUtilityObserver* iObserver;   // new for Streaming/PDL, call back to controller when average bitrate changes
		TInt iFrameTimeMs;                          // remember how many millisec per frame contains
		TUint iReferenceBitRate;                    // new for Streaming/PDL, remember the last return/callback bitrate
        TBool iScannedOnce;                         // new for Streaming/PDL, remember whether the first frame has already been scanned
        
        CAdvancedAudioDecoder* iDecoder;            // pointer to decoder object own the play controller, does not have ownership
        TBool iBitRateFrozen;
        TInt iTotalFrameBytes;
        TInt iTotalFrames;
    };

#endif 		// CADVANCEDAUDIOUTILITY_H
