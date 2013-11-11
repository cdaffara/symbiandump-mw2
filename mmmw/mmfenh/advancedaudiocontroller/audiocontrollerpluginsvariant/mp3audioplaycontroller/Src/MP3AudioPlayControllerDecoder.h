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
* Description:  This file defines the MP3 Play Controller Decoder class.
*
*/


#ifndef CMP3PLAYCONTROLLERDECODER_H
#define CMP3PLAYCONTROLLERDECODER_H

//  INCLUDES
#include "AdvancedAudioDecoder.h"

// FORWARD DECLARATIONS
class CMMFCodec;
class TAudioFrameInfo;

// CLASS DECLARATION

/**
*  This class defines the MP3 play controller decoder.
*
*  @lib MP3AudioPlayController.lib
*  @since 3.0
*/
class CMP3AudioPlayControllerDecoder : public CAdvancedAudioDecoder
    {

	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMP3AudioPlayControllerDecoder* NewL();

        /**
        * Destructor.
        */
        virtual ~CMP3AudioPlayControllerDecoder();

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

        /**
        * C++ default constructor.
        */
        CMP3AudioPlayControllerDecoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

		void InitCodecConfigs();
		TInt FrameInfo(const TUint8* aBuf,TInt aBufLen,	TAudioFrameInfo& aInfo);
		TInt SeekSync(TUint8* aBuf, TInt aBufLen);
		TInt FrameLength(const TUint8* aBuf, TInt aBufLen, TInt& aFrameLength);

    private: // Data
    
	};


#endif		// CMP3PLAYCONTROLLERDECODER_H

