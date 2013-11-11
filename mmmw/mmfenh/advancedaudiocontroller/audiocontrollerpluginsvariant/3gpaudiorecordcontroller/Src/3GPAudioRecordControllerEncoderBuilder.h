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
* Description:  This is a builder class for encoder object.
*
*/



#ifndef C3GPAUDIORECORDCONTROLLERENCODERBUILDER_H
#define C3GPAUDIORECORDCONTROLLERENCODERBUILDER_H

// INCLUDES
#include <e32base.h>
#include <mmfutilities.h>

// CONSTANTS
const TInt K3GPEncoder		= 1;
const TInt K3GPConverter	= 2;

// FORWARD DECLARATIONS
class CAdvancedAudioEncoder;

// CLASS DECLARATION

/**
*  This class implements converter builder.
*
*  @lib 3GPAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioRecordControllerEncoderBuilder : public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static C3GPAudioRecordControllerEncoderBuilder* NewL();

        /**
        * Destructor.
        */
        virtual ~C3GPAudioRecordControllerEncoderBuilder();

    public: // New functions

		/**
		* Creates an encoder of specified type.
		* @param aType
		* @param aSinkDatatype FourCC code of sink.
		* @return pointer to CAdvancedAudioEncoder object
		*/
		CAdvancedAudioEncoder* BuildEncoderL( TInt aType, const TFourCC& aSinkDatatype );

    private:

        /**
        * C++ default constructor.
        */
        C3GPAudioRecordControllerEncoderBuilder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // C3GPAUDIORECORDCONTROLLERENCODERBUILDER_H

// End of File
