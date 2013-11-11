/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: 
*   ?description_line
*
*/

//  INCLUDES
#include "FT_CPosTp157.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <LbsPosition.h>
#include <e32math.h>
#include <bautils.h>
#include <utf.h>
#include <xml/xmlparsererrors.h>

// CONSTANTS

_LIT(KFileTrace, "c:\\documents\\LandmarksTP157Trace.txt");

// See \epoc32\include\xml\XmlParserErrors.h for error codes




// See \epoc32\include\xml\XmlParserErrors.h for error codes


_LIT(KCorrectFile, "c:\\system\\test\\TestData\\CorrectFileFor_LandmarksTP157Trace.txt");

_LIT(KKMZFile, "c:\\system\\test\\TestData\\Tp157KMZFile.kmz");


// Maxmimum size for each line in trace file
const TInt KBufSize = 5000;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp157::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp157::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    delete iEncoderBuffer;
    iEncoderBuffer = NULL;

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();


    delete iLandmarkEncoder;
    iLandmarkEncoder = NULL;

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;


    iLog->Log(_L("CloseTest Done"));
    }
    
// ---------------------------------------------------------
// CPosTp157::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp157::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KKMZFile );


    // Delete the trace file
    DeleteFileL(KFileTrace);


    HBufC8* emptyBuffer=NULL;


   	iLog->Log(_L("Test with Tp157KMZFile.kmz"));
	PrintParsedDataFromEncoderL(KKMZFile, *emptyBuffer, KErrNone, KKmzMimeType );

   
    // Now compare tha trace file against the correct one
    RFile correctFile;
    RFile file;

    iLog->Log(_L(">>>>>>>Comparing files<<<<<"));
    
    TFileName filename;
    CreateCorrectXmlFileL( KCorrectFile, filename );
    
    User::LeaveIfError(correctFile.Open(iFileSession, filename,  EFileRead));
    CleanupClosePushL(correctFile);
    User::LeaveIfError(file.Open(iFileSession, KFileTrace,  EFileRead));
    CleanupClosePushL(file);

    HBufC* correctLinebuffer = HBufC::NewLC( KBufSize );
            TPtr correctLine = correctLinebuffer->Des();    
            HBufC* linebuffer = HBufC::NewLC( KBufSize );
            TPtr line = linebuffer->Des();    
   	
    TInt err = KErrNone;
    TBool done = EFalse;
    TInt lineNr = 0;

    while (ReadLn(correctFile, correctLine) != KErrEof && !done)
        {
        lineNr++;
        err = ReadLn(file, line);
        if ( err == KErrEof ) 
            {
            done = ETrue;    
            }
        correctLine.Trim();
        line.Trim();
        if (correctLine.Compare(line) != KErrNone)
            {
            HBufC* buf = HBufC::NewLC( 128 );
            TPtr buffer = buf->Des();
            
            buffer.Format(_L("ERROR in TraceFile on line %d: "), lineNr);
            
            iLog->Log( buffer );
            iLog->Log( correctLine );
            iLog->Log( line );
            iErrorsFound++;
            CleanupStack::PopAndDestroy( buf );
            }
        }
		CleanupStack::PopAndDestroy(linebuffer);
    CleanupStack::PopAndDestroy(correctLinebuffer);
    CleanupStack::PopAndDestroy(&file);
    CleanupStack::PopAndDestroy(&correctFile);

    // EOF for trace file was reach before EOF in the correct file
    if (done)
        {
        iLog->Log(_L("ERROR: EOF for Trace file was reached to early"));
        iErrorsFound++;
        }
        
        
  
    if (iErrorsFound != KErrNone)
        {
    	iLog->Log(_L("Errors found in TP157"));
    	User::Leave(-1);
        }
      
	}


 // ---------------------------------------------------------
// CPosTp157::PrintParsedDataFromEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp157::PrintParsedDataFromEncoderL(
    const TDesC& aFile, 
    const TDesC8&  aBuffer, 
    TInt aExpectedStatusCode,
    const TDesC8& aMimeType)
    {
    iLog->Log(_L("===== PrintParsedDataFromEncoderL ======="));
    
    iLandmarkParser = CPosLandmarkParser::NewL(aMimeType);

   
        TFileName file( aFile );
        iLog->Log(_L("FILE (%S)"), &file );
        TraceL(_L("----->Using FILE<-------"));
        TraceL(file);
        TRAPD(err,iLandmarkParser->SetInputFileL(file));
	    if(err != KErrNone)
        {
		    TBuf<100> buffe;
			buffe.Format(_L("\t SetInputFile  leaves with error code : %d"), err);
			iLog->Log(buffe);
			      		        
        }
	    else
		{
		    iErrorsFound++;
		   	TBuf<100> buffe;
			buffe.Format(_L("\tERROR: SetInputFile should leave with error code"));
			iLog->Log(buffe);
		       		
		}
	       
   
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    
    iLog->Log(_L("====== PrintParsedDataFromEncoderL Done ========\n")); 
    }
    
    
 
    
    
// ---------------------------------------------------------
// CPosTp157::TraceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp157::TraceL(const TDesC& msg)
    {
    _LIT8( KEnd, "\r\n" );

    RFile file;
    TInt err = file.Open(iFileSession, KFileTrace, EFileWrite);
    if (err == KErrNotFound)
        {
        file.Create(iFileSession, KFileTrace, EFileWrite);
        }

    HBufC8* line8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L( msg );
    
    TInt pos( 0 );
    file.Seek( ESeekEnd, pos );
    file.Write( *line8 );
    file.Write( KEnd );
    file.Close();

    delete line8;
    }

//  End of File 