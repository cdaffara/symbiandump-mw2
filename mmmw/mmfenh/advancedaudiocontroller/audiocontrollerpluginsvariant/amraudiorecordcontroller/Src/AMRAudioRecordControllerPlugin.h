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
* Description:  Record controller for AMR.
*
*/


#ifndef AMRAUDIORECORDCONTROLLERPLUGIN_H
#define AMRAUDIORECORDCONTROLLERPLUGIN_H

// INCLUDES
#include "AdvancedAudioRecordController.h"

// CLASS DECLARATION

/**
*  CAMRAudioRecordControllerPlugin
*
*  @lib VBRAudioRecordController.lib
*  @since 3.0
*/
class CAMRAudioRecordControllerPlugin : public CAdvancedAudioRecordController
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CAMRAudioRecordControllerPlugin* NewL();

        /**
        * Destructor
        */
        virtual ~CAMRAudioRecordControllerPlugin();

    private: // Functions from base classes

        /**
		* From CAdvancedAudioRecordController
        * Format specific initialization for Data Source
        * @since 3.0
        * @return void
        */
        void DoAddDataSourceL();

        /**
		* From CAdvancedAudioRecordController
        * Format specific initialization for Data Sink
        * @since 3.0
        * @return void
        */
        void DoAddDataSinkL();

        /**
        * From CAdvancedAudioPlayController
        * Will leave if controller state is already stopped.
        *
        * @since 3.0
        * @param none
        * @return void
        */
        void StopL();
                
        /**
		* From CAdvancedAudioRecordController
        * Writes the format header to file.
        * @since 3.0
        * @return void
        */
        void WriteHeaderL();

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the bit rate of the data sink in bits/second.
		* @param aRate bit rate, in bits/second.
		* @return void
		*/
		void MacSetSinkBitRateL( TUint aRate );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of bit rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkBitRatesL( RArray<TUint>& aSupportedRates );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the number of channels in the data sink.
		* For example, one channel for mono, two channels for stereo, etc.
		* @param aNumChannels number of channels
		* @return void
		*/
		void MacSetSinkNumChannelsL( TUint aNumChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of sample rates supported by the data sink.
		* @param aSupportedRates supported rates
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkSampleRatesL( RArray<TUint>& aSupportedRates );
		
		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Gets the list of channels supported by the data sink (ie mono, stereo etc).
		* @param aSupportedChannels supported channels
		*        Warning: Existing objects in this array will be removed by this method.
		* @return void
		*/
		void MacGetSupportedSinkNumChannelsL( RArray<TUint>& aSupportedChannels );

		/**
		* From MMMFAudioControllerCustomCommandImplementor
		* Sets the sample rate of the data sink in samples/second.
		* @param aSampleRate The sample rate, in samples/second.
		* @return void
		*/
		void MacSetSinkSampleRateL( TUint aSampleRate );

    private:
        /**
        * C++ default constructor.
        */
        CAMRAudioRecordControllerPlugin();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

    };

#endif	// AMRAUDIORECORDCONTROLLERPLUGIN_H

// End of File

