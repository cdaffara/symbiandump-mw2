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


#include "BalanceEffectImpl.h"
// For KUidInterfaceMMFAudioPlayDevice
#include <mmf/common/mmfstandardcustomcommands.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <mmf/common/mmfstandardcustomcommandsenums.h>
#endif
// For TMMFAudioConfig
#include <mmf/common/mmfaudio.h>
// For TMMFMessageDestinationPckg
#include <mmf/common/mmfcontrollerframework.h>

using namespace multimedia;

CBalanceEffect::CBalanceEffect()
: iBalance(KMMFBalanceCenter)
	{
    // No Impl	
	}

CBalanceEffect::~CBalanceEffect()
	{
    // No Impl	
	}

TInt CBalanceEffect::PostConstructor()
    {
    TRAPD( status, CEffectControlBase::ConstructL( KUidInterfaceMMFAudioPlayDevice ) );
    return status;
    }

// From MControl begins
TInt CBalanceEffect::AddObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;
    }

TInt CBalanceEffect::RemoveObserver( MControlObserver& /*aObserver*/ )
    {
    return KErrNotSupported;
    }

TUid CBalanceEffect::Type()
    {
    return KBalanceEffectControl;
    }

TControlType CBalanceEffect::ControlType()
    {
	return EEffectControl;
    }
// From MControl ends

// From MEffectControl begins
TInt CBalanceEffect::Apply()
    {
        iEnabled = true;
        return DoApply();
    }

// From MEffectControl ends

// From MBalanceEffect begins

TInt CBalanceEffect::DoApply()
    {
    TInt error(KErrNone);    
    TMMFMessageDestination msgDest( KUidInterfaceMMFAudioPlayDevice );
    TMMFMessageDestinationPckg msgDestPckg( msgDest );
	TPckgBuf<TMMFAudioConfig> configPackage;
	configPackage().iBalance = iBalance;
	error =  CEffectControlBase::CustomCommandSync( msgDestPckg,
			                                      EMMFAudioPlayDeviceSetBalance,
                                                  configPackage,
                                                  KNullDesC8);
    return error;
    }

// From MBalanceEffectt begins
TInt CBalanceEffect::SetBalance( TInt& aBalance )
    {
    iBalance = aBalance;
    return KErrNone;
    }

TInt CBalanceEffect::GetBalance( TInt& aBalance )
    {
    aBalance = iBalance;
    return KErrNone;
    }

// From CEffectControlerBase begins

void CBalanceEffect::Event ( TEffectControlEvent aEvent)
	{
	TMMFMessageDestination msgDest ( KUidInterfaceMMFAudioPlayDevice );
	TMMFMessageDestinationPckg msgDestPckg ( msgDest );
	TPckgBuf<TMMFAudioConfig> configPackage;
	 // Controller loaded with ECIBuilderCreated
	if ( aEvent == ECIBuilderCreated )
		{
		if ( iBalance == KMMFBalanceCenter)
			{
			TInt error = CEffectControlBase::CustomCommandSync(msgDestPckg,
					                                          EMMFAudioPlayDeviceGetBalance,
					                                          KNullDesC8,
					                                          KNullDesC8,
					                                          configPackage);
			if ( !error )
				{
				iBalance = configPackage().iBalance;				
				}
			}		
		}
	if ( iEnabled && iBalance != KMMFBalanceCenter)
		{		
		DoApply();
		}	
	}

// From CEffectControlBase ends

// End of file
