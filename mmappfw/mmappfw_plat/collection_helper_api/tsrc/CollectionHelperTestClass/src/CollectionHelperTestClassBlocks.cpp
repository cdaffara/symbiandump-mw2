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
* Description:   CCollectionHelperTestClass block implementaion for STIF Test Framework TestScripter.
*
*/


// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "CollectionHelperTestClass.h"
#include <mpxmessagegeneraldefs.h> // KMPXMessageGeneralEvent, KMPXMessageGeneralType
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxattribute.h>
#include "debug.h"
#include <mpxmediamtpdefs.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCollectionHelperTestClass::Delete() 
    {
    FTRACE(FPrint(_L("CTunerUtilityTestClass::Delete")));
    
    iExpectedEvents.Close();
    iOcurredEvents.Close();
    
    iLog->Log(_L("Deleting test class..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));
    
    if(iUIHelper)
   		{
        //delete iUIHelper;
        iUIHelper = NULL;
 	  	}
    if(iHelper)
   		{
        //delete iHelper;
        iHelper = NULL;
   		}
    if(iCachedHelper)
   		{
        //delete iCachedHelper;
        iCachedHelper = NULL;
   		}
    }

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCollectionHelperTestClass::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {        
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "OpenCollectionUtility", CCollectionHelperTestClass::OpenCollectionUtilityL ),
        ENTRY( "NewUiHelper", CCollectionHelperTestClass::NewUiHelperL ),
        ENTRY( "NewHelper", CCollectionHelperTestClass::NewHelperL ),
        ENTRY( "NewCachedHelper", CCollectionHelperTestClass::NewCachedHelperL ),
        ENTRY( "AddSong", CCollectionHelperTestClass::AddSongL ),
        ENTRY( "RemoveSong", CCollectionHelperTestClass::RemoveSongL ),
        ENTRY( "AddAbstractAlbum", CCollectionHelperTestClass::AddAbstractAlbumL ),
        ENTRY( "RemoveAbstractAlbum", CCollectionHelperTestClass::RemoveAbstractAlbumL ),
        ENTRY( "SetAbstractAlbum", CCollectionHelperTestClass::SetAbstractAlbumL ),
        ENTRY( "SetSongAlbumArtist", CCollectionHelperTestClass::SetSongAlbumArtistL ),
        ENTRY( "GetSongAlbumArtist", CCollectionHelperTestClass::GetSongAlbumArtistL ),
        ENTRY( "SetAbstractAlbumArtist", CCollectionHelperTestClass::SetAbstractAlbumArtistL ),
        ENTRY( "GetAbstractAlbumArtist", CCollectionHelperTestClass::GetAbstractAlbumArtistL ),
        ENTRY( "UpdateAbstractAlbumName", CCollectionHelperTestClass::UpdateAbstractAlbumNameL ),
        ENTRY("GetAbstractAlbumAndSongs",CCollectionHelperTestClass::GetAbstractAlbumAndSongsL),
        ENTRY( "RenameAbstractAlbum", CCollectionHelperTestClass::RenameAbstractAlbumL ),
        ENTRY( "CloseUiHelper", CCollectionHelperTestClass::CloseUiHelperL ),
        ENTRY( "CloseHelper", CCollectionHelperTestClass::CloseHelperL ),
        ENTRY( "CloseCachedHelper", CCollectionHelperTestClass::CloseCachedHelperL ),        
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    }


// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::EventName
// Return descriptor with the notification description
// -----------------------------------------------------------------------------
TPtrC CCollectionHelperTestClass::EventName( TInt aKey )
{
	static TText* const badKeyword = (TText*)L"BadKeyword";
	static TText* const keywords[] =
	{
		(TText*)L"EHandleCollectionMessage",
		(TText*)L"EHandleOpen",
		(TText*)L"EHandleAddFileComplete",
		(TText*)L"EHandleRemove",
		(TText*)L"EHandleFindAllComplete"
	};

	if( (TUint)aKey >= (sizeof( keywords )/sizeof(TText*)) )
	{
		iLog->Log(_L("Keyword out of bounds"));
		TPtrC keyword( badKeyword );
		return keyword;
	}
	else
	{
		TPtrC keyword( keywords[aKey] );
		return keyword;
	}
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::AddExpectedEvent
// Add an event to the expected events' list
// -----------------------------------------------------------------------------
void CCollectionHelperTestClass::AddExpectedEvent(TCollectionHelperExpectedEvent event, TInt ms)
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::AddExpectedEvent")));
	iExpectedEvents.Append(event);
	TPtrC eventName = EventName(event);
	iLog->Log(_L("Adding expected event:(0x%02x)%S Total=%d"), event, &eventName, iExpectedEvents.Count() );

	if ( iTimeoutController && !iTimeoutController->IsActive() )
	{
		if (ms > 0)
		{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(ms * 1000) );
		}
		else
		{
			iLog->Log(_L("Timeout with default value (1s)"));
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
		}
	}
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::RemoveExpectedEvent
// Remove the indicated event from the expected events' list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool CCollectionHelperTestClass::RemoveExpectedEvent(TCollectionHelperExpectedEvent aEvent)
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::RemoveExpectedEvent")));
	TBool match = EFalse;
	for (TUint i=0; i < iExpectedEvents.Count() ; i++)
	{
		if (iExpectedEvents[i] == aEvent)
		{
			iExpectedEvents.Remove(i);
			match = ETrue;
			break;
		}
	}

	return match;
}


// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::RemoveAllExpectedEvents
// Remove the indicated event from the expected events' list
// -----------------------------------------------------------------------------
void CCollectionHelperTestClass::RemoveAllExpectedEvents()
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::RemoveAllExpectedEvents")));
	iLog->Log(_L("Removing all expected events"));

	iExpectedEvents.Reset();
	iOcurredEvents.Reset();
}


// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::ProcessEvent
// Process events.
// -----------------------------------------------------------------------------
void CCollectionHelperTestClass::ProcessEvent(TCollectionHelperExpectedEvent aEvent, TInt aError)
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::ProcessExpectedEvent")));
	TPtrC nameEvent = EventName(aEvent);

	// Check for error
	if (aError == KErrNone)
	{
		// Remove the event
		if (RemoveExpectedEvent(aEvent))
		{
			iLog->Log(_L("Expected Event: (0x%02x)%S has ocurred Total=%d"), aEvent, &nameEvent,iExpectedEvents.Count());
		}
		else
		{
			iLog->Log(_L("Event: (0x%02x)%S has ocurred"), aEvent, &nameEvent);
			return;
		}

		// All expected events have ocurred
		if (iExpectedEvents.Count() == 0 )
		{
			Signal();
			iTimeoutController->Cancel();
		}
	}
	else
	{
		iLog->Log(_L("[Error] Event: (0x%02x)%S return with error code=%d"), aEvent, &nameEvent, aError);
		if (iExpectedEvents.Count() != 0 )
		{
			RemoveExpectedEvent(aEvent);
		}
		iTimeoutController->Cancel();
		Signal(KErrCallbackErrorCode);
	}
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::HandleTimeout
// Review if all the expected events have ocurred once the time is over
// -----------------------------------------------------------------------------
void CCollectionHelperTestClass::HandleTimeout(TInt aError)
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::HandleTimeout")));
	// All expected events have ocurred
	if (aError != KErrNone)
	{
		if (iExpectedEvents.Count() == 0 )
		{
			iLog->Log(_L("Timing out but events have ocurred"));
			Signal();
		}
		else
		{
			RemoveAllExpectedEvents();
			iLog->Log(_L("Timing out and events still pending"));
			Signal(KErrEventPending);
		}
	}
	else
	{
		iLog->Log(_L("Timing out return a error %d"), aError);
		Signal(aError);
	}
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::SetTimeout
// Create a timer and set a timeout
// When the timeout is reached the test case is marked as failed
// It's used rather than the "timeout" keyword in the configuration file
// because in this way the log continues
// -----------------------------------------------------------------------------
TInt CCollectionHelperTestClass::SetTimeout( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::SetTimeout")));
	TInt timeout=0;
	TInt error = aItem.GetNextInt(timeout) ;
	if ( iTimeoutController )
	{
		if ( timeout > 0 )
		{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(timeout*1000) );
		}
		else
		{
			iTimeoutController->Start( TTimeIntervalMicroSeconds(1000000) );
		}
	}
	else
	{
		iLog->Log(_L("Timeout Controller doesn't exist"));
		error = KErrTimeoutController;
	}
	return error;
}


// -----------------------------------------------------------------------------
// Uses the TestModuleBase API to allow a panic as exit reason for a test case
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCollectionHelperTestClass::SetAllowedPanic( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::SetAllowedPanic")));
	TInt error = KErrNone;
	TInt panicCode;
	TPtrC panicType;
	if (  ( KErrNone == aItem.GetNextString(panicType) ) &&
	( KErrNone == aItem.GetNextInt(panicCode) )  )
	{
		iLog->Log(_L("Allowing panic: %S %d"), &panicType, panicCode);
		iTestModuleIf.SetExitReason( CTestModuleIf::EPanic, panicCode );
		iNormalExitReason = EFalse;
	}
	else
	{
		iLog->Log(KMsgBadTestParameters);
		error = KErrBadTestParameter;
	}
	return error;
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::SetExpectedEvents()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCollectionHelperTestClass::SetExpectedEvents( CStifItemParser& aItem )
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::SetExpectedEvents")));
	TInt error = KErrNone;
	TInt event=0;
	while ( KErrNone == aItem.GetNextInt(event))
	{
		AddExpectedEvent(static_cast<TCollectionHelperExpectedEvent>(event), 0); // Default timeout value
	}
	return error;
}
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::CreateTestMediaL()
// Returns: no return value.
// -----------------------------------------------------------------------------
void CCollectionHelperTestClass::CreateTestMediaL( CMPXMedia*& aNewProperty )
    {
    RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic );
    contentIDs.AppendL( KMPXMediaIdDrm ); 
            
    aNewProperty = NULL;
    CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( media );
    contentIDs.Close();
    
    // CMPXMedia default types    
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, 
                                              EMPXItem );
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, 
                                                  EMPXSong );
    media->SetTextValueL( KMPXMediaGeneralUri, KTestFile() );
    
    // Title, default is file name
    media->SetTextValueL( KMPXMediaGeneralTitle, KTestFile() );
    
    // Default album track
    media->SetTextValueL( KMPXMediaMusicAlbumTrack, KNullDesC );
    
    // Set the Mime Type and collection UID
    media->SetTextValueL( KMPXMediaGeneralMimeType, KTestFileMimeType() );
    //media->SetTObjectValueL( KMPXMediaGeneralCollectionId, );

    // Set the pointers now that the object is ready
    CleanupStack::Pop( media );
    aNewProperty = media;
    }

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCollectionHelperTestClass::ExampleL( CStifItemParser& aItem )
    {
    // Print to UI
    _LIT( KCollectionHelperTestClass, "CollectionHelperTestClass" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KCollectionHelperTestClass, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KCollectionHelperTestClass, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;
    }     
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::OpenCollectionUtilityL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
    
TInt CCollectionHelperTestClass::OpenCollectionUtilityL(CStifItemParser& /*aItem*/)
	{    
	FTRACE(FPrint(_L("CCollectionHelperTest::OpenCollectionUtilityL")));
	iLog->Log(_L("CCollectionHelperTestClass::OpenCollectionUtilityL"));
		
    TRAPD( result, iCollectionUtility->Collection().OpenL() );
    
	if( KErrNone == result )
		{		
		AddExpectedEvent( EHandleCollectionMessage, KMediumTimeout );
		AddExpectedEvent( EHandleOpen, KMediumTimeout );
		}
	
	return result;
    }    
	    	
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::NewUiHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
	    	
TInt CCollectionHelperTestClass::NewUiHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewUiHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::NewUiHelperL"));

	TRAPD( result, iUIHelper = CMPXCollectionHelperFactory::NewCollectionUiHelperL() ); 

	return result;
    }    

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::NewHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
		
