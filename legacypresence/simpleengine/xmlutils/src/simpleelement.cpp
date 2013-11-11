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
#include <SenBaseElement.h>
#include <SenBaseAttribute.h>
#include <SenXmlUtils.h>

// own simple
#include "simpleelement.h"
#include "simplenamespace.h"
#include "simpleattribute.h"
#include "msimplenamespace.h"

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleElement::CSimpleElement
// ----------------------------------------------------------
//
CSimpleElement::CSimpleElement( )
: iBase(NULL), iRef(NULL)
    {
    }

// ----------------------------------------------------------
// CSimpleElement::~CSimpleElement
// ----------------------------------------------------------
//
CSimpleElement::~CSimpleElement()
    {
    iAttributes.ResetAndDestroy();  
    iElements.ResetAndDestroy();      
    delete iBase;
    }

// ----------------------------------------------------------
// CSimpleElement::ConstructL
// ----------------------------------------------------------
//
void CSimpleElement::ConstructL(
    const TDesC8& aNsUri,
  const TDesC8& aLocalName )
    {
    if ( aNsUri.Length() )
        {
        iBase = CSenBaseElement::NewL( aNsUri, aLocalName);
        }
    else
        {
        iBase = CSenBaseElement::NewL( aLocalName );
        }
    }

// ----------------------------------------------------------
// CSimpleElement::ConstructL
// ----------------------------------------------------------
//
void CSimpleElement::ConstructL(
    CSenElement* aElem, TBool aOwn )
    {
    if ( aOwn )
        {
        iBase = aElem;
        }
    else
        {
        iRef = aElem;
        }

    }

