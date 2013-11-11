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
* Description:  mmftsplay test component
*
*/

// INCLUDE FILES
#include <StifTestInterface.h>
#include "MmfTsPlay.h"
#include <mmf/common/mmcaf.h>

// Constants
const TInt CMmfTsPlay::MAX_TESTCASE_ID = 160;
const TInt CMmfTsPlay::KDefaultRepeats = 3;
const TInt CMmfTsPlay::KDefaultRepeatForever = -2;
const TInt CMmfTsPlay::KDefaultPauseDelay = 1000000;
const TInt CMmfTsPlay::KDefaultPlayDelay = 3000000;
const TInt CMmfTsPlay::KDefaultStopOpenPlayDelay = 1000000;
const TInt CMmfTsPlay::KDefaultReportDelay = 500000;

//Cases
const TInt CMmfTsPlay::KCaseLoopPlayTest=99;
const TInt CMmfTsPlay::KCaseAudioOutput=100;
const TInt CMmfTsPlay::KCaseImplementationInformation = 101;
const TInt CMmfTsPlay::KCasePlayFile = 102;
const TInt CMmfTsPlay::KCasePlayFileStopOpen = 103;
const TInt CMmfTsPlay::KCasePlayDes = 104;
const TInt CMmfTsPlay::KCasePlayDesStopOpen = 105;
const TInt CMmfTsPlay::KCasePlayUrl = 106;
const TInt CMmfTsPlay::KCasePlayUrlStopOpen = 107;
const TInt CMmfTsPlay::KCasePlayFileWithPause = 108;
const TInt CMmfTsPlay::KCasePlayFileWithPosition = 109;
const TInt CMmfTsPlay::KCasePlayFileReportPosition = 110;
const TInt CMmfTsPlay::KCasePlayFileReportDuration = 111;
const TInt CMmfTsPlay::KCasePlayFilesWithVolumes = 112;
const TInt CMmfTsPlay::KCasePlayFileWithRamp = 113;
const TInt CMmfTsPlay::KCasePlayFileMetaInfo = 114;
const TInt CMmfTsPlay::KCasePlayFileWindow = 115;
const TInt CMmfTsPlay::KCasePlayFileClearWindow = 116;
const TInt CMmfTsPlay::KCasePlayFileWithRepeats = 117;
const TInt CMmfTsPlay::KCasePlayFileWithBalance = 118;
const TInt CMmfTsPlay::KCasePlayFilesWithPriorities = 119;
const TInt CMmfTsPlay::KCasePlayHandler = 120;
const TInt CMmfTsPlay::KCasePlayHandlerStopOpen = 121;
const TInt CMmfTsPlay::KCasePlayHandlerWithPause = 122;
const TInt CMmfTsPlay::KCasePlayHandlerWithRamp = 123;
const TInt CMmfTsPlay::KCasePlayHandlerWithSetPosition = 124;
const TInt CMmfTsPlay::KCasePlayHandlerWithGetPosition = 125;
const TInt CMmfTsPlay::KCasePlayHandlerWithDuration = 126;
const TInt CMmfTsPlay::KCasePlayHandlerWithWindow = 127;
const TInt CMmfTsPlay::KCasePlayHandlerWithClearWindow = 128;
// 129
const TInt CMmfTsPlay::KCaseMetaDataData = 130;
const TInt CMmfTsPlay::KCaseMetaDataDataFileHandler = 131;
const TInt CMmfTsPlay::KCasePlayFileMetaInfoFileHandler = 132;
const TInt CMmfTsPlay::KCaseMetaDataCount = 133;
const TInt CMmfTsPlay::KCaseMetaDataCountFileHandler = 134;
const TInt CMmfTsPlay::KCasePlayHandlerWithPriorities = 135;
const TInt CMmfTsPlay::KCaseNewFilePlayer = 136;
const TInt CMmfTsPlay::KCaseNewDesPlayer = 137;
const TInt CMmfTsPlay::KCaseNewDesReadOnlyPlayer = 138;
const TInt CMmfTsPlay::KCaseGetLoadPercentage = 139;
const TInt CMmfTsPlay::KCaseGetBitRate = 140;
const TInt CMmfTsPlay::KCaseGetDRMCmd = 141;
const TInt CMmfTsPlay::KCaseCancelNotification = 142;
const TInt CMmfTsPlay::KCaseCtrImpInfo = 143;
const TInt CMmfTsPlay::KCaseSet_Priority = 144;
const TInt CMmfTsPlay::KCaseOpenFile = 145;
const TInt CMmfTsPlay::KCase_PlayFile = 146;
const TInt CMmfTsPlay::KCasePlayFileHandler = 147;
const TInt CMmfTsPlay::KCasePlayFileSource = 148;
const TInt CMmfTsPlay::KCasePlayDesc = 149;
const TInt CMmfTsPlay::KCaseOpenUrl = 150;
const TInt CMmfTsPlay::KCaseRegNotification = 152;
const TInt CMmfTsPlay::KCaseRegLoadNotification = 153;

const TInt CMmfTsPlay::KCasePlayFileWithRepeatForever = 154;
const TInt CMmfTsPlay::KCasePlayFileWithRepeatsWithPause = 155;
const TInt CMmfTsPlay::KCasePlayFileWithRepeatsInPlaywindow = 156;
const TInt CMmfTsPlay::KCasePlayFileWithRepeatsInPlaywindowPause = 157;
const TInt CMmfTsPlay::KCasePlayFileWithRepeatForeverInPlaywindow = 158;

// ** FilePlay *******************************************************
TInt CMmfTsPlay::TestCasePlayFileWithPauseL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds32 PauseDelay=KDefaultPauseDelay;
	TTimeIntervalMicroSeconds32 PlayDelay=KDefaultPlayDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************************************
		TBool UsingDefault;
		PauseDelay = GetTimeIntervalL(section, KTagPauseDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPauseDelay);
		if (UsingDefault) {iLogger->Log(_L("Error retrieving pause delay, using default"));}

		PlayDelay = GetTimeIntervalL(section, KTagPlayDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPlayDelay);
		if (UsingDefault) {iLogger->Log(_L("Error retrieving play delay, using default"));}

		if (PauseDelay > PlayDelay)
			{
			//Pause delay longer than play delay. Switching
			TTimeIntervalMicroSeconds32 TempDelay = PlayDelay;
			PlayDelay = PauseDelay;
			PauseDelay = TempDelay;
			}
		iLogger->Log(_L("Setting Pause and Play delays to [%d] and [%d]"), PauseDelay.Int(), PlayDelay.Int());

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}

		//*********************************************

		CParameters *pauseParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPause );
		CleanupStack::PushL(pauseParams);

		CParameters *playParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPlay );
		CleanupStack::PushL(playParams);

		CMediaEvent *mEventPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PlayDelay, PlayDelay, sndPlayer, playParams);
		CMediaEvent *mEventPause = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PauseDelay, PauseDelay, sndPlayer, pauseParams, CActive::EPriorityStandard+1);

		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime, EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );

		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		TTime ExpectedEnd = StartTime + sndPlayer->GetDuration() + PlayDelay - PauseDelay;
		TTime Range1 = ExpectedEnd - ErrorRange;
		TTime Range2 = ExpectedEnd + ErrorRange;

		TTimeIntervalMicroSeconds ExpectedDuration = ExpectedEnd.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Perceived duration (%d)") , PerceivedDuration.Int64() );

		iLogger->Log(_L("Expected duration  (%d)"), ExpectedDuration.Int64() );
	//	iLogger->Log(_L("Difference = (%d)") , ExpectedEnd.MicroSecondsFrom(EndTime) );

		if (mEventPlay->GetCount()<=0 || mEventPause->GetCount()<=0)
			{
			iLogger->Log( KTestCaseResultFailSkippedEvents() );
			aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
			aResult.iResult = KErrSkippedEvents;
			}
		else if (EndTime < Range1 || Range2 < EndTime)
			{	//Durations too different
			iLogger->Log(_L("The clips duration + silence duration (%d) is too different from the actual duration (%d)") , ExpectedDuration.Int64() , PerceivedDuration.Int64() );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else if (sndPlayer->iFinalError)
			{
			aResult.iResult = sndPlayer->iFinalError;
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(mEventPause);
		CleanupStack::PopAndDestroy(mEventPlay);
		CleanupStack::Pop(playParams);
		CleanupStack::Pop(pauseParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayHandlerWithPauseL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds32 PauseDelay = KDefaultPauseDelay;
	TTimeIntervalMicroSeconds32 PlayDelay = KDefaultPlayDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;

		iLogger->Log(_L("Creating simple player with file handler [%S]"), &FileNamePtr);
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));

	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);

		iFile.Close();
		iFs.Close();

		CleanupStack::PushL(sndPlayer);

		//*********************************************
		TBool UsingDefault;
		PauseDelay = GetTimeIntervalL(section, KTagPauseDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPauseDelay);
		if (UsingDefault) {iLogger->Log(_L("Error retrieving pause delay, using default"));}

		PlayDelay = GetTimeIntervalL(section, KTagPlayDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPlayDelay);
		if (UsingDefault) {iLogger->Log(_L("Error retrieving play delay, using default"));}

		if (PauseDelay > PlayDelay)
			{
			//Pause delay longer than play delay. Switching
			TTimeIntervalMicroSeconds32 TempDelay = PlayDelay;
			PlayDelay = PauseDelay;
			PauseDelay = TempDelay;
			}
		iLogger->Log(_L("Setting Pause and Play delays to [%d] and [%d]"), PauseDelay.Int(), PlayDelay.Int());

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}

		//*********************************************

		CParameters *pauseParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPauseResume );
		CleanupStack::PushL(pauseParams);

		CParameters *playParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPlay );
		CleanupStack::PushL(playParams);

		CMediaEvent *mEventPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PlayDelay, PlayDelay, sndPlayer, playParams);
		CMediaEvent *mEventPause = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PauseDelay, PauseDelay, sndPlayer, pauseParams, CActive::EPriorityStandard+1);

		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime, EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );

		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		TTime ExpectedEnd = StartTime + sndPlayer->GetDuration() + PlayDelay - PauseDelay;
		TTime Range1 = ExpectedEnd - ErrorRange;
		TTime Range2 = ExpectedEnd + ErrorRange;

		TTimeIntervalMicroSeconds ExpectedDuration = ExpectedEnd.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Perceived duration (%d)") , PerceivedDuration.Int64() );

		iLogger->Log(_L("Expected duration  (%d)"), ExpectedDuration.Int64() );
	//	iLogger->Log(_L("Difference = (%d)") , ExpectedEnd.MicroSecondsFrom(EndTime) );

		if (mEventPlay->GetCount()<=0 || mEventPause->GetCount()<=0)
			{
			iLogger->Log( KTestCaseResultFailSkippedEvents() );
			aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
			aResult.iResult = KErrSkippedEvents;
			}
		else if (EndTime < Range1 || Range2 < EndTime)
			{	//Durations too different
			iLogger->Log(_L("The clips duration + silence duration (%d) is too different from the actual duration (%d)") , ExpectedDuration.Int64() , PerceivedDuration.Int64() );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else if (sndPlayer->iFinalError)
			{
			aResult.iResult = sndPlayer->iFinalError;
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(mEventPause);
		CleanupStack::PopAndDestroy(mEventPlay);
		CleanupStack::Pop(playParams);
		CleanupStack::Pop(pauseParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);




		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseGetBitRateL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);

		sndPlayer -> GetBit_Rate();


		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseGetDRMCmdL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
	    sndPlayer -> GetDRMCmd();



		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}




TInt CMmfTsPlay::TestCaseCancelNotificationL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
	    sndPlayer -> CancelNotification();



		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseCtrImpInfoL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);

		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();
        sndPlayer -> CtrImpInfo();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseSet_PriorityL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
	    sndPlayer -> Set_Priority();



		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}





TInt CMmfTsPlay::TestCaseNewFilePlayerL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewFileLC( FileName, TestModuleIf() , *iLogger);
	    CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);


		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}
TInt CMmfTsPlay::TestCasePlayHandlerL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;

	User::LeaveIfError(iFs.Connect());
	iFs.ShareProtected();

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);

		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
	//	TRAP(err, iAudioPlayer->OpenFileL( iFile ));

	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);

		iFile.Close();
		iFs.Close();

		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileWithRepeatsL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TInt Repeats = KDefaultRepeats;
	TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
	TInt ParseError=KErrNone;
	CStifItemParser *item;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//Setup repeat count
		ParseError=KErrNotFound;
		item = section->GetItemLineL(KTagRepeatTimes);
		if (item)
			{
			CleanupStack::PushL(item);
			ParseError = item->GetInt(KTagRepeatTimes, Repeats);
			CleanupStack::PopAndDestroy(item);
			}
		iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
		if (ParseError)
			{
			iLogger->Log(_L("Using default value of %d"), KDefaultRepeats); Repeats=KDefaultRepeats;
			}

		TBool UsingDefault;
		SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
		// * ****************************************************
		iLogger->Log(_L("Setting interval to %d"), SilenceDuration.Int64());

		if (UsingDefault) { iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64()) ; }
		sndPlayer->SetRepeats(Repeats, SilenceDuration);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;

		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(Repeats, ( DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), SilenceDuration, ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;	//aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileWithPositionL(CStifSectionParser *section, TTestResult & aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds Position = KDefaultPosition;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		TBool UsingDefault;
		Position = GetTimeIntervalL(section, KTagPosition, UsingDefault, KDefaultPosition);

		iLogger->Log(_L("Seting position to: %d"), Position.Int64());
		if (UsingDefault) {iLogger->Log(_L("Using default position duration of: (%d)") , KDefaultPosition.Int64());}
		sndPlayer->iPosition = Position;

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}
		// * **************************************************************


		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime,EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();



		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		iLogger->Log(_L("Perceived duration: %d") , PerceivedDuration.Int64() );


		TTimeIntervalMicroSeconds ExpectedDuration = TTimeIntervalMicroSeconds(I64INT(sndPlayer->GetDuration().Int64()) - I64INT(Position.Int64()));
		iLogger->Log(_L("ExpectedDuration: %d") ,ExpectedDuration.Int64() );

		if (  Abs(ExpectedDuration.Int64() - PerceivedDuration.Int64()) > ErrorRange.Int64() )
			{	//Durations too different
			iLogger->Log(_L("The clips duration is too different from the actual duration + position") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else
			{
			aResult.iResult = sndPlayer->iFinalError;
			if (aResult.iResult)
				{
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy(KTestCaseResultFail());
				}
			else
				{
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy(KTestCaseResultSuccess());
				}
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCaseGetLoadPercentageL(CStifSectionParser *section, TTestResult & aResult)
	{
	TPtrC FileNamePtr;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

	    CActiveScheduler::Start();
 		sndPlayer -> GetLoad_Percentage();
		
		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
    	aResult.iResult = sndPlayer->iFinalError;
		if (aResult.iResult)
	    	{
		    iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
			
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}
//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithSetPositionL(CStifSectionParser *section, TTestResult & aResult)
	{

	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds Position = KDefaultPosition;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();

		TBool UsingDefault;
		Position = GetTimeIntervalL(section, KTagPosition, UsingDefault, KDefaultPosition);

		iLogger->Log(_L("Seting position to: %d"), Position.Int64());
		if (UsingDefault) {iLogger->Log(_L("Using default position duration of: (%d)") , KDefaultPosition.Int64());}
		sndPlayer->iPosition = Position;

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}
		// * **************************************************************

		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime,EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();



		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		iLogger->Log(_L("Perceived duration: %d") , PerceivedDuration.Int64() );


		TTimeIntervalMicroSeconds ExpectedDuration = TTimeIntervalMicroSeconds(I64INT(sndPlayer->GetDuration().Int64()) - I64INT(Position.Int64()));
		iLogger->Log(_L("ExpectedDuration: %d") ,ExpectedDuration.Int64() );

		if (  Abs(ExpectedDuration.Int64() - PerceivedDuration.Int64()) > ErrorRange.Int64() )
			{	//Durations too different
			iLogger->Log(_L("The clips duration is too different from the actual duration + position") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else
			{
			aResult.iResult = sndPlayer->iFinalError;
			if (aResult.iResult)
				{
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy(KTestCaseResultFail());
				}
			else
				{
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy(KTestCaseResultSuccess());
				}
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}



TInt CMmfTsPlay::TestCasePlayHandlerWithPrioritiesL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{


		TInt PriorityLow = EMdaPriorityNormal+1;
		TInt PriorityHigh = EMdaPriorityNormal+20;

		TBool UsingDefaultPriority;
		PriorityHigh = GetIntL(section, KTagPriority, UsingDefaultPriority, PriorityHigh);
		PriorityLow = GetIntL(section, KTagPriority, UsingDefaultPriority, PriorityLow, ETrue);

		if (PriorityHigh < PriorityLow)
			{
			TInt tempPriority = PriorityHigh;
			PriorityHigh = PriorityLow;
			PriorityLow = tempPriority;
			}

		TBool UsingDefaultPriorityPreference;
		TInt tmpPrefHigh = GetIntL(section, KTagPriorityPreference, UsingDefaultPriorityPreference, -1);
		TInt tmpPrefLow = GetIntL(section, KTagPriorityPreference, UsingDefaultPriorityPreference, tmpPrefHigh, ETrue);

		TMdaPriorityPreference prefHigh;
		switch (tmpPrefHigh)
			{
			case 0:
				prefHigh = EMdaPriorityPreferenceNone; break;
			case 1:
				prefHigh = EMdaPriorityPreferenceTime; break;
			case 2:
				prefHigh = EMdaPriorityPreferenceQuality; break;
			case 3:
			default:
				prefHigh = EMdaPriorityPreferenceTimeAndQuality; break;
			}
		TMdaPriorityPreference prefLow;
		switch (tmpPrefLow)
			{
			case 0:
				prefLow = EMdaPriorityPreferenceNone; break;
			case 1:
				prefLow = EMdaPriorityPreferenceTime; break;
			case 2:
				prefLow = EMdaPriorityPreferenceQuality; break;
			case 3:
			default:
				prefLow = EMdaPriorityPreferenceTimeAndQuality; break;
			}




		TObjectCountDown counter;



	    TFileName FileName = FileNamePtr;
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);  //JZ ID=135
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();

		if ( section->GetNextLine(KTagSoundFile, FileNamePtr, ENoTag) )
			{
			iLogger->Log(_L("Second filename not found, using the same file twice"));
			}
		else
			{
			FileName=FileNamePtr;
			iLogger->Log(_L("Second filename set to [%S]") , &FileName );
			}

		// ***************
		iLogger->Log(_L("Creating playerLow with file [%S]"), &FileName);
		CSimpleSoundPlayer *sndPlayerLow = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);


		iLogger->Log(_L("SetThread_Priority()"));
		sndPlayerLow -> SetThread_Priority();

		iLogger->Log(_L("Setting playerHigh priority to (%d)"), PriorityHigh);
		sndPlayer->SetPriority( PriorityHigh, prefHigh);

		sndPlayer->SetSupervisor(&counter);
		sndPlayer->SetSupervisor(&counter);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from playerHigh, errorcode: (%d)"), sndPlayer->iFinalError );
		iLogger->Log(_L("Returned from playerLow, errorcode: (%d)"), sndPlayer->iFinalError );

		aResult.iResult = sndPlayer->iFinalError ;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayerLow);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileStopOpenL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds32 StopDelay=KDefaultStopOpenPlayDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);



		//*********************************************
		//FileName of second file to play
		CParameters *stopParams ;
		if ( section->GetNextLine(KTagSoundFile, FileNamePtr, ENoTag) )
			{
			stopParams = CFileNameParameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayFile, FileName );
			iLogger->Log(_L("Second filename not found, using the same file twice"));
			}
		else
			{
			stopParams = CFileNameParameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayFile, FileNamePtr );
			iLogger->Log(_L("Second filename set to [%S]") , &FileNamePtr );
			}

		//*********************************************
		TBool UsingDefaultDelay;
		StopDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultDelay, (TTimeIntervalMicroSeconds32)KDefaultStopOpenPlayDelay);

		if (UsingDefaultDelay) {iLogger->Log(_L("Error retrieving stop delay, using default"));}
		iLogger->Log(_L("Setting delays to stop and open file to [%d]"), StopDelay.Int());
		//*********************************************

		CMediaEvent *mEventStopPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, StopDelay, StopDelay, sndPlayer, stopParams);

		if (sndPlayer->GetDuration() < TTimeIntervalMicroSeconds(StopDelay.Int() ) )
			{
			iLogger->Log(_L("Warning: sound clip duration longer than delay for stop"));
			}


		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		iLogger->Log(_L("Playcount: (%d)") , mEventStopPlay->GetCount() );
		if (mEventStopPlay->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFailSkippedEvents());
			}
		else if (sndPlayer->iFinalError)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			aResult.iResult = sndPlayer->iFinalError;
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			aResult.iResult = sndPlayer->iFinalError;
			}
		CleanupStack::PopAndDestroy(mEventStopPlay);
		CleanupStack::Pop(stopParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}



