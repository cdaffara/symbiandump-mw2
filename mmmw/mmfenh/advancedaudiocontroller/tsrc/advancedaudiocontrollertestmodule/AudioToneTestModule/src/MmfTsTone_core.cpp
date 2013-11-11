/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MmfTsTone class member functions
*
*/


// INCLUDE FILES
#include "MmfTsTone.h"
#include "TemplateParameters.h"
#include "DTMFParameters.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
const TInt CMmfTsTone::KTestCaseVolume=501;
const TInt CMmfTsTone::KTestCaseVolumeRamp=502;
const TInt CMmfTsTone::KTestCaseRepeat=503;
const TInt CMmfTsTone::KTestCaseFixedSequences=504;
const TInt CMmfTsTone::KTestCasePlayFileSequence=505;
const TInt CMmfTsTone::KTestCaseBalance=506;
const TInt CMmfTsTone::KTestCasePriorities=507;
const TInt CMmfTsTone::KTestCaseDTMFString=508;
const TInt CMmfTsTone::KTestCaseCancelPrepare=509;
const TInt CMmfTsTone::KTestCaseDualTone=510;
const TInt CMmfTsTone::KTestCasePlayToneFile=511;
const TInt CMmfTsTone::KTestCasePlayToneRFile=512;  
const TInt CMmfTsTone::KTestCaseCustomInterface=513;  
const TInt CMmfTsTone:: KTestCaseCheckState=514;
const TInt CMmfTsTone:: KTestCaseMaxVolumePlay=515;
const TInt CMmfTsTone::KTestCasePlayCancel=516;
const TInt CMmfTsTone::KTestCasePlayChangeVolume=517;
const TInt CMmfTsTone::KTestCaseInvalidFrequencyCheck=518;
const TInt CMmfTsTone::KTestCaseCorruptFile=519;


// MACROS
// None

// LOCAL CONSTANTS AND MACROS
//typdef TFixedArray<TInt, 2> TTwoIntArray;

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================
// None

/*-------------------------------------------------------------------------------

    DESCRIPTION

    This module contains the implementation of MmfTsTone class 
	member functions.

-------------------------------------------------------------------------------*/

// ================= MEMBER FUNCTIONS =========================================

TInt CMmfTsTone::RunTestCaseFunctionL(TInt id, CStifSectionParser * section, TTestResult &aResult) 
	{
	TInt error = KErrNone;
	switch (id)
		{
		case KTestCaseVolume:
		    {
			error = TestCaseVolumeL(section, aResult);
			break;
		    }
		case KTestCaseVolumeRamp:
		    {
		    error = TestCaseVolumeRampL(section, aResult);
			break;
		    }
		case KTestCaseRepeat:
		    {
		    error = TestCaseRepeatL(section, aResult);
			break;
		    }
		case KTestCaseFixedSequences:
		    {
		    error = TestCaseFixedSecuencesL(section, aResult);
		    break;
		    }
		case KTestCasePlayFileSequence:
		    {
		    error = TestCasePlayFileSequenceL(section, aResult);
			break;
		    }
		case KTestCaseBalance:
		    {
		    error = TestCaseBalanceL(section, aResult);
			break;
		    }
		case KTestCasePriorities:
		    {
		    error = TestCasePrioritiesL(section, aResult);
		    break;
		    }
		case KTestCaseDTMFString:
		    {
		    error = TestCaseDTMFStringL(section, aResult);
		    break;
		    }
		case KTestCaseDualTone:
		    {
		    error = TestCaseDualToneL(section, aResult);
			break;
		    }
		case KTestCaseCancelPrepare:
		    {
		    error = TestCaseCancelPrepareL(section, aResult);
		    break;
		    }
		case KTestCasePlayToneFile:
		    {
		    error = TestCasePlayToneFileL(section, aResult);
			break;
		    }
		case KTestCasePlayToneRFile:
		    {
		    error = TestCasePlayToneRFileL(section, aResult);
		    break;
		    }
		case KTestCaseCustomInterface:
		    {
		    error = TestCaseCustomInterfaceL(section, aResult);
		    break;
		    }
		case KTestCaseCheckState:
		    {
		    error=TestCaseCheckStateL(section,aResult);
		    break;
		    }
		case KTestCaseMaxVolumePlay:
		    {
		    error=TestCaseMaxVolumePlayL(section,aResult);
		    break;
		    }
		case KTestCasePlayCancel:
		    {
		    error=TestCaseCancelPlayL(section,aResult);
		    break;
		    }
		case KTestCasePlayChangeVolume:
		    {
		    error=TestCaseVolumeChangeL(section,aResult);
		    break;
		    }
		case KTestCaseInvalidFrequencyCheck:
		    {
		    error = TestCaseDualToneL(section, aResult);
		    break;
		    }
		case KTestCaseCorruptFile:
		    {
		    error=TestCasePlayCorruptFileL(section,aResult);
		    break;
		    }
		default:
		    {
		    aResult.iResultDes.Copy(KConfigInvalid());
		    aResult.iResult = KErrNotExecuted;
		    }
		}
	REComSession::FinalClose();
	return error;
	}

TInt CMmfTsTone::TestCaseVolumeL(CStifSectionParser *section , TTestResult &aResult) 
	{
	TBool UsingDefault=EFalse;
	TInt Frequency1, Frequency2;
	TTimeIntervalMicroSeconds Duration1, Duration2;
	TObjectCountDown counter;
	aResult.iResult=KErrNone;
	
	Frequency1=Frequency2=KDefaultToneFrequency;
	
	//Obtain the tone's frequency
	Frequency1 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone frequency for player1") );}
	Frequency2 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency, ETrue);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone frequency for player2") );}

	Duration1 = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone duration for player1") );}
	Duration2 = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration, ETrue);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone duration for player2") );}

	iOutputPrinter.Printf(_L("Creating Tone Player1 with frequency (%d) and duration (%d)") , Frequency1, Duration1);
	CTonePlayer *Player1 = CTonePlayer::NewLC(&iOutputPrinter, Frequency1, Duration1);
	Player1->SetId(1);
	Player1->SetSupervisor(&counter);

	iOutputPrinter.Printf(_L("Creating Tone Player2 with frequency (%d) and duration (%d)") , Frequency2, Duration2);
	CTonePlayer *Player2 = CTonePlayer::NewLC(&iOutputPrinter, Frequency2, Duration2, EFalse);
	Player2->SetId(2);
	Player2->SetSupervisor(&counter);

	CParameters *PlayParameters = new(ELeave) CParameters(CTonePlayer::KPlayerActionPlay);
	CleanupStack::PushL(PlayParameters);
	Player1->SetExitCompleteTarget(Player2, PlayParameters);

	TInt MaxVolume1 = Player1->GetMaxVolume();
	TInt MaxVolume2 = Player2->GetMaxVolume();

	TInt Volume1 = MaxVolume1;
	TInt Volume2 = MaxVolume2 / 2;

	Player1->SetVolume(Volume1);
	Player2->SetVolume(Volume2);

	CActiveScheduler::Start();

	iOutputPrinter.Printf( _L("Player 1: Max (%d) Set (%d) Read (%d)") , MaxVolume1, Volume1, Player1->GetVolume() );
	iOutputPrinter.Printf( _L("Player 2: Max (%d) Set (%d) Read (%d)") , MaxVolume2, Volume2, Player2->GetVolume() );

	if (Volume1 != Player1->GetVolume() )
		{
		aResult.iResult = KErrExpectedValueDifferent;
		iOutputPrinter.Printf(_L("Volume for Player1 different than volume setup"));
		return KErrExecuted;
		}
	if (Volume2 != Player2->GetVolume() )
		{
		aResult.iResult = KErrExpectedValueDifferent;
		iOutputPrinter.Printf(_L("Volume for Player2 different than volume setup"));
		return KErrExecuted;
		}

	if (aResult.iResult == KErrExpectedValueDifferent) 
		{
		aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
		//_LIT(LDifferentValues, "volumes are different");
		iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
		}
	else
		{
		aResult.iResult = KErrExecuted;
		aResult.iResultDes.Copy(KTestCaseResultSuccess());
		//_LIT(LSuccess, "success");
		iOutputPrinter.Printf( KTestCaseResultFinished() ,  &KTestCaseResultSuccess() );
		}

	CleanupStack::PopAndDestroy(PlayParameters);
	CleanupStack::PopAndDestroy(Player2);
	CleanupStack::PopAndDestroy(Player1);

	return KErrExecuted;
	}

