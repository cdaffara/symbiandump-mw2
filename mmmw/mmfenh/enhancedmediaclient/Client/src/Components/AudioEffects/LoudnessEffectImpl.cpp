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


#include "LoudnessEffectImpl.h"
#include <LoudnessBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CLoudnessEffect::CLoudnessEffect()
    {
    // No Impl
    }

CLoudnessEffect::~CLoudnessEffect()
    {
	delete iPrevLoudnessProxy;
	delete  iLoudnessProxy;
	iObservers.Close();
    }

TInt CLoudnessEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidLoudnessEffect) );
    return status;
    }

// From MControl begins
TInt CLoudnessEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CLoudnessEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CLoudnessEffect::Type()
    {
    return KLoudnessEffectControl;
    }

TControlType CLoudnessEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CLoudnessEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CLoudnessEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iLoudnessProxy)
		{
		TRAP(error,iLoudnessProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MLoudnessEffect begins
// From MLoudnessEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CLoudnessEffect::Disable()
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		TRAP(status,iLoudnessProxy->DisableL());	
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
TInt CLoudnessEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CLoudnessEffect::Enable() [%x]"),iLoudnessProxy);
	if(iLoudnessProxy)
		{
		TRAP(status,iLoudnessProxy->EnableL());	
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
TInt CLoudnessEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		TRAP(status,iLoudnessProxy->EnforceL(aEnforced));	
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
TInt CLoudnessEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		aHaveUpdateRights = iLoudnessProxy->HaveUpdateRights();	
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
TInt CLoudnessEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		aEnabled = iLoudnessProxy->IsEnabled();	
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
TInt CLoudnessEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		aEnforced = iLoudnessProxy->IsEnforced();	
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
TInt CLoudnessEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iLoudnessProxy)
		{
		aUid = iLoudnessProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CLoudnessEffect::Event( TEffectControlEvent aEvent )
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

TInt CLoudnessEffect::CreateEffectProxy()
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

	TRAP(status,iLoudnessProxy = CLoudnessProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iLoudnessProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CLoudnessEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iLoudnessProxy)
		{
		iPrevLoudnessProxy = iLoudnessProxy;	
		}
	iLoudnessProxy = NULL;	
	return status;
	}

TInt CLoudnessEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevLoudnessProxy)
		{
		TBool enforce = iPrevLoudnessProxy->IsEnforced();
		iLoudnessProxy->EnforceL(enforce);	

		TBool enable = iPrevLoudnessProxy->IsEnabled();
		if(enable)
			{
			iLoudnessProxy->EnableL();	
			}

		delete iPrevLoudnessProxy;
		iPrevLoudnessProxy = NULL;
		}				
	return status;	
	}

 void CLoudnessEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
