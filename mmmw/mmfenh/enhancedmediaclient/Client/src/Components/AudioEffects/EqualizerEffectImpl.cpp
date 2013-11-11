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


#include "EqualizerEffectImpl.h"
#include <AudioEqualizerBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CEqualizerEffect::CEqualizerEffect()
    {
    // No Impl
    }

CEqualizerEffect::~CEqualizerEffect()
    {
	delete iPrevEqualizerProxy;
	delete  iEqualizerProxy;
	iObservers.Close();
    }

TInt CEqualizerEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidAudioEqualizerEffect) );
    return status;
    }

// From MControl begins
TInt CEqualizerEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CEqualizerEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CEqualizerEffect::Type()
    {
    return KEqualizerEffectControl;
    }

TControlType CEqualizerEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CEqualizerEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CEqualizerEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iEqualizerProxy)
		{
		TRAP(error,iEqualizerProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MEqualizerEffect begins
/**
* Get the band level in mB for the specified band
* @since 5.0
* @param aBand Frequency Band
* @return Returns the band level in mB for the specified band
*/
TInt CEqualizerEffect::BandLevel( TInt& aBand, TInt& aBandLevel )
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		// status here returns the Band Level	
		//TUint8 band(aBand);
		//TInt32 bandlevel;
		aBandLevel = iEqualizerProxy->BandLevel(aBand);	
		//aBandLevel = bandlevel;
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Get the band width in Hz for the specified band.
* @since 5.0
* @param aBand Frequency Band
* @return The band width in Hz for the specified band
*/
TInt CEqualizerEffect::BandWidth( TInt& aBand, TInt& aBandWidth )
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		TRAP(aBandWidth,iEqualizerProxy->BandWidth(aBand));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Get the center frequency in Hz for a given band
* @since 5.0
* @param aBand Frequency Band
* @return The center frequency in Hz for a given band.
*/
TInt CEqualizerEffect::CenterFrequency( TInt& aBand, TInt& aCenterFrequency ) 
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aCenterFrequency = iEqualizerProxy->CenterFrequency(aBand);	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Get the cross-over frequency between the given frequency band (aBand) and the next band
* @since 5.0
* @param aBand Frequency Band
* @return Crossover frequency.
*/
TInt CEqualizerEffect::CrossoverFrequency( TInt& aBand, TInt& aCrossoverFreq ) 
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aCrossoverFreq = iEqualizerProxy->CenterFrequency(aBand);	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Get the dB range in mB for the equalizer
* @since 5.0
* @param aMin Minimum level in dB
* @param aMin Maximum level in dB
*/
TInt CEqualizerEffect::DbLevelLimits( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		TInt32 min,max;	
		TRAP(status,iEqualizerProxy->DbLevelLimits(min,max));	
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
* Get number of equalizer bands.
* @since 5.0
* @param -
* @return The number of equalizer bands.
*/
TInt CEqualizerEffect::NumberOfBands(TInt& aNumberofBands) 
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aNumberofBands= iEqualizerProxy->NumberOfBands();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

/**
* Sets the equalizer band level value in mB, ranging from Min to Max
* @since 5.0
* @param aBand Frequency Band
* @param aLevel band level in dB, ranges from DbLevelLimits()
* @return -
*/
TInt CEqualizerEffect::SetBandLevel( TInt& aBand, TInt& aLevel )
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		TRAP(status,iEqualizerProxy->SetBandLevelL(aBand,aLevel));	
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
TInt CEqualizerEffect::Disable()
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		TRAP(status,iEqualizerProxy->DisableL());	
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
TInt CEqualizerEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CEqualizerEffect::Enable() [%x]"),iEqualizerProxy);
	if(iEqualizerProxy)
		{
		TRAP(status,iEqualizerProxy->EnableL());	
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
TInt CEqualizerEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		TRAP(status,iEqualizerProxy->EnforceL(aEnforced));	
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
TInt CEqualizerEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aHaveUpdateRights = iEqualizerProxy->HaveUpdateRights();	
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
TInt CEqualizerEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aEnabled = iEqualizerProxy->IsEnabled();	
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
TInt CEqualizerEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aEnforced = iEqualizerProxy->IsEnforced();	
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
TInt CEqualizerEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iEqualizerProxy)
		{
		aUid = iEqualizerProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CEqualizerEffect::Event( TEffectControlEvent aEvent )
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

TInt CEqualizerEffect::CreateEffectProxy()
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

	TRAP(status,iEqualizerProxy = CAudioEqualizerProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iEqualizerProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CEqualizerEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iEqualizerProxy)
		{
		iPrevEqualizerProxy = iEqualizerProxy;	
		}
	iEqualizerProxy = NULL;	
	return status;
	}

TInt CEqualizerEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevEqualizerProxy)
		{
		TInt bands = iPrevEqualizerProxy->NumberOfBands();
		for(TInt i = 1; i <= bands; i++)
			{	
			TRAP(status , iEqualizerProxy->SetBandLevelL(i,iPrevEqualizerProxy->BandLevel(i)));	
			}

		TBool enforce = iPrevEqualizerProxy->IsEnforced();
		iEqualizerProxy->EnforceL(enforce);	

		TBool enable = iPrevEqualizerProxy->IsEnabled();
		if(enable)
			{
			iEqualizerProxy->EnableL();	
			}

		delete iPrevEqualizerProxy;
		iPrevEqualizerProxy = NULL;
		}				
	return status;	
	}

 void CEqualizerEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