TInt CMmfTsTone::TestCaseVolumeRampL(CStifSectionParser *section , TTestResult &aResult) 
	{
	TBool UsingDefault=EFalse;
	TInt Frequency=KDefaultToneFrequency;
	TTimeIntervalMicroSeconds Duration, RampDuration;
	TTimeIntervalMicroSeconds32 Delay;
	
	aResult.iResult=KErrNone;
	
	//Obtain the tone's frequency
	Frequency = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone frequency for player") );}

	Duration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default tone duration for player") );}

	RampDuration = GetTimeIntervalL(section, KTagRampDuration, UsingDefault, KDefaultRampDuration);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default ramp duration for player") );}

	Delay = GetTimeIntervalL(section, KTagDelay, UsingDefault, KDefaultReportDelay);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default report delay for player") );}

	iOutputPrinter.Printf(_L("Creating Tone Player with frequency (%d) and duration (%d)") , Frequency, Duration);
	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter, Frequency, Duration);
	iOutputPrinter.Printf(_L("Setting a volume ramp of (%d) microseconds"), RampDuration);
	iOutputPrinter.Printf(_L("Volume sampled every (%d) microseconds"), Delay);

	Player->SetVolumeRamp(RampDuration);

	CParameters *ReportParams = new (ELeave) CParameters( CTonePlayer::KPlayerActionReportVolume );
	CleanupStack::PushL(ReportParams);
	//CMediaEvent *mEventReport = CMediaEvent::NewL(TestModuleIf() , *iLogger, Delay, Delay, Player, ReportParams);
	CMediaEvent *mEventReport = CMediaEvent::NewL(&iOutputPrinter, Delay, Delay, Player, ReportParams);
	CleanupStack::Pop(ReportParams);
	CleanupStack::PushL(mEventReport);

	CActiveScheduler::Start();

	if (mEventReport->GetCount() <= 0)
	{
		iOutputPrinter.Printf(_L("Not enought events"));
	}

	if (Player->GetFinalError() )
		{
		aResult.iResult = Player->GetFinalError();
		aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
		iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
		}
	else
		{
		aResult.iResult = KErrExecuted;
		aResult.iResultDes.Copy(KTestCaseResultSuccess());
		iOutputPrinter.Printf( KTestCaseResultFinished() ,  &KTestCaseResultSuccess() );
		}
	CleanupStack::PopAndDestroy(mEventReport);
	CleanupStack::PopAndDestroy(Player);

	return KErrExecuted;
	}


TInt CMmfTsTone::TestCaseRepeatL(CStifSectionParser *section , TTestResult &aResult)
	{
	CTonePlayer *Player;
	TBool UsingDefault=EFalse;
	RArray<TUint> RepeatArray;
	RArray<TTimeIntervalMicroSeconds> RepeatSilenceArray;
	RArray<TUint> FrequencyArray;
	RArray<TTimeIntervalMicroSeconds> DurationArray;

	RArray<TInt> ErrorArray;

	//Obtain the error range
	TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
	if (UsingDefault) {iOutputPrinter.Printf(_L("Using default error range") );}

	iOutputPrinter.Printf(_L("Reading list of repetition configuration"));
	TInt RepeatTimes = GetIntL(section, KTagRepeatTimes, UsingDefault, KDefaultRepeats);
	do {	//If the configuration is not found, at least add 1 default test
		RepeatArray.Append(RepeatTimes);
		iOutputPrinter.Printf(_L("Adding (%d) repetitions") , RepeatTimes);
	} while ( RepeatTimes=GetIntL(section, KTagRepeatTimes, UsingDefault, 1, ETrue) , !UsingDefault );

	iOutputPrinter.Printf(_L("Reading list of repetition silence intervals"));
	TTimeIntervalMicroSeconds RepeatSilence = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
	do {	//If the configuration is not found, at least add 1 default test
		RepeatSilenceArray.Append(RepeatSilence);
		iOutputPrinter.Printf(_L("Adding silence interval of (%d) microseconds") , RepeatSilence);
	} while ( RepeatSilence=GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration, ETrue) , !UsingDefault );

	// Reading Tones
	iOutputPrinter.Printf(_L("Reading list of tone frequencies"));
	TInt Frequency = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
	do {	//If the configuration is not found, at least add 1 default test
		FrequencyArray.Append(Frequency);
		iOutputPrinter.Printf(_L("Adding frequency (%d)") , Frequency);
	} while ( Frequency=GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency, ETrue) , !UsingDefault );

	iOutputPrinter.Printf(_L("Reading list of tone durations"));
	TTimeIntervalMicroSeconds Duration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
	do {	//If the configuration is not found, at least add 1 default test
		DurationArray.Append(Duration);
		iOutputPrinter.Printf(_L("Adding tone duration of (%d) microseconds") , Duration);
	} while ( Duration=GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration, ETrue) , !UsingDefault );

	TInt TestNum;
	RepeatSilence=KDefaultSilenceDuration;
	Frequency = KDefaultToneFrequency;

	TInt Error=KErrNone;
	TBool HadErrors=EFalse;
	for (TestNum=0; TestNum < RepeatArray.Count() && !Error ; ++TestNum) {
		Error = KErrNone;
		//setup the case for RepeatArray[TestNum] repeats
		RepeatTimes=RepeatArray[TestNum];
		if (TestNum < RepeatSilenceArray.Count() ) { RepeatSilence = RepeatSilenceArray[TestNum]; }
		if (TestNum < FrequencyArray.Count() ) { Frequency = FrequencyArray[TestNum]; }
		if (TestNum < DurationArray.Count() ) { Duration = DurationArray[TestNum];}
		//TestWithRepeats: RepeatTimes & RepeatSilence
		iOutputPrinter.Printf(_L("Creating Tone Player with frequency (%d) and duration (%d)") , Frequency, Duration);
		Player = CTonePlayer::NewLC(&iOutputPrinter, Frequency, Duration);
		iOutputPrinter.Printf(_L("Repeating (%d) times with silence intervals of (%d) microseconds") , RepeatTimes,RepeatSilence );
		Player->SetRepeats(RepeatTimes,RepeatSilence);
		CActiveScheduler::Start();
		//Error=Player->GetFinalError() ;
		
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if (!IsDurationInRange(RepeatTimes, Duration, RepeatSilence, ErrorRange, Player->GetPlayStartTime(), Player->GetPlayStopTime() ) )
			{
			Error = KErrExpectedValueDifferent;
			}
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//TODO: Calculate the expected duration taking into account the final silence (it should be present, according to SACF)
		if ( !Error) { Error=Player->GetFinalError(); }
		if ( Error ) { HadErrors = ETrue; }
		ErrorArray.Append(Error);

		CleanupStack::PopAndDestroy(Player);
	}

	//Do test with the last parameters, and interrupt it.
	if (!Error)
		{
		TTimeIntervalMicroSeconds32 Delay = GetTimeIntervalL(section, KTagDelay, UsingDefault, KDefaultStopDelay);
		//Player = CTonePlayer::NewLC();
		iOutputPrinter.Printf(_L("Creating Tone Player with frequency (%d) and duration (%d)") , Frequency, Duration);
		CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter, Frequency, Duration);
		iOutputPrinter.Printf(_L("Repeating (%d) times with silence intervals of (%d) microseconds") , RepeatTimes,RepeatSilence );
		Player->SetRepeats(RepeatTimes,RepeatSilence);

		CParameters *StopParams = new (ELeave) CParameters( CTonePlayer::KPlayerActionStop );
		CleanupStack::PushL(StopParams);
		//CMediaEvent *MEventStop = CMediaEvent::NewL(TestModuleIf() , *iLogger, Delay, Delay, Player, StopParams);
		CMediaEvent *MEventStop = CMediaEvent::NewL(&iOutputPrinter, Delay, Delay, Player, StopParams);
		CleanupStack::Pop(StopParams);
		CleanupStack::PushL(MEventStop);

		CActiveScheduler::Start();

		//TODO: Calculate the expected duration taking into account the final silence (it should be present, according to SACF)
		TTimeIntervalMicroSeconds ExpectedDuration = TInt64(Delay.Int());

		TTime StartTime = Player->GetPlayStartTime();
		TTime EndTimeRange1 = StartTime;
		EndTimeRange1+= ExpectedDuration;
		TTime EndTimeRange2 = EndTimeRange1;
		EndTimeRange1-=ErrorRange;
		EndTimeRange2+=ErrorRange;
		
		iOutputPrinter.Printf(_L("Play duration (%d)") , Player->GetPlayDuration() );
		iOutputPrinter.Printf(_L("Expected duration (%d)") , ExpectedDuration);
		TTime StopTime= Player->GetPlayStopTime();
		if (StopTime<EndTimeRange1 
			|| StopTime>EndTimeRange2) {
			Error = KErrExpectedValueDifferent;
		}
		if ( !Error) { Error=Player->GetFinalError(); }
		//if ( Player->GetFinalError() ) { Error = Player->GetFinalError(); }

		ErrorArray.Append(Error);

		CleanupStack::PopAndDestroy(MEventStop);
		CleanupStack::PopAndDestroy(Player);
		}

	RepeatArray.Close();
	RepeatSilenceArray.Close();
	FrequencyArray.Close();
	DurationArray.Close();
	ErrorArray.Close();

	if (Error == KErrExpectedValueDifferent)
		{
		aResult.iResult = Error;
		aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
		iOutputPrinter.Printf(KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
		return KErrExecuted;
		}
	else if (Error)
		{
		aResult.iResult = Error;
		aResult.iResultDes.Copy(KTestCaseResultFail());
		iOutputPrinter.Printf(KTestCaseResultFinished() , &KTestCaseResultFail() );
		return KErrExecuted;
		}
	else
		{
		aResult.iResult = KErrExecuted;
		aResult.iResultDes.Copy(KTestCaseResultSuccess());
		iOutputPrinter.Printf(KTestCaseResultFinished() , &KTestCaseResultSuccess() );
		return KErrExecuted;
		}
	}

TInt CMmfTsTone::TestCaseFixedSecuencesL(CStifSectionParser * /*section*/ , TTestResult &aResult) 
	{
	TInt Error=KErrNone;
	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);
	TInt FixedSequenceCount = Player->GetFixedSequenceCount();
	
	iOutputPrinter.Printf(_L("count of the sequence:%d"),FixedSequenceCount);
	for (TInt x=0; x<FixedSequenceCount && !Error; ++x)
		{
		const TDesC &Name = Player->GetFixedSequenceName(x);
		iOutputPrinter.Printf(_L("Name of sequence (%d) is %S") , x , &Name );
		Player->PrepareFixedSequence(x, ETrue);
		CActiveScheduler::Start();
		Error = Player->GetFinalError();
		}

	CleanupStack::PopAndDestroy(Player);

	if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	return KErrExecuted;
	}

TInt CMmfTsTone::TestCasePlayFileSequenceL(CStifSectionParser * section , TTestResult &aResult) 
	{
	TPtrC FileNamePtr;
	TPtrC DesFilePtr;
	TBool haveFileName = !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) ;
	TBool haveDesFileName = !section->GetLine(KTagSoundDes, DesFilePtr, ENoTag) ;
	if ( haveFileName && haveDesFileName )
		{
		TBool UsingDefault;
		TTimeIntervalMicroSeconds RepeatSilence = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		TTimeIntervalMicroSeconds ToneSequenceDurationDes = GetTimeIntervalL(section, KTagExpectedDurationDes, UsingDefault, KDefaultToneDuration);
		TTimeIntervalMicroSeconds ToneSequenceDuration = GetTimeIntervalL(section, KTagExpectedDuration, UsingDefault, KDefaultToneDuration);

		TFileName FileName = DesFilePtr;
		HBufC8 *Des = GetDescriptor8LC(FileName);
		FileName = FileNamePtr;

		TInt Error=KErrNone;
		iOutputPrinter.Printf(_L("Creating Tone Player") );
		CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);

		RArray<TUint> RepeatArray;
		{
		iOutputPrinter.Printf(_L("Reading list of repetition configuration"));
		TInt RepeatTimes = GetIntL(section, KTagRepeatTimes, UsingDefault, KDefaultRepeats);
		do {	//If the configuration is not found, at least add 1 default test
			RepeatArray.Append(RepeatTimes);
			iOutputPrinter.Printf(_L("Adding (%d) repetitions") , RepeatTimes);
		} while ( RepeatTimes=GetIntL(section, KTagRepeatTimes, UsingDefault, 1, ETrue) , !UsingDefault );
		}
		
	//	for (TInt x=0 ; x < RepeatArray.Count() ; ++x)
	//		{
			Player->PrepareFileSequence(FileName);
//			Player->SetRepeats(RepeatArray[x], RepeatSilence);
			CActiveScheduler::Start();
			Error = Player->GetFinalError();
//			if (Error) { break; }
//			if (!IsDurationInRange(RepeatArray[x], ToneSequenceDuration, RepeatSilence, ErrorRange, Player->GetPlayStartTime() , Player->GetPlayStopTime() ))
//				{ Error = KErrExpectedValueDifferent; break; }
			//check durations

			Player->PrepareDesSequence(*Des);
			//Player->SetRepeats(RepeatArray[x], RepeatSilence);
			CActiveScheduler::Start();
			Error = Player->GetFinalError();
//			if (Error) { break; }
			//check durations
//			if (!IsDurationInRange(RepeatArray[x], ToneSequenceDurationDes, RepeatSilence, ErrorRange, Player->GetPlayStartTime() , Player->GetPlayStopTime() ))
//				{ Error = KErrExpectedValueDifferent; break;}

	//		}

		CleanupStack::PopAndDestroy(Player);
		CleanupStack::PopAndDestroy(Des);
		
		if (Error==KErrExpectedValueDifferent) 
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
			}
		else if (Error)
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFail() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
			}
		else
			{
			aResult.iResult=KErrNone;
			aResult.iResultDes.Copy( KTestCaseResultSuccess() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
			}
		RepeatArray.Close();
		return KErrExecuted;
		}
	else
		{
		//File to play missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}




TInt CMmfTsTone::TestCaseBalanceL(CStifSectionParser * section , TTestResult &aResult) 
	{
	TBool UsingDefault;
	TInt Error=KErrNone;
	RArray<TInt> BalanceArray;

	// Reading Balance list
	{
	iOutputPrinter.Printf(_L("Reading list of balance settings"));
	TInt Balance = GetIntL(section, KTagBalance, UsingDefault, KDefaultToneFrequency);
	do {	//If the configuration is not found, at least add 1 default test
		BalanceArray.Append(Balance);
		iOutputPrinter.Printf(_L("Adding balance (%d)") , Balance);
	} while ( Balance=GetIntL(section, KTagBalance, UsingDefault, KDefaultToneFrequency, ETrue) , !UsingDefault );
	}

	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);
	TInt FixedSequenceCount = Player->GetFixedSequenceCount();
	iOutputPrinter.Printf(_L("Fixed sequence count is (%d)") , FixedSequenceCount );

	TInt FixedSequenceNumber = GetIntL(section, KTagSequenceNumber, UsingDefault, KDefaultSequenceNumber);
	const TDesC &Name = Player->GetFixedSequenceName(FixedSequenceNumber);
	iOutputPrinter.Printf(_L("Name of sequence (%d) is %S") , FixedSequenceNumber , &Name );
	
	for (TInt x=0 ; x<BalanceArray.Count() && !Error; ++x)
		{
		TInt Balance;
		Player->PrepareFixedSequence(FixedSequenceNumber);
		iOutputPrinter.Printf(_L("Setting balance to (%d)") , BalanceArray[x] );
		Player->SetBalanceL(BalanceArray[x]);
		CActiveScheduler::Start();
		Error = Player->GetFinalError();
		if (!Error) 
			{
			Balance = Player->GetBalanceL();
			iOutputPrinter.Printf(_L("Balance is set to (%d)") , Balance);
			if ( BalanceArray[x] != Balance)
				{
				iOutputPrinter.Printf(_L("Balance set is different from balance read"));
				Error = KErrExpectedValueDifferent;
				}
			}
		}

	CleanupStack::PopAndDestroy(Player);

	if (Error==KErrExpectedValueDifferent)
		{
		aResult.iResult=KErrExpectedValueDifferent;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
		}
	else if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	BalanceArray.Close();
	return KErrExecuted;
	}