TInt CCollectionHelperTestClass::NewHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::NewHelperL"));

	TRAPD( result, iHelper = CMPXCollectionHelperFactory::NewCollectionHelperL() ); 

	return result;
	}
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::NewCachedHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCollectionHelperTestClass::NewCachedHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewCachedHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::NewCachedHelperL"));

	TRAPD( result, iCachedHelper = CMPXCollectionHelperFactory::NewCollectionCachedHelperL() ); 

	return result;
	}


//Removing song from collection
//By calling CMPXCollectionCachedHelper API RemoveL
TInt CCollectionHelperTestClass::RemoveSongL(CStifItemParser& /*aItem*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::RemoveSongL")));
	TRAPD(result, iCachedHelper->RemoveL(KFileWmaSong, EMPXSong));
	iLog->Log(_L("CCollectionHelperTestClass::RemoveSong done with result=[%d]"), result);
	return result;
	}

//Removing AbstractAlbum from collection
//By calling CMPXCollectionCachedHelper API RemoveL
//precondition:  e:\\data\\sounds\\digital\\1.alb  on test HW
//output: 1.alb is delete from MPX DB
TInt CCollectionHelperTestClass::RemoveAbstractAlbumL(CStifItemParser& /*aItem*/)
  {
	FTRACE(FPrint(_L("CCollectionHelperTest::RemoveAbstractAlbumL")));
	TRAPD(result, iCachedHelper->RemoveL(KAbstractalbum1, EMPXAbstractAlbum));
	iLog->Log(_L("CCollectionHelperTestClass::RemoveAbstractAlbumL done with result=[%d]"), result);
	return result;
	}
//Adds song to the database
//WMP to MTP step1
//By calling CMPXCollectionCachedHelper API AddL
//precondition:  c:\\data\\sounds\\digital\\1.wma  on test emulator
//precondition:  e:\\data\\sounds\\digital\\1.wma  on test HW
//output: 1.wma is added into MPX DB
TInt CCollectionHelperTestClass::AddSongL(CStifItemParser& /*aItem*/ )
    {
    FTRACE(FPrint(_L("CCollectionHelperTest::AddSongL")));
    //Does a record already exist for this abstractalbum?
    //this is the part to check if the song already exist in db?
    TInt result;
    TPtrC name(KFileWmaSong);
    TInt mediafound = FindMediaL(name, EMPXSong);
    
    //media already in collection DB, no need to add
    if (mediafound == 1)
        {
        return 0;
        }

    else if (mediafound == 0)  //media not in collection DB, continue to add*/
        {
        // Create media properties for the song will added
        iLog->Log(_L("Create media properties"));
        RArray<TInt> contentIDs;
        contentIDs.AppendL(KMPXMediaIdGeneral);
        contentIDs.AppendL(KMPXMediaIdAudio);
        contentIDs.AppendL(KMPXMediaIdMusic);
        contentIDs.AppendL(KMPXMediaIdMTP);

        CMPXMedia* media = CMPXMedia::NewL(contentIDs.Array());
        CleanupStack::PushL(media);
        contentIDs.Close();

        // MPXMedia default types
        media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
        media->SetTObjectValueL<TMPXGeneralCategory>(KMPXMediaGeneralCategory, EMPXSong);

        // File Path
        TParsePtrC parse(name);

        media->SetTextValueL(KMPXMediaGeneralUri, name);
        media->SetTextValueL(KMPXMediaGeneralDrive, parse.Drive());

        // Title
		media->SetTextValueL(KMPXMediaGeneralTitle, parse.NameAndExt());
		// Comment
		media->SetTextValueL(KMPXMediaGeneralComment, KNullDesC);
		// Artist
		media->SetTextValueL(KMPXMediaMusicArtist, KNullDesC);
		// Album
		media->SetTextValueL(KMPXMediaMusicAlbum, KNullDesC);
		//AlbumArtist
		media->SetTextValueL(KMPXMediaMusicAlbumArtist, KAlbumArtistShort);
		// Track
		media->SetTextValueL(KMPXMediaMusicAlbumTrack, KNullDesC);
		// Genre
		media->SetTextValueL(KMPXMediaMusicGenre, KNullDesC);
		// Composer
		media->SetTextValueL(KMPXMediaMusicComposer, KNullDesC);
		// URL
		media->SetTextValueL(KMPXMediaMusicURL, KNullDesC);
		iLog->Log(_L("Default values set to Media"));

        // Update MPX WMP Roundtrip Metadata of the media object
        media->SetTObjectValueL<TBool>(KMPXMediaGeneralDeleted, EFalse);
        media->SetTObjectValueL<TBool>(KMPXMediaGeneralModified, EFalse);
        media->SetTObjectValueL<TBool>(KMPXMediaGeneralSynchronized, ETrue);

        media->SetTObjectValueL<TUint>(KMPXMediaGeneralFlags, KMPXMediaGeneralFlagsIsInvalid|KMPXMediaGeneralFlagsIsCorrupted);

        // Add new media to collection
        TRAP(result, iCachedHelper->AddL(media));
        FTRACE(FPrint(_L("CCollectionHelperTestClass::AddSongL, Media added into collection")));

        CleanupStack::PopAndDestroy(media);
        }
	iLog->Log(_L("CCollectionHelperTestClass::AddSongL done with result=[%d]"), result);
	
	return result;
    }

//Adds abstractalbum to the database, *.alb file
//WMP to MTP step2
//By calling CMPXCollectionCachedHelper API AddL
//precondition:  c:\\data\\sounds\\digital\\1.alb  on test emulator
//precondition:  e:\\data\\sounds\\digital\\1.alb  on test HW
//output: 1.alb is added into MPX DB
TInt  CCollectionHelperTestClass::AddAbstractAlbumL(CStifItemParser& /*aItem*/)
   {
	FTRACE(FPrint(_L("CCollectionHelperTestClass::AddAbstractAlbumL")));
    TInt result;
	TPtrC name(KAbstractalbum1);
    TInt mediafound = FindMediaL(name, EMPXAbstractAlbum);
    
    //media already in collection DB, no need to add
    if (mediafound == 1)
        {
        return 0;
        }

    else if(mediafound == 0)  // media not in collection DB, continue to add*/
        {
        RArray<TInt> contentIDs;
        contentIDs.AppendL(KMPXMediaIdGeneral);

        CMPXMediaArray* abstractalbumArray = CMPXMediaArray::NewL();
        CleanupStack::PushL(abstractalbumArray);

        CMPXMedia* media = CMPXMedia::NewL(contentIDs.Array());
        CleanupStack::PushL(media);
        contentIDs.Close();

        // MPXMedia default types
        media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
        //EMPXAbstractAlbum is the new category added for non-embedded art
        media->SetTObjectValueL<TMPXGeneralCategory>(KMPXMediaGeneralCategory, EMPXAbstractAlbum);

        // File Path
        media->SetTextValueL(KMPXMediaGeneralUri, name);
        TParsePtrC parse(name);
        media->SetTextValueL(KMPXMediaGeneralDrive, parse.Drive());
        media->SetTextValueL(KMPXMediaGeneralTitle, parse.Name());

        //AlbumArtist
        media->SetTextValueL(KMPXMediaMusicAlbumArtist, KAlbumArtistShort);
        media->SetTObjectValueL<TBool>(KMPXMediaGeneralSynchronized, ETrue);
        media->SetTObjectValueL( KMPXMediaMTPSampleDataFlag, EFalse );
        media->SetCObjectValueL(KMPXMediaArrayContents, abstractalbumArray);
        media->SetTObjectValueL(KMPXMediaArrayCount, abstractalbumArray->Count());

        // Add new media to collection
        TRAP( result, iCachedHelper->AddL(media));
        CleanupStack::PopAndDestroy(media);

        // Clear the array
        CleanupStack::PopAndDestroy(abstractalbumArray);
        }
    iLog->Log(_L("CCollectionHelperTestClass::AddAbstractAlbumL done with result=[%d]"), result);
	return result;
   }
 
//Associate abstractalbum with songs
//WMP to MTP step3
//By calling CMPXCollectionCachedHelper API SetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.wma
//                           c:\\data\\sounds\\digital\\1.alb
//precondition on HW      :  f:\\1.wma
//                           f:\\1.alb
//output: 1.wma and 1.alb is associated
TInt CCollectionHelperTestClass::SetAbstractAlbumL(CStifItemParser& /*aItem*/)
 {
    FTRACE(FPrint(_L("CCollectionHelperTest::SetAbstractAlbumL")));

    CMPXMediaArray* mediaArray = CMPXMediaArray::NewL();
    CleanupStack::PushL(mediaArray);
    //add songs to media array
    AttachSongsL(mediaArray, KFileWmaSong());
    iLog->Log(_L("song added to abstractalbumArray, mediaArray->Count()=%d"), mediaArray->Count() );
    RArray<TInt> contentIDs;
    contentIDs.AppendL(KMPXMediaIdGeneral);

    CMPXMedia* abstractalbumMedia = CMPXMedia::NewL(contentIDs.Array());
    CleanupStack::PushL(abstractalbumMedia);
    contentIDs.Close();

    abstractalbumMedia->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    abstractalbumMedia->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXAbstractAlbum);

    abstractalbumMedia->SetTextValueL(KMPXMediaGeneralUri, KAbstractalbum1());

    TParsePtrC parse(KAbstractalbum1());
    abstractalbumMedia->SetTextValueL(KMPXMediaGeneralDrive, parse.Drive());
    abstractalbumMedia->SetTObjectValueL<TBool>(KMPXMediaGeneralSynchronized, ETrue);

    abstractalbumMedia->SetCObjectValueL(KMPXMediaArrayContents, mediaArray);
    abstractalbumMedia->SetTObjectValueL(KMPXMediaArrayCount, mediaArray->Count());

    TRAPD( result, iCachedHelper->SetL(abstractalbumMedia));
    CleanupStack::PopAndDestroy(abstractalbumMedia);

    // Clear the array
    CleanupStack::PopAndDestroy(mediaArray);
    iLog->Log(_L("CCollectionHelperTestClass::SetAbstractAlbumL done with result=[%d]"), result);
	
    return result;
}

