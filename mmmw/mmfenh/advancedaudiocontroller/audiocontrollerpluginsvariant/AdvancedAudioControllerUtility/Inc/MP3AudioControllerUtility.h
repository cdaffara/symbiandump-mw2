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
* Description:  Class definition for the MP3 utility functions.
*
*/


#ifndef CMP3AUDIOCONTROLLERUTILITY_H
#define CMP3AUDIOCONTROLLERUTILITY_H

//  INCLUDES
#include "AdvancedAudioUtility.h"

// FORWARD DECLARATIONS
class CMMFDataBuffer;

// CONSTANTS
// Number of entries in the TOCTable for VBR MP3
const TUint32 KTOCElements = 100;

// CLASS DEFINITIONS

/**
*  This class provides MP3 utility functions.
*
*  @lib CMP3AudioControllerUtility.lib
*  @since 3.0
*/
class CMP3AudioControllerUtility : public CAdvancedAudioUtility
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CMP3AudioControllerUtility* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CMP3AudioControllerUtility();

    public: // Functions from base classes

		/**
		* From CAdvancedAudioUtility
        * Scans header information
		* @param aDataBuf
		* @return void
		*/
		void ScanHeaderL(CMMFDataBuffer* aDataBuf);

		/**
        * From CAdvancedAudioUtility
		* @param aBuf
		* @param aBufLen
		* @return TInt Seek offset
		*/
        TInt SeekSync(CMMFDataBuffer* aBuf, TInt aBufLen);

		/**
        * From CAdvancedAudioUtility
		* Returns frame info.
		* @param aBuf Pointer to the data
		* @param aBufLen Length of the data
		* @param aInfo Contains the frame info on return
		* @return frame length
		*/
		TInt FrameInfo(const TUint8* aBuf, TInt aBufLen, TAudioFrameInfo& aInfo);

		/**
        * From CAdvancedAudioUtility
		* Sets the codec config sample rate.
		* @param aCodecConfig codec's configuration data
		* @return void
		*/
		void SetCodecConfigData(RArray<TInt>& aCodecConfig);

		/**
		* From CAdvancedAudioUtility
		* Returns frame header size.
		* @param none
		* @return frame header size
		*/
		TInt FrameHeaderSize();
		
        /**
        * From CAdvancedAudioUtility
		* Set/Update the iClipSize varilable
		* @param TUint the new ClipSize
		* @return void
		*/
//		void SetClipSizeL(TUint aSize);
		
	protected:
	
	private:

        /**
        * C++ default constructor.
        */
		CMP3AudioControllerUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

		TBool ParseFirstFrame(const CMMFDataBuffer* aBuf, TInt *aNumFrames);
        TInt64 Duration();
        TInt BitRate();

	private:	// Data

        TBool iFormatIsVBR;
		// Frame positions
		RArray<TInt> iFramesArray;

		// True if the file has TOC
        TBool iTOCExist;
		TInt64 iTotalDuration;
		
		TInt iSamplingRateOut;          // to remember the value for bitrate calculation purpose in VBR situation
		TInt iNumFrames;                // to remember the value for bitrate calculation purpose in VBR situation
		CMMFDataBuffer* iTempBuffer;    // for remembering partial frame during ReadHeader time on Streaming/PDL situation
    };

#endif 		// CMP3AUDIOCONTROLLERUTILITY_H

// End of File