TInt CMmfTsTone::TestCasePrioritiesL(CStifSectionParser * section , TTestResult &aResult) 
	{
	TBool UsingDefault;
	TInt Error=KErrNone;
	RArray<TInt> PrioritiesArray;

	//Read the priorities
	//Sort them from high to low

	TInt FixedSequenceNumber = GetIntL(section, KTagSequenceNumber, UsingDefault, KDefaultSequenceNumber);
	iOutputPrinter.Printf(_L("Fixed sequence number read (%d)") , FixedSequenceNumber);

	iOutputPrinter.Printf(_L("Reading list of priorities"));
	CStifItemParser *item = section->GetItemLineL(KTagPriority, ENoTag);
	if (item)
		{
		TInt Priority=0;
		TInt x=0;
		CleanupStack::PushL(item);

		Error = item->GetInt(KNullDesC, Priority);
		while (!Error && x < 3)
		{	
			PrioritiesArray.InsertInOrderAllowRepeats(Priority);
			Error = item->GetNextInt(Priority);
			++x;
		}
		//if (Error) { Error = KErrConfigInvalid; }
		Error = ( (PrioritiesArray.Count() < 3) ? KErrNotFound:KErrNone );
		CleanupStack::PopAndDestroy(item);
		}
	else
		{
		Error = KErrNotFound;
		}
	if (Error) 
		{
		PrioritiesArray.Close();
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KConfigInvalid() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KConfigInvalid() );
		return KErrNotExecuted;
		}

	TObjectCountDown counter;
	
	// We have 3 priorities sorted in PrioritiesArray
	CTonePlayer *PlayerHigh = CTonePlayer::NewLC(&iOutputPrinter);
	CTonePlayer *PlayerMed = CTonePlayer::NewLC(&iOutputPrinter);
	CTonePlayer *PlayerLow = CTonePlayer::NewLC(&iOutputPrinter);

	iOutputPrinter.Printf(_L("Creating PlayerLow with id (1) and priority (%d)") , PrioritiesArray[0] );
	PlayerLow->SetPriority( PrioritiesArray[0] , EMdaPriorityPreferenceNone);
	PlayerLow->SetId(1);
	PlayerLow->SetSupervisor(&counter);

	iOutputPrinter.Printf(_L("Creating PlayerMed with id (2) and priority (%d)") , PrioritiesArray[1] );
	PlayerMed->SetPriority( PrioritiesArray[1] , EMdaPriorityPreferenceNone);
	PlayerMed->SetId(2);
	PlayerMed->SetSupervisor(&counter);

	iOutputPrinter.Printf(_L("Creating PlayerLow with id (3) and priority (%d)") , PrioritiesArray[2] );
	PlayerHigh->SetPriority( PrioritiesArray[2] , EMdaPriorityPreferenceNone);
	PlayerHigh->SetId(3);
	PlayerHigh->SetSupervisor(&counter);

	//Test 1, Medium - Low - High
	iOutputPrinter.Printf(_L("Playing Med(2), Low(1), High(3)"));
	PlayerMed->PrepareFixedSequence(FixedSequenceNumber);
	PlayerLow->PrepareFixedSequence(FixedSequenceNumber);
	PlayerHigh->PrepareFixedSequence(FixedSequenceNumber);
	CActiveScheduler::Start();

	Error = PlayerHigh->GetFinalError();

	if (!Error)
		{
		iOutputPrinter.Printf(_L("Playing High(3), Low(1), Med(2)"));
		PlayerLow->SetSupervisor(&counter);
		PlayerMed->SetSupervisor(&counter);
		PlayerHigh->SetSupervisor(&counter);

		PlayerHigh->PrepareFixedSequence(FixedSequenceNumber);
		PlayerLow->PrepareFixedSequence(FixedSequenceNumber);
		PlayerMed->PrepareFixedSequence(FixedSequenceNumber);
		CActiveScheduler::Start();

		Error = PlayerHigh->GetFinalError();
		}
	if (!Error)
		{
		iOutputPrinter.Printf(_L("Playing Low(1), High(3), Med(2)"));
		PlayerLow->SetSupervisor(&counter);
		PlayerMed->SetSupervisor(&counter);
		PlayerHigh->SetSupervisor(&counter);

		PlayerLow->PrepareFixedSequence(FixedSequenceNumber);
		PlayerHigh->PrepareFixedSequence(FixedSequenceNumber);
		PlayerMed->PrepareFixedSequence(FixedSequenceNumber);
		CActiveScheduler::Start();

		Error = PlayerHigh->GetFinalError();
		}
	
	CleanupStack::PopAndDestroy(PlayerLow);
	CleanupStack::PopAndDestroy(PlayerMed);
	CleanupStack::PopAndDestroy(PlayerHigh);

	if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	PrioritiesArray.Close();
	return KErrExecuted;
	}

TInt CMmfTsTone::TestCaseDTMFStringL(CStifSectionParser * section , TTestResult &aResult) 
	{
	//aResult.Result=KErrLeft;
	TBool UsingDefault;
	TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
	TTimeIntervalMicroSeconds32 ToneDuration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultDTMFToneDuration);
	TTimeIntervalMicroSeconds32 ToneOffDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultDTMFSilenceDuration);
	TTimeIntervalMicroSeconds32 Pause = GetTimeIntervalL(section, KTagDelay, UsingDefault, KDefaultPauseDelay);
	TInt Error = KErrNone;
	HBufC *DTMFString;

	{
	TPtrC PtrDTMFString;
	if (section->GetLine(KTagDTMFString, PtrDTMFString, ENoTag) )
		{
		DTMFString = KDefaultDTMFString().AllocLC();
		}
	else
		{
		DTMFString = PtrDTMFString.AllocLC();
		}
	}

	iOutputPrinter.Printf(_L("Creating Tone Player") );
	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);
	iOutputPrinter.Printf(_L("Preparing DTMF sequence [%S]") , DTMFString);
	Player->PrepareDTMFSequence(*DTMFString);
	Player->SetDTMFLengths(ToneDuration, ToneOffDuration, Pause);

	iOutputPrinter.Printf(_L("Starting scheduler"));
	CActiveScheduler::Start();

	Error = Player->GetFinalError();

	TTime ExpectedEnd = Player->GetPlayStartTime();
	for (TInt x=0 ; x < DTMFString->Length() ; ++x)
		{
		if ( (*DTMFString)[x] == KPauseCharacter()[0] )
			{
			ExpectedEnd += Pause;
			}
		else
			{
			ExpectedEnd += ToneDuration;
			ExpectedEnd += ToneOffDuration;
			}
		}

	iOutputPrinter.Printf(_L("Expected duration: (%d)") , ExpectedEnd.MicroSecondsFrom(Player->GetPlayStartTime()) );
	iOutputPrinter.Printf(_L("Real duration: (%d)") , Player->GetPlayDuration() );

	TTime ExpectedEnd2 = ExpectedEnd;
	ExpectedEnd -= ErrorRange;
	ExpectedEnd2 += ErrorRange;

	TBool InRange = ExpectedEnd<Player->GetPlayStopTime() && Player->GetPlayStopTime()<ExpectedEnd2 ;
	if (!InRange)
		{
		Error = KErrExpectedValueDifferent;
		}

	CleanupStack::PopAndDestroy(Player);
	CleanupStack::PopAndDestroy(DTMFString);
	
	if (!InRange)
		{
		aResult.iResult = KErrExpectedValueDifferent;
		aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
		iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
		}
	else if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	return KErrExecuted;
	
	}

TInt CMmfTsTone::TestCaseDualToneL(CStifSectionParser * section , TTestResult &aResult) 
	{
	
	TBool UsingDefault;
	TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
	TTimeIntervalMicroSeconds ToneDuration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
	TInt Frequency1=KDefaultToneFrequency;
	TInt Frequency2=KDefaultToneFrequency;
	
	TInt Error = KErrNone;
	
    TInt ValidityCheck=GetIntL(section, KTagValidity, UsingDefault, KDefaultValidity);
	//Obtain the tone's frequency
	Frequency1 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
	if (UsingDefault)
		{
		iOutputPrinter.Printf(_L("Using default tone frequency for player") );
		}
	else
		{
		iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , Frequency1);
		}
	Frequency2 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency, ETrue);
	if (UsingDefault)
		{
		iOutputPrinter.Printf(_L("Using default tone frequency for player") );
		}
	else
		{
		iOutputPrinter.Printf(_L("Using frequency 2 for player (%d)") , Frequency2);
		}

	

	iOutputPrinter.Printf(_L("Creating Tone Player") );
	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);
	
	Player->PrepareDualTone(Frequency1, Frequency2, ToneDuration);

	iOutputPrinter.Printf(_L("Starting scheduler"));
	CActiveScheduler::Start();

	Error = Player->GetFinalError();

	TBool InRange=ETrue;
	if (!IsDurationInRange(0, ToneDuration, TTimeIntervalMicroSeconds(0), ErrorRange, Player->GetPlayStartTime(), Player->GetPlayStopTime() ) )
		{ //validity check=1 for invalid timeduration
		if(ValidityCheck!=1 && Error!=KErrArgument)
		    {
		    Error = KErrExpectedValueDifferent;
		    }
		
		InRange=EFalse;
		}


	CleanupStack::PopAndDestroy(Player);
	
	
	if (!InRange)
		{
		if(ValidityCheck==1)
		    {
		    if(Error==KErrArgument)
              {
               aResult.iResult=KErrNone;
               aResult.iResultDes.Copy( KTestCaseResultSuccess() );
               iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
              }
		    else
		       {
		        aResult.iResult=Error;
		        aResult.iResultDes.Copy( KTestCaseResultFail() );
		        iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		       }
		    }
		else
		    {
		aResult.iResult = KErrExpectedValueDifferent;
		aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
		iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
		
		}
	}
	//validity check=2,for invalid frequencies
	else if(ValidityCheck==2)
	    {
	      if(Error==KErrArgument)
	        {
               aResult.iResult=KErrNone;
               aResult.iResultDes.Copy( KTestCaseResultSuccess() );
               iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
            }
          else
           {
                aResult.iResult=Error;
                aResult.iResultDes.Copy( KTestCaseResultFail() );
                iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
           }
	    }
	
	else if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	return KErrExecuted;
	
	}