TInt CMmfTsPlay::TestCasePlayHandlerStopOpenL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TPtrC FileNamePtr2;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds32 StopDelay=KDefaultStopOpenPlayDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);

		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);

		iFile.Close();
		iFs.Close();

		CleanupStack::PushL(sndPlayer);


		//*********************************************
		//FileName of second file to play
		CParameters *stopParams ;
		if ( section->GetNextLine(KTagSoundFile, FileNamePtr, ENoTag) )
			{
			stopParams = CFileNameParameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayFileHandler, FileName );
			iLogger->Log(_L("Second filename not found, using the same file twice"));
			}
		else
			{
			stopParams = CFileNameParameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayFileHandler, FileNamePtr );
			iLogger->Log(_L("Second filename set to [%S]") , &FileNamePtr );
			}

		//*********************************************
		TBool UsingDefaultDelay;
		StopDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultDelay, (TTimeIntervalMicroSeconds32)KDefaultStopOpenPlayDelay);

		if (UsingDefaultDelay) {iLogger->Log(_L("Error retrieving stop delay, using default"));}
		iLogger->Log(_L("Setting delays to stop and open file to [%d]"), StopDelay.Int());
		//*********************************************

		CMediaEvent *mEventStopPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, StopDelay, StopDelay, sndPlayer, stopParams);

		if (sndPlayer->GetDuration() < TTimeIntervalMicroSeconds(StopDelay.Int() ) )
			{
			iLogger->Log(_L("Warning: sound clip duration longer than delay for stop"));
			}
		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		iLogger->Log(_L("Playcount: (%d)") , mEventStopPlay->GetCount() );
		if (mEventStopPlay->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFailSkippedEvents());
			}
		else if (sndPlayer->iFinalError)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			aResult.iResult = sndPlayer->iFinalError;
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			aResult.iResult = sndPlayer->iFinalError;
			}
		CleanupStack::PopAndDestroy(mEventStopPlay);
		CleanupStack::Pop(stopParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileWithBalanceL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TInt Balance = KMMFBalanceCenter;
	TInt ParseError=KErrNone;
	CStifItemParser *item;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
	//	TInt BalanceError=KErrNone;
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************
		ParseError=KErrNotFound;
		item = section->GetItemLineL(KTagBalance);
		if (item)
			{
			CleanupStack::PushL(item);
			ParseError = item->GetInt(KTagBalance, Balance);
			CleanupStack::PopAndDestroy(item);
			}
		if (ParseError)
			{
			iLogger->Log(_L("Value for Balance not found, using default (%d)") , KMMFBalanceCenter);
			Balance = KMMFBalanceCenter;
			}
		else
			{
			iLogger->Log(_L("Setting Balance to: %d"), Balance);
			sndPlayer->aBalance = Balance;
			}

		sndPlayer->setBalance = true;

//		iLogger->Log(_L("Setting Balance to: %d"), Balance);
//		BalanceError = sndPlayer->SetBalance(Balance);
	//	if (BalanceError)
		//	{
		//	iLogger->Log(_L("Error setting balance"));
		//	aResult.iResult = BalanceError;
		//	}
	//	else
		//	{
			iLogger->Log(_L("Starting scheduler"));
			CActiveScheduler::Start();

			iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
			aResult.iResult = sndPlayer->iFinalError;
		//	}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayDesL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundDes, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;

		HBufC8 *SoundFile = GetDescriptor8LC(FileName, section);

		iLogger->Log(_L("Creating simple player with descriptor from [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( *SoundFile, TestModuleIf() , *iLogger);

		CleanupStack::PushL(sndPlayer);

		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);

		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration
						? sndPlayer->GetDuration()
						: ExpectedDuration
						), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}


		CleanupStack::PopAndDestroy(sndPlayer);

		CleanupStack::PopAndDestroy(SoundFile);

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;

		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseNewDesPlayerL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundDes, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;

		HBufC8 *SoundFile = GetDescriptor8LC(FileName, section);

		iLogger->Log(_L("Creating simple player with descriptor from [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewDesL( *SoundFile, TestModuleIf() , *iLogger);
CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( *SoundFile, TestModuleIf() , *iLogger);


		CleanupStack::PushL(sndPlayer);

		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);

		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration
						? sndPlayer->GetDuration()
						: ExpectedDuration
						), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}


		CleanupStack::PopAndDestroy(sndPlayer);

		CleanupStack::PopAndDestroy(SoundFile);

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;

		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCaseNewDesReadOnlyPlayerL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundDes, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;

		HBufC8 *SoundFile = GetDescriptor8LC(FileName, section);

		iLogger->Log(_L("Creating simple player with descriptor from [%S]"), &FileNamePtr);


 //		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewDesLC( *SoundFile, TestModuleIf() , *iLogger);
 		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( *SoundFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);

		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration
						? sndPlayer->GetDuration()
						: ExpectedDuration
						), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}


		CleanupStack::PopAndDestroy(sndPlayer);

		CleanupStack::PopAndDestroy(SoundFile);

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;

		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileReportPositionL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		TBool UsingDefaultReportDelay;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************************************
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());
		//*********************************************

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithGetPositionL(CStifSectionParser *section , TTestResult &aResult)
	{

	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		TBool UsingDefaultReportDelay;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();

		//*********************************************
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());
		//*********************************************

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileReportDurationL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds32 ReportDelay=KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool ExpectedDurationNotSet = EFalse, UsingDefaultReportDelay;;
		TTimeIntervalMicroSeconds ExpectedDuration;

		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************************************
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());
		//*********************************************
		ExpectedDuration = GetTimeIntervalL( section, KTagExpectedDuration, ExpectedDurationNotSet) ;
		//*********************************************

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportDuration);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );

		aResult.iResult = sndPlayer->iFinalError;

		if (mEventReportPosition->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			}

		if (!ExpectedDurationNotSet)
			{
			iLogger->Log(_L("Expected duration: (%d)") , ExpectedDuration.Int64() );

			if ( ExpectedDuration!=sndPlayer->GetDuration() )
				{
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		switch (aResult.iResult)
			{
			case KErrNone:
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy( KTestCaseResultSuccess() );
				break;
			case KErrExpectedValueDifferent:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
				break;
			case KErrSkippedEvents:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
				break;
			default:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFail() );
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrNone;	//aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithDurationL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds32 ReportDelay=KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool ExpectedDurationNotSet = EFalse, UsingDefaultReportDelay;;
		TTimeIntervalMicroSeconds ExpectedDuration;

		TFileName FileName = FileNamePtr;
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();

		//*********************************************
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());
		//*********************************************
		ExpectedDuration = GetTimeIntervalL( section, KTagExpectedDuration, ExpectedDurationNotSet) ;
		//*********************************************

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportDuration_Para);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );

		aResult.iResult = sndPlayer->iFinalError;

		if (mEventReportPosition->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			}

		if (!ExpectedDurationNotSet)
			{
			iLogger->Log(_L("Expected duration: (%d)") , ExpectedDuration.Int64() );

			if ( ExpectedDuration!=sndPlayer->GetDuration() )
				{
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		switch (aResult.iResult)
			{
			case KErrNone:
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy( KTestCaseResultSuccess() );
				break;
			case KErrExpectedValueDifferent:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailExpectedValueDifferent() );
				break;
			case KErrSkippedEvents:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
				break;
			default:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFail() );
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrNone;	//aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileWindowL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;
	TBool UsingDefault;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool UsingStartDefault, UsingEndDefault, UsingDefaultReportDelay ,UsingNegativeTestCaseDefault;
		TFileName FileName = FileNamePtr;
	//	TInt WindowError;

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************
		StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()-1E6));
		EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()+1E6));

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}


		sndPlayer->startPosition = StartPosition;
		sndPlayer->endPosition = EndPosition;
		sndPlayer->playWindow = true;

		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());
		
	
		TInt NegativeTestCaseValue = GetIntL(section, KNegativeTag, UsingNegativeTestCaseDefault,0);
		
		if  (NegativeTestCaseValue == 1)
		    {
		     sndPlayer->iNegativePlayBackWindow = true;
		    }

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime,EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();

		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		iLogger->Log(_L("Perceived duration: %d") , PerceivedDuration.Int64() );


		TTimeIntervalMicroSeconds ExpectedDuration = TTimeIntervalMicroSeconds(I64INT(EndPosition.Int64()) - I64INT(StartPosition.Int64()));
		iLogger->Log(_L("ExpectedDuration: %d") ,ExpectedDuration.Int64() );
		
	    if((ExpectedDuration > (sndPlayer->GetDuration())) && (sndPlayer->iNegativePlayBackWindow)) 
		   {
		   iLogger->Log(_L("ExpectedDuration is greater than the actual duration of the file"));
		   iLogger->Log(_L("Negative Test was successful"));		   
		   aResult.iResultDes.Copy(KTestCaseResultSuccess());
       } 
	    else if (  Abs(ExpectedDuration.Int64() - PerceivedDuration.Int64()) > ErrorRange.Int64() )
			{	//Durations too different
			iLogger->Log(_L("The clips duration is too different from the actual duration + position") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else if (aResult.iResult = sndPlayer->iFinalError , aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithWindowL(CStifSectionParser *section , TTestResult &aResult)
	{

	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;
	TBool UsingDefault;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool UsingStartDefault, UsingEndDefault, UsingDefaultReportDelay;
		TFileName FileName = FileNamePtr;
	//	TInt WindowError;

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);


		iFile.Close();
		iFs.Close();

		//*********************
		StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()-1E6));
		EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()+1E6));

		TTimeIntervalMicroSeconds ErrorRange = GetTimeIntervalL(section, KTagErrorRange, UsingDefault, KDefaultErrorRange);
		if (UsingDefault) {iLogger->Log(_L("Using default error range"));}


		sndPlayer->startPosition = StartPosition;
		sndPlayer->endPosition = EndPosition;
		sndPlayer->playWindow = true;

		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		TTime StartTime,EndTime;
		StartTime.HomeTime();
		CActiveScheduler::Start();
		EndTime.HomeTime();

		TTimeIntervalMicroSeconds PerceivedDuration = EndTime.MicroSecondsFrom( StartTime );

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		iLogger->Log(_L("Perceived duration: %d") , PerceivedDuration.Int64() );


		TTimeIntervalMicroSeconds ExpectedDuration = TTimeIntervalMicroSeconds(I64INT(EndPosition.Int64()) - I64INT(StartPosition.Int64()));
		iLogger->Log(_L("ExpectedDuration: %d") ,ExpectedDuration.Int64() );

		if (  Abs(ExpectedDuration.Int64() - PerceivedDuration.Int64()) > ErrorRange.Int64() )
			{	//Durations too different
			iLogger->Log(_L("The clips duration is too different from the actual duration + position") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			}
		else if (aResult.iResult = sndPlayer->iFinalError , aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayFileWindowClearL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool UsingStartDefault, UsingEndDefault, UsingDefaultReportDelay;
		TFileName FileName = FileNamePtr;

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************
		StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()-1E6));
		EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()+1E6));

		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		sndPlayer->startPosition = StartPosition;
		sndPlayer->endPosition = EndPosition;
		sndPlayer->playWindow = true;
		sndPlayer->clearWindow = true;


		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		TBool PositionReset=ETrue;
		TTimeIntervalMicroSeconds position;
		TInt PosError = sndPlayer->GetPosition(position);
		if ( !PosError && (position!=TTimeIntervalMicroSeconds(0) ) )
			{
			iLogger->Log(_L("Position not reset to zero, position is (%d)") , position.Int64() );
			PositionReset=EFalse;
			}
		if (PosError)
			{
			iLogger->Log(_L("Error (%d) obtaining clip position"), PosError );
			}


		if (!PositionReset)
			{
			iLogger->Log(_L("Error clearing PlayWindow, position not reset to zero") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFail() );
			}
		else if (aResult.iResult = sndPlayer->iFinalError , aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithClearWindowL(CStifSectionParser *section , TTestResult &aResult)
	{

	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
	TTimeIntervalMicroSeconds32 ReportDelay = KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TBool UsingStartDefault, UsingEndDefault, UsingDefaultReportDelay;
		TFileName FileName = FileNamePtr;

		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();


		//*********************
		StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()-1E6));
		EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds)(KDefaultPosition.Int64()+1E6));

		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultReportDelay, (TTimeIntervalMicroSeconds32)KDefaultReportDelay);
		iLogger->Log(_L("Setting delays to report position to [%d]"), ReportDelay.Int());

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportPosition);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		sndPlayer->startPosition = StartPosition;
		sndPlayer->endPosition = EndPosition;
		sndPlayer->playWindow = true;
		sndPlayer->clearWindow = true;


		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		TBool PositionReset=ETrue;
		TTimeIntervalMicroSeconds position;
		TInt PosError = sndPlayer->GetPosition(position);
		if ( !PosError && (position!=TTimeIntervalMicroSeconds(0) ) )
			{
			iLogger->Log(_L("Position not reset to zero, position is (%d)") , position.Int64() );
			PositionReset=EFalse;
			}
		if (PosError)
			{
			iLogger->Log(_L("Error (%d) obtaining clip position"), PosError );
			}


		if (!PositionReset)
			{
			iLogger->Log(_L("Error clearing PlayWindow, position not reset to zero") );
			aResult.iResult = KErrExpectedValueDifferent;
			aResult.iResultDes.Copy(KTestCaseResultFail() );
			}
		else if (aResult.iResult = sndPlayer->iFinalError , aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCasePlayDesStopOpenL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;

	if ( !section->GetLine(KTagSoundDes, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with descriptor from [%S]"), &FileName);
		//*********************************************
		//Open descriptor
		HBufC8 *SoundFile = GetDescriptor8LC(FileName, section);
		HBufC8 *SoundFile2;

		//*********************************************
		//FileName of second file to play
		CParameters *stopParams ;
		if ( section->GetNextLine(KTagSoundDes, FileNamePtr, ENoTag) )
			{	//Using same filename
			SoundFile2 = SoundFile->AllocLC();
			stopParams = CDes8Parameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayDes, *SoundFile2 );
			iLogger->Log(_L("Second descriptor filename not found, using the same file twice"));
			}
		else
			{	//Found second filename
			FileName=FileNamePtr;
			SoundFile2 = GetDescriptor8LC(FileName, section);
			stopParams = CDes8Parameters::NewLC(CSimpleSoundPlayer::KPlayerActionStopPlayDes, *SoundFile2 );
			iLogger->Log(_L("Second descriptor from [%S]") , &FileName );
			}

		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewLC( *SoundFile, TestModuleIf() , *iLogger);

		TBool UsingDefault;
		TTimeIntervalMicroSeconds32 StopDelay = GetTimeIntervalL(section, KTagDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultStopOpenPlayDelay);
		if (UsingDefault) {iLogger->Log(_L("Error retrieving stop delay, using default"));}
		iLogger->Log(_L("Setting delays to stop and open descriptor to [%d]"), StopDelay.Int());
		//*********************************************

		CMediaEvent *mEventStopPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, StopDelay, StopDelay, sndPlayer, stopParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );

		//]TBool DontCheckExpectedDuration;
		//]TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		//]TBool DontUseExpectedDuration;
		//]TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);

		if (mEventStopPlay->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			aResult.iResultDes.Copy(KTestCaseResultFailSkippedEvents);
			iLogger->Log(KTestCaseResultFailSkippedEvents() );
			}
		else if ( sndPlayer->iFinalError)
			{
			aResult.iResult = sndPlayer->iFinalError;
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail() );
			}
		else
			{
			//]aResult.iResult = KErrNone;
			//]if (!DontCheckExpectedDuration && !DontUseExpectedDuration)
			//]	{
			//]	if (!IsDurationInRange(0, ExpectedDuration, TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
			//]		{
			//]		iLogger->Log(_L("Duration too different"));
			//]		aResult.iResult = KErrExpectedValueDifferent;
			//]		}
			//]	}
			//]if (aResult.iResult)
			//]	{
			//]	aResult.iResultDes.Copy(KTestCaseResultFailExpectedValueDifferent());
			//]	}
			//]else
			//]	{
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy(KTestCaseResultSuccess());
			//]	}
			}

		CleanupStack::PopAndDestroy(mEventStopPlay);
		CleanupStack::PopAndDestroy(sndPlayer);
		CleanupStack::Pop(stopParams);
		CleanupStack::PopAndDestroy(SoundFile2);
		CleanupStack::PopAndDestroy(SoundFile);
		return KErrExecuted;
		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileWithVolumeRampL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	TTimeIntervalMicroSeconds RampDuration = KDefaultRampDuration;
	TTimeIntervalMicroSeconds32 ReportDelay=KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		//*********************
		TBool UsingReportDefault, UsingRampDefault;
		RampDuration = GetTimeIntervalL(section, KTagRampDuration, UsingRampDefault, KDefaultRampDuration);
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingReportDefault, (TTimeIntervalMicroSeconds32)KDefaultReportDelay );

		// ******************************************
		iLogger->Log(_L("Seting up volume ramp with duration: %d"), RampDuration.Int64());
		if (UsingRampDefault) {iLogger->Log(_L("Using default Ramp duration of: %d") , KDefaultRampDuration.Int64());}

		sndPlayer->SetVolumeRamp(RampDuration);

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportVolume);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;

		if (mEventReportPosition->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			}

		switch (aResult.iResult)
			{
			case KErrNone:
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy( KTestCaseResultSuccess() );
				break;
			case KErrSkippedEvents:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
				break;
			default:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFail() );
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


