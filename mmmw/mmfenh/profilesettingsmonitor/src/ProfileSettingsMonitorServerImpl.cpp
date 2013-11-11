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
* Description:  This file contains implementation of 
                    ProfileSettingsMonitorServerImpl.
*
*/


#include "ProfileSettingsMonitorServerImpl.h"
#include <AudioClientsListPSKeys.h>
#include <AudioClientsListPSData.h>
#include <MediaStreamPropertiesPSKeys.h>
#include <MProfileExtended.h>

#ifdef _DEBUG
#define DEB_TRACE0(str)             RDebug::Print(str)
#define DEB_TRACE1(str, val1)       RDebug::Print(str, val1)
#else
#define DEB_TRACE0(str)
#define DEB_TRACE1(str, val1)
#endif // _DEBUG


static _LIT_SECURITY_POLICY_PASS(KClientStateReadPolicy);
static _LIT_SECURITY_POLICY_S0(KMMFAudioServerSIDPolicy, KUidMmfAudioServerDllUnicodeDefine);


CProfileSettingsMonitorServerImpl::CProfileSettingsMonitorServerImpl()
	{
	}

CProfileSettingsMonitorServerImpl::~CProfileSettingsMonitorServerImpl()
	{
	iWarningTonesProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings, KGASWarningTones);
	
	iMessagingTonesProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings, KGASMessageTones);
	
	iSilentProfileProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings, KGASSilentProfile);
	
	iVibraProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings, KGASVibra);
	
	iRingingTypeProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASRingingType);
	
	iMessageToneNameProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings, KGASMessageToneName);
	
	iEmailToneNameProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASEmailToneName);
	
	iVideoCallToneNameProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASVideoCallToneName);
	
	iRingingTone1NameProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASRingingTone1Name);
	
	iRingingTone2NameProperty.Close();
    RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASRingingTone2Name);
	
	iKeypadVolumeProperty.Close();
	RProperty::Delete(KGASPSUidGlobalAudioSettings,KGASKeypadToneVolume);
	
	if(iMessageToneName)
	  {
	  delete iMessageToneName;
	  iMessageToneName=NULL;
	  }
	if(iEmailToneName)
	  {
	  delete iEmailToneName;
	  iEmailToneName=NULL;
	  }
	if(iVideoCallToneName)
	  {
	  delete iVideoCallToneName;
	  iVideoCallToneName=NULL;
	  }
	if(iRingingTone1Name)
	  {
	  delete iRingingTone1Name;
	  iRingingTone1Name=NULL;
	  }
	if(iRingingTone2Name)
	   {
	   delete iRingingTone2Name;
	   iRingingTone2Name=NULL;
	   }
	if(iProfileEngineExtended)
	    {
	    iProfileEngineExtended->Release();
	    }
	//delete iProfileEngineExtended;
	// Release profile engine resources
	if ( iEngine )
	    {
	    iEngine->Release();
	    }
	delete iEngine;
	delete iNotifyHandler;
	}

CProfileSettingsMonitorServerImpl* CProfileSettingsMonitorServerImpl::NewL()
	{
	CProfileSettingsMonitorServerImpl* s = new(ELeave) CProfileSettingsMonitorServerImpl();
	CleanupStack::PushL(s);
	s->ConstructL();
	CleanupStack::Pop();
	return s;
	}