TInt CMmfTsTone::TestCaseCancelPrepareL(CStifSectionParser *section , TTestResult &aResult)
	{
	TBool UsingDefault, NotUsingCancel;
	TInt PrepareWhat = GetIntL(section, KTagPrepare, UsingDefault, 1);
	TTimeIntervalMicroSeconds32 TimeOutDuration = GetTimeIntervalL(section, KTagTimeOut, UsingDefault, KDefaultTimeOutDuration);
	TInt Error = KErrNone;
	TInt CancelPriority = GetIntL(section, KTagCancelPriority, NotUsingCancel, CActive::EPriorityHigh);
	HBufC8 *Des=0;

	CTonePlayer *Player=0;
	CImmediateMediaEvent *PrepareMediaEvent=0;
	CParameters *PrepareParameters=0;

	switch (PrepareWhat) {
		case CTonePlayer::EPrepareTone:
			{
			//Obtain the tone's frequency
			TInt Frequency = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
			if (UsingDefault)
				{
				iOutputPrinter.Printf(_L("Using default tone frequency (%d) for player") , Frequency);
				}
			else
				{
				iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , Frequency);
				}
			TTimeIntervalMicroSeconds ToneDuration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
			iOutputPrinter.Printf(_L("Creating Tone Player") );
			Player = CTonePlayer::NewLC(&iOutputPrinter);
			
			TFixedArray<TInt64, 2> FreqAndDuration;
			FreqAndDuration[0] = Frequency;
			FreqAndDuration[1] = ToneDuration.Int64() ;
			PrepareParameters = CTemplateParameters<TFixedArray<TInt64, 2> >::NewL(CTonePlayer::KPlayerActionPrepareTone, FreqAndDuration);
			CleanupStack::PushL(PrepareParameters);

			//PrepareMediaEvent = CImmediateMediaEvent::NewLC(TestModuleIf(), *iLogger, Player, PrepareParameters, CancelPriority+1);
			PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
			PrepareMediaEvent->FireExecute();
			break;
			}

		case CTonePlayer::EPrepareFixedSequence:
			{
			TInt SequenceNumber = GetIntL(section, KTagSequenceNumber, UsingDefault, 0);
			Player = CTonePlayer::NewLC(&iOutputPrinter);
			//Player->PrepareFixedSequence(SequenceNumber);

			PrepareParameters = CTemplateParameters<TInt>::NewL(CTonePlayer::KPlayerActionPrepareFixedSequence, SequenceNumber);
			CleanupStack::PushL(PrepareParameters);

			PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
			PrepareMediaEvent->FireExecute();
			break;
			}

		case CTonePlayer::EPrepareFileSequence:
			{
			TPtrC FileNamePtr;
			TBool haveFileName = !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag);
			if (haveFileName)
				{
				Player = CTonePlayer::NewLC(&iOutputPrinter);

				TFileName a = FileNamePtr;
				PrepareParameters = CTemplateParameters<TFileName>::NewL(CTonePlayer::KPlayerActionPrepareFileSequence, a);
				CleanupStack::PushL(PrepareParameters);
				
				PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
				PrepareMediaEvent->FireExecute();
				}
			else
				{
				iOutputPrinter.Printf(_L("File Name is required"));
				}
			break;
			}
		case CTonePlayer::EPrepareDesSequence:
			{
			TPtrC FileNamePtr;
			TBool haveFileName = !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag);
			if (haveFileName)
				{
				Des = GetDescriptor8LC(FileNamePtr);
				Player = CTonePlayer::NewLC(&iOutputPrinter);

				PrepareParameters = CTemplateParameters<HBufC8*>::NewL(CTonePlayer::KPlayerActionPrepareDesSequence, Des);
				CleanupStack::PushL(PrepareParameters);
				
				PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
				PrepareMediaEvent->FireExecute();
				}
			else
				{
				iOutputPrinter.Printf(_L("File Name is required"));
				}
			break;
			}
		case CTonePlayer::EPrepareDTMF:
			{
			TTimeIntervalMicroSeconds32 ToneDuration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultDTMFToneDuration);
			TTimeIntervalMicroSeconds32 ToneOffDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultDTMFSilenceDuration);
			TTimeIntervalMicroSeconds32 Pause = GetTimeIntervalL(section, KTagDelay, UsingDefault, KDefaultPauseDelay, ETrue);

			iOutputPrinter.Printf(_L("Creating Tone Player") );
			Player = CTonePlayer::NewLC(&iOutputPrinter);
			{
			TPtrC PtrDTMFString;
			if (section->GetLine(KTagDTMFString, PtrDTMFString, ENoTag) )
				{
				PrepareParameters = CDTMFParameters::NewLC(CTonePlayer::KPlayerActionPrepareDTMFSequence, ToneDuration, ToneOffDuration, Pause, KDefaultDTMFString);
				}
			else
				{
				PrepareParameters = CDTMFParameters::NewLC(CTonePlayer::KPlayerActionPrepareDTMFSequence, ToneDuration, ToneOffDuration, Pause, PtrDTMFString);
				}
			}
			iOutputPrinter.Printf(_L("Preparing DTMF sequence [%S]") , static_cast<CDTMFParameters*>(PrepareParameters)->GetDTMFString() );
			PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
			PrepareMediaEvent->FireExecute();
			break;
			}
		case CTonePlayer::EPrepareDualTone:
			{
			//Obtain the tone's frequency
			TInt Frequency1 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency);
			if (UsingDefault)
				{
				iOutputPrinter.Printf(_L("Using default tone frequency 1 for player (%d)") , Frequency1);
				}
			else
				{
				iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , Frequency1);
				}
			TInt Frequency2 = GetIntL(section, KTagToneFrequency, UsingDefault, KDefaultToneFrequency, ETrue);
			if (UsingDefault)
				{
				iOutputPrinter.Printf(_L("Using default tone frequency 2 for player (%d) ") , Frequency2);
				}
			else
				{
				iOutputPrinter.Printf(_L("Using frequency 2 for player (%d)") , Frequency2);
				}
			TTimeIntervalMicroSeconds ToneDuration = GetTimeIntervalL(section, KTagToneDuration, UsingDefault, KDefaultToneDuration);
			iOutputPrinter.Printf(_L("Using duration (%d) for player"), ToneDuration);
			iOutputPrinter.Printf(_L("Creating Tone Player") );
			Player = CTonePlayer::NewLC(&iOutputPrinter);

			TFixedArray<TInt64, 3> FreqAndDuration;
			FreqAndDuration[0] = Frequency1;
			FreqAndDuration[1] = Frequency2;
			FreqAndDuration[2] = ToneDuration.Int64() ;
			PrepareParameters = CTemplateParameters<TFixedArray<TInt64, 3> >::NewL(CTonePlayer::KPlayerActionPrepareDualTone, FreqAndDuration);
			CleanupStack::PushL(PrepareParameters);

			PrepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, PrepareParameters, CancelPriority+1);
			PrepareMediaEvent->FireExecute();
			break;
			}
	}
	if (!Player)
		{
		iOutputPrinter.Printf(_L("Tone player not created properly"));
		aResult.iResult=KErrNotFound;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		return KErrNotExecuted;
		}
	else
		{
		//Player created succesfully, testing
		
		// Add Timeout to check that MatoComplete in Player is not called without hanging the test
		CParameters *TimeOutParams = new(ELeave) CParameters(CTonePlayer::KPlayerActionStop);
		CleanupStack::PushL(TimeOutParams);
		CMediaEvent *TimeOut = CMediaEvent::NewLC(&iOutputPrinter, TimeOutDuration, TimeOutDuration, Player, TimeOutParams);

		// Add object to cancel preparation
		CParameters *params = new(ELeave) CParameters(CTonePlayer::KPlayerActionCancelPrepare);
		CleanupStack::PushL(params);
		CImmediateMediaEvent *ime = CImmediateMediaEvent::NewLC(&iOutputPrinter, Player, params, CancelPriority );
		if (!NotUsingCancel) ime->FireExecute();
		
		iOutputPrinter.Printf(_L("Starting scheduler"));
		CActiveScheduler::Start();

		Error = Player->GetFinalError();
		TInt CompleteCallbackCount = Player->GetPrepareCompleteCallbackCount();
		iOutputPrinter.Printf(_L("MatoPrepareCompleteCallback was called (%d) times, last error was (%d)"), CompleteCallbackCount, Player->GetLastPrepareCompleteError());
		iOutputPrinter.Printf(_L("TimeOut was called (%d) times"), TimeOut->GetCount() );

		CleanupStack::PopAndDestroy(ime);
		CleanupStack::Pop(params);
		CleanupStack::PopAndDestroy(TimeOut);
		CleanupStack::Pop(TimeOutParams);

		
		/*if (PrepareMediaEvent)*/ CleanupStack::PopAndDestroy(PrepareMediaEvent);
		/*if (PrepareParameters)*/ CleanupStack::Pop(PrepareParameters);
		CleanupStack::PopAndDestroy(Player);
		if (Des) CleanupStack::PopAndDestroy(Des);
		
		if (CompleteCallbackCount > 0) 
			{
			aResult.iResult=KErrExpectedValueDifferent;
			aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
			iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
			}
		else if (Error)
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFail() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
			}
		else
			{
			aResult.iResult=KErrNone;
			aResult.iResultDes.Copy( KTestCaseResultSuccess() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
			}
		return KErrExecuted;
		}
	}

TInt CMmfTsTone::TestCasePlayToneFileL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TBool haveFileName = !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) ;
	if ( haveFileName )
		{
		TBool UsingDefaultErrorRange;
		TBool UsingDefaultExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefaultErrorRange, KDefaultErrorRange);
		TTimeIntervalMicroSeconds ToneSequenceDuration = GetTimeIntervalL(section, KTagExpectedDuration, UsingDefaultExpectedDuration, KDefaultToneDuration);

		TFileName FileName = FileNamePtr;

		TInt Error=KErrNone;
		iOutputPrinter.Printf(_L("Creating Tone Player") );
		CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);

		Player->PrepareFileSequence(FileName);
		CActiveScheduler::Start();
		Error = Player->GetFinalError();

		if (!UsingDefaultExpectedDuration && !Error) 
			{
			iOutputPrinter.Printf(_L("Comparing duration with expected duration") );
			if (!IsDurationInRange(0, ToneSequenceDuration, TTimeIntervalMicroSeconds(0), ErrorRange, Player->GetPlayStartTime() , Player->GetPlayStopTime() ))
				{
				Error = KErrExpectedValueDifferent; 
				}
			}

		CleanupStack::PopAndDestroy(Player);
		
		if (Error==KErrExpectedValueDifferent) 
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
			}
		else if (Error)
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFail() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
			}
		else
			{
			aResult.iResult=KErrNone;
			aResult.iResultDes.Copy( KTestCaseResultSuccess() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
			}
		//RepeatArray.Close();
		return KErrExecuted;
		}
	else
		{
		//File to play missing
		iOutputPrinter.Printf(_L("Filename to play not configured"));
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsTone::TestCasePlayToneRFileL(CStifSectionParser *section , TTestResult &aResult)   
	{
	TPtrC FileNamePtr;
	TBool haveFileName = !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) ;
	if ( haveFileName )
		{
		TBool UsingDefaultErrorRange;
		TBool UsingDefaultExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefaultErrorRange, KDefaultErrorRange);
		TTimeIntervalMicroSeconds ToneSequenceDuration = GetTimeIntervalL(section, KTagExpectedDuration, UsingDefaultExpectedDuration, KDefaultToneDuration);

		TFileName FileName = FileNamePtr;

		TInt Error=KErrNone;
		iOutputPrinter.Printf(_L("Creating Tone Player") );
		CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);

		Player->PrepareRFileSequence(FileName);
		CActiveScheduler::Start();
		Error = Player->GetFinalError();

		if (!UsingDefaultExpectedDuration && !Error) 
			{
			iOutputPrinter.Printf(_L("Comparing duration with expected duration") );
			if (!IsDurationInRange(0, ToneSequenceDuration, TTimeIntervalMicroSeconds(0), ErrorRange, Player->GetPlayStartTime() , Player->GetPlayStopTime() ))
				{
				Error = KErrExpectedValueDifferent; 
				}
			}

		CleanupStack::PopAndDestroy(Player);
		
		if (Error==KErrExpectedValueDifferent) 
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
			}
		else if (Error)
			{
			aResult.iResult=Error;
			aResult.iResultDes.Copy( KTestCaseResultFail() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
			}
		else
			{
			aResult.iResult=KErrNone;
			aResult.iResultDes.Copy( KTestCaseResultSuccess() );
			iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
			}
		//RepeatArray.Close();
		return KErrExecuted;
		}
	else
		{
		//File to play missing
		iOutputPrinter.Printf(_L("Filename to play not configured"));
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}
	
	
TInt CMmfTsTone::TestCaseCustomInterfaceL(CStifSectionParser * /*section*/ , TTestResult &aResult) 
	{
	TInt Error=KErrNone;
	CTonePlayer *Player = CTonePlayer::NewLC(&iOutputPrinter);
	
	
	TInt FixedSequenceCount = Player->GetFixedSequenceCount();
	for (TInt x=0; x<FixedSequenceCount && !Error; ++x)
		{
		const TDesC &Name = Player->GetFixedSequenceName(x);
		iOutputPrinter.Printf(_L("Name of sequence (%d) is %S") , x , &Name );
		Player->PrepareFixedSequenceCustomInterface(x, ETrue);
		CActiveScheduler::Start();
		Error = Player->GetFinalError();
		}

	CleanupStack::PopAndDestroy(Player);

	if (Error)
		{
		aResult.iResult=Error;
		aResult.iResultDes.Copy( KTestCaseResultFail() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
		}
	else
		{
		aResult.iResult=KErrNone;
		aResult.iResultDes.Copy( KTestCaseResultSuccess() );
		iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
		}
	return KErrExecuted;
	}


TInt CMmfTsTone::TestCaseMaxVolumePlayL(CStifSectionParser *section , TTestResult &aResult) 
    {
        TPtrC fileNamePtr;
        TPtrC desFilePtr;
        TBool haveFileName = !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) ;
        TBool haveDesFileName = !section->GetLine(KTagSoundDes, desFilePtr, ENoTag) ;
        //TTimeIntervalMicroSeconds ToneDuration;
        if ( haveFileName && haveDesFileName )
            {
            TBool usingDefault;
            TTimeIntervalMicroSeconds repeatSilence = GetTimeIntervalL(section, KTagSilenceDuration, usingDefault, KDefaultSilenceDuration);
            TTimeIntervalMicroSeconds errorRange = GetTimeIntervalL(section, KTagErrorRange, usingDefault, KDefaultErrorRange);
            TTimeIntervalMicroSeconds toneSequenceDurationDes = GetTimeIntervalL(section, KTagExpectedDurationDes, usingDefault, KDefaultToneDuration);
            TTimeIntervalMicroSeconds toneSequenceDuration = GetTimeIntervalL(section, KTagExpectedDuration, usingDefault, KDefaultToneDuration);

            TFileName fileName = desFilePtr;
            HBufC8 *des = GetDescriptor8LC(fileName);
            fileName = fileNamePtr;

            TInt error1=KErrNone,error2=KErrNone;
            iOutputPrinter.Printf(_L("Creating Tone Player") );
            CTonePlayer *Player1 = CTonePlayer::NewLC(&iOutputPrinter);
            CTonePlayer *Player2 = CTonePlayer::NewLC(&iOutputPrinter);
            TInt maxVolume1 = Player1->GetMaxVolume();
            TInt currentVolume2=Player2->GetVolume();
            TInt volume1 = maxVolume1;
            TInt volume2=currentVolume2;
                    
            Player1->SetVolume(volume1);

            iOutputPrinter.Printf( _L("Player 1: Max (%d) Set (%d) Read (%d)") , maxVolume1, volume1, Player1->GetVolume() );
            iOutputPrinter.Printf( _L("Player 2: Max (%d) Set (%d) Read (%d)") , currentVolume2, volume2, Player2->GetVolume() );
                    
            Player1->PrepareDesSequence(*des);
            CActiveScheduler::Start();
            error1 = Player1->GetFinalError();
                    
            Player2->PrepareDesSequence(*des);
            CActiveScheduler::Start();
            error2 = Player2->GetFinalError();
       

        
            
        if (volume1 != Player1->GetVolume() )
            {
            aResult.iResult = KErrExpectedValueDifferent;
            iOutputPrinter.Printf(_L("Volume for Player1 different than volume setup"));
            return KErrExecuted;
            }
         

       
        if (error1==KErrExpectedValueDifferent) 
            {
            aResult.iResult=error1;
            aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
            }
        else if (error1)
            {
            aResult.iResult=error1;
            aResult.iResultDes.Copy( KTestCaseResultFail() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
            }
        else if(error2==KErrExpectedValueDifferent)
            
            {
            aResult.iResult=error2;
            aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
            }
        else if (error2)
            {
            aResult.iResult=error2;
            aResult.iResultDes.Copy( KTestCaseResultFail() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
            }
        else
            {
            aResult.iResult=KErrNone;
            aResult.iResultDes.Copy( KTestCaseResultSuccess() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
            }
        CleanupStack::PopAndDestroy(Player2);
        CleanupStack::PopAndDestroy(Player1); 
        if(des) CleanupStack::PopAndDestroy(des);
        
        return KErrExecuted;
        }
    else
        {
        //File to play missing
        iOutputPrinter.Printf(_L("Filename to play not configured"));
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        }
    
}
TInt CMmfTsTone::TestCaseCancelPlayL(CStifSectionParser *section , TTestResult &aResult) 
{
    TBool usingDefault, notUsingCancel;
    TInt playWhat = GetIntL(section, KTagPlay, usingDefault, 1);
    TTimeIntervalMicroSeconds32 timeOutDuration = GetTimeIntervalL(section, KTagTimeOut, usingDefault, KDefaultTimeOutDuration);
    TInt error = KErrNone;
    TInt cancelPriority = GetIntL(section, KTagCancelPriority, notUsingCancel, CActive::EPriorityHigh);
    HBufC8 *des=0;
    TTimeIntervalMicroSeconds toneDuration;
    CTonePlayer *player=0;
    CImmediateMediaEvent *prepareMediaEvent=0;
    CParameters *prepareParameters=0;
    TTimeIntervalMicroSeconds32 delay = GetTimeIntervalL(section, KTagDelay, usingDefault, KDefaultReportDelay);;
    
    switch (playWhat) {
        case CTonePlayer::EPlayTone:
            {
            //Obtain the tone's frequency
            TInt frequency = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency);
            if (usingDefault)
                {
                iOutputPrinter.Printf(_L("Using default tone frequency (%d) for player") , frequency);
                }
            else
                {
                iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , frequency);
                }
            TTimeIntervalMicroSeconds toneDuration = GetTimeIntervalL(section, KTagToneDuration, usingDefault, KDefaultToneDuration);
            iOutputPrinter.Printf(_L("Creating Tone Player") );
            player = CTonePlayer::NewLC(&iOutputPrinter);
            
            TFixedArray<TInt64, 2> freqAndDuration;
            freqAndDuration[0] = frequency;
            freqAndDuration[1] = toneDuration.Int64() ;
           
            
            prepareParameters = CTemplateParameters<TFixedArray<TInt64, 2> >::NewL(CTonePlayer::KPlayerActionPrepareTone, freqAndDuration);
            CleanupStack::PushL(prepareParameters);

            //PrepareMediaEvent = CImmediateMediaEvent::NewLC(TestModuleIf(), *iLogger, Player, PrepareParameters, CancelPriority+1);
            prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
            prepareMediaEvent->FireExecute();
            break;
            }
        case CTonePlayer::EPlayFixedSequence:
            {
            TInt sequenceNumber = GetIntL(section, KTagSequenceNumber, usingDefault, 0);
          
            player = CTonePlayer::NewLC(&iOutputPrinter);
            
            prepareParameters = CTemplateParameters<TInt>::NewL(CTonePlayer::KPlayerActionPrepareFixedSequence, sequenceNumber);
            CleanupStack::PushL(prepareParameters);

            prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
            prepareMediaEvent->FireExecute();
            break;
            }

        case CTonePlayer::EPlayFileSequence:
            {
            TPtrC fileNamePtr;
          
            TBool haveFileName = !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag);
            if (haveFileName)
                {
                player = CTonePlayer::NewLC(&iOutputPrinter);

                TFileName file = fileNamePtr;
                
                prepareParameters = CTemplateParameters<TFileName>::NewL(CTonePlayer::KPlayerActionPrepareFileSequence, file);
                CleanupStack::PushL(prepareParameters);
                
                prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
                prepareMediaEvent->FireExecute();
                }
            else
                {
                iOutputPrinter.Printf(_L("File Name is required"));
                }
            break;
            }
        case CTonePlayer::EPlayDesSequence:
            {
            TPtrC fileNamePtr;
          
            TBool haveFileName = !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag);
            if (haveFileName)
                {
                des = GetDescriptor8LC(fileNamePtr);
                player = CTonePlayer::NewLC(&iOutputPrinter);
                
                prepareParameters = CTemplateParameters<HBufC8*>::NewL(CTonePlayer::KPlayerActionPrepareDesSequence, des);
                CleanupStack::PushL(prepareParameters);
                
                prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
               
                prepareMediaEvent->FireExecute();
                }
            else
                {
                iOutputPrinter.Printf(_L("File Name is required"));
                }
            break;
            }
        case CTonePlayer::EPlayDTMF:
            {
            TTimeIntervalMicroSeconds32 toneDuration = GetTimeIntervalL(section, KTagToneDuration, usingDefault, KDefaultDTMFToneDuration);
            TTimeIntervalMicroSeconds32 toneOffDuration = GetTimeIntervalL(section, KTagSilenceDuration, usingDefault, KDefaultDTMFSilenceDuration);
            TTimeIntervalMicroSeconds32 pause = GetTimeIntervalL(section, KTagDelay, usingDefault, KDefaultPauseDelay, ETrue);
            
            iOutputPrinter.Printf(_L("Creating Tone Player") );
            player = CTonePlayer::NewLC(&iOutputPrinter);
            {
            TPtrC PtrDTMFString;
            if (section->GetLine(KTagDTMFString, PtrDTMFString, ENoTag) )
                {
                prepareParameters = CDTMFParameters::NewLC(CTonePlayer::KPlayerActionPrepareDTMFSequence, toneDuration, toneOffDuration, pause, KDefaultDTMFString);
                }
            else
                {
                prepareParameters = CDTMFParameters::NewLC(CTonePlayer::KPlayerActionPrepareDTMFSequence, toneDuration, toneOffDuration, pause, PtrDTMFString);
                }
            }
            iOutputPrinter.Printf(_L("Preparing DTMF sequence [%S]") , static_cast<CDTMFParameters*>(prepareParameters)->GetDTMFString() );
            prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
            prepareMediaEvent->FireExecute();
            break;
            }
        case CTonePlayer::EPlayDualTone:
            {
            //Obtain the tone's frequency
            TInt frequency1 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency);
            if (usingDefault)
                {
                iOutputPrinter.Printf(_L("Using default tone frequency 1 for player (%d)") , frequency1);
                }
            else
                {
                iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , frequency1);
                }
            TInt frequency2 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency, ETrue);
            if (usingDefault)
                {
                iOutputPrinter.Printf(_L("Using default tone frequency 2 for player (%d) ") , frequency2);
                }
            else
                {
                iOutputPrinter.Printf(_L("Using frequency 2 for player (%d)") , frequency2);
                }
            toneDuration = GetTimeIntervalL(section, KTagToneDuration, usingDefault, KDefaultToneDuration);
            iOutputPrinter.Printf(_L("Using duration (%d) for player"), toneDuration);
            iOutputPrinter.Printf(_L("Creating Tone Player") );
            player = CTonePlayer::NewLC(&iOutputPrinter);

            TFixedArray<TInt64, 3> freqAndDuration;
            freqAndDuration[0] = frequency1;
            freqAndDuration[1] = frequency2;
            freqAndDuration[2] = toneDuration.Int64() ;
           
            
            prepareParameters = CTemplateParameters<TFixedArray<TInt64, 3> >::NewL(CTonePlayer::KPlayerActionPrepareDualTone, freqAndDuration);
            CleanupStack::PushL(prepareParameters);

            prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, cancelPriority+1);
            prepareMediaEvent->FireExecute();
         
            break;
            }
            
    }
            if (!player)
                {
                iOutputPrinter.Printf(_L("Tone player not created properly"));
                aResult.iResult=KErrNotFound;
                aResult.iResultDes.Copy( KTestCaseResultFail() );
                iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
                return KErrNotExecuted;
                }
            else
                {

                CParameters *params = new (ELeave) CParameters( CTonePlayer::KPlayerActionStop );
                CleanupStack::PushL(params);
                CMediaEvent *ime = CMediaEvent::NewL(&iOutputPrinter,delay,0, player, params);
                CleanupStack::PushL(ime);
          
                
                iOutputPrinter.Printf(_L("Starting scheduler"));
                CActiveScheduler::Start();
                error=player->GetFinalError();
                CleanupStack::PopAndDestroy(ime);
                CleanupStack::Pop(params);
        
                                                                                                                                                                                                                                                                                                                                                                                                               
              
                
                /*if (PrepareMediaEvent)*/ CleanupStack::PopAndDestroy(prepareMediaEvent);
                /*if (PrepareParameters)*/ CleanupStack::Pop(prepareParameters);
                CleanupStack::PopAndDestroy(player);
                if (des) CleanupStack::PopAndDestroy(des);
               
                

                
                if (error==KErrExpectedValueDifferent) 
                    {
                    aResult.iResult=error;
                    aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
                    iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
                    }
                else if (error)
                    {
                    aResult.iResult=error;
                    aResult.iResultDes.Copy( KTestCaseResultFail() );
                    iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
                    }
                else
                    {
                    aResult.iResult=KErrNone;
                    aResult.iResultDes.Copy( KTestCaseResultSuccess() );
                    iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
                    }
                
                
                return KErrExecuted;
                }
            
    }


