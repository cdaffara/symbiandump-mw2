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
* Description:  Audio stream test component
*
*/

/*

  Author: Binh Vu
  Version: 1.0

*/

#ifndef AUDIO_STREAM_CONSTANTS_H
#define AUDIO_STREAM_CONSTANTS_H



const TInt KErrConfigInvalid = KErrNone;	//-1100;
const TInt KErrExpectedValueDifferent = -1502;	//-1101
const TInt KErrSkippedEvents = -1102;
const TInt KErrNotExecuted = -1003;
const TInt KErrExecuted = KErrNone;
const TInt KErrLeft = -1503;
const TInt KErrTestTimeout = -1404;

const TInt KDefaultStreamBufferSize = 1024;
const TInt KDefaultRelVolume = 100;
const TInt KDefaultStopDelay = 1000000;
const TInt KDefaultRelGain = 100;
const TInt KDefaultBalance = KMMFBalanceCenter;
const TInt KDefaultTimeoutDelay = 16000000;
const TInt KDefaultRecordDelay = 2000000;
const TInt KDefaultPriority = 90;
const TInt KDefaultPriorityPreference = 3;
const TInt KDefaultSampleRate = 8000;
const TInt KDefaultChannels = 1;
const TInt KDefaultDataType = KMMFFourCCCodeNULL;
const TInt KDefaultSampleSize = 16;

const TInt KRelVolumeMin=0;
const TInt KRelVolumeMax=100;
const TInt KRelGainMin=0;
const TInt KRelGainMax=100;
const TInt KBufferSize=4096;


_LIT(KSectionStart, "[general]");
_LIT(KSectionEnd, "[general_end]");

_LIT(KTestCaseConfigStart, "[TestCase]");
_LIT(KTestCaseConfigEnd, "[End_TestCase]");

// Tags
_LIT(KTagId, "id");
_LIT(KTagTitle, "Title");
_LIT(KTagSoundDes, "SoundDes");
_LIT(KTagBufferSize, "BufferSize");
_LIT(KTagVolume, "Volume");
_LIT(KTagSampleRate, "SampleRate");
_LIT(KTagChannelCount, "Channels");
_LIT(KTagDelay, "Delay");
_LIT(KTagBalance, "Balance");
_LIT(KTagPriority, "Priority");
_LIT(KTagPriorityPreference, "PriorityPreference");
_LIT(KTagGain, "Gain");
_LIT(KTagDataType, "DataType");
_LIT(KTagStopDelay, "StopDelay");
_LIT(KTagSampleSize, "SampleSize");


_LIT(KLogFileTag, "logFileName");
_LIT(KLogPathTag, "logPath");

_LIT(KDefLogFile, "audioInputStream.log");
_LIT(KDefLogPath, "");

//Log literals
//_LIT(KTestCaseTitleFormat, "MMFTS.PLAY.%d-%S");
_LIT(KTestCaseTitleFormat, "%$2$S - %$1$d");
//_LIT(KDefaultLogger, "Using Default Logger");
_LIT(KLoggerStarted, "Started logger for INI file [%S]");
_LIT(KOpeningConfig, "Opening configuration file [%S]");
_LIT(KOpeningLogFilepath, "Opening logfile path [%S]");
_LIT(KConfigRequired, "TestCases need to be configured in file");
_LIT(KConfigInvalid, "TestCases need to be configured properly (file might have change between GetTestCases and RunTestCase)");

//Results
_LIT(KTestCaseResultFinished, "TestCase finished, result: %S");
_LIT(KTestCaseResultSuccess, "Successful");
_LIT(KTestCaseResultNotImplemented, "TestCase not implemented");
_LIT(KTestCaseResultNotSupported, "TestCase Not Supported");
_LIT(KTestCaseResultFail, "Failed");
_LIT(KTestCaseResultLeft, "A LEAVE occurred");
_LIT(KTestCaseResultFailExpectedValueDifferent, "Expected result is different from actual result");
_LIT(KTestCaseResultFailSkippedEvents, "Not enough events took place to complete test");
_LIT(KTestCaseResultFailTimeOut, "The test timedout");

_LIT(KTestCaseNotFound, "The TestCase id is invalid");


#endif  //AUDIO_STREAM_CONSTANTS_H

// End of File