/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Play controller for MP3.
*
*/


#ifndef MP3AUDIOPLAYCONTROLLERPLUGIN_H
#define MP3AUDIOPLAYCONTROLLERPLUGIN_H

// INCLUDES
#include "AdvancedAudioPlayController.h"

// FORWARD DECLARATION
class CMP3AudioPlayControllerDecoderBuilder;

// CLASS DECLARATION

/**
*  CMP3AudioPlayControllerPlugin
*
*  @lib AdvancedAudioPlayController.lib
*  @since 3.0
*/
class CMP3AudioPlayControllerPlugin : public CAdvancedAudioPlayController
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMP3AudioPlayControllerPlugin* NewL();

        /**
        * Destructor
        */
        virtual ~CMP3AudioPlayControllerPlugin();

    private: // Functions from base classes

        /**
		* From CAdvancedAudioPlayController
        * Format specific initialization for Data Source
        * @since 3.0
        * @return void
        */
        void DoAddDataSourceL();

        /**
		* From CAdvancedAudioPlayController
        * Format specific initialization for Data Sink
        * @since 3.0
        * @return void
        */
        void DoAddDataSinkL();
        
        /**
		* From CAdvancedAudioPlayController
        * Format specific header scanning
        * @since 3.1
        * @return void
        */
        void DoReadHeaderL(CMMFDataBuffer* aBuffer);
        
        /**
		* From CAdvancedAudioPlayController
        * Instantiates the codec.
        * @since 5.0
        * @return CAdvancedAudioDecoder*
        */
        CAdvancedAudioDecoder* BuildDecoderL();

        
    private:
        /**
        * C++ default constructor.
        */
        CMP3AudioPlayControllerPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
    private: // Data

    	// Builder for MP3 decoder object
    	CMP3AudioPlayControllerDecoderBuilder* iDecoderBuilder;
    };

#endif	// MP3AUDIOPLAYCONTROLLERPLUGIN_H

// End of File

