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



#ifndef CAACAUDIOPLAYCONTROLLERDECODERBUILDER_H
#define CAACAUDIOPLAYCONTROLLERDECODERBUILDER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KAACDecoder		= 1;
const TInt KAACConverter	= 2;

// FORWARD DECLARATIONS
class CAdvancedAudioDecoder;

// CLASS DECLARATION

/**
*  This class implements converter builder.
*
*  @lib AACAudioPlayControllerPlugin.lib
*  @since 3.0
*/
class CAACAudioPlayControllerDecoderBuilder : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAACAudioPlayControllerDecoderBuilder* NewL();

        /**
        * Destructor.
        */
        virtual ~CAACAudioPlayControllerDecoderBuilder();

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
        CAACAudioPlayControllerDecoderBuilder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // CAACAUDIOPLAYCONTROLLERDECODERBUILDER_H

// End of File
