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

#include <e32def.h>           // First to avoid NULL redefine warning (no #ifndef NULL).
#include <e32std.h>
#include <string.h>

#include <imcvcodc.h>

// #include <e32std.h>
#include <s32strm.h>
#include <SenBaseElement.h>
#include <SenBaseAttribute.h>
#include <SenXmlUtils.h>
#include <SenXmlReader.h>
#include <SenDomFragment.h>

#include <multipartparser.h>
#include <bodypart.h>

// own simple
#include "simplecommon.h"
#include "simpleelement.h"
#include "simplenamespace.h"
#include "simpleattribute.h"
#include "simpledocument.h"
#include "simplebasedocument.h"
#include "simpleutils.h"
#include "simplecontent.h"


_LIT8 ( KSimpleEntity, "entity");

// Multipart CONSTANTS

const char Multipart_CRLF_Text[] = {"\r\n"};
const char Multipart_DoubleCRLF_Text[] = {"\r\n\r\n"};

// MACROS
#define MULTIPART_CONTENT_LOCATION_LENGTH           17
#define MULTIPART_CONTENT_TYPE_LENGTH               13
#define MULTIPART_CONTENT_TRANSFER_ENCODING_LENGTH  26
// violates RFC2045; but upon vodafone request
#define MULTIPART_CONTENT_ENCODING_LENGTH           17
#define MULTIPART_CONTENT_LENGTH_LENGTH             15
#define MULTIPART_LAST_MODIFIED_LENGTH              14
#define MULTIPART_CONTENT_ID_LENGTH                 11

#define MULTIPART_CONTENT_LOCATION           2
#define MULTIPART_CONTENT_TRANSFER_ENCODING  3
#define MULTIPART_CONTENT_TYPE               4
#define MULTIPART_CONTENT_ID                 5

#define MULTIPART_INTERNET_DATE_STRING_LENGTH       29

#define SLASH_CHAR    '/'
#define DOT_CHAR      '.'
#define AT_CHAR       '@'
#define COLON_CHAR    ':'
// <scheme>://
// #define SCHEME_SEPARATOR_LENGTH               3
// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimpleDocument::CSimpleDocument
// ----------------------------------------------------------
//
CSimpleDocument::CSimpleDocument( )
: CSimpleBaseDocument()
    {
    }

// ----------------------------------------------------------
// CSimpleDocument::~CSimpleDocument
// ----------------------------------------------------------
//
CSimpleDocument::~CSimpleDocument()
    {
    iContents.ResetAndDestroy();
    iContents.Close();
    }

// ----------------------------------------------------------
// CSimpleDocument::ConstructL
// ----------------------------------------------------------
//
void CSimpleDocument::ConstructL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    BaseConstructL(aNsUri, aLocalName);
    }
    
// ----------------------------------------------------------
// CSimpleDocument::ConstructMultiPartL
// ----------------------------------------------------------
//
void CSimpleDocument::ConstructMultiPartL(
    const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart )
    {    
    _LIT(KUrl, "http://dummy.com/d1/d.html");
           
    // body part array
    RPointerArray<CBodyPart> bodyPartArray;
    TCleanupItem clItem( ResetAndDestroy, &bodyPartArray  );
    CleanupStack::PushL( clItem );
    
    // remove "..." characters from boundary if needed
    TPtrC8 pUnQuoted = aBoundary;
    TInt quoted = aBoundary.Locate('"');
    if (!quoted)
        {
        pUnQuoted.Set( aBoundary.Mid( 1, aBoundary.Length() - 2 ));
        } 
    // parse
    MultipartParser::ParseL( aData, KSimpleMultipartType, pUnQuoted, KUrl, bodyPartArray  );    
    DoConstructL( bodyPartArray, aStart );
   
    CleanupStack::PopAndDestroy( ); // bodyPartArray     
    }    

// ----------------------------------------------------------
// CSimpleDocument::ConstructL
// ----------------------------------------------------------
//
void CSimpleDocument::ConstructL(
    const TDesC8& aXml )
    {
    BaseConstructL( aXml );
    }

// ----------------------------------------------------------
// CSimpleDocument::NewL
// ----------------------------------------------------------
//
CSimpleDocument* CSimpleDocument::NewL( )
    {
    CSimpleDocument* self = new (ELeave) CSimpleDocument( );
    CleanupStack::PushL( self );
    self->ConstructL( KSimpleNsDefault, KDocumentLocalName );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimpleDocument::NewL
// ----------------------------------------------------------
//
CSimpleDocument* CSimpleDocument::NewL( const TDesC8& aXml )
    {
    CSimpleDocument* self = new (ELeave) CSimpleDocument( );
    CleanupStack::PushL( self );
    self->ConstructL( aXml );
    CleanupStack::Pop( self );
    return self;
    }
    
// ----------------------------------------------------------
// CSimpleDocument::NewInMultiPartL
// ----------------------------------------------------------
//
CSimpleDocument* CSimpleDocument::NewInMultiPartL( 
    const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart )
    {
    CSimpleDocument* self = new (ELeave) CSimpleDocument( );
    CleanupStack::PushL( self );
    self->ConstructMultiPartL( aData, aBoundary, aStart );
    CleanupStack::Pop( self );
    return self;
    } 
    
// ----------------------------------------------------------
// CSimpleDocument::GetDirectContentsL
// ----------------------------------------------------------
//
void CSimpleDocument::GetDirectContentsL( 
    RPointerArray<MSimpleContent>& aContents )
    {
    aContents.Reset();
    TInt myCount = iContents.Count();
    for ( TInt i = 0; i<myCount; i++ )
        {
        User::LeaveIfError( aContents.Append( iContents[i] ));
        }               
    }
    
// ----------------------------------------------------------
// CSimpleDocument::AddDirectContentL
// ----------------------------------------------------------
//    
void CSimpleDocument::AddDirectContentL( 
    MSimpleContent& aContent,
    TBool aCopyContent )
    {
    CSimpleContent* content = CSimpleContent::NewLC(
        aContent.ContentID(), aContent.ContentType());
    User::LeaveIfError( iContents.Append( content ));
    CleanupStack::Pop( content );
    
    if ( aCopyContent )
        {
        content->CopyBodyL( aContent.Body() );
        }
    else
        {
        content->SetBody( aContent.GiveBodyOwnerShip() );
        }                       
    }
    
// ----------------------------------------------------------
// CSimpleDocument::ValidateXmlL
// ----------------------------------------------------------
//
void CSimpleDocument::ValidateXmlL( const TDesC8& aName )
    {
    if ( aName.CompareF( KDocumentLocalName ) )
        {
        User::Leave( KErrCorrupt );
        }
    }

// ----------------------------------------------------------
// CSimpleDocument::EntityURI
// ----------------------------------------------------------
//
const TDesC8* CSimpleDocument::EntityURI()
    {
    return Root()->AttrValue( KSimpleEntity );
    }

// ----------------------------------------------------------
// CSimpleDocument::SetEntityURIL
// ----------------------------------------------------------
//
void CSimpleDocument::SetEntityURIL( const TDesC8& aValue )
    {
    Root()->AddAttr8L( KSimpleEntity, aValue );
    }

// ----------------------------------------------------------
// CSimpleDocument::DefaultNamespace
// ----------------------------------------------------------
//

TPtrC8 CSimpleDocument::DefaultNamespace()
    {
    return CSimpleBaseDocument::DefaultNamespace();
    }


// ----------------------------------------------------------
// CSimpleDocument::AddNamespaceL
// ----------------------------------------------------------
//
void CSimpleDocument::AddNamespaceL(
    const TDesC8& aPrefix,
    const TDesC8& aUri )
    {
    CSimpleBaseDocument::AddNamespaceL( aPrefix, aUri );
    }

// ----------------------------------------------------------
// CSimpleDocument::NamespacesL
// ----------------------------------------------------------
//
RPointerArray<MSimpleNamespace>& CSimpleDocument::NamespacesL()
    {
    return CSimpleBaseDocument::NamespacesL();
    }

// ----------------------------------------------------------
// CSimpleDocument::ExternalizeL
// ----------------------------------------------------------
//
void CSimpleDocument::ExternalizeL( RWriteStream& aStream )
    {
    
    // notice: check the max message size 
    
    if ( !iContents.Count() )
        {            
        // single pidf+xml    
        CSimpleBaseDocument::ExternalizeL( aStream );
        }
    else
        {
        RPointerArray<CBodyPart> bodyPartsArray;
        RPointerArray<HBufC8> bufferArray;
        
        TRAPD( err, DoExternalizeMultiPartL( bodyPartsArray, bufferArray, aStream ) );                      
                               
        bufferArray.ResetAndDestroy();        
        bodyPartsArray.ResetAndDestroy();         
        bufferArray.Close();        
        bodyPartsArray.Close();        
        
        User::LeaveIfError( err );                                                                          
        }                                                
    }

// ----------------------------------------------------------
// CSimpleDocument::Close
// ----------------------------------------------------------
//
void CSimpleDocument::Close()
    {
    CSimpleBaseDocument::Close();
    }

// ----------------------------------------------------------
// CSimpleDocument::LocalName
// ----------------------------------------------------------
//
const TDesC8& CSimpleDocument::LocalName()
    {
    return CSimpleBaseDocument::LocalName();
    }

// ----------------------------------------------------------
// CSimpleDocument::DefNamespaceL
// ----------------------------------------------------------
//
MSimpleNamespace* CSimpleDocument::DefNamespaceL()
    {
    return CSimpleBaseDocument::DefNamespaceL();
    }

// ----------------------------------------------------------
// CSimpleDocument::HasContent
// ----------------------------------------------------------
//
TBool CSimpleDocument::HasContent()
    {
    return CSimpleBaseDocument::HasContent();
    }

// ----------------------------------------------------------
// CSimpleDocument::ContentUnicodeL
// ----------------------------------------------------------
//
HBufC* CSimpleDocument::ContentUnicodeL()
    {
    return CSimpleBaseDocument::ContentUnicodeL();
    }

// ----------------------------------------------------------
// CSimpleDocument::SetContentUnicodeL
// ----------------------------------------------------------
//
void CSimpleDocument::SetContentUnicodeL( const TDesC& aContent )
    {
    CSimpleBaseDocument::SetContentUnicodeL( aContent );
    }

// ----------------------------------------------------------
// CSimpleDocument::SimpleElementsL
// ----------------------------------------------------------
//
TInt CSimpleDocument::SimpleElementsL( RPointerArray<MSimpleElement>& aElementArray )
    {
    return CSimpleBaseDocument::SimpleElementsL( aElementArray );
    }

// ----------------------------------------------------------
// CSimpleDocument::AttrValueLC
// ----------------------------------------------------------
//
HBufC* CSimpleDocument::AttrValueLC( const TDesC8& aName )
    {
    return CSimpleBaseDocument::AttrValueLC( aName );
    }

// ----------------------------------------------------------
// CSimpleDocument::AttrValue
// ----------------------------------------------------------
//
const TDesC8* CSimpleDocument::AttrValue( const TDesC8& aName )
    {
  return CSimpleBaseDocument::AttrValue( aName );
    }

// ----------------------------------------------------------
// CSimpleDocument::AddAttrL
// ----------------------------------------------------------
//
void CSimpleDocument::AddAttrL( const TDesC8& aName, const TDesC& aValue )
    {
    CSimpleBaseDocument::AddAttrL( aName, aValue );
    }

// ----------------------------------------------------------
// CSimpleDocument::SimpleAttributesL
// ----------------------------------------------------------
//
TInt CSimpleDocument::SimpleAttributesL( RPointerArray<MSimpleAttribute>& aArray )
    {
    return CSimpleBaseDocument::SimpleAttributesL( aArray );
    }

// ----------------------------------------------------------
// CSimpleDocument::SimpleParentL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleDocument::SimpleParentL()
    {
    return NULL;
    }

// ----------------------------------------------------------
// CSimpleDocument::DetachSimpleL
// ----------------------------------------------------------
//
void CSimpleDocument::DetachSimpleL()
    {
    User::Leave( KErrNotFound );
    }


// ----------------------------------------------------------
// CSimpleDocument::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleDocument::AddSimpleElementL(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aNsUri, aLocalName );
    }

// ----------------------------------------------------------
// CSimpleDocument::AddSimpleElementL
// ----------------------------------------------------------
//
MSimpleElement* CSimpleDocument::AddSimpleElementL(
    const TDesC8& aLocalName )
    {
    return CSimpleBaseDocument::AddSimpleElementL( aLocalName );
    }


// ----------------------------------------------------------
// CSimpleDocument::RemoveSimpleElement
// ----------------------------------------------------------
//
void CSimpleDocument::RemoveSimpleElement(
    const TDesC8& aNsUri,
    const TDesC8& aLocalName )
    {
    CSimpleBaseDocument::RemoveSimpleElement( aNsUri, aLocalName );
    }
    
// ----------------------------------------------------------
// CSimpleDocument::ResetAndDestroy
// ----------------------------------------------------------
//
void CSimpleDocument::ResetAndDestroy( TAny* aPtrArray )
    {
    RPointerArray<CBodyPart>* array =
    	static_cast<RPointerArray<CBodyPart>*>( aPtrArray );
    array->ResetAndDestroy();
    array->Close();	
    }   
       
// ----------------------------------------------------------
// CSimpleDocument::DoConstructL
// ----------------------------------------------------------
//    
void CSimpleDocument::DoConstructL( 
    RPointerArray<CBodyPart>& aParts, const TDesC8& aStart )
    {
    // Handle body parts one by one
    TInt size = aParts.Count();
    TInt i;
    CBodyPart* cp = NULL;
    TPtrC8 boundary;
    TPtrC8 start;
    
    TBool pidfOk ( EFalse );
    
          
    for (i = 0; i < size; i++)
        {
        cp = aParts[i];      
                                     
        if ( (( aStart.Length() > 0 && !aStart.Compare( cp->ContentID()) ) || 
                !pidfOk ) && 
             !cp->ContentType().Left(sizeof(KSimpleDocumentType)).CompareF( KSimpleDocumentType ))
            {
            // Multipart root is in the pidf+xml format
            BaseConstructL( cp->Body() );
            pidfOk = ETrue;
            }
        else 
            {
            // Direct contents, as a separate MIME multipart parts                           
            CSimpleContent* cd = CSimpleContent::NewL( 
                cp->ContentID(), cp->ContentType() );
            cd->CopyBodyL( cp->Body() );                 
            CleanupStack::PushL( cd );           
            User::LeaveIfError( iContents.Append( cd ) );
            CleanupStack::Pop( cd );           
            }                  
        }   
    }    
            
// ------------------------------------------------------------------------- 
// CSimpleDocument::DoComposeMultiPartL
// ------------------------------------------------------------------------- 
HBufC8* CSimpleDocument::DoComposeMultiPartL( RPointerArray<CBodyPart>& aBodyArray,
                                const TDesC8& aBoundary )
    {
    // --(aBoundary)
    using namespace NSimpleDocument;
    HBufC8* boundary = HBufC8::NewLC( aBoundary.Length() + 4 );
    boundary->Des().Format( KBoundary, &aBoundary );
    
    // CALCULATE the size of this document.
    TInt bodySize = 0;
    //    a. for each CBodyPart
    //       boundaries + CRLF between headers and body (constant addition)
    bodySize += (boundary->Length() + strlen(Multipart_CRLF_Text)) * aBodyArray.Count() ;
    TInt bodyCounter = aBodyArray.Count();
    for (TInt i = 0; i < bodyCounter; i++)
        {
        if (!(aBodyArray[i]->Headers().Length() +
            aBodyArray[i]->Body().Length()))
            {
            // one less boundary
            bodySize -= boundary->Length() + strlen(Multipart_CRLF_Text);
            // skip empty bodypart
            continue;
            }
        bodySize += aBodyArray[i]->Headers().Length();
        //  ensure there are only 2 CRLFs between header and body
        if (aBodyArray[i]->Headers().Length() > 0)
            {
            TPtrC8 bodyHeaders(aBodyArray[i]->Headers().Ptr(), aBodyArray[i]->Headers().Length());
            TUint newEnd = bodyHeaders.Length() - 1;
            while( bodyHeaders[ newEnd ] == '\r' || bodyHeaders[ newEnd ] == '\n' )
                {
                --newEnd;
                --bodySize;
                }
            // two CRLFs
            bodySize += strlen(Multipart_CRLF_Text);
            }
        bodySize += aBodyArray[i]->Body().Length();
        //  CRLF (end of body, add one only if there is body AND does not end with CRLF)
        TPtrC8 bodyBody(aBodyArray[i]->Body().Ptr(), aBodyArray[i]->Body().Length());
        if (bodyBody.Length() > 0 
            && bodyBody.Right(2) != TPtrC8((TUint8*)Multipart_CRLF_Text, strlen(Multipart_CRLF_Text)))
            {
            bodySize += strlen(Multipart_CRLF_Text);
            }
        }
    // end boundary (boundary - '\r\n' + "--")
    bodySize += boundary->Length(); 
    TInt docSize = bodySize; 
    
    // CALCULATE the size of Headers
    using namespace NSimpleDocument::NSimpleMulti;    
       
    // extra CRLF for separating header and body
    docSize += strlen(Multipart_CRLF_Text);
    //  CALCULATION COMPLETE
    //  at this point, bodySize contains the size of bodyparts, i.e. Content-Length:
    //  and docSize contains the size of the entire document (use it to create HBufC8*
    //  of appropriate size)
  
    //  CONSTRUCT MULTIPART DOCUMENT
    HBufC8* document = HBufC8::NewLC(docSize);
        
    TPtr8 docAppend(document->Des());
           
    //  BODYPARTS
    for (TInt i = 0; i < aBodyArray.Count(); i++)
        {
        if (!(aBodyArray[i]->Headers().Length() +
            aBodyArray[i]->Body().Length()))
            {
            // skip empty bodypart
            continue;
            }
        docAppend.Append( *boundary );
        TInt headerLength = aBodyArray[i]->Headers().Length() - 1;
        while ( headerLength > 0 &&
                (aBodyArray[i]->Headers()[headerLength] == '\r'
                || aBodyArray[i]->Headers()[headerLength] == '\n' ))
            {
            --headerLength;
            }
        docAppend.Append( aBodyArray[i]->Headers().Ptr(), headerLength + 1 );

        if ( headerLength > 0 )
            {
            docAppend.Append((TUint8*)Multipart_DoubleCRLF_Text, strlen(Multipart_DoubleCRLF_Text));
            }
        else
            {
            docAppend.Append((TUint8*)Multipart_CRLF_Text, strlen(Multipart_CRLF_Text));
            }
        //  body
        TPtrC8 opa = aBodyArray[i]->Body();
        docAppend.Append(aBodyArray[i]->Body());
        //  CRLF only if body exists and doesn't end with CRLF
        TPtrC8 bodyBody(aBodyArray[i]->Body().Ptr(), aBodyArray[i]->Body().Length());
        if (bodyBody.Length() > 0
            && bodyBody.Right(2) != TPtrC8((TUint8*)Multipart_CRLF_Text, strlen(Multipart_CRLF_Text)))
            {
            docAppend.Append((TUint8*)Multipart_CRLF_Text, strlen(Multipart_CRLF_Text));
            }
        }
    //  end boundary
    docAppend.AppendFormat(KEndBoundary, &aBoundary);
    CleanupStack::Pop( document );
    CleanupStack::PopAndDestroy( boundary );
    return document;
    } 
    