void CProfileSettingsMonitorServerImpl::ConstructL()
	{
    iEngine = ProEngFactory::NewEngineL();
    iNotifyHandler = ProEngFactory::NewNotifyHandlerL();    
    User::LeaveIfError( iNotifyHandler->RequestProfileActivationNotificationsL( *this ));    
    User::LeaveIfError( iNotifyHandler->RequestActiveProfileNotificationsL( *this ));
    
    // Initialize iSilentProfile
    iActiveProfileId = iEngine->ActiveProfileId();
    if(iActiveProfileId == 1) 
        iSilentProfile = ETrue;
    else
        iSilentProfile = EFalse;
    
    // Initialize iWarningTones, iMessagingTones and iVibra
    MProEngProfile* currentProfile = iEngine->ActiveProfileL();
    //CleanupStack::PushL(currentProfile);
    MProEngToneSettings& settings = currentProfile->ToneSettings();
    iWarningTones = settings.WarningAndGameTones();
    iMessagingTones = !( currentProfile->IsSilent() );
    iVibra = settings.VibratingAlert();
  
    CMapGASRingingType( settings.RingingType());
    
    //CleanupStack::PopAndDestroy(currentProfile);
   
    MProEngTones& profileTones=currentProfile->ProfileTones();
    
    iMessageToneName=profileTones.MessageAlertTone().AllocL();
   
    iEmailToneName=profileTones.EmailAlertTone().AllocL();
    
    iVideoCallToneName=profileTones.VideoCallRingingTone().AllocL();
    
    iRingingTone1Name=profileTones.RingingTone1().AllocL();
    
    iRingingTone2Name=profileTones.RingingTone2().AllocL();
        
  
    CMapGASKeypadVolume(settings.KeypadVolume());
    
    iProfileEngineExtended =::CreateProfileEngineExtended2L();
   
     iSilenceMode = iProfileEngineExtended->SilenceModeL();
     RDebug::Printf("value of silence mode %d",iSilenceMode);
    // Get PublicSilence from CenRep
    TInt featureBitmask( 0 );     
    CRepository* profileCenRep = CRepository::NewL( KCRUidProfilesLV );
    CleanupStack::PushL(profileCenRep);
    User::LeaveIfError( profileCenRep->Get( KProfilesLVFlags, featureBitmask ) );
    if  ( featureBitmask & KProEngFeatureIdForceSilentMode )
        iPublicSilence = ETrue;
    else
        iPublicSilence = EFalse;
    CleanupStack::PopAndDestroy(profileCenRep);
    
    TSecurityPolicy read = TSecurityPolicy(ECapabilityWriteDeviceData);
    TSecurityPolicy write = TSecurityPolicy(ECapabilityMultimediaDD);

    // Define and Initialize Warning Tones PnS key
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASWarningTones,
                                    RProperty::EInt,
                                    read,
                                    write));	    
    User::LeaveIfError(iWarningTonesProperty.Attach(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASWarningTones));
    User::LeaveIfError(iWarningTonesProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASWarningTones,
                                                iWarningTones));
    
    // Define and Initialize Messaging Tones PnS key
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASMessageTones,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iMessagingTonesProperty.Attach(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASMessageTones));
    User::LeaveIfError(iMessagingTonesProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASMessageTones,
                                                iMessagingTones));
    
    // Define and Initialize Silent Profile PnS key
     User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASSilentProfile,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iSilentProfileProperty.Attach(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASSilentProfile));
    User::LeaveIfError(iSilentProfileProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASSilentProfile,
                                                iSilentProfile));
    
    // Define and Initialize Vibra PnS key
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASVibra,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iVibraProperty.Attach(
                                        KGASPSUidGlobalAudioSettings,
                                        KGASVibra));
    User::LeaveIfError(iVibraProperty.Set(
                                        KGASPSUidGlobalAudioSettings,
                                        KGASVibra,
                                        iVibra));
                        
    // Define and Initialize Public Silence PnS key
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASPublicSilence,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iPublicSilenceProperty.Attach(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASPublicSilence));
    User::LeaveIfError(iPublicSilenceProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASPublicSilence,
                                                iPublicSilence));
    
    
    
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASRingingType,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iRingingTypeProperty.Attach(
                                              KGASPSUidGlobalAudioSettings,
                                              KGASRingingType));
    User::LeaveIfError(iRingingTypeProperty.Set(
                                              KGASPSUidGlobalAudioSettings,
                                              KGASRingingType,
                                              iGASRingingType));
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASMessageToneName,
                                    RProperty::EText,
                                    read,
                                    write));
    User::LeaveIfError(iMessageToneNameProperty.Attach(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASMessageToneName));
    User::LeaveIfError(iMessageToneNameProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASMessageToneName,
                                                *iMessageToneName));
    
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASEmailToneName,
                                    RProperty::EText,
                                    read,
                                    write));
    User::LeaveIfError(iEmailToneNameProperty.Attach(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASEmailToneName));
    User::LeaveIfError(iEmailToneNameProperty.Set(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASEmailToneName,
                                               *iEmailToneName));
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASVideoCallToneName,
                                    RProperty::EText,
                                    read,
                                    write));
    User::LeaveIfError(iVideoCallToneNameProperty.Attach(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASVideoCallToneName));
    User::LeaveIfError(iVideoCallToneNameProperty.Set(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASVideoCallToneName,
                                               *iVideoCallToneName));
        
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASRingingTone1Name,
                                    RProperty::EText,
                                    read,
                                    write));
    User::LeaveIfError(iRingingTone1NameProperty.Attach(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASRingingTone1Name));
    User::LeaveIfError(iRingingTone1NameProperty.Set(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASRingingTone1Name,
                                               *iRingingTone1Name));
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASRingingTone2Name,
                                    RProperty::EText,
                                    read,
                                    write));
    User::LeaveIfError(iRingingTone2NameProperty.Attach(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASRingingTone2Name));
    User::LeaveIfError(iRingingTone2NameProperty.Set(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASRingingTone2Name,
                                               *iRingingTone2Name));
    
    User::LeaveIfError(RProperty::Define(
                                    KGASPSUidGlobalAudioSettings,
                                    KGASKeypadToneVolume,
                                    RProperty::EInt,
                                    read,
                                    write));
    User::LeaveIfError(iKeypadVolumeProperty.Attach(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASKeypadToneVolume));
    User::LeaveIfError(iKeypadVolumeProperty.Set(
                                               KGASPSUidGlobalAudioSettings,
                                               KGASKeypadToneVolume,
                                               iGASKeypadVolume));
    
    User::LeaveIfError(RProperty::Define(
                                        KGASPSUidGlobalAudioSettings,
                                        KGASSilenceMode,
                                        RProperty::EInt,
                                        read,
                                        write));
        User::LeaveIfError(iSilenceModeProperty.Attach(
                                                   KGASPSUidGlobalAudioSettings,
                                                   KGASSilenceMode));
        User::LeaveIfError(iSilenceModeProperty.Set(
                                                   KGASPSUidGlobalAudioSettings,
                                                   KGASSilenceMode,
                                                   iSilenceMode));

    //Stream Gain Control
    TInt status = RProperty::Define(
                                KPSUidMediaStreamProperties,
                                KMediaStreamCurrentGain,
                                RProperty::EInt,
                                read,
                                write);
    if ( status != KErrAlreadyExists )
	    User::LeaveIfError(status);

    // Audio Clients List
    InitializeAudioClientsListPSKeysL();
    }

