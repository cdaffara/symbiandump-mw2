/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Implementation of CMessageAddress interface.
*                Encapsulates the ECom plugged sending service data.
*
*/




// INCLUDE FILES
#include "SendingServiceInfoImpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::CSendingServiceInfoImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendingServiceInfoImpl::CSendingServiceInfoImpl()
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendingServiceInfoImpl* CSendingServiceInfoImpl::NewL()
    {
    CSendingServiceInfoImpl* self = CSendingServiceInfoImpl::NewLC();
    
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------
// CSendingServiceInfoImpl::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
CSendingServiceInfoImpl* CSendingServiceInfoImpl::NewLC()
    {
    CSendingServiceInfoImpl* self = new (ELeave) CSendingServiceInfoImpl;
    CleanupStack::PushL( self );
    return self;
    }


// Destructor
CSendingServiceInfoImpl::~CSendingServiceInfoImpl()
    {
    delete iServiceName;
    delete iServiceMenuName;
    delete iServiceAddress;
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::CopyL
// Creates a copy of CSendingServiceInfoImpl object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CSendingServiceInfoImpl* CSendingServiceInfoImpl::CopyL() const
    {
    CSendingServiceInfoImpl* serviceInfoImpl = CSendingServiceInfoImpl::NewL();
    CleanupStack::PushL( serviceInfoImpl );

    if ( iServiceName )
        {
        serviceInfoImpl->SetServiceNameL( *iServiceName );
        }

    if ( iServiceMenuName )
        {
        serviceInfoImpl->SetServiceMenuNameL( *iServiceMenuName );
        }

    if ( iServiceAddress )
        {
        serviceInfoImpl->SetServiceAddressL( *iServiceAddress );        
        }

    serviceInfoImpl->iServiceId = iServiceId;
    serviceInfoImpl->iServiceProviderId = iServiceProviderId;
    serviceInfoImpl->iServiceCapabilities = iServiceCapabilities;
    serviceInfoImpl->iTechnologyTypeId = iTechnologyTypeId;
    serviceInfoImpl->iServiceFeatures = iServiceFeatures;

    CleanupStack::Pop( serviceInfoImpl );

    return serviceInfoImpl;
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::SetServiceNameL
// Sets service human readable name.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSendingServiceInfoImpl::SetServiceNameL(
    const TDesC& aServiceName )
    {
    delete iServiceName;
    iServiceName = NULL;
    iServiceName = aServiceName.AllocL();
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::SetServiceMenuNameL
// Sets service human readable name for the "Send"-menu.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSendingServiceInfoImpl::SetServiceMenuNameL(
    const TDesC& aServiceMenuName )
    {
    delete iServiceMenuName;
    iServiceMenuName = NULL;
    iServiceMenuName = aServiceMenuName.AllocL();
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfoImpl::SetServiceAddressL
// Sets service address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSendingServiceInfoImpl::SetServiceAddressL(
    const TDesC& aServiceAddress )
    {
    delete iServiceAddress;
    iServiceAddress = NULL;
    iServiceAddress = aServiceAddress.AllocL();
    }


//  End of File