//FH
TInt CMmfTsPlay::TestCasePlayHandlerWithRampL(CStifSectionParser *section , TTestResult &aResult)
	{

	TPtrC FileNamePtr;
	User::LeaveIfError(iFs.Connect());
	TTimeIntervalMicroSeconds RampDuration = KDefaultRampDuration;
	TTimeIntervalMicroSeconds32 ReportDelay=KDefaultReportDelay;

	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		User::LeaveIfError(iFile.Open( iFs, FileName, EFileRead));
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( iFile, TestModuleIf() , *iLogger);
		CleanupStack::PushL(sndPlayer);

		iFile.Close();
		iFs.Close();
		//*********************
		TBool UsingReportDefault, UsingRampDefault;
		RampDuration = GetTimeIntervalL(section, KTagRampDuration, UsingRampDefault, KDefaultRampDuration);
		ReportDelay = GetTimeIntervalL(section, KTagDelay, UsingReportDefault, (TTimeIntervalMicroSeconds32)KDefaultReportDelay );

		// ******************************************
		iLogger->Log(_L("Seting up volume ramp with duration: %d"), RampDuration.Int64());
		if (UsingRampDefault) {iLogger->Log(_L("Using default Ramp duration of: %d") , KDefaultRampDuration.Int64());}

		sndPlayer->SetVolumeRamp(RampDuration);

		CParameters *reportParams = new(ELeave)CParameters(CSimpleSoundPlayer::KPlayerActionReportVolume);
		CleanupStack::PushL(reportParams);
		CMediaEvent *mEventReportPosition = CMediaEvent::NewLC(TestModuleIf() , *iLogger, ReportDelay, ReportDelay, sndPlayer, reportParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;

		if (mEventReportPosition->GetCount() <= 0)
			{
			aResult.iResult = KErrSkippedEvents;
			}

		switch (aResult.iResult)
			{
			case KErrNone:
				iLogger->Log(_L("Test was successful"));
				aResult.iResultDes.Copy( KTestCaseResultSuccess() );
				break;
			case KErrSkippedEvents:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFailSkippedEvents() );
				break;
			default:
				iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
				aResult.iResultDes.Copy( KTestCaseResultFail() );
			}

		CleanupStack::PopAndDestroy(mEventReportPosition);
		CleanupStack::Pop(reportParams);
		CleanupStack::PopAndDestroy(sndPlayer);
		return aResult.iResult;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFilesWithPrioritiesL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TInt PriorityLow = EMdaPriorityNormal+1;
		TInt PriorityHigh = EMdaPriorityNormal+20;

		TBool UsingDefaultPriority;
		PriorityHigh = GetIntL(section, KTagPriority, UsingDefaultPriority, PriorityHigh);
		PriorityLow = GetIntL(section, KTagPriority, UsingDefaultPriority, PriorityLow, ETrue);

		if (PriorityHigh < PriorityLow)
			{
			TInt tempPriority = PriorityHigh;
			PriorityHigh = PriorityLow;
			PriorityLow = tempPriority;
			}

		TBool UsingDefaultPriorityPreference;
		TInt tmpPrefHigh = GetIntL(section, KTagPriorityPreference, UsingDefaultPriorityPreference, -1);
		TInt tmpPrefLow = GetIntL(section, KTagPriorityPreference, UsingDefaultPriorityPreference, tmpPrefHigh, ETrue);

		TMdaPriorityPreference prefHigh;
		switch (tmpPrefHigh)
			{
			case 0:
				prefHigh = EMdaPriorityPreferenceNone; break;
			case 1:
				prefHigh = EMdaPriorityPreferenceTime; break;
			case 2:
				prefHigh = EMdaPriorityPreferenceQuality; break;
			case 3:
			default:
				prefHigh = EMdaPriorityPreferenceTimeAndQuality; break;
			}
		TMdaPriorityPreference prefLow;
		switch (tmpPrefLow)
			{
			case 0:
				prefLow = EMdaPriorityPreferenceNone; break;
			case 1:
				prefLow = EMdaPriorityPreferenceTime; break;
			case 2:
				prefLow = EMdaPriorityPreferenceQuality; break;
			case 3:
			default:
				prefLow = EMdaPriorityPreferenceTimeAndQuality; break;
			}

		TFileName FileName = FileNamePtr;
		TObjectCountDown counter;

		iLogger->Log(_L("Creating playerHigh with file [%S]"), &FileName);
		CSimpleSoundPlayer *sndPlayerHigh = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);

		if ( section->GetNextLine(KTagSoundFile, FileNamePtr, ENoTag) )
			{
			iLogger->Log(_L("Second filename not found, using the same file twice"));
			}
		else
			{
			FileName=FileNamePtr;
			iLogger->Log(_L("Second filename set to [%S]") , &FileName );
			}

		// ***************
		iLogger->Log(_L("Creating playerLow with file [%S]"), &FileName);
		CSimpleSoundPlayer *sndPlayerLow = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);

		iLogger->Log(_L("Setting playerLow priority to (%d)"), PriorityLow);
		sndPlayerLow->SetPriority( PriorityLow, prefLow);

		iLogger->Log(_L("Setting playerHigh priority to (%d)"), PriorityHigh);
		sndPlayerHigh->SetPriority( PriorityHigh, prefHigh);

		sndPlayerLow->SetSupervisor(&counter);
		sndPlayerHigh->SetSupervisor(&counter);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from playerHigh, errorcode: (%d)"), sndPlayerHigh->iFinalError );
		iLogger->Log(_L("Returned from playerLow, errorcode: (%d)"), sndPlayerLow->iFinalError );

		aResult.iResult = sndPlayerHigh->iFinalError ;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayerLow);
		CleanupStack::PopAndDestroy(sndPlayerHigh);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFilesWithVolumesL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		TObjectCountDown counter;

		CSimpleSoundPlayer *sndPlayerHigh = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);
	//	TInt VolumeHigh;
	//	sndPlayerHigh->SetVolume(VolumeHigh=sndPlayerHigh->MaxVolume());
	//	iLogger->Log(_L("Creating player 1 (playerHigh) with file [%S] and volume (%d), max=(%d)"), &FileName, VolumeHigh, sndPlayerHigh->MaxVolume() );
		iLogger->Log(_L("Creating player 1 (playerHigh) with file [%S] "), &FileName);

		sndPlayerHigh->playVolume = true;

		if ( section->GetNextLine(KTagSoundFile, FileNamePtr, ENoTag) )
			{
			iLogger->Log(_L("Second filename not found, using the same file twice"));
			}
		else
			{
			FileName=FileNamePtr;
			iLogger->Log(_L("Second filename set to [%S]") , &FileName );
			}

		// ***************
		CSimpleSoundPlayer *sndPlayerLow = CSimpleSoundPlayer::NewLC( FileName , TestModuleIf() , *iLogger , EFalse);
	//	TInt VolumeLow;
	//	sndPlayerLow->SetVolume(VolumeLow=sndPlayerLow->MaxVolume()/2);
	//	iLogger->Log(_L("Creating player 2 (playerLow) with file [%S] and volume (%d), max=(%d)"), &FileName, VolumeLow, sndPlayerLow->MaxVolume() );
		iLogger->Log(_L("Creating player 2 (playerHigh) with file [%S] "), &FileName);
		sndPlayerLow->playVolume = true;

		CParameters *mPlayParameters = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPlay );
		CleanupStack::PushL(mPlayParameters);

		sndPlayerHigh->SetPlayCompleteTarget(sndPlayerLow, mPlayParameters);

		sndPlayerLow->SetSupervisor(&counter);
		sndPlayerHigh->SetSupervisor(&counter);

		iLogger->Log(_L("Starting scheduler"));


		CActiveScheduler::Start();


		iLogger->Log(_L("Returned from playerHigh, errorcode: (%d)"), sndPlayerHigh->iFinalError );
		iLogger->Log(_L("Returned from playerLow, errorcode: (%d)"), sndPlayerLow->iFinalError );

		aResult.iResult = sndPlayerHigh->iFinalError ;

		if (!sndPlayerHigh->iFinalError && !sndPlayerLow->iFinalError)
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			aResult.iResult = KErrNone;
			}
		else
			{
			if (sndPlayerHigh->iFinalError)
				{
				aResult.iResult = sndPlayerHigh->iFinalError;
				}
			if (sndPlayerLow->iFinalError)
				{
				aResult.iResult = sndPlayerLow->iFinalError;
				}

			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		CleanupStack::PopAndDestroy(mPlayParameters);
		CleanupStack::PopAndDestroy(sndPlayerLow);
		CleanupStack::PopAndDestroy(sndPlayerHigh);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}




