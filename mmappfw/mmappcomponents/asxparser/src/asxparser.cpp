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
* Description:  Simple v2/v3 ASX-fileparser
*
*/

// Version : %version: 10.1.5.1.4 %



// INCLUDE FILES
#include <e32base.h>
#include <f32file.h>
#include <bautils.h>
#include <utf.h>
#include <asxparser.h>

#include <xml/dom/xmlengdomimplementation.h>
#include <xml/dom/xmlengdomparser.h>
#include <xml/dom/xmlengdocument.h>
#include <xml/dom/xmlengelement.h>
#include <xml/dom/xmlengnodelist.h>

#include "AsxParser_debug.h"

// CONSTANTS
#define KMaxAsxFileSize 5192
_LIT8( KNoSkip, "no" );

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
//   CAsxParser::NewL
//   Two-phased constructor.
// -----------------------------------------------------------------------------
EXPORT_C CAsxParser* CAsxParser::NewL( const TDesC& aFileName )
{
    ASX_DEBUG(_L("#MP# CAsxParser::NewL(%S)"), &aFileName);

    CAsxParser* self = new( ELeave ) CAsxParser();

    CleanupStack::PushL( self );
    self->ConstructL( aFileName );
    CleanupStack::Pop( self );
    return self;
}

// -----------------------------------------------------------------------------
//   CAsxParser::NewL
//   Two-phased constructor.
// -----------------------------------------------------------------------------
EXPORT_C CAsxParser* CAsxParser::NewL( RFile& aFile )
{
    ASX_DEBUG(_L("#MP# CAsxParser::NewL(aFile)"));

    CAsxParser* self = new( ELeave ) CAsxParser();

    CleanupStack::PushL( self );
    self->ConstructL( aFile );
    CleanupStack::Pop( self );
    return self;
}

// -----------------------------------------------------------------------------
//   CAsxParser::~CAsxParser()
//   Destructor
// -----------------------------------------------------------------------------
EXPORT_C CAsxParser::~CAsxParser()
{
    ASX_DEBUG(_L("#MP# CAsxParser::~CAsxParser()"));

    for ( TInt i = 0 ; i < iAsxArray.Count() ; i++ )
    {
        delete iAsxArray[i]->url;
    }

    iAsxArray.ResetAndDestroy();
}

// -----------------------------------------------------------------------------
//   CAsxParser::CAsxParser
//   C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
CAsxParser::CAsxParser()
{
    ASX_DEBUG(_L("#MP# CAsxParser::CAsxParser()"));
}

// -----------------------------------------------------------------------------
//   CAsxParser::ConstructL
//   Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CAsxParser::ConstructL( const TDesC& aFileName )
{
    ASX_DEBUG(_L("#MP# CAsxParser::CAsxParser(%S)"), &aFileName);

    RFs rFs;
    RFile asxFile;

    User::LeaveIfError( rFs.Connect() );
    CleanupClosePushL( rFs );

    if ( ! BaflUtils::FileExists( rFs, aFileName ) )
    {
        ASX_DEBUG(_L("#MP# CAsxParser::ConstructL() file not found"));
        User::Leave( KErrNotFound );
    }

    User::LeaveIfError( asxFile.Open( rFs, aFileName, EFileShareAny ) );
    CleanupClosePushL( asxFile );
	HandleFileParsingL( asxFile );
    CleanupStack::PopAndDestroy( 2 ); // rFs, asxFile

}


// -----------------------------------------------------------------------------
//   CAsxParser::ConstructL
//   Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
void CAsxParser::ConstructL( RFile& aFile  )
{
    ASX_DEBUG(_L("#MP# CAsxParser::ConstructL(aFile)"));
    RFile asxFile;
	
    if( ! aFile.SubSessionHandle() )
    {
        ASX_DEBUG(_L("#MP# CAsxParser::ConstructL(aFile) file handle not found"));
        User::Leave( KErrNotFound );
    }

    User::LeaveIfError( asxFile.Duplicate( aFile ) );
    CleanupClosePushL( asxFile );
    HandleFileParsingL( asxFile );
    asxFile.Close();
    CleanupStack::PopAndDestroy( ); // asxFile
}