// ----------------------------------------------------------
// CSimpleElement::NewL
// ----------------------------------------------------------
//
CSimpleElement* CSimpleElement::NewL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSimpleElement* self = new (ELeave) CSimpleElement( );
    CleanupStack::PushL( self );
    self->ConstructL( aNsUri, aLocalName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleElement::NewL
// ----------------------------------------------------------
//
CSimpleElement* CSimpleElement::NewL(
    CSenElement* aElement, TBool aOwn )
    {
    CSimpleElement* self = new (ELeave) CSimpleElement( );
    CleanupStack::PushL( self );
    self->ConstructL( aElement, aOwn );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleElement::BaseElement
// ----------------------------------------------------------
//
CSenElement* CSimpleElement::BaseElement()
    {
    return iBase ? iBase : iRef;
    }

// ----------------------------------------------------------
// CSimpleElement::Close
// ----------------------------------------------------------
//
void CSimpleElement::Close()
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleElement::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleElement::LocalName()
    {
    return BaseElement()->LocalName();
    }

// ----------------------------------------------------------
// CSimpleElement::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleElement::DefNamespaceL()
    {
    const CSenNamespace* ns = BaseElement()->Namespace();
    if ( !ns )
        {
        return NULL;
        }
    CSimpleNamespace* simpleNs = CSimpleNamespace::NewL( ns->Prefix(), ns->URI() );
    return simpleNs;
    }

// ----------------------------------------------------------
// CSimpleElement::HasContent
// ----------------------------------------------------------
//
TBool CSimpleElement::HasContent()
    {
    return BaseElement()->HasContent();
    }

// ----------------------------------------------------------
// CSimpleElement::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleElement::ContentUnicodeL()
    {
    // UTF-8 -> Unicode conversion   
    TRAPD( errx, SenXmlUtils::LeaveOnXmlEscapesL( BaseElement()->Content()));
    if ( errx )
        {
    	HBufC8* b8 = SenXmlUtils::DecodeHttpCharactersLC( BaseElement()->Content());
        HBufC16* uniB = SenXmlUtils::ToUnicodeLC( b8->Des() );
        CleanupStack::Pop( uniB );
        CleanupStack::PopAndDestroy( b8 );
        return uniB;    	        
        }
    else
        {
        return BaseElement()->ContentUnicodeL();
        }        
    }
    
// ----------------------------------------------------------
// CSimpleElement::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleElement::SetContentUnicodeL( const TDesC& aContent )
    {
    // Unicode -> UTF-8 conversion    		    
    HBufC8* b8 = SenXmlUtils::ToUtf8LC( aContent);
    TRAPD( errx, SenXmlUtils::LeaveOnXmlEscapesL( b8->Des()));
    if ( errx )
        {
    	HBufC8* b8escaped = SenXmlUtils::EncodeHttpCharactersLC( b8->Des());
        BaseElement()->SetContentL( b8escaped->Des() );
        CleanupStack::PopAndDestroy( b8escaped );
        }
     else
        {
        BaseElement()->SetContentL( b8->Des() );        
        }
    CleanupStack::PopAndDestroy( b8);  
    }
    
// ----------------------------------------------------------
// CSimpleElement::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleElement::SimpleElementsL( 
    RPointerArray<MSimpleElement>& aElementArray )
    {
    // Reset the output array first.
    aElementArray.Reset();
    iElements.ResetAndDestroy();

    // Go through CSenElements and convert them one by one and 
    // add to the output array.
    RPointerArray<CSenElement>& elems = BaseElement()->ElementsL();
    TInt elemCount = elems.Count();
    for ( TInt i = 0; i < elemCount; i++ )
        {
        CSenElement* pElement = elems[i];
        CSimpleElement* simple = CSimpleElement::NewL( pElement, EFalse );
        CleanupStack::PushL( simple );
        User::LeaveIfError( aElementArray.Append( simple ));
        CleanupStack::Pop( simple );
        User::LeaveIfError( iElements.Append( simple ) );
        }
    return elemCount ? KErrNone : KErrNotFound;
    }

// ----------------------------------------------------------
// CSimpleElement::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleElement::AttrValueLC( const TDesC8& aName )
    {
    // Get 8-bit data, ownership is not transferred
    const TDesC8* p8 = BaseElement()->AttrValue( aName );
    // UTF-8 -> Unicode conversion
    // new buffer is created
    HBufC16* b16 = SenXmlUtils::ToUnicodeLC( *p8 );
    return b16;
    }

// ----------------------------------------------------------
// CSimpleElement::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleElement::AttrValue( const TDesC8& aName )
    {
    // Get 8-bit data, ownership is not transferred
    return BaseElement()->AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleElement::AddAttrL
// ----------------------------------------------------------
//
void CSimpleElement::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    // Unicode -> UTF-8 conversion
    HBufC8* b8 = SenXmlUtils::ToUtf8LC( aValue);
    BaseElement()->AddAttrL( aName, b8->Des() );
    CleanupStack::PopAndDestroy( b8 );
    }

// ----------------------------------------------------------
// CSimpleElement::AddAttr8L
// ----------------------------------------------------------
//
void CSimpleElement::AddAttr8L( const TDesC8& aName, const TDesC8& aValue )
    {
    BaseElement()->AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleElement::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleElement::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {  
    // Reset the output array first.
    aArray.Reset();
    iAttributes.ResetAndDestroy();

    // Go through CSenAttributes and convert them one by one and add to the output array.
    RPointerArray<CSenBaseAttribute>& elems = BaseElement()->AttributesL();
    TInt elemCount = elems.Count();
    for ( TInt i = 0; i < elemCount; i++ )
        {
        CSenBaseAttribute* pAttr = elems[i];
        CSimpleAttribute* attr = CSimpleAttribute::NewL( pAttr );
        CleanupStack::PushL( attr );
        User::LeaveIfError(iAttributes.Append( attr ));
        CleanupStack::Pop( attr );        
        User::LeaveIfError(aArray.Append( attr ));
        }
    return elemCount ? KErrNone : KErrNotFound;
    }

// ----------------------------------------------------------
// CSimpleElement::SimpleParentL
// OWNERSHIP IS TRANSFERRED
// ----------------------------------------------------------
//
CSimpleElement* CSimpleElement::SimpleParentL()
    {
    CSenElement* el = BaseElement()->Parent();
    if ( !el )
        {
        return NULL;
        }   
    CSimpleElement* simple = CSimpleElement::NewL( el, ETrue );
    return simple;
    }

// ----------------------------------------------------------
// CSimpleElement::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleElement::DetachSimpleL()
    {
    // Ownership in not transferred.
    CSenElement* e = BaseElement()->DetachL();
    if (!e)
        {
        User::Leave( KErrNotFound );
        }
    }

// ----------------------------------------------------------
// CSimpleElement::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleElement::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSenElement& e = BaseElement()->AddElementL( aNsUri, aLocalName );
    CSimpleElement* simple = CSimpleElement::NewL( &e, EFalse );
    return simple;
    }

// ----------------------------------------------------------
// CSimpleElement::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleElement::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    CSenElement& e = BaseElement()->AddElementL( aLocalName );
    CSimpleElement* simple = CSimpleElement::NewL( &e, EFalse );
    return simple;
    }

// ----------------------------------------------------------
// CSimpleElement::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleElement::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSenElement* e = BaseElement()->RemoveElement( aNsUri, aLocalName );
    delete e;
    }


