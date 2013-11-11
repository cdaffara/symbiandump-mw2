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



#ifndef CAMRAUDIOPLAYCONTROLLERDECODERBUILDER_H
#define CAMRAUDIOPLAYCONTROLLERDECODERBUILDER_H

// INCLUDES
#include <e32base.h>

// CONSTANTS
const TInt KAMRDecoder		= 1;
const TInt KAMRConverter	= 2;

// FORWARD DECLARATIONS
class CAdvancedAudioDecoder;

// CLASS DECLARATION

/**
*  This class implements converter builder.
*
*  @lib AACAudioPlayControllerPlugin.lib
*  @since 3.0
*/
class CAMRAudioPlayControllerDecoderBuilder : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAMRAudioPlayControllerDecoderBuilder* NewL();

        /**
        * Destructor.
        */
        virtual ~CAMRAudioPlayControllerDecoderBuilder();

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
        CAMRAudioPlayControllerDecoderBuilder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // CAMRAUDIOPLAYCONTROLLERDECODERBUILDER_H

// End of File
