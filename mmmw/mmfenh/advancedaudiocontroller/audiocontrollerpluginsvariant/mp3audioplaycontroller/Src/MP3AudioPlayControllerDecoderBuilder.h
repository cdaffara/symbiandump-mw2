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
* Description:  This is a builder class for decoder object.
*
*/



#ifndef CMP3AUDIOPLAYCONTROLLERDECODERBUILDER_H
#define CMP3AUDIOPLAYCONTROLLERDECODERBUILDER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KMP3Decoder		= 1;
const TInt KMP3Converter	= 2;

// FORWARD DECLARATIONS
class CAdvancedAudioDecoder;

// CLASS DECLARATION

/**
*  This class implements converter builder.
*
*  @lib MP3AudioPlayControllerPlugin.lib
*  @since 3.0
*/
class CMP3AudioPlayControllerDecoderBuilder : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMP3AudioPlayControllerDecoderBuilder* NewL();

        /**
        * Destructor.
        */
        virtual ~CMP3AudioPlayControllerDecoderBuilder();

    public: // New functions

		/**
		* Creates a decoder of specified type.
		* @param none
		* @return pointer to CAdvancedAudioDecoder object
		*/
		CAdvancedAudioDecoder* BuildDecoderL( TInt aType );

    private:

        /**
        * C++ default constructor.
        */
        CMP3AudioPlayControllerDecoderBuilder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // CMP3AUDIOPLAYCONTROLLERDECODERBUILDER_H

// End of File