TInt CMmfTsPlay::TestCasePlayUrlL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC UrlPtr;
	if ( !section->GetLine(KTagSoundUrl, UrlPtr, ENoTag) )
		{
		CStifItemParser *item;
		HBufC *Url = UrlPtr.AllocLC();
		TInt IapId = KUseDefaultIap;
		TPtrC MimePtr;
		HBufC *Mime;
		HBufC8 *Mime8;
		if ( ! section->GetLine(KTagMime, MimePtr, ENoTag) )
			{
			Mime = MimePtr.AllocLC();
			}
		else
			{
			Mime = KNullDesC().AllocLC();
			}

		if ( item=section->GetItemLineL(KTagIapId) , item)
			{
			CleanupStack::PushL(item);
			TInt newVal;
			if ( ! item->GetInt(KTagIapId, newVal) )
				{
				IapId = newVal;
				}
			CleanupStack::PopAndDestroy(item);
			}

		Mime8 = HBufC8::NewLC( Mime->Length() );
		TPtr8 Mime8Ptr = Mime8->Des();

		for (TInt x=0 ; x < Mime->Length() ; x++)
			{
			//TUint8 tmp8 = (TUint8)( (*Mime)[x]);
			//TBufC8<10> tmp( &tmp8 );
			TUint8 tmp8Array[2] = {0, 0};
			//tmp8Array[1]=0;
			tmp8Array[0]=(TUint8)( (*Mime)[x] );
			TBufC8<10> tmp( tmp8Array );
			Mime8Ptr += tmp.Left(1);
			}

		iLogger->Log(_L("Creating simple player with url [%S], IapId [%d] "), Url, IapId);
		iLogger->Log(_L8("   and MIME Type [%S]L(%d)") , Mime8, Mime8->Length() );
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewUrlLC( *Url, IapId, *Mime8, TestModuleIf() , *iLogger);



		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		CleanupStack::PopAndDestroy(Mime8);
		CleanupStack::PopAndDestroy(Mime);
		CleanupStack::PopAndDestroy(Url);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayUrlStopOpenL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC UrlPtr;
	TTimeIntervalMicroSeconds32 StopDelay=KDefaultStopOpenPlayDelay;

	if ( !section->GetLine(KTagSoundUrl, UrlPtr, ENoTag) )
		{
		CStifItemParser *item;
		TInt IapId = KUseDefaultIap;
		TInt IapIdB = KUseDefaultIap;

		TPtrC MimePtr;
		HBufC *Mime;
		HBufC8 *Mime8;

		HBufC *MimeB;
		HBufC8 *Mime8B;

		HBufC *Url = UrlPtr.AllocLC();
		if ( ! section->GetLine(KTagMime, MimePtr, ENoTag) )
			{
			Mime = MimePtr.AllocLC();
			}
		else
			{
			Mime = KNullDesC().AllocLC();
			}

		Mime8 = HBufC8::NewLC( Mime->Length() );
		TPtr8 Mime8Ptr = Mime8->Des();
		TInt x;
		for (x=0 ; x < Mime->Length() ; x++)
			{
			TUint8 tmp8[2];
			tmp8[0] = (TUint8)( (*Mime)[x]);
			tmp8[1] = '\0';
			TBufC8<10> tmp( tmp8 );
			Mime8Ptr += tmp.Left(1);
			}

		if ( item=section->GetItemLineL(KTagIapId) , item)
			{
			CleanupStack::PushL(item);
			TInt newVal;
			if ( ! item->GetInt(KTagIapId, newVal) )
				{
				IapId = newVal;
				}
			CleanupStack::PopAndDestroy(item);
			}

		iLogger->Log(_L("Creating simple player with url [%S], IapId [%d] "), Url, IapId);
		iLogger->Log(_L8("   and MIME Type [%S]L(%d)") , Mime8, Mime8->Length() );
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewUrlLC( *Url, IapId, *Mime8, TestModuleIf() , *iLogger);

		//*********************************************
		//Url of second file to play
		HBufC *UrlB;
		if ( ! section->GetLine(KTagMime, MimePtr, ENoTag) )
			{
			MimeB = MimePtr.AllocLC();
			}
		else
			{
			MimeB = KNullDesC().AllocLC();
			}

		Mime8B = HBufC8::NewLC( MimeB->Length() );
		TPtr8 Mime8PtrB = Mime8B->Des();
		for (x=0 ; x < MimeB->Length() ; x++)
			{
			TUint8 tmp8B = (TUint8)( (*MimeB)[x]);
			TBufC8<10> tmpB( &tmp8B );
			Mime8PtrB += tmpB.Left(1);
			}

		if ( item=section->GetItemLineL(KTagIapId) , item)
		{
			CleanupStack::PushL(item);
			TInt newValB;
			if ( ! item->GetInt(KTagIapId, newValB) )
				{
				IapIdB = newValB;
				}
			CleanupStack::PopAndDestroy(item);
		}

		CUrlParameters *stopParams ;
		if ( section->GetNextLine(KTagSoundUrl, UrlPtr, ENoTag) )
			{
			iLogger->Log(_L("Second url not found, using the same file twice"));
			UrlB = Url->AllocLC();
			}
		else
			{
			iLogger->Log(_L("Second url set to [%S]") , &UrlPtr );
			UrlB = UrlPtr.AllocLC();
			}

		stopParams = new(ELeave) CUrlParameters (CSimpleSoundPlayer::KPlayerActionStopPlayUrl, *UrlB, IapIdB, *Mime8B);
		CleanupStack::PushL(stopParams);

		TBool UsingDefaultDelay=EFalse;
		StopDelay = GetTimeIntervalL(section, KTagDelay, UsingDefaultDelay, (TTimeIntervalMicroSeconds32)KDefaultStopOpenPlayDelay);

		iLogger->Log(_L("Setting delays to stop and open url to [%d]"), StopDelay.Int());
		//*********************************************

		CMediaEvent *mEventStopPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, StopDelay, StopDelay, sndPlayer, stopParams);

		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		CleanupStack::PopAndDestroy(mEventStopPlay);
		CleanupStack::Pop(stopParams);

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(UrlB);
		CleanupStack::PopAndDestroy(Mime8B);
		CleanupStack::PopAndDestroy(MimeB);
		CleanupStack::PopAndDestroy(sndPlayer);

		CleanupStack::PopAndDestroy(Mime8);
		CleanupStack::PopAndDestroy(Mime);

		CleanupStack::PopAndDestroy(Url);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

// ** Module *****************************************************************
HBufC8 *CMmfTsPlay::GetDescriptor8LC(const TFileName &aFileName, CStifSectionParser *section)
	{
	TInt FileError, SoundSize=0;
	RFile file;
	RFs FsSession;
	HBufC8 *SoundFile;


	TInt MimeUsed = 0;
	CStifItemParser *item;
	item = section->GetItemLineL(KTagMime);
	if (item)
	{
		CleanupStack::PushL(item);
		item->GetInt(KTagMime, MimeUsed) ;
		CleanupStack::PopAndDestroy(item);
	}

	iLogger->Log(_L("Connecting to File Server Session") );
	FileError = FsSession.Connect();
	if (FileError)
		{
		iLogger->Log(_L("File error FsSession.Connect() (%d)") , FileError);
		User::LeaveIfError(FileError);
		}

	iLogger->Log(_L("Opening file to get size") );
	FileError = file.Open(FsSession, aFileName, EFileStream);
	if (FileError)
		{
		iLogger->Log(_L("File error file.Open() (%d)") , FileError);
		User::LeaveIfError(FileError);
		}
	FileError = file.Size(SoundSize);
	iLogger->Log(_L("File Size (%d)") , SoundSize);
	if (FileError)
		{
		iLogger->Log(_L("Error getting size") );
		User::LeaveIfError(FileError);
		}

//	TPtr8 helper;

	if (MimeUsed == 1)
	{
		_LIT(MP3MIME,"zzaudio/mpeg");
		SoundFile = HBufC8::NewLC(SoundSize+12);

		TPtr8 helper(SoundFile->Des());
		helper.Append (MP3MIME);
		helper.AppendFill (0x00, 1);
		TPtr8 helper1(&helper[12], SoundSize);
		User::LeaveIfError(file.Read(helper1, SoundSize));
		helper.SetLength (SoundSize+12);




	}
	else
	{
		SoundFile = HBufC8::NewLC(SoundSize);

	//	TPtr8 helper = SoundFile->Des();
		TPtr8 helper(SoundFile->Des());
//		FileError = file.Read( helper );
		User::LeaveIfError(file.Read(helper, SoundSize));


	}

	iLogger->Log(_L("Descriptor length [%d]") , SoundFile->Size() );
	file.Close();
	FsSession.Close();
	return SoundFile;
	}

TTimeIntervalMicroSeconds CMmfTsPlay::GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefaults, const TTimeIntervalMicroSeconds &aDefaultInterval)
	{
	TInt TempInterval1=0 , TempInterval2=0;
	TInt64 tmp=0;
	TInt ParseError=KErrNotFound;
	TTimeIntervalMicroSeconds ReturnValue = aDefaultInterval;
	aUsingDefaults = ETrue;

	CStifItemParser *item = aSection->GetItemLineL(aTag);
	if (item)
		{
		CleanupStack::PushL(item);
		ParseError = item->GetInt(aTag, TempInterval1) ;
		if (!ParseError)
			{
			//First delay found.
			aUsingDefaults = EFalse;
			tmp = TempInterval1*1000000;	//convert from seconds to microseconds

			if ( ! item->GetNextInt(TempInterval2) )
				{
				tmp += TempInterval2*1000;	//add miliseconds
				if ( ! item->GetNextInt(TempInterval2) )
					{
					tmp += TempInterval2;
					}
				}
			ReturnValue = tmp;
			}
		CleanupStack::PopAndDestroy(item);
		}
	if (ParseError) {iLogger->Log(_L("Error retrieving interval, [%S] not found"), &aTag);}
	iLogger->Log(_L("Interval retrieved: (%d)") , ReturnValue.Int64());
	return ReturnValue;
	}

	/*TTimeIntervalMicroSeconds CMmfTsPlay::GetDelayL(CStifSectionParser *aSection, const TDesC &aTag)
{return 0;}*/

TTimeIntervalMicroSeconds32 CMmfTsPlay::GetTimeIntervalL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TTimeIntervalMicroSeconds32 &aDefaultDelay)
	{
	TInt ParseError=KErrNotFound;
	TInt TempDelay=0;
	TTimeIntervalMicroSeconds32 ReturnValue=aDefaultDelay;
	CStifItemParser *item = aSection->GetItemLineL(aTag);
	if (item)
		{
		CleanupStack::PushL(item);
		ParseError = item->GetInt(aTag, TempDelay) ;
		if (ParseError)
			{	//Delay not found, using defaults
			aUsingDefault=ETrue;
			}
		else
			{	//Delay found.
			aUsingDefault=EFalse;
			ReturnValue=TempDelay;
			}
		CleanupStack::PopAndDestroy(item);
		}
		if (ParseError) {iLogger->Log(_L("Error retrieving report delay, [%S] not found") , &aTag);}
	return ReturnValue;
	}

