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
#include "simplemeta.h"
#include "simplebasedocument.h"
#include "simpleutils.h"


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleMeta::CSimpleMeta
// ----------------------------------------------------------
//
CSimpleMeta::CSimpleMeta( )
: CSimpleBaseDocument()
    {
    }

// ----------------------------------------------------------
// CSimpleMeta::~CSimpleMeta
// ----------------------------------------------------------
//
CSimpleMeta::~CSimpleMeta()
    {
    }

// ----------------------------------------------------------
// CSimpleMeta::ConstructL
// ----------------------------------------------------------
//
void CSimpleMeta::ConstructL(
    const TDesC8& aNsUri,
  const TDesC8& aLocalName )
    {
    BaseConstructL(aNsUri, aLocalName);
    }

// ----------------------------------------------------------
// CSimpleMeta::ConstructL
// ----------------------------------------------------------
//
void CSimpleMeta::ConstructL(
  const TDesC8& aXml )
    {
    BaseConstructL( aXml );
    }

// ----------------------------------------------------------
// CSimpleMeta::NewL
// ----------------------------------------------------------
//
CSimpleMeta* CSimpleMeta::NewL( const TDesC8& aXml )
    {
    CSimpleMeta* self = new (ELeave) CSimpleMeta( );
    CleanupStack::PushL( self );
    self->ConstructL( aXml );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleMeta::ValidateXmlL
// ----------------------------------------------------------
//
void CSimpleMeta::ValidateXmlL( const TDesC8& aName )
    {
    if ( aName.CompareF( KRlmiLocalName ) )
        {
        User::Leave( KErrCorrupt );
        }
    }

// ----------------------------------------------------------
// CSimpleMeta::DefaultNamespace
// ----------------------------------------------------------
//
TPtrC8 CSimpleMeta::DefaultNamespace()
    {
    return CSimpleBaseDocument::DefaultNamespace();
    }


// ----------------------------------------------------------
// CSimpleMeta::AddNamespaceL
// ----------------------------------------------------------
//
void CSimpleMeta::AddNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    CSimpleBaseDocument::AddNamespaceL( aPrefix, aUri );
    }

// ----------------------------------------------------------
// CSimpleMeta::NamespacesL
// ----------------------------------------------------------
//
RPointerArray<MSimpleNamespace>& CSimpleMeta::NamespacesL()
    {
    return CSimpleBaseDocument::NamespacesL();
    }

// ----------------------------------------------------------
// CSimpleMeta::ExternalizeL
// ----------------------------------------------------------
//
void CSimpleMeta::ExternalizeL( RWriteStream& aStream )
    {
    CSimpleBaseDocument::ExternalizeL( aStream );
    }

// ----------------------------------------------------------
// CSimpleMeta::Close
// ----------------------------------------------------------
//
void CSimpleMeta::Close()
    {
    CSimpleBaseDocument::Close();
    }

// ----------------------------------------------------------
// CSimpleMeta::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleMeta::LocalName()
    {
    return CSimpleBaseDocument::LocalName();
    }

// ----------------------------------------------------------
// CSimpleMeta::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleMeta::DefNamespaceL()
    {
    return CSimpleBaseDocument::DefNamespaceL();
    }

// ----------------------------------------------------------
// CSimpleMeta::HasContent
// ----------------------------------------------------------
//
TBool CSimpleMeta::HasContent()
    {
    return CSimpleBaseDocument::HasContent();
    }

// ----------------------------------------------------------
// CSimpleMeta::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleMeta::ContentUnicodeL()
    {
    return CSimpleBaseDocument::ContentUnicodeL();
    }

// ----------------------------------------------------------
// CSimpleMeta::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleMeta::SetContentUnicodeL( const TDesC& aContent )
    {
    CSimpleBaseDocument::SetContentUnicodeL( aContent );
    }

// ----------------------------------------------------------
// CSimpleMeta::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleMeta::SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray )
    {
    return CSimpleBaseDocument::SimpleElementsL( aElementArray );
    }

// ----------------------------------------------------------
// CSimpleMeta::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleMeta::AttrValueLC( const TDesC8& aName )
    {
    return CSimpleBaseDocument::AttrValueLC( aName );
    }

// ----------------------------------------------------------
// CSimpleMeta::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleMeta::AttrValue( const TDesC8& aName )
    {
  return CSimpleBaseDocument::AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleMeta::AddAttrL
// ----------------------------------------------------------
//
void CSimpleMeta::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    CSimpleBaseDocument::AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleMeta::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleMeta::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {
    return CSimpleBaseDocument::SimpleAttributesL( aArray );
    }

// ----------------------------------------------------------
// CSimpleMeta::SimpleParentL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleMeta::SimpleParentL()
    {
    return CSimpleBaseDocument::SimpleParentL( );
    }

// ----------------------------------------------------------
// CSimpleMeta::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleMeta::DetachSimpleL()
    {
    User::Leave( KErrNotFound );
    }


// ----------------------------------------------------------
// CSimpleMeta::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleMeta::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleMeta::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleMeta::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aLocalName );
    }


// ----------------------------------------------------------
// CSimpleMeta::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleMeta::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSimpleBaseDocument::RemoveSimpleElement( aNsUri, aLocalName );
    }