// ------------------------------------------------------------------------- 
// CSimpleDocument::DoExternalizeMultiPartL
// -------------------------------------------------------------------------    
void CSimpleDocument::DoExternalizeMultiPartL( 
        RPointerArray<CBodyPart>& aBodies, 
        RPointerArray<HBufC8>& aBuffers,
        RWriteStream& aStream )
    {
    const TReal KB64Expand = 1.5; // reserve room for Base64 encoding
    const TInt KBufferSize = 500;
        
    // Convert all the multipart into CBodyPart.
           
    // Let's convert first ROOT element into CBodyPart
    CBodyPart* root = CBodyPart::NewL();
    // Add into cleanup array
    aBodies.Append( root );     
    CSenElement* e = Root()->BaseElement();
    
    // externalize the document into stream
    CBufFlat* myBuffer = CBufFlat::NewL( KBufferSize );
    CleanupStack::PushL( myBuffer );        
    myBuffer->Reset();
    
    RBufWriteStream stream( *myBuffer );
    stream.Open( *myBuffer );  
    CSimpleBaseDocument::ExternalizeL( stream );
    stream.Close();
    
    // Add content, no need to transfer encode
    TPtrC8 body ( myBuffer->Ptr(0) );
    root->SetBody( body ); 
    
    // Handle the headers of the ROOT part
    using namespace NSimpleDocument::NSimpleRoot;
    
    // calculate the size of headers
    TInt headerSize = KContentTypeSize + KCIDSize;
    HBufC8* headers = HBufC8::NewL( headerSize );  
    aBuffers.Append( headers );
    TPtr8 pH(headers->Des());
    // append to MIME headers for the root part   
    pH.Append( NSimpleDocument::NSimpleRoot::KContentType );
    pH.Append( NSimpleDocument::NSimpleRoot::KCID );    
    root->SetHeaders( pH );    
                                       
    // handle the direct contents one by one
    TInt contCount = iContents.Count();
    for ( TInt i=0; i<contCount; i++ )
        { 
        // Let's convert next element into CBodyPart
        CBodyPart* cp = CBodyPart::NewL();
        aBodies.Append( cp );                
         
        // Set Headers
        headerSize = NSimpleDocument::NSimpleContent::KContentTypeSize + 
                     NSimpleDocument::NSimpleContent::KCIDSize + 
                     NSimpleDocument::NSimpleContent::KContentEncodingSize;
        headerSize += (iContents[i])->ContentID().Length();
        headerSize += (iContents[i])->ContentType().Length();                 
        
        headers = HBufC8::NewL( headerSize );
        // Append to cleanup array  
        aBuffers.Append( headers );
        pH.Set( headers->Des() );

        // _LIT8( KMyContentType, "Content-Type: %S\r\n"); 
        TPtrC8 myValue = (iContents[i])->ContentType();     
        pH.Format( NSimpleDocument::NSimpleContent::KContentType, &myValue );    
        myValue.Set((iContents[i])->ContentID() );         
        pH.AppendFormat( NSimpleDocument::NSimpleContent::KCID, &myValue );            
        pH.Append( NSimpleDocument::NSimpleContent::KContentEncoding ); 
        // append to MIME headers           
        cp->SetHeaders( pH );
                     
        // Body
                    
        // BASE64 encode
        HBufC8* body64 = HBufC8::NewL( (iContents[i])->Body().Length() * KB64Expand );  
        aBuffers.Append( body64 );        
        TImCodecB64 codec64;
        codec64.Initialise();        
        TPtr8 desti8 = body64->Des();  
        
        // notice: leave if error?
        TInt err = codec64.Encode( (iContents[i])->Body(), desti8 );                                 
        
        cp->SetBody( body64->Des() );                                                                                 
        }
                            
    // Compose the multipart MIME flat data   
    HBufC8* entireMsg = NULL;      
    entireMsg = DoComposeMultiPartL( aBodies, NSimpleDocument::KSimpleBoundary );
    CleanupStack::PushL( entireMsg );
    // Finally stream entireMsg, 
    // Notice: later DoComposeMultiPartL could stream directly?
    aStream.WriteL( entireMsg->Des() ); 
    CleanupStack::PopAndDestroy( entireMsg );  
    CleanupStack::PopAndDestroy( myBuffer );                 
 
    }





