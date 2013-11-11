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
* Description:  Scripted STIF cases for MPXCollectionHelper
*
*/

// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "collectionhelpertest.h"
#include <mpxmessagegeneraldefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxmediaaudiodefs.h>
#include <mpxmediamusicdefs.h>
#include <mpxmediadrmdefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxattribute.h>


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCollectionHelperTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCollectionHelperTest::Delete() 
    {
    FTRACE(FPrint(_L("CTunerUtilityTest::Delete")));
    
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
// CCollectionHelperTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCollectionHelperTest::RunMethodL( CStifItemParser& aItem ) 
    {
    
    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
             
        ENTRY( "OpenCollectionUtility", CCollectionHelperTest::OpenCollectionUtilityL ),
        ENTRY( "NewUiHelper", CCollectionHelperTest::NewUiHelperL ),
        ENTRY( "NewHelper", CCollectionHelperTest::NewHelperL ),
        ENTRY( "NewCachedHelper", CCollectionHelperTest::NewCachedHelperL ),
        ENTRY( "CloseUiHelper", CCollectionHelperTest::CloseUiHelperL ),
        ENTRY( "CloseHelper", CCollectionHelperTest::CloseHelperL ),
        ENTRY( "CloseCachedHelper", CCollectionHelperTest::CloseCachedHelperL ),        
        ENTRY( "AddFile", CCollectionHelperTest::AddFileL),
        ENTRY( "AddMedia", CCollectionHelperTest::AddMediaL ),
        ENTRY( "RemoveFile", CCollectionHelperTest::RemoveFileL ),
        ENTRY( "RemoveAll", CCollectionHelperTest::RemoveAllL ),
        ENTRY( "CleanupDeletedMedias", CCollectionHelperTest::CleanupDeletedMediasL ),
        ENTRY( "SetMedia", CCollectionHelperTest::SetMediaL ),
        ENTRY( "RenameUri", CCollectionHelperTest::RenameUriL ),
        ENTRY( "GetFile", CCollectionHelperTest::GetFileL ),
        ENTRY( "GetFile2", CCollectionHelperTest::GetFile2L ),
        ENTRY( "FindAll", CCollectionHelperTest::FindAllL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );
    
    }

TInt CCollectionHelperTest::OpenCollectionUtilityL(CStifItemParser& /*aItem*/)
	{    
	FTRACE(FPrint(_L("CCollectionHelperTest::OpenCollectionUtilityL")));
	iLog->Log(_L("CCollectionHelperTest::OpenCollectionUtilityL"));
		
    TRAPD( result, iCollectionUtility->Collection().OpenL() );
    
	if( KErrNone == result )
		{		
		AddExpectedEvent( EColHandleCollectionMessage, KMediumTimeout );
		AddExpectedEvent( EColHandleOpen, KMediumTimeout );
		}
	
	return result;
    }    
	    	
	    	
TInt CCollectionHelperTest::NewUiHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewUiHelperL")));
	iLog->Log(_L("CCollectionHelperTest::NewUiHelperL"));

	TRAPD( result, 
	       iUIHelper = CMPXCollectionHelperFactory::NewCollectionUiHelperL() ); 

	return result;
    }    

		
TInt CCollectionHelperTest::NewHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewHelperL")));
	iLog->Log(_L("CCollectionHelperTest::NewHelperL"));

	TRAPD( result, 
	       iHelper = CMPXCollectionHelperFactory::NewCollectionHelperL() ); 

	return result;
	}

TInt CCollectionHelperTest::NewCachedHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::NewCachedHelperL")));
	iLog->Log(_L("CCollectionHelperTest::NewCachedHelperL"));

	TRAPD( result, 
	       iCachedHelper = 
               CMPXCollectionHelperFactory::NewCollectionCachedHelperL() ); 

	return result;
	}

TInt CCollectionHelperTest::CloseUiHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseUiHelperL")));
	iLog->Log(_L("CCollectionHelperTest::CloseUiHelperL"));

	TRAPD( result, iUIHelper->Close() ); 

	return result;
	}
	
TInt CCollectionHelperTest::CloseHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseHelperL")));
	iLog->Log(_L("CCollectionHelperTest::CloseHelperL"));
	
	TRAPD( result, iHelper->Close() ); 

	return result;
	}
	
TInt CCollectionHelperTest::CloseCachedHelperL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CloseCachedHelperL")));
	iLog->Log(_L("CCollectionHelperTest::CloseCachedHelperL"));

	TRAPD( result, iCachedHelper->Close() ); 

	return result;
	}
			
