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
* Description:  This file contains the definition of the AMR record controller
*				 encoder class.
*
*/



#ifndef CAMRRECORDCONTROLLERENCODER_H
#define CAMRRECORDCONTROLLERENCODER_H

// INCLUDES
#include "AdvancedAudioEncoder.h"

// FORWARD DECLARATIONS
class CMMFCodec;

// CLASS DECLARATIONS

/**
*  This class defines the AMR Record controller encoder.
*
*  @lib AMRAudioRecordController.lib
*  @since 3.0
*/
class CAMRAudioRecordControllerEncoder : public CAdvancedAudioEncoder
    {

	public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAMRAudioRecordControllerEncoder* NewL();

        /**
        * Destructor.
        */
        virtual ~CAMRAudioRecordControllerEncoder();

    public: // Functions from base classes

        /**
		* From CAdvancedAudioEncoder
		* Sets audio decoder settings and attributes.
		* @since 3.0
        * @param   aSampleRate 			a sink sampling rate
        * @param   aNumChannel 			required number of channels (mono/stereo indication)
        * @param   aSWConvertSampleRate a sampling rate after conversion
        * @param   aSWConvertChannels   number of channels after conversion
        * @param   aCodecConfigData  	specific codec configuration data
        *
        * @return  void
        */
        void SetConfigL( TUint aSampleRate, TUint aChannels,
						 TUint aSWConvertSampleRate, TUint aSWConvertChannels,
						 const RArray<TInt>& aCodecConfigData );

        /**
		* From CAdvancedAudioEncoder
		* Sets the bit rate.
		* @since 3.0
        * @param   aBitRate 			bit rate
        * @param   aCodecConfigData  	specific codec configuration data
        *
        * @return  void
        */
        void SetBitRateL( TUint aBitRate, const RArray<TInt>& aCodecConfigData );

        /**
		* From CAdvancedAudioEncoder
		* Resets audio encoder settings and attributes.
		* @since 3.0
        * @return void
        */
		void ResetL();

        /**
		* From CAdvancedAudioEncoder
	    * Check if the audio encoder is hardware accelerated.
	    * @since 3.0
	    * @return ETrue if hardare accelerated, otherwise, EFalse.
	    */
        TBool IsHwAccelerated( );

    private: // Functions from base classes

       /**
		* From CAdvancedAudioEncoder
        * Handle emptying of the buffer received in EmptyBuffer().
        * @param   none
        * @return  void
        */
		void HandleEmptyBufferL();

	private:

        /**
        * C++ default constructor.
        */
        CAMRAudioRecordControllerEncoder();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // soft codec pointer if soft codec is used
		CMMFCodec* iSoftCodec;
		// bitrate to be configured codec with
		TUint iBitRate;
	};

#endif 		// CAMRRECORDCONTROLLERENCODER_H

