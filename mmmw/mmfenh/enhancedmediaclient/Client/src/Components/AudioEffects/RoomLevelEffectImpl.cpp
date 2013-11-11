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


#include "RoomLevelEffectImpl.h"
#include <RoomLevelBase.h>
#include <ControlObserver.h>
#include "ReverbEffectImpl.h"

#ifdef _DEBUG
#include <e32debug.h>
#define _U(s) _L(s)
#define RET_ERR_IF_ERR(err) if (err!=KErrNone) { \
    RDebug::Print(_L("RET_ERR_IF_ERR:Line[%d]Err[%d]"), __LINE__, err); \
    RDebug::Print(_U( __FILE__ )); \
    return err; }
#else
#define RET_ERR_IF_ERR(err) if (err!=KErrNone) return err
#endif // _DEBUG

using namespace multimedia;

CRoomLevelEffect::CRoomLevelEffect()
    {
    // No Impl
    }

CRoomLevelEffect::~CRoomLevelEffect()
    {
	delete iPrevRoomLevelProxy;
	delete  iRoomLevelProxy;
	iObservers.Close();
    }

TInt CRoomLevelEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL(KUidRoomLevelEffect) );
    return status;
    }

// From MControl begins
TInt CRoomLevelEffect::AddObserver( MControlObserver& aObserver )
    {
    return iObservers.Append(&aObserver);
    }

TInt CRoomLevelEffect::RemoveObserver( MControlObserver& aObserver )
    {
	TInt index = iObservers.Find(&aObserver);
	if( index != KErrNotFound )
		{
		iObservers.Remove(index);
		}
	return index;
    }

TUid CRoomLevelEffect::Type()
    {
    return KRoomLevelEffectControl;
    }

TControlType CRoomLevelEffect::ControlType()
    {
	return EEffectControl;
    }

// From MControl ends

// From MEffectControl begins
TInt CRoomLevelEffect::Apply()
    {
    return DoApply();
    }

// From MEffectControl ends

TInt CRoomLevelEffect::DoApply()
    {
    TInt error(KErrNone);    
	if(iRoomLevelProxy)
		{
		TRAP(error,iRoomLevelProxy->ApplyL());	
		}	
	else
		{
		error = KErrNotReady;	
		}			
    return error;    
    }

// From MRoomLevelEffect begins
/**
* Gets the RoomLevel current level in mB
* @since 5.0
* @return RoomLevel current level
*/
TInt CRoomLevelEffect::Level(TInt& aLevel)
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		aLevel = iRoomLevelProxy->Level();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;						
	}

/**
* Gets the RoomLevel current level maximum and minimum in mB
* @since 5.0
* @param aMin Minimum current level
* @param aMax Maximum current level
* @return -
*/
TInt CRoomLevelEffect::LevelRange( TInt& aMin, TInt& aMax )
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		TInt32 min,max;	
		iRoomLevelProxy->LevelRange(min,max);
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
* Sets the RoomLevel level, it will leave if aRoomLevel is not within range of Min and Max
* @since 5.0
* @param aRoomLevelLevel The RoomLevel level in mB
*/
TInt CRoomLevelEffect::SetRoomLevel( TInt aRoomLevel )
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		TRAP(status,iRoomLevelProxy->SetRoomLevelL(aRoomLevel));	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;				
	}

// From MRoomLevelEffect ends

