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
* Description:  AudioRecorder Test
*
*/

#ifndef __CONSTANTS_H
#define __CONSTANTS_H

//Generic Tags
_LIT(KInitSectionStart, "[general]");
_LIT(KInitSectionEnd, "[general_end]");
_LIT(KLogFileName, "logFileName");
_LIT(KLogPath, "logPath");
_LIT(KSectionStart, "[TestCase]");
_LIT(KSectionEnd, "[End_TestCase]");
_LIT(KTestName, "Title");
_LIT(KTestId, "Id");
_LIT(KFileName, "File");
_LIT(KFileName1, "File1");
_LIT(KFileName3, "File3");
_LIT(KRFileName3, "RFile3");
_LIT(KRFileName1, "RFile1");
_LIT(KDescriptor, "Desc");
_LIT(KURL, "URL");
_LIT(KErrorRange, "ErrorRange");
_LIT(KFileSize, "FileSize");
_LIT(KSource, "Source");
_LIT(KTarget, "Target");
_LIT(KAudioIn, "AudioIn");
_LIT(KFileName2, "File2");
_LIT(KRFileName, "RFile");
_LIT(KRFileName2, "RFile2");
_LIT(KSFileName, "SFile");
_LIT(KSFileName2, "SFile2");
_LIT(KDescriptor2, "Desc2");
_LIT(KDescriptor3, "Desc3");
_LIT(KURL2, "URL2");
_LIT(KLocation, "Location");

//Record Tags
_LIT(KRecordTime, "RecTime");
_LIT(KRelativePosition, "RelativePos");
_LIT(KRelativeGain, "RelativeGain");
_LIT(KBalance, "Balance");
_LIT(KMDName, "MD_Name");
_LIT(KMDValue, "MD_Value");
_LIT(KTimeout, "Timeout");
_LIT(KTestNoName, "No name");
_LIT(KMimeType, "MIME");
_LIT(KIAPID, "IAPID");
_LIT(KPause, "Pause");
_LIT(KMaxVolume, "MaxVolume");
_LIT(KRepeats, "Repeats");
_LIT(KVolume, "Volume");
_LIT(KRamp, "Ramp");
_LIT(KClearWindow, "ClearWindow");
_LIT(KDataFormat, "DataFormat");
_LIT(KFormatAmr, "amr");
_LIT(KFormatMp4, "mp4");
_LIT(KMaxLength, "MaxLength");


//Convert Tags
_LIT(KConvertAppend, "Append");
_LIT(KConvertFormat, "Convert");
_LIT(KDataType, "DataType");
_LIT(KBitRate, "BitRate");
_LIT(KSampleRate, "SampleRate");
_LIT(KChannels, "Channels");
_LIT(KCancelTime, "CancelTime");
_LIT(KFormatRaw, "raw");
_LIT(KFormatAu, "au");
_LIT(KFormatWav, "wav");
_LIT(KConvertFormat2, "Convert2");
_LIT(KDataType2, "DataType2");
_LIT(KBitRate2, "BitRate2");
_LIT(KSampleRate2, "SampleRate2");
_LIT(KChannels2, "Channels2");
_LIT(KPosition, "Position");
_LIT(KStartPos, "WindowStart");
_LIT(KEndPos, "WindowEnd");

//Codec Tags
_LIT(KCodecId, "CodecId");
_LIT(KSrcBufferSize, "SourceBufferSize");
_LIT(KDestBufferSize, "DestinationBufferSize");
_LIT(KSrcPosition, "SourcePosition");
_LIT(KDestPosition, "DestinationPosition");
_LIT(KSrcFrNum, "SourceFrameNumber");
_LIT(KDestFrNum, "DestinationFrameNumber");
_LIT(KDestType, "DestinationType");
_LIT(KSrcType, "SourceType");
_LIT(KSupplier, "Supplier");
_LIT(KCodecResult, "Result");