TInt CMmfTsPlay::RunTestCaseFunctionL(TInt id , CStifSectionParser *section , TTestResult &aResult)
	{
	switch (id)
		{
		case KCasePlayFile:
			return TestCasePlayFileL(section, aResult);
		case KCaseGetBitRate:
			return TestCaseGetBitRateL(section, aResult);
		case KCaseGetDRMCmd:
			return TestCaseGetDRMCmdL(section, aResult);
		case KCaseCancelNotification:
			return TestCaseCancelNotificationL(section, aResult);
		case KCaseCtrImpInfo:
			return TestCaseCtrImpInfoL(section, aResult);
		case KCaseSet_Priority:
			return TestCaseSet_PriorityL(section, aResult);
		case KCasePlayFileWithRamp:
			return TestCasePlayFileWithVolumeRampL(section, aResult);
		case KCasePlayFileWithRepeats:
			return TestCasePlayFileWithRepeatsL(section, aResult);
		case KCasePlayFileWithPause:
			return TestCasePlayFileWithPauseL(section, aResult);
		case KCasePlayFileWithPosition:
			return TestCasePlayFileWithPositionL(section, aResult);
		case KCaseGetLoadPercentage:
			return TestCaseGetLoadPercentageL(section, aResult);
		case KCasePlayFileWithBalance:
			return TestCasePlayFileWithBalanceL(section, aResult);
		case KCasePlayFileStopOpen:
			return TestCasePlayFileStopOpenL(section, aResult);
		case KCaseNewFilePlayer:
			return TestCaseNewFilePlayerL(section, aResult);
		case KCaseNewDesPlayer:
			return TestCaseNewDesPlayerL(section, aResult);
		case KCaseNewDesReadOnlyPlayer:
			return TestCaseNewDesReadOnlyPlayerL(section, aResult);
		case KCasePlayFileReportDuration:
			return TestCasePlayFileReportDurationL(section, aResult);
		case KCasePlayFileReportPosition:
			return TestCasePlayFileReportPositionL(section, aResult);
		case KCasePlayFileWindow:
			return TestCasePlayFileWindowL(section, aResult);
		case KCasePlayFileClearWindow:
			return TestCasePlayFileWindowClearL(section, aResult);
		case KCasePlayFilesWithPriorities:
			return TestCasePlayFilesWithPrioritiesL(section, aResult);
		case KCasePlayFilesWithVolumes:
			return TestCasePlayFilesWithVolumesL(section, aResult);

		case KCasePlayDes:
			return TestCasePlayDesL(section, aResult);
		case KCasePlayDesStopOpen:
			return TestCasePlayDesStopOpenL(section, aResult);

		case KCasePlayUrl:
			return TestCasePlayUrlL(section, aResult);
		case KCasePlayUrlStopOpen:
			return TestCasePlayUrlStopOpenL(section, aResult);

		case KCasePlayFileMetaInfo:
			return TestCasePlayFileMetaInfoL(section, aResult);

		case KCaseImplementationInformation:
			return TestCasePlayFileFormatsL(section, aResult);

		case KCasePlayHandler:
			return TestCasePlayHandlerL(section, aResult);
		case KCasePlayHandlerStopOpen:
			return TestCasePlayHandlerStopOpenL(section, aResult);
		case KCasePlayHandlerWithPause:
			return TestCasePlayHandlerWithPauseL(section, aResult);

		case KCasePlayHandlerWithRamp:
			return TestCasePlayHandlerWithRampL(section, aResult);
		case KCasePlayHandlerWithSetPosition:
			return TestCasePlayHandlerWithSetPositionL(section, aResult);
		case KCasePlayHandlerWithGetPosition:
			return TestCasePlayHandlerWithGetPositionL(section, aResult);
		case KCasePlayHandlerWithDuration:
			return TestCasePlayHandlerWithDurationL(section, aResult);
		case KCasePlayHandlerWithWindow:
			return TestCasePlayHandlerWithWindowL(section, aResult);
		case KCasePlayHandlerWithClearWindow:
			return TestCasePlayHandlerWithClearWindowL(section, aResult);
		case KCasePlayHandlerWithPriorities:
			return TestCasePlayHandlerWithPrioritiesL(section, aResult);


		case KCaseOpenFile:
            return TestCaseOpenFileL(section, aResult);
        case KCase_PlayFile:
            return TestCasePlayFileL(section, aResult, KSubCaseNone);
        case KCasePlayFileHandler:
            return TestCasePlayFileHandlerL(section, aResult, KSubCaseNone);
        case KCasePlayFileSource:
            return TestCasePlayFileSourceL(section, aResult, KSubCaseNone);
        case KCasePlayDesc:
            return TestCasePlayDescL(section, aResult, KSubCaseNone);

        case KCaseRegNotification:
			return TestCaseRegNotificationL(section, aResult);
		case KCaseRegLoadNotification:
			return TestCaseRegLoadNotificationL(section, aResult);


		// Binh
		case KCaseMetaDataData:
		    return TestMetaDataDataL(section, aResult, EFalse);
		case KCaseMetaDataDataFileHandler:
		    return TestMetaDataDataL(section, aResult, ETrue);
		case KCasePlayFileMetaInfoFileHandler:
			return TestCasePlayFileMetaInfoL(section, aResult, ETrue);
		case KCaseMetaDataCount:
		    return TestMetaDataCountL(section, aResult);
		case KCaseMetaDataCountFileHandler:
		    return TestMetaDataCountL(section, aResult, ETrue);
        case KCasePlayFileWithRepeatForever:
            return TestCasePlayFileWithRepeatForeverL(section, aResult);
        case KCasePlayFileWithRepeatsWithPause:
            return TestCasePlayFileWithRepeatsWithPauseL(section, aResult);
        case KCasePlayFileWithRepeatsInPlaywindow:
            return TestCasePlayFileWithRepeatsInPlaywindowL(section, aResult);
        case KCasePlayFileWithRepeatsInPlaywindowPause:
            return TestCasePlayFileWithRepeatsInPlaywindowPauseL(section,
                    aResult);
        case KCasePlayFileWithRepeatForeverInPlaywindow:
            return TestCasePlayFileWithRepeatForeverInPlaywindowL(section,
                    aResult);

		case KCaseAudioOutput:
		    return TestAudioOutputL(section, aResult);
    	case KCaseLoopPlayTest:
		    return TestLoopPlayL(section,aResult);
		}

	aResult.iResultDes.Copy(KConfigInvalid());
	aResult.iResult = KErrNotExecuted;
	return KErrConfigInvalid;
	}

TBool CMmfTsPlay::IsDurationInRange
	(	TInt aRepeatTimes, TTimeIntervalMicroSeconds aToneDuration, TTimeIntervalMicroSeconds aRepeatSilence,
		TTimeIntervalMicroSeconds aErrorRange , TTime aStartTime , TTime aEndTime )
	{
	TInt64 tmp=aToneDuration.Int64();
	TInt64 tmp2=aRepeatSilence.Int64();

	tmp*=(aRepeatTimes+1);  tmp2*=(aRepeatTimes);
	TTimeIntervalMicroSeconds ExpectedDuration = tmp+tmp2;

	TTime EndTimeRange1 = aStartTime;
	EndTimeRange1+= ExpectedDuration;
	TTime EndTimeRange2 = EndTimeRange1;
	EndTimeRange1-=aErrorRange;
	EndTimeRange2+=aErrorRange;

	iLogger->Log(_L("Actual play duration (%d)") , (aEndTime.MicroSecondsFrom(aStartTime)).Int64() );
	iLogger->Log(_L("Expected duration (%d)") , ExpectedDuration.Int64());
	iLogger->Log(_L("Error range (%d)") , aErrorRange.Int64());
	return (EndTimeRange1<aEndTime && aEndTime<EndTimeRange2) ;
	}






TInt CMmfTsPlay::TestCaseOpenFileL(CStifSectionParser *section ,
        TTestResult &aResult)
{

    RDebug::Print(_L("JZ - CMmfTsPlay::TestCaseOpenFileL:"));

    TPtrC fileNamePtr;

    if ( !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) )
    {
        TFileName fileName = fileNamePtr;
        iLogger->Log(_L("Opening simple player with file [%S]"), &fileNamePtr);

        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(TestModuleIf() , *iLogger);
        CleanupStack::PushL(sndPlayer);



        if ((sndPlayer->OpenFile (fileName)) == KErrNone)
        {
            CActiveScheduler::Start();
        }


        if (sndPlayer->iFinalError)
        {
            aResult.iResult = sndPlayer->iFinalError;
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
        }
        else
        {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
        }


        CleanupStack::PopAndDestroy(sndPlayer);

        iLogger->Log(_L("TestCaseOpenClient - Done"));
    }
    else
    {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
    }

    return KErrExecuted;

}


TInt CMmfTsPlay::TestCasePlayFileL(CStifSectionParser *section ,
        TTestResult &aResult, TInt aSubCase)
{

    RDebug::Print(_L("JZ - CMmfTsPlay::TestCasePlayFileL:"));

    TPtrC fileNamePtr;

    if ( !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) )
    {
        TFileName fileName = fileNamePtr;
        iLogger->Log(_L("Opening simple player with file [%S]"), &fileNamePtr);

        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(TestModuleIf() , *iLogger);
        CleanupStack::PushL(sndPlayer);



        if ((sndPlayer->OpenFile (fileName)) == KErrNone)
        {
            CActiveScheduler::Start();
        }

        if (sndPlayer->iFinalError)
        {
            aResult.iResult = sndPlayer->iFinalError;
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
        }
        else
        {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
        }


        CleanupStack::PopAndDestroy(sndPlayer);

        iLogger->Log(_L("CMmfTsPlay - Done"));
    }
    else
    {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
    }

    return KErrExecuted;

}


TInt CMmfTsPlay::TestCasePlayFileHandlerL(CStifSectionParser *section ,
        TTestResult &aResult, TInt aSubCase)
{

    RDebug::Print(_L("JZ - CMmfTsPlay::TestCasePlayFileHandlerL:"));

    TPtrC fileNamePtr;
    RFile file;
    RFs fileServer;

    User::LeaveIfError(fileServer.Connect());

    if ( !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) )
    {
        TFileName fileName = fileNamePtr;
        iLogger->Log(_L("Opening simple player with file [%S]"), &fileNamePtr);

        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(TestModuleIf() , *iLogger);
        CleanupStack::PushL(sndPlayer);


        TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {
            iLogger->Log(_L("Opening RFile - failed, err=%d"), err);
            file.Close();
            fileServer.Close();
            User::Leave(err);
        }

        if ((sndPlayer->OpenWithFileHandler (file)) == KErrNone)
        {
            file.Close();
            CActiveScheduler::Start();
        }
        else
        {
            file.Close();
            sndPlayer->iFinalError = KErrIncorrect;
        }


        if (sndPlayer->iFinalError)
        {
            aResult.iResult = sndPlayer->iFinalError;
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
        }
        else
        {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
        }


        fileServer.Close();

        CleanupStack::PopAndDestroy(sndPlayer);

        iLogger->Log(_L("CMmfTsPlay - Done"));
    }
    else
    {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
    }

    return KErrExecuted;

}



TInt CMmfTsPlay::TestCasePlayFileSourceL(CStifSectionParser *section ,
        TTestResult &aResult, TInt aSubCase)
{

    RDebug::Print(_L("JZ - CMmfTsPlay::TestCasePlayFileSourceL:"));

    TPtrC fileNamePtr;
    RFile file;
    RFs fileServer;

    User::LeaveIfError(fileServer.Connect());

    if ( !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) )
    {
        TFileName fileName = fileNamePtr;
        iLogger->Log(_L("Opening simple player with file [%S]"), &fileNamePtr);

        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(TestModuleIf() , *iLogger);
        CleanupStack::PushL(sndPlayer);


        TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {
            iLogger->Log(_L("Opening RFile - failed, err=%d"), err);
            file.Close();
            fileServer.Close();
            User::Leave(err);
        }


        TMMFileHandleSource filesource(file, ContentAccess::KDefaultContentObject, ContentAccess::EPlay);

        if ((sndPlayer->OpenWithFileSource (filesource)) == KErrNone)
        {
            file.Close();
            CActiveScheduler::Start();
        }
        else
        {
            file.Close();
            sndPlayer->iFinalError = KErrIncorrect;
        }

        if (sndPlayer->iFinalError)
        {
            aResult.iResult = sndPlayer->iFinalError;
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
        }
        else
        {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
        }

        fileServer.Close();

        CleanupStack::PopAndDestroy(sndPlayer);

        iLogger->Log(_L("CMmfTsPlay - Done"));
    }
    else
    {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
    }

    return KErrExecuted;

}



