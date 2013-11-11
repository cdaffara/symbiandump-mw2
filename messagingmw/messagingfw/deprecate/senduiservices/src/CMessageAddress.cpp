/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Encapsulates message real address and alias.
*
*/



// INCLUDE FILES
#include <CMessageAddress.h>
#include "MessageAddressImpl.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMessageAddress::CMessageAddress
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMessageAddress::CMessageAddress()
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMessageAddress::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMessageAddress* CMessageAddress::NewL()
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CMessageAddress* CMessageAddress::NewLC()
    {
    }


// Destructor
EXPORT_C CMessageAddress::~CMessageAddress()
    {
    }


// -----------------------------------------------------------------------------
// CMessageAddress::SetAddressL
// Sets real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageAddress::SetAddressL( const TDesC& aAddress )
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::Address
// Returns real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CMessageAddress::Address() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::SetAliasL
// Sets alias for real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageAddress::SetAliasL( const TDesC& aAlias )
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::Alias
// Returns the alias for the real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C const TPtrC CMessageAddress::Alias() const
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::ExternalizeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CMessageAddress::ExternalizeL( RWriteStream& aStream )
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddress::InternalizeL
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMessageAddress::InternalizeL( RReadStream& aStream )
    {
    }

//  End of File
