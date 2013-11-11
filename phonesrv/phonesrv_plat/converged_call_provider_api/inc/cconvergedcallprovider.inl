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
* Description:  ECOM specific functionality for Converged Call Provider 
*
*/



#ifndef CCONVERGEDCALLPROVIDER_INL
#define CCONVERGEDCALLPROVIDER_INL

// INCLUDE FILES
#include <ecom/ecom.h>
#include <cconvergedcallprovider.h>


/**
CCP API Concerged Call Provider UID
*/
const TUid KConvergedCallProviderUid = { 0x10282588 };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConvergedCallProvider::CConvergedCallProvider
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
inline CConvergedCallProvider::CConvergedCallProvider()
    {
    }

// -----------------------------------------------------------------------------
// CConvergedCallProvider::~CConvergedCallProvider()
// Destructor.
// -----------------------------------------------------------------------------
//
inline CConvergedCallProvider::~CConvergedCallProvider()
    {
    // Inform the ECOM framework that this specific instance of the
    // interface has been destroyed.
    REComSession::DestroyedImplementation( iDtor_ID_Key );
    }

// -----------------------------------------------------------------------------
// CConvergedCallProvider::NewL.  
// Interface's static factory method implementation.
// Asks ECOM plugin framework to instantiate plugin implementation.
// Leaves if the implementation cannot be found
// -----------------------------------------------------------------------------
//
inline CConvergedCallProvider* CConvergedCallProvider::NewL( const TUid& aImplementationUid )
    {
     // Set up the interface find for the default resolver.
    return REINTERPRET_CAST( CConvergedCallProvider*,
                REComSession::CreateImplementationL( aImplementationUid,
                _FOFF(CConvergedCallProvider,iDtor_ID_Key )) );
    }

// -----------------------------------------------------------------------------
// CCCPInterface::ListImplementationsL
// Lists all implementations for this interface
// -----------------------------------------------------------------------------
//
inline void CConvergedCallProvider::ListImplementationsL( RImplInfoPtrArray& aImplArray )
    {
    REComSession::ListImplementationsL( KConvergedCallProviderUid, aImplArray );
    }

// -----------------------------------------------------------------------------
// CCCPInterface::InitializeL
// Initializes plugin 
// -----------------------------------------------------------------------------
//
inline void CConvergedCallProvider::InitializeL( const TUint32 /*aServiceId*/,
                              const MCCPObserver& aObserver, 
                              const MCCPSsObserver& aSsObserver )
    {
    InitializeL( aObserver, aSsObserver );
    }

#endif      // CCONVERGEDCALLPROVIDER_INL

// End of File

