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


#include "ListenerLocationEffectImpl.h"
#include <ListenerLocationBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CListenerLocationEffect::CListenerLocationEffect()
    {
    // No Impl
    }

CListenerLocationEffect::~CListenerLocationEffect()
    {
	delete iPrevListenerLocationProxy;
	delete  iListenerLocationProxy;
	iObservers.Close();
    }

TInt CListenerLocationEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidListenerLocationEffect) );
    return status;
    }

// From MControl begins
TInt CListenerLocationEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CListenerLocationEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CListenerLocationEffect::Type()
    {
    return KListenerLocationEffectControl;
    }

TControlType CListenerLocationEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CListenerLocationEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CListenerLocationEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iListenerLocationProxy)
		{
		TRAP(error,iListenerLocationProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MListenerLocationEffect begins
/**
* Gets the cartesian coordinates for the location of the position.
* @since 5.0
* @param aX The x-coordinate of the position (in millimeters)
* @param aY The y-coordinate of the position (in millimeters)
* @param aZ The z-coordinate of the position (in millimeters)
* @return -
*/

TInt CListenerLocationEffect::LocationCartesian( TInt& aX, TInt& aY, TInt& aZ )
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TInt32 a,b,c;
		iListenerLocationProxy->LocationCartesian(a,b,c);	
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
TInt CListenerLocationEffect::LocationSpherical( TInt& aAzimuth, TInt& aElevation, TInt& aRadius )
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TInt32 a,b,c;
		iListenerLocationProxy->LocationSpherical(a,b,c);	
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
TInt CListenerLocationEffect::SetLocationCartesian( TInt aX, TInt aY, TInt aZ )
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TInt32 a,b,c;	
		a = aX;
		b = aY;
		c = aZ;
		TRAP(status,iListenerLocationProxy->SetLocationCartesianL(a,b,c));	
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
TInt CListenerLocationEffect::SetLocationSpherical( TInt aAzimuth, TInt aElevation, TInt aRadius )
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TInt32 a,b,c;	
		a = aAzimuth;
		b = aElevation;
		c = aRadius;
		TRAP(status,iListenerLocationProxy->SetLocationSphericalL(a,b,c));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

// From MListenerLocationEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CListenerLocationEffect::Disable()
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TRAP(status,iListenerLocationProxy->DisableL());	
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
TInt CListenerLocationEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CListenerLocationEffect::Enable() [%x]"),iListenerLocationProxy);
	if(iListenerLocationProxy)
		{
		TRAP(status,iListenerLocationProxy->EnableL());	
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
TInt CListenerLocationEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		TRAP(status,iListenerLocationProxy->EnforceL(aEnforced));	
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
TInt CListenerLocationEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		aHaveUpdateRights = iListenerLocationProxy->HaveUpdateRights();	
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
TInt CListenerLocationEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		aEnabled = iListenerLocationProxy->IsEnabled();	
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
TInt CListenerLocationEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		aEnforced = iListenerLocationProxy->IsEnforced();	
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
TInt CListenerLocationEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iListenerLocationProxy)
		{
		aUid = iListenerLocationProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CListenerLocationEffect::Event( TEffectControlEvent aEvent )
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

TInt CListenerLocationEffect::CreateEffectProxy()
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

	TRAP(status,iListenerLocationProxy = CListenerLocationProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iListenerLocationProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CListenerLocationEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iListenerLocationProxy)
		{
		iPrevListenerLocationProxy = iListenerLocationProxy;	
		}
	iListenerLocationProxy = NULL;	
	return status;
	}

TInt CListenerLocationEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevListenerLocationProxy)
		{
		TBool enforce = iPrevListenerLocationProxy->IsEnforced();
		iListenerLocationProxy->EnforceL(enforce);	

		TBool enable = iPrevListenerLocationProxy->IsEnabled();
		if(enable)
			{
			iListenerLocationProxy->EnableL();	
			}

		delete iPrevListenerLocationProxy;
		iPrevListenerLocationProxy = NULL;
		}				
	return status;	
	}

 void CListenerLocationEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
