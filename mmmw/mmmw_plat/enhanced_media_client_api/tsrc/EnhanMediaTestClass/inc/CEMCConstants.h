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
* Description:  EMC Test App
*
*/


#ifndef __CEMCSTIFCMDS_H__
#define __CEMCSTIFCMDS_H__

// server name and UID
_LIT(KEMCServerName,"EMCStifServer");
const TUid KEMCServerUid3 = {0x10207BD9};

// EMC Server version
const TUint KEMCServMajorVersionNumber = 1;
const TUint KEMCServMinorVersionNumber = 0;
const TUint KEMCServBuildVersionNumber = 0;

const TUint KEMCMsgQSlots    = 1;
const TUint KEMCMsgComQSlots = 4;

// CONSTANTS
const TInt KErrBadTestParameter= -1000;         // Error on configuration file
const TInt KErrEventPending =-2000;             // Timeout and not callback
const TInt KErrCallbackErrorCode = -2001;       // Callback return error code
const TInt KErrUnexpectedValue =-2002;          // Unexpected value at setting
const TInt KErrTimeoutController = -2007;		// Unexpected notification

const TInt KShortTimeout = 2000;
const TInt KMediumTimeout = 4000;
const TInt KLongTimeout = 100000;



_LIT( KMsgBadTestParameters, "[Error] No valid test case parameters");

// General
_LIT( KTagG711, "G711");
_LIT( KTagOutput32Bit, "Output32Bit");
_LIT( KTagDownMixToStereo, "DownMixToStereo");
_LIT( KTagLostDataConcealment, "LostDataConcealment");
_LIT( KTagTrue, "True");
_LIT( KTagFalse, "False");
_LIT( KTagDevSound, "DevSound");
_LIT( KTagOutputStream, "OutputStream");

// MACROS

_LIT( KTagMimeType, "audio/mpeg" );

//#define ?macro ?macro_def
// Logging path
#ifdef __WINSCW__
_LIT( KTagTestClassLogPath, "\\logs\\testframework\\" );
_LIT( KTagFileTest, "c:\\Testing\\data\\mp3mp2s8b17.mp3" );
#else
_LIT( KTagTestClassLogPath, "e:\\testing\\Log\\" );
_LIT( KTagFileTest, "e:\\Testing\\data\\mp3mp2s8b17.mp3" );
#endif

// Log file
_LIT( KTagTestClassLogFile, "EnhanMediaTestClass.txt" );



enum TControl
    {
    EFILESOURCE = 0,
    EDATABUFFERSOURCE,
    EDESCRIPTORSOURCE,
    ESTREAMCONTROL,
    EMMFAUDIOOUTPUTSINKCONTROL,
    EVOLUMECONTROL,
    EPDLSOURCE,
    EEQUALIZERCONTROL,
    EREVERBCONTROL,
	EBASSBOOSTCONTROL,
    ESTEREOWIDCONTROL,
    ELOUDNESSCONTROL,
    EROOMLEVELCONTROL,
    EDISTATTCONTROL,
    ELISTDOPPCONTROL,
    ESOURDOPPCONTROL,
    ELISTLOCCONTROL,
    ESOURLOCCONTROL,
    ELISTORIENTCONTROL,
    ESOURORIENTCONTROL,
    EBALANCECONTROL
    };

enum eEMCSTIFCommands
{
    EMC_CreateFactoryL = 0,
    EMC_DeleteFactoryL,
    EMC_CreateStreamL,
    EMC_CreateSourceL,
    EMC_CreateSinkL,
    EMC_DeleteStreamL,
    EMC_DeleteSourceL,
    EMC_DeleteSinkL,
    EMC_CreateEffectL,
    EMC_DeleteEffectL,
    EMC_CreateDataBufferL,
    EMC_DeleteDataBufferL,
    EMC_ControlAddObserverL,
    EMC_ControlRemoveObserverL,
    EMC_ControlTypeL,
    EMC_ControlControlTypeL,
    EMC_StreamAddSourceL,
    EMC_StreamAddSinkL,
    EMC_StreamAddEffectL,
    EMC_StreamRemoveEffectL,
    EMC_StreamOpenL,
    EMC_StreamPrimeL,
    EMC_StreamStartL,
    EMC_StreamPauseL,
    EMC_StreamStopL,
    EMC_StreamCloseL,
    EMC_StreamSetPositionL,
    EMC_StreamSetPriorityL,
    EMC_StreamGetPositionL,
    EMC_StreamGetDurationL,
    EMC_StreamGetSSL,
    EMC_StreamGetRSSL,
    EMC_StreamGetStateL,
    EMC_StreamGetCIL,
    EMC_StreamPlaybackComplete,
    EMC_StreamOpenComplete,
    EMC_StreamPrimeComplete,
    EMC_SourceGetMimeTypeL,
    EMC_SourceGetSizeL,
    EMC_SourceCloseL,
    EMC_EffectApplyL,
    EMC_DBSOpenL,
    EMC_DBSGetMinBufSizeL,
    EMC_DBSWriteDataL,
    EMC_DBSSetSizeL,
    EMC_DBSGetBufConfigL,
    EMC_DBSSetBufConfigL,
    EMC_DBSGetBitRateL,
    EMC_DBSGetBufTypesSupL,
    EMC_DBSGetInterfaceL,
    EMC_PDLSOpenL,
    EMC_PDLSMoveFileL,
    EMC_PDLSCancelDownloadL,
    EMC_PDLSResumeDownloadL,
    EMC_PDLSGetCurFileSizeL,
    EMC_PDLSGetExpFileSizeL,
    EMC_PDLSGetDLStatusL,
    EMC_PDLSIsDLCompleteL,
    EMC_PDLSGetPerDownloadedL,
    EMC_PDLSGetPerBufferedL,
    EMC_PDLSGetDLRateL,
    EMC_PDLSGetBitRateL,
    EMC_DescSOpenL,
    EMC_DescSGetBitRateL,
    EMC_FileSOpenL,
    EMC_FileSGetBitRateL,
    EMC_VolGetMinVolumeL,
    EMC_VolGetMaxVolumeL,
    EMC_VolGetDefaultVolumeL,
    EMC_VolGetVolumeL,
    EMC_VolSetVolumeL,
    EMC_VolSetVolumeRampL,
    EMC_BalGetBalanceL,
    EMC_BalSetBalanceL,
    EMC_GeneralOpenFileL,
    EMC_GeneralSetWriteLoopL,

