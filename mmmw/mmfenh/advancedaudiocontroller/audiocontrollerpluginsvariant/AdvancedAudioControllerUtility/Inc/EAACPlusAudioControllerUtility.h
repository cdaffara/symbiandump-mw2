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
* Description:  Class definition for the AAC utility functions.
*
*/


#ifndef CEAACPLUSAUDIOCONTROLLERUTILITY_H
#define CEAACPLUSAUDIOCONTROLLERUTILITY_H

//  INCLUDES
#include "AdvancedAudioUtility.h"
//#include "MmfEnhAacPlusUtil.h"
#include <EAacPlusUtil.h>

// FORWARD DECLARATIONS
class CMMFDataBuffer;
//class CMmfEAacPlusUtil;
class CEAacPlusUtil;

// CLASS DEFINITIONS

/**
*  This class provides AAC utility functions.
*
*  @lib EAACPlusAudioControllerUtility.lib
*  @since 3.0
*/
class CEAACPlusAudioControllerUtility : public CAdvancedAudioUtility
    {
	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CEAACPlusAudioControllerUtility* NewL();

        /**
        * Destructor.
        */
        IMPORT_C virtual ~CEAACPlusAudioControllerUtility();

    public: // New functions

		/**
		* Returns true if header should be included in the first buffer
		* sent to the codec.
		* @param none
		* @return ETrue if header should be prepended, EFalse otherwise
		*/
		IMPORT_C TBool PrependHeader();

    public: // Functions from base classes

        /**
        * From CAdvancedAudioUtility
		* @param aDataBuf
		* @return void
		*/
		void ScanHeaderL(CMMFDataBuffer* aDataBuf);
		
		/**
        * From CAdvancedAudioUtility
		* @param none
		* @return TInt Last scanned frame position
		*/
        TInt LastScannedFramePosition();

		/**
        * From CAdvancedAudioUtility
		* @param aBuf
		* @param aBufLen
		* @return TInt Seek offset
		*/
        TInt SeekSync(CMMFDataBuffer* aBuf, TInt aBufLen);

		/**
        * From CAdvancedAudioUtility
		* Sets the codec config sample rate.
		* @param aCodecConfig codec's configuration data
		* @return void
		*/
		void SetCodecConfigData(RArray<TInt>& aCodecConfig);

	private:

        /**
        * C++ default constructor.
        */
		CEAACPlusAudioControllerUtility();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
		void ConstructL();

        /**
        * Gets the bitrate information
        * @since 3.0
		* @param aBuffer buffer containing the header data
		* @return TInt bitrate
        */
        TInt GetBitrate(CMMFDataBuffer* aBuffer);

        /**
        * Test if the pointer points to a valid AAC frame
        * @since 2.1
        * @param aPtr Pointer to the presumed frame
        * @return Length of the buffer
        */
        TBool IsValidAacFrame(const TUint8* aPtr, TInt aBufLen);

	private:	// Data

		// Format type - ADIF, ADTS, Raw
		CEAacPlusUtil::TEAacPlusFormatType iFormatType;

		// Pointer to AAC utility
//		CMmfEAacPlusUtil* iEAacPlusUtils;
		CEAacPlusUtil* iAacUtil;

		// Pointer to a data buffer
		CMMFDataBuffer* iDataBuffer;
		// AAC profile
		TInt iProfile;
		TInt iOutFrameSize; //AAC Frame Length
		TInt iOutSamplingFrequency;
		TInt iSamplingFrequency;
		TInt iExtObjectType;
		TInt iDownSampledMode;

		// Used for adding frame length
        TInt iSumFrameLength;
        // Number of frames scanned
        TInt iNumFramesScanned;
        // Number of frames to scan
        TInt iNumFramesToScan;
    };

#endif 		// CEAACPLUSAUDIOCONTROLLERUTILITY_H

// End of File
