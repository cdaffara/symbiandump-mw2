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


#include "ListenerDopplerEffectImpl.h"
#include <ListenerDopplerBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CListenerDopplerEffect::CListenerDopplerEffect()
    {
    // No Impl
    }

CListenerDopplerEffect::~CListenerDopplerEffect()
    {
	delete iPrevListenerDopplerProxy;
	delete  iListenerDopplerProxy;
	iObservers.Close();
    }

TInt CListenerDopplerEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidListenerDopplerEffect) );
    return status;
    }

// From MControl begins
TInt CListenerDopplerEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CListenerDopplerEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CListenerDopplerEffect::Type()
    {
    return KListenerDopplerEffectControl;
    }

TControlType CListenerDopplerEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CListenerDopplerEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CListenerDopplerEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iListenerDopplerProxy)
		{
		TRAP(error,iListenerDopplerProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MListenerDopplerEffect begins

/**
* Get the velocity's cartesian settings
* @since 3.0
* @param aX Velocity in X direction (mm/s)
* @param aY Velocity in Y direction (mm/s)
* @param aZ Velocity in Z direction (mm/s)
*/
TInt CListenerDopplerEffect::CartesianVelocity( TInt& aX, TInt& aY, TInt& aZ )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TInt32 a,b,c;
		iListenerDopplerProxy->CartesianVelocity(a,b,c);	
		aX = a;
		aY = b;
		aZ = c;	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}


/**
* Get the current multiplier factor
* @since 3.0
* @return multiplier factor
*/
TInt CListenerDopplerEffect::Factor(TUint& aFactor)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aFactor = iListenerDopplerProxy->Factor();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Get the maximum multiplier factor
* @since 3.0
* @return multiplier factor
*/
TInt CListenerDopplerEffect::FactorMax(TUint& aFactorMax)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aFactorMax = iListenerDopplerProxy->FactorMax();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
	
/**
* Sets the velocity in Cartesian coordinates of the sound source with respect to the listener.
* @since 3.0
* @param aX Velocity in X direction (mm/s)
* @param aY Velocity in Y direction (mm/s)
* @param aZ Velocity in Z direction (mm/s)
* @return -
*/
TInt CListenerDopplerEffect::SetCartesianVelocity( TInt aX, TInt aY, TInt aZ )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TInt32 a,b,c;	
		a = aX;
		b = aY;
		c = aZ;
		TRAP(status,iListenerDopplerProxy->SetCartesianVelocityL(a,b,c));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

/**
* Sets the multiplier factor.
* @since 3.0
* @param aFactor Factor value in hundredths that ranges from 0 to FactorMax(),
*                where 100 corresponds to 1.00, 200 corresponds to 2.00, etc.
* @return -
*/
TInt CListenerDopplerEffect::SetFactor( TUint aFactor )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TRAP(status,iListenerDopplerProxy->SetFactorL(aFactor));	
		}	
	else
		{
		status = KErrNotReady;	
		}	
	return status;			
	}
	
/**
* Sets the velocity in spherical coordinates of the sound source with respect to the listener.
* @since 3.0
* @param aAzimuth the Azimuth (thousandths of radians)
* @param aElevation the elevation (thousandths of radians)
* @param aRadius the radius (thousandths of radians)
* @return -
*/
TInt CListenerDopplerEffect::SetSphericalVelocity( TInt aAzimuth, TInt aElevation, TInt aRadius )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TInt32 a,b,c;	
		a = aAzimuth;
		b = aElevation;
		c = aRadius;
		TRAP(status,iListenerDopplerProxy->SetSphericalVelocityL(a,b,c));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

/**
* Gets the velocity's spherical coordinates settings.
* @since 3.0
* @param aAzimuth the Azimuth (thousandths of radians)
* @param aElevation the elevation (thousandths of radians)
* @param aRadius the radius (thousandths of radians)
* @return -
*/
TInt CListenerDopplerEffect::SphericalVelocity( TInt& aAzimuth, TInt& aElevation, TInt& aRadius )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TInt32 a,b,c;
		iListenerDopplerProxy->SphericalVelocity(a,b,c);	
		aAzimuth = a;
		aElevation = b;
		aRadius = c;	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

// From MListenerDopplerEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CListenerDopplerEffect::Disable()
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TRAP(status,iListenerDopplerProxy->DisableL());	
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
TInt CListenerDopplerEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CListenerDopplerEffect::Enable() [%x]"),iListenerDopplerProxy);
	if(iListenerDopplerProxy)
		{
		TRAP(status,iListenerDopplerProxy->EnableL());	
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
TInt CListenerDopplerEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		TRAP(status,iListenerDopplerProxy->EnforceL(aEnforced));	
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
TInt CListenerDopplerEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aHaveUpdateRights = iListenerDopplerProxy->HaveUpdateRights();	
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
TInt CListenerDopplerEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aEnabled = iListenerDopplerProxy->IsEnabled();	
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
TInt CListenerDopplerEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aEnforced = iListenerDopplerProxy->IsEnforced();	
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
TInt CListenerDopplerEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iListenerDopplerProxy)
		{
		aUid = iListenerDopplerProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CListenerDopplerEffect::Event( TEffectControlEvent aEvent )
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

TInt CListenerDopplerEffect::CreateEffectProxy()
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

	TRAP(status,iListenerDopplerProxy = CListenerDopplerProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iListenerDopplerProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CListenerDopplerEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iListenerDopplerProxy)
		{
		iPrevListenerDopplerProxy = iListenerDopplerProxy;	
		}
	iListenerDopplerProxy = NULL;	
	return status;
	}

TInt CListenerDopplerEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevListenerDopplerProxy)
		{
		TBool enforce = iPrevListenerDopplerProxy->IsEnforced();
		iListenerDopplerProxy->EnforceL(enforce);	

		TBool enable = iPrevListenerDopplerProxy->IsEnabled();
		if(enable)
			{
			iListenerDopplerProxy->EnableL();	
			}

		delete iPrevListenerDopplerProxy;
		iPrevListenerDopplerProxy = NULL;
		}				
	return status;	
	}

 void CListenerDopplerEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
