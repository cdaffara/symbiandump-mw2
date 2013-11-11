/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Project file for EnhancedMediaClient Utility
*
*/


#include "VolumeEffectImpl.h"
// For KUidInterfaceMMFAudioPlayDevice
#include <mmf/common/mmfstandardcustomcommands.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <mmf/common/mmfstandardcustomcommandsimpl.h>
#include <mmf/common/mmfstandardcustomcommandsenums.h>
#endif
// For TMMFAudioConfig
#include <mmf/common/mmfaudio.h>
// For TMMFMessageDestinationPckg
#include <mmf/common/mmfcontrollerframework.h>


using namespace multimedia;

CVolumeEffect::CVolumeEffect()
: iVolume(KErrUnknown)
    ,iRampDuration(0)
    ,iEnabled(false)
    {
    // No Impl
    }

CVolumeEffect::~CVolumeEffect()
    {
    // No Impl
    }

TInt CVolumeEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL( KUidInterfaceMMFAudioPlayDevice ) );
    return status;
    }

// From MControl begins
TInt CVolumeEffect::AddObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;
    }

TInt CVolumeEffect::RemoveObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;
    }

TUid CVolumeEffect::Type()
    {
    return KVolumeEffectControl;
    }

TControlType CVolumeEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CVolumeEffect::Apply()
    {
        iEnabled = true;
        return DoApply();
    }

// From MEffectControl ends

TInt CVolumeEffect::DoApply()
    {
    TInt error(KErrNone);    
    TMMFMessageDestination msgDest( KUidInterfaceMMFAudioPlayDevice );
    TMMFMessageDestinationPckg msgDestPckg( msgDest );
	TPckgBuf<TMMFAudioConfig> configPackage;
	configPackage().iVolume = iVolume;
	error =  CEffectControlBase::CustomCommandSync( msgDestPckg,
                                                  EMMFAudioPlayDeviceSetVolume,
                                                  configPackage,
                                                  KNullDesC8);
    
    
    
    if(iRampDuration > 0)
        {
        configPackage().iRampDuration = iRampDuration;
    	error =  CEffectControlBase::CustomCommandSync( msgDestPckg, 
               										 EMMFAudioPlayDeviceSetVolumeRamp, 
              										 configPackage,
               										 KNullDesC8);  
    	}
    
    return error;    
    }



// From MVolumeEffect begins
TInt CVolumeEffect::SetVolume( TInt& aVolume )
    {
    iVolume = aVolume;
    return KErrNone;
    }

TInt CVolumeEffect::GetVolume( TInt& aVolume )
    {    
    aVolume = iVolume;
    return KErrNone;
    }

TInt CVolumeEffect::GetMaxVolume( TInt& aVolume )
    {
    TMMFMessageDestination msgDest( KUidInterfaceMMFAudioPlayDevice );
    TMMFMessageDestinationPckg msgDestPckg( msgDest );
	TPckgBuf<TMMFAudioConfig> configPackage;
	TInt error =  CEffectControlBase::CustomCommandSync( msgDestPckg, 
                                                         EMMFAudioPlayDeviceGetMaxVolume, 
                                                         KNullDesC8,
                                                         KNullDesC8,
                                                         configPackage );
	if (!error)
		aVolume = configPackage().iMaxVolume;
	
	return error;
    
    }

TInt CVolumeEffect::GetMinVolume( TInt& aMinVolume )
    {
    aMinVolume = 0;
    return KErrNone;
    }

TInt CVolumeEffect::GetDefaultVolume( TInt& aDefaultVolume )
    {
    TInt maxVol=0;    
    TInt err = GetMaxVolume(maxVol);
    if(err)
        return err;
        
    aDefaultVolume = (maxVol)/2;
    return KErrNone;
    }

TInt CVolumeEffect::SetVolumeRamp(TInt /*aInitialVol*/,TInt aFinalVol,TUint64& aDuration,TVolumeRampMode /*aMode*/)
    {
    iVolume = aFinalVol;    
    iRampDuration = aDuration;
    return KErrNone;
    }

// From CEffectControlBase begins
void CVolumeEffect::Event( TEffectControlEvent aEvent )
    {
    TMMFMessageDestination msgDest( KUidInterfaceMMFAudioPlayDevice );
    TMMFMessageDestinationPckg msgDestPckg( msgDest );
	TPckgBuf<TMMFAudioConfig> configPackage;
        // Controller Loaded with ECIBuilderCreated
    if ( aEvent == ECIBuilderCreated )
        {
		if(iVolume == KErrUnknown)
			{
			TPckgBuf<TMMFAudioConfig> configPackage;
			TInt error =  CEffectControlBase::CustomCommandSync(msgDestPckg, 
														EMMFAudioPlayDeviceGetVolume, 
														KNullDesC8,
														KNullDesC8,
														configPackage);
	
			if (!error)
				{
				iVolume = configPackage().iVolume;
				}
			}
		}
	        	
    if(iEnabled && iVolume != KErrUnknown)
        {
        DoApply();
        }
    }

// From CEffectControlBase ends

// End of file