    EMC_AudioEffectDisableL,
    EMC_AudioEffectEnableL,
    EMC_AudioEffectEnforceL,
    EMC_AudioEffectHaveUpdateRightsL,
    EMC_AudioEffectIsEnabledL,
    EMC_AudioEffectIsEnforcedL,
    EMC_AudioEffectUidL,

    EMC_EQBandLevelL,
    EMC_EQBandWidthL,
    EMC_EQCenterFrequencyL,
    EMC_EQCrossoverFrequencyL,
    EMC_EQDbLevelLimitsL,
    EMC_EQNumberOfBandsL,
    EMC_EQSetBandLevelL,

    EMC_ERDecayHFRatioL,
    EMC_ERDecayHFRatioRangeL,
    EMC_ERDecayTimeL,
    EMC_ERDecayTimeRangeL,
    EMC_ERDensityL,
    EMC_ERDiffusionL,
    EMC_ERReflectionsDelayL,
    EMC_ERReflectionsDelayMaxL,
    EMC_ERReflectionsLevelL,
    EMC_ERReflectionLevelRangeL,
    EMC_ERReverbDelayL,
    EMC_ERReverbDelayMaxL,
    EMC_ERReverbLevelL,
    EMC_ERReverbLevelRangeL,
    EMC_ERRoomHFLevelL,
    EMC_ERRoomHFLevelRangeL,
    EMC_ERRoomLevelL,
    EMC_ERRoomLevelRangeL,
    EMC_ERSetDecayHFRatioL,
    EMC_ERSetDecayTimeL,
    EMC_ERSetDensityL,
    EMC_ERSetDiffusionL,
    EMC_ERSetReflectionsDelayL,
    EMC_ERSetReflectionsLevelL,
    EMC_ERSetReverbDelayL,
    EMC_ERSetReverbLevelL,
    EMC_ERSetRoomHFLevelL,
    EMC_ERSetRoomLevelL,
    EMC_ERDelayMaxL,

	EMC_SWIsContinuousLevelSupportedL,
	EMC_SWSetStereoWideningLevelL,
	EMC_SWStereoWideningLevelL,

	EMC_RLLevelL,
	EMC_RLLevelRangeL,
	EMC_RLSetRoomLevelL,

	EMC_DADistanceAttenuationL,
	EMC_DARollOffFactorMaxL,
	EMC_DARoomRollOffFactorMaxL,
	EMC_DASetDistanceAttenuationL,

	EMC_LDCartesianVelocityL,
	EMC_LDFactorL,
	EMC_LDFactorMaxL,
	EMC_LDSetCartesianVelocityL,
	EMC_LDSetFactorL,
	EMC_LDSetSphericalVelocityL,
	EMC_LDSphericalVelocityL,

	EMC_SDCartesianVelocityL,
	EMC_SDFactorL,
	EMC_SDFactorMaxL,
	EMC_SDSetCartesianVelocityL,
	EMC_SDSetFactorL,
	EMC_SDSetSphericalVelocityL,
	EMC_SDSphericalVelocityL,

    EMC_SOOrientationL,
    EMC_SOOrientationVectorsL,
    EMC_SOSetOrientationL,
    EMC_SOSetOrientationVectorsL,

    EMC_LOOrientationL,
    EMC_LOOrientationVectorsL,
    EMC_LOSetOrientationL,
    EMC_LOSetOrientationVectorsL,

	EMC_SLLocationCartesianL,
	EMC_SLLocationSphericalL,
	EMC_SLSetLocationCartesianL,
	EMC_SLSetLocationSphericalL,

	EMC_LLLocationCartesianL,
	EMC_LLLocationSphericalL,
	EMC_LLSetLocationCartesianL,
	EMC_LLSetLocationSphericalL,
	EMC_ER_DelayMaxMax,
    EMC_NoCommand

};

#endif


