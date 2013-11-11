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


#include "SourceDopplerEffectImpl.h"
#include <SourceDopplerBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CSourceDopplerEffect::CSourceDopplerEffect()
    {
    // No Impl
    }

CSourceDopplerEffect::~CSourceDopplerEffect()
    {
	delete iPrevSourceDopplerProxy;
	delete  iSourceDopplerProxy;
	iObservers.Close();
    }

TInt CSourceDopplerEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidSourceDopplerEffect) );
    return status;
    }

// From MControl begins
TInt CSourceDopplerEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CSourceDopplerEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CSourceDopplerEffect::Type()
    {
    return KSourceDopplerEffectControl;
    }

TControlType CSourceDopplerEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CSourceDopplerEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CSourceDopplerEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iSourceDopplerProxy)
		{
		TRAP(error,iSourceDopplerProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MSourceDopplerEffect begins

/**
* Get the velocity's cartesian settings
* @since 3.0
* @param aX Velocity in X direction (mm/s)
* @param aY Velocity in Y direction (mm/s)
* @param aZ Velocity in Z direction (mm/s)
*/
TInt CSourceDopplerEffect::CartesianVelocity( TInt& aX, TInt& aY, TInt& aZ )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TInt32 a,b,c;
		iSourceDopplerProxy->CartesianVelocity(a,b,c);	
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
TInt CSourceDopplerEffect::Factor(TUint& aFactor)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aFactor = iSourceDopplerProxy->Factor();	
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
TInt CSourceDopplerEffect::FactorMax(TUint& aFactorMax)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aFactorMax = iSourceDopplerProxy->FactorMax();	
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
TInt CSourceDopplerEffect::SetCartesianVelocity( TInt aX, TInt aY, TInt aZ )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TInt32 a,b,c;	
		a = aX;
		b = aY;
		c = aZ;
		TRAP(status,iSourceDopplerProxy->SetCartesianVelocityL(a,b,c));	
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
TInt CSourceDopplerEffect::SetFactor( TUint aFactor )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TRAP(status,iSourceDopplerProxy->SetFactorL(aFactor));	
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
TInt CSourceDopplerEffect::SetSphericalVelocity( TInt aAzimuth, TInt aElevation, TInt aRadius )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TInt32 a,b,c;	
		a = aAzimuth;
		b = aElevation;
		c = aRadius;
		TRAP(status,iSourceDopplerProxy->SetSphericalVelocityL(a,b,c));	
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
TInt CSourceDopplerEffect::SphericalVelocity( TInt& aAzimuth, TInt& aElevation, TInt& aRadius )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TInt32 a,b,c;
		iSourceDopplerProxy->SphericalVelocity(a,b,c);	
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

// From MSourceDopplerEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CSourceDopplerEffect::Disable()
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TRAP(status,iSourceDopplerProxy->DisableL());	
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
TInt CSourceDopplerEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CSourceDopplerEffect::Enable() [%x]"),iSourceDopplerProxy);
	if(iSourceDopplerProxy)
		{
		TRAP(status,iSourceDopplerProxy->EnableL());	
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
TInt CSourceDopplerEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		TRAP(status,iSourceDopplerProxy->EnforceL(aEnforced));	
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
TInt CSourceDopplerEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aHaveUpdateRights = iSourceDopplerProxy->HaveUpdateRights();	
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
TInt CSourceDopplerEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aEnabled = iSourceDopplerProxy->IsEnabled();	
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
TInt CSourceDopplerEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aEnforced = iSourceDopplerProxy->IsEnforced();	
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
TInt CSourceDopplerEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iSourceDopplerProxy)
		{
		aUid = iSourceDopplerProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CSourceDopplerEffect::Event( TEffectControlEvent aEvent )
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

TInt CSourceDopplerEffect::CreateEffectProxy()
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

	TRAP(status,iSourceDopplerProxy = CSourceDopplerProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iSourceDopplerProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CSourceDopplerEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iSourceDopplerProxy)
		{
		iPrevSourceDopplerProxy = iSourceDopplerProxy;	
		}
	iSourceDopplerProxy = NULL;	
	return status;
	}

TInt CSourceDopplerEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevSourceDopplerProxy)
		{
		TBool enforce = iPrevSourceDopplerProxy->IsEnforced();
		iSourceDopplerProxy->EnforceL(enforce);	

		TBool enable = iPrevSourceDopplerProxy->IsEnabled();
		if(enable)
			{
			iSourceDopplerProxy->EnableL();	
			}

		delete iPrevSourceDopplerProxy;
		iPrevSourceDopplerProxy = NULL;
		}				
	return status;	
	}

 void CSourceDopplerEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
