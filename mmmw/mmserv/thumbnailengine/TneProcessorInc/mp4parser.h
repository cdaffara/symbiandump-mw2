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
* Description:   Header for mp4parser.cpp
*
*/




#ifndef __MP4PARSER_H__
#define __MP4PARSER_H__

//  INCLUDES

#include "Parser.h"
#include <mp4lib.h>

// CONSTANTS
// MACROS
// DATA TYPES
// FUNCTION PROTOTYPES
// FORWARD DECLARATIONS


// CLASS DECLARATION
class CTNEProcessorImpl;
// class CAmrFader;
	
enum TFrameType
{
	EFrameTypeNone = 0,
	EFrameTypeAudio,
	EFrameTypeVideo
};

/**
*  MP4 -format parser class
*  ?other_description_lines
*/
class CMP4Parser : public CParser
{
	
public:  // Constructors and destructor
	
	/**
	* C++ default constructor.
	*/
	CMP4Parser();
	
	/**
	* Two-phased constructor.
	*/

	static CMP4Parser* NewL(CTNEProcessorImpl* aProcessor, RFile& aFileHandle);
	
	/**
	* Destructor.
	*/
	~CMP4Parser();
	
public: 

 	/**
	* Reads information of next available packet in inserted input data
	* @param aType Frame type
    * @param aFrameLength Frame length in bytes
	* @return TInt error code
	*/
    TInt GetNextFrameInformation(TFrameType& aType, TUint& aLength, TBool& aIsAvailable);
	

				
public: // Functions from base classes           
	
	/**
	* From CParser Parser the stream header.
	* @param aStreamParameters Common stream parameters	
	*/
	TInt ParseHeaderL(CParser::TStreamParameters& aStreamParameters);
	
	/**
	* From CParser Resets parser to initial state.
	* @return TInt error code
	*/     
		
	TInt Reset();	
    
    /**
	* From CParser Seek to optimal intra before given time
	*/       

    TInt SeekOptimalIntraFrame(TTimeIntervalMicroSeconds aStartTime, TInt aIndex); 
    
    /**
	* From CParser 
	* Gets the number of frames in current clip
	*/ 
    TInt GetNumberOfVideoFrames();

    /**
	* From CParser 
	* Gets the size of video frame at given index
	*/ 
    TInt GetVideoFrameSize(TInt aIndex);

    /**
	* From CParser 
	* Gets the timestamp of video frame at given index
	*/ 
    TInt GetVideoFrameStartTime(TInt aIndex, TInt* iTimeStamp);

    /**
	* From CParser 
	* Gets the type of video frame at given index
	*/ 
    TInt8 GetVideoFrameType(TInt aIndex);  
          
    /**
	* From CParser Retrieves average video frame rate of current clip
	* @param aBitrate Average frame rate
    *
    * @return error code
	*/
    TInt GetVideoFrameRate(TReal& aFrameRate);    
    
		
    /**
    * Read a number of frames of requested type from inserted data or file
    * @param aDstBuffer Destination buffer
    * @param aType Type of frame(s) to be read        
    * @param aNumRead Number of frames actually read
    * @return TInt error code
    */		
    TInt ReadFrames(TDes8& aDstBuffer, TFrameType aType, TUint32& aNumRead, TUint32& aTimeStamp);
   
private:        
	
   /**
    * By default Symbian OS constructor is private.
	*/
	void ConstructL(CTNEProcessorImpl* aProcessor, RFile& aFileHandle);	
	
private:  // Internal constants
	
	// Stream source type
	enum TStreamSource
	{
		ESourceNone = 0, // not set yet
	    ESourceFile, // reading from a file
		ESourceUser // user of this object provides data
	};        

private:    // Data

    // video processor instance
    CTNEProcessorImpl *iProcessor;
        
	TStreamSource iStreamSource;        
	
	mp4_u32 iVideoType;
    
	TUint iBytesRead;
	
	// RFile handle for 
	RFile iFileHandle;
		
	// The MP4 parser library handle
	MP4Handle iMP4Handle;
	
	TFrameType iNextFrameType;
	TUint iNextFrameLength;
	
	TBool iFirstRead;
	TBool iFirstFrameInfo;

    // Max video frame length & AMR sample size, used for sanity checks to avoid crashes in case of corrupted input
    TInt iMaxVideoFrameLength;

    };
		
#endif      // __MP4PARSER_H__
		
		
// End of File
