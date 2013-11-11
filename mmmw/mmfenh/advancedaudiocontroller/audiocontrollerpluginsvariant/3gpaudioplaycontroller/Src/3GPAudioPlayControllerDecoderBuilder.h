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



#ifndef C3GPAUDIOPLAYCONTROLLERDECODERBUILDER_H
#define C3GPAUDIOPLAYCONTROLLERDECODERBUILDER_H

// INCLUDES
#include <e32base.h>
#include <mmfutilities.h>

// CONSTANTS
const TInt K3GPDecoder		= 1;
const TInt K3GPConverter	= 2;

// FORWARD DECLARATIONS
class CAdvancedAudioDecoder;

// CLASS DECLARATION

/**
*  This class implements converter builder.
*
*  @lib 3GPAudioPlayControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioPlayControllerDecoderBuilder : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static C3GPAudioPlayControllerDecoderBuilder* NewL();

        /**
        * Destructor.
        */
        virtual ~C3GPAudioPlayControllerDecoderBuilder();

    public: // New functions

		/**
		* Creates a decoder of specified type.
		* @param none
		* @return pointer to CAdvancedAudioDecoder object
		*/
		CAdvancedAudioDecoder* BuildDecoderL( TInt aType, const TFourCC& aSrcDatatype );

    private:

        /**
        * C++ default constructor.
        */
        C3GPAudioPlayControllerDecoderBuilder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // C3GPAUDIOPLAYCONTROLLERDECODERBUILDER_H

// End of File
