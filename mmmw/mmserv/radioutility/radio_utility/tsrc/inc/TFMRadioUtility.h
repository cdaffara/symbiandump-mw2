/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Header class of CTRadioUtility.
*
*
*/

#ifndef __CTRADIOUTILITY_H
#define __CTRADIOUTILITY_H

//#include <e32base.h>
//#include <e32cons.h>
//#include "RadioSession.h"
#include <RadioUtility.h>
#include <RadioPlayerUtility.h>
#include <RadioFmTunerUtility.h>
#include <RadioRdsUtility.h>


// FORWARD DECLARATIONS
class CTRadioUtilityConsole;
class CRadioUtility;

GLREF_C void Panic(TInt aPanic);

class CTRadioUtility : public CBase,
						MRadioPlayerObserver,
						MRadioFmTunerObserver,
						MRadioRdsObserver
	{
public:

/**
* NewL method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
	static CTRadioUtility* NewL();
/**
* NewLC method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
	static CTRadioUtility* NewLC();
/**
* Destructor
**/
	~CTRadioUtility();
	
	//
	//Player Utility Methods to Test...
	//
	void Play();
	void PlayerState();
	void ClosePlayer();
	void StopPlayer();
	void Mute();
	void IsMute();
	void SetVolume();
	void GetVolume();
	void SetVolumeRamp();
	void GetMaxVolume();
	void SetBalance();
	void GetBalance();
	
	//
	//Tuner Utility Methods to Test...
	//
	void RequestTunerControl();
	void CloseTuner();
	void GetTunerCapabilities();
	void EnableTunerInOfflineMode();
	void SetFrequencyRange();
	void CancelSetFrequencyRange();
	void GetFrequencyRange();
	void SetFrequency();
	void CancelSetFrequency();
	void GetFrequency();
	void StationSeek();
	void CancelStationSeek();
	void GetSignalStrength();
	void GetMaxSignalStrength();
	void GetStereoMode();
	void ForceMonoReception();
	void GetForceMonoReception();
	void SetSquelch();
	void GetSquelch();
	
	//
	//Rds Utility Methods to Test...
	//
	
	 void Close();
     void GetRdsCapabilities();
     void GetRdsSignalStatus();
     void NotifyRdsDataChange();
     void CancelNotifyRdsDataChange();
     void SetAutomaticSwitching();
     void GetAutomaticSwitching();
     void CancelAFSearch();
     void SetAutomaticTrafficAnnouncement();
     void GetAutomaticTrafficAnnouncement();
     void StationSeekByPTY();
     void StationSeekByTA();
     void StationSeekByTP();
     void CancelRdsStationSeek();
     void GetFreqByPTY();
     void CancelGetFreqByPTY();
     void GetFreqByTA();
     void CancelGetFreqByTA();
     void GetPSByPTY();
     void CancelGetPSByPTY();
     void GetPSByTA();
     void CancelGetPSByTA();
     void GetProgrammeIdentification();
     void GetProgrammeType();
     void GetProgrammeService();
     void GetRadioText();
     void GetRadioTextPlus();
     void GetClockTime();
     void GetTrafficAnnouncementStatus();
     void GetTrafficProgrammeStatus();
    
	//
	// MRadioPlayerObserver Methods
	//
  
     void MrpoStateChange( TPlayerState aState, TInt aError );
     void MrpoVolumeChange( TInt aVolume );
     void MrpoMuteChange( TBool aMute );
     void MrpoBalanceChange( TInt aLeftPercentage, TInt aRightPercentage );
     
    //
	// MRadioFmTunerObserver Methods
	//

    void MrftoRequestTunerControlComplete( TInt aError );
    void MrftoSetFrequencyRangeComplete( TInt aError );
    void MrftoSetFrequencyComplete( TInt aError ); 
    void MrftoStationSeekComplete( TInt aError, TInt aFrequency );
    void MrftoFmTransmitterStatusChange( TBool aActive );
    void MrftoAntennaStatusChange( TBool aAttached );
    void MrftoOfflineModeStatusChange( TBool aOfflineMode ); 
    void MrftoFrequencyRangeChange( TFmRadioFrequencyRange aNewRange ); 
    void MrftoFrequencyChange( TInt aNewFrequency ); 
    void MrftoForcedMonoChange( TBool aForcedMono );
    void MrftoSquelchChange( TBool aSquelch );
	
    //
	// MRadioRdsTunerObserver Methods
	//
    void MrroStationSeekByPTYComplete( TInt aError, TInt aFrequency );
    void MrroStationSeekByTAComplete( TInt aError, TInt aFrequency );
    void MrroStationSeekByTPComplete( TInt aError, TInt aFrequency ); 
    void MrroGetFreqByPTYComplete( TInt aError, RArray<TInt>& aFreqList );
    void MrroGetFreqByTAComplete( TInt aError, RArray<TInt>& aFreqList );
    void MrroGetPSByPTYComplete( TInt aError, RArray<TRdsPSName>& aPsList );
    void MrroGetPSByTAComplete( TInt aError, RArray<TRdsPSName>& aPsList );
    void MrroRdsDataPI( TInt aPi );
    void MrroRdsDataPTY( TRdsProgrammeType aPty );
    void MrroRdsDataPS( TRdsPSName& aPs );
    void MrroRdsDataRT( TRdsRadioText& aRt );
    void MrroRdsDataRTplus( TRdsRTplusClass aRtPlusClass, TRdsRadioText& aRtPlusData );
    void MrroRdsDataCT( TDateTime& aCt );
    void MrroRdsDataTA( TBool aTaOn );
    void MrroRdsSearchBeginAF();
    void MrroRdsSearchEndAF( TInt aError, TInt aFrequency );
	void MrroRdsStationChangeTA( TInt aFrequency );
    void MrroRdsEventAutomaticSwitchingChange( TBool aAuto );
    void MrroRdsEventAutomaticTrafficAnnouncement( TBool aAuto );
    void MrroRdsEventSignalChange( TBool aSignal );
 

/**
* Callback that is used to indicate that a new hci command has been received 
* from the chip.
* @param aString - The content of the hci frame that was received.
**/
	//void HandleRadioEngineCallBack(TFMRadioNotifyEvent aEventCode, TInt aErrorCode);
/**
* Start listening for client keypresses.
**/
	void StartTestingL();	
	void StopTesting();
/**
* Simple helper method to retrieve a pointer to the console method
* @return a pointer to the console
**/
	CConsoleBase* ConsoleWin();

private:
/**
* 2nd phase constructor
**/
	void ConstructL();
/**
* c++ default constructor
**/
	CTRadioUtility();
	void HandleNotifyL( const TUid aUid, const TDesC& aKey, const TDesC& aValue );
public:
	CConsoleBase* iConsole; //pointer to the console window
private:
	CRadioUtility*			iRadioUtility;
	CTRadioUtilityConsole* 	iReadConsole; 
	CRadioFmTunerUtility*	iFmTunerUtility;
	CRadioPlayerUtility*	iPlayerUtility;
	CRadioRdsUtility*		iRdsUtility;
	TFmRadioFrequencyRange  iRange;
	};

class CTRadioUtilityConsole : public CActive
	{
public:
/**
* NewL method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
	static CTRadioUtilityConsole* NewL(CTRadioUtility* aParent);
/**
* NewLC method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
	static CTRadioUtilityConsole* NewLC(CTRadioUtility* aParent);
/**
* Destructor
**/
	~CTRadioUtilityConsole();
/**
* Display the console options, and wait for another key press.
**/
	void IssueRequestL();
/**
* This method is called whenever a key is pressed in the console window.
**/
	void RunL();
/**
* Cancel any outstanding test requests.
**/
	void DoCancel();
protected:
	CTRadioUtilityConsole(CTRadioUtility* aParent);
private:
/**
* 2nd phase constructor
**/
	void ConstructL();
/**
* Outstanding request has been completed, start waiting for another request.
**/
	void ReadConsoleL();
	CRadioUtility* GetRadioEngine();
private:
	CTRadioUtility* iParent;
	CConsoleBase* iConsole;
	};

#endif // __CTRADIOUTILITY_H
