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
* Description:   Encapsulates the ECom plugged sending service data.
*
*/




// INCLUDE FILES
#include <CSendingServiceInfo.h>
#include "SendingServiceInfoImpl.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendingServiceInfo::CSendingServiceInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendingServiceInfo::CSendingServiceInfo()
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendingServiceInfo::ConstructL()
    {
     }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingServiceInfo* CSendingServiceInfo::NewL()
    {
    }

// ---------------------------------------------------------
// CSendingServiceInfo::NewLC
// Two-phased constructor.
// ---------------------------------------------------------
//
EXPORT_C CSendingServiceInfo* CSendingServiceInfo::NewLC()
    {
    }


// Destructor
EXPORT_C CSendingServiceInfo::~CSendingServiceInfo()
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::CopyLC
// Creates a copy of CSendingServiceInfo object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CSendingServiceInfo* CSendingServiceInfo::CopyLC() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceNameL
// Sets service human readable name.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceNameL( const TDesC& aServiceName )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceName
// Returns service human readable name.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CSendingServiceInfo::ServiceName() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceMenuNameL
// Sets service human readable name for the "Send"-menu.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceMenuNameL(
    const TDesC& aServiceMenuName )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceMenuName
// Returns service human readable name for the "Send"-menu.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CSendingServiceInfo::ServiceMenuName() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceAddressL
// Sets service address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceAddressL(
    const TDesC& aServiceAddress )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceAddress
// Returns service address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CSendingServiceInfo::ServiceAddress() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceId
// Sets service id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceId( TUid aServiceId )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceId
// Returns service id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendingServiceInfo::ServiceId() const
    {
    }


// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceId
// Returns service id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetTechnologyTypeId( TUid aTechnologyType )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceId
// Returns service id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendingServiceInfo::TechnologyTypeId() const
    {
    }


// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceProviderId
// Sets service provider id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceProviderId( TUid aServiceProviderId )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceProviderId
// Returns service provider id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TUid CSendingServiceInfo::ServiceProviderId() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceCapabilities
// Set service sending capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceCapabilities(
    TSendingCapabilities aServiceCapabilities )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceCapabilities
// Returns service sending capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TSendingCapabilities CSendingServiceInfo::ServiceCapabilities() const
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::SetServiceCapabilities
// Set service sending capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CSendingServiceInfo::SetServiceFeatures(
    TInt aServiceFeatures )
    {
    }

// -----------------------------------------------------------------------------
// CSendingServiceInfo::ServiceCapabilities
// Returns service sending capabilities.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSendingServiceInfo::ServiceFeatures() const
    {
    }
//  End of File
