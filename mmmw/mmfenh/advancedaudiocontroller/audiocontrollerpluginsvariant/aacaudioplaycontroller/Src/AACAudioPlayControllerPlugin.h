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
* Description:  Play controller for AAC.
*
*/


#ifndef AACAUDIOPLAYCONTROLLERPLUGIN_H
#define AACAUDIOPLAYCONTROLLERPLUGIN_H

// INCLUDES
#include "AdvancedAudioPlayController.h"

// FORWARD DECLARATION
class CAACAudioPlayControllerDecoderBuilder;

// CLASS DECLARATION

/**
*  CAACAudioPlayControllerPlugin
*
*  @lib VBRAudioPlayController.lib
*  @since 3.0
*/
class CAACAudioPlayControllerPlugin : public CAdvancedAudioPlayController
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAACAudioPlayControllerPlugin* NewL();

        /**
        * Destructor
        */
        virtual ~CAACAudioPlayControllerPlugin();
        
    private:    // Functions from base classes

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
        
    protected:  // from base class
    
    private:
        /**
        * C++ default constructor.
        */
        CAACAudioPlayControllerPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

    	// Builder for AAC decoder object
    	CAACAudioPlayControllerDecoderBuilder* iDecoderBuilder;

    };

#endif	// AACAUDIOPLAYCONTROLLERPLUGIN_H

// End of File

