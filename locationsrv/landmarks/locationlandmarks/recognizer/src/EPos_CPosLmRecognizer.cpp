/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Source for landmark recognizer
*
*/


#include <f32file.h>
#include "EPos_CPosLmRecognizer.h"
#include "EPos_CPosLmSimpleParser.h"

// CONSTANTS
enum TMimeTypes
    {
    EMimeLmxType = 0,
    EMimeGpxType,
    EMimeKmlType,
    EMimeKmzType,
    ELastType
    };

// The recognizer UID.
//
const TUid KUidLmRecognizer = {0x101fdf83};

// The priority of the recognizer, can be EHigh, ENormal, ELow.
//
const TInt KLmRecognizerPriority = CApaDataRecognizerType::EHigh;

// The preferred size of the data buffer that
// will be passed to the recognizer so that it
// performs the recognition.
//
const TInt KLmRecognizerBufferSize = 250;

// The MIME Type that will be recognized.
//
_LIT8(KLmxMimeType, "application/vnd.nokia.landmarkcollection+xml");
_LIT8(KGpxMimeType, "application/gps+xml");
_LIT8(KKmlMimeType, "application/vnd.google-earth.kml+xml");
_LIT8(KKmzMimeType, "application/vnd.google-earth.kmz");
// File extensions
_LIT(KLmxFileExtension, ".lmx");
_LIT(KGpxFileExtension, ".gpx");
_LIT(KKmlFileExtension, ".kml");
_LIT(KKmzFileExtension, ".kmz");

// Constant for searching for the XML start tag.
//
_LIT8(KPosLmXmlStartTag, "<?xml");

// Constant for searching for the end of the XML start tag.
//
_LIT8(KPosLmXmlStartTagEnd, "?>");

// Constant for searching for an XML comment tag.
//
_LIT8(KPosLmCommentTag, "<!--");

// Constant for searching for the end of an XML comment tag.
//
_LIT8(KPosLmCommentTagEnd, "-->");

// Constant for searching for an XML tag.
//
_LIT8(KPosLmXmlTagStart, "<");

// Constant for searching for the end of an XML tag.
//
_LIT8(KPosLmXmlTagEnd, ">");

// Constant for searching for root tag.
//
_LIT8(KPosLmLmx, "lmx");
_LIT8(KPosLmGpx, "gpx");
_LIT8(KPosLmKml, "kml");
_LIT8(KPosLmKmz, "kmz");

// Constant for searching for the Landmarks Exchange Format link.
//
_LIT8(KPosLmLmxLink, "http://www.nokia.com/schemas/location/landmarks/");
_LIT8(KPosLmGpxLink, "http://www.topografix.com/GPX/");
_LIT8(KPosLmKmlLink, "http://earth.google.com/kml/");

#ifdef _DEBUG
#define LOG(_msg)           RDebug::Print(_L(_msg));
#define LOG1(_msg,_par)     RDebug::Print(_L(_msg),_par);
#else
#define LOG(_msg)
#define LOG1(_msg,_par)
#endif
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
CPosLmRecognizer::CPosLmRecognizer() :
    CApaDataRecognizerType(KUidLmRecognizer, KLmRecognizerPriority)
    {
    iCountDataTypes = ELastType;
    }