//Update AlbumArtist field in Music table
//By calling CMPXCollectionCachedHelper API SetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.wma
//precondition on HW:  f:\\1.wma
//output: MUSIC.AlbumArtist field of song 1.wma is changed
TInt CCollectionHelperTestClass::SetSongAlbumArtistL(CStifItemParser& /*aItem*/)
 {
    FTRACE(FPrint(_L("CCollectionHelperTest::SetSongAlbumArtistL")));

    CMPXMedia* media = NULL;

    // Creat media properties for the song
    RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic );
    contentIDs.AppendL( KMPXMediaIdMTP );

    media = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( media ); // + media
    contentIDs.Close();

    media->SetTObjectValueL<TMPXGeneralCategory>(
        KMPXMediaGeneralCategory,
        EMPXSong );

    // MPXMedia default types
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXItem );

    // Get file path
    media->SetTextValueL( KMPXMediaGeneralUri, KFileWmaSong() );   //wma test file

    media->SetTextValueL( KMPXMediaGeneralDrive, KStoreRoot() );
    media->SetTextValueL( KMPXMediaMusicAlbumArtist, KAlbumArtistShort());

    TRAPD( result, iCachedHelper->SetL(media));

   CleanupStack::PopAndDestroy( media ); // - media
    iLog->Log(_L("CCollectionHelperTestClass::SetSongAlbumArtistL done with result=[%d]"), result);
	
   return result;
}
//Retrive MUSIC.AlbumArtist field
//By calling CMPXCollectionCachedHelper GetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.wma
//precondition on HW:  f:\\1.wma
//output: song 1.wma is returned and MUSIC.AlbumArtist field of song 1.wma is retrived.
TInt CCollectionHelperTestClass::GetSongAlbumArtistL(CStifItemParser& /*aItem*/)
{
    FTRACE(FPrint(_L("CCollectionHelperTest::GetSongAlbumArtistL")));
    TMPXGeneralCategory category = EMPXSong;
   // CMPXMedia* foundMedia= NULL;
   TInt result(KErrNotFound);
   const CMPXMedia& foundMedia = iCachedHelper->GetL( KFileWmaSong(), category );

    if (foundMedia.IsSupported(KMPXMediaMusicAlbumArtist))
        {
        const TDesC& albumartist = foundMedia.ValueText( KMPXMediaMusicAlbumArtist );
        result = KErrNone;
	    }
   iLog->Log(_L("CCollectionHelperTestClass::GetSongAlbumArtistL done "));

   return result;
}



//Update ABSTRACTALBUM.AlbumArtist
//Update AlbumArtist field in Abstractalbum table
//By calling CMPXCollectionCachedHelper API SetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.alb
//precondition on HW:  f:\\1.alb
//output: ABSTRACTALBUM.AlbumArtist field of abstractalbum 1.alb is changed
TInt CCollectionHelperTestClass::SetAbstractAlbumArtistL(CStifItemParser& /*aItem*/)
 {
    FTRACE(FPrint(_L("CCollectionHelperTest::SetAbstractAlbumArtistL")));

    CMPXMedia* media = NULL;

    RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );

    media = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( media ); // + media
    contentIDs.Close();
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory,
           EMPXAbstractAlbum );
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXItem );

    media->SetTextValueL( KMPXMediaGeneralUri, KAbstractalbum1() );
    media->SetTextValueL( KMPXMediaGeneralDrive, KStoreRoot() );

    media->SetTextValueL( KMPXMediaMusicAlbumArtist, KAlbumArtistShort() );

    TRAPD( result, iCachedHelper->SetL(media));

    CleanupStack::PopAndDestroy(media);
    iLog->Log(_L("CCollectionHelperTestClass::SetAbstractAlbumArtistL done with result=[%d]"), result);


    return result;
}

//Retrieve ABSTRACTALBUM.AlbumArtist
//By calling CMPXCollectionCachedHelper API GetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.alb
//precondition on HW:  f:\\1.alb
//output: abstractalbum 1.alb is returned and ABSTRACTALBUM.AlbumArtist field of 1.alb is retrived.
TInt CCollectionHelperTestClass::GetAbstractAlbumArtistL(CStifItemParser& /*aItem*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::GetAbstractAlbumArtistL")));
    TMPXGeneralCategory category = EMPXAbstractAlbum;
    const CMPXMedia& foundMedia = iCachedHelper->GetL( KAbstractalbum1(), category );   //wma test file
    TInt result(0);

    //print out foundMedia attributes
    if (foundMedia.IsSupported(KMPXMediaMusicAlbumArtist))
        {
		iLog->Log(_L("CCollectionHelperTestClass::GetAbstractAlbumArtistL, ABSTRACTALBUM.AlbumArtist retrived"));


        

         const TDesC& albumartist = foundMedia.ValueText( KMPXMediaMusicAlbumArtist );
         result++;
        }

		if (foundMedia.IsSupported(KMPXMediaGeneralTitle))
		 {

		  
			  //  HBufC* albumartist;
			   //     albumartist = foundMedia.ValueText( KMPXMediaMusicAlbumArtist ).AllocLC(); // + data
				 const TDesC& name = foundMedia.ValueText( KMPXMediaGeneralTitle );
				 iLog->Log(_L("CCollectionHelperTestClass::GetAbstractAlbumArtistL, ABSTRACTALBUM.name retrived"));
				 
				 result++;

		}
		if (foundMedia.IsSupported(KMPXMediaGeneralUri))
			 {

			  
				  //  HBufC* albumartist;
				   //     albumartist = foundMedia.ValueText( KMPXMediaMusicAlbumArtist ).AllocLC(); // + data
		     const TDesC& uri = foundMedia.ValueText( KMPXMediaGeneralUri );
			 iLog->Log(_L("CCollectionHelperTestClass::GetAbstractAlbumArtistL, ABSTRACTALBUM.uri retrived"));
			 result++;	  
			}
	iLog->Log(_L("CCollectionHelperTestClass::GetAbstractAlbumArtistL done"));
	if(result == 3)
		return KErrNone;
	else
	    return KErrNotFound;
	}


//Update ABSTRACTALBUM.Name
//Update Name field in Abstractalbum table
//By calling CMPXCollectionCachedHelper API SetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.alb
//precondition on HW:  f:\\1.alb
//output: ABSTRACTALBUM.name field of 1.alb is changed.
TInt CCollectionHelperTestClass::UpdateAbstractAlbumNameL(CStifItemParser& /*aItem*/)
 {
	FTRACE(FPrint(_L("CCollectionHelperTest::UpdateAbstractAlbumNameL")));
    CMPXMedia* media = NULL;

    // Creat media properties for the song
    RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );

	media = CMPXMedia::NewL( contentIDs.Array() );
	
	CleanupStack::PushL( media ); // + media
	contentIDs.Close();           //- contentIDs

    media->SetTObjectValueL<TMPXGeneralCategory>(
        KMPXMediaGeneralCategory,
        EMPXAbstractAlbum );

    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, EMPXItem );

	media->SetTextValueL( KMPXMediaGeneralUri, KAbstractalbum1() );
	media->SetTextValueL( KMPXMediaGeneralDrive, KStoreRoot() );

	//media->SetTextValueL( KMPXMediaMusicAlbumArtist, KAbstractAlbumName() );
    media->SetTextValueL( KMPXMediaGeneralTitle, KAbstractAlbumName() );
   // media->SetTextValueL( KMPXMediaMusicAlbumArtFileName, KNewAbstractAlbumName() );

	TRAPD( result, iCachedHelper->SetL(media));
    CleanupStack::PopAndDestroy(media);  // - media

    iLog->Log(_L("CCollectionHelperTestClass::UpdateAbstractAlbumNameL, done with result[%d]"), result);
    return result;
}


