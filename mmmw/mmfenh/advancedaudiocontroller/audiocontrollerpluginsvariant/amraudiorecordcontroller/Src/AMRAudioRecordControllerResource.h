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
* Description:  This class implements resource file handling for AMR RecordControllerPlugin.
*
*/



#ifndef CAMRAUDIORECORDCONTROLLERRESOURCE_H
#define CAMRAUDIORECORDCONTROLLERRESOURCE_H

// INCLUDES
#include "AdvancedAudioResource.h"

// CLASS DECLARATION

/**
*  This class implements resource file handling functions.
*
*  @lib AMRAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class CAMRAudioRecordControllerResource : public CAdvancedAudioResource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAMRAudioRecordControllerResource* NewL();

        /**
        * Destructor.
        */
        virtual ~CAMRAudioRecordControllerResource();

    public: // New functions

		/**
        * Returns the supported bit rates.
        * @since 3.0
        * @param	-
        * @return	Supported bit rates
        */
		const RArray<TUint>& SupportedBitRatesL();

		/**
        * Returns the supported sample rates.
        * @since 3.0
        * @param	-
        * @return	Supported sample rates
        */
		const RArray<TUint>& SupportedSampleRatesL();

    private: // Functions from base classes

		/**
        * From CAdvancedAudioResource
        * Reads the configuration parameters for the AMR codec.
        * @since 3.0
        * @param	-
        * @return	-
        */
		void ReadCodecConfigParametersL();

    private:

        /**
        * C++ default constructor.
        */
        CAMRAudioRecordControllerResource();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

    	// Supported bit rates
		RArray<TUint> iSupportedBitRates;
    	// Supported sample rates
		RArray<TUint> iSupportedSampleRates;

	};

#endif      // CAMRAUDIOPLAYCONTROLLERRESOURCE_H

// End of File
