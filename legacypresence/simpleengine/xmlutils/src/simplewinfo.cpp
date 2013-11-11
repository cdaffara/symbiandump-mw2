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
#include <s32strm.h>
#include <SenBaseElement.h>
#include <SenBaseAttribute.h>
#include <SenXmlUtils.h>
#include <SenXmlReader.h>
#include <SenDomFragment.h>

// own simple
#include "simplecommon.h"
#include "simpleelement.h"
#include "simplenamespace.h"
#include "simpleattribute.h"
#include "simplebasedocument.h"
#include "simpleutils.h"
#include "simplewinfo.h"


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleWinfo::CSimpleWinfo
// ----------------------------------------------------------
//
CSimpleWinfo::CSimpleWinfo( )
: CSimpleBaseDocument()
    {
    }

// ----------------------------------------------------------
// CSimpleWinfo::~CSimpleWinfo
// ----------------------------------------------------------
//
CSimpleWinfo::~CSimpleWinfo()
    {
    }

// ----------------------------------------------------------
// CSimpleWinfo::ConstructL
// ----------------------------------------------------------
//
void CSimpleWinfo::ConstructL(
    const TDesC8& aNsUri,
  const TDesC8& aLocalName )
    {
    BaseConstructL(aNsUri, aLocalName);
    }

// ----------------------------------------------------------
// CSimpleWinfo::ConstructL
// ----------------------------------------------------------
//
void CSimpleWinfo::ConstructL(
  const TDesC8& aXml )
    {
    BaseConstructL( aXml );
    }

// ----------------------------------------------------------
// CSimpleWinfo::NewL
// ----------------------------------------------------------
//
CSimpleWinfo* CSimpleWinfo::NewL( )
    {
    CSimpleWinfo* self = new (ELeave) CSimpleWinfo( );
    CleanupStack::PushL( self );
    self->ConstructL( KSimpleNsWinfo, KWinfoLocalName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleWinfo::NewL
// ----------------------------------------------------------
//
CSimpleWinfo* CSimpleWinfo::NewL( const TDesC8& aXml )
    {
    CSimpleWinfo* self = new (ELeave) CSimpleWinfo( );
    CleanupStack::PushL( self );
    self->ConstructL( aXml );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleWinfo::ValidateXmlL
// ----------------------------------------------------------
//
void CSimpleWinfo::ValidateXmlL( const TDesC8& aName )
    {
    if ( aName.CompareF( KWinfoLocalName ) )
        {
        User::Leave( KErrCorrupt );
        }
    }

// ----------------------------------------------------------
// CSimpleWinfo::DefaultNamespace
// ----------------------------------------------------------
//
TPtrC8 CSimpleWinfo::DefaultNamespace()
    {
    return CSimpleBaseDocument::DefaultNamespace();
    }


// ----------------------------------------------------------
// CSimpleWinfo::AddNamespaceL
// ----------------------------------------------------------
//
void CSimpleWinfo::AddNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    CSimpleBaseDocument::AddNamespaceL( aPrefix, aUri );
    }

// ----------------------------------------------------------
// CSimpleWinfo::NamespacesL
// ----------------------------------------------------------
//
RPointerArray<MSimpleNamespace>& CSimpleWinfo::NamespacesL()
    {
    return CSimpleBaseDocument::NamespacesL();
    }

// ----------------------------------------------------------
// CSimpleWinfo::ExternalizeL
// ----------------------------------------------------------
//
void CSimpleWinfo::ExternalizeL( RWriteStream& aStream )
    {
    CSimpleBaseDocument::ExternalizeL( aStream );
    }

// ----------------------------------------------------------
// CSimpleWinfo::Close
// ----------------------------------------------------------
//
void CSimpleWinfo::Close()
    {
    CSimpleBaseDocument::Close();
    }

// ----------------------------------------------------------
// CSimpleWinfo::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleWinfo::LocalName()
    {
    return CSimpleBaseDocument::LocalName();
    }

// ----------------------------------------------------------
// CSimpleWinfo::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleWinfo::DefNamespaceL()
    {
    return CSimpleBaseDocument::DefNamespaceL();
    }

// ----------------------------------------------------------
// CSimpleWinfo::HasContent
// ----------------------------------------------------------
//
TBool CSimpleWinfo::HasContent()
    {
    return CSimpleBaseDocument::HasContent();
    }

// ----------------------------------------------------------
// CSimpleWinfo::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleWinfo::ContentUnicodeL()
    {
    return CSimpleBaseDocument::ContentUnicodeL();
    }

// ----------------------------------------------------------
// CSimpleWinfo::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleWinfo::SetContentUnicodeL( const TDesC& aContent )
    {
    CSimpleBaseDocument::SetContentUnicodeL( aContent );
    }

// ----------------------------------------------------------
// CSimpleWinfo::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleWinfo::SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray )
    {
    return CSimpleBaseDocument::SimpleElementsL( aElementArray );
    }

// ----------------------------------------------------------
// CSimpleWinfo::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleWinfo::AttrValueLC( const TDesC8& aName )
    {
    return CSimpleBaseDocument::AttrValueLC( aName );
    }

// ----------------------------------------------------------
// CSimpleWinfo::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleWinfo::AttrValue( const TDesC8& aName )
    {
  return CSimpleBaseDocument::AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleWinfo::AddAttrL
// ----------------------------------------------------------
//
void CSimpleWinfo::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    CSimpleBaseDocument::AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleWinfo::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleWinfo::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {
    return CSimpleBaseDocument::SimpleAttributesL( aArray );
    }

// ----------------------------------------------------------
// CSimpleWinfo::SimpleParentL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleWinfo::SimpleParentL()
    {
    return CSimpleBaseDocument::SimpleParentL( );
    }

// ----------------------------------------------------------
// CSimpleWinfo::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleWinfo::DetachSimpleL()
    {
    User::Leave( KErrNotFound );
    }


// ----------------------------------------------------------
// CSimpleWinfo::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleWinfo::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleWinfo::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleWinfo::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aLocalName );
    }


// ----------------------------------------------------------
// CSimpleWinfo::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleWinfo::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSimpleBaseDocument::RemoveSimpleElement( aNsUri, aLocalName );
    }