//Controller Tags
_LIT(KControllerId, "ControllerId");
_LIT(KSourceFile, "SourceFile");
_LIT(KSourceDesc, "SourceDesc");
_LIT(KSourceAudio, "SourceAudio");
_LIT(KSinkFile, "SinkFile");
_LIT(KSinkDesc, "SinkDesc");
_LIT(KSinkAudio, "SinkAudio");
_LIT(KRemoveSource, "RemoveSource");
_LIT(KRemoveSink, "RemoveSink");
_LIT(KInstructions, "Commands:");
_LIT(KCPrepare, "Prepare");
_LIT(KCPlay, "Play");
_LIT(KCPause, "Pause");
_LIT(KCStop, "Stop");
_LIT(KLogPosition, "LogPosition");

//Priority tags
_LIT(KPriority, "Priority");
_LIT(KPPTime, "time");
_LIT(KPPQuality, "quality");
_LIT(KPPNone, "none");
_LIT(KPPBoth, "both");
_LIT(KCropFromBegining, "FromBegining");


//Others
_LIT(KDefaultLogFileName, "RecordTest.log");
_LIT(KDefaultLogPath, ""/*"C:\\Logs\\AudioRecordTest\\"*/);
_LIT(KDefaultFileName, "C:\\Sounds\\test.wav");
_LIT(KLibPath, "C:\\System\\Libs\\");
_LIT(KEmptyString,"");



const TInt KDefaultRecordTime = 4000000;
const TInt KDefaultErrorRange = 500000;
const TInt KMaxInt32 = 2147483647;
const TInt KNumberOfRecordTests = 18;

//Errors
const TInt KErrSyntax			= -1000;
const TInt KErrInvalidId		= -1001;
const TInt KErrFileDuration		= -1002;
const TInt KErrOutOfRange		= -1003;
const TInt KErrAPI				= -1004;
const TInt KErrFileSize			= -1005;
const TInt KErrObjectNotCreated = -1006;
const TInt KErrCropFile			= -1007;
const TInt KErrVerifying		= -1008;
const TInt KErrOpenFile			= -1009;

//Recorder
const TInt KErrRecOpenFile		= -1201;
const TInt KErrNoRecorder		= -1202;
const TInt KErrStartRecord		= -1203;
const TInt KErrGetGain			= -1204;
const TInt KErrSetBalance		= -1205;
const TInt KErrRecBySize		= -1206;
const TInt KErrNoMetaData		= -1207;
const TInt KErrAddMetaData		= -1208;
const TInt KErrMetaDataNotFound	= -1209;
const TInt KPriorityError		= -1210;
const TInt KErrRecFormats		= -1211;
const TInt KErrSetPlayBalance	= -1212; //new
const TInt KErrSetRepeats		= -1213; //new
const TInt KErrPlayRepeatsTime	= -1214; //new
const TInt KErrPlayRepeats		= -1215; //new
const TInt KErrPlayFile			= -1216; //new
const TInt KErrGetVolume		= -1217; //new
const TInt KErrDiffVolume		= -1218; //new
const TInt KErrVolumeRamp		= -1219; //new
const TInt KErrPlayWindow		= -1220; //new

const TInt KErrProgressPect		= -1221; //new
const TInt KErrCropFromBeginning = -1222; //new

//Converter
const TInt KErrConvertOpenFile  = -1301;
const TInt KErrConvertSetType   = -1302;
const TInt KErrConvertSetFormat = -1303;
const TInt KErrUnknownFormat	= -1304;
const TInt KErrConversionWrong  = -1305;
const TInt KErrConvertSetBitRate	= -1306;
const TInt KErrConvertSetSampleRate = -1307;
const TInt KErrFileModifiedOnCancel = -1308;
const TInt KErrConvertSetChannels = -1309;

//Codec
const TInt KErrInvalidCodecId		= -1601;
const TInt KErrNoDataSource			= -1602;
const TInt KErrCodecProcess			= -1603;
const TInt KErrCodecNotSupported	= -1604;
const TInt KErrCodecResult			= -1605;

//Controller
const TInt KErrControllerOpen			= -1801;
const TInt KErrControllerAddSource		= -1802;
const TInt KErrControllerRemoveSource	= -1803;
const TInt KErrControllerAddSink		= -1804;
const TInt KErrControllerRemoveSink		= -1805;

//Parameters Actions
const TInt KActionRecord		= 201;
const TInt KActionStopRecord	= 202;

#endif