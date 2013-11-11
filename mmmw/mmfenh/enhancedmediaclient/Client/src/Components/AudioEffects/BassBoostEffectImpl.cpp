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


#include "BassBoostEffectImpl.h"
#include <BassBoostBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CBassBoostEffect::CBassBoostEffect()
    {
    // No Impl
    }

CBassBoostEffect::~CBassBoostEffect()
    {
	delete iPrevBassBoostProxy;
	delete  iBassBoostProxy;
	iObservers.Close();
    }

TInt CBassBoostEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidBassBoostEffect) );
    return status;
    }

// From MControl begins
TInt CBassBoostEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CBassBoostEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CBassBoostEffect::Type()
    {
    return KBassBoostEffectControl;
    }

TControlType CBassBoostEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CBassBoostEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CBassBoostEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iBassBoostProxy)
		{
		TRAP(error,iBassBoostProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MBassBoostEffect begins
// From MBassBoostEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CBassBoostEffect::Disable()
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		TRAP(status,iBassBoostProxy->DisableL());	
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
TInt CBassBoostEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CBassBoostEffect::Enable() [%x]"),iBassBoostProxy);
	if(iBassBoostProxy)
		{
		TRAP(status,iBassBoostProxy->EnableL());	
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
TInt CBassBoostEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		TRAP(status,iBassBoostProxy->EnforceL(aEnforced));	
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
TInt CBassBoostEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		aHaveUpdateRights = iBassBoostProxy->HaveUpdateRights();	
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
TInt CBassBoostEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		aEnabled = iBassBoostProxy->IsEnabled();	
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
TInt CBassBoostEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		aEnforced = iBassBoostProxy->IsEnforced();	
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
TInt CBassBoostEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iBassBoostProxy)
		{
		aUid = iBassBoostProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CBassBoostEffect::Event( TEffectControlEvent aEvent )
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

TInt CBassBoostEffect::CreateEffectProxy()
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

	TRAP(status,iBassBoostProxy = CBassBoostProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iBassBoostProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CBassBoostEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iBassBoostProxy)
		{
		iPrevBassBoostProxy = iBassBoostProxy;	
		}
	iBassBoostProxy = NULL;	
	return status;
	}

TInt CBassBoostEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevBassBoostProxy)
		{
		TBool enforce = iPrevBassBoostProxy->IsEnforced();
		iBassBoostProxy->EnforceL(enforce);	

		TBool enable = iPrevBassBoostProxy->IsEnabled();
		if(enable)
			{
			iBassBoostProxy->EnableL();	
			}

		delete iPrevBassBoostProxy;
		iPrevBassBoostProxy = NULL;
		}				
	return status;	
	}

 void CBassBoostEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
