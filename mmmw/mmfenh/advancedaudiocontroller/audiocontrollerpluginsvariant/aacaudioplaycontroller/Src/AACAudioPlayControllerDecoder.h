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
* Description:  This file defines the AAC Play Controller Decoder class.
*
*/


#ifndef CAACPLAYCONTROLLERDECODER_H
#define CAACPLAYCONTROLLERDECODER_H

//  INCLUDES
#include "AdvancedAudioDecoder.h"
#include <EAacPlusUtil.h>

// FORWARD DECLARATIONS
class CMMFCodec;
class CEAacPlusUtil;

// CLASS DECLARATION
/**
*  This class defines the AAC play controller decoder.
*
*  @lib AACAudioPlayController.lib
*  @since 3.0
*/
class CAACAudioPlayControllerDecoder : public CAdvancedAudioDecoder
    {

	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAACAudioPlayControllerDecoder* NewL();

        /**
        * Destructor.
        */
        virtual ~CAACAudioPlayControllerDecoder();

    public: // Functions from base classes

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
		
	private:
        /**
        * C++ default constructor.
        */
        CAACAudioPlayControllerDecoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

		// these are defined for us in hwdecoder implementation only
		void InitCodecConfigs();
		TInt SeekSync(TUint8* aBuf, TInt aBufLen);
		TInt FrameLength(const TUint8* aBuf,TInt aBufLen,TInt& aFrameLength);
		TInt SeekAdtsSync(TUint8* aBuf, TInt aBufLen);
		TBool IsValidAdtsFrame(const TUint8* aPtr, TInt aBufLen);

    private: // Data
        
        CEAacPlusUtil*  iAacUtil;
        CEAacPlusUtil::TEAacPlusFormatType iFormatType;
		CEAacPlusUtil::TEAacPlusFrameLenConfig iFrameLenConfig;
	};


#endif		// CAACPLAYCONTROLLERDECODER_H