//Find all abstractalbums on KStoreRoot
//Find all songs associated with the found Abstractalbums
//By calling CMPXCollectionCachedHelper API GetL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.wma
//                           c:\\data\\sounds\\digital\\1.alb
//1.wma and 1.alb are associated
//precondition on HW      :  f:\\1.wma
//                           f:\\1.alb
//1.wma and 1.alb are associated
//all AbstractAlbum on drive c: (e:) is returned, all songs associated with the returned AbstractAlbum are returned.
TInt CCollectionHelperTestClass::GetAbstractAlbumAndSongsL(CStifItemParser& /*aItem*/)
    {
 	FTRACE(FPrint(_L("CCollectionHelperTest::GetAlbumAndSongsL")));
 	TUint32 abstractalbumId(0);
    RArray<TInt> contentIDs;
    contentIDs.AppendL(KMPXMediaIdGeneral);

    CMPXMedia* searchMedia = CMPXMedia::NewL(contentIDs.Array());
    CleanupStack::PushL(searchMedia);
    contentIDs.Close();

    searchMedia->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    searchMedia->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXAbstractAlbum);
    searchMedia->SetTextValueL(KMPXMediaGeneralDrive, KStoreRoot());

    RArray<TMPXAttribute> abstractalbumAttributes;
    CleanupClosePushL(abstractalbumAttributes);
    abstractalbumAttributes.AppendL(KMPXMediaGeneralId);
    abstractalbumAttributes.AppendL(KMPXMediaGeneralTitle);
    abstractalbumAttributes.AppendL(KMPXMediaGeneralUri);

   //foundMedia will hold all .alb on KStoreRoot()
   CMPXMedia* foundMedia = NULL;
   TRAPD( err, foundMedia = iCachedHelper->FindAllL(
            *searchMedia, abstractalbumAttributes.Array()));

   iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, foundMedia assigned from FindAllL"));

   CleanupStack::PopAndDestroy(&abstractalbumAttributes);
   CleanupStack::PopAndDestroy(searchMedia);
   CleanupStack::PushL(foundMedia);  //+ foundMedia

   if ( err != KErrNone )
       {
       iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, err = [%d]"), err );
       iCachedHelper->Close();
       iCachedHelper = NULL;
       User::Leave( KErrGeneral );
       }

   if (!foundMedia->IsSupported(KMPXMediaArrayCount))
       {
       User::Leave(KErrNotSupported);
       }

   TInt foundItemCount = *foundMedia->Value<TInt>(KMPXMediaArrayCount);
   iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, foundItemCount=%d"), foundItemCount );

   CMPXMediaArray* foundArray =
       foundMedia->Value<CMPXMediaArray>(KMPXMediaArrayContents);
   

   for (TInt j = 0; j < foundItemCount; ++j)
        {
        CMPXMedia* abstractalbumMedia = CMPXMedia::NewL(*(*foundArray)[j]);
        CleanupStack::PushL(abstractalbumMedia);

        //to print out founded media attributes
        if (abstractalbumMedia->IsSupported(KMPXMediaGeneralTitle))
            {
	        const TDesC& name = abstractalbumMedia->ValueText( KMPXMediaGeneralTitle );
       //     iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, found abstractalbumName = [%s]"), name );
	        }

        if (abstractalbumMedia->IsSupported(KMPXMediaGeneralId))
            {
            abstractalbumId = abstractalbumMedia->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
            abstractalbumMedia->ValueTObjectL<TMPXItemId>(KMPXMediaGeneralId);
            }

        // find the media object that contains a list of songs in the abstractalbum
        RArray<TInt> contentIDs;
        contentIDs.AppendL(KMPXMediaIdGeneral);

        CMPXMedia* searchMediaSong = CMPXMedia::NewL(contentIDs.Array());
        CleanupStack::PushL(searchMediaSong);
        contentIDs.Close();

        searchMediaSong->SetTObjectValueL(KMPXMediaGeneralType, EMPXGroup);

        searchMediaSong->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong);
        searchMediaSong->SetTObjectValueL<TMPXItemId>(KMPXMediaGeneralId, abstractalbumId);

        RArray<TMPXAttribute> songAttributes;
        CleanupClosePushL(songAttributes);
        songAttributes.AppendL(KMPXMediaGeneralId);
        songAttributes.AppendL(KMPXMediaGeneralUri);

        //search from MPX collection to get all songs associated with .alb
        CMPXMedia* foundSongs = NULL;
        TRAPD( err, foundSongs = iCachedHelper->FindAllL(
            *searchMediaSong, songAttributes.Array()));

        CleanupStack::PopAndDestroy(&songAttributes);
        CleanupStack::PopAndDestroy(searchMediaSong);
        CleanupStack::PopAndDestroy(abstractalbumMedia);
        CleanupStack::PopAndDestroy(foundMedia);

        CleanupStack::PushL( foundSongs ); // + foundSongs
        if ( err != KErrNone )
            {
            iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, err = [%d]"), err );
            iCachedHelper->Close();
            iCachedHelper = NULL;
            User::Leave( KErrGeneral );
            }

        if (!foundSongs->IsSupported(KMPXMediaArrayCount))
            {
            User::Leave(KErrNotSupported);
            }

        // Number of references
        TInt numOfSongsRefs = *foundSongs->Value<TInt>(KMPXMediaArrayCount);
        iLog->Log(_L("CCollectionHelperTestClass::GetAlbumAndSongsL, numOfSongsRefs=%d"), numOfSongsRefs );
        CleanupStack::PopAndDestroy( foundSongs ); // - foundSong
    }
   return err;
 }


