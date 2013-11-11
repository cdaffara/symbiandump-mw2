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
* Description:  This class implements resource file handling for AAC PlayControllerPlugin.
*
*/



#ifndef CAACAUDIOPLAYCONTROLLERRESOURCE_H
#define CAACAUDIOPLAYCONTROLLERRESOURCE_H

// INCLUDES
#include "AdvancedAudioResource.h"

// CLASS DECLARATION

/**
*  This class implements resource file handling functions.
*
*  @lib AACAudioPlayControllerPlugin.lib
*  @since 3.0
*/
class CAACAudioPlayControllerResource : public CAdvancedAudioResource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAACAudioPlayControllerResource* NewL();

        /**
        * Destructor.
        */
        virtual ~CAACAudioPlayControllerResource();

    private: // Functions from base classes

		/**
        * From CAdvancedAudioResource
        * Reads the configuration parameters for the AAC codec.
        * @since 3.0
        * @param	-
        * @return	-
        */
		void ReadCodecConfigParametersL();

    private:

        /**
        * C++ default constructor.
        */
        CAACAudioPlayControllerResource();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // CAACAUDIOPLAYCONTROLLERRESOURCE_H

// End of File