TInt CMmfTsTone::TestCaseVolumeChangeL(CStifSectionParser *section , TTestResult &aResult) 
    {   
        TBool notUsingCancel;
        TTimeIntervalMicroSeconds32 delay;
      
        TInt volumeLevel=GetIntL(section,KTagVolume,notUsingCancel,1);
        HBufC8 *des=0;
    
        CTonePlayer *player=0;
        CImmediateMediaEvent *prepareMediaEvent=0;
        CParameters *prepareParameters=0;
        
        TBool usingDefault;
        TTimeIntervalMicroSeconds errorRange = GetTimeIntervalL(section, KTagErrorRange, usingDefault, KDefaultErrorRange);
        TTimeIntervalMicroSeconds toneDuration = GetTimeIntervalL(section, KTagToneDuration, usingDefault, KDefaultToneDuration);
        TInt frequency1=KDefaultToneFrequency;
        TInt frequency2=KDefaultToneFrequency;
        delay = GetTimeIntervalL(section, KTagDelay, usingDefault, KDefaultReportDelay);
       
        TInt error = KErrNone;
       

        //Obtain the tone's frequency
        frequency1 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency);
        if (usingDefault)
            {
            iOutputPrinter.Printf(_L("Using default tone frequency for player") );
            }
        else
            {
            iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , frequency1);
            }
        frequency2 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency, ETrue);
        if (usingDefault)
            {
            iOutputPrinter.Printf(_L("Using default tone frequency for player") );
            }
        else
            {
            iOutputPrinter.Printf(_L("Using frequency 2 for player (%d)") , frequency2);
            }
        iOutputPrinter.Printf(_L("Creating Tone Player") );
            
          
        iOutputPrinter.Printf(_L("Using duration (%d) for player"), toneDuration);
        iOutputPrinter.Printf(_L("Creating Tone Player") );
        player = CTonePlayer::NewLC(&iOutputPrinter);

        TFixedArray<TInt64, 3> freqAndDuration;
        freqAndDuration[0] = frequency1;
        freqAndDuration[1] = frequency2;
        freqAndDuration[2] = toneDuration.Int64() ;
        
       
        prepareParameters = CTemplateParameters<TFixedArray<TInt64, 3> >::NewL(CTonePlayer::KPlayerActionPrepareDualTone, freqAndDuration);
        CleanupStack::PushL(prepareParameters);

        prepareMediaEvent = CImmediateMediaEvent::NewLC(&iOutputPrinter, player, prepareParameters, 80);
        prepareMediaEvent->FireExecute();
                     
        iOutputPrinter.Printf(_L("Starting scheduler"));
        //volumelevel=11(max volume),=12(average volume),=13(zero volume)        
        CParameters *reportParams = new (ELeave) CParameters( volumeLevel );
        CleanupStack::PushL(reportParams);
       
       
        CMediaEvent *ime = CMediaEvent::NewL(&iOutputPrinter,delay,0, player, reportParams);
                          
        CleanupStack::PushL(ime);
        TInt expectedVolume;
        iOutputPrinter.Printf(_L("current volume before change:%d"),player->GetVolume());
        RDebug::Print(_L("current volume before change:%d"),player->GetVolume());
        switch(volumeLevel)
            {
            case EMaxVolume:
                {
                expectedVolume=player->GetMaxVolume();
                break;
                }
            case EAvgVolume:
                {
                expectedVolume=(player->GetMaxVolume())/2;
                break;
                }
            case EZeroVolume:
                {
                expectedVolume=0;
                break;
                }
            }
        iOutputPrinter.Printf(_L("Starting scheduler"));
        CActiveScheduler::Start();
        error=player->GetFinalError();
        RDebug::Print(_L("current volume after change:%d"),player->GetVolume());
        iOutputPrinter.Printf(_L("current volume after change:%d"),player->GetVolume());
        if (player->GetVolume()!=expectedVolume)
            {
            error=KErrExpectedValueDifferent;
            }
        CleanupStack::PopAndDestroy(ime);
        CleanupStack::Pop(reportParams);

    if (error )
        {
        aResult.iResult = error;
        aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
        iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
        }
    else
        {
        aResult.iResult = KErrExecuted;
        aResult.iResultDes.Copy(KTestCaseResultSuccess());
        iOutputPrinter.Printf( KTestCaseResultFinished() ,  &KTestCaseResultSuccess() );
        }
   
    /*if (PrepareMediaEvent)*/ CleanupStack::PopAndDestroy(prepareMediaEvent);
    /*if (PrepareParameters)*/ CleanupStack::Pop(prepareParameters);
           
    CleanupStack::PopAndDestroy(player);
    return KErrExecuted;
    }


