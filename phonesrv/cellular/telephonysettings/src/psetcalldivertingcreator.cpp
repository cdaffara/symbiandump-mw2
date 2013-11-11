/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPSetCallDivertingCreator class.
*
*/



// INCLUDE FILES
#include    "psetcalldivertingbasicimpl.h" 
#include    "psetcalldivertingdualaffectimpl.h" 
#include    "psetcalldivertingcreator.h" 
#include    "psetvariationproxy.h" 

#include    <psetcalldiverting.h> 
#include    <centralrepository.h>
#include    <telservicesinternalcrkeys.h> // telephony service local variation keys. 
#include    <telservicesvariant.hrh>


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// CreateCallDivertingL
// ---------------------------------------------------------------------------
//
MCallDiverting* CPSetCallDivertingCreator::CreateCallDivertingL(
        MPsetDivertObserver& aObserver, 
        RMobilePhone& aPhone,
        CPsetCallDiverting* aDivert  )
    {
    if( IsDivertDualAffectFeatureEnabledL() )
        {
        return CPSetCallDivertingDualAffectImpl::NewL( aObserver, aPhone, aDivert );
        }
    else
        {
        return CPSetCallDivertingBasicImpl::NewL( aObserver, aPhone, aDivert );
        }
    }

// ---------------------------------------------------------------------------
// IsDivertDualAffectFeatureEnabledL
// ---------------------------------------------------------------------------
//
TBool CPSetCallDivertingCreator::IsDivertDualAffectFeatureEnabledL()
    {
    TBool retValue(EFalse);
    CPSetVariationProxy* variationProxy = 
        CPSetVariationProxy::NewL( KCRUidTelSrvVariation, KTelSrvVariationFlags );
    CleanupStack::PushL( variationProxy );
    retValue = variationProxy->IsFeatureEnabled( KTelephonyLVFlagDivertDualAffect );
    CleanupStack::PopAndDestroy( variationProxy );
    return retValue; 
    }

// End of File