void CProfileSettingsMonitorServerImpl::InitializeAudioClientsListPSKeysL()
    {
    DEB_TRACE0(_L("CProfileSettingsMonitorServerImpl::InitializeAudioClientsListPSKeysL:Enter"));
	// define KAudioPolicyApplicationAudioStatePlaying property to be a byte array
	TInt status = RProperty::Define(KPSUidMMFAudioServer,
	                                KAudioPolicyApplicationAudioStatePlaying,
	                                RProperty::EByteArray,
		                            KClientStateReadPolicy,
		                            KMMFAudioServerSIDPolicy,
		                            sizeof(struct TAudioPolicyProcessIdListStruct));
	DEB_TRACE1(_L("Define KAudioPolicyApplicationAudioStatePlaying returns %d"), status);
	if (status != KErrAlreadyExists)
	    {
	    User::LeaveIfError(status);
	    }

	// define KAudioPolicyApplicationAudioStateRecording property to be a byte array
	status = RProperty::Define(KPSUidMMFAudioServer,
	                           KAudioPolicyApplicationAudioStateRecording,
	                           RProperty::EByteArray,
	                           KClientStateReadPolicy,
	                           KMMFAudioServerSIDPolicy,
	                           sizeof(struct TAudioPolicyProcessIdListStruct));
	DEB_TRACE1(_L("Define KAudioPolicyApplicationAudioStateRecording returns %d"), status);
	if (status != KErrAlreadyExists)
	    {
	    User::LeaveIfError(status);
	    }

	// define KAudioPolicyApplicationAudioStatePaused property to be a byte array
	status = RProperty::Define(KPSUidMMFAudioServer,
	                           KAudioPolicyApplicationAudioStatePaused,
	                           RProperty::EByteArray,
	                           KClientStateReadPolicy,
	                           KMMFAudioServerSIDPolicy,
	                           sizeof(struct TAudioPolicyProcessIdListStruct));
	DEB_TRACE1(_L("Define KAudioPolicyApplicationAudioStatePaused returns %d"), status);
	if (status != KErrAlreadyExists)
    	{
    	User::LeaveIfError(status);
    	}

	// define KAudioPolicyForceMutedClients property to be a byte array
	status = RProperty::Define(KPSUidMMFAudioServer,
	                           KAudioPolicyForceMutedClients,
	                           RProperty::EByteArray,
	                           KClientStateReadPolicy,
	                           KMMFAudioServerSIDPolicy,
	                           sizeof(struct TAudioPolicyProcessIdListStruct));
	DEB_TRACE1(_L("Define KAudioPolicyForceMutedClients returns %d"), status);
	if (status != KErrAlreadyExists)
    	{
    	User::LeaveIfError(status);
    	}

	// define KAudioPolicyAudioClients property to be a byte array
	status = RProperty::Define(KPSUidMMFAudioServer,
	                           KAudioPolicyAudioClients,
	                           RProperty::EByteArray,
	                           KClientStateReadPolicy,
	                           KMMFAudioServerSIDPolicy,
	                           sizeof(struct TAudioClientListStruct));
	DEB_TRACE1(_L("Define KAudioPolicyAudioClients returns %d"), status);
	if (status != KErrAlreadyExists)
    	{
    	User::LeaveIfError(status);
    	}
    DEB_TRACE0(_L("CProfileSettingsMonitorServerImpl::InitializeAudioClientsListPSKeysL:Exit"));
    }

