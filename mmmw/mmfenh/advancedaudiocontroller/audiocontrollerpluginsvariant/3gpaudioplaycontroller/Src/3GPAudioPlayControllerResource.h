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



#ifndef C3GPAUDIOPLAYCONTROLLERRESOURCE_H
#define C3GPAUDIOPLAYCONTROLLERRESOURCE_H

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
*  @lib 3GPAudioPlayControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioPlayControllerResource : public CAdvancedAudioResource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static C3GPAudioPlayControllerResource* NewL( TInt aResourceType );

        /**
        * Destructor.
        */
        virtual ~C3GPAudioPlayControllerResource();

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
        C3GPAudioPlayControllerResource();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL( TInt aResourceType );

    private:	// Data

    	// Resource type
    	TInt iResourceType;

	};

#endif      // C3GPAUDIOPLAYCONTROLLERRESOURCE_H

// End of File
