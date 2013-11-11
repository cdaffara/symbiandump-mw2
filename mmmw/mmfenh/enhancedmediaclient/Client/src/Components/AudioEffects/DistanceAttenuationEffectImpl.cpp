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


#include "DistanceAttenuationEffectImpl.h"
#include <DistanceAttenuationBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CDistanceAttenuationEffect::CDistanceAttenuationEffect()
    {
    // No Impl
    }

CDistanceAttenuationEffect::~CDistanceAttenuationEffect()
    {
	delete iPrevDistanceAttenuationProxy;
	delete  iDistanceAttenuationProxy;
	iObservers.Close();
    }

TInt CDistanceAttenuationEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidDistanceAttenuationEffect) );
    return status;
    }

// From MControl begins
TInt CDistanceAttenuationEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CDistanceAttenuationEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CDistanceAttenuationEffect::Type()
    {
    return KDistanceAttenuationEffectControl;
    }

TControlType CDistanceAttenuationEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CDistanceAttenuationEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CDistanceAttenuationEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iDistanceAttenuationProxy)
		{
		TRAP(error,iDistanceAttenuationProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MDistanceAttenuationEffect begins

/**
* Gets the Distance Attenuation of the listener.
* @since 3.0
* @param aRMin The source-to-listener distance below which the sound level is constant
* @param aRMax The source-to-listener distance above which the sound level is constant or zero
* @param aMuteAfterMax Indicate if the sound is muted beyond RMax
* @param aRollOffFactor Multiplier factor to source-to-listener distance
* @param aRoomRollOffFactor Multiplier factor to source-to-listener distance for Room effect.
* @return -
*/
TInt CDistanceAttenuationEffect::DistanceAttenuation( TInt& aRMin, TInt& aRMax, TBool& aMuteAfterMax,
                                   TUint& aRollOffFactor, TUint& aRoomRollOffFactor )
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TInt32 a,b;
		TBool c;
		TUint32 d,e;
		iDistanceAttenuationProxy->DistanceAttenuation(a,b,c,d,e);	
		aRMin = a;
		aRMax = b;
		aMuteAfterMax = c;	
		aRollOffFactor = d;
		aRoomRollOffFactor = e;
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
                                   

/**
* Get roll off factor maximum value.
* @since 3.0
* @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
* @return -
*/
TInt CDistanceAttenuationEffect::RollOffFactorMax( TUint& aRollOfFactorMax )
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TUint32 factormax;
		iDistanceAttenuationProxy->RollOffFactorMax(factormax);	
		aRollOfFactorMax = factormax;
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Get room roll off factor maximum value.
* @since 3.0
* @param aRollOffFactor The maximum multiplier factor to source-to-listener distance for Room effect.
* @return -
*/
TInt CDistanceAttenuationEffect::RoomRollOffFactorMax( TUint& aRoomRollOfFactorMax )
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TUint32 factormax;
		iDistanceAttenuationProxy->RoomRollOffFactorMax(factormax);	
		aRoomRollOfFactorMax = factormax;
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Sets the Distance Attenuation
* @since 3.0
* @param aRMin The source-to-listener distance below which the sound level is constant
* @param aRMax The source-to-listener distance above which the sound level is constant or zero
* @param aMuteAfterMax Indicate if the sound is muted beyond RMax
* @param aRollOffFactor Multiplier factor to source-to-listener distance
* @param aRoomRollOffFactor Multiplier factor to source-to-listener distance for Room effect.
* @return -
*/
TInt CDistanceAttenuationEffect::SetDistanceAttenuation( TInt aRMin, TInt aRMax, TBool aMuteAfterMax,
                                       TUint aRollOffFactor, TUint aRoomRollOffFactor )
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TInt32 a,b;
		TBool c;	
		TUint32 d,e;
		a = aRMin;
		b = aRMax;
		c = aMuteAfterMax;
		d = aRollOffFactor;
		e = aRoomRollOffFactor;
		
		TRAP(status,iDistanceAttenuationProxy->SetDistanceAttenuationL(a,b,c,d,e));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

// From MDistanceAttenuationEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CDistanceAttenuationEffect::Disable()
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TRAP(status,iDistanceAttenuationProxy->DisableL());	
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
TInt CDistanceAttenuationEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CDistanceAttenuationEffect::Enable() [%x]"),iDistanceAttenuationProxy);
	if(iDistanceAttenuationProxy)
		{
		TRAP(status,iDistanceAttenuationProxy->EnableL());	
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
TInt CDistanceAttenuationEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		TRAP(status,iDistanceAttenuationProxy->EnforceL(aEnforced));	
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
* @param ETrue if this object currently has rights to update the settings of this effect,
*         EFalse otherwise.
*/
TInt CDistanceAttenuationEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		aHaveUpdateRights = iDistanceAttenuationProxy->HaveUpdateRights();	
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
* @param ETrue if the effect is enabled, EFalse if the effect is disabled.
*/
TInt CDistanceAttenuationEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		aEnabled = iDistanceAttenuationProxy->IsEnabled();	
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
* @param ETrue if the effect is enforced, EFalse if the effect isn ot enforced.
*/
TInt CDistanceAttenuationEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		aEnforced = iDistanceAttenuationProxy->IsEnforced();	
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
* @param Unique identifier of the audio effect object.
*/
TInt CDistanceAttenuationEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iDistanceAttenuationProxy)
		{
		aUid = iDistanceAttenuationProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CDistanceAttenuationEffect::Event( TEffectControlEvent aEvent )
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

TInt CDistanceAttenuationEffect::CreateEffectProxy()
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

	TRAP(status,iDistanceAttenuationProxy = CDistanceAttenuationProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iDistanceAttenuationProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CDistanceAttenuationEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iDistanceAttenuationProxy)
		{
		iPrevDistanceAttenuationProxy = iDistanceAttenuationProxy;	
		}
	iDistanceAttenuationProxy = NULL;	
	return status;
	}

TInt CDistanceAttenuationEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevDistanceAttenuationProxy)
		{
		TBool enforce = iPrevDistanceAttenuationProxy->IsEnforced();
		iDistanceAttenuationProxy->EnforceL(enforce);	

		TBool enable = iPrevDistanceAttenuationProxy->IsEnabled();
		if(enable)
			{
			iDistanceAttenuationProxy->EnableL();	
			}

		delete iPrevDistanceAttenuationProxy;
		iPrevDistanceAttenuationProxy = NULL;
		}				
	return status;	
	}

 void CDistanceAttenuationEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