TInt CCollectionHelperTest::AddFileL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::AddFileL")));	
	iLog->Log(_L("CCollectionHelperTest::AddFileL"));
	
	TRAPD( result, iHelper->AddL( KTestFile, this ) );
	
	if( KErrNone == result )
		{		
		AddExpectedEvent( EColHandleAddFileComplete, KMediumTimeout );
		}
	
	return result;
	}
	
TInt CCollectionHelperTest::AddMediaL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::AddMediaL")));
	iLog->Log(_L("CCollectionHelperTest::AddMediaL"));
    	  	
    CMPXMedia* media( NULL );
	CreateTestMediaL( media );
    CleanupStack::PushL( media );

    TRAPD( result, iHelper->AddL( media ) );
		    
    CleanupStack::PopAndDestroy( media );    	
    
	return result; 
	};

TInt CCollectionHelperTest::RemoveFileL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::RemoveFileL")));
	iLog->Log(_L("CCollectionHelperTest::RemoveFileL"));
    TRAPD( result, iHelper->RemoveL( KTestFile ) );
    
	//if( KErrNone == result )
	//	{		
	//	AddExpectedEvent( EHandleRemove, KMediumTimeout );
	//	}
		
	return result;
	}

TInt CCollectionHelperTest::RemoveAllL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::RemoveAllL")));
	iLog->Log(_L("CCollectionHelperTest::RemoveAllL"));
    
    TRAPD( result, iHelper->RemoveAllL() );
    
    //if( KErrNone == result )
	//	{		
	//	AddExpectedEvent( EHandleRemove, KMediumTimeout );
	//	}
		
	return result;
	}

TInt CCollectionHelperTest::CleanupDeletedMediasL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::CleanupDeletedMediasL")));
	iLog->Log(_L("CCollectionHelperTest::CleanupDeletedMediasL"));
	
    TRAPD( result, iHelper->CleanupDeletedMediasL() );  
      
    // This method is unsupported.
    result = (result == KErrNotSupported) ? KErrNone : result;
	return result;
	}

TInt CCollectionHelperTest::SetMediaL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::SetMediaL")));
	iLog->Log(_L("CCollectionHelperTest::SetMediaL"));
    	
    CMPXMedia* media( NULL );
	CreateTestMediaL( media );
    CleanupStack::PushL( media );

    TRAPD( result, iHelper->SetL( media ) );
		    
    CleanupStack::PopAndDestroy( media );           	
		
	return result;
	}

TInt CCollectionHelperTest::RenameUriL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::RenameUriL")));
	iLog->Log(_L("CCollectionHelperTest::RenameUriL"));
    	
    TRAPD( result, iHelper->RenameL( KRenameOldUri,KRenameNewUri ) );
		
    return result;
	}

TInt CCollectionHelperTest::GetFileL(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::GetFileL")));
	iLog->Log(_L("CCollectionHelperTest::GetFileL"));
    	
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    
    CMPXMedia* theFile = NULL;
	TRAPD( result, theFile = iHelper->GetL( KTestFile, attrs.Array() ) );
	if(theFile)
		{
		delete theFile;
		}
	
	CleanupStack::PopAndDestroy( &attrs );   // attrs
		
    return result;
	}
	
TInt CCollectionHelperTest::GetFile2L(CStifItemParser& /*aItem*/)
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::GetFile2L")));
	iLog->Log(_L("CCollectionHelperTest::GetFile2L"));
	
	TRAPD( result, const CMPXMedia& theFile = iHelper->GetL( KTestFile ) );
		
	// This method is unsupported.
	result = (result == KErrNotSupported) ? KErrNone : result;		
    return result;
	}

TInt CCollectionHelperTest::FindAllL(CStifItemParser& /*aItem*/)
	{	
	FTRACE(FPrint(_L("CCollectionHelperTest::FindAllL")));
	iLog->Log(_L("CCollectionHelperTest::FindAllL"));
    
    RArray<TMPXAttribute> attrs;
    CleanupClosePushL( attrs );
    
    CMPXMedia* media( NULL );
	CreateTestMediaL( media );
    CleanupStack::PushL( media );
    
    CMPXMedia* allMedia = NULL;
	TRAPD( result, allMedia = iHelper->FindAllL( *media, attrs.Array() ) );
	if(allMedia)
		{
		delete allMedia;
		}
			
	CleanupStack::PopAndDestroy( media );
	CleanupStack::PopAndDestroy( &attrs );   // attrs
			
    return result;
	}

