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

#include "FT_CPosTp42.h"
#include <EPos_CPosLandmarkDatabase.h>
#include <bautils.h>

// CONSTANTS
_LIT(KTraceDir, "c:\\documents\\");
_LIT(KFileTrace, "c:\\documents\\TP42_File%d.xml");
_LIT(KFileTraceWild, "c:\\documents\\TP42_File*.xml");

_LIT( KDump, "%02d: '%S'" );


// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CPosTp42::CloseTest
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::CloseTest()
    {
    iDataToRecognize.ResetAndDestroy();
    }

// ---------------------------------------------------------
// CPosTp42::StartL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::StartL()
    {
    BaflUtils::EnsurePathExistsL( iFileSession, KFileTrace );
    
    iErrorsFound = 0;
    // Test reading from buffer
    iLog->Log(_L("**************BUFFER***************"));
    TestWithBufferL();
    
    iLog->Log(_L("**************FILE***************"));
    //Test reading from file and filehandle
    TestWithFileL();
    
    iLog->Log(_L("**************EMPTY FILE***************"));
    //Test reading from empty file
    TestWithEmptyFileL();
    
    iLog->Log(_L("**************LARGE FILE***************"));
    
    //Test reading from large file
    TestWithLargeFilesL(_L("<?xml *?><lala:lmx xmlns:n=\"http://www.nokia.com/schemas/location/landmarks\""), CApaDataRecognizerType::ECertain);
    TestWithLargeFilesL(_L("<?xml k?> < *:lmx*>"), CApaDataRecognizerType::EProbable);
    
    iDataToRecognize.ResetAndDestroy();

    if (iErrorsFound != KErrNone)
        {
        TBuf<64> buf;
        buf.Format(_L("%d Errors were found in TP42"), iErrorsFound );
        iLog->Log(buf);
        User::Leave(-1);
        }
    }
    
    
// ---------++++++++++++++------------------------------------------------
// CPosTp42::TestWithBufferL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::TestWithEmptyFileL()
    {
    // Create empty file
    RFile file;
    TBuf<100> buf;
    _LIT(KFileName, "c:\\documents\\DummyFile.txt");
    
    TInt err = file.Open(iFileSession, KFileName , EFileWrite);
    if (err != KErrNone && err != KErrAlreadyExists)
        {
        if (err == KErrNotFound)
            {
            User::LeaveIfError(file.Create(iFileSession, KFileName, EFileWrite));
            }
        else
           {   
            buf.Format(_L("Error %d when creating empty file"), err);
            iLog->Log(buf);
           }
        }
        
    file.Close();
    TBuf8<1> kalle;
    
    RApaLsSession lsSession;
    CleanupClosePushL(lsSession);
    User::LeaveIfError(lsSession.Connect());
    TDataRecognitionResult recognizerResultFromFile;
    TDataRecognitionResult recognizerResultFromFileHandle;
    
    // Check file
    err = lsSession.RecognizeData(KFileName, kalle , recognizerResultFromFile);
    if (err != KErrNone) 
    {
    	iLog->Log(_L("Error from lsSession.RecognizeData"));
    	User::Leave(err);
    }
    iLog->Log(recognizerResultFromFile.iDataType.Des());
    TInt expCode = CApaDataRecognizerType::EProbable;
    
    if (recognizerResultFromFile.iDataType.Des() != _L("text/plain"))
        {
       
        iLog->Log(_L("datatype from recognizer"));
        }
        
    if (recognizerResultFromFile.iConfidence != expCode)
        {
      
      	iLog->Log(_L(" confidence from recognizer"));
        }
    
    // Check filehandle
	User::LeaveIfError(iFileSession.ShareProtected());

    RFile fileHandle;
    CleanupClosePushL(fileHandle);
    err = fileHandle.Open(iFileSession, KFileName, EFileRead | EFileShareAny);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error from file.Open"));
    	User::Leave(err);

    }
    err = lsSession.RecognizeData(fileHandle, recognizerResultFromFileHandle);
    if (err != KErrNone)
    {
    	iLog->Log(_L("Error from lsSession.RecognizeData"));
    	User::Leave(err);

    }
    CleanupStack::PopAndDestroy(&fileHandle);
    iLog->Log(recognizerResultFromFileHandle.iDataType.Des());
    
   if (recognizerResultFromFileHandle.iDataType.Des() != _L("text/plain"))
        {
         iLog->Log(_L(" datatype from recognizer"));
        }
        
    if (recognizerResultFromFileHandle.iConfidence != expCode)
        {
         iLog->Log(_L("confidence from recognizer"));
        }
    
    CleanupStack::PopAndDestroy(&lsSession);
    
    }