// -----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
CApaDataRecognizerType* CPosLmRecognizer::CreateRecognizerL()
    {
    return new (ELeave) CPosLmRecognizer;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUint CPosLmRecognizer::PreferredBufSize()
    {
    return KLmRecognizerBufferSize;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TDataType CPosLmRecognizer::SupportedDataTypeL( TInt aIndex ) const
    {
    switch ( aIndex ) 
        {
        case EMimeLmxType:
            return TDataType(KLmxMimeType);
        case EMimeGpxType:
            return TDataType(KGpxMimeType);
        case EMimeKmlType:
            return TDataType(KKmlMimeType); 
        case EMimeKmzType:
            return TDataType(KKmzMimeType);    
        default:
            User::Leave( KErrNotSupported );
            return iDataType; // just to return anything, never happens
        }
    }

// -----------------------------------------------------------------------------
// If shared file handle is given, only it is used for recognition.
// Otherwise, given buffer is checked. If not successful - check file
// If buffer does not contain XML prolog, then file not even checked.
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::DoRecognizeL(
    const TDesC& aName,
    const TDesC8& aBuffer)
    {
    // Initialise the result status
    
    iDataType = TDataType( KNullDesC8 );
    iConfidence = ENotRecognized;
    iXmlPrologFound = EFalse;
    iIsNotXmlData = EFalse;

    // First of all - check shared file
    RFile* fileToRecognize = FilePassedByHandleL();
    if ( fileToRecognize )
        {
        LOG("LmRecognizer: Checking shared file");
        // A filehandle was passed. Recognize in passed filehandle
        CheckFileL( *fileToRecognize );
        }
    else
        {
        LOG1("LmRecognizer: Checking buffer of size %d", aBuffer.Size());

        // Check aBuffer
        CheckBufferL( aBuffer );

        if ( iConfidence != ECertain )
            {
            // check file. it may contain more data than buffer
            // but no checks if sure that file is not XML
            if ( !iIsNotXmlData )
                {
                CheckFileL( aName );
                }
            }
        }

    // last resort in recognition
    // this can happen is given buffer is empty and
    // file cannot be accessed
    if ( iConfidence < EProbable && !iIsNotXmlData )
        {
        CheckFileExtension( aName );
        }
     
     //for supporting Kmz file. these files will not be xml files. zzz
    if (iConfidence == ENotRecognized)
    	CheckFileExtension( aName );

#ifdef _DEBUG
    if ( iConfidence >= EPossible )
        {
        LOG1("LmRecognizer: Content recognized with confidence %d " , iConfidence );
        }
    else
        {
        LOG("LmRecognizer: Content unknown");
        }
#endif        
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::CheckBufferL( const TDesC8& aBuffer )
    {
    CPosLmSimpleParser* parser = CPosLmSimpleParser::NewLC( aBuffer );
    CheckXmlContent( *parser );
    CleanupStack::PopAndDestroy( parser );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::CheckFileL( const TDesC& aFileName )
    {
    LOG1("LmRecognizer: Checking file %S", &aFileName);            

    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );

    RFile file;
    CleanupClosePushL( file );
    TInt err = file.Open( fs, aFileName, EFileRead | EFileShareReadersOnly );
    if (!err)
        {
        CheckFileL( file );
        }
    CleanupStack::PopAndDestroy( &file );
    CleanupStack::PopAndDestroy( &fs );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::CheckFileL( const RFile& aFile)
    {
    CPosLmSimpleParser* parser = CPosLmSimpleParser::NewLC(
        aFile, KLmRecognizerBufferSize );

    CheckXmlContent( *parser );
    CleanupStack::PopAndDestroy( parser );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmRecognizer::CheckXmlProlog( CPosLmSimpleParser& aParser )
    {
    // Check that the XML tag is the first thing in the input.
    TInt err = aParser.CheckForStringNext( KPosLmXmlStartTag );
    if ( err )
        {
        if ( err == KErrNotFound )
            {
            iIsNotXmlData = ETrue;
            }
        return err;
        }

    // Find the end of the XML start tag.
    err = aParser.FindString( KPosLmXmlStartTagEnd ); 
    if ( err )
        {
        return err;
        }
    
    LOG("LmRecognizer: xml prolog found");
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::SkipComments( CPosLmSimpleParser& aParser )
    {
    TBool isComment = ETrue;
    do
        {
        aParser.SkipSpaces();
        if ( aParser.CheckForStringNext( KPosLmCommentTag ) )
            {
            isComment = EFalse;
            }
        else
            {
            aParser.FindString( KPosLmCommentTagEnd );
            }
        } while ( isComment );

    aParser.SkipSpaces();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmRecognizer::CheckXmlContent( CPosLmSimpleParser& aParser )
    {
    // First check for LMX format
    LOG("LmRecognizer: checking for LMX...");
    TInt err = CheckXmlContent( aParser, KPosLmLmx, KPosLmLmxLink, KLmxMimeType ); 
    if ( err && iXmlPrologFound )
        {
        // Otherwise check for GPX format
        // However there is no point to check for other formats
        // if XML prolog wasn't found (the file is not XML or more data needed)
        LOG("LmRecognizer: checking for GPX...");
        aParser.Restart();
        err = CheckXmlContent( aParser, KPosLmGpx, KPosLmGpxLink, KGpxMimeType ); 
        if(err)
        {
        LOG("LmRecognizer: checking for KML...");
        aParser.Restart();
        err = CheckXmlContent( aParser, KPosLmKml, KPosLmKmlLink, KKmlMimeType ); 	
        }
        }
        
    return err;
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TInt CPosLmRecognizer::CheckXmlContent( 
    CPosLmSimpleParser& aParser,
    const TDesC8& aRootTag,
    const TDesC8& aNamespace,
    const TDesC8& aMimeType )
    {
    // Ignore UTF-8 Byte Order Marks
    aParser.SkipUTF8Marks();

    TInt err = CheckXmlProlog( aParser );
    if ( err )
        {
        LOG("LmRecognizer: XML prolog not found");
        return err;
        }
    else
        {
        iXmlPrologFound = ETrue;
        }

    // Ignore any comment tags
    SkipComments( aParser );

    // Check that an XML tag is found next
    err = aParser.CheckForStringNext( KPosLmXmlTagStart );
    if ( err )
        {
        return err;
        }

    // Check root tag
    if ( aParser.FindString( aRootTag, KPosLmXmlTagEnd ) == KErrNone )
        {
        LOG("LmRecognizer: root tag found" );
        iDataType = TDataType( aMimeType );
        iConfidence = EProbable;

        // Search for namespace string
        // If it is found, increase the confidence level
        if ( aParser.FindString( aNamespace, KPosLmXmlTagEnd ) == KErrNone )
            {
            iConfidence = ECertain;
            LOG("LmRecognizer: namespace found" );
            }
        return KErrNone;
        }
    else
        {
        return KErrNotFound;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CPosLmRecognizer::CheckFileExtension( const TDesC& aFilename )
    {
    TParse parse;
    parse.Set( aFilename, NULL, NULL );
    if ( parse.Ext().CompareF( KLmxFileExtension ) == 0 )
        {
        iConfidence = EPossible;
        iDataType = TDataType( KLmxMimeType );
        }
    else if ( parse.Ext().CompareF( KGpxFileExtension ) == 0 )
        {
        iConfidence = EPossible;
        iDataType = TDataType( KGpxMimeType );
        }
    else if ( parse.Ext().CompareF( KKmlFileExtension ) == 0 )
        {
        iConfidence = EPossible;
        iDataType = TDataType( KKmlMimeType );
        }
    else if ( parse.Ext().CompareF( KKmzFileExtension ) == 0 )
        {
        iConfidence = EPossible;
        iDataType = TDataType( KKmzMimeType );
        }    
    }

//  End of File
