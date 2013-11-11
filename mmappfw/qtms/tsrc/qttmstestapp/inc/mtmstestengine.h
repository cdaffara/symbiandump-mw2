/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service - TestApp
 *
 */

#ifndef MTMSTESTENGINE_H
#define MTMSTESTENGINE_H

// INCLUDES
#include <glib.h>
#include <e32base.h>
#include <e32def.h>
#include <e32const.h>

// CLASS DECLARATION
/**
 *  CTmsTestEngine application engine class.
 */
class MTmsTestEngine
    {
public:
    virtual ~MTmsTestEngine() {}

    virtual void GetSupportedDownlinkFormats(TBool aDisplayCodecs = EFalse) =0;
    virtual void GetSupportedUplinkFormats(TBool aDisplayCodecs = EFalse)=0;
    virtual void SetDownlinkFormat(gint aCodecFormat)=0;
    virtual void OpenDownlink()=0;
    virtual void StartDownlink()=0;
    virtual void StopDownlink()=0;
    virtual void CloseDownlink()=0;
    virtual void GetMaxVolume()=0;
    virtual void GetVolume()=0;
    virtual void SetVolume(guint aVolume)=0;
    virtual void MuteSpeaker()=0;
    virtual void SetMaxVolume()=0;
    virtual void VolumeUp()=0;
    virtual void VolumeDn()=0;

    virtual void SetUplinkFormat(gint aCodecFormat)=0;
    virtual void OpenUplink()=0;
    virtual void StartUplink()=0;
    virtual void StopUplink()=0;
    virtual void CloseUplink()=0;
    virtual void GetMaxGain()=0;
    virtual void GetGain()=0;
    virtual void SetMaxGain()=0;
    virtual void MuteMic()=0;

    virtual void GetSupportedBitrates()=0;
    virtual TInt GetSupportedBitrates(std::vector<guint>& aBrArr)=0;
    virtual void SelectMinBitrate()=0;
    virtual void SelectMaxBitrate()=0;
    virtual void SetBitrate(TUint aBitrate)=0;
    virtual void GetBitrate()=0;
    virtual void GetBitrate(TUint& aBitrate)=0;
    virtual void ToggleVad()=0;
    virtual void GetVad()=0;
    virtual void ToggleCng()=0;
    virtual void GetCng()=0;
    virtual void TogglePlc()=0;
    virtual void GetPlc()=0;
    virtual void SetDnLinkG711ALAW()=0;
    virtual void SetDnLinkG711uLAW()=0;
    virtual void SetDnLinkILBC20MS()=0;
    virtual void SetDnLinkILBC30MS()=0;
    virtual void SetUpLinkG711ALAW()=0;
    virtual void SetUpLinkG711uLAW()=0;
    virtual void SetUpLinkILBC20MS()=0;
    virtual void SetUpLinkILBC30MS()=0;
    virtual void GetDnLinkG711Mode()=0;
    virtual void GetDnLinkILBCMode()=0;
    virtual void GetUpLinkG711Mode()=0;
    virtual void GetUpLinkILBCMode()=0;

    virtual void SetOutputDevice(gint device)=0;
    virtual void SetHandset()=0;
    virtual void SetLoudSpeaker()=0;
    virtual void GetAudioDevice()=0;
    virtual void GetAvailableOutput()=0;
    virtual void DoLoopback()=0;
    virtual void OneTouchLoopback()=0;

    virtual void InitDTMFTonePlayerDnlink()=0;
    virtual void InitDTMFTonePlayerUplink()=0;
    virtual void DTMFTonePlayDnlink()=0;
    virtual void DTMFTonePlayUplink()=0;
    virtual void CloseDTMFPlayerDnlink()=0;
    virtual void CloseDTMFPlayerUplink()=0;

    virtual gint CreateRingTonePlayer()=0;
    virtual void InitRingTonePlayerFromFile()=0;
    virtual void InitRingTonePlayerFromProfiles()=0;
    virtual void InitRingToneWithTTS()=0;
    virtual void InitRingToneSequencePlayer()=0;
    virtual void InitRingToneBeepOnce()=0;
    virtual void InitRingToneSilent()=0;
    virtual void PlayRingTone()=0;
    virtual void PauseVideoRingTone()=0;
    virtual void StopRingTone()=0;
    virtual void MuteRingTone()=0;
    virtual void DeinitRingTonePlayer()=0;
    virtual void CloseRingTonePlayer()=0;

    virtual void SetCallType(gint calltype)=0;

    virtual void CreateInbandTonePlayer()=0;
    virtual void StartInbandTone(gint inbandtonetype)=0;
    virtual void StopInbandTone()=0;

    virtual gint CreateFactory()=0;
    virtual gint CreateCall(gint calltype)=0;
    virtual gint CreateUplink()=0;
    virtual gint CreateDownlink()=0;
    virtual gint CreateClientSource()=0;
    virtual gint CreateModemSource()=0;
    virtual gint AddClientSourceToStream()=0;
    virtual gint AddModemSourceToStream()=0;
    virtual gint CreateClientSink()=0;
    virtual gint CreateModemSink()=0;
    virtual gint AddClientSinkToStream()=0;
    virtual gint AddModemSinkToStream()=0;
    virtual gint CreateMicSource()=0;
    virtual gint AddMicSourceToStream()=0;
    virtual gint CreateSpeakerSink()=0;
    virtual gint AddSpeakerSinkToStream()=0;

    virtual gint CreateVolumeEffect()=0;
    virtual gint AddVolumeEffectToStream()=0;
    virtual gint CreateGainEffect()=0;
    virtual gint CreateGlobalRouting()=0;
    virtual gint AddGainEffectToStream()=0;

    virtual void CreateGlobalVol()=0;
    virtual void CreateGlobalGain()=0;
    virtual void GetGlobalVol()=0;
    virtual void GetGlobalMaxVol()=0;
    virtual void SetGlobalVol()=0;
    virtual void GetGlobalGain()=0;
    virtual void GetGlobalMaxGain()=0;
    virtual void SetGlobalGain()=0;
    };

#endif // MTMSTESTENGINE_H

// End of file
