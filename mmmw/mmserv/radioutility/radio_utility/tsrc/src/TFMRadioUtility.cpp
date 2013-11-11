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
* Description: The class provides a way to test Radio Utility's functions
*
*
*/


// INCLUDES
#include <e32cons.h>
#include <f32file.h>
#include <RadioUtility.h>
#include "RadioSession.h"
#include "TFMRadioUtility.h"

class CRadioUtility;

/**
* Cause a panic
* @param aPanic - The panic to be raised
**/
GLDEF_C void Panic(TInt aPanic)
	{
	_LIT(KNameOfApp,"TRadioEngine");
	User::Panic(KNameOfApp, aPanic);
	}

/**
* Start the BTTestHarness up.
**/
void StartupL()
	{
	CActiveScheduler *pA=new(ELeave) CActiveScheduler;
	CleanupStack::PushL(pA);
	CActiveScheduler::Install(pA);
	CTRadioUtility* tester = CTRadioUtility::NewLC();
	tester->StartTestingL();
	CActiveScheduler::Start();
	CleanupStack::Pop(); 
	delete tester;
	tester = NULL;
	CleanupStack::PopAndDestroy(1);
	}

/**
* The main entrypoint for the application
**/
GLDEF_C TInt E32Main()
	{

	__UHEAP_MARK;
	CTrapCleanup* cleanup=CTrapCleanup::New(); 
	TRAPD(error, StartupL() );

	delete cleanup; 
	__UHEAP_MARKEND;
	if (error == KErrNone)
		{
		return KErrNone;
		}
		else
		{
			return error;
		}
	}

/**
* NewL method for creating new instances of this class.
**/
CTRadioUtility* CTRadioUtility::NewL()
	{
	CTRadioUtility* s = CTRadioUtility::NewLC();
	CleanupStack::Pop();
	return s;
	}


/**
* NewLC method for creating new instances of this class.
**/
CTRadioUtility* CTRadioUtility::NewLC()
	{
	RDebug::Print(_L("CTRadioUtility::NewLC"));
	CTRadioUtility* s = new(ELeave) CTRadioUtility();
	CleanupStack::PushL(s);
	s->ConstructL();
	return s;
	}
/**
* MRadioPlayerObserver Callback 
* 
*
**/
void CTRadioUtility::MrpoStateChange( TPlayerState aState, TInt aError )
	{
		iConsole->Printf(_L("Player State Change: %x Error Code: %x\n"),aState, aError);
	}

/**
* MRadioPlayerObserver MrpoVolumeChange Callback 
* 
*
**/
void CTRadioUtility::MrpoVolumeChange( TInt aVolume )
	{
		iConsole->Printf(_L("Volume Change: %x\n"),aVolume);
	}

/**
* MRadioPlayerObserver MrpoMuteChange Callback 
* 
*
**/
void CTRadioUtility::MrpoMuteChange( TBool aMute )
	{
		iConsole->Printf(_L("Mute Change: %x\n"),aMute);
	}

/**
* MRadioPlayerObserver MrpoBalanceChange Callback 
* 
*
**/
void CTRadioUtility::MrpoBalanceChange( TInt aLeftPercentage, TInt aRightPercentage )
	{
		iConsole->Printf(_L("Left Percentage: %x Right Percentage: %x\n"),aLeftPercentage, aRightPercentage);
	}
	
/**
* MRadioFmTunerObserver MrftoRequestTunerControlComplete Callback 
* 
*
**/
void CTRadioUtility::MrftoRequestTunerControlComplete( TInt aError )
	{
		iConsole->Printf(_L("Request Tuner Control Complete: %x\n"),aError);
	}
	
/**
* MRadioFmTunerObserver MrftoSetFrequencyRangeComplete Callback 
* 
*
**/
void CTRadioUtility::MrftoSetFrequencyRangeComplete( TInt aError )
	{
		iConsole->Printf(_L("Set Frequency Range Complete: %x\n"),aError);
	}
	
/**
* MRadioFmTunerObserver MrftoSetFrequencyComplete Callback 
* 
*
**/
void CTRadioUtility::MrftoSetFrequencyComplete( TInt aError )
	{
		iConsole->Printf(_L("Set Frequency Complete: %x\n"),aError);
	}
	
/**
* MRadioFmTunerObserver  Callback 
* 
*
**/
void CTRadioUtility::MrftoStationSeekComplete( TInt aError, TInt aFrequency )
	{
		iConsole->Printf(_L("Station Seek Complete: %x Frequency: %x\n"),aError, aFrequency);
	}
	
/**
* MRadioFmTunerObserver MrftoFmTransmitterStatusChange Callback 
* 
*
**/
void CTRadioUtility::MrftoFmTransmitterStatusChange( TBool aActive )
	{
		iConsole->Printf(_L("Transmitter Status Change: %x\n"),aActive);
	}
	
/**
* MRadioFmTunerObserver MrftoAntennaStatusChange Callback 
* 
*
**/
void CTRadioUtility::MrftoAntennaStatusChange( TBool aAttached )
	{
		iConsole->Printf(_L("Antenna Status Change: %x\n"),aAttached);
	}
	
/**
* MRadioFmTunerObserver MrftoOfflineModeStatusChange Callback 
* 
*
**/
void CTRadioUtility::MrftoOfflineModeStatusChange( TBool aOfflineMode )
	{
		iConsole->Printf(_L("Offline Mode Change: %x\n"),aOfflineMode);
	}
	
