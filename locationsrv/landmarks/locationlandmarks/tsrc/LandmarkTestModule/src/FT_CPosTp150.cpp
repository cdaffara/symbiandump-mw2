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
#include "FT_CPosTp150.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <LbsPosition.h>
#include <e32math.h>
#include <bautils.h>
#include <utf.h>
#include <xml/xmlparsererrors.h>

// CONSTANTS

_LIT(KFileTrace, "c:\\documents\\LandmarksTP150Trace.txt");

// See \epoc32\include\xml\XmlParserErrors.h for error codes



// See \epoc32\include\xml\XmlParserErrors.h for error codes


_LIT(KCorrectFile, "c:\\system\\test\\TestData\\CorrectFileFor_LandmarksTP150Trace.txt");

_LIT(KKMZFile, "c:\\system\\test\\TestData\\Tp150KMZFile.kmz");


// Maxmimum size for each line in trace file
const TInt KBufSize = 5000;

// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CPosTp150::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::CloseTest()
    {
    iLog->Log(_L("CloseTest"));

    

    iLandmarks.ResetAndDestroy();
    iLandmarks.Close();

    

   

    delete iLandmarkParser;
    iLandmarkParser = NULL;

    delete iOperation;
    iOperation = NULL;



    iLog->Log(_L("CloseTest Done"));
    }
    
// ---------------------------------------------------------
// CPosTp150::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KKMZFile );

    // Delete the trace file
    DeleteFileL(KFileTrace);


    HBufC8* emptyBuffer=NULL;
    
   	iLog->Log(_L("Test with Tp150KMZFile.kmz"));
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
        
        
    DoCancelTestL();

    if (iErrorsFound != KErrNone)
        {
    	iLog->Log(_L("Errors found in TP150"));
    	User::Leave(-1);
        }
	}

// ---------------------------------------------------------
// CPosTp150::DoCancelTestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::DoCancelTestL()
    {
    iLog->Log(_L("DoCancelTestL"));
    iLandmarkParser = CPosLandmarkParser::NewL(KKmzMimeType);
    iLandmarkParser->SetInputFileL(KKMZFile);

    iOperation = iLandmarkParser->ParseContentL();

    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;

    // 1 ) Cancel parse function by deleting iOperation
    delete iOperation;
    iOperation = NULL;

    TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 0)
    {
    	iLog->Log(_L("No landmarks should be found"));
    	User::Leave(KErrNone);
    }

    // No landmark should be found
    TRAPD(err, iLandmarkParser->LandmarkLC());
    if (err != KErrNotFound)
        {
        iLog->Log(_L("ERROR: No Landmark should be found"));
        iErrorsFound++;
        }

    // 1 ) Cancel parse function by changing input
    iOperation = iLandmarkParser->ParseContentL();

    // Get the first landmark
    iOperation->NextStep(status, progress);
    // Wait for NextStep to complete
    User::WaitForRequest(status);
    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 1)
    {
    	iLog->Log(_L("One landmark should be found"));
    	User::Leave(KErrNone);
    }

    // Change input
    iLandmarkParser->SetInputFileL(KKMZFile);

    nr = iLandmarkParser->NumOfParsedLandmarks();
    if ((TInt)nr != 0)
    {
    	iLog->Log(_L("No landmarks should be found"));
    	User::Leave(KErrNone);
    }

    // No landmark should be found
    TRAP(err, iLandmarkParser->LandmarkLC());
    if (err != KErrNotFound)
        {
        iLog->Log(_L("ERROR: No Landmark should be found"));
        iErrorsFound++;
        }

    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("DoCancelTestL Done"));
    }
    
 // ---------------------------------------------------------
