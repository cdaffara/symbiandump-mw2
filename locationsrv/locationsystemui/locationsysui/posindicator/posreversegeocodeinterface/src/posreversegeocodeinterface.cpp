/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of Geo coding interface class.
*
*/

#include "posreversegeocodeinterface.h"
#include "posreversegeocodeinterface.hrh"

#include <ecom.h>

// static cleanup function
static void RImpleInfoPtrArrayCleanup( TAny* aArray )
    {
    static_cast<RImplInfoPtrArray*>( aArray )->ResetAndDestroy();
    }


//------------------------------------------------------------------------------
// CPosReverseGeocodeInterface::NewL
//------------------------------------------------------------------------------
EXPORT_C CPosReverseGeocodeInterface* CPosReverseGeocodeInterface::NewL( 
                                            MPosReverseGeocodeObserver& aObserver )
    {
    RImplInfoPtrArray implInfoPtrArray;
    TCleanupItem arrayCleanup( RImpleInfoPtrArrayCleanup, &implInfoPtrArray );
    CleanupStack::PushL(arrayCleanup);
    
    TUid implementationUID;
    
    REComSession::ListImplementationsL( TUid::Uid(KPosReverseGeoCodeInterfaceId),
                                        implInfoPtrArray );
    
    if( implInfoPtrArray.Count() == 0 )
        {
        implInfoPtrArray.ResetAndDestroy();
        User::Leave( KErrNotFound );
        }

    const CImplementationInformation* pluginInformation = implInfoPtrArray[0];
    implementationUID = pluginInformation->ImplementationUid();
                                            
    TAny* ptr = REComSession::CreateImplementationL( implementationUID,
                                                     _FOFF(CPosReverseGeocodeInterface,iDtor_ID_Key),
                                                     reinterpret_cast<TAny*>( &aObserver ));
    REComSession::FinalClose();
    CleanupStack::PopAndDestroy(); // arrayCleanup
    return reinterpret_cast<CPosReverseGeocodeInterface*>(ptr);
    }

//----------------------------------------------------------------------------------
// CPosReverseGeocodeInterface::~CPosReverseGeocodeInterface
//----------------------------------------------------------------------------------
EXPORT_C CPosReverseGeocodeInterface::~CPosReverseGeocodeInterface()
    {
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }
