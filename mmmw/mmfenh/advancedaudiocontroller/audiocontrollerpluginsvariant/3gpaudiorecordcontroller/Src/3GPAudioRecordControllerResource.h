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
* Description:  This class provides the main API for the specific codec resources.
*
*/



#ifndef C3GPAUDIORECORDCONTROLLERRESOURCE_H
#define C3GPAUDIORECORDCONTROLLERRESOURCE_H

// INCLUDES
#include "AdvancedAudioResource.h"

// CONSTANTS
const TInt K3GPResourceAAC 		= 1;
const TInt K3GPResourceAMR 		= 2;
const TInt K3GPResourceAWB		= 3;

// CLASS DECLARATION

/**
*  This class implements base class for all resource file handling classes.
*
*  @lib 3GPAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioRecordControllerResource : public CAdvancedAudioResource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static C3GPAudioRecordControllerResource* NewL( TInt aResourceType );

        /**
        * Destructor.
        */
        virtual ~C3GPAudioRecordControllerResource();

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
        * Reads the configuration parameters for the 3GP codec.
        * @since 3.0
        * @param	-
        * @return	-
        */
		void ReadCodecConfigParametersL();

    private:

        /**
        * C++ default constructor.
        */
        C3GPAudioRecordControllerResource();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TInt aResourceType );

    private:	// Data

    	// Resource type
    	TInt iResourceType;
    	// Supported bit rates
		RArray<TUint> iSupportedBitRates;
    	// Supported sample rates
		RArray<TUint> iSupportedSampleRates;
	};

#endif      // C3GPAUDIORECORDCONTROLLERRESOURCE_H

// End of File