/**
* MRadioFmTunerObserver MrftoFrequencyRangeChange Callback 
* 
*
**/
void CTRadioUtility::MrftoFrequencyRangeChange( TFmRadioFrequencyRange aNewRange )
	{
		iConsole->Printf(_L("Frequency Range Change: Range = %d"),aNewRange);
	}
	
/**
* MRadioFmTunerObserver MrftoFrequencyChange Callback 
* 
*
**/
void CTRadioUtility::MrftoFrequencyChange( TInt aNewFrequency )
	{
		iConsole->Printf(_L("Frequency Change: %x\n"),aNewFrequency);
	}
	
/**
* MRadioFmTunerObserver MrftoForcedMonoChange Callback 
* 
*
**/
void CTRadioUtility::MrftoForcedMonoChange( TBool aForcedMono )
	{
		iConsole->Printf(_L("Forced Mono Change: %x\n"),aForcedMono);
	}
	
/**
* MRadioFmTunerObserver  MrftoSquelchChange Callback 
* 
*
**/
void CTRadioUtility::MrftoSquelchChange( TBool aSquelch )
	{
		iConsole->Printf(_L("Squelch Change: %x\n"),aSquelch);
	}
		

/**
* MRadioRdsTunerObserver MrroStationSeekByPTYComplete Callback 
* 
*
**/
void CTRadioUtility::MrroStationSeekByPTYComplete( TInt /*aError*/, TInt aFrequency )
	{
		iConsole->Printf(_L(" Station Seek By PTY Complete: %x\n"),aFrequency);
	}
	
/**
* MRadioRdsTunerObserver MrroStationSeekByTAComplete Callback 
* 
*
**/
void CTRadioUtility::MrroStationSeekByTAComplete( TInt /*aError*/, TInt aFrequency )
	{
		iConsole->Printf(_L(" Station Seek By TA Complete: %x\n"),aFrequency);
	}

/**
* MRadioRdsTunerObserver MrroStationSeekByTPComplete Callback 
* 
*
**/
void CTRadioUtility::MrroStationSeekByTPComplete( TInt /*aError*/, TInt aFrequency ) 
	{
		iConsole->Printf(_L(" Statio Seek TP Complete: %x\n"),aFrequency);
	}

/**
* MRadioRdsTunerObserver MrroGetFreqByPTYComplete Callback 
* 
*
**/
void CTRadioUtility::MrroGetFreqByPTYComplete( TInt aError, RArray<TInt>& /*aFreqList*/ )
	{
		iConsole->Printf(_L(" Freq By PTY Complete: %x\n"),aError);
	}

/**
* MRadioRdsTunerObserver MrroGetFreqByTAComplete Callback 
* 
*
**/
void CTRadioUtility::MrroGetFreqByTAComplete( TInt aError, RArray<TInt>& /*aFreqList*/ )
	{
		iConsole->Printf(_L(" Freq By TA Complete: %x\n"),aError);
	}

/**
* MRadioRdsTunerObserver MrroGetPSByPTYComplete Callback 
* 
*
**/
void CTRadioUtility::MrroGetPSByPTYComplete( TInt aError, RArray<TRdsPSName>& /*aPsList*/ )
	{
		iConsole->Printf(_L(" PSByPTY Complete: %x\n"),aError);
	}

/**
* MRadioRdsTunerObserver MrroGetPSByTAComplete Callback 
* 
*
**/
void CTRadioUtility::MrroGetPSByTAComplete( TInt aError, RArray<TRdsPSName>& /*aPsList*/ )
	{
		iConsole->Printf(_L(" PSByTA Complete: %x\n"),aError);
	}
	
/**
* MRadioRdsTunerObserver MrroRdsDataPI Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataPI( TInt aPi )
	{
		iConsole->Printf(_L(" Data PI: %x\n"),aPi);
	}	

/**
* MRadioRdsTunerObserver MrroRdsDataPTY Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataPTY( TRdsProgrammeType aPty )
	{
		iConsole->Printf(_L(" Data PTY: %x\n"),aPty);
	}	

/**
* MRadioRdsTunerObserver MrroRdsDataPS Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataPS( TRdsPSName& /*aPs*/ )
	{
		iConsole->Printf(_L(" Data PS:\n"));
	}

/**
* MRadioRdsTunerObserver MrroRdsDataRT Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataRT( TRdsRadioText& /*aRt*/ )
	{
		iConsole->Printf(_L(" Data RT: \n"));
	}
	
/**
* MRadioRdsTunerObserver MrroRdsDataRTPlus Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataRTplus( TRdsRTplusClass /*aRtPlusClass*/, TRdsRadioText& /*aRtPlusData*/ )
	{
		iConsole->Printf(_L(" Data RTPlusData: \n"));
	}

/**
* MRadioRdsTunerObserver MrroRdsDataCT Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataCT( TDateTime& /*aCt*/ )
	{
		iConsole->Printf(_L(" Data CT: \n"));
	}

/**
* MRadioRdsTunerObserver MrroRdsDataTA Callback 
* 
*
**/
void CTRadioUtility::MrroRdsDataTA( TBool aTaOn )
	{
		iConsole->Printf(_L(" Data TA: %x\n"),aTaOn);
	}

/**
* MRadioRdsTunerObserver MrroRdsSearchBeginAF Callback 
* 
*
**/
void CTRadioUtility::MrroRdsSearchBeginAF()
	{
		iConsole->Printf(_L(" Search Begin AF:\n"));
	}

