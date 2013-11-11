/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: harvestermetadataextractortest block implementation for STIF Test Framework TestScripter
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "harvestermetadataextractortest.h"
#include <f32file.h>

#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxharvesterutility.h>
#include <mpxcollectionutility.h>



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void Charvestermetadataextractortest::Delete() 
    {
    
    }

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt Charvestermetadataextractortest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", Charvestermetadataextractortest::ExampleL ),
        ENTRY( "CMPXMetadataExtractorCreateInstance", Charvestermetadataextractortest::CMPXMetadataExtractorCreateInstance ),
        ENTRY( "CMPXMetadataExtractorCreateMediaL", Charvestermetadataextractortest::CMPXMetadataExtractorCreateMediaL ),
        ENTRY( "CMPXMetadataExtractorExtractAlbumArtL", Charvestermetadataextractortest::CMPXMetadataExtractorExtractAlbumArtL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleCollectionMediaL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleCollectionMediaL( const CMPXMedia& /*aMedia*/, TInt aError )
	{
	iLog->Log(_L("Charvestermetadataextractortest::HandleCollectionMediaL: %d"), aError);
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleCollectionMessage
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleCollectionMessage( CMPXMessage* /*aMsg*/, TInt aError )
	{
	iLog->Log(_L("Charvestermetadataextractortest::HandleCollectionMessage: %d"), aError);
	}
	
// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleOpenL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleOpenL( const CMPXMedia& /*aEntries*/, TInt /*aIndex*/, TBool /*aComplete*/, TInt aError )
	{
	iLog->Log(_L("Charvestermetadataextractortest::HandleOpenL: %d"), aError);
	}
// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleOpenL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleOpenL( const CMPXCollectionPlaylist& /*aPlaylist*/, TInt aError ) 
	{
	iLog->Log(_L("Charvestermetadataextractortest::HandleOpenL: %d"), aError);
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandlePlaylistExportCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandlePlaylistExportCompletedL( CMPXMedia* /*aMedia*/, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandlePlaylistExportCompletedL) )
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandlePlaylistExportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandlePlaylistExportCompletedL: %d"), aErr);
		}
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandlePlaylistImportCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandlePlaylistImportCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileImportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileImportCompletedL: %d"), aErr);
		}

	if ( aErr == KErrNone )
		{
		iLog->Log(_L("Number of attributes: %d"), aMedia->Count() );
		delete aMedia;
		aMedia = NULL;
		}
	if ( (iResult && (aErr == KErrNone )) || (!iResult && (aErr != KErrNone)) )
		{
		Signal();
		}
	else
		{
		Signal(aErr);
		}
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandleFileAddCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileAddCompletedL) )
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandleFileAddCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandleFileAddCompletedL: %d"), aErr);
		}

	if ( aErr == KErrNone )
		{
		iLog->Log(_L("Number of attributes: %d"), aMedia->Count() );
		delete aMedia;
		aMedia = NULL;
		}
	Signal();
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandleDeleteCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleDeleteCompletedL( TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleDeleteCompletedL) )
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandleDeleteCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Charvestermetadataextractortest::HandleDeleteCompletedL: %d"), aErr);
		}

	if ( aErr == KErrNone )
		{
		Signal();
		}
	else
		{
		Signal(aErr);
		}
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandleFileImportCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileImportCompletedL) )
		{
		iLog->Log(_L("Charvestermetadataextractortest::EEventHandleFileImportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Charvestermetadataextractortest::EEventHandleFileImportCompletedL: %d"), aErr);
		}

	if ( aErr == KErrNone )
		{
		iLog->Log(_L("Number of attributes: %d"), aMedia->Count() );
		delete aMedia;
		aMedia = NULL;
		}
	if ( (iResult && (aErr == KErrNone )) || (!iResult && (aErr != KErrNone)) )
		{
		Signal();
		}
	else
		{
		Signal(aErr);
		}
	}

// ---------------------------------------------------------------------------
// Charvestermetadataextractortest::HandleFileImportCompletedL
// ---------------------------------------------------------------------------
//
void Charvestermetadataextractortest::HandleFileGetMediaCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileGetMediaCompletedL) )
			{
			iLog->Log(_L("Charvestermetadataextractortest::HandleFileGetMediaCompletedL: %d [EXPECTED]"), aErr);
			}
		else
			{
			iLog->Log(_L("Charvestermetadataextractortest::HandleFileGetMediaCompletedL: %d"), aErr);
			}

		if ( aErr == KErrNone )
			{
			iLog->Log(_L("Number of attributes: %d"), aMedia->Count() );
			delete aMedia;
			aMedia = NULL;
			}
		Signal();
		
	}


// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::RemoveExpectedEvent
// Remove the event from the expected event list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool Charvestermetadataextractortest::RemoveExpectedEvent( TExpectedEvents aEvent )
	{
	iLog->Log(_L("Charvestermetadataextractortest::RemoveExpectedEvent: %d"), aEvent);
	TBool match = EFalse;
	TInt index = iExpectedEvents.Find(aEvent);
	if ( index != KErrNotFound )
		{
		iExpectedEvents.Remove(index);
		match = ETrue;
		}
	return match;
	}

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Charvestermetadataextractortest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kharvestermetadataextractortest, "harvestermetadataextractortest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kharvestermetadataextractortest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kharvestermetadataextractortest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::CMPXMetadataExtractorCreateInstance
// CMPXMetadataExtractor::NewL function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Charvestermetadataextractortest::CMPXMetadataExtractorCreateInstance( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Charvestermetadataextractortest::CMPXMetadataExtractorCreateInstance"));
    TInt err = KErrNone;
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    RApaLsSession appArcSession;
    User::LeaveIfError(appArcSession.Connect());   
    RPointerArray<CMPXCollectionType> dummy;
    CleanupClosePushL( dummy );
    
    TRAP(err, iMetadataExtractor = CMPXMetadataExtractor::NewL( fs, appArcSession, dummy ));
    dummy.ResetAndDestroy();
    CleanupStack::PopAndDestroy( &dummy );
    fs.Close();
    appArcSession.Close();    
 
    // delete iMetadataExtractor;
    if ( iMetadataExtractor != NULL )
    	{
    	delete iMetadataExtractor;
    	iMetadataExtractor = NULL;
    	}
    
	if ( err != KErrNone )
   		{
   		iLog->Log(_L("Error: CMPXMetadataExtractor::CMPXMetadataExtractorCreateInstance returned: %d"), err);
   		}
    return err; 
    }
// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::CMPXMetadataExtractorCreateMediaL
// CMPXMetadataExtractor::CreateMediaL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Charvestermetadataextractortest::CMPXMetadataExtractorCreateMediaL( CStifItemParser& aItem )
    {
    iLog->Log(_L("Charvestermetadataextractortest::CreateMediaL"));
  
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
       {
		TBuf<120> KFile;
		KFile.Append(KmpxMetadataExtractor_digitalPath);
		KFile.Append(string);
	
		CMPXMedia* media = NULL;
		RFile file;
		RFs fs;
		User::LeaveIfError(fs.Connect());
		file.Open(fs,KFile,EFileStreamText|EFileShareAny);  

	    RApaLsSession appArcSession;
	    User::LeaveIfError(appArcSession.Connect());  
	    
	   
	    CMPXCollectionType* collectionType;
	    collectionType=new(ELeave) CMPXCollectionType();
	    	          
	    RPointerArray <CMPXCollectionType> mpxcollectiontypetest;
	    mpxcollectiontypetest.AppendL(collectionType);
	 
	    iMetadataExtractor = CMPXMetadataExtractor::NewL( fs, appArcSession, mpxcollectiontypetest );
	
	    TRAP(err,iMetadataExtractor->CreateMediaL( KFile, media, ETrue ));
		
	    file.Close();  
		fs.Close();
	    appArcSession.Close();
	    
	    if ( iMetadataExtractor != NULL )
	    	{
	    	delete iMetadataExtractor;
	    	iMetadataExtractor = NULL;
	    	}
	    if ( media != NULL )
	    	{
	    	delete media;
	    	media = NULL;
	    	}
		if ( err != KErrNone )
	   		{
	   		iLog->Log(_L("Error: CreateMediaL returned: %d"), err);
	   		}		    
	   }
    return err;
    }


// -----------------------------------------------------------------------------
// Charvestermetadataextractortest::CMPXMetadataExtractorExtractAlbumArtL
// CMPXMetadataExtractor::NewL function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt Charvestermetadataextractortest::CMPXMetadataExtractorExtractAlbumArtL( CStifItemParser& aItem )
    {
    iLog->Log(_L("Charvestermetadataextractortest::CMPXMetadataExtractorExtractAlbumArtL"));
    TInt err = KErrNone;
    TPtrC string;
    RFs fs;
	if ( KErrNone == aItem.GetNextString(string) )
	 {
		TBuf<120> uri;
		uri.Append(KmpxMetadataExtractor_digitalPath);
		uri.Append(string);
	    
	    User::LeaveIfError( fs.Connect() );
	    RApaLsSession apaLsSession;
	    User::LeaveIfError(apaLsSession.Connect());   
	    RPointerArray<CMPXCollectionType> pointArray;
	    CleanupClosePushL( pointArray );
	    
	    TRAP(err, iMetadataExtractor = CMPXMetadataExtractor::NewL( fs, apaLsSession, pointArray ));
	    CMPXMedia* media=CMPXMedia::NewL(); 
	    CleanupStack::PushL(media);
	    media->SetTextValueL(KMPXMediaGeneralUri,uri);
//	    media->SetTextValueL(KMPXMediaGeneralMimeType,_L("image/jpeg"));
	    TRAP( err , iMetadataExtractor->ExtractAlbumArtL(media) );
		CleanupStack::PopAndDestroy(media);
		pointArray.ResetAndDestroy();
	    CleanupStack::PopAndDestroy( &pointArray );
	    fs.Close();
	    apaLsSession.Close();    
	    
	    // delete iMetadataExtractor;
	    if ( iMetadataExtractor != NULL )
	    	{
	    	delete iMetadataExtractor;
	    	iMetadataExtractor = NULL;
	    	}
	    
		if ( err != KErrNone )
	   		{
	   		iLog->Log(_L("Error: CMPXMetadataExtractor::CMPXMetadataExtractorExtractAlbumArtL returned: %d"), err);
	   		}
	}
    return err; 
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