// -----------------------------------------------------------------------------
//   CAsxParser::ParseAsxV2HeaderL
// -----------------------------------------------------------------------------
void CAsxParser::ParseAsxV2HeaderL( TPtr8 aPtr )
{
    ASX_DEBUG(_L("#MP# CAsxParser::ParseAsxV2HeaderL()"));

    iVersion = 2;
    TInt location = 0;

    //
    //  Find all RefX= string and the URLs
    //
    //  v2 ASX file has simple Ref-strings for URLs, one URL per line
    //  File starts with [Reference]
    //  Example:
    //     [Reference]
    //        Ref1=http://server.com:8888/wm/file.wmv?MSWMExt=.asf
    //        Ref2=http://server2.com:8888/wm/file.wmv?MSWMExt=.asf
    //
    for ( TInt refNumber = 1 ; location >= 0 ; refNumber++ )
    {
        HBufC8* refString = HBufC8::NewLC(10);
        TPtr8 refPtr = refString->Des();

        refPtr.Append(_L("Ref"));
        refPtr.AppendNum(refNumber);
        refPtr.Append(_L("="));

        location = aPtr.FindF( refPtr );

        if ( location != KErrNotFound )
        {
            location     += refPtr.Length();
            TPtrC8 mid    = aPtr.Mid(location);
            TInt length   = mid.Locate(EKeyEnter);
           
            if ( length == KErrNotFound )
            {
                length = mid.Length();
            }
                               
            AsxStruct* asxItem = new( ELeave ) AsxStruct ;
            CleanupStack::PushL( asxItem );

            // default the seek to true 
            asxItem->seek = ETrue;
            TPtrC8 urlStr    = mid.Left(length);
            asxItem->url = urlStr.AllocL();
            iAsxArray.Append(asxItem);  
            
            CleanupStack::Pop();   // pop the asxItem
        }

        CleanupStack::PopAndDestroy( refString );
    }
}


// -----------------------------------------------------------------------------
//  CAsxParser::ParseAsxV3HeaderL
// -----------------------------------------------------------------------------
void CAsxParser::ParseAsxV3HeaderL( TPtr8 aPtr )
{
    ASX_DEBUG(_L("#MP# CAsxParser::ParseAsxV3HeaderL()"));

    RXmlEngDOMImplementation DOM_impl;
    DOM_impl.OpenL();

    RXmlEngDOMParser parser;
    TInt error = parser.Open( DOM_impl );
    
    if (error == KErrNone)
    {
	    RXmlEngDocument doc;

	    //
	    //  Parse the xml data stream
	    //
	    TRAPD( err, doc = parser.ParseL(aPtr) );

	    if ( ! err )
	    {
	        iVersion = 3;
	        TXmlEngElement element;
	        TXmlEngNode node;
	        RXmlEngNodeList<TXmlEngElement> nodelist1;
	        RXmlEngNodeList<TXmlEngElement> nodelist2;
	        node = doc.DocumentElement();
	        node.AsElement().GetChildElements(nodelist1);

	        CleanupClosePushL(nodelist1);
	        CleanupClosePushL(nodelist2);

	        _LIT8(KEntry,"ENTRY");
	        _LIT8(KRef,"REF");
            _LIT8(KHRef,"HREF");
            _LIT8(KClientSkip,"CLIENTSKIP");

	        while ( nodelist1.HasNext() )
	        {
	            element = nodelist1.Next();
	            element.GetChildElements(nodelist2);

	            //
	            //  In v3 ASX file the streaming URLs are REF tags under
	            //  ENTRY-element
	            //
	            //  Search for all ENTRYs and REFs under them
	            //  Example
	            //  <ENTRY>
	            //        <REF HREF = "rtsp://ServerName/Path/FileName.wmv" />
	            //        <BANNER HREF="http://ServerName/Path/Banner1.gif">
	            //            <MOREINFO HREF ="http://www.microsoft.com/windows/windowsmedia" />
	            //            <ABSTRACT>This is the description for this clip.</ABSTRACT>
	            //        </BANNER>
	            //  </ENTRY>
	            //
	            TPtrC8 name = element.Name();

	            if ( ! element.Name().CompareF(KEntry) )
	            {
                    AsxStruct* asxItem = new( ELeave ) AsxStruct;
                    CleanupStack::PushL( asxItem );

                    // init to True
                    asxItem->seek = ETrue;
	                
                    TBool entryHasAttributes = element.HasAttributes();
                    if ( entryHasAttributes )
                    {
                        RXmlEngNodeList<TXmlEngAttr> attributeList;
                        element.GetAttributes(attributeList);
                        CleanupClosePushL(attributeList);

                        while ( attributeList.HasNext() )
                        {
                            TXmlEngAttr attr = attributeList.Next();

                            if ( ! attr.Name().CompareF(KClientSkip) )
                            {
                                TPtrC8 attrData = attr.Value();

                                if ( ! attrData.CompareF(KNoSkip) )
                                {
                                    asxItem->seek = EFalse; 
                                }
                            }
                        }
                        CleanupStack::PopAndDestroy(); //attributeList
                    }
	            
                    TBool urlIsSet = EFalse;
	                while( !urlIsSet && nodelist2.HasNext() )
	                {
	                    element = nodelist2.Next();

	                    if ( ! element.IsNull() )
	                    {
	                        TPtrC8 name = element.Name();

	                        if ( ! element.Name().CompareF(KRef) )
	                        {
	                            TBool hasAttributes = element.HasAttributes();

	                            RXmlEngNodeList<TXmlEngAttr> attributeList;

	                            element.GetAttributes(attributeList);

	                            CleanupClosePushL(attributeList);

	                            while ( attributeList.HasNext() )
	                            {
	                                TXmlEngAttr attr = attributeList.Next();

	                                if ( ! attr.Name().CompareF(KHRef) )
	                                {
	                                    TPtrC8 attrData = attr.Value();
	                                    asxItem->url = attrData.AllocL();
	                                    iAsxArray.Append(asxItem);
	                                    urlIsSet = ETrue;
	                                    break;
	                                }
	                            }

	                            CleanupStack::PopAndDestroy(); //attributeList
	                        }
	                    }
	                }
                    CleanupStack::Pop();   // pop the asxItem                    
	            }
	        }

	        CleanupStack::PopAndDestroy(); //nodelist2
	        CleanupStack::PopAndDestroy(); //nodelist1
	    }

	    doc.Close();
	    parser.Close();
	    DOM_impl.Close();
    
    }
}

