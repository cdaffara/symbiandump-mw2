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
* Description:    Simple Engine
*
*/




// INCLUDE FILES

#include <e32std.h>

#include <SenNameSpace.h>
#include "simplenamespace.h"



// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleNamespace::CSimpleNamespace
// ----------------------------------------------------------
//
CSimpleNamespace::CSimpleNamespace(  )
: iPrefix( NULL ), iUri(NULL)
    {
    }

// ----------------------------------------------------------
// CSimpleNamespace::~CSimpleNamespace
// ----------------------------------------------------------
//
CSimpleNamespace::~CSimpleNamespace()
    {
    delete iPrefix;
    delete iUri;
    }

// ----------------------------------------------------------
// CSimpleNamespace::NewL
// ----------------------------------------------------------
//
CSimpleNamespace* CSimpleNamespace::NewL(
    const TDesC8& aPrefix,
  const TDesC8& aUri)
    {
    CSimpleNamespace* self = new (ELeave) CSimpleNamespace( );
    CleanupStack::PushL( self );
    self->ConstructL( aPrefix, aUri );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleNamespace::ConstructL
// ----------------------------------------------------------
//
void CSimpleNamespace::ConstructL(
    const TDesC8& aPrefix,
  const TDesC8& aUri)
    {
    iUri = aUri.AllocL();
    iPrefix = aPrefix.AllocL();
    }

// ----------------------------------------------------------
// CSimpleNamespace::Close
// ----------------------------------------------------------
//
void CSimpleNamespace::Close()
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleNamespace::URI
// ----------------------------------------------------------
//
TPtrC8 CSimpleNamespace::URI()
    {
    if ( iUri )
        {
        return TPtrC8(*iUri);
        }
    else
        {
        return TPtrC8();
        }
    }

// ----------------------------------------------------------
// CSimpleNamespace::Prefix
// ----------------------------------------------------------
//
TPtrC8 CSimpleNamespace::Prefix()
    {
    if ( iPrefix )
        {
        return TPtrC8(*iPrefix);
        }
    else
        {
        return TPtrC8();
        }
    }

// ----------------------------------------------------------
// CSimpleNamespace::SetUriL
// ----------------------------------------------------------
//
void CSimpleNamespace::SetUriL(const TDesC8& aUri)
    {
    delete iUri;
    iUri = NULL;
    iUri = aUri.AllocL();
    }

// ----------------------------------------------------------
// CSimpleNamespace::SetPrefixL
// ----------------------------------------------------------
//
void CSimpleNamespace::SetPrefixL(const TDesC8& aPrefix)
    {
    delete iPrefix;
    iPrefix = NULL;
    iPrefix = aPrefix.AllocL();
    }

