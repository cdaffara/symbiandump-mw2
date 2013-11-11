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
    delete iRealAddress;
    delete iAlias;
    }


// -----------------------------------------------------------------------------
// CMessageAddressImpl::SetAddressL
// Sets real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::SetAddressL( const TDesC& aAddress )
    {
    delete iRealAddress;
    iRealAddress = NULL;
    iRealAddress = aAddress.AllocL();
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::SetAliasL
// Sets alias for real address.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::SetAliasL( const TDesC& aAlias )
    {
    delete iAlias;
    iAlias = NULL;
    iAlias = aAlias.AllocL();
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::ExternalizeL
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::ExternalizeL( RWriteStream& aStream ) const
    {
    TInt length(0);
    
    if (iAlias) 
        {
        length = iAlias->Length();
        }
    
    aStream.WriteUint32L( length );
    
    if ( length )
        {
        aStream << iAlias->Des();
        }
    
    if (iRealAddress) 
        {
        length = iRealAddress->Length();
        }
    else
        {
        length = 0;
        }
    
    aStream.WriteUint32L( length );
    
    if ( length )
        {
        aStream << iRealAddress->Des();
        }
    }

// -----------------------------------------------------------------------------
// CMessageAddressImpl::InternalizeL
// -----------------------------------------------------------------------------
//
void CMessageAddressImpl::InternalizeL( RReadStream& aStream )
    {
    if ( aStream.ReadUint32L( ) )
        {
        iAlias = NULL;
        iAlias = HBufC::NewL(aStream, KMaxLength); 
        }
        
    if ( aStream.ReadUint32L( ) )
        {
        iRealAddress = NULL;
        iRealAddress = HBufC::NewL( aStream, KMaxLength );
        }
    }

//  End of File