TInt CMmfTsPlay::TestCasePlayDescL(CStifSectionParser *section ,
        TTestResult &aResult, TInt aSubCase)
{

    RDebug::Print(_L("JZ - CMmfTsPlay::TestCasePlayDescL:"));

    TPtrC fileNamePtr;
    RFile file;
    RFs fileServer;
    HBufC8* buffer = NULL;

    User::LeaveIfError(fileServer.Connect());

    if ( !section->GetLine(KTagSoundFile, fileNamePtr, ENoTag) )
    {
        TFileName fileName = fileNamePtr;
        iLogger->Log(_L("Opening simple player with file [%S]"), &fileNamePtr);

        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(TestModuleIf() , *iLogger);
        CleanupStack::PushL(sndPlayer);



        TInt err = file.Open(fileServer, fileName, EFileShareReadersOnly);
        if (err != KErrNone)
        {
            iLogger->Log(_L("Opening RFile - failed, err=%d"), err);
            file.Close();
            fileServer.Close();
            User::Leave(err);
        }

        TInt size;

        if ((err = file.Size(size)) == KErrNone)
        {
            TRAP(err, buffer = HBufC8::NewL(size));
            if (err != KErrNone)
            {
                iLogger->Log(_L("HBufC8::NewL, err=%d"), err);
                file.Close();
                fileServer.Close();
                User::Leave(err);
            }

            TPtr8 bufferpointer = buffer->Des();
            if ((err = file.Read(bufferpointer)) != KErrNone)
            {
                iLogger->Log(_L("RFile.Read, err=%d"), err);
                file.Close();
                fileServer.Close();
                delete buffer;
                User::Leave(err);
            }

            if ((sndPlayer->OpenWithDesc (*buffer)) == KErrNone)
            {
                file.Close();
                CActiveScheduler::Start();
            }
            else
            {
                file.Close();
                sndPlayer->iFinalError = KErrIncorrect;
            }
        }

        if (sndPlayer->iFinalError)
        {
            aResult.iResult = sndPlayer->iFinalError;
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
        }
        else
        {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
        }

        fileServer.Close();
        delete buffer;

        CleanupStack::PopAndDestroy(sndPlayer);

        iLogger->Log(_L("CMmfTsPlay - Done"));
    }
    else
    {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
    }

    return KErrExecuted;

}

TInt CMmfTsPlay::TestCaseRegNotificationL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
	    sndPlayer -> RegNotification();



		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();



		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}


TInt CMmfTsPlay::TestCaseRegLoadNotificationL(CStifSectionParser *section , TTestResult &aResult)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
		CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger);
	    sndPlayer -> RegLoadNotification();



		CleanupStack::PushL(sndPlayer);
		iLogger->Log(_L("Starting scheduler"));

		CActiveScheduler::Start();



		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError ;

		TBool DontCheckExpectedDuration;
		TTimeIntervalMicroSeconds ErrorRange=GetTimeIntervalL(section, KTagErrorRange, DontCheckExpectedDuration);
		if (!DontCheckExpectedDuration && !aResult.iResult)
			{
			TBool DontUseExpectedDuration;
			TTimeIntervalMicroSeconds ExpectedDuration=GetTimeIntervalL(section, KTagExpectedDuration, DontUseExpectedDuration);
			if (!IsDurationInRange(0, (DontUseExpectedDuration ? sndPlayer->GetDuration() : ExpectedDuration), TTimeIntervalMicroSeconds(0), ErrorRange, sndPlayer->GetPlayStartTime(), sndPlayer->GetPlayStopTime() ) )
				{
				iLogger->Log(_L("Duration too different"));
				aResult.iResult = KErrExpectedValueDifferent;
				}
			}

		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}

		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;

        }
    else
        {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        }
    }

TInt CMmfTsPlay::TestCasePlayFileWithRepeatForeverL(CStifSectionParser *section, TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    TInt Repeats = KDefaultRepeatForever;
    TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
    TInt ParseError = KErrNone;
    CStifItemParser *item;

    if (!section->GetLine(KTagSoundFile, FileNamePtr, ENoTag))
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(FileName, TestModuleIf(), *iLogger);
        CleanupStack::PushL(sndPlayer);
        //Setup repeat count
        ParseError = KErrNotFound;
        item = section->GetItemLineL(KTagRepeatTimes);
        if (item)
            {
            CleanupStack::PushL(item);
            ParseError = item->GetInt(KTagRepeatTimes, Repeats);
            CleanupStack::PopAndDestroy(item);
            }
        iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
        if (ParseError)
            {
            iLogger->Log(_L("Using default value of %d"), KDefaultRepeats);
            Repeats = KDefaultRepeats;
            }
        TBool UsingDefault;
        SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
        iLogger->Log(_L("Setting interval to %d"), SilenceDuration.Int64());
        if (UsingDefault)
            {
            iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64());
            }
		sndPlayer->SetRepeats(Repeats, SilenceDuration);
		sndPlayer->SetVolumeRamp(100000);
		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();
		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
        sndPlayer->SetRepeats(0,0);
		CleanupStack::PopAndDestroy(sndPlayer);
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;    //aResult.iResult;
		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileWithRepeatsWithPauseL(CStifSectionParser *section, TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    TInt Repeats = KDefaultRepeats;
    TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
    TTimeIntervalMicroSeconds32 PauseDelay = KDefaultPauseDelay;
    TTimeIntervalMicroSeconds32 PlayDelay = KDefaultPlayDelay;
    TInt ParseError = KErrNone;
    CStifItemParser *item;
    if (!section->GetLine(KTagSoundFile, FileNamePtr, ENoTag))
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(FileName, TestModuleIf(), *iLogger);
        CleanupStack::PushL(sndPlayer);
        //Setup repeat count
        ParseError = KErrNotFound;
        item = section->GetItemLineL(KTagRepeatTimes);
        if (item)
            {
            CleanupStack::PushL(item);
            ParseError = item->GetInt(KTagRepeatTimes, Repeats);
            CleanupStack::PopAndDestroy(item);
            }
        iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
        if (ParseError)
            {
            iLogger->Log(_L("Using default value of %d"), KDefaultRepeats);
            Repeats = KDefaultRepeats;
            }
        TBool UsingDefault;
        SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
        iLogger->Log(_L("Setting interval to %d"), SilenceDuration.Int64());
        if (UsingDefault)
            {
            iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64());
            }
        PauseDelay = GetTimeIntervalL(section, KTagPauseDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPauseDelay);
        if (UsingDefault) {iLogger->Log(_L("Error retrieving pause delay, using default"));}
        PlayDelay = GetTimeIntervalL(section, KTagPlayDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPlayDelay);
        if (UsingDefault) {iLogger->Log(_L("Error retrieving play delay, using default"));}
        sndPlayer->SetRepeats(Repeats, SilenceDuration);
        sndPlayer->SetVolumeRamp(100000);
        sndPlayer->iDontPlayAudioClip = ETrue; // don't start the play back after initialize complete
        iLogger->Log(_L("Starting scheduler"));
        CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        TTimeIntervalMicroSeconds dur = sndPlayer->GetDuration();
        iLogger->Log(_L("Duration of the sound file [%d]"), dur.Int64());
        iLogger->Log(_L("Setting Pause and Play delays to [%d] and [%d]"), PauseDelay.Int(), PlayDelay.Int());
        TTimeIntervalMicroSeconds32 interval = TTimeIntervalMicroSeconds32(dur.Int64() + SilenceDuration.Int64() + (PlayDelay.Int() - PauseDelay.Int()));
        iLogger->Log(_L("Interval time between Pause Play operations [%d]"), interval.Int());
        sndPlayer->Play(); // Start the loop play
        CParameters *pauseParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPause );
        CleanupStack::PushL(pauseParams);
        CParameters *playParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPlay );
        CleanupStack::PushL(playParams);
        CMediaEvent* mEventPause = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PauseDelay, interval, sndPlayer, pauseParams);
        CMediaEvent* mEventPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PlayDelay, interval, sndPlayer, playParams);
        CActiveScheduler::Start(); // start the playback and loop play
        sndPlayer->SetRepeats(0,0);
        iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
        aResult.iResult = sndPlayer->iFinalError;
        if (aResult.iResult)
            {
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
            }
        else
            {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
            }
        CleanupStack::PopAndDestroy(mEventPlay);
        CleanupStack::PopAndDestroy(mEventPause);
        CleanupStack::Pop(playParams);
        CleanupStack::Pop(pauseParams);
        CleanupStack::PopAndDestroy(sndPlayer);
        return KErrExecuted;
        }
    else
        {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        }
    }

TInt CMmfTsPlay::TestCasePlayFileWithRepeatsInPlaywindowL(CStifSectionParser *section, TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    TInt Repeats = KDefaultRepeats;
    TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
    TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
    TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
    TInt ParseError = KErrNone;
    CStifItemParser *item;
    if (!section->GetLine(KTagSoundFile, FileNamePtr, ENoTag))
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(FileName, TestModuleIf(), *iLogger);
        CleanupStack::PushL(sndPlayer);
        //Setup repeat count
        ParseError = KErrNotFound;
        item = section->GetItemLineL(KTagRepeatTimes);
        if (item)
            {
            CleanupStack::PushL(item);
            ParseError = item->GetInt(KTagRepeatTimes, Repeats);
            CleanupStack::PopAndDestroy(item);
            }
        iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
        if (ParseError)
            {
            iLogger->Log(_L("Using default value of %d"), KDefaultRepeats);
            Repeats = KDefaultRepeats;
            }
        TBool UsingStartDefault, UsingEndDefault, UsingDefault;
        TInt pwErr;

        SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
        iLogger->Log(_L("Silence Duration [%d]"), SilenceDuration.Int64());
        if (UsingDefault) { iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64()); }

        StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() - 1E6));
        if (UsingStartDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() - 1E6)); }
        iLogger->Log(_L("PlayWindow Start Position [%d]"), StartPosition.Int64());

        EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() + 1E6));
        if (UsingEndDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() + 1E6)); }
        iLogger->Log(_L("PlayWindow End Position [%d]"), EndPosition.Int64());


        // Case 1: SetPlayWindow - InitializeComplete - Play
        sndPlayer->SetRepeats(Repeats, SilenceDuration);
        sndPlayer->SetVolumeRamp(100000);
        sndPlayer->playWindow = ETrue;
        sndPlayer->startPosition = StartPosition;
        sndPlayer->endPosition = EndPosition;
        iLogger->Log(_L("Starting scheduler"));
        CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event

        // Case 2: InitializeComplete - SetPlayWindow - Play
        // sndPlayer->SetRepeats(Repeats, SilenceDuration);
        // sndPlayer->SetVolumeRamp(100000);
        // sndPlayer->iDontPlayAudioClip = ETrue; // don't start the play back after initialize complete
        // iLogger->Log(_L("Starting scheduler"));
        // CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        // pwErr = sndPlayer->SetPlayWindow(StartPosition, EndPosition);
        // iLogger->Log(_L("SetPlayWindow err=[%d]"), pwErr);
        // iLogger->Log(_L("Starting playback"));
        // sndPlayer->Play(); // Start the loop play
        // iLogger->Log(_L("Starting scheduler"));
        // CActiveScheduler::Start();
        // Both the cases 1 & 2 are same ... the result is same.

        // This should work .....
        // Case 3: InitializeComplete - Play - SetPlayWindow
        // sndPlayer->SetRepeats(Repeats, SilenceDuration);
        // sndPlayer->SetVolumeRamp(100000);
        // sndPlayer->iDontPlayAudioClip = ETrue; // don't start the play back after initialize complete
        // iLogger->Log(_L("Starting scheduler"));
        // CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        // iLogger->Log(_L("Starting playback"));
        // sndPlayer->Play(); // Start the loop play
        // If we call SetPlayWindow immediately after Play(), DevsoundAudioOutput is not yet initialized
        // due to which the internal states are changed randomly leading to a halt in the playback !!!
        // pwErr = sndPlayer->SetPlayWindow(StartPosition, EndPosition);
        // iLogger->Log(_L("SetPlayWindow err=[%d]"), pwErr);
        // iLogger->Log(_L("Starting scheduler"));
        // CActiveScheduler::Start();

        iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;

		sndPlayer->SetRepeats(0,0);
        sndPlayer->playWindow = EFalse;
        pwErr = sndPlayer->ClearPlayWindow();
		iLogger->Log(_L("ClearPlayWindow err=[%d]"), pwErr);

		CleanupStack::PopAndDestroy(sndPlayer);
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;    //aResult.iResult;
		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