TInt CMmfTsTone::TestCasePlayCorruptFileL(CStifSectionParser *section , TTestResult &aResult)
    {
    TPtrC fileNamePtr;
    TBool haveFileName = !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) ;
    if ( haveFileName )
        {
        TBool usingDefaultErrorRange;
        TBool usingDefaultExpectedDuration;
        TTimeIntervalMicroSeconds errorRange = GetTimeIntervalL(section, KTagErrorRange, usingDefaultErrorRange, KDefaultErrorRange);
        TTimeIntervalMicroSeconds toneSequenceDuration = GetTimeIntervalL(section, KTagExpectedDuration, usingDefaultExpectedDuration, KDefaultToneDuration);

        TFileName fileName = fileNamePtr;

        TInt error=KErrNone;
        iOutputPrinter.Printf(_L("Creating Tone Player") );
        CTonePlayer *player = CTonePlayer::NewLC(&iOutputPrinter);

        player->PrepareFileSequence(fileName);
        CActiveScheduler::Start();
        error = player->GetFinalError();

        if (!usingDefaultExpectedDuration && !error) 
            {
            iOutputPrinter.Printf(_L("Comparing duration with expected duration") );
            if (!IsDurationInRange(0, toneSequenceDuration, TTimeIntervalMicroSeconds(0), errorRange, player->GetPlayStartTime() , player->GetPlayStopTime() ))
                {
                error = KErrExpectedValueDifferent; 
                }
            }

        CleanupStack::PopAndDestroy(player);
        
        if (error==KErrExpectedValueDifferent) 
            {
            aResult.iResult=error;
            aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFailExpectedValueDifferent() );
            }
        else if (error==KErrCorrupt)
            {
            aResult.iResult=KErrNone;
            aResult.iResultDes.Copy( KTestCaseResultSuccess() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
            
            }
        else
            {
            aResult.iResult=error;
            aResult.iResultDes.Copy( KTestCaseResultFail() );
            iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
            }
        
        return KErrExecuted;
        }
    else
        {
        //File to play missing
        iOutputPrinter.Printf(_L("Filename to play not configured"));
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        }
    }
TInt CMmfTsTone::TestCaseCheckStateL(CStifSectionParser * section , TTestResult &aResult) 
    {
    
    TBool usingDefault;
    TTimeIntervalMicroSeconds errorRange = GetTimeIntervalL(section, KTagErrorRange, usingDefault, KDefaultErrorRange);
    TTimeIntervalMicroSeconds toneDuration = GetTimeIntervalL(section, KTagToneDuration, usingDefault, KDefaultToneDuration);
    TInt frequency1=KDefaultToneFrequency;
    TInt frequency2=KDefaultToneFrequency;
    CImmediateMediaEvent *prepareMediaEvent=0;
    CParameters *prepareParameters=0;
    
    TInt error = KErrNone;
    TInt initialState=KErrNone,preparedState=KErrNone,playingState=KErrNone;
    //HBufC *DTMFString;
    TInt validityCheck=GetIntL(section, KTagValidity, usingDefault, KDefaultValidity);
    //Obtain the tone's frequency
    frequency1 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency);
    if (usingDefault)
        {
        iOutputPrinter.Printf(_L("Using default tone frequency for player") );
        }
    else
        {
        iOutputPrinter.Printf(_L("Using frequency 1 for player (%d)") , frequency1);
        }
    frequency2 = GetIntL(section, KTagToneFrequency, usingDefault, KDefaultToneFrequency, ETrue);
    if (usingDefault)
        {
        iOutputPrinter.Printf(_L("Using default tone frequency for player") );
        }
    else
        {
        iOutputPrinter.Printf(_L("Using frequency 2 for player (%d)") , frequency2);
        }

   

    iOutputPrinter.Printf(_L("Creating Tone Player") );
    CTonePlayer *player = CTonePlayer::NewLC(&iOutputPrinter);
    
    if (player->State()==CTonePlayer::ETonePlayerNotReady)
        {
        initialState=KErrNone;
        RDebug::Printf("current state:EMdaAudioUtilityNotReady");
        }
    else
        {
        initialState=KErrExpectedValueDifferent;
        }
    
    player->PrepareDualTone(frequency1,frequency2,toneDuration,EFalse);
    
    iOutputPrinter.Printf(_L("Starting scheduler"));
    CActiveScheduler::Start();
    if (player->State()==CTonePlayer::ETonePlayerPrepared)
       {
       preparedState=KErrNone;
       RDebug::Printf("current state:EMdaAudioUtilityPrepared");
       }
   else
       {
       preparedState=KErrExpectedValueDifferent;
       }
   error = player->GetFinalError();
   player->Play();
   if(player->State()==CTonePlayer::ETonePlayerPlaying)
       {
       playingState=KErrNone;
       RDebug::Printf("current state:EMdaAudioUtilityPlaying");
       }
   else
       {
       playingState=KErrExpectedValueDifferent;
       }
    CActiveScheduler::Start();

    error = player->GetFinalError();
    TBool InRange=ETrue;
    if (!IsDurationInRange(0, toneDuration, TTimeIntervalMicroSeconds(0), errorRange, player->GetPlayStartTime(), player->GetPlayStopTime() ) )
        {
        error = KErrExpectedValueDifferent;
        InRange=EFalse;
        }

    CleanupStack::PopAndDestroy(player);
    
     if (!InRange)
        {       
        aResult.iResult = KErrExpectedValueDifferent;
        aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
        iOutputPrinter.Printf( KTestCaseResultFinished() , &KTestCaseResultFailExpectedValueDifferent() );
        }
    
    
    else if (error || initialState || preparedState || playingState)
        {
        aResult.iResult=error;
        aResult.iResultDes.Copy( KTestCaseResultFail() );
        iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultFail() );
        }
   
    else
        {
        aResult.iResult=KErrNone;
        aResult.iResultDes.Copy( KTestCaseResultSuccess() );
        iOutputPrinter.Printf(KTestCaseResultFinished, &KTestCaseResultSuccess() );
        }
    return KErrExecuted;
    
    }