// ---------------------------------------------------------
// CPosTp42::TestWithBufferL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::TestWithBufferL()
    { 
    //start reading the file line by line and create objects.
    _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmRecognizerData.txt");

    RFile file;
    User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));
    CleanupClosePushL(file);
        
    TBuf<500> line;    //declarera som en konstant tack
    while (ReadLn(file, line) != KErrEof)
        {
        TLex lex(line);
        if ( lex.Peek() != '#' )
            {
            CRecognizerData* recognizerData = CRecognizerData::NewL(line);
            CleanupStack::PushL(recognizerData);

            User::LeaveIfError(iDataToRecognize.Append(recognizerData));
            CleanupStack::Pop(recognizerData);
            }
        }

	CleanupStack::PopAndDestroy(&file);

    RApaLsSession lsSession;
    CleanupClosePushL(lsSession);
    User::LeaveIfError(lsSession.Connect());
    TDataRecognitionResult recognizerResult;
    TBuf<10> dummy; //dummy
    
    TInt numberOfResults = iDataToRecognize.Count();
    for (TInt i = 0; i < numberOfResults; i++)
        {
        CRecognizerData* data = iDataToRecognize[i];
        TBuf8<200> kalle;
        kalle.Copy((data -> RecognizerData()));
        
        // Check buffer
        User::LeaveIfError(lsSession.RecognizeData(dummy, kalle , recognizerResult));
        
        const TDataRecognitionResult expected = data->ExpectedResult();

        TBuf<512> buf;
        TPtrC content( data->RecognizerData() );
        buf.Format( KDump, i+1, &content );
        iLog->Log( buf );

        CheckResultL(expected, recognizerResult);
        }
        
	CleanupStack::PopAndDestroy(&lsSession);  
    iDataToRecognize.ResetAndDestroy();
	}

// ---------------------------------------------------------
// CPosTp42::TestWithFileL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::TestWithFileL()
    { 
    //start reading the file line by line and create objects.
    _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmRecognizerDataFile.txt");

    RFile file;
    
    CFileMan* fileMan = CFileMan::NewL(iFileSession);
    CleanupStack::PushL(fileMan);
    // Delete all old files
    TInt err = fileMan->Delete(KFileTraceWild);
    if (err != KErrNone && err != KErrNotFound) iLog->Log(_L("Error from CFileMan::Delete"));
    CleanupStack::PopAndDestroy(fileMan);

    CleanupClosePushL(file);
    User::LeaveIfError(file.Open(iFileSession, KFileTestValues,  EFileRead));
        
    TBuf<500> line;
    while (ReadLn(file, line) != KErrEof)
        {
        TLex lex(line);
        if (lex.Peek() != '#')
            {
            CRecognizerData* recognizerData = CRecognizerData::NewL(line);
            CleanupStack::PushL(recognizerData);

            User::LeaveIfError(iDataToRecognize.Append(recognizerData));
            CleanupStack::Pop(recognizerData);
            }
        }

    CleanupStack::PopAndDestroy(&file);

    //start comparing
    RApaLsSession lsSession;
    CleanupClosePushL(lsSession);
    User::LeaveIfError(lsSession.Connect());
    TDataRecognitionResult recognizerResultFromFile;
    TDataRecognitionResult recognizerResultFromFileHandle;
    TBuf<40> fileName; //dummy

    TInt numberOfResults = iDataToRecognize.Count();
    for (TInt i = 0; i < numberOfResults; i++)
        {
        CRecognizerData* data = iDataToRecognize[i];
       
        TBuf8<512> buf8;
        buf8.Copy( data->RecognizerData() );

        RFile file;
        TFileName fileName;
        TInt err = file.Temp( iFileSession, KTraceDir, fileName, EFileWrite );
        if ( err )
            {
            iLog->Log(_L("Failed to create temp file, %d"), err);
            User::Leave( err );
            }
        file.Write( buf8 );
        _LIT8( KCrLf, "\n" );
        file.Write( KCrLf );
        file.Close();
        
        const TDataRecognitionResult expected = data->ExpectedResult();

        TBuf<512> buf;
        TPtrC content( data->RecognizerData() );
        buf.Format( KDump, i+1, &content );
        iLog->Log( buf );
        
        // Check file
        recognizerResultFromFile.Reset();
        buf8.Zero();
        lsSession.RecognizeData( fileName, buf8, recognizerResultFromFile );
        CheckResultL(expected, recognizerResultFromFile);
        
        // Check FileHandle
        
        RFs fileSessionNew;
		User::LeaveIfError(fileSessionNew.Connect());
		CleanupClosePushL(fileSessionNew);

        //iLog->Log(_L("Check filehandle"));
		err = fileSessionNew.ShareProtected();
    	if (err != KErrNone) 
    	{
    		iLog->Log(_L("Error from file.ShareProtected"));
    		User::Leave(err);
    	}

        RFile fileHandle;
        CleanupClosePushL(fileHandle);
        err = fileHandle.Open(fileSessionNew, fileName, EFileRead | EFileShareAny);
        if (err != KErrNone) 
        {
        	iLog->Log(_L("Error from file.open"));
    		User::Leave(err);
        }
        err = lsSession.RecognizeData(fileHandle, recognizerResultFromFileHandle);
        if (err != KErrNone) 
        {
        	iLog->Log(_L("Error from lsSession.RecognizeData"));
    		User::Leave(err);
        }
        
        CleanupStack::PopAndDestroy(&fileHandle);
        CleanupStack::PopAndDestroy(&fileSessionNew);
        CheckResultL(expected, recognizerResultFromFileHandle);
        
        iFileSession.Delete( fileName );
        }

    CleanupStack::PopAndDestroy(&lsSession);
    
    iDataToRecognize.ResetAndDestroy();
	}
	