/**
* MRadioRdsTunerObserver MrroRdsSearchEndAF Callback 
* 
*
**/
void CTRadioUtility::MrroRdsSearchEndAF( TInt /*aError*/, TInt aFrequency )
	{
		iConsole->Printf(_L(" Search End AF: %x\n"),aFrequency);
	}

/**
* MRadioRdsTunerObserver MrroRdsStationChangeTA Callback 
* 
*
**/
void CTRadioUtility::MrroRdsStationChangeTA( TInt aFrequency )
	{
		iConsole->Printf(_L("Station Change TA: %x\n"),aFrequency);
	}

/**
* MRadioRdsTunerObserver MrroRdsEventAutomaticSwitchingChange Callback 
* 
*
**/
void CTRadioUtility::MrroRdsEventAutomaticSwitchingChange( TBool aAuto )
	{
		iConsole->Printf(_L(" Auto Switch Change: %x\n"),aAuto);
	}

/**
* MRadioRdsTunerObserver MrroRdsEventAutomaticTrafficAnnouncement Callback 
* 
*
**/
void CTRadioUtility::MrroRdsEventAutomaticTrafficAnnouncement( TBool aAuto )
	{
		iConsole->Printf(_L(" Auto TA Change: %x\n"),aAuto);
	}

/**
* MRadioRdsTunerObserver MrroRdsEventSignalChange Callback 
* 
*
**/
void CTRadioUtility::MrroRdsEventSignalChange( TBool aSignal )
	{
		iConsole->Printf(_L(" Signal Change: %x\n"),aSignal);
	}
    
/**
* 2nd phase constructor
**/
void CTRadioUtility::ConstructL()
	{
	RDebug::Print(_L("CTRadioUtility::ConstructL()"));
	iConsole=Console::NewL(_L("CTestRadioUtility"), TSize(-1,-1));
	iReadConsole = CTRadioUtilityConsole::NewL(this);
	TBool PrimaryClient = ETrue;
	iRadioUtility = CRadioUtility::NewL(PrimaryClient);
	iFmTunerUtility = &iRadioUtility->RadioFmTunerUtilityL(*this);
	iPlayerUtility  = &iRadioUtility->RadioPlayerUtilityL(*this);
	iRdsUtility     = &iRadioUtility->RadioRdsUtilityL(*this);
	}

/**
* c++ default constructor
**/
CTRadioUtility::CTRadioUtility()
	{
	}

/**
* destructor
**/
CTRadioUtility::~CTRadioUtility()
	{
//	iSharedDataClient.Close();
	delete iRadioUtility;
	delete iReadConsole;
	delete iConsole;
	User::After(20000000);
	}

/**
* Simple helper method to retrieve a pointer to the console method
* @return a pointer to the console
**/
CConsoleBase* CTRadioUtility::ConsoleWin()
	{
	return iConsole;
	}
	
//	
//
// Tuner Utility Test Cases
//
//

/**
* RequestTunerControl
* @return None
**/
void CTRadioUtility::RequestTunerControl()
	{
	iFmTunerUtility->RequestTunerControl();
	}
	
/**
* CloseTuner 
* @return None
**/
void CTRadioUtility::CloseTuner()
	{
	//iFmTunerUtility->Close();
	}
	
/**
* GetCapabilities 
* @return None
**/
void CTRadioUtility::GetTunerCapabilities()
	{
	TFmTunerCapabilities Caps;
	TInt err = iFmTunerUtility->GetCapabilities(Caps);
	iConsole->Printf(_L("TunerCapabilities: %x\n"),Caps.iTunerBands);
	}
	
/**
* EnableTunerInOfflineMode 
* @return None
**/
void CTRadioUtility::EnableTunerInOfflineMode()
	{
	TBool Enable = EFalse;
	TInt err = iFmTunerUtility->EnableTunerInOfflineMode(Enable);
	}
	
/**
* SetFrequencyRange 
* @return None
**/
void CTRadioUtility::SetFrequencyRange()
	{
	iRange = EFmRangeJapan;
	iConsole->Printf(_L("Frequency was Japanese: %x\n"),iRange);
	TFmRadioFrequencyRange Range = EFmRangeEuroAmerica;
	iFmTunerUtility->SetFrequencyRange(Range);
	iRange = Range;
	iConsole->Printf(_L("Frequency now set to America: %x\n"),iRange);
	}
	
/**
* GetFrequencyRange 
* @return None
**/
void CTRadioUtility::GetFrequencyRange()
	{
	TFmRadioFrequencyRange Range = EFmRangeEuroAmerica;
	TInt MinFreq;
	TInt MaxFreq;
	TInt err = iFmTunerUtility->GetFrequencyRange(Range, MinFreq, MaxFreq );
	iConsole->Printf(_L("GetFrequncy Range: Min %x Max %x\n"),MinFreq, MaxFreq);
	}
	
/**
* SetFrequency 
* @return None
**/
void CTRadioUtility::SetFrequency()
	{
	TInt Freq = 106100000;
	
	iFmTunerUtility->SetFrequency(Freq);
	iConsole->Printf(_L("Set Frequency to 106100000: %d\n"),Freq);
	}
	
/**
* GetFrequency 
* @return None
**/
void CTRadioUtility::GetFrequency()
	{
	TInt Freq;
	TInt err = iFmTunerUtility->GetFrequency(Freq);
	iConsole->Printf(_L("GetFrequncy: %d\n"),Freq);
	}
	
