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

// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleBaseDocument::CSimpleBaseDocument
// ----------------------------------------------------------
//
CSimpleBaseDocument::CSimpleBaseDocument( )
: iRoot( NULL )
    {
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::~CSimpleBaseDocument
// ----------------------------------------------------------
//
CSimpleBaseDocument::~CSimpleBaseDocument()
    {
    // Reset and delete namespace C-class entities.
    iNsps.ResetAndDestroy();
    // Just reset array of M-class entities, 
    // do not try to delete entities.
    iMNsps.Reset();
    // Delete root entity.
    delete iRoot;
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::BaseConstructL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::BaseConstructL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    iRoot = CSimpleElement::NewL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::BaseConstructL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::BaseConstructL(
    const TDesC8& aXml )
    {       
    CSenXmlReader* reader = CSenXmlReader::NewL();
    CleanupStack::PushL(reader);
    //create a CSenDomFragment
    CSenDomFragment* pBase = CSenDomFragment::NewL();
    CleanupStack::PushL( pBase );

    //must set the content handler
    reader->SetContentHandler( *pBase );
    // and the reader
    pBase->SetReader( *reader );
    //do the parsing
    reader->ParseL( aXml );

    // Get the root element
    // OWNERSHIP IS TRANSFERRED
    //
    CSenElement* e = pBase->ExtractElement();
    const TDesC8& p8 = e->LocalName();
    ValidateXmlL( p8 );
    // This is a document root
    iRoot = CSimpleElement::NewL( e, ETrue );

    CleanupStack::PopAndDestroy( pBase );
    CleanupStack::PopAndDestroy( reader );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::DefaultNamespace
// ----------------------------------------------------------
//
TPtrC8 CSimpleBaseDocument::DefaultNamespace()
    {
    TInt err(KErrNone);
    TPtrC8 p8;
    TRAP( err, p8.Set( DoDefaultNamespaceL() ));
    if ( err )
        {
        return TPtrC8();
        }
    return p8;
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::AddNamespaceL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::AddNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    iRoot->BaseElement()->AddNamespaceL( aPrefix, aUri );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::NamespacesL
// ----------------------------------------------------------
//
RPointerArray<MSimpleNamespace>& CSimpleBaseDocument::NamespacesL()
    {
    // Notice: 
    // We have to maintain two arrays, one for C-classes and another
    // for M-classes, since deletion of M-classes does not work.
    // This is not very good design to return RpointerArray of
    // M-classes, but let's not change API for this minor drawback.
     
    // reset old stuff
    iNsps.ResetAndDestroy();
    iMNsps.Reset();

    RPointerArray<CSenNamespace>& nsa = iRoot->BaseElement()->NamespacesL();

    TInt elemCount = nsa.Count();
    for ( TInt i = 0; i < elemCount; i++ )
        {
        CSenNamespace* ns = nsa[i];
        CSimpleNamespace* simpleNs = CSimpleNamespace::NewL( ns->Prefix(), ns->URI() );
        User::LeaveIfError( iNsps.Append( simpleNs ) );
        User::LeaveIfError( iMNsps.Append( simpleNs ) );
        }

    return iMNsps;
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::ExternalizeL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::ExternalizeL( RWriteStream& aStream )
    {
    aStream.WriteL( KSimpleXmlStart );
    iRoot->BaseElement()->WriteAsXMLToL( aStream);
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::Close
// ----------------------------------------------------------
//
void CSimpleBaseDocument::Close()
    {
    delete this;
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleBaseDocument::LocalName()
    {
    return iRoot->LocalName();
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleBaseDocument::DefNamespaceL()
    {
    return iRoot->DefNamespaceL();
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::HasContent
// ----------------------------------------------------------
//
TBool CSimpleBaseDocument::HasContent()
    {
    return iRoot->HasContent();
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleBaseDocument::ContentUnicodeL()
    {
    return iRoot->ContentUnicodeL();
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::SetContentUnicodeL( const TDesC& aContent )
    {
    iRoot->SetContentUnicodeL( aContent );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleBaseDocument::SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray )
    {
    return iRoot->SimpleElementsL( aElementArray );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleBaseDocument::AttrValueLC( const TDesC8& aName )
    {
    return iRoot->AttrValueLC( aName );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleBaseDocument::AttrValue( const TDesC8& aName )
    {
  return iRoot->AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::AddAttrL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    iRoot->AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleBaseDocument::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {
    return iRoot->SimpleAttributesL( aArray );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::SimpleParentL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleBaseDocument::SimpleParentL()
    {
    return iRoot->SimpleParentL( );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleBaseDocument::DetachSimpleL()
    {
    return iRoot->DetachSimpleL( );
    }


// ----------------------------------------------------------
// CSimpleBaseDocument::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleBaseDocument::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return iRoot->AddSimpleElementL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleBaseDocument::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    return iRoot->AddSimpleElementL( aLocalName );
    }


// ----------------------------------------------------------
// CSimpleBaseDocument::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleBaseDocument::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    iRoot->RemoveSimpleElement( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleBaseDocument::Root
// ----------------------------------------------------------
//
CSimpleElement* CSimpleBaseDocument::Root()
    {
    return iRoot;
    }
    
// ----------------------------------------------------------
// CSimpleBaseDocument::DoDefaultNamespaceL
// ----------------------------------------------------------
//
TPtrC8 CSimpleBaseDocument::DoDefaultNamespaceL()
    {
    RPointerArray<CSenNamespace>& nsa = iRoot->BaseElement()->NamespacesL();
    TInt elemCount = nsa.Count();
    if ( !elemCount )
        {
        User::Leave( KErrNotFound );
        }
    CSenNamespace* ns = nsa[0];
    return ns->URI();
    }    