// ---------------------------------------------------------
// CPosTp42::TestWithLargeFilesL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CPosTp42::TestWithLargeFilesL(const TDesC& msg, TInt aExpCode)
    {
    RApaLsSession lsSession;
    CleanupClosePushL(lsSession);
    User::LeaveIfError(lsSession.Connect());
    
    TDataRecognitionResult recognizerResultFromFile;
    TDataRecognitionResult recognizerResultFromFileHandle;
    
    const TInt KBufSize = 400;
    TBuf8<KBufSize> tmp1;
    TBuf8<KBufSize> tmp;
    TBuf8<1> kalle;

    _LIT(KEnd,"\n");

    RFile file;
    TFileName fileName;
    file.Temp( iFileSession, KTraceDir, fileName, EFileWrite );
    CleanupClosePushL(file);

    for (TInt i=0;i<10;i++)
        {
        tmp1.Append(_L("   \n\t   "));
        }
        
    tmp.Append(tmp1);
    tmp.Append(msg);
    tmp.Append(KEnd);
    tmp.Append(tmp1);
    tmp.Append(KEnd);
    file.Write(tmp);
    
    CleanupStack::PopAndDestroy(&file);
    
    lsSession.RecognizeData(fileName, kalle , recognizerResultFromFile);
    iLog->Log(recognizerResultFromFile.iDataType.Des());

    if (recognizerResultFromFile.iDataType.Des() != _L("application/vnd.nokia.landmarkcollection+xml"))
        {
        iLog->Log(_L("datatype from recognizer"));
        }
    
    if (recognizerResultFromFile.iConfidence != aExpCode)
        {
        iLog->Log(_L(" confidence from recognizer"));
        }
    
    // Check FileHandle
    iLog->Log(_L("Check filehandle"));
    
    RFs fs;
    CleanupClosePushL(fs);
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());

    RFile fileHandle;
    CleanupClosePushL(fileHandle);
    TInt err = fileHandle.Open(fs, fileName, EFileRead | EFileShareAny);
    if (err != KErrNone)
    {
    iLog->Log(_L("Error from file.Open"));
   	User::Leave(err);	
    }
    err = lsSession.RecognizeData(fileHandle, recognizerResultFromFileHandle);
    if (err != KErrNone)
    {
    iLog->Log(_L("Error from lsSession.RecognizeData"));
   	User::Leave(err);	
    }
    CleanupStack::PopAndDestroy(&fileHandle);
    
    if (recognizerResultFromFileHandle.iDataType.Des() != _L("application/vnd.nokia.landmarkcollection+xml"))
        {
        iLog->Log(_L("datatype from recognizer"));
        }
        
    if (recognizerResultFromFileHandle.iConfidence != aExpCode)
        {
        iLog->Log(_L("confidence from recognizer"));
        }
    
    iFileSession.Delete( fileName );
    CleanupStack::PopAndDestroy(&fs);
    CleanupStack::PopAndDestroy(&lsSession);
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CPosTp42::CheckResultL(TDataRecognitionResult aExpectedResult, TDataRecognitionResult aResult)
    {
    _LIT(KExpected, "Expect: '%S' (%d)");
    _LIT(KActual,   "Actual: '%S' (%d)");

    TBuf<256> exp, act;
    TPtrC expMime( aExpectedResult.iDataType.Des() );
    TPtrC actMime( aResult.iDataType.Des() );

    exp.Format(KExpected, &expMime, aExpectedResult.iConfidence );
    act.Format(KActual, &actMime, aResult.iConfidence );

    if ( aExpectedResult.iConfidence == NOLANDMARKNUMBER ) // Magic number for not any landmark
        {
        _LIT8(KLmxType, "application/vnd.nokia.landmarkcollection+xml");
        _LIT8(KGpxType, "application/gps+xml");

        if ( aResult.iDataType.Des8().Compare( KLmxType ) == 0 ||
             aResult.iDataType.Des8().Compare( KGpxType ) == 0 )
            {
            iLog->Log(_L( "ERR: landmark detected when it shouldn't be" ));
            iLog->Log(exp);
            iLog->Log(act);
            iErrorsFound++;
            }
        }
    else //if (aExpectedResult.iConfidence == CApaDataRecognizerType::ECertain) //a landmark buffer is expected
        {
        if ( aExpectedResult.iDataType != aResult.iDataType )
            {
            iLog->Log(_L( "ERR: datatype mismatch" ));
            iLog->Log(exp);
            iLog->Log(act);
            iErrorsFound++;
            }
        if ( aExpectedResult.iConfidence != aResult.iConfidence )
            {
            iLog->Log(_L("ERR: confidence mismatch"));
            iLog->Log(exp);
            iLog->Log(act);
            iErrorsFound++;
            }
        }
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
//  Implementation of the supporter class
CRecognizerData* CRecognizerData::NewL(const TDesC& aDes)
    {
    CRecognizerData* self = new(ELeave) CRecognizerData;
    CleanupStack::PushL(self);
    self->ConstructL(aDes);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
CRecognizerData::CRecognizerData()
    {
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
CRecognizerData::~CRecognizerData()
    {
    delete iRecognizerData;
    iRecognizerData = NULL;
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
TPtrC CRecognizerData::RecognizerData() const
    {
    return *iRecognizerData;
    }
    
// ---------------------------------------------------------
// ---------------------------------------------------------
//
const TDataRecognitionResult& CRecognizerData::ExpectedResult() const
    {
    return iExpectedResult;
    }
  
// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CRecognizerData::ConstructL(const TDesC& aDes)
    {
    _LIT( KDelim, "#" );
    TInt delim = aDes.Find( KDelim );
    iRecognizerData = aDes.Left( delim ).AllocL();
    iRecognizerData->Des().TrimRight();
    
    TLex line( aDes.Mid( delim + 1 ) );
    
    ParseResult( line.NextToken() );
    ParseType( line.NextToken() );
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CRecognizerData::ParseResult(const TDesC& aDes)
    {
    _LIT(KECertain, "ECertain");
    _LIT(KEProbable, "EProbable");
    if (aDes.CompareF(KECertain) == 0)
        {
        iExpectedResult.iConfidence = CApaDataRecognizerType::ECertain;
        }
    else if (aDes.CompareF(KEProbable) == 0)
        {
        iExpectedResult.iConfidence = CApaDataRecognizerType::EProbable;
        }
    else
        {
        iExpectedResult.iConfidence = NOLANDMARKNUMBER;
        }
    }

// ---------------------------------------------------------
// ---------------------------------------------------------
//
void CRecognizerData::ParseType(const TDesC& aDes)
    {
    _LIT8(KLmxType, "application/vnd.nokia.landmarkcollection+xml");
    _LIT8(KGpxType, "application/gps+xml");
    _LIT(KGpx, "gpx");
    _LIT(KLmx, "lmx");
    
    TDataType dataLmx(KLmxType);
    TDataType dataGpx(KGpxType);

    if (aDes.CompareF(KGpx) == 0)
        {
        iExpectedResult.iDataType = dataGpx;
        }
    else if (aDes.CompareF(KLmx) == 0)
        {
        iExpectedResult.iDataType = dataLmx;
        }
    else
        {
        }
    }

//  End of File
