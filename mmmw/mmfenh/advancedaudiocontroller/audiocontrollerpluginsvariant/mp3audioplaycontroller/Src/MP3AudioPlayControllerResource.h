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
* Description:  This class implements resource file handling for MP3 PlayControllerPlugin.
*
*/



#ifndef CMP3AUDIOPLAYCONTROLLERRESOURCE_H
#define CMP3AUDIOPLAYCONTROLLERRESOURCE_H

// INCLUDES
#include "AdvancedAudioResource.h"

// CLASS DECLARATION

/**
*  This class implements resource file handling functions.
*
*  @lib MP3AudioPlayControllerPlugin.lib
*  @since 3.0
*/
class CMP3AudioPlayControllerResource : public CAdvancedAudioResource
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMP3AudioPlayControllerResource* NewL();

        /**
        * Destructor.
        */
        virtual ~CMP3AudioPlayControllerResource();

    private: // Functions from base classes

		/**
        * From CAdvancedAudioResource
        * Reads the configuration parameters for the MP3 codec.
        * @since 3.0
        * @param	-
        * @return	-
        */
		void ReadCodecConfigParametersL();

    private:

        /**
        * C++ default constructor.
        */
        CMP3AudioPlayControllerResource();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:	// Data

	};

#endif      // CMP3AUDIOPLAYCONTROLLERRESOURCE_H

// End of File
