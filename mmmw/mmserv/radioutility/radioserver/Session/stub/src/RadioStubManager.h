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
* Description:  
*
*/

#ifndef RADIOSTUBMANAGER_H_
#define RADIOSTUBMANAGER_H_
#include <e32std.h>
#include <e32base.h>

#include <RadioFmTunerUtility.h>
#include <RadioPlayerUtility.h>
#include <RadioRdsUtility.h>
#include <remconcallhandlingtargetobserver.h>
#include <remconcoreapitargetobserver.h>

#include "RadioServerData.h"
#include "mradioheadseteventobserver.h"

_LIT(KRadioStubManagerLocalChunkName, "RadioStubManagerLocalChunkName");

struct TStubError
    {

    void SetStubError( TFmRadioTunerError aError, TUint aFailCounter = 1, TBool aFailForEver = EFalse ) 
        {
        iError = aError;
        iFailCounter = aFailCounter;
        iFailForEver = aFailForEver; };
    TInt Error() 
        {
        TInt aError (iError);
        if (iFailCounter)
            {
            --iFailCounter;
            }
        else if (iFailForEver)
            {
            }
        else
            {
            aError = KErrNone;
            }            
        return aError;
        }
    TFmRadioTunerError iError;
    TUint iFailCounter;
    TBool iFailForEver;
    };

struct TLeaveError
    {
    void Set( TInt aError = KErrGeneral );
    TInt iError;
    };

