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
*                Encapsulates message real address and alias.
*
*/




// INCLUDE FILES
#include "MessageAddressImpl.h"
#include <msvstore.h>
#include <sendui.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMessageDataImpl::CMessageAddressImpl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CMessageAddressImpl::CMessageAddressImpl()
    {
    }

// Destructor
CMessageAddressImpl::~CMessageAddressImpl()
    {
    }


// -----------------------------------------------------------------------------
// CMessageAddressImpl::SetAddressL
// Sets real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::SetAddressL( const TDesC& aAddress )
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::SetAliasL
// Sets alias for real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::SetAliasL( const TDesC& aAlias )
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::ExternalizeL( RWriteStream& aStream ) const
    {
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::InternalizeL
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::InternalizeL( RReadStream& aStream )
    {
    }

//  End of File