/**
* StationSee 
* @return None
**/
void CTRadioUtility::StationSeek()
	{
	TBool Upwards = ETrue;
	iFmTunerUtility->StationSeek(Upwards);
	}
	
/**
* GetSignalStrength 
* @return None
**/
void CTRadioUtility::GetSignalStrength()
	{
	TInt SignalStrength;
	TInt err = iFmTunerUtility->GetSignalStrength(SignalStrength);
	iConsole->Printf(_L("GetSignalStrength: %d\n"),SignalStrength);
	}
	
/**
* GetMaxSignalStrength
* @return None
**/
void CTRadioUtility::GetMaxSignalStrength()
	{
	TInt SignalStrength;
	TInt err = iFmTunerUtility->GetMaxSignalStrength(SignalStrength);
	iConsole->Printf(_L("GetMaxSignalStrength: %x\n"),SignalStrength);
	}
	
/**
* GetStereoMode 
* @return None
**/
void CTRadioUtility::GetStereoMode()
	{
	TBool Stereo;
	TInt err = iFmTunerUtility->GetStereoMode(Stereo);
	iConsole->Printf(_L("Stereo Mode: %x\n"),Stereo);
	} 

/**
* ForceMonoReception 
* @return None
**/
void CTRadioUtility::ForceMonoReception()
	{
	TBool ForceMono = ETrue;
	TInt err = iFmTunerUtility->ForceMonoReception(ForceMono);
	iConsole->Printf(_L("Set ForceMonoReception: %x\n"),ForceMono);
	}
	
/**
* GetForceMonoReception 
* @return None
**/
void CTRadioUtility::GetForceMonoReception()
	{
	TBool ForceMono;
	TInt err = iFmTunerUtility->GetForcedMonoReception(ForceMono);
	iConsole->Printf(_L("GetForceMonoReception: %x\n"),ForceMono);
	}
	
/**
* SetSquelch 
* @return None
**/
void CTRadioUtility::SetSquelch()
	{
	TBool Squelch = ETrue;
	TInt err = iFmTunerUtility->SetSquelch(Squelch);
	iConsole->Printf(_L("SetSquelch: %x\n"),Squelch);
	}
	
/**
* GetSquelch 
* @return None
**/
void CTRadioUtility::GetSquelch()
	{
	TBool Squelch;
	TInt err = iFmTunerUtility->GetSquelch(Squelch);
	iConsole->Printf(_L("GetSquelch: %x\n"),Squelch);
	}
	
/**
* CancelStationSeek 
* @return None
**/
void CTRadioUtility::CancelStationSeek()
	{
	iFmTunerUtility->CancelStationSeek();
	}
	
/**
* CancelSetFrequencyRange 
* @return None
**/
void CTRadioUtility::CancelSetFrequencyRange()
	{
	iFmTunerUtility->CancelSetFrequencyRange();
	}
	
/**
* CancelSetFrequency 
* @return None
**/
void CTRadioUtility::CancelSetFrequency()
	{
	iFmTunerUtility->CancelSetFrequency();
	}
	
	
//
// Player Utility Test Cases
//

/**
* Play Radio
* @return None
**/
void CTRadioUtility::Play()
	{
	iPlayerUtility->Play();
	}
	
/**
* PlayerState Radio
* @return None
**/
void CTRadioUtility::PlayerState()
	{ 
		TPlayerState PState;	
		PState = iPlayerUtility->PlayerState();
		iConsole->Printf(_L("PlayerState: %x\n"),PState);
	}
	
/**
* Close Radio
* @return None
**/
void CTRadioUtility::ClosePlayer()
	{
	iPlayerUtility->Close();
	}
	
/**
* Stop Radio
* @return None
**/
void CTRadioUtility::StopPlayer()
	{
	iPlayerUtility->Stop();
	}
	
/**
* Mute Radio
* @return None
**/
void CTRadioUtility::Mute()
	{
	TInt err = iPlayerUtility->Mute(ETrue);
	}
	
/**
* IsMute Radio
* @return None
**/
void CTRadioUtility::IsMute()
	{
	TBool Mute = iPlayerUtility->IsMute();
	iConsole->Printf(_L("IsMute: %x\n"),Mute);
	}
	
/**
* SetVolume Radio
* @return None
**/
void CTRadioUtility::SetVolume()
	{
	TInt Volume = 6;
	TInt err = iPlayerUtility->SetVolume(Volume);
	}
	
/**
* GetVolume Radio
* @return None
**/
void CTRadioUtility::GetVolume()
	{
	TInt Volume;
	TInt err = iPlayerUtility->GetVolume(Volume);
	iConsole->Printf(_L("Volume: %x\n"),Volume);
	}
	
/**
* SetVolumeRamp Radio
* @return None
**/
void CTRadioUtility::SetVolumeRamp()
	{
	TTimeIntervalMicroSeconds RampInterval = 4;
	TInt err = iPlayerUtility->SetVolumeRamp(RampInterval);
	iConsole->Printf(_L("RampInterval: %x\n"),RampInterval.Int64() );
	}
	   
/**
* GetMaxVolume Radio
* @return None
**/
void CTRadioUtility::GetMaxVolume()
	{
	TInt MaxVolume;
	TInt err = iPlayerUtility->GetMaxVolume(MaxVolume);
	iConsole->Printf(_L("MaxVolume: %x\n"),MaxVolume);
	}
	
