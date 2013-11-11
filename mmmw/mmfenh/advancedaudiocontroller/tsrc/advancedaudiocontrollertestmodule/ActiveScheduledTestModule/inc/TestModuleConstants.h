/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Scheduler Test Module
*
*/


#include <MdaAudioTonePlayer.h>

#ifndef TEST_MODULE_CONSTANTS_H
#define TEST_MODULE_CONSTANTS_H

	//KErrConfigInvalid is a TestCase error, not for use for aResult.iResult
	const TInt KErrConfigInvalid = KErrNone;	//-1100;
	const TInt KErrExpectedValueDifferent = -1502;	//-1101
	const TInt KErrSkippedEvents = -1102;
	const TInt KErrNotExecuted = -1003;
	const TInt KErrExecuted = KErrNone;
	const TInt KErrLeft = -1503;
	const TInt KErrTestTimeout = -1404;

	const TInt KMaxMessageLength = 2000;
	_LIT(KDefaultTextSeparator, " - ");
	_LIT(KDefaultConsoleTraceDescription, "Trace");
	_LIT(KDefaultTracePrefix, "CActiveScheduledTestModule: ");

	//MMF TestModules - Tags and defaults
	_LIT(KPauseCharacter, ",");

	const TInt KBufferSize=4096;	//5000;

//		static const TInt MAX_TESTCASE_ID;
	const TInt KDefaultRepeats=3;

	_LIT(KTagTimeOut, "TimeOut");
#define KDefaultTimeOutDuration (TTimeIntervalMicroSeconds32 (10000000))

#define KDefaultSilenceDuration (TTimeIntervalMicroSeconds ((TInt64)750000))
#define KDefaultRampDuration (TTimeIntervalMicroSeconds ((TInt64)2000000))
#define KDefaultPosition (TTimeIntervalMicroSeconds ((TInt64) 2000000))
#define KDefaultErrorRange (TTimeIntervalMicroSeconds ((TInt64) 250000))
	const TInt KDefaultTimeoutDelay = 16000000;
//	const TInt KDefaultStopDelay = 1000000;
//	const TInt KDefaultPauseDelay = 1000000;
#define	KDefaultPauseDelay (TTimeIntervalMicroSeconds32 (1000000))
	const TInt KDefaultPlayDelay = 3000000;
	const TInt KDefaultRecordDelay = 3500000;
	const TInt KDefaultStopOpenPlayDelay = 1000000;
//	const TInt KDefaultReportDelay = 500000;
#define	KDefaultStopDelay (TTimeIntervalMicroSeconds32 (1000000))
#define	KDefaultReportDelay (TTimeIntervalMicroSeconds32 (500000))

#define KDefaultToneDuration (TTimeIntervalMicroSeconds ((TInt64) 2000000))
#define KDefaultDTMFToneDuration (TTimeIntervalMicroSeconds32 (200000))
#define KDefaultDTMFSilenceDuration (TTimeIntervalMicroSeconds32 (100000))
	const TInt KDefaultToneFrequency = 900;
	const TInt KDefaultSequenceNumber = 0;
	const TInt KDefaultBalance = KMMFBalanceCenter;
	const TInt KDefaultPriority = 0;
	const TInt KDefaultStreamBufferSize = 1024;
	const TInt KDefaultRelVolume = 100;
	const TInt KDefaultRelGain = 100;

	const TInt KRelVolumeMin=0;
	const TInt KRelVolumeMax=100;

	const TInt KRelGainMin=0;
	const TInt KRelGainMax=100;
    const TInt KDefaultValidity=0;
	_LIT(KDefaultDTMFString, "123456789abcdef*#");

	_LIT(KSectionStart, "[general]");
	_LIT(KSectionEnd, "[general_end]");

	_LIT(KTestCaseConfigStart, "[TestCase]");
	_LIT(KTestCaseConfigEnd, "[End_TestCase]");

	_LIT(KTagId, "id");
	_LIT(KTagTitle, "Title");

	_LIT(KTagSoundFile, "SoundFile");
	_LIT(KTagSoundUrl, "SoundUrl");
	_LIT(KTagSoundDes, "SoundDes");

	_LIT(KTagRepeatTimes, "RepeatTimes");
	_LIT(KTagSilenceDuration, "SilenceDuration");

	_LIT(KTagRampDuration, "RampDuration");

	_LIT(KTagPosition, "Position");
	_LIT(KTagErrorRange, "ErrorRange");

	_LIT(KTagDelay, "Delay");
	_LIT(KTagBalance, "Balance");

	_LIT(KTagPauseDelay, "PauseDelay");
	_LIT(KTagPlayDelay, "PlayDelay");

	_LIT(KTagExpectedDuration, "ExpectedDuration");
	_LIT(KTagExpectedDurationDes, "ExpectedDurationDes");
	_LIT(KTagWindowStart, "StartPosition");
	_LIT(KTagWindowEnd, "EndPosition");

	_LIT(KTagMime, "MimeType");
	_LIT(KTagIapId, "IapId");

	_LIT(KTagSequenceNumber, "SequenceNumber");

	_LIT(KLogFileTag, "logFileName");
	_LIT(KLogPathTag, "logPath");

	_LIT(KTagToneFrequency, "ToneFrequency");
	_LIT(KTagToneDuration, "ToneDuration");

	_LIT(KTagPriority, "Priority");
	_LIT(KTagCancelPriority, "CancelPriority");
	_LIT(KTagPriorityPreference, "PriorityPreference");

	_LIT(KTagDTMFString, "DTMFString");

	_LIT(KTagSampleRate, "SampleRate");
	_LIT(KTagChannelCount, "Channels");
	_LIT(KTagBufferSize, "BufferSize");

	_LIT(KTagRelVolume, "RelVolume");
	_LIT(KTagRelGain, "RelGain");

	_LIT(KTagPrepare, "Prepare");
    _LIT(KTagPlay,"Play");
    
    _LIT(KTagValidity,"ValidityCheck");
    _LIT(KTagVolume,"VolumeLevel");
	//Change to reflect the actual test module
	//TODO: Implement functionality to allow this file to remain unchanged and have each test module specify its default log filename
	_LIT(KDefLogFile, "Default.log");
	_LIT(KDefLogPath, "");

	//Log literals
	//_LIT(KTestCaseTitleFormat, "MMFTS.TONE.%d-%S");
	_LIT(KTestCaseTitleFormat, "%$2$S - %$1$d");

	//_LIT(KDefaultLogger, "Using Default Logger");
	_LIT(KLoggerStarted, "Started logger for INI file [%S]");
	_LIT(KOpeningConfig, "Opening configuration file [%S]");
	_LIT(KConfigRequired, "TestCases need to be configured in file");
	_LIT(KConfigInvalid, "TestCases need to be configured properly (file might have change between GetTestCases and RunTestCase)");

	//Results
	_LIT(KTestCaseResultFinished, "TestCase finished, result: %S");
	_LIT(KTestCaseResultNotImplemented, "TestCase not implemented");
	_LIT(KTestCaseResultSuccess, "Successful");
	_LIT(KTestCaseResultFail, "Failed");
	_LIT(KTestCaseResultLeft, "A LEAVE occurred");

	_LIT(KTestCaseResultFailExpectedValueDifferent, "Expected result is different from actual result");
	_LIT(KTestCaseResultFailSkippedEvents, "Not enough events took place to complete test");
	_LIT(KTestCaseResultFailTimeOut, "The test timedout");

	_LIT(KTestCaseNotFound, "The TestCase id is invalid");

#endif	//TEST_MODULE_CONSTANTS_H