void CProfileSettingsMonitorServerImpl::HandleProfileActivatedL( TInt aProfileId )
    {
    DEB_TRACE1(_L("CProfileSettingsMonitorServerImpl::HandleProfileActivatedL[%d]"), aProfileId);
    
    if ( iActiveProfileId != aProfileId )
        {
        MProEngProfile* currentProfile = iEngine->ActiveProfileL();
        CleanupReleasePushL(*currentProfile);
        
        TBool silentProfile;
        iActiveProfileId = aProfileId;
        //iActiveProfileId = iEngine->ActiveProfileId();
        
        if(iActiveProfileId == 1) // Silent profile is always '1'
            silentProfile = ETrue;
        else
            silentProfile = EFalse;
        
        if(silentProfile != iSilentProfile)
            {
            iSilentProfile = silentProfile;
            User::LeaveIfError(iSilentProfileProperty.Set(
                                                        KGASPSUidGlobalAudioSettings,
                                                        KGASSilentProfile,
                                                        iSilentProfile));
            }
            
        MProEngToneSettings& settings = currentProfile->ToneSettings();
        
        if(iWarningTones != settings.WarningAndGameTones())
            {
            iWarningTones = settings.WarningAndGameTones();
            User::LeaveIfError(iWarningTonesProperty.Set(
                                                        KGASPSUidGlobalAudioSettings,
                                                        KGASWarningTones,
                                                        iWarningTones));
            }
        
        if(iMessagingTones != (!( currentProfile->IsSilent() )))
            {
            iMessagingTones = !( currentProfile->IsSilent() );
            User::LeaveIfError(iMessagingTonesProperty.Set(
                                                        KGASPSUidGlobalAudioSettings,
                                                        KGASMessageTones,
                                                        iMessagingTones));
            }
        if(iVibra != settings.VibratingAlert())
            {
            iVibra = settings.VibratingAlert();
            User::LeaveIfError(iVibraProperty.Set(
                                                KGASPSUidGlobalAudioSettings,
                                                KGASVibra,
                                                iVibra));
            }
        if(iGASRingingType != settings.RingingType())
            {
            
             CMapGASRingingType(settings.RingingType());
            
             User::LeaveIfError(iRingingTypeProperty.Set(
                                                         KGASPSUidGlobalAudioSettings,
                                                         KGASRingingType,
                                                         iGASRingingType));
             
            }
        MProEngTones &profileTones=currentProfile->ProfileTones();
        if(iMessageToneName->Compare(profileTones.MessageAlertTone())!= 0)
            {
              delete iMessageToneName;
              iMessageToneName=NULL;
              iMessageToneName=profileTones.MessageAlertTone().AllocL();
              User::LeaveIfError(iMessageToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                              KGASMessageToneName,
                                                              *iMessageToneName));
            }
        if(iEmailToneName->Compare(profileTones.EmailAlertTone())!= 0)
            {
              delete iEmailToneName;
              iEmailToneName=NULL;
              iEmailToneName=profileTones.EmailAlertTone().AllocL();
              User::LeaveIfError(iEmailToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                            KGASEmailToneName,
                                                                            *iEmailToneName));
            }
        if(iVideoCallToneName->Compare(profileTones.VideoCallRingingTone())!=0)
            {
              delete iVideoCallToneName;
              iVideoCallToneName=NULL;
              iVideoCallToneName=profileTones.VideoCallRingingTone().AllocL();
              User::LeaveIfError(iVideoCallToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                          KGASVideoCallToneName,
                                                                                          *iVideoCallToneName));
              
            }
        if(iRingingTone1Name->Compare(profileTones.RingingTone1())!=0)
            {
          delete iRingingTone1Name;
          iRingingTone1Name=NULL;
          iRingingTone1Name=profileTones.RingingTone1().AllocL();
          User::LeaveIfError(iRingingTone1NameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                      KGASRingingTone1Name,
                                                                                      *iRingingTone1Name));
            }
        if(iRingingTone2Name->Compare(profileTones.RingingTone2())!=0)
                    {
                   delete iRingingTone2Name;
                  iRingingTone2Name=NULL;
                  iRingingTone2Name=profileTones.RingingTone2().AllocL();
                  User::LeaveIfError(iRingingTone2NameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                              KGASRingingTone2Name,
                                                                                              *iRingingTone2Name));
                    }
        if(iGASKeypadVolume!=settings.KeypadVolume())
            {
              
              CMapGASKeypadVolume(settings.KeypadVolume());
              User::LeaveIfError(iKeypadVolumeProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                        KGASKeypadToneVolume,
                                                                                        iGASKeypadVolume));
              
              
            }
        if(iSilenceMode != iProfileEngineExtended->SilenceModeL())
            {
             RDebug::Printf("silence mode changed");
             iSilenceMode = iProfileEngineExtended->SilenceModeL();
             User::LeaveIfError(iSilenceModeProperty.Set(
                                                        KGASPSUidGlobalAudioSettings,
                                                        KGASSilenceMode,
                                                        iSilenceMode));
            }
        
        CleanupStack::PopAndDestroy(currentProfile);
        }
