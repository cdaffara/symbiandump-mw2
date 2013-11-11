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
#include <e32base.h>
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
#include "simpledocument.h"
#include "simplemeta.h"
#include "simpleutils.h"
#include "simplepresencelist.h"

#include "simplecommon.h"

#ifdef _DEBUG
#include "simpledebugutils.h"
#endif


// ================= MEMBER FUNCTIONS =======================
//

// ----------------------------------------------------------
// CSimplePresenceList::CSimplePresenceList
// ----------------------------------------------------------
//
CSimplePresenceList::CSimplePresenceList( )
: iMeta( NULL )
    {
    }

// ----------------------------------------------------------
// CSimplePresenceList::~CSimplePresenceList
// ----------------------------------------------------------
//
CSimplePresenceList::~CSimplePresenceList()
    {
    delete iMeta;
    iPresList.ResetAndDestroy();
    iDocuments.ResetAndDestroy();         
    iPresList.Close();
    iDocuments.Close();
    }

// ----------------------------------------------------------
// CSimplePresenceList::ConstructL
// ----------------------------------------------------------
//
void CSimplePresenceList::ConstructL(
  const TDesC8& aData, const TDesC8& aBoundary, const TDesC8& aStart )
    {       
    _LIT(KUrl, "http://dummy.com/d1/d.html");
           
    // body part array
    RPointerArray<CBodyPart> bodyPartArray;
    // Cleanup-routine
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
// CSimplePresenceList::NewL
// ----------------------------------------------------------
//
CSimplePresenceList* CSimplePresenceList::NewL( const TDesC8& aXml,
    const TDesC8& aBoundary, const TDesC8& aStart  )
    {
    CSimplePresenceList* self = new (ELeave) CSimplePresenceList( );
    CleanupStack::PushL( self );
    self->ConstructL( aXml, aBoundary, aStart );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------
// CSimplePresenceList::Close
// ----------------------------------------------------------
//
void CSimplePresenceList::Close()
    {
    delete this;
    }
    
// ----------------------------------------------------------
// CSimplePresenceList::ResetAndDestroy
// ----------------------------------------------------------
//
void CSimplePresenceList::ResetAndDestroy( TAny* aPtrArray )
    {
    RPointerArray<CBodyPart>* array =
    	static_cast<RPointerArray<CBodyPart>*>( aPtrArray );
    array->ResetAndDestroy();
    array->Close();	
    }    

// ----------------------------------------------------------
// CSimplePresenceList::GetDocuments
// ----------------------------------------------------------
//
void CSimplePresenceList::GetDocuments( RPointerArray<MSimpleDocument>& aDocs )
    {
    aDocs.Reset();
    TInt size = iDocuments.Count();
    for ( TInt i = 0; i<size ; i++ )
        {
        (void)aDocs.Append( iDocuments[i] );
        }    
    }

// ----------------------------------------------------------
// CSimplePresenceList::MetaData
// ----------------------------------------------------------
//
MSimpleMeta* CSimplePresenceList::MetaData()
    {
    return iMeta;
    }

// ----------------------------------------------------------
// CSimplePresenceList::GetSubLists
// ----------------------------------------------------------
//
void CSimplePresenceList::GetSubLists( RPointerArray<MSimplePresenceList>& aLists )
    {
    aLists.Reset();
    TInt size = iPresList.Count();
    for ( TInt i = 0; i<size ; i++ )
        {
        (void)aLists.Append( iPresList[i] );
        }     
    }
    
// ----------------------------------------------------------
// CSimplePresenceList::DoConstructL
// ----------------------------------------------------------
//    
void CSimplePresenceList::DoConstructL( 
    RPointerArray<CBodyPart>& aParts, const TDesC8& aStart )
    {
    
    _LIT8( KMyContentType, "Content-Type: multipart/related;");  
    _LIT8( KMyBoundary, "boundary="); 
    
    const TInt myBoundaryLen = 9;   // boundary=  9 characters         
       
    // Handle body parts one by one
    TInt size = aParts.Count();
    TInt i;
    CBodyPart* cp = NULL;
    TPtrC8 boundary;
    TPtrC8 start;
    
    // remove "..." quoted marks when needed
    TPtrC8 pStartUnquoted( KNullDesC8 );
    if ( aStart.Locate( '"') == 0 )
        {
        pStartUnquoted.Set( aStart.Mid( 1, aStart.Length() - 2 ));
        }
    else
        {
        pStartUnquoted.Set( aStart );        
        } 

    // remove <...> marks when needed
    if ( pStartUnquoted.Locate( '<') == 0 )
        {
        pStartUnquoted.Set( pStartUnquoted.Mid( 1, pStartUnquoted.Length() - 2 ));
        } 
    
    TPtrC8 p8;
    p8.Set( KSimpleDocumentType );
    TInt mySize = p8.Length();
    p8.Set( KSimpleMultipartType );
    TInt mySize2 = p8.Length();    
          
#ifdef _DEBUG                
    TPtrC p16b; // notice: for debug
    TPtrC8 p8b;
#endif 

// Make this handle direct content too in the RLMI list, i.e. 
// content-type: multipart/related; type="application/pidf+xml"; boundary=...;
    for (i = 0; i < size; i++)
        {
        cp = aParts[i];
        // inline const TDesC8& Headers() { return iHeaders; }
        TPtrC8 allHeaders = cp->Headers();
        HBufC8* headersBuff = allHeaders.AllocL();
        CleanupStack::PushL( headersBuff );
               
#ifdef _DEBUG        
        p16b.Set( cp->Url() ); // debug only
        p8b.Set( cp->ContentType() ); // debug only
        p8b.Set( cp->ContentID() ); // debug only
#endif                         
        if ( !pStartUnquoted.Compare( cp->ContentID()) &&
             !cp->ContentType().Left(sizeof(KSimpleListType)).CompareF( KSimpleListType ))
            {
            // Meta data is the root         
            iMeta = CSimpleMeta::NewL( cp->Body() );                               
            }
        else if ( !cp->ContentType().CompareF( KSimpleDocumentType ))
            {
            // Ordinary presence element                    
            CSimpleDocument* cd = CSimpleDocument::NewL( cp->Body() );             
            CleanupStack::PushL( cd );           
            User::LeaveIfError( iDocuments.Append( cd ) );
            CleanupStack::Pop( cd );
            }             
        else if ( (!cp->ContentType().Left(mySize2).CompareF( KSimpleMultipartType ))) 
            {
            // multipart for user's direct content data
            // get boundary from headers
            TPtrC8 pStart(KNullDesC8);
            TPtrC8 pBoundary(KNullDesC8);          
            
            TPtrC8 pHeaders = headersBuff->Des();
            TPtrC8 pContentType;
            TInt pos1 = pHeaders.Find( KMyContentType );
            if ( pos1 >= 0 )
                {
                TPtrC8 h2 = pHeaders.Mid( pos1 );
                TInt pos2 = h2.Locate( '\r');
                if ( pos2 < 0 )
                    {
                    pContentType.Set( h2 );
                    }
                else
                    {
                    pContentType.Set( h2.Left( h2.Length() - pos2 ));                    
                    }    
                                                                
                // search boundary 
                TInt posx = pContentType.Find( KMyBoundary );
                if ( posx >= 0 )
                    {                    
                    TPtrC8 h5 = pContentType.Mid( posx );
                    TInt pos5 = h5.Locate( ';');
                    if ( pos5 < 0 )
                        {
                        // There are no more parameters
                        pBoundary.Set( h5.Mid( myBoundaryLen )); 
                        }
                    else
                        {
                        // There are more in the line, has to cut off                        
                        pBoundary.Set( h5.Mid( myBoundaryLen, pos5 - myBoundaryLen ));                        
                        }    
                    }                 
                }
            
            // "..." characters are removed later from boundary in NewInMultiPartL(
            CSimpleDocument* cd = CSimpleDocument::NewInMultiPartL( cp->Body(), pBoundary, pStart );
            CleanupStack::PushL( cd );           
            User::LeaveIfError( iDocuments.Append( cd ) );
            CleanupStack::Pop( cd );            
                        
            }        
        /* notice: later nested multiparts if type = rlmi   */
        else
            {
            // This is something that should not be in the document.
            // Ignore it.
            } 
        CleanupStack::PopAndDestroy( headersBuff );                             
        }    
    }