TInt CMmfTsPlay::TestCasePlayFileWithRepeatsInPlaywindowPauseL(CStifSectionParser *section, TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    TInt Repeats = KDefaultRepeats;
    TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
    TTimeIntervalMicroSeconds32 PauseDelay = KDefaultPauseDelay;
    TTimeIntervalMicroSeconds32 PlayDelay = KDefaultPlayDelay;
    TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
    TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
    TInt ParseError = KErrNone;
    CStifItemParser *item;
    if (!section->GetLine(KTagSoundFile, FileNamePtr, ENoTag))
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(FileName, TestModuleIf(), *iLogger);
        CleanupStack::PushL(sndPlayer);
        //Setup repeat count
        ParseError = KErrNotFound;
        item = section->GetItemLineL(KTagRepeatTimes);
        if (item)
            {
            CleanupStack::PushL(item);
            ParseError = item->GetInt(KTagRepeatTimes, Repeats);
            CleanupStack::PopAndDestroy(item);
            }
        iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
        if (ParseError)
            {
            iLogger->Log(_L("Using default value of %d"), KDefaultRepeats);
            Repeats = KDefaultRepeats;
            }
        TBool UsingStartDefault, UsingEndDefault, UsingDefault;
        SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
        iLogger->Log(_L("Silence Duration [%d]"), SilenceDuration.Int64());
        if (UsingDefault) { iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64()); }
        StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() - 1E6));
        if (UsingStartDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() - 1E6)); }
        iLogger->Log(_L("PlayWindow Start Position [%d]"), StartPosition.Int64());
        EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() + 1E6));
        if (UsingEndDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() + 1E6)); }
        iLogger->Log(_L("PlayWindow End Position [%d]"), EndPosition.Int64());
        PauseDelay = GetTimeIntervalL(section, KTagPauseDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPauseDelay);
        if (UsingDefault) {iLogger->Log(_L("Error retrieving pause delay, using default"));}
        PlayDelay = GetTimeIntervalL(section, KTagPlayDelay, UsingDefault, (TTimeIntervalMicroSeconds32)KDefaultPlayDelay);
        if (UsingDefault) {iLogger->Log(_L("Error retrieving play delay, using default"));}
        iLogger->Log(_L("Setting Pause and Play delays to [%d] and [%d]"), PauseDelay.Int(), PlayDelay.Int());
        TTimeIntervalMicroSeconds32 interval = TTimeIntervalMicroSeconds32((EndPosition.Int64() - StartPosition.Int64()) + SilenceDuration.Int64() + (PlayDelay.Int() - PauseDelay.Int()));
        iLogger->Log(_L("Interval time between Pause Play operations [%d]"), interval.Int());
        sndPlayer->iDontPlayAudioClip = ETrue; // don't start the play back after initialize complete
        iLogger->Log(_L("Starting scheduler"));
        sndPlayer->SetRepeats(Repeats, SilenceDuration);
        sndPlayer->SetVolumeRamp(100000);
        sndPlayer->playWindow = ETrue;
        sndPlayer->startPosition = StartPosition;
        sndPlayer->endPosition = EndPosition;
        CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        sndPlayer->Play(); // Start the loop play in the play window
        // TInt pwErr = sndPlayer->SetPlayWindow(StartPosition, EndPosition);
        /// iLogger->Log(_L("SetPlayWindow err=[%d]"), pwErr);
        /*********************************************/
        /* For generating the Pause event during playback */
        /*********************************************/
        CParameters *pauseParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPause );
        CleanupStack::PushL(pauseParams);
        CParameters *playParams = new (ELeave) CParameters( CSimpleSoundPlayer::KPlayerActionPlay );
        CleanupStack::PushL(playParams);
        CMediaEvent* mEventPause = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PauseDelay, interval, sndPlayer, pauseParams);
        CMediaEvent* mEventPlay = CMediaEvent::NewLC(TestModuleIf() , *iLogger, PlayDelay, interval, sndPlayer, playParams);
        // Set the play window ONLY after the and playback is started.
        iLogger->Log(_L("Starting scheduler"));
        CActiveScheduler::Start();
        iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
        sndPlayer->SetRepeats(0,0);
        sndPlayer->playWindow = EFalse;
        TInt pwErr = sndPlayer->ClearPlayWindow();
        iLogger->Log(_L("ClearPlayWindow err=[%d]"), pwErr);
        aResult.iResult = sndPlayer->iFinalError;
        if (aResult.iResult)
            {
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
            }
        else
            {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
            }
        CleanupStack::PopAndDestroy(mEventPlay);
        CleanupStack::PopAndDestroy(mEventPause);
        CleanupStack::Pop(playParams);
        CleanupStack::Pop(pauseParams);
        CleanupStack::PopAndDestroy(sndPlayer);
        return KErrExecuted;
        }
    else
        {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        }
    }

TInt CMmfTsPlay::TestCasePlayFileWithRepeatForeverInPlaywindowL(CStifSectionParser *section, TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    TInt Repeats = KDefaultRepeatForever;
    TTimeIntervalMicroSeconds SilenceDuration = KDefaultSilenceDuration;
    TTimeIntervalMicroSeconds StartPosition = KDefaultPosition;
    TTimeIntervalMicroSeconds EndPosition = KDefaultPosition;
    TInt ParseError = KErrNone;
    CStifItemParser *item;
    if (!section->GetLine(KTagSoundFile, FileNamePtr, ENoTag))
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL(FileName, TestModuleIf(), *iLogger);
        CleanupStack::PushL(sndPlayer);
        //Setup repeat count
        ParseError = KErrNotFound;
        item = section->GetItemLineL(KTagRepeatTimes);
        if (item)
            {
            CleanupStack::PushL(item);
            ParseError = item->GetInt(KTagRepeatTimes, Repeats);
            CleanupStack::PopAndDestroy(item);
            }
        iLogger->Log(_L("Setting up repeat count to %d"), Repeats);
        if (ParseError)
            {
            iLogger->Log(_L("Using default value of %d"), KDefaultRepeats);
            Repeats = KDefaultRepeats;
            }
        TBool UsingStartDefault, UsingEndDefault, UsingDefault;
        SilenceDuration = GetTimeIntervalL(section, KTagSilenceDuration, UsingDefault, KDefaultSilenceDuration);
        iLogger->Log(_L("Silence Duration [%d]"), SilenceDuration.Int64());
        if (UsingDefault) { iLogger->Log(_L("Using default of %d"), KDefaultSilenceDuration.Int64()); }
        StartPosition = GetTimeIntervalL(section, KTagWindowStart, UsingStartDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() - 1E6));
        if (UsingStartDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() - 1E6)); }
        iLogger->Log(_L("PlayWindow Start Position [%d]"), StartPosition.Int64());
        EndPosition = GetTimeIntervalL(section, KTagWindowEnd, UsingEndDefault, (TTimeIntervalMicroSeconds) (KDefaultPosition.Int64() + 1E6));
        if (UsingEndDefault) { iLogger->Log(_L("Using default of %d"), (KDefaultPosition.Int64() + 1E6)); }
        iLogger->Log(_L("PlayWindow End Position [%d]"), EndPosition.Int64());
        sndPlayer->iDontPlayAudioClip = ETrue; // don't start the play back after initialize complete
        sndPlayer->SetRepeats(Repeats, SilenceDuration);
        sndPlayer->SetVolumeRamp(100000);
        sndPlayer->playWindow = ETrue;
        sndPlayer->startPosition = StartPosition;
        sndPlayer->endPosition = EndPosition;
        iLogger->Log(_L("Starting scheduler"));
        CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        iLogger->Log(_L("Starting playback"));
        sndPlayer->Play(); // Start the loop play
        // TInt pwErr = sndPlayer->SetPlayWindow(StartPosition, EndPosition);
        // iLogger->Log(_L("SetPlayWindow err=[%d]"), pwErr);
        iLogger->Log(_L("Starting scheduler"));
        CActiveScheduler::Start(); // initialize the player and wait for the intialize complete event
        iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
        sndPlayer->SetRepeats(0,0);
        sndPlayer->playWindow = EFalse;
        sndPlayer->ClearPlayWindow();
		CleanupStack::PopAndDestroy(sndPlayer);
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		return KErrExecuted;    //aResult.iResult;
		}
	else
		{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}
TInt CMmfTsPlay::TestAudioOutputL(CStifSectionParser *section , TTestResult &aResult)
    {
    TPtrC FileNamePtr;
    if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
        {
        TFileName FileName = FileNamePtr;
        iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
        CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger, EFalse);
                        
        CleanupStack::PushL(sndPlayer);
        iLogger->Log(_L("Starting scheduler"));

        CActiveScheduler::Start();

        iLogger->Log(_L("calling AudioOutputCreationL"));
        sndPlayer->AudioOutputCreationL();
        
        
        iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
        aResult.iResult = sndPlayer->iFinalError ;

        if (aResult.iResult)
            {
            iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
            aResult.iResultDes.Copy(KTestCaseResultFail());
            }
        else
            {
            iLogger->Log(_L("Test was successful"));
            aResult.iResultDes.Copy(KTestCaseResultSuccess());
            }

        CleanupStack::PopAndDestroy(sndPlayer);
        return KErrExecuted;

        }
    else
        {
        //The file name of the clip to play is missing
        aResult.iResultDes.Copy(KConfigInvalid());
        aResult.iResult = KErrNotExecuted;
        return KErrConfigInvalid;
        } 
    
  }
TInt CMmfTsPlay::TestLoopPlayL(CStifSectionParser *section , TTestResult &aResult)
    {
        TPtrC FileNamePtr;
        if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
            {
            TFileName FileName = FileNamePtr;
            iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
            CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewL( FileName, TestModuleIf() , *iLogger,ETrue);
              
            iLogger->Log(_L("calling loopplay"));
            sndPlayer->LoopPlayL();
            
            CleanupStack::PushL(sndPlayer);
            iLogger->Log(_L("Starting scheduler"));
                         
            CActiveScheduler::Start();
      
            iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
            aResult.iResult = sndPlayer->iFinalError ;
            
            CleanupStack::PopAndDestroy(sndPlayer);
            
            if (aResult.iResult)
                {
                iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
                aResult.iResultDes.Copy(KTestCaseResultFail());
                }
            else
                {
                iLogger->Log(_L("Test was successful"));
                aResult.iResultDes.Copy(KTestCaseResultSuccess());
                }

            //CleanupStack::PopAndDestroy(sndPlayer);
            return KErrExecuted;

            }
        else
            {
            //The file name of the clip to play is missing
            aResult.iResultDes.Copy(KConfigInvalid());
            aResult.iResult = KErrNotExecuted;
            return KErrConfigInvalid;
            } 
        
    
    }