#ifdef _DEBUG
    else
        {
        RDebug::Print(_L("CProfileSettingsMonitorServerImpl::HandleProfileActivatedL[%d]- \
            Activated is same as current profile"), aProfileId);
        }
#endif // _DEBUG    
    }

void CProfileSettingsMonitorServerImpl::HandleProfileActivationNotificationError( TInt /*aError*/ )
    {
    DEB_TRACE0(_L("CMyProfileNameArrayObserver::HandleProfileActivationNotificationError"));
	}

void CProfileSettingsMonitorServerImpl::HandleActiveProfileModifiedL()
    {
    DEB_TRACE0(_L("CProfileSettingsMonitorServerImpl::HandleActiveProfileModifiedL"));
    
    iActiveProfileId = iEngine->ActiveProfileId();
    MProEngProfile* currentProfile = iEngine->ActiveProfileL();
    CleanupReleasePushL(*currentProfile);
    
    MProEngToneSettings& settings = currentProfile->ToneSettings();
    MProEngTones& profileTones=currentProfile->ProfileTones();
    if(iWarningTones != settings.WarningAndGameTones())
        {
        iWarningTones = settings.WarningAndGameTones();
        User::LeaveIfError(iWarningTonesProperty.Set(
                                                    KGASPSUidGlobalAudioSettings,
                                                    KGASWarningTones,
                                                    iWarningTones));
        }
    if(iMessagingTones != (!( currentProfile->IsSilent() )))
        {
        iMessagingTones = !( currentProfile->IsSilent() );
        User::LeaveIfError(iMessagingTonesProperty.Set(
                                                    KGASPSUidGlobalAudioSettings,
                                                    KGASMessageTones,
                                                    iMessagingTones));
        }

    if(iVibra != settings.VibratingAlert())
        {
        iVibra = settings.VibratingAlert();
        User::LeaveIfError(iVibraProperty.Set(
                                            KGASPSUidGlobalAudioSettings,
                                            KGASVibra,
                                            iVibra));
        }
    if(iGASRingingType != settings.RingingType())
        {
        
         CMapGASRingingType(settings.RingingType());
         
         User::LeaveIfError(iRingingTypeProperty.Set(
                                                     KGASPSUidGlobalAudioSettings,
                                                     KGASRingingType,
                                                     iGASRingingType));
        }
    if(iMessageToneName->Compare(profileTones.MessageAlertTone())!= 0)
        {
          delete iMessageToneName;
          iMessageToneName=NULL;
          iMessageToneName=profileTones.MessageAlertTone().AllocL();
          
          User::LeaveIfError(iMessageToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                          KGASMessageToneName,
                                                          *iMessageToneName));
        }
    
    if(iEmailToneName->Compare(profileTones.EmailAlertTone())!= 0)
        {
          delete iEmailToneName;
          iEmailToneName=NULL;
          iEmailToneName=profileTones.EmailAlertTone().AllocL();
          User::LeaveIfError(iEmailToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                        KGASEmailToneName,
                                                                        *iEmailToneName));
        }
    if(iVideoCallToneName->Compare(profileTones.VideoCallRingingTone())!=0)
        {
          delete iVideoCallToneName;
          iVideoCallToneName=NULL;
          iVideoCallToneName=profileTones.VideoCallRingingTone().AllocL();
          User::LeaveIfError(iVideoCallToneNameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                      KGASVideoCallToneName,
                                                                                      *iVideoCallToneName));
          
        }
    if(iRingingTone1Name->Compare(profileTones.RingingTone1())!=0)
        {
         delete iRingingTone1Name;
         iRingingTone1Name=NULL;
         iRingingTone1Name=profileTones.RingingTone1().AllocL();
         User::LeaveIfError(iRingingTone1NameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                  KGASRingingTone1Name,
                                                                                  *iRingingTone1Name));
         
        }
    if(iRingingTone2Name->Compare(profileTones.RingingTone2())!=0)
        {
          delete iRingingTone2Name;
          iRingingTone2Name=NULL;
          iRingingTone2Name=profileTones.RingingTone2().AllocL();
          User::LeaveIfError(iRingingTone2NameProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                      KGASRingingTone2Name,
                                                                                      *iRingingTone2Name));
         
            }
    if(iGASKeypadVolume!=settings.KeypadVolume())
        {
         
          CMapGASKeypadVolume( settings.KeypadVolume());
          User::LeaveIfError(iKeypadVolumeProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                                KGASKeypadToneVolume,
                                                                                                iGASKeypadVolume));
          
          
        }
    if(iSilenceMode != iProfileEngineExtended ->SilenceModeL())
        {
    RDebug::Printf("silence mode changed");
         iSilenceMode = iProfileEngineExtended ->SilenceModeL();
         User::LeaveIfError(iSilenceModeProperty.Set(KGASPSUidGlobalAudioSettings,
                                                                                KGASSilenceMode,
                                                                                iSilenceMode));
         }
   
    CleanupStack::PopAndDestroy(currentProfile);
    }

