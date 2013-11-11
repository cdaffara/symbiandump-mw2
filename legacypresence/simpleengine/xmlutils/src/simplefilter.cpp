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
#include "simplefilter.h"

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleFilter::CSimpleFilter
// ----------------------------------------------------------
//
CSimpleFilter::CSimpleFilter( )
: CSimpleBaseDocument()
    {
    }

// ----------------------------------------------------------
// CSimpleFilter::~CSimpleFilter
// ----------------------------------------------------------
//
CSimpleFilter::~CSimpleFilter()
    {
    }

// ----------------------------------------------------------
// CSimpleFilter::ConstructL
// ----------------------------------------------------------
//
void CSimpleFilter::ConstructL(
    const TDesC8& aNsUri,
  const TDesC8& aLocalName )
    {
    BaseConstructL(aNsUri, aLocalName);
    }

// ----------------------------------------------------------
// CSimpleFilter::ConstructL
// ----------------------------------------------------------
//
void CSimpleFilter::ConstructL(
  const TDesC8& aXml )
    {       
    BaseConstructL( aXml );
    }

// ----------------------------------------------------------
// CSimpleFilter::NewL
// ----------------------------------------------------------
//
CSimpleFilter* CSimpleFilter::NewL( )
    {
    CSimpleFilter* self = new (ELeave) CSimpleFilter( );
    CleanupStack::PushL( self );
    self->ConstructL( KSimpleNsFilter, KFilterLocalName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleFilter::NewL
// ----------------------------------------------------------
//
CSimpleFilter* CSimpleFilter::NewL( const TDesC8& aXml )
    {
    CSimpleFilter* self = new (ELeave) CSimpleFilter( );
    CleanupStack::PushL( self );
    self->ConstructL( aXml );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleFilter::ValidateXmlL
// ----------------------------------------------------------
//
void CSimpleFilter::ValidateXmlL( const TDesC8& aName )
    {
    if ( aName.CompareF( KFilterLocalName ) )
        {
        User::Leave( KErrCorrupt );
        }        
    }

// ----------------------------------------------------------
// CSimpleFilter::DefaultNamespace
// ----------------------------------------------------------
//
TPtrC8 CSimpleFilter::DefaultNamespace()
    {
    return CSimpleBaseDocument::DefaultNamespace();
    }

// ----------------------------------------------------------
// CSimpleFilter::AddNamespaceL
// ----------------------------------------------------------
//
void CSimpleFilter::AddNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    CSimpleBaseDocument::AddNamespaceL( aPrefix, aUri );
    }

// ----------------------------------------------------------
// CSimpleFilter::NamespacesL
// ----------------------------------------------------------
//
RPointerArray<MSimpleNamespace>& CSimpleFilter::NamespacesL()
    {
    return CSimpleBaseDocument::NamespacesL();
    }

// ----------------------------------------------------------
// CSimpleFilter::ExternalizeL
// ----------------------------------------------------------
//
void CSimpleFilter::ExternalizeL( RWriteStream& aStream )
    {
    CSimpleBaseDocument::ExternalizeL( aStream );
    }

// ----------------------------------------------------------
// CSimpleFilter::Close
// ----------------------------------------------------------
//
void CSimpleFilter::Close()
    {
    CSimpleBaseDocument::Close();
    }

// ----------------------------------------------------------
// CSimpleFilter::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleFilter::LocalName()
    {
    return CSimpleBaseDocument::LocalName();
    }

// ----------------------------------------------------------
// CSimpleFilter::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleFilter::DefNamespaceL()
    {
    return CSimpleBaseDocument::DefNamespaceL();
    }

// ----------------------------------------------------------
// CSimpleFilter::HasContent
// ----------------------------------------------------------
//
TBool CSimpleFilter::HasContent()
    {
    return CSimpleBaseDocument::HasContent();
    }

// ----------------------------------------------------------
// CSimpleFilter::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleFilter::ContentUnicodeL()
    {
    return CSimpleBaseDocument::ContentUnicodeL();
    }

// ----------------------------------------------------------
// CSimpleFilter::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleFilter::SetContentUnicodeL( const TDesC& aContent )
    {
    CSimpleBaseDocument::SetContentUnicodeL( aContent );
    }

// ----------------------------------------------------------
// CSimpleFilter::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleFilter::SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray )
    {
    return CSimpleBaseDocument::SimpleElementsL( aElementArray );
    }

// ----------------------------------------------------------
// CSimpleFilter::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleFilter::AttrValueLC( const TDesC8& aName )
    {
    return CSimpleBaseDocument::AttrValueLC( aName );
    }

// ----------------------------------------------------------
// CSimpleFilter::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleFilter::AttrValue( const TDesC8& aName )
    {
  return CSimpleBaseDocument::AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleFilter::AddAttrL
// ----------------------------------------------------------
//
void CSimpleFilter::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    CSimpleBaseDocument::AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleFilter::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleFilter::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {
    return CSimpleBaseDocument::SimpleAttributesL( aArray );
    }

// ----------------------------------------------------------
// CSimpleFilter::SimpleParentL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleFilter::SimpleParentL()
    {
    return CSimpleBaseDocument::SimpleParentL( );
    }

// ----------------------------------------------------------
// CSimpleFilter::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleFilter::DetachSimpleL()
    {
    User::Leave( KErrNotFound );
    }

// ----------------------------------------------------------
// CSimpleFilter::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleFilter::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleFilter::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleFilter::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aLocalName );
    }

// ----------------------------------------------------------
// CSimpleFilter::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleFilter::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSimpleBaseDocument::RemoveSimpleElement( aNsUri, aLocalName );
    }




