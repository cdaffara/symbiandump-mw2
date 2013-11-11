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


#include "ReverbEffectImpl.h"
#include <EnvironmentalReverbBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CReverbEffect::CReverbEffect()
    {
    // No Impl
    }

CReverbEffect::~CReverbEffect()
    {
	delete iPrevReverbProxy;
	delete  iReverbProxy;
	iObservers.Close();
    }

TInt CReverbEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidEnvironmentalReverbEffect) );
    return status;
    }

// From MControl begins
TInt CReverbEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CReverbEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
	}

TUid CReverbEffect::Type()
    {
    return KReverbEffectControl;
    }

TControlType CReverbEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CReverbEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CReverbEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iReverbProxy)
		{
		TRAP(error,iReverbProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

/**
* Gets the decay HF Ratio in hundredths
* @since 5.0
* @return decay HF Ratio
*/
TInt CReverbEffect::DecayHFRatio(TUint &aRatio)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aRatio = iReverbProxy->DecayHFRatio();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb decay HF Ratio minimum and maximum in hundredths.
* @since 5.0
* @param aMin Minimum decay HF Ratio
* @param aMax Maximum decay HF Ratio
*/
TInt CReverbEffect::DecayHFRatioRange(TUint& aMin, TUint& aMax)
	{
	TInt status(KErrNone);
	TUint32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->DecayHFRatioRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the decay time in milliseconds
* @since 5.0
* @return decay time
*/
TInt CReverbEffect::DecayTime(TUint &aDecayTime)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDecayTime = iReverbProxy->DecayTime();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the allowable reverb decay time range in milliseconds.
* @since 5.0
* @param aMin Minimum decay time in milliseconds
* @param aMax Maximum decay time in milliseconds
*/
TInt CReverbEffect::DecayTimeRange(TUint& aMin, TUint& aMax)
	{
	TInt status(KErrNone);
	TUint32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->DecayTimeRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the density current value as a percentage in hundredths
* @since 5.0
* @return density value
*/
TInt CReverbEffect::Density(TUint &aDensity)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDensity = iReverbProxy->Density();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the diffusion current value as a percentage in hundredths.
* @since 5.0
* @return diffusion value
*/
TInt CReverbEffect::Diffusion(TUint &aDiffusion)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDiffusion = iReverbProxy->Diffusion();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb reflections delay in ms.
* @since 5.0
* @return reverb reflections delay
*/
TInt CReverbEffect::ReflectionsDelay(TUint &aDelay)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDelay = iReverbProxy->ReflectionsDelay();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb reflections delay maximum in milliseconds.
* @since 5.0
* @return reverb reflections delay maximum
*/
TInt CReverbEffect::ReflectionsDelayMax(TUint &aDelayMax)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDelayMax = iReverbProxy->ReflectionsDelayMax();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb reflections level in mB
* @since 5.0
* @return Reverb reflections level
*/
TInt CReverbEffect::ReflectionsLevel(TInt &aLevel)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aLevel = iReverbProxy->ReflectionsLevel();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb reflections level maximum and minimum in mB
* @since 5.0
* @param aMin Minimum reflections level
* @param aMax Maximum reflections level
*/
TInt CReverbEffect::ReflectionLevelRange( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	TInt32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->ReflectionLevelRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb delay in milliseconds
* @since 5.0
* @return reverb delay
*/
TInt CReverbEffect::ReverbDelay(TUint &aDelay)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDelay = iReverbProxy->ReverbDelay();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb delay maximum in milliseconds
* @since 5.0
* @return reverb delay maximum
*/
TInt CReverbEffect::ReverbDelayMax(TUint &aDelayMax)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDelayMax = iReverbProxy->ReverbDelayMax();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb current level in mB
* @since 5.0
* @return reverb current level
*/
TInt CReverbEffect::ReverbLevel(TInt &aLevel)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aLevel = iReverbProxy->ReverbLevel();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the reverb current level maximum and minimum in mB
* @since 5.0
* @param aMin Minimum current level
* @param aMax Maximum current level
* @return -
*/
TInt CReverbEffect::ReverbLevelRange( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	TInt32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->ReverbLevelRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the room HF level current ratio
* @since 5.0
* @return room HF level ratio
*/
TInt CReverbEffect::RoomHFLevel(TInt &aLevel)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aLevel = iReverbProxy->RoomHFLevel();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the room HF level maximum and minimum ratios
* @since 5.0
* @param aMin Minimum current room HF level
* @param aMax Maximum current room HF level
* @return -
*/
TInt CReverbEffect::RoomHFLevelRange( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	TInt32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->RoomHFLevelRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the room level current value in mB
* @since 5.0
* @return room level value
*/
TInt CReverbEffect::RoomLevel(TInt &aLevel)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aLevel = iReverbProxy->RoomLevel();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the room level maximum and minimum in mB
* @since 5.0
* @param aMin Minimum current room level
* @param aMax Maximum current room level
* @return -
*/
TInt CReverbEffect::RoomLevelRange( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	TInt32 min, max;
	if(iReverbProxy)
		{
		iReverbProxy->RoomLevelRange(min,max);	
		aMin = min;
		aMax = max;
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the decay HF Ratio in hundredths
* @since 5.0
* @param aDecayHFRatio The decay high frequence ratio in hundredths
* @return -
*/
TInt CReverbEffect::SetDecayHFRatio( TUint aDecayHFRatio )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetDecayHFRatioL(aDecayHFRatio));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the decay time in millisecond
* @since 5.0
* @param aDecayTime Decay time in ms
*/
TInt CReverbEffect::SetDecayTime( TUint aDecayTime )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetDecayTimeL(aDecayTime));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the density value as percentage in hundredths
* @since 5.0
* @param aDensity The density.
*/
TInt CReverbEffect::SetDensity( TUint aDensity )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetDensityL(aDensity));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the diffusion value as a percentage in hundredths
* @since 5.0
* @param aDiffusion The diffusion.
*/
TInt CReverbEffect::SetDiffusion( TUint aDiffusion )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetDiffusionL(aDiffusion));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the reverb reflections delay
* @since 5.0
* @param aRefectionsDelay The reflection delay in ms.
*/
TInt CReverbEffect::SetReflectionsDelay( TUint aReflectionsDelay )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetReflectionsDelayL(aReflectionsDelay));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the reverb reflections level in milli-dB
* @since 5.0
* @param aRefectionsLevel The reflection level in mB
*/
TInt CReverbEffect::SetReflectionsLevel( TInt aReflectionsLevel )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetReflectionsLevelL(aReflectionsLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the reverb delay
* @since 5.0
* @param aReverbDelay The reverb delay in ms
*/
TInt CReverbEffect::SetReverbDelay( TUint aReverbDelay )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetReverbDelayL(aReverbDelay));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the reverb level
* @since 5.0
* @param aReverbLevel The reverb level in mB
*/
TInt CReverbEffect::SetReverbLevel( TInt aReverbLevel )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetReverbLevelL(aReverbLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the room HF level ratio
* @since 5.0
* @param aRoomHFLevel The room high frequency ratio
*/
TInt CReverbEffect::SetRoomHFLevel( TInt aRoomHFLevel )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetRoomHFLevelL(aRoomHFLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Sets the room level value in milli-dB
* @since 5.0
* @param aRoomLevel The room level
*/
TInt CReverbEffect::SetRoomLevel( TInt aRoomLevel )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->SetRoomLevelL(aRoomLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Gets the total delay maximum in milliseconds
* @since 5.0
* @return reverb delay maximum
*/
TInt CReverbEffect::DelayMax(TUint &aDelayMax)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aDelayMax = iReverbProxy->DelayMax();	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}




// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CReverbEffect::Disable()
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->DisableL());	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Enable the effect
* @since 5.0
*/
TInt CReverbEffect::Enable()
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->EnableL());	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;					
	}

/**
* Enforce the effect.
* @since 5.0
* @param aEnforced Indicate the effect is to be enforced or not. ETrue = Enforced.
*/
TInt CReverbEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		TRAP(status,iReverbProxy->EnforceL(aEnforced));	
		}	
	else
		{
		status = KErrNotReady;	
		}					
	return status;		
	}

/**
* Check if this effect object currently has update rights.
* A client can lose update rights in some hardware platforms where there are a limited
* number of instances of an effect that can exist at the same time. When an effect instance
* has lost update rights the user can still change settings, but any calls to Apply the
* settings will be deferred until update rights are regained.
* @since 5.0
* @return ETrue if this object currently has rights to update the settings of this effect,
*         EFalse otherwise.
*/
TInt CReverbEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aHaveUpdateRights = iReverbProxy->HaveUpdateRights();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

/**
* Check if the effect is enabled
* @since 5.0
* @return ETrue if the effect is enabled, EFalse if the effect is disabled.
*/
TInt CReverbEffect::IsEnabled(TBool &aEnabled)
	{		
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aEnabled = iReverbProxy->IsEnabled();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Check if the effect is enforced.
* @since 5.0
* @return ETrue if the effect is enforced, EFalse if the effect isn ot enforced.
*/
TInt CReverbEffect::IsEnforced(TBool &aEnforced)
	{		
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aEnforced = iReverbProxy->IsEnforced();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/*
* Get the unique identifier of the audio effect
* @since 5.0
* @return Unique identifier of the audio effect object.
*/
TInt CReverbEffect::Uid(TUid &aUid)
	{		
	TInt status(KErrNone);
	if(iReverbProxy)
		{
		aUid = iReverbProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CReverbEffect::Event( TEffectControlEvent aEvent )
    {
	TInt status(KErrNone);    	
    // Controller Loaded with ECIBuilderCreated
    if ( aEvent == ECIBuilderCreated )
        {
    	RDebug::Print(_L("Deleting Proxy"));	
		status = DeleteEffectProxy();
		RDebug::Print(_L("Deleting Proxy [%d]"),status);	
		status = CreateEffectProxy();	
		RDebug::Print(_L("Creating Proxy [%d]"),status);	
		if(status == KErrNone)
			{
			SavePreviousSettings();	
			}
		else
			{
			for ( TInt i = 0; i < iObservers.Count(); i++ )
				{
				iObservers[i]->Event(this,MAudioEffectControl::KDisabled,NULL);
				} 		
			}	
        }
    else if ( aEvent == EMessageHandlerDeleted )
        {
        if(status == KErrNone)
        	{
        	DeleteEffectProxy();	
        	}
        }
    }

// From CEffectControlBase ends
TInt CReverbEffect::GetCEnvironmentalReverb( CEnvironmentalReverb*& aOutEnvRev )
    {
    TInt status(KErrNotFound);
    if ( iReverbProxy )
        {
        aOutEnvRev = iReverbProxy;
        status = KErrNone;
        }
    return status;
    }

TInt CReverbEffect::CreateEffectProxy()
	{
	TInt status(KErrNone);
	status = GetMessageHandle(iMsgHndlrHandlePckg);
	if(status != KErrNone)
		{
		return status;	
		}
	iCustomCommand = GetCustomCommand();		
	if(!iCustomCommand)
		{
		return KErrNotReady;	
		}

	TRAP(status,iReverbProxy = CEnvironmentalReverbProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));
	if(status == KErrNone)
		{
		TRAP(status,iReverbProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CReverbEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iReverbProxy)
		{
		iPrevReverbProxy = iReverbProxy;	
		}
	iReverbProxy = NULL;	
	return status;
	}

TInt CReverbEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevReverbProxy)
		{
		TInt decayRatio = iPrevReverbProxy->DecayHFRatio();
		TRAP(status , iReverbProxy->SetDecayHFRatioL(decayRatio));	

		TInt decayTime = iPrevReverbProxy->DecayTime();
		TRAP(status , iReverbProxy->SetDecayTimeL(decayTime));	

		TInt density = iPrevReverbProxy->Density();
		TRAP(status , iReverbProxy->SetDensityL(density));	

		TInt diffusion = iPrevReverbProxy->Diffusion();
		TRAP(status , iReverbProxy->SetDiffusionL(diffusion));	

		TInt refDelay = iPrevReverbProxy->ReflectionsDelay();
		TRAP(status , iReverbProxy->SetReflectionsDelayL(refDelay));	

		TInt refLevel = iPrevReverbProxy->ReflectionsLevel();
		TRAP(status , iReverbProxy->SetReflectionsLevelL(refLevel));	

		TInt reverbDelay = iPrevReverbProxy->ReverbDelay();
		TRAP(status , iReverbProxy->SetReverbDelayL(reverbDelay));	

		TInt reverbLevel = iPrevReverbProxy->ReverbLevel();
		TRAP(status , iReverbProxy->SetReverbLevelL(reverbLevel));	

		TInt roomHfLevel = iPrevReverbProxy->RoomHFLevel();
		TRAP(status , iReverbProxy->SetRoomHFLevelL(roomHfLevel));	

		TInt roomLevel = iPrevReverbProxy->RoomLevel();
		TRAP(status , iReverbProxy->SetRoomLevelL(roomLevel));	

		TBool enforce = iPrevReverbProxy->IsEnforced();
		iReverbProxy->EnforceL(enforce);	

		TBool enable = iPrevReverbProxy->IsEnabled();
		if(enable)
			{
			iReverbProxy->EnableL();	
			}

		delete iPrevReverbProxy;
		iPrevReverbProxy = NULL;
		}				
	return status;	
	}

 void CReverbEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
