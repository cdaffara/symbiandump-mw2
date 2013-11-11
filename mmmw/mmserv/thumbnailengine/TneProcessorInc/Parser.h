/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Definition of an abstract base class for file format parsers
*
*/




#ifndef __PARSER_H__
#define __PARSER_H__

//  INCLUDES
#ifndef __E32BASE_H__
#include <e32base.h>
#endif

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS
// CLASS DECLARATION

/**
*  Base class for file format parser
*  ?other_description_lines
*/
class CParser : public CBase
{
	
public: // Constants
	
	enum TErrorCode 
	{
		EInternalAssertionFailure   = -2200,
			EParserNotEnoughData = -2201,
			EParserEndOfStream = -2202,
			EParserBufferTooSmall = -2203,
			EParserUnsupportedFormat = -2204,
			EParserStreamCorrupted = -2205,
			EParserFailure = -2206
	};

	// file format
	enum TFileFormat {
		EFileFormatUnrecognized = 0,
	    EFileFormat3GP,
		EFileFormatMP4
	};
	
	// video format
	enum TVideoFormat {
		EVideoFormatNone = 0,
		EVideoFormatH263Profile0Level10,
		EVideoFormatH263Profile0Level45,        
        EVideoFormatMPEG4,
        EVideoFormatAVCProfileBaseline,
        EVideoFormatAVCProfileMain,
        EVideoFormatAVCProfileExtended
	};
	
	
#ifdef __VIDEOEDIT__
		enum TProcessingMode
		{
			EParseOnly = 1,
				EParseAndDecode,
				EDecodeOnly
		};
#endif
		
public: // Data structures
	
	// common stream parameters
	struct TStreamParameters 
	{
		TBool iHaveVideo;  // is there video in the stream ?
		TFileFormat iFileFormat;  // file format
		TVideoFormat iVideoFormat;  // video format
		    
		TUint iVideoWidth;  // width of a video frame
		TUint iVideoHeight;   // height of a video frame
		TInt64 iVideoPicturePeriodNsec;  // one PCF tick period in nanoseconds
		TUint iVideoIntraFrequency;  // intra frame frequency in stream
		
		TUint iStreamLength;  // stream length in milliseconds
		TUint iVideoLength;
		
		TBool iCanSeek;   // TRUE if seeking in file is possible
		
		TUint iStreamSize;  // stream size in bytes
		TUint iStreamBitrate;  // stream average bitrate            
		
		TUint iMaxPacketSize; // The maximum media packet size
		TUint iLogicalChannelNumberVideo; // Logical channel number for video data
		TUint iReferencePicturesNeeded; // Number of reference pictures
		// the video decoder needs to store.
		TUint iNumScalabilityLayers; // The number of different scalability layers used
		
		TReal iFrameRate;
		TUint iVideoTimeScale; 
		
	};
		
	public: // New functions
				
		/**
		* Parses the stream header.
		* @param aStreamParameters Stream parameters        
		*/	
        
        virtual TInt ParseHeaderL(TStreamParameters& aStreamParameters) = 0;
				
        /**
        * Resets the parser to initial state.
        * => If input stream is in a file, rewinds it.
        * => If input is written to parser, all buffered bytes are flushed
        * @param aStreamParameters Stream parameters        
        */
        virtual TInt Reset() = 0; 				        

        /**
        * Gets the number of frames in the current clip
        *
        * @return Number of frames
        */
        inline TInt GetNumberOfFramesInClip() { return iNumberOfFrames; }
        
        virtual	TInt GetNumberOfVideoFrames() = 0; 

  //      virtual TInt GetNumberOfFrames() = 0;

        /**
        * Seeks to intra frame before given time
        *
        * @param aStartTime Time in microseconds to seek
        * @param aIndex Index of the frame at aStartTime, if known. 0 otherwise
        *
        * @return Number of frames
        */
		virtual	TInt SeekOptimalIntraFrame(TTimeIntervalMicroSeconds aStartTime, TInt aIndex) = 0; 

        /**
        * Gets the size of video frame at given index
        *
        * @param aIndex Index of the frame
        *
        * @return Size of frame in bytes
        */
        virtual	TInt GetVideoFrameSize(TInt aIndex) = 0;

        /**
        * Gets the timestamp of video frame at given index
        *
        * @param aIndex Index of the frame
        * @param aTimeStamp Output: timestamp in ticks
        *
        * @return Timestamp in milliseconds
        */
        virtual	TInt GetVideoFrameStartTime(TInt aIndex, TInt* aTimeStampInTicks) = 0;

        /**
        * Gets the type of video frame at given index
        *
        * @param aIndex Index of the frame
        *
        * @return Frame type: 1 = INTRA, 0 = INTER
        */
        virtual	TInt8 GetVideoFrameType(TInt aIndex) = 0;			        
        
        /**
        * Gets the start frame index of the current clip                
        *
        * @return Frame index
        */
        inline TInt GetStartFrameIndex() { return iStartFrameIndex; }


    public:  // Data

        // stream parameters
        TStreamParameters iStreamParameters;
        // ETrue if parsing this clip for the first time
        TBool iFirstTimeClipParsing;
         // number of frames in output clip
        TInt iOutputNumberOfFrames; 

    protected:  // Data

        // current frame number
        TInt iFrameNumber;  
        // number of frames in current clip
        TInt iNumberOfFrames; 
        // index of the first included frame
        TInt iStartFrameIndex;
		
    };
		
#endif      // __PARSER_H__   
		
// End of File