// From MAudioEffectControl
/**
* Disable the effect
* @since 5.0
*/
TInt CRoomLevelEffect::Disable()
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		TRAP(status,iRoomLevelProxy->DisableL());	
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
TInt CRoomLevelEffect::Enable()
	{
	TInt status(KErrNone);
	RDebug::Print(_L("CRoomLevelEffect::Enable() [%x]"),iRoomLevelProxy);
	if(iRoomLevelProxy)
		{
		TRAP(status,iRoomLevelProxy->EnableL());	
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
TInt CRoomLevelEffect::Enforce( TBool &aEnforced )
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		TRAP(status,iRoomLevelProxy->EnforceL(aEnforced));	
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
TInt CRoomLevelEffect::HaveUpdateRights(TBool &aHaveUpdateRights)
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		aHaveUpdateRights = iRoomLevelProxy->HaveUpdateRights();	
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
TInt CRoomLevelEffect::IsEnabled(TBool &aEnabled)
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		aEnabled = iRoomLevelProxy->IsEnabled();	
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
TInt CRoomLevelEffect::IsEnforced(TBool &aEnforced)
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		aEnforced = iRoomLevelProxy->IsEnforced();	
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
TInt CRoomLevelEffect::Uid(TUid &aUid)
	{
	TInt status(KErrNone);
	if(iRoomLevelProxy)
		{
		aUid = iRoomLevelProxy->Uid();	
		}	
	else
		{
		status = KErrNotReady;	
		}			
	return status;			
	}
// From MAudioEffectControl Ends


// From CEffectControlBase begins
void CRoomLevelEffect::Event( TEffectControlEvent aEvent )
    {
	TInt status(KErrNone);    	
    // Controller Loaded with ECIBuilderCreated
    if ( aEvent == ECIBuilderCreated )
        {
    	//RDebug::Print(_L("Deleting Proxy"));	
		status = DeleteEffectProxy();
		//RDebug::Print(_L("Deleting Proxy [%d]"),status);	
		status = CreateEffectProxy();	
		//RDebug::Print(_L("Creating Proxy [%d]"),status);	
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

TInt CRoomLevelEffect::AttachReverb( MReverbControl& aReverb )
    {
    TInt status(KErrNone);
    if ( iCReverbEffect )
        {
        status = KErrAlreadyExists;
        }
    RET_ERR_IF_ERR( status );

    if ( aReverb.Type() != KReverbEffectControl )
        {
        status = KErrArgument;
        }
    RET_ERR_IF_ERR( status );

    iCReverbEffect = dynamic_cast<CReverbEffect*>(&aReverb);
    status = KErrNone;
    return status;
    }

TInt CRoomLevelEffect::DetachReverb( MReverbControl& aReverb )
    {
    TInt status(KErrNone);
    if ( aReverb.Type() != KReverbEffectControl )
        {
        status = KErrArgument;
        }
    RET_ERR_IF_ERR( status );

    status = KErrNotFound;
    iCReverbEffect = dynamic_cast<CReverbEffect*>(&aReverb);
    if ( iCReverbEffect == dynamic_cast<CReverbEffect*>(&aReverb) )
        {
        status = KErrNone;
        
        // Make sure CRoomLevelEffect::AttachReverb is called before proxy gets created.
        if ( !iCReverbEffect )
            {
            status = KErrNotReady;  
            }
        RET_ERR_IF_ERR( status );

        // Detach reverb proxy here
        CEnvironmentalReverb* aReverb(NULL);
        status = iCReverbEffect->GetCEnvironmentalReverb(aReverb);
        RET_ERR_IF_ERR( status );

        status = iRoomLevelProxy->DettachReverb(*aReverb);
        RET_ERR_IF_ERR( status );

        iCReverbEffect = NULL;
        }
    return status;
    }

// From CEffectControlBase ends

TInt CRoomLevelEffect::CreateEffectProxy()
	{
	TInt status(KErrNone);
	status = GetMessageHandle(iMsgHndlrHandlePckg);
    RET_ERR_IF_ERR( status );

    iCustomCommand = GetCustomCommand();		
	if(!iCustomCommand)
		{
		status = KErrNotReady;	
		}
    RET_ERR_IF_ERR( status );
    
    // Make sure CRoomLevelEffect::AttachReverb is called before proxy gets created.
    if ( !iCReverbEffect )
        {
        status = KErrNotReady;  
        }
    RET_ERR_IF_ERR( status );

	TRAP(status,iRoomLevelProxy = CRoomLevelProxy::NewL(iMsgHndlrHandlePckg, *iCustomCommand, NULL));
    RET_ERR_IF_ERR( status );

    // Attach reverb proxy here
    CEnvironmentalReverb* aReverb(NULL);
    status = iCReverbEffect->GetCEnvironmentalReverb(aReverb);
    RET_ERR_IF_ERR( status );

    status = iRoomLevelProxy->AttachReverb(*aReverb);
    RET_ERR_IF_ERR( status );
    
    TRAP(status,iRoomLevelProxy->RegisterObserverL(*this));	

	return status;
	}

TInt CRoomLevelEffect::DeleteEffectProxy()
	{
	TInt status(KErrNone);	
	if(iRoomLevelProxy)
		{
		iPrevRoomLevelProxy = iRoomLevelProxy;	
		}
	iRoomLevelProxy = NULL;	
	return status;
	}

TInt CRoomLevelEffect::SavePreviousSettings()
	{
	TInt status(KErrNone);	
	if(iPrevRoomLevelProxy)
		{
		TBool enforce = iPrevRoomLevelProxy->IsEnforced();
		iRoomLevelProxy->EnforceL(enforce);	

		TBool enable = iPrevRoomLevelProxy->IsEnabled();
		if(enable)
			{
			iRoomLevelProxy->EnableL();	
			}

		delete iPrevRoomLevelProxy;
		iPrevRoomLevelProxy = NULL;
		}				
	return status;	
	}

 void CRoomLevelEffect::EffectChanged( const CAudioEffect* /*aObservedEffect*/, TUint8 aEvent )
 	{
	for ( TInt i = 0; i < iObservers.Count(); i++ )
		{
		iObservers[i]->Event(this,aEvent,NULL);
		} 		
 	}

// End of file