void CProfileSettingsMonitorServerImpl::HandleActiveProfileNotificationError( TInt /*aError*/ )
    {
    DEB_TRACE0(_L("CMyProfileNameArrayObserver::HandleProfileActivationNotificationError"));
	}
void CProfileSettingsMonitorServerImpl::CMapGASRingingType(TProfileRingingType aRingingType)
    {
    switch(aRingingType)
            {
            case EProfileRingingTypeRinging :   iGASRingingType=CGlobalAudioSettings::EGASRingingTypeRinging;
                                                 break;
               
            case EProfileRingingTypeAscending:iGASRingingType=CGlobalAudioSettings::EGASRingingTypeAscending;
                                               break;
              
            case EProfileRingingTypeRingingOnce :iGASRingingType=CGlobalAudioSettings::EGASRingingTypeRingingOnce;
                                                  break;
               
            case EProfileRingingTypeBeepOnce :iGASRingingType=CGlobalAudioSettings::EGASRingingTypeBeepOnce;
                                              break;
               
            case EProfileRingingTypeSilent:iGASRingingType=CGlobalAudioSettings::EGASRingingTypeSilent;
                                           break;
            }
    }
void CProfileSettingsMonitorServerImpl::CMapGASKeypadVolume(TProfileKeypadVolume aKeypadVolume)
    {
     switch(aKeypadVolume)
         {
         case  EProfileKeypadVolumeOff  : iGASKeypadVolume=CGlobalAudioSettings::EGASKeypadVolumeOff;
                                          break;
         case EProfileKeypadVolumeLevel1 :iGASKeypadVolume=CGlobalAudioSettings::EGASKeypadVolumeLevel1;
                                          break;
         case EProfileKeypadVolumeLevel2 :iGASKeypadVolume=CGlobalAudioSettings::EGASKeypadVolumeLevel2;
                                          break;
         case  EProfileKeypadVolumeLevel3 :iGASKeypadVolume=CGlobalAudioSettings::EGASKeypadVolumeLevel3;
                                           break;
         }
    }
// End of file
	
