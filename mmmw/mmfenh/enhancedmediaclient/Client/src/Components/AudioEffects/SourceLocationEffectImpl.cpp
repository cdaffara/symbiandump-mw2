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


#include "SourceLocationEffectImpl.h"
#include <SourceLocationBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CSourceLocationEffect::CSourceLocationEffect()
    {
    // No Impl
    }

CSourceLocationEffect::~CSourceLocationEffect()
    {
	delete iPrevSourceLocationProxy;
	delete  iSourceLocationProxy;
	iObservers.Close();
    }

TInt CSourceLocationEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidSourceLocationEffect) );
    return status;
    }

// From MControl begins
TInt CSourceLocationEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CSourceLocationEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CSourceLocationEffect::Type()
    {
    return KSourceLocationEffectControl;
    }

TControlType CSourceLocationEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CSourceLocationEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CSourceLocationEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iSourceLocationProxy)
		{
		TRAP(error,iSourceLocationProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MSourceLocationEffect begins
/**
* Gets the cartesian coordinates for the location of the position.
* @since 5.0
* @param aX The x-coordinate of the position (in millimeters)
* @param aY The y-coordinate of the position (in millimeters)
* @param aZ The z-coordinate of the position (in millimeters)
* @return -
*/

TInt CSourceLocationEffect::LocationCartesian( TInt& aX, TInt& aY, TInt& aZ )
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TInt32 a,b,c;
		iSourceLocationProxy->LocationCartesian(a,b,c);	
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
* Gets the spherical coordinates for the location of the position.
* @since 5.0
* @param aAzimuth The Azimuth of the position (thousandths of radians)
* @param aElevation The elevation of the position (thousandths of radians)
* @param aRadius The radius of the position (thousandths of radians)
* @return -
*/
TInt CSourceLocationEffect::LocationSpherical( TInt& aAzimuth, TInt& aElevation, TInt& aRadius )
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TInt32 a,b,c;
		iSourceLocationProxy->LocationSpherical(a,b,c);	
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

/**
* Sets the cartesian coordinates for the location of the position.
* @since 5.0
* @param aX The x-coordinate of the position
* @param aY The y-coordinate of the position
* @param aZ The z-coordinate of the position
* @return -
*/
TInt CSourceLocationEffect::SetLocationCartesian( TInt aX, TInt aY, TInt aZ )
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TInt32 a,b,c;	
		a = aX;
		b = aY;
		c = aZ;
		TRAP(status,iSourceLocationProxy->SetLocationCartesianL(a,b,c));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

/**
* Sets the spherical coordinates for the location of the position.
* @since 5.0
* @param aAzimuth The Azimuth of the position (thousandths of radians)
* @param aElevation The elevation of the position (thousandths of radians)
* @param aRadius The radius of the position (thousandths of radians)
* @return -
*/
TInt CSourceLocationEffect::SetLocationSpherical( TInt aAzimuth, TInt aElevation, TInt aRadius )
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TInt32 a,b,c;	
		a = aAzimuth;
		b = aElevation;
		c = aRadius;
		TRAP(status,iSourceLocationProxy->SetLocationSphericalL(a,b,c));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

// From MSourceLocationEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CSourceLocationEffect::Disable()
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TRAP(status,iSourceLocationProxy->DisableL());	
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
TInt CSourceLocationEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CSourceLocationEffect::Enable() [%x]"),iSourceLocationProxy);
	if(iSourceLocationProxy)
		{
		TRAP(status,iSourceLocationProxy->EnableL());	
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
TInt CSourceLocationEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		TRAP(status,iSourceLocationProxy->EnforceL(aEnforced));	
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
TInt CSourceLocationEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		aHaveUpdateRights = iSourceLocationProxy->HaveUpdateRights();	
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
TInt CSourceLocationEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		aEnabled = iSourceLocationProxy->IsEnabled();	
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
TInt CSourceLocationEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		aEnforced = iSourceLocationProxy->IsEnforced();	
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
TInt CSourceLocationEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iSourceLocationProxy)
		{
		aUid = iSourceLocationProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CSourceLocationEffect::Event( TEffectControlEvent aEvent )
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

TInt CSourceLocationEffect::CreateEffectProxy()
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

	TRAP(status,iSourceLocationProxy = CSourceLocationProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iSourceLocationProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CSourceLocationEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iSourceLocationProxy)
		{
		iPrevSourceLocationProxy = iSourceLocationProxy;	
		}
	iSourceLocationProxy = NULL;	
	return status;
	}

TInt CSourceLocationEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevSourceLocationProxy)
		{
		TBool enforce = iPrevSourceLocationProxy->IsEnforced();
		iSourceLocationProxy->EnforceL(enforce);	

		TBool enable = iPrevSourceLocationProxy->IsEnabled();
		if(enable)
			{
			iSourceLocationProxy->EnableL();	
			}

		delete iPrevSourceLocationProxy;
		iPrevSourceLocationProxy = NULL;
		}				
	return status;	
	}

 void CSourceLocationEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