// -----------------------------------------------------------------------------
//  CAsxParser::GetUrl
//  First URL at position 1
// -----------------------------------------------------------------------------
EXPORT_C TInt CAsxParser::GetUrl( const TUint aUrlIndex, TPtrC8& aUrl )
{
    ASX_DEBUG(_L("#MP# CAsxParser::GetUrl(%d)"), aUrlIndex);

    TInt retVal = KErrNone;

    if ( aUrlIndex - 1 > iAsxArray.Count() || iAsxArray.Count() == 0 )
    {
        ASX_DEBUG(_L("#MP# CAsxParser::GetUrl() Bad index"));
        retVal = KErrArgument;
    }
    else
    {
        aUrl.Set( (iAsxArray[aUrlIndex - 1]->url)->Des() );
    }

    return retVal;
}

// -----------------------------------------------------------------------------
//  CAsxParser::GetUrl
//  First URL at position 1
// -----------------------------------------------------------------------------
EXPORT_C AsxStruct* CAsxParser::GetUrl( const TUint aUrlIndex )
{
    ASX_DEBUG(_L("#MP# CAsxParser::GetUrl(%d)"), aUrlIndex);

    AsxStruct* retVal = NULL;

    if ( aUrlIndex  <= iAsxArray.Count() )
    {
        retVal = iAsxArray[aUrlIndex - 1];
    }

    return retVal;
}


// -----------------------------------------------------------------------------
//  CAsxParser::GetUrlCount
// -----------------------------------------------------------------------------
EXPORT_C void CAsxParser::GetUrlCount( TUint &aHowManyUrls )
{
    aHowManyUrls = iAsxArray.Count();

    ASX_DEBUG(_L("#MP# CAsxParser::GetUrlCount(%d)"), aHowManyUrls);
}

// -----------------------------------------------------------------------------
//  CAsxParser::IsValid
// -----------------------------------------------------------------------------
EXPORT_C TInt CAsxParser::FileVersion()
{
    ASX_DEBUG(_L("#MP# CAsxParser::FileVersion(%d)"), iVersion);

    return iVersion;
}

// -----------------------------------------------------------------------------
// CAsxParser::HandleFileParsingL
// -----------------------------------------------------------------------------
//
void CAsxParser::HandleFileParsingL( RFile& aFile )
{
    iVersion = KErrNotFound;

    TInt size = 0;
    User::LeaveIfError( aFile.Size( size ) );

    if ( size > KMaxAsxFileSize )
    {
        ASX_DEBUG(_L("#MP# CAsxParser::HandleFileParsingL() file size > max size"));
        User::Leave( KErrNotSupported );
    }

    HBufC8* urlBuf = HBufC8::NewLC( size );
    TPtr8 ptr = urlBuf->Des();

    //
    //  Read ASX-file to urlBuf
    //
    User::LeaveIfError( aFile.Read( ptr ) );

    //
    //  V2 file will start with [Reference]
    //
    _LIT8(KAsxV2Start,"[Reference]");
    TInt location = ptr.FindF( KAsxV2Start );

    if ( location == 0 )
    {
        //
        //  Found V2 ASX file header
        //
        ParseAsxV2HeaderL( ptr );
    }
    else
    {
        //
        //  Did not find V2 ASX file, V3 will start with <ASX
        //
        _LIT8(KAsxV3Start,"<ASX");
        TInt location = ptr.FindF( KAsxV3Start );

        if ( location == 0 )
        {
            ParseAsxV3HeaderL( ptr );
        }
        else
        {
            ASX_DEBUG(_L("#MP# CAsxParser::HandleFileParsingL() ASX file not V2 or V3"));
            User::Leave( KErrNotSupported );
        }
    }
    
    CleanupStack::PopAndDestroy(); // urlBuf   
}

// -----------------------------------------------------------------------------
//  CAsxParser::PrintUrl
//  First URL at position 1
// -----------------------------------------------------------------------------
EXPORT_C void CAsxParser::PrintUrl( TPtrC8& aUrl8, TPtr& aUrl )
{
    ASX_DEBUG(_L("#MP# CAsxParser::PrintUrl()"));

	if ( aUrl8.Length() )
	{
		CnvUtfConverter::ConvertToUnicodeFromUtf8(aUrl, aUrl8);	
	    ASX_DEBUG(_L("#MP# CAsxParser::PrintUrl(%S)"), &aUrl);
	}
}
//  EOF