//By calling CMPXCollectionCachedHelper API RenameL
//precondition on emulator:  c:\\data\\sounds\\digital\\1.wma
//                           c:\\data\\sounds\\digital\\1.alb
//1.wma and 1.alb are associated
//precondition on HW      :  f:\\1.wma
//                           f:\\1.alb
//output: 1.alb is renamed to 2.alb and all songs associated with 1.alb are updated in MUSIC table
TInt CCollectionHelperTestClass::RenameAbstractAlbumL(CStifItemParser& /*aItem*/)
   {
	TRAPD( result, iCachedHelper->RenameL( KAbstractalbum1, KAbstractalbum2, EMPXAbstractAlbum ) );
	iLog->Log(_L("CCollectionHelperTestClass::RenameAbstractAlbumL, done with result=[%d]"));
	
	return result;
	}

//Helper function for Finding in the MPX DB
//By calling CMPXCollectionCachedHelper API FindAllL
//output: all media with filename: aPath and aCategory is returned
TInt CCollectionHelperTestClass::FindMediaL(const TDesC& aPath, TMPXGeneralCategory aCategory)
{
    FTRACE(FPrint(_L("CCollectionHelperTestClass::FindL")));
	// Check does a record already exist for this aPath(URI)?
	RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );

    CMPXMedia* searchMedia = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( searchMedia ); // + searchMedia
    contentIDs.Close();
    searchMedia->SetTObjectValueL( KMPXMediaGeneralType, EMPXItem );
    searchMedia->SetTObjectValueL( KMPXMediaGeneralCategory, aCategory );
    searchMedia->SetTextValueL( KMPXMediaGeneralUri, aPath );
    
//note: some of the attributes are not needed
    RArray<TMPXAttribute> mediaAttributes;
    CleanupClosePushL( mediaAttributes ); // + mediaAttributes
    mediaAttributes.AppendL( KMPXMediaGeneralId );
    mediaAttributes.AppendL( KMPXMediaGeneralTitle );
    mediaAttributes.AppendL( KMPXMediaGeneralUri );

    CMPXMedia* foundMedia = iCachedHelper->FindAllL( *searchMedia,
        mediaAttributes.Array() );

    CleanupStack::PopAndDestroy( &mediaAttributes ); // - mediaAttributes
    CleanupStack::PopAndDestroy( searchMedia ); // - searchMedia
    CleanupStack::PushL( foundMedia ); // + foundMedia

    if ( !foundMedia->IsSupported( KMPXMediaArrayCount ) )
        {
        CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia
        return KErrNotSupported;
        }
    TInt foundItemCount = *foundMedia->Value<TInt>( KMPXMediaArrayCount );
    CleanupStack::PopAndDestroy( foundMedia ); // - foundMedia
    
    iLog->Log(_L("CCollectionHelperTestClass::FindMediaL, foundItemCount=[%d]"), foundItemCount);
    if ( foundItemCount >1) //more than 1 same item in collection
        {
	    iLog->Log(_L("CCollectionHelperTestClass::FindMediaL, error, find more than 1 same media in collection"));
        return  KErrGeneral;
	    }

    else if ( foundItemCount ==0 || foundItemCount ==1 ) //no such item in collection
        {
        iLog->Log(_L("CCollectionHelperTestClass::FindMediaL, error, no this item in collection"));
        return  foundItemCount;
        }

    return KErrNone;
    }