/**
* SetBalance Radio
* @return None
**/
void CTRadioUtility::SetBalance()
	{
	TInt LeftPercentage = 49;
	TInt RightPercentage = 51;
	TInt err = iPlayerUtility->SetBalance(LeftPercentage, RightPercentage);
	}
	
/**
* GetBalance Radio
* @return None
**/
void CTRadioUtility::GetBalance() 
	{
	TInt LeftPercentage;
	TInt RightPercentage;
	TInt err = iPlayerUtility->GetBalance(LeftPercentage, RightPercentage);
	iConsole->Printf(_L("GetBalance: left %x right %x \n"),LeftPercentage,RightPercentage);
	}
	
//
// Rds Utility
//

void CTRadioUtility::Close()
	{
	iRdsUtility->Close();
	}

void CTRadioUtility::GetRdsCapabilities()
	{
	TRdsCapabilities Caps;
	TInt err = iRdsUtility->GetCapabilities( Caps );
	}


void CTRadioUtility::GetRdsSignalStatus()
	{
	TBool RdsSignal = ETrue;
	TInt err = iRdsUtility->GetRdsSignalStatus( RdsSignal );
	}

void CTRadioUtility::NotifyRdsDataChange()
	{
	TRdsData RdsData;
	RdsData.iAdditionalFunctions1 = 0;
	RdsData.iAdditionalFunctions2 = 0;
	RdsData.iRdsFunctions = ERdsTrafficAnnouncement | ERdsRadioText | ERdsClockTime;
	TInt err = iRdsUtility->NotifyRdsDataChange( RdsData );
	}

void CTRadioUtility::CancelNotifyRdsDataChange()
	{
	iRdsUtility->CancelNotifyRdsDataChange();
	}

void CTRadioUtility::SetAutomaticSwitching()
	{
	TBool Auto = ETrue;
	TInt err = iRdsUtility->SetAutomaticSwitching( Auto );
	}

void CTRadioUtility::GetAutomaticSwitching()
	{
	TBool Auto = EFalse;
	TInt err = iRdsUtility->GetAutomaticSwitching( Auto );
	}

void CTRadioUtility::CancelAFSearch()
	{
	iRdsUtility->CancelAFSearch();
	}

void CTRadioUtility::SetAutomaticTrafficAnnouncement()
	{
	TBool Auto = ETrue;
	TInt err = iRdsUtility->SetAutomaticTrafficAnnouncement( Auto );
	}

void CTRadioUtility::GetAutomaticTrafficAnnouncement()
	{
	TBool Auto = EFalse;
	TInt err = iRdsUtility->GetAutomaticTrafficAnnouncement( Auto );
	}

void CTRadioUtility::StationSeekByPTY()
	{
	TRdsProgrammeType Pty = KRdsPtyNone;
	TBool SeekUp = ETrue;
	iRdsUtility->StationSeekByPTY( Pty, SeekUp );
	}

void CTRadioUtility::StationSeekByTA()
	{
	TBool SeekUp = ETrue;
	iRdsUtility->StationSeekByTA( SeekUp );
	}

void CTRadioUtility::StationSeekByTP()
	{
	TBool SeekUp = ETrue;
	iRdsUtility->StationSeekByTP( SeekUp );
	}

void CTRadioUtility::CancelRdsStationSeek()
	{
	iRdsUtility->CancelRdsStationSeek();
	}

void CTRadioUtility::GetFreqByPTY()
	{
	TRdsProgrammeType Pty = KRdsPtyNone;
	iRdsUtility->GetFreqByPTY( Pty );
	}

void CTRadioUtility::CancelGetFreqByPTY()
	{
	iRdsUtility->CancelGetFreqByPTY();
	}

void CTRadioUtility::GetFreqByTA()
	{
	iRdsUtility->GetFreqByTA();
	}

void CTRadioUtility::CancelGetFreqByTA()
	{
	iRdsUtility->CancelGetFreqByTA();
	}

void CTRadioUtility::GetPSByPTY()
	{
	TRdsProgrammeType Pty = KRdsPtyNone;
	iRdsUtility->GetPSByPTY( Pty );
	}

void CTRadioUtility::CancelGetPSByPTY()
	{
	iRdsUtility->CancelGetPSByPTY();
	}

void CTRadioUtility::GetPSByTA()
	{
	iRdsUtility->GetPSByTA();
	}

void CTRadioUtility::CancelGetPSByTA()
	{
	iRdsUtility->CancelGetPSByTA();
	}  

void CTRadioUtility::GetProgrammeIdentification()
	{
	TInt Pi = 0;
	TInt err = iRdsUtility->GetProgrammeIdentification( Pi );
	}

void CTRadioUtility::GetProgrammeType()
	{
	TRdsProgrammeType Pty = KRdsPtyNone;
	TInt err = iRdsUtility->GetProgrammeType( Pty );
	}

void CTRadioUtility::GetProgrammeService()
	{
	TRdsPSName Ps = NULL;
	TInt err = iRdsUtility->GetProgrammeService( Ps );
	}

void CTRadioUtility::GetRadioText()
	{
	TRdsRadioText Rt;
	TInt err = iRdsUtility->GetRadioText( Rt );
	}
	
void CTRadioUtility::GetRadioTextPlus()
	{
	TRdsRTplusClass RtPlusClass( ERTplusItemDummy );
	TRdsRadioText   RtPlusData;
	
	TInt err = iRdsUtility->GetRadioTextPlus( RtPlusClass, RtPlusData );
	}

