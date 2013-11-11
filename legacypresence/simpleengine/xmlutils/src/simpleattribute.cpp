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
#include <SenBaseAttribute.h>
#include <SenXmlUtils.h>
#include "simpleattribute.h"



// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleAttribute::CSimpleAttribute
// ----------------------------------------------------------
//
CSimpleAttribute::CSimpleAttribute(  )
: iBase( NULL )
    {
    }

// ----------------------------------------------------------
// CSimpleAttribute::~CSimpleAttribute
// ----------------------------------------------------------
//
CSimpleAttribute::~CSimpleAttribute()
    {
    delete iBase;
    }

// ----------------------------------------------------------
// CSimpleAttribute::NewL
// ----------------------------------------------------------
//
CSimpleAttribute* CSimpleAttribute::NewL(
    const TDesC8& aName,
    const TDesC8& aValue )
    {
    CSimpleAttribute* self = new (ELeave) CSimpleAttribute( );
    CleanupStack::PushL( self );
    self->ConstructL( aName, aValue );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleElement::NewL
// ----------------------------------------------------------
//
CSimpleAttribute* CSimpleAttribute::NewL(
    CSenBaseAttribute* aAttr )
    {
    CSimpleAttribute* self = new (ELeave) CSimpleAttribute( );
    CleanupStack::PushL( self );
    self->ConstructL( aAttr );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleAttribute::ConstructL
// ----------------------------------------------------------
//
void CSimpleAttribute::ConstructL(
    const TDesC8& aName,
    const TDesC8& aValue )
    {
    iBase = CSenBaseAttribute::NewL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleAttribute::ConstructL
// ----------------------------------------------------------
//
void CSimpleAttribute::ConstructL(
    CSenBaseAttribute* aAttr )
    {
    iRef = aAttr;
    }

// ----------------------------------------------------------
// CSimpleElement::BaseElement
// ----------------------------------------------------------
//
CSenBaseAttribute* CSimpleAttribute::BaseElement()
    {
    return iBase ? iBase : iRef;
    }

// ----------------------------------------------------------
// CSimpleAttribute::Name
// ----------------------------------------------------------
//
const TDesC8& CSimpleAttribute::Name()
    {
    return BaseElement()->Name();
    }

// ----------------------------------------------------------
// CSimpleAttribute::Value
// ----------------------------------------------------------
//
const TDesC8& CSimpleAttribute::Value()
    {
    return BaseElement()->Value();
    }

// ----------------------------------------------------------
// CSimpleAttribute::ValueUniLC
// ----------------------------------------------------------
//
HBufC* CSimpleAttribute::ValueUniLC()
    {
    // UTF-8 -> Unicode conversion
    // new buffer is created, ownership is transferred
    HBufC16* b16 = SenXmlUtils::ToUnicodeLC( BaseElement()->Value( ) );
    return b16;
    }

// ----------------------------------------------------------
// CSimpleAttribute::SetValueL
// ----------------------------------------------------------
//
void CSimpleAttribute::SetValueL( const TDesC& aValue )
    {
    HBufC8* b8 = SenXmlUtils::ToUtf8LC( aValue);
    BaseElement()->SetValueL( b8->Des() );
    CleanupStack::PopAndDestroy( b8 );
    }