//Helper function for appending songs for SetAbstractAlbumL()
void CCollectionHelperTestClass::AttachSongsL(CMPXMediaArray* aAbstractAlbumArray, const TDesC& aPath)
{
	FTRACE(FPrint(_L("CCollectionHelperTestClass::AttachSongsL")));
    RArray<TInt> contentIDs;
    contentIDs.AppendL(KMPXMediaIdGeneral);
    contentIDs.AppendL(KMPXMediaIdAudio);
    contentIDs.AppendL(KMPXMediaIdMusic);
    contentIDs.AppendL(KMPXMediaIdMTP);
    CMPXMedia* media = CMPXMedia::NewL(contentIDs.Array());
    CleanupStack::PushL(media);
    contentIDs.Close();

    // MPXMedia default types
    media->SetTObjectValueL<TMPXGeneralType>(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TMPXGeneralCategory>(KMPXMediaGeneralCategory, EMPXSong);
    // File Path
    TParsePtrC parse(aPath);
    media->SetTextValueL(KMPXMediaGeneralUri, aPath);
    media->SetTextValueL(KMPXMediaGeneralDrive, parse.Drive());

    // Add media into array contents
    aAbstractAlbumArray->AppendL(media);
    CleanupStack::Pop(media);
}

// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::CloseUiHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCollectionHelperTestClass::CloseUiHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseUiHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::CloseUiHelperL"));

	TRAPD( result, iUIHelper->Close() ); 

	return result;
	}
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::CloseHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
	
TInt CCollectionHelperTestClass::CloseHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::CloseHelperL"));
	
	TRAPD( result, iHelper->Close() ); 

	return result;
	}
// -----------------------------------------------------------------------------
// CCollectionHelperTestClass::CloseCachedHelperL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
	
TInt CCollectionHelperTestClass::CloseCachedHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseCachedHelperL")));
	iLog->Log(_L("CCollectionHelperTestClass::CloseCachedHelperL"));

	TRAPD( result, iCachedHelper->Close() ); 

	return result;
	}

// ---------------------------------------------------------------------------
// Handle collection message
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleCollectionMessageL(const CMPXMessage& aMsg)
    {
	FTRACE(FPrint(_L("CCollectionHelperTestClass::HandleCollectionMessageL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleCollectionMessageL"));
	
	if( aMsg.IsSupported(KMPXMessageGeneralEvent) &&
        aMsg.IsSupported(KMPXMessageGeneralType) )         
        {
        TMPXCollectionMessage::TEvent event = 
        *aMsg.Value<TMPXCollectionMessage::TEvent>( KMPXMessageGeneralEvent );
        TInt type = *aMsg.Value<TInt>( KMPXMessageGeneralType );
        TInt data = *aMsg.Value<TInt>( KMPXMessageGeneralData );
        
        TMPXCollectionMessage message(event, type, data);
	    if(message.Event() == TMPXCollectionMessage::EPathChanged && 
	       message.Type() == EMcPathChangedByOpen)
        	{
        	iCollectionUtility->Collection().OpenL();
        	
        	this->ProcessEvent(EHandleCollectionMessage, KErrNone);
        	}	
        }
    }

// ---------------------------------------------------------------------------
//  Handles the collection entries being opened. Typically called
//  when client has Open()'d a folder
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleOpenL(const CMPXMedia& /*aEntries*/,
                                  TInt /*aIndex*/,TBool /*aComplete*/,TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOpenL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleOpenL"));
	
	this->ProcessEvent(EHandleOpen, aError);
    }
                     
// ---------------------------------------------------------------------------
// Handles the item being opened. Typically called
// when client has Open()'d an item. Client typically responds by
// 'playing' the item via the playlist
// ---------------------------------------------------------------------------
void CCollectionHelperTestClass::HandleOpenL(const CMPXCollectionPlaylist& /*aPlaylist*/,TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOpenL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleOpenL"));
    }

// ---------------------------------------------------------------------------
// Handle completion of a asynchronous command
// ---------------------------------------------------------------------------
void CCollectionHelperTestClass::HandleCommandComplete(CMPXCommand* /*aCommandResult*/, TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleCommandComplete")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleCommandComplete"));
	
	Signal();
	iTimeoutController->Cancel();
    }
    
// ---------------------------------------------------------------------------
//  Handle extended media properties
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleCollectionMediaL(const CMPXMedia& /*aMedia*/, 
                                             TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleCollectionMediaL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleCollectionMediaL"));
	
	Signal();
	iTimeoutController->Cancel();
    }

// ---------------------------------------------------------------------------
//  Handles removing a collection path
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleRemoveL(MDesCArray& /*aUriArray*/, TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleRemoveL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleRemoveL"));
	
	this->ProcessEvent(EHandleRemove, aError);
    }

// ---------------------------------------------------------------------------
//  Handle callback for "find" operation
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleFindAllL(const CMPXMedia& /*aResults*/, TBool aComplete,
                                     TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleFindAllL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleFindAllL"));
	
	if(aComplete)
		{
		this->ProcessEvent(EHandleFindAllComplete, aError);
		}
		else 
		{
		this->ProcessEvent(EHandleFindAllComplete, KErrGeneral );
		}	
    }
   
// ---------------------------------------------------------------------------
//  Handle callback for "AddFileComplete" operation
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleAddFileCompleteL( TInt aErr )
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleAddFileCompleteL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleAddFileCompleteL"));
	
	this->ProcessEvent(EHandleAddFileComplete, aErr);
	}
// ---------------------------------------------------------------------------
//  Handle callback for "OperationComplete" operation
// ---------------------------------------------------------------------------
//
void CCollectionHelperTestClass::HandleOperationCompleteL( 
		TCHelperOperation /*aOperation*/, TInt /*aErr*/, void* /*aArgument*/ )
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOperationCompleteL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleOperationCompleteL"));
	
	Signal();
	iTimeoutController->Cancel();
	}      
// ---------------------------------------------------------------------------
//  Handle callback for "EmbeddedOpen" operation
// ---------------------------------------------------------------------------
//	                                 
void CCollectionHelperTestClass::HandleEmbeddedOpenL( 
		TInt /*aErr*/, TMPXGeneralCategory /*aCategory*/ )
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleEmbeddedOpenL")));
	iLog->Log(_L("CCollectionHelperTestClass::HandleEmbeddedOpenL"));
	
	Signal();
	iTimeoutController->Cancel();
	}
//add new function