void CTRadioUtility::GetClockTime()
	{
	TDateTime Ct;
	TInt err = iRdsUtility->GetClockTime( Ct );
	}

void CTRadioUtility::GetTrafficAnnouncementStatus()
	{
	TBool TaStatus = EFalse;
	TInt err = iRdsUtility->GetTrafficAnnouncementStatus( TaStatus );
	}

void CTRadioUtility::GetTrafficProgrammeStatus()
	{
	TBool TpStatus = EFalse;
	TInt err = iRdsUtility->GetTrafficProgrammeStatus( TpStatus );
	}

/**
* Start listening for client keypresses.
**/
void CTRadioUtility::StartTestingL()
	{
	iReadConsole->IssueRequestL();
	}

/**
* Shutdown the BTTestHarness console.
**/
void CTRadioUtility::StopTesting()
	{
	CActiveScheduler::Stop();
	}

void CTRadioUtility::HandleNotifyL(const TUid /*aUid*/, const TDesC& /*aKey*/, const TDesC& /*aValue*/)
	{
	}
	
/**
* NewL method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
CTRadioUtilityConsole* CTRadioUtilityConsole::NewL(CTRadioUtility* aParent)
	{
	CTRadioUtilityConsole* s = CTRadioUtilityConsole::NewLC(aParent);
	CleanupStack::Pop();
	return s;
	}
	
/**
* NewLC method for creating new instances of this class.
* @param aParent - a pointer to the BTTestHarness
* @return - a pointer to the newly created CBTTestHarnessConsole
**/
CTRadioUtilityConsole* CTRadioUtilityConsole::NewLC(CTRadioUtility* aParent)
	{
	CTRadioUtilityConsole* s = new(ELeave) CTRadioUtilityConsole(aParent);
	CleanupStack::PushL(s);
	s->ConstructL();
	return s;
	}

/**
* Destructor
**/
CTRadioUtilityConsole::~CTRadioUtilityConsole()
	{
	Cancel();
	}


/**
* This method is called whenever a key is pressed in the console window.
* 
* Player Utility Selections
*
**/

void CTRadioUtilityConsole::RunL()
	{
	TInt key = iConsole->KeyCode();
	switch (key)
		{
	case '1':
		iParent->Play();
		IssueRequestL();
		break;		
	case '2':
		iParent->PlayerState();
		IssueRequestL();
		break;
	case '3':
		iParent->ClosePlayer();
		IssueRequestL();
		break;
	case '4':
		iParent->StopPlayer();
		IssueRequestL();
		break;
	case '5':
		iParent->Mute();
		IssueRequestL();
		break;
	case '6':
		iParent->IsMute();
		IssueRequestL();
		break;
	case '7':
		iParent->SetVolume();
		IssueRequestL();
		break;
	case '8':
		iParent->GetVolume();
		IssueRequestL();
		break;
	case '9':
		iParent->SetVolumeRamp();
		IssueRequestL();
		break;
	case '0':
		iParent->GetMaxVolume();
		IssueRequestL();
		break;
	case 'a':
		iParent->RequestTunerControl();
		IssueRequestL();
		break;
	case 'd':
		iParent->SetFrequency();
		IssueRequestL();
		break;
	case 'g':
		iParent->Play();
		IssueRequestL();
		break;		
	case 'j':
		iParent->SetVolume();
		IssueRequestL();
		break;	
	case 'm':
		iParent->StopPlayer();
		IssueRequestL();
		break;	
	case 'p':
		iParent->IsMute();
		IssueRequestL();
		break;	
	case 't':
		iParent->GetVolume();
		IssueRequestL();
		break;	
		
	case 'z':
		IssueRequestL();
		break;
	case 'x':
		iParent->StopTesting();
		break;
	default:
		IssueRequestL();
		break;
		}
	}

/**
* This method is called whenever a key is pressed in the console window.
* 
* Tuner Utility Selections
*
**/
/*
void CTRadioUtilityConsole::RunL()
	{

	TInt key = iConsole->KeyCode();
	switch (key)
		{
	case '1':
		iParent->RequestTunerControl();
		IssueRequestL();
		break;
	case '2':
		iParent->EnableTunerInOfflineMode();
		IssueRequestL();
		break;
	case '3':
		iParent->CloseTuner();
		IssueRequestL();
		break;
	case '4':
		iParent->GetCapabilities();
		IssueRequestL();
		break;
	case '5':
		iParent->SetFrequencyRange();
		IssueRequestL();
		break;
	case '6':
		iParent->GetFrequencyRange();
		IssueRequestL();
		break;
	case '7':
		iParent->SetFrequency();
		IssueRequestL();
		break;
	case '8':
		iParent->GetFrequency();
		IssueRequestL();
		break;
	case '9':
		iParent->StationSeek();
		IssueRequestL();
		break;
	case '0':
		iParent->GetSignalStrength();
		IssueRequestL();
		break;
	case 'a':
		iParent->GetMaxSignalStrength();
		IssueRequestL();
		break;
	case 'b':
		iParent->GetStereoMode();
		IssueRequestL();
		break;
	case 'c':
		iParent->ForceMonoReception();
		IssueRequestL();
		break;
	case 'd':
		iParent->GetForceMonoReception();
		IssueRequestL();
		break;
	case 'e':
		iParent->SetSquelch();
		IssueRequestL();
		break;
	case 'f':
		iParent->GetSquelch();
		IssueRequestL();
		break;
	case 'g':
		iParent->CancelStationSeek();
		IssueRequestL();
		break;
	case 'h':
		iParent->CancelSetFrequency();
		IssueRequestL();
		break;
	case 'i':
		iParent->CancelSetFrequencyRange();
		IssueRequestL();
		break;
	case 'j':
		iParent->Play();
		IssueRequestL();
		break;	

	case 'z':
		IssueRequestL();
		break;
	case 'x':
		iParent->StopTesting();
		break;
	default:
		IssueRequestL();
		break;
		}
	key = 0;
	}
*/

