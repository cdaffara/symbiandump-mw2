/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class maintains all the current radio settings. It's also
*				 responsible for sending spontaneous events using Publish & Subscribe.
*
*/



#ifndef RADIOSERVERSETTINGS_H
#define RADIOSERVERSETTINGS_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>
#include <RadioRdsControl.h>		// Adaptation
#include "RadioServerData.h"

// CLASS DECLARATION

/**
*  Maintains current radio settings and generates spontaneous events.
*
*  @lib RadioServer.exe
*  @since S60 3.0
*/
class CRadioServerSettings : public CBase
    {
public:  // Constructors and destructor

	/**
	 * Two-phased constructor.
	 */
	static CRadioServerSettings* NewL();

	/**
	 * Destructor.
	 */
	virtual ~CRadioServerSettings();

public: // New functions

	/**
	 * Sets the radio monitor status.
	 * @since S60 3.2
	 * @param aMonitorOn Monitor status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetRadioMonitorStatus( TBool aMonitorOn, TBool aPublish=ETrue );

	/**
	 * Sets the radio state to ON.
	 * @since Series 60 3.0
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetRadioOn(TBool aPublish=ETrue);

	/**
	 * Sets the radio state to OFF.
	 * @since Series 60 3.0
	 * @param aError Error code for setting radio off
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetRadioOff( TInt aError, TBool aPublish=ETrue );

	/**
	 * Returns the radio state.
	 * @since S60 3.0
	 * @return ETrue if Radio is ON
	 */
	TBool IsRadioOn() const;

	/**
	 * Store the current antenna status.
	 * @since S60 3.0
	 * @param aAttached Antenna status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetAntennaStatus( TBool aAttached, TBool aPublish=ETrue );

	/**
	 * Returns the antenna status.
	 * @since S60 3.0
	 * @return ETrue if antenna is attached
	 */
	TBool IsAntennaAttached() const;

	/**
	 * Store the current offline mode status.
	 * @since S60 3.0
	 * @param aOfflineMode Offline mode status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetOfflineModeStatus( TBool aOfflineMode, TBool aPublish=ETrue );

	/**
	 * Returns the device's offline mode status.
	 * @since S60 3.0
	 * @return ETrue if device is in offline mode
	 */
	TBool IsOfflineModeEnabled() const;

	/**
	 * Store the current trasmitter status.
	 * @since S60 3.0
	 * @param aActive FM transmitter status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetTransmitterStatus( TBool aActive, TBool aPublish=ETrue );

	/**
	 * Returns the transmitter status.
	 * @since S60 3.0
	 * @return ETrue if transmitter is active
	 */
	TBool IsTransmitterActive() const;

	/**
	 * Store the current frequency.
	 * @since S60 3.0
	 * @param aFrequency Frequency in Hz
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetFrequency( TInt aFrequency, TBool aPublish=ETrue );

	/**
	 * Returns the current frequency.
	 * @since S60 3.0
	 * @return Current frequency in Hz
	 */
	TInt Frequency() const;

	/**
	 * Store the current frequency range.
	 * @since S60 3.2
	 * @param aRange Frequency range
	 * @param aMinFreq Minimum frequency for the current frequency range
	 * @param aMaxFreq Maximum frequency for the current frequency range
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetFrequencyRange( TRsFrequencyRange aRange, TInt aMinFreq, TInt aMaxFreq, TBool aPublish=ETrue );

	/**
	 * Returns the current frequency range.
	 * @since S60 3.2
	 * @return Current frequency range
	 */
	TRsFrequencyRange FrequencyRange() const;

	/**
     * Gets the current frequency range.
     * @since S60 3.2
     * @param aRange On return contains the current frequency range.
     * @param aMinFreq On return contains the minimum frequency for the current frequency range.
     * @param aMaxFreq On return contains the maximum frequency for the current frequency range.
     * @return A standard system error code
	 */
	TInt GetFrequencyRange( TRsFrequencyRange& aRange, TInt& aMinFreq, TInt& aMaxFreq ) const;

	/**
	 * Store the current forced mono status.
	 * @since S60 3.0
	 * @param aForceMono Forced mode status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetForcedMonoStatus( TBool aForceMono, TBool aPublish=ETrue );

	/**
	 * Returns the current force mono status.
	 * @since S60 3.0
	 * @return Current force mono status
	 */
	TBool IsForceMonoEnabled() const;

	/**
	 * Store the current squelch status.
	 * @since S60 3.0
	 * @param aSquelch Squelch status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetSquelch( TBool aEnabled, TBool aPublish=ETrue );

	/**
	 * Returns the current squelch status.
	 * @since S60 3.0
	 * @return Current squelch status
	 */
	TBool IsSquelchEnabled() const;

	/**
	 * Store the current volume.
	 * @since S60 3.0
	 * @param aVolume Volume
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetVolume( TInt aVolume, TBool aPublish=ETrue );

	/**
	 * Returns the current volume.
	 * @since S60 3.0
	 * @return Current volume
	 */
	TInt Volume() const;

	/**
	 * Store the current mute status.
	 * @since S60 3.0
	 * @param aMute Mute status
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetMuteStatus( TBool aMute, TBool aPublish=ETrue );

	/**
	 * Returns the mute status.
	 * @since S60 3.0
	 * @return ETrue if muted
	 */
	TBool IsMute() const;

	/**
	 * Store the current audio balance.
     * @since S60 3.0
     * @param aLeft Left speaker volume percentage. This value ranges from 0 to 100.
     * @param aRight Right speaker volume percentage. This value ranges from 0 to 100.
	 * @param aPublish ETrue to publish the status to clients
	 */
	void SetBalance( TInt aLeft, TInt aRight, TBool aPublish=ETrue );

	/**
	 * Returns the current balance.
	 * @since S60 3.0
	 * @param aLeft On return contains the left balance
	 * @param aRight On return contains the right balance
	 */
	void GetBalance( TInt& aLeft, TInt& aRight ) ;

	/**
	 * Store the current RDS signal status.
	 * @since S60 3.2
	 * @param aSignal Signal status
	 */
	void SetRdsSignalChange( TBool aSignal );

	/**
	 * Store the current auto switch (AF) status.
	 * @since S60 3.2
	 * @param aAuto Auto switch status
	 */
	void SetAutoSwitchStatus( TBool aAuto );

	/**
	 * Returns the current auto switch (AF) status.
	 * @since S60 3.2
	 * @return Current volume
	 */
	TBool AutoSwitch() const;

	/**
	 * Store the current Programme Identification code.
	 * @since S60 3.2
	 * @param aPi Programme Identification code
	 */
	void SetProgrammeIdentification( TInt aPi );

	/**
	 * Store the current Programme Type.
	 * @since S60 3.2
	 * @param aPty Programme Type
	 */
	void SetProgrammeType( TRdsProgrammeType aPty );

	/**
	 * Store the current Programme Service name.
	 * @since S60 3.2
	 * @param aPs Programme Service name
	 */
	void SetProgrammeService( TRdsPSName& aPs );

	/**
	 * Store the current Radio Text.
	 * @since S60 3.2
	 * @param aRt Radio Text
	 */
	void SetRadioText( TRdsRadioText& aRt );
	
  /**
   * Store the current Radio Text+ tags.
   * @since S60 5.0
   * @param aRtPlusObject1 Radio Text+ object string
   * @param aContentType1 Radio Text+ object class type
   * @param aRtPlusObject2 Radio Text+ object string
   * @param aContentType2 Radio Text+ object class type
   */
  void SetRadioTextPlusObjects( TRdsRadioText& aRtPlusObject1, TUint32 aContentType1,
          TRdsRadioText& aRtPlusObject2, TUint32 aContentType2);

    /**
     * Do character conversion for the RDS string payload.
     * @since S60 5.0
     * @param aTRdsStrSource 8-bit wide character payload, RadioText
     * @param aTRsRdsStrDest 16-bit wide converted string
     */
    void SetRDSCharConv( TRdsRadioText& aTRdsStrSource, TRsRdsRadioText& aTRsRdsStrDest );

	/**
	 * Store the current Clock Time and date.
	 * @since S60 3.2
	 * @param aCt Clock Time and date
	 */
	void SetClockTime( TDateTime& aCt );

	/**
	 * Store the current Traffic Announcement status.
	 * @since S60 3.2
	 * @param aTaOn Traffic Announcement status
	 */
	void SetTrafficAnnouncementStatus( TBool aTaOn );

	/**
	 * Set AF search status to ON.
	 * @since S60 3.2
	 */
	void SetRdsBeginAF();

	/**
	 * Set AF search status to OFF
	 * @since S60 3.2
	 * @param aError A standard system/RadioServer error code.
	 * @param aFrequency Frequency in Hz
	 */
	void SetRdsEndAF( TInt aError, TInt aFrequency );

	/**
	 * Reset all settings to default values.
	 */
	void Reset();

private:

	/**
	 * C++ default constructor.
	 */
	CRadioServerSettings();

	/**
	 * By default Symbian 2nd phase constructor is private.
	 */
	void ConstructL();

private:    // Data
	TBool iMonitorOn;				// Radio monitor status
	TBool iRadioOn;					// Radio state
	TBool iAntennaAttached;			// Antenna status
	TBool iOfflineMode;				// Offline mode
	TBool iTransmitterActive;		// Transmitter status
	TRsFrequencyRange iRange;		// Frequency range
	TInt iMinFreq;					// Minimum frequency for the current frequency range
	TInt iMaxFreq;					// Maximum frequency for the current frequency range
	TInt iFrequency;				// Frequency in Hz
	TBool iForceMonoEnabled;		// Force mono status
	TBool iSquelchEnabled;			// Squelch status
	TInt iVolume;					// Volume
	TBool iMute;					// Mute status
	TInt iBalanceLeft;				// Balance - left
	TInt iBalanceRight;				// Balance - right
	TBool iAutoSwitch;				// Automatic switching (AF) status
	};

#endif      // RADIOSERVERSETTINGS_H

// End of File