struct SRadioStubManager
    {
public:
    void  FillZ() 
        { 
        Mem::FillZ( this, sizeof( SRadioStubManager ) ); 
        }
public:
    enum TCenRepDbs
        {
        // Redefine CenRep databases for test purposes.
        // Original constant names are got by eliminating leading "ESub_"-prefix.
        EStub_KPSUidCtsyCallInformation = 0x01001001,
        EStub_KUidSystemCategory = 0x01001002,
        EStub_KPSUidVoiceUiAccMonitor = 0x01001003,
        EStub_KPSUidMMFAudioServer = 0x01001004,    
        EStub_KRadioPSUid = 0x01001005,
        EStub_KRadioServerPropertyCategory = 0x01001006
        };
    
    struct TRemConTargetData
    {
    TLeaveError iLeaveNewL;
    TLeaveError iLeaveConstructL;
    
    
    MRemConCoreApiTargetObserver*   iRemConCoreApiTargetObserver;
    MRemConCallHandlingTargetObserver*  iRemConCallHandlingTargetObserver;
    } mutable iRemConTarget;
    
    struct TAccessoryObserverData
    {
    TLeaveError iLeaveNewL;
    TLeaveError iLeaveIsHeadsetConnectedL;
    TLeaveError iLeaveConstructL;
    
    TBool iHeadsetConnected;
    MRadioHeadsetEventObserver* iObserver;
    
    
    } mutable iAccessoryObserver;
    
    struct TFmTunerStubData
        {
        TRsTunerCapabilities iCaps;
        TStubError iGetCapabilitiesError;
        
        
        TBool iTunerInOfflineMode;
        TStubError iEnableTunerInOfflineModeError;
        
        
        TRsFrequencyRange iRange;
        TRsFrequencyRange iPreviousRange;
        TInt iMinFreq;
        TInt iMaxFreq;
        TStubError iGetFrequencyRangeError;
        TStubError iSetFrequencyRangeError;
        
        TInt iFrequency;
        TInt iPreviousFrequency;
        TStubError iGetFrequencyError;
        TStubError iSetFrequencyError;

        struct TScanStations
        {
        TInt iCount;
        TInt iMinFreq;
        TInt iFrequencyStepSize;        
        } mutable iScanStations;
        
        TBool iStationSeekUpwards;

        TInt iSignalStrength;
        TStubError iGetSignalStrengthError;
        
        TInt iMaxSignalStrength;
        TStubError iGetMaxSignalStrengthError;
        
        TBool iStereoMode;
        TStubError iGetStereoModeError;
        
        TBool iForcedMono;
        TStubError iGetForcedMonoReceptionError;
        TStubError iForcedMonoReceptionError;
        
        TBool iSquelch;
        TStubError iSetSquelchError;
        TStubError iGetSquelchError;
        } mutable iTuner;

    struct TPlayerStubData
        {
        TLeaveError iLeaveNewL;
        TLeaveError iLeaveErrorForConstructL;
        MRadioPlayerObserver* iRadioPlayerUtilityClient;

        TRsPlayerState iPlayerState;
        
        TBool iMuteStatus;
        TStubError iMuteError;
        
        TInt iVolume;
        TStubError iSetVolumeError;
        TStubError iGetVolumeError;
        
        TTimeIntervalMicroSeconds iRampInterval;
        TStubError iSetVolumeRampError;
        
        TInt iMaxVolume;
        TStubError iGetMaxVolumeError;
        
        TInt iLeftPercentage;
        TInt iRightPercentage;
        TStubError iSetBalanceError;
        TStubError iGetBalanceError;
        } mutable iPlayer;
        
    struct TRdsStubData
        {
        TLeaveError iLeaveNewL;
        TLeaveError iLeaveErrorForConstructL;
        MRadioRdsObserver* iRadioRdsUtilityClient;
        TRsRdsCapabilities iCaps;
        TStubError iGetCapabilitiesError;
        
        TBool iRdsSignal;
        TStubError iGetRdsSignalStatusError;
        
        TStubError iNotifyRdsDataChangeError;    
        
        TBool iAutomaticSwitching;
        TStubError iSetAutomaticSwitchingError;
        TStubError iGetAutomaticSwitchingError;
        
        TStubError iStationSeekByPTYError;
        TStubError iStationSeekByTAError;
        TStubError iStationSeekByTPError;
        TInt iFrequency;
        TStubError iGetFreqByPTYError;
        TStubError iGetFreqByTAError;
        TStubError iGetPSByPTYError;
        TStubError iGetPSByTAError;
            
        RArray<TRdsPSName> iPsList;
        RArray<TInt> iFreqList;
        
        TInt iPi;
        TStubError iGetProgrammeIdentificationError;
        
        TRdsProgrammeType iPty;
        TStubError iGetProgrammeTypeError;
        
        TRdsPSName iPs;
        TStubError iGetProgrammeServiceError;
        
        // Radio Text
        TRdsRTplusClass iRtPlusClass;
        TRdsRadioText iRtPlusData;
        TStubError iGetRadioTextPlusError;
        TRsRdsRadioText iRt;
        TStubError iGetRadioTextError;
        
        // Automatic traffic announcement
        TBool iAuto;
        TStubError iGetTrafficAnnouncementStatusError;
        TStubError iSetTrafficAnnouncementStatusError;
    
        TBool iTaStatus;
        TStubError iGetAutomaticTrafficAnnouncementError;
        TStubError iSetAutomaticTrafficAnnouncementError;
        
        TDateTime iCt;
        TStubError iGetClockTimeError;
        
        TBool iTpStatus;
        TStubError iGetTrafficProgrammeStatusError;
        } mutable iRds;
        
    struct TMonitorStubData
        {
        TInt    iNone;
        } mutable iMonitor;
    };

const TUid KStub_KRadioServerPropertyCategory = {SRadioStubManager::EStub_KRadioServerPropertyCategory};
const TUid KStub_KUidSystemCategory = {SRadioStubManager::EStub_KUidSystemCategory};
const TUid KStub_KPSUidCtsyCallInformation = {SRadioStubManager::EStub_KPSUidCtsyCallInformation};
const TUid KStub_KPSUidVoiceUiAccMonitor = {SRadioStubManager::EStub_KPSUidVoiceUiAccMonitor};
const TUid KStub_KPSUidMMFAudioServer = {SRadioStubManager::EStub_KPSUidMMFAudioServer};
const TUid KStub_KRadioPSUid = {SRadioStubManager::EStub_KRadioPSUid};
const TUid KStub_KRadioCRUid = {0x2002FF52};

#endif /* RADIOSTUBMANAGER_H_ */