/**
* This method is called whenever a key is pressed in the console window.
* 
* Rds Utility Selections
*
**/
/*
void CTRadioUtilityConsole::RunL()
	{

	TInt key = iConsole->KeyCode();
	switch (key)
		{
	case '1':
		iParent->RequestTunerControl();
		IssueRequestL();
		break;
	case '2':
		iParent->EnableTunerInOfflineMode();
		IssueRequestL();
		break;
	case '3':
		iParent->Close();
		IssueRequestL();
		break;
	case '4':
		iParent->GetCapabilities();
		IssueRequestL();
		break;
	case '5':
		iParent->GetRdsSignalStatus();
		IssueRequestL();
		break;
	case '6':
		iParent->NotifyRdsDataChange();
		IssueRequestL();
		break;
	case '7':
		iParent->CancelNotifyRdsDataChange();
		IssueRequestL();
		break;
	case '8':
		iParent->SetAutomaticSwitching();
		IssueRequestL();
		break;
	case '9':
		iParent->GetAutomaticSwitching();
		IssueRequestL();
		break;
	case '0':
		iParent->CancelAFSearch();
		IssueRequestL();
		break;
	case 'a':
		iParent->SetAutomaticTrafficAnnouncement();
		IssueRequestL();
		break;
	case 'b':
		iParent->GetAutomaticTrafficAnnouncement();
		IssueRequestL();
		break;
	case 'c':
		iParent->StationSeekByPTY();
		IssueRequestL();
		break;
	case 'd':
		iParent->StationSeekByTA();
		IssueRequestL();
		break;
	case 'e':
		iParent->StationSeekByTP();
		IssueRequestL();
		break;
	case 'f':
		iParent->CancelRdsStationSeek();
		IssueRequestL();
		break;
	case 'g':
		iParent->GetFreqByPTY();
		IssueRequestL();
		break;
	case 'h':
		iParent->CancelGetFreqByPTY();
		IssueRequestL();
		break;
	case 'i':
		iParent->GetFreqByTA();
		IssueRequestL();
		break;
	case 'j':
		iParent->CancelGetFreqByTA();
		IssueRequestL();
		break;	
	case 'k':
		iParent->GetPSByPTY();
		IssueRequestL();
		break;
	case 'l':
		iParent->CancelGetPSByPTY();
		IssueRequestL();
		break;
	case 'm':
		iParent->GetPSByTA();
		IssueRequestL();
		break;	
	case 'n':
		iParent->CancelGetPSByTA();
		IssueRequestL();
		break;
	case 'o':
		iParent->GetProgrammeIdentification();
		IssueRequestL();
		break;
	case 'p':
		iParent->GetProgrammeType();
		IssueRequestL();
		break;
	case 'q':
		iParent->GetProgrammeService();
		IssueRequestL();
		break;
	case 'r':
		iParent->GetRadioText();
		IssueRequestL();
		break;
	case 's':
		iParent->GetRadioTextPlus();
		IssueRequestL();
		break;
	case 't':
		iParent->GetClockTime();
		IssueRequestL();
		break;
	case 'u':
		iParent->GetTrafficAnnouncementStatus();
		IssueRequestL();
		break;
	case 'v':
		iParent->GetTrafficProgrammeStatus();
		IssueRequestL();
		break;
		

	case 'z':
		IssueRequestL();
		break;
	case 'x':
		iParent->StopTesting();
		break;
	default:
		IssueRequestL();
		break;
		}
	key = 0;
	}
*/
/**
* Cancel any outstanding test requests.
**/
void CTRadioUtilityConsole::DoCancel()
	{
	if (iConsole)
		{
		iConsole->ReadCancel();
		iConsole->Printf(_L("Console read cancelled...\n"));
		}
	}

/**
* Constructor
* @param aParent - a pointer to the BTTestHarness
**/
CTRadioUtilityConsole::CTRadioUtilityConsole(CTRadioUtility* aParent) :
	CActive(EPriorityStandard),
	iParent(aParent)
	{
	iConsole = iParent->ConsoleWin();
	CActiveScheduler::Add(this);
	}

/**
* 2nd phase constructor
**/
void CTRadioUtilityConsole::ConstructL()
	{
	}

/**
* Display the console options, and wait for another key press.
*
* Player Utility
*
**/

void CTRadioUtilityConsole::IssueRequestL() // here is what you can choose
	{
	iConsole->Printf(_L("1. Play\n"));
	iConsole->Printf(_L("2. PlayerState\n"));
	iConsole->Printf(_L("3. Close\n"));
	iConsole->Printf(_L("4. Stop\n"));
	iConsole->Printf(_L("5. Mute\n"));
	iConsole->Printf(_L("6. IsMute\n"));
	iConsole->Printf(_L("7. SetVolume\n"));
	iConsole->Printf(_L("8. GetVolume\n"));
	iConsole->Printf(_L("9. SetVolumeRamp\n"));
	iConsole->Printf(_L("0. GetMaxVolume\n"));
	iConsole->Printf(_L("a. RequestTunerControl\n"));
	iConsole->Printf(_L("d. SetFrequency\n"));
	iConsole->Printf(_L("g. Play\n"));
	iConsole->Printf(_L("j. SetVolume\n"));
	iConsole->Printf(_L("m. Stop\n"));
	iConsole->Printf(_L("p. IsMute\n"));
	iConsole->Printf(_L("t. GetVolume\n"));

	iConsole->Printf(_L("---------------------\n"));
	iConsole->Printf(_L("x. Exit\n"));
	ReadConsoleL();
	}

	
/**
* Display the console options, and wait for another key press.
*
* Tuner Utility
*
**/
/*
void CTRadioUtilityConsole::IssueRequestL() 
	{
	iConsole->Printf(_L("1. RequestTunerControl\n"));
	iConsole->Printf(_L("2. EnableTunerInOfflineMode\n"));
	iConsole->Printf(_L("3. CloseTuner\n"));
	iConsole->Printf(_L("4. GetTunerCapabilities\n"));
	iConsole->Printf(_L("5. SetFrequencyRange\n"));
	iConsole->Printf(_L("6. GetFrequencyRange\n"));
	iConsole->Printf(_L("7. SetFrequency\n"));
	iConsole->Printf(_L("8. GetFrequency\n"));
	iConsole->Printf(_L("9. StationSeek\n"));
	iConsole->Printf(_L("0. GetSignalStrength\n"));
	iConsole->Printf(_L("a. GetMaxSignalStrength\n"));
	iConsole->Printf(_L("b. GetStereoMode\n"));
	iConsole->Printf(_L("c. ForceMonoRecepion\n"));
	iConsole->Printf(_L("d. GetForceMonoReception\n"));
	iConsole->Printf(_L("e. SetSquelch\n"));
	iConsole->Printf(_L("f. GetSquelch\n"));
	iConsole->Printf(_L("g. CancelStationSeek\n"));
	iConsole->Printf(_L("h. CancelSetFrequency\n"));
	iConsole->Printf(_L("i. CancelSetFrequencyRange\n"));
	iConsole->Printf(_L("j. Play\n"));


	iConsole->Printf(_L("---------------------\n"));
	iConsole->Printf(_L("x. Exit\n"));
	ReadConsoleL();
	}
*/

/**
* Display the console options, and wait for another key press.
*
* Rds Utility
*
**/
/*
void CTRadioUtilityConsole::IssueRequestL() 
	{
	iConsole->Printf(_L("1. RequestTunerControl\n"));
	iConsole->Printf(_L("2. EnableTunerInOfflineMode\n"));
	iConsole->Printf(_L("3. CloseRds\n"));
	iConsole->Printf(_L("4. GetRdsCapabilities\n"));
	iConsole->Printf(_L("5. GetRdsSignalStatus\n"));
	iConsole->Printf(_L("6. NotifyRdsDataChange\n"));
	iConsole->Printf(_L("7. CancelNotifyRdsDataChange\n"));
	iConsole->Printf(_L("8. SetAutomaticSwitching\n"));
	iConsole->Printf(_L("9. GetAutomaticSwitching\n"));
	iConsole->Printf(_L("0. CancelAFSearch\n"));
	iConsole->Printf(_L("a. SetAutomaticTrafficAnnouncement\n"));
	iConsole->Printf(_L("b. GetAutomaticTrafficAnnouncement\n"));
	iConsole->Printf(_L("c. StationSeekByPTY\n"));
	iConsole->Printf(_L("d. StationSeekByTA\n"));
	iConsole->Printf(_L("e. StationSeekByTP\n"));
	iConsole->Printf(_L("f. CancelRdsStationSeek\n"));
	iConsole->Printf(_L("g. GetFreqByPTY\n"));
	iConsole->Printf(_L("h. CancelGetFreqByPTY\n"));
	iConsole->Printf(_L("i. GetFreqByTA\n"));
	iConsole->Printf(_L("j. CancelGetFreqByTA\n"));
	iConsole->Printf(_L("k. GetPSByPTY\n"));
	iConsole->Printf(_L("l. CancelGetPSByPTY\n"));
	iConsole->Printf(_L("m. GetPSByTA\n"));
	iConsole->Printf(_L("n. CancelGetPSByTA\n"));
	iConsole->Printf(_L("o. GetProgrammeIdentification\n"));
	iConsole->Printf(_L("p. GetProgrammeType\n"));
	iConsole->Printf(_L("q. GetProgrammeService\n"));
	iConsole->Printf(_L("r. GetRadioText\n"));
	iConsole->Printf(_L("s. GetRadioTextPlus\n"));
	iConsole->Printf(_L("t. GetClockTime\n"));
	iConsole->Printf(_L("u. GetTrafficAnnouncementStatus\n"));
	iConsole->Printf(_L("v. GetTrafficProgrammeStatus\n"));

	iConsole->Printf(_L("---------------------\n"));
	iConsole->Printf(_L("x. Exit\n"));
	ReadConsoleL();
	}

*/
/**
* Outstanding request has been completed, start waiting for another request.
**/
void CTRadioUtilityConsole::ReadConsoleL()
	{
	iConsole->Read(iStatus);
	SetActive();
	}

	
