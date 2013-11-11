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


#include "ListenerOrientationEffectImpl.h"
#include <ListenerOrientationBase.h>
#include <ControlObserver.h>

using namespace multimedia;

CListenerOrientationEffect::CListenerOrientationEffect()
    {
    // No Impl
    }

CListenerOrientationEffect::~CListenerOrientationEffect()
    {
	delete iPrevListenerOrientationProxy;
	delete  iListenerOrientationProxy;
	iObservers.Close();
    }

TInt CListenerOrientationEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidListenerOrientationEffect) );
    return status;
    }

// From MControl begins
TInt CListenerOrientationEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CListenerOrientationEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CListenerOrientationEffect::Type()
    {
    return KListenerOrientationEffectControl;
    }

TControlType CListenerOrientationEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CListenerOrientationEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CListenerOrientationEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iListenerOrientationProxy)
		{
		TRAP(error,iListenerOrientationProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MListenerOrientationEffect begins
/**
* Get the orientation vectors for the position
* @since 5.0
* @param aHeading The heading (thousandths of radians)
* @param aPitch The Pitch (thousandths of radians)
* @param aRoll The Roll (thousandths of radians)
* @return -
*/
TInt CListenerOrientationEffect::Orientation( TInt& aHeading, TInt& aPitch, TInt& aRoll )
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TInt32 heading,pitch,roll;
		iListenerOrientationProxy->Orientation(heading,pitch,roll);	
		aHeading = heading;
		aPitch = pitch;
		aRoll = roll;	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Gets the orientation of the position.
* @since 5.0
* @param aFrontX X value of Front vector
* @param aFrontY Y value of Front vector
* @param aFrontZ Z value of Front vector
* @param aAboveX X value of Above vector
* @param aAboveY Y value of Above vector
* @param aAboveZ Z value of Above vector
* @return -
*/
TInt CListenerOrientationEffect::OrientationVectors( TInt& aFrontX, TInt& aFrontY, TInt& aFrontZ,
							TInt& aAboveX, TInt& aAboveY, TInt& aAboveZ )
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TInt32 frontx;
		TInt32 fronty;
		TInt32 frontz;
		TInt32 abovex;
		TInt32 abovey;
		TInt32 abovez;

		iListenerOrientationProxy->OrientationVectors(frontx,fronty,frontz,abovex,abovey,abovez);	
		aFrontX = frontx;
		aFrontY = fronty;
		aFrontZ = frontz;	
		aAboveX = abovex;;
		aAboveY = abovey;;
		aAboveZ = abovez;;	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}

/**
* Sets the Heading, Pitch, Roll values for the orientation of the source
* @since 5.0
* @param aHeading The heading (thousandths of radians)
* @param aPitch The Pitch (thousandths of radians)
* @param aRoll The Roll (thousandths of radians)
* @return -
*/
TInt CListenerOrientationEffect::SetOrientation( TInt aHeading, TInt aPitch, TInt aRoll )
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TInt32 heading = aHeading;
		TInt32 pitch = aPitch;
		TInt32 roll = aRoll;	
		TRAP(status,iListenerOrientationProxy->SetOrientationL(heading,pitch,roll));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}
/**
* Sets the Front and Above vectors for the orientation of the position.
* @since 5.0
* @param aFrontX X value of Front vector
* @param aFrontY Y value of Front vector
* @param aFrontZ Z value of Front vector
* @param aAboveX X value of Above vector
* @param aAboveY Y value of Above vector
* @param aAboveZ Z value of Above vector
* @return -
*/
TInt CListenerOrientationEffect::SetOrientationVectors( TInt aFrontX, TInt aFrontY, TInt aFrontZ,
								TInt aAboveX, TInt aAboveY, TInt aAboveZ )
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TInt32 frontx = aFrontX;
		TInt32 fronty = aFrontY;
		TInt32 frontz = aFrontZ;
		TInt32 abovex = aAboveX;
		TInt32 abovey = aAboveY;
		TInt32 abovez = aAboveZ;
		TRAP(status,iListenerOrientationProxy->SetOrientationVectorsL(frontx,fronty,frontz,abovex,abovey,abovez));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;	
	}

// From MListenerOrientationEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CListenerOrientationEffect::Disable()
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TRAP(status,iListenerOrientationProxy->DisableL());	
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
TInt CListenerOrientationEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CListenerOrientationEffect::Enable() [%x]"),iListenerOrientationProxy);
	if(iListenerOrientationProxy)
		{
		TRAP(status,iListenerOrientationProxy->EnableL());	
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
TInt CListenerOrientationEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		TRAP(status,iListenerOrientationProxy->EnforceL(aEnforced));	
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
TInt CListenerOrientationEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		aHaveUpdateRights = iListenerOrientationProxy->HaveUpdateRights();	
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
TInt CListenerOrientationEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		aEnabled = iListenerOrientationProxy->IsEnabled();	
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
TInt CListenerOrientationEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		aEnforced = iListenerOrientationProxy->IsEnforced();	
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
TInt CListenerOrientationEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iListenerOrientationProxy)
		{
		aUid = iListenerOrientationProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CListenerOrientationEffect::Event( TEffectControlEvent aEvent )
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

TInt CListenerOrientationEffect::CreateEffectProxy()
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

	TRAP(status,iListenerOrientationProxy = CListenerOrientationProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));	
	if(status == KErrNone)
		{
		TRAP(status,iListenerOrientationProxy->RegisterObserverL(*this));	
		}		

	return status;
	}

TInt CListenerOrientationEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iListenerOrientationProxy)
		{
		iPrevListenerOrientationProxy = iListenerOrientationProxy;	
		}
	iListenerOrientationProxy = NULL;	
	return status;
	}

TInt CListenerOrientationEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevListenerOrientationProxy)
		{
		TBool enforce = iPrevListenerOrientationProxy->IsEnforced();
		iListenerOrientationProxy->EnforceL(enforce);	

		TBool enable = iPrevListenerOrientationProxy->IsEnabled();
		if(enable)
			{
			iListenerOrientationProxy->EnableL();	
			}

		delete iPrevListenerOrientationProxy;
		iPrevListenerOrientationProxy = NULL;
		}				
	return status;	
	}

 void CListenerOrientationEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