void CCollectionHelperTest::CreateTestMediaL( CMPXMedia*& aNewProperty )
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
    
// ---------------------------------------------------------------------------
// Handle collection message
// ---------------------------------------------------------------------------
//
void CCollectionHelperTest::HandleCollectionMessageL(const CMPXMessage& aMsg)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleCollectionMessageL")));
	iLog->Log(_L("CCollectionHelperTest::HandleCollectionMessageL"));
	
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
        	
        	this->ProcessEvent(EColHandleCollectionMessage, KErrNone);
        	}	
        }
    }

// ---------------------------------------------------------------------------
//  Handles the collection entries being opened. Typically called
//  when client has Open()'d a folder
// ---------------------------------------------------------------------------
//
void CCollectionHelperTest::HandleOpenL(const CMPXMedia& /*aEntries*/,
                                  TInt /*aIndex*/, TBool aComplete,TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOpenL")));
	iLog->Log(_L("CCollectionHelperTest::HandleOpenL"));
	
	this->ProcessEvent(EColHandleOpen, aError);
    }
                     
// ---------------------------------------------------------------------------
// Handles the item being opened. Typically called
// when client has Open()'d an item. Client typically responds by
// 'playing' the item via the playlist
// ---------------------------------------------------------------------------
void CCollectionHelperTest::HandleOpenL(
        const CMPXCollectionPlaylist& /*aPlaylist*/, TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOpenL")));
	iLog->Log(_L("CCollectionHelperTest::HandleOpenL"));
    }

// ---------------------------------------------------------------------------
// Handle completion of a asynchronous command
// ---------------------------------------------------------------------------
void CCollectionHelperTest::HandleCommandComplete(
        CMPXCommand* /*aCommandResult*/, TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleCommandComplete")));
	iLog->Log(_L("CCollectionHelperTest::HandleCommandComplete"));
	
	Signal();
	iTimeoutController->Cancel();
    }
    
// ---------------------------------------------------------------------------
//  Handle extended media properties
// ---------------------------------------------------------------------------
//
void CCollectionHelperTest::HandleCollectionMediaL(const CMPXMedia& /*aMedia*/, 
                                             TInt /*aError*/)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleCollectionMediaL")));
	iLog->Log(_L("CCollectionHelperTest::HandleCollectionMediaL"));
	
	Signal();
	iTimeoutController->Cancel();
    }

// ---------------------------------------------------------------------------
//  Handles removing a collection path
// ---------------------------------------------------------------------------
//
void CCollectionHelperTest::HandleRemoveL(MDesCArray& /*aUriArray*/, TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleRemoveL")));
	iLog->Log(_L("CCollectionHelperTest::HandleRemoveL"));
	
	this->ProcessEvent(EColHandleRemove, aError);
    }

// ---------------------------------------------------------------------------
//  Handle callback for "find" operation
// ---------------------------------------------------------------------------
//
void CCollectionHelperTest::HandleFindAllL(const CMPXMedia& /*aResults*/, 
        TBool aComplete, TInt aError)
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleFindAllL")));
	iLog->Log(_L("CCollectionHelperTest::HandleFindAllL"));
	
	if(aComplete)
		{
		this->ProcessEvent(EColHandleFindAllComplete, aError);
		}
		else 
		{
		this->ProcessEvent(EColHandleFindAllComplete, KErrGeneral );
		}	
    }
   

void CCollectionHelperTest::HandleAddFileCompleteL( TInt aErr )
	{
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleAddFileCompleteL")));
	iLog->Log(_L("CCollectionHelperTest::HandleAddFileCompleteL"));
	
	this->ProcessEvent(EColHandleAddFileComplete, aErr);
	}

void CCollectionHelperTest::HandleOperationCompleteL( 
		TCHelperOperation /*aOperation*/, TInt /*aErr*/, void* /*aArgument*/ )
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleOperationCompleteL")));
	iLog->Log(_L("CCollectionHelperTest::HandleOperationCompleteL"));
	
	Signal();
	iTimeoutController->Cancel();
	}      
	                                 
void CCollectionHelperTest::HandleEmbeddedOpenL( 
		TInt /*aErr*/, TMPXGeneralCategory /*aCategory*/ )
    {
	FTRACE(FPrint(_L("CCollectionHelperTest::HandleEmbeddedOpenL")));
	iLog->Log(_L("CCollectionHelperTest::HandleEmbeddedOpenL"));
	
	Signal();
	iTimeoutController->Cancel();
	}
