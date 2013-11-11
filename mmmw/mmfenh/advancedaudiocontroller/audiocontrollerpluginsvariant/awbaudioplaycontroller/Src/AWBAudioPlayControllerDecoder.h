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
* Description:  This file defines the AWB Play Controller Decoder class.
*
*/



#ifndef CAWBPLAYCONTROLLERDECODER_H
#define CAWBPLAYCONTROLLERDECODER_H

// INCLUDES
#include "AdvancedAudioDecoder.h"

// FORWARD DECLARATIONS
class CMMFCodec;
class TAudioFrameInfo;

// CLASS DECLARATION

/**
*  This class defines the AWB play controller decoder.
*
*  @lib AWBAudioPlayController.lib
*  @since 3.0
*/
class CAWBAudioPlayControllerDecoder : public CAdvancedAudioDecoder
    {

	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAWBAudioPlayControllerDecoder* NewL();

        /**
        * Destructor.
        */
        virtual ~CAWBAudioPlayControllerDecoder();

	public:   // Functions from base classes

        /**
		* From CAdvancedAudioDecoder
	    * Check if the audio converter is hardware accelerated.
	    * @since 3.0
	    * @return ETrue if hardare accelerated, otherwise, EFalse.
	    */
        TBool IsHwAccelerated();
		TCodecProcessResult ProcessL(CMMFBuffer& aSrc, CMMFBuffer& aDst);
		TInt CodecConfig(RArray<TInt>& aCodecConfigData);
		TInt CodecCmd(TCodecCmd aCmd);

    private: // Functions from base classes

		void ResetL();

        /**
        * C++ default constructor.
        */
        CAWBAudioPlayControllerDecoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

		void InitCodecConfigs();
		TInt FrameInfo(const TUint8* aBuf,TInt aBufLen,	TAudioFrameInfo& aInfo);
		TInt SeekSync(TUint8* aBuf, TInt aBufLen);
		TInt FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength);

	private:	// Data

	};


#endif 		// CAWBPLAYCONTROLLERDECODER_H