// CPosTp150::PrintParsedDataFromEncoderL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::PrintParsedDataFromEncoderL(
    const TDesC& aFile, 
    const TDesC8&  aBuffer, 
    TInt aExpectedStatusCode,
    const TDesC8& aMimeType)
    {
    iLog->Log(_L("===== PrintParsedDataFromEncoderL ======="));
    
    iLandmarkParser = CPosLandmarkParser::NewL(aMimeType);

    if (aFile != KNullDesC)
        {
        TFileName file( aFile );
        iLog->Log(_L("FILE (%S)"), &file );
        TraceL(_L("----->Using FILE<-------"));
        TraceL(file);
        iLandmarkParser->SetInputFileL(file);
        }
    else
        {
        iLog->Log(_L("BUFFER (size %d)"), aBuffer.Size() );
        TraceL(_L("----->Using BUFFER<-------"));
        iLandmarkParser->SetInputBuffer(aBuffer);
        }

    iOperation = iLandmarkParser->ParseContentL();
    TRequestStatus status = KPosLmOperationNotComplete;
    TReal32 progress;
    TInt number = 0;

    RArray<TPosLmItemId> array;
    CleanupClosePushL(array);

    while (status == KPosLmOperationNotComplete)
        {
        iLog->Log(_L("--- Parsing ---------------------------"));
        TraceL(_L("------------------------------"));
        iOperation->NextStep(status, progress);

        // Wait for NextStep to complete
        User::WaitForRequest(status);
        if (status != KPosLmOperationNotComplete && status != KErrNone)
            {
            iLog->Log(_L("Parsing Complete"));
            
            HBufC* buffer = HBufC::NewLC( 128);
            TPtr buf = buffer->Des();
                       
            buf.Format(_L("\tStatus %d"), status.Int());
            iLog->Log(buf);
            TraceL(buf);
            
            CleanupStack::PopAndDestroy( buffer );
            buffer = NULL;              
            }
        else
            {
            iLog->Log(_L("Parsing element"));
            if ( iLandmarkParser->NumOfParsedLandmarks() )
                {
                // Get last parsed landmark
                CPosLandmark* lm = iLandmarkParser->LandmarkLC();
                TPtrC lmName;
                TPtrC catName;
                lm->GetLandmarkName(lmName);
                lm->GetCategoriesL(array);
                //iLog->Log(lmName);
                for (TInt i=0;i<array.Count();i++)
                    {
                    CPosLandmarkCategory* category = iLandmarkParser->LandmarkCategoryLC( array[i] );
                    category->GetCategoryName( catName );
                     
                    HBufC* buffer = HBufC::NewLC( 128 + catName.Length());
                    TPtr buf = buffer->Des();
                                            
                    if ( category->GlobalCategory()) 
                        {
                        buf.Append(_L("\tGlobal category: "));    
                        }
                    else 
                        {
                        buf.Append(_L("\tLocal category: "));    
                        }                        
                                        
                    buf.Append( catName );
                    iLog->Log( buf );
                    TraceL( buf );

                    CleanupStack::PopAndDestroy( buffer );
                    buffer = NULL;
                    CleanupStack::PopAndDestroy(category);
                    }
                    
               	PrintLandmarkFieldsWithDescriptionL(*lm, ETrue);	
               
                
                number++;
                CleanupStack::PopAndDestroy(lm);
                }
            }
        }

    iLog->Log(_L("--- Parsing end ---------------------------"));
    TraceL(_L("------------------------------"));
    CleanupStack::PopAndDestroy(&array);

    if (status.Int() != aExpectedStatusCode)
        {
        HBufC* buffer = HBufC::NewLC( 128 );
        TPtr buf = buffer->Des();
                    
        buf.Format(_L("\tERROR: Wrong status returned, was %d, should be %d"), status.Int(), aExpectedStatusCode);
        iLog->Log( buf );
        iErrorsFound++;
        
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;        
        }

   
   
   
     TUint32 nr = iLandmarkParser->NumOfParsedLandmarks();
    HBufC* buffer = HBufC::NewLC( 256 );
    TPtr buf = buffer->Des();
    buf.Format(_L("\tNr of Parsed landmarks %d, should be %d"), nr, number);
    iLog->Log(buf);
    TraceL(buf);

    CleanupStack::PopAndDestroy( buffer );
    buffer = NULL;
    if (nr != (TUint32)number)
        {
        iLog->Log(_L("\tERROR: Wrong number of landmarks parsed returned!"));
        iErrorsFound++;
        }

    TPosLmCollectionDataId lmId = iLandmarkParser->FirstCollectionDataId();

    iLog->Log(_L("--- CollectionData ---"));
    TraceL(_L("--- CollectionData ---"));

    if (lmId != EPosLmCollDataNone)
        {
        TPtrC first = iLandmarkParser->CollectionData(lmId);
        HBufC* buffer = HBufC::NewLC( first.Length() + 256 );
        TPtr buf = buffer->Des();
        buf.Zero();
        buf.Format(_L("\t(1)Collection Id: %d CollectionData: "), lmId);
        buf.Append(first);
        iLog->Log(buf);
        TraceL(buf);
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;      
        }
    else 
        {
        iLog->Log(_L("\tNo collection data found 1"));    
        }

    while (lmId != EPosLmCollDataNone)
        {
        lmId = iLandmarkParser->NextCollectionDataId(lmId);
        if (lmId != EPosLmCollDataNone)
        {
            TPtrC first = iLandmarkParser->CollectionData(lmId);
            
            HBufC* buffer = HBufC::NewLC( first.Length() + 256 );
            TPtr buf = buffer->Des();         
            buf.Zero();
            buf.Format(_L("\t(2)Collection Id: %d CollectionData: "), lmId);
            buf.Append(first);
            iLog->Log(buf);
            TraceL(buf);
            
            CleanupStack::PopAndDestroy( buffer );
            buffer = NULL;              
            }
        else 
            {
            iLog->Log(_L("\tNo collection data found 2"));   
            }
        }

    iLog->Log(_L("--- CollectionData ---"));
    TraceL(_L("--- CollectionData ---"));

    delete iOperation;
    iOperation = NULL;
    delete iLandmarkParser;
    iLandmarkParser = NULL;
    iLog->Log(_L("====== PrintParsedDataFromEncoderL Done ========\n"));
    }
    
    
 // ---------------------------------------------------------
