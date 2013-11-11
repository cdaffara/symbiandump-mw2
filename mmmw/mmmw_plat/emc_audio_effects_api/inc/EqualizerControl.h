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
* Description:  Header of EqualizerControl interface.
*
*/


#ifndef EQUALIZERCONTROL_H
#define EQUALIZERCONTROL_H

#include <AudioEffectControl.h>

namespace multimedia
    {
    
    // Uid identifying Equalizer Control
    const TUid KEqualizerEffectControl = {0x10207BDD};
    
    class MEqualizerControl : public MAudioEffectControl
        {
		public:
			static const TUint KBand1Changed = KSpecificEffectBase + 1;
			static const TUint KBand2Changed = KSpecificEffectBase + 2;
			static const TUint KBand3Changed = KSpecificEffectBase + 3;
			static const TUint KBand4Changed = KSpecificEffectBase + 4;
			static const TUint KBand5Changed = KSpecificEffectBase + 5;
			static const TUint KBand6Changed = KSpecificEffectBase + 6;
			static const TUint KBand7Changed = KSpecificEffectBase + 7;
			static const TUint KBand8Changed = KSpecificEffectBase + 8;
			static const TUint KBand9Changed = KSpecificEffectBase + 9;
			static const TUint KBand10Changed = KSpecificEffectBase + 10;
			static const TUint KBand11Changed = KSpecificEffectBase + 11;
			static const TUint KBand12Changed = KSpecificEffectBase + 12;		

        public:
			/**
	        * Get the band level in mB for the specified band
	        * @since 3.0
	        * @param aBand Frequency Band
	        * @return Returns the band level in mB for the specified band
	        */
			virtual TInt BandLevel( TInt& aBand, TInt& aBandLevel ) = 0;
	
			/**
	        * Get the band width in Hz for the specified band.
	        * @since 3.0
	        * @param aBand Frequency Band
	        * @return The band width in Hz for the specified band
	        */
			virtual TInt BandWidth( TInt& aBand, TInt& aBandWidth ) = 0;
	
			/**
	        * Get the center frequency in Hz for a given band
	        * @since 3.0
	        * @param aBand Frequency Band
	        * @return The center frequency in Hz for a given band.
	        */
			virtual TInt CenterFrequency( TInt& aBand, TInt& aCenterFrequency ) = 0;
	
			/**
	        * Get the cross-over frequency between the given frequency band (aBand) and the next band
	        * @since 3.0
	        * @param aBand Frequency Band
	        * @return Crossover frequency.
	        */
			virtual TInt CrossoverFrequency( TInt& aBand, TInt& aCrossoverFreq ) = 0;
	
			/**
	        * Get the dB range in mB for the equalizer
	        * @since 3.0
	        * @param aMin Minimum level in dB
	        * @param aMin Maximum level in dB
	        */
			virtual TInt DbLevelLimits( TInt& aMin, TInt& aMax ) = 0;
	
			/**
	        * Get number of equalizer bands.
	        * @since 3.0
	        * @param -
	        * @return The number of equalizer bands.
	        */
			virtual TInt NumberOfBands(TInt& aNumberofBands) = 0;
	
			/**
	        * Sets the equalizer band level value in mB, ranging from Min to Max
	        * @since 3.0
	        * @param aBand Frequency Band
	        * @param aLevel band level in dB, ranges from DbLevelLimits()
	        * @return -
	        */
			virtual TInt SetBandLevel( TInt& aBand, TInt& aLevel ) = 0;
        };
    } // namespace multimedia

#endif // EQUALIZERCONTROL_H

// End of file
