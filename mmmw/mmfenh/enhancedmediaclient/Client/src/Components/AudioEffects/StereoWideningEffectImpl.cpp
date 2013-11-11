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


#include "StereoWideningEffectImpl.h"
#include <StereoWideningBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CStereoWideningEffect::CStereoWideningEffect()
    {
    // No Impl
    }

CStereoWideningEffect::~CStereoWideningEffect()
    {
	delete iPrevStereoWideningProxy;
	delete  iStereoWideningProxy;
	iObservers.Close();
    }

TInt CStereoWideningEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidStereoWideningEffect) );
    return status;
    }

// From MControl begins
TInt CStereoWideningEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CStereoWideningEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CStereoWideningEffect::Type()
    {
    return KStereoWideningEffectControl;
    }

TControlType CStereoWideningEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CStereoWideningEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CStereoWideningEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iStereoWideningProxy)
		{
		TRAP(error,iStereoWideningProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MStereoWideningEffect begins
/**
* Check if continuous level is supported for stereo widening.
* @since 5.0
* @param  ETrue if continuous level is supported for stereo widening.  If this returns
*         EFalse, then the adaptation is actually setting the level to the nearest supported discrete
*         value, when SetStereoWidenlingLevelL is called.
* @return Error Code
*/
TInt CStereoWideningEffect::IsContinuousLevelSupported(TBool& aSupported)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aSupported = iStereoWideningProxy->IsContinuousLevelSupported();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Set stereo widening level.
* @since 5.0
* @param aLevel Stereo widening level ranges from 0-100, where 0 is None and 100 is Maximum widening.
*/
TInt CStereoWideningEffect::SetStereoWideningLevel( TInt aLevel )
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		TRAP(status,iStereoWideningProxy->SetStereoWideningLevelL(aLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Get the stereo widening settings.
* @since 5.0
* @param aLevel Gets the Current Stereo widening level
* @return Error Code
*/
TInt CStereoWideningEffect::StereoWideningLevel(TInt& aLevel)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aLevel = iStereoWideningProxy->StereoWideningLevel();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

// From MStereoWideningEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CStereoWideningEffect::Disable()
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		TRAP(status,iStereoWideningProxy->DisableL());	
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
TInt CStereoWideningEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CStereoWideningEffect::Enable() [%x]"),iStereoWideningProxy);
	if(iStereoWideningProxy)
		{
		TRAP(status,iStereoWideningProxy->EnableL());	
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
TInt CStereoWideningEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		TRAP(status,iStereoWideningProxy->EnforceL(aEnforced));	
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
TInt CStereoWideningEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aHaveUpdateRights = iStereoWideningProxy->HaveUpdateRights();	
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
TInt CStereoWideningEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aEnabled = iStereoWideningProxy->IsEnabled();	
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
TInt CStereoWideningEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aEnforced = iStereoWideningProxy->IsEnforced();	
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
TInt CStereoWideningEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iStereoWideningProxy)
		{
		aUid = iStereoWideningProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CStereoWideningEffect::Event( TEffectControlEvent aEvent )
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

TInt CStereoWideningEffect::CreateEffectProxy()
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

	TRAP(status,iStereoWideningProxy = CStereoWideningProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iStereoWideningProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CStereoWideningEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iStereoWideningProxy)
		{
		iPrevStereoWideningProxy = iStereoWideningProxy;	
		}
	iStereoWideningProxy = NULL;	
	return status;
	}

TInt CStereoWideningEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevStereoWideningProxy)
		{
		TBool enforce = iPrevStereoWideningProxy->IsEnforced();
		iStereoWideningProxy->EnforceL(enforce);	

		TBool enable = iPrevStereoWideningProxy->IsEnabled();
		if(enable)
			{
			iStereoWideningProxy->EnableL();	
			}

		delete iPrevStereoWideningProxy;
		iPrevStereoWideningProxy = NULL;
		}				
	return status;	
	}

 void CStereoWideningEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