// CPosTp150::PrintLandmarkFieldsWithDescriptionL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::PrintLandmarkFieldsWithDescriptionL(const CPosLandmark& aLandmark, TBool aTraceFlag)
    {
    iLog->Log(_L("Parsing Description ... \n"));
    TPositionFieldId sourceFieldId = aLandmark.FirstPositionFieldId();
    TLocality loc;

    TInt err;
    TPtrC landmarkName;
    TPtrC landmarkDescription;
    err = aLandmark.GetLandmarkName(landmarkName);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( landmarkName.Length() + 256 );
        TPtr buf = buffer->Des();
                    
        buf.Append( _L(" \tLandmark Name: "));
        buf.Append( landmarkName );
        iLog->Log( buf );
        if( aTraceFlag ) 
            {
            TraceL( buf );   
            }
            
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;  
        }

    err = aLandmark.GetPosition(loc);
    if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( 1024 );
        TPtr buf = buffer->Des();        
        TRealFormat format( 12, KRealFormatFixed );
        format.iPoint = TChar('.');
        format.iTriLen = KDoNotUseTriads;
        format.iPlaces = 6;
        
        TBuf<20> sLon, sLat, sVacc, sHacc, sAlt, sRad;
        sLon.Num( loc.Longitude(), format );
        sLat.Num( loc.Latitude(), format );
        sAlt.Num( loc.Altitude(), format );
        sVacc.Num( loc.VerticalAccuracy(), format );
        sHacc.Num( loc.HorizontalAccuracy(), format );
        buf.Format(
            _L("\tLM: Long %S Lat %S vertAcc %S horAcc %S alt %S "), 
            &sLon, &sLat, &sVacc, &sHacc, &sAlt);
        
        TReal32 sourceR;
        err = aLandmark.GetCoverageRadius(sourceR);
        if (err == KErrNone )
            {
            sRad.Num( sourceR, format );
            buf.AppendFormat(_L("srcRadius %S"), &sRad);
            }
        iLog->Log(buf);
        if (aTraceFlag) TraceL(buf);
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;  
        }
        
      //Get landmark description  
      err = aLandmark.GetLandmarkDescription(landmarkDescription);
      if (err == KErrNone)
        {
        HBufC* buffer = HBufC::NewLC( landmarkDescription.Length() + 256 );
        TPtr buf = buffer->Des();         
        buf.Format(_L("\tLandmark Description: "));
        buf.Append(landmarkDescription);
        iLog->Log(buf);
        if ( aTraceFlag ) 
            {
            TraceL( buf );    
            }
        
        CleanupStack::PopAndDestroy( buffer );
        buffer = NULL;         
        }  
    

    while (sourceFieldId != EPositionFieldNone)
        {
        TPtrC sourceValue;
        aLandmark.GetPositionField(sourceFieldId, sourceValue);
        
        HBufC* buffer = HBufC::NewLC( sourceValue.Length() + 256 );
        TPtr buf = buffer->Des();  
        buf.Format(_L("\tIdField Id: %d Value:"), sourceFieldId);
        buf.Append(sourceValue);
        iLog->Log(buf);
        if ( aTraceFlag ) 
            {
            TraceL( buf );
            }
        sourceFieldId = aLandmark.NextPositionFieldId(sourceFieldId);
        
        CleanupStack::PopAndDestroy( buffer );
        }
    }
    
    
// ---------------------------------------------------------
// CPosTp150::TraceL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp150::TraceL(const TDesC& msg)
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