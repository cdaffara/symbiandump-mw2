/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  STIF testing for MPX Harvester
*
*/


// INCLUDE FILES
#include <e32svr.h>
#include <badesca.h>
#include <StifParser.h>
#include <Stiftestinterface.h>

#include <mpxharvesterutility.h>
#include <mpxcollectionutility.h>
#include <mpxattribute.h>
#include <mpxplaylistenginedefs.h>
#include "mpxharvestertest.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Cmpxharvestertest::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::Delete()
    {

	if ( iHarvester )
		{
		iHarvester->Close();
		}
	if ( iCollection )
		{
		iCollection->Close();
		}
	iExpectedEvents.Close();
    }


// -----------------------------------------------------------------------------
// Cmpxharvestertest::AddExpectedEvent
// Add an event to the expected event list
// -----------------------------------------------------------------------------

void Cmpxharvestertest::AddExpectedEvent( TExpectedEvents aEvent )
	{
	iLog->Log(_L("Cmpxharvestertest::AddExpectedEvent: %d"), aEvent);
	iExpectedEvents.Append(aEvent);
	}

// -----------------------------------------------------------------------------
// Cmpxharvestertest::RemoveExpectedEvent
// Remove the event from the expected event list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool Cmpxharvestertest::RemoveExpectedEvent( TExpectedEvents aEvent )
	{
	iLog->Log(_L("Cmpxharvestertest::RemoveExpectedEvent: %d"), aEvent);
	TBool match = EFalse;
	TInt index = iExpectedEvents.Find(aEvent);
	if ( index != KErrNotFound )
		{
		iExpectedEvents.Remove(index);
		match = ETrue;
		}
	return match;
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RunMethodL(
    CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "Example", Cmpxharvestertest::ExampleL ),
        //ADD NEW ENTRY HERE
        ENTRY( "CreateUtility", Cmpxharvestertest::CreateUtility ),
        ENTRY( "CopyTestFile", Cmpxharvestertest::CopyTestFile ),
        ENTRY( "DeleteTestFile", Cmpxharvestertest::DeleteTestFile ),
        ENTRY( "VerifyEvents", Cmpxharvestertest::VerifyEvents ),
        ENTRY( "Scan", Cmpxharvestertest::Scan ),
        ENTRY( "CancelScan", Cmpxharvestertest::CancelScan ),
        ENTRY( "AddFileByPath", Cmpxharvestertest::AddFileByPath ),
        //ENTRY( "AddFileByMedia", Cmpxharvestertest::AddFileByMedia ),
        ENTRY( "RemoveFile", Cmpxharvestertest::RemoveFile ),
        ENTRY( "RemoveFiles", Cmpxharvestertest::RemoveFiles ),
        ENTRY( "RecreateDatabases", Cmpxharvestertest::RecreateDatabases ),
        ENTRY( "DeleteFile", Cmpxharvestertest::DeleteFile ),
        ENTRY( "DeleteFiles", Cmpxharvestertest::DeleteFiles ),
        //ENTRY( "UpdateFile", Cmpxharvestertest::UpdateFile ),
        ENTRY( "FindCollectionId", Cmpxharvestertest::FindCollectionId ),
        //ENTRY( "RenameFileByMedia", Cmpxharvestertest::RenameFileByMedia ),
        ENTRY( "RenameFileByPath", Cmpxharvestertest::RenameFileByPath ),
        //ENTRY( "ExportPlaylist", Cmpxharvestertest::ExportPlaylist ),
        ENTRY( "ImportPlaylist", Cmpxharvestertest::ImportPlaylist ),
        ENTRY( "ImportFile", Cmpxharvestertest::ImportFile ),
        ENTRY( "RequiredAttributes", Cmpxharvestertest::RequiredAttributes ),
        ENTRY( "OptionalAttributes", Cmpxharvestertest::OptionalAttributes ),
        ENTRY( "PlaylistFileExtension", Cmpxharvestertest::PlaylistFileExtension ),
        ENTRY( "IsPlaylist", Cmpxharvestertest::IsPlaylist ),
        ENTRY( "CheckForSystemEvents", Cmpxharvestertest::CheckForSystemEvents )
        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandlePlaylistExportCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandlePlaylistExportCompletedL( CMPXMedia* /*aMedia*/, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandlePlaylistExportCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandlePlaylistExportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandlePlaylistExportCompletedL: %d"), aErr);
		}
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandlePlaylistImportCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandlePlaylistImportCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandlePlaylistImportCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandlePlaylistImportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandlePlaylistImportCompletedL: %d"), aErr);
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
// Cmpxharvestertest::HandleFileAddCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleFileAddCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileAddCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileAddCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileAddCompletedL: %d"), aErr);
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
// Cmpxharvestertest::HandleDeleteCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleDeleteCompletedL( TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleDeleteCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandleDeleteCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandleDeleteCompletedL: %d"), aErr);
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
// Cmpxharvestertest::HandleFileImportCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleFileImportCompletedL( CMPXMedia* aMedia, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileImportCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::EEventHandleFileImportCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::EEventHandleFileImportCompletedL: %d"), aErr);
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
// Cmpxharvestertest::HandleFileGetMediaCompletedL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleFileGetMediaCompletedL( CMPXMedia* /*aMedia*/, TInt aErr )
	{
	if ( RemoveExpectedEvent(EEventHandleFileGetMediaCompletedL) )
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileGetMediaCompletedL: %d [EXPECTED]"), aErr);
		}
	else
		{
		iLog->Log(_L("Cmpxharvestertest::HandleFileGetMediaCompletedL: %d"), aErr);
		}
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleCollectionMediaL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleCollectionMediaL( const CMPXMedia& /*aMedia*/, TInt aError )
	{
	iLog->Log(_L("Cmpxharvestertest::HandleCollectionMediaL: %d"), aError);
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleCollectionMessage
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleCollectionMessage( CMPXMessage* /*aMsg*/, TInt aError )
	{
	iLog->Log(_L("Cmpxharvestertest::HandleCollectionMessage: %d"), aError);
	}
	
// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleOpenL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleOpenL( const CMPXMedia& /*aEntries*/, TInt /*aIndex*/, TBool /*aComplete*/, TInt aError )
	{
	iLog->Log(_L("Cmpxharvestertest::HandleOpenL: %d"), aError);
	}
// ---------------------------------------------------------------------------
// Cmpxharvestertest::HandleOpenL
// ---------------------------------------------------------------------------
//
void Cmpxharvestertest::HandleOpenL( const CMPXCollectionPlaylist& /*aPlaylist*/, TInt aError ) 
	{
	iLog->Log(_L("Cmpxharvestertest::HandleOpenL: %d"), aError);
	}

// ---------------------------------------------------------------------------
// Cmpxharvestertest::ExampleL
// Example test method function.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( Kmpxharvestertest, "mpxharvestertest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, Kmpxharvestertest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, Kmpxharvestertest,
                                KParam, i, &string );
        i++;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::CreateUtility
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::CreateUtility( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxharvestertest::CreateUtility"));
    TRAPD(err, iHarvester = CMPXHarvesterFactory::NewL());
    if ( err == KErrNone )
    	{
    	TRAP(err, iCollection = MMPXCollectionUtility::NewL(this));
	    if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("MMPXCollectionUtility::NewL returned: %d"), err);
	    	}
    	}
	else
		{
    	iLog->Log(_L("CMPXHarvesterFactory::NewL returned: %d"), err);
    	}

    iExpectedEvents.Reset();
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::CopyTestFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::CopyTestFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::CopyTestFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
        {
		TBuf<120> KFrom;
		KFrom.Append(Kmpxharvester_testPath);
		KFrom.Append(string);

		TBuf<120> KTo;
		KTo.Append(Kmpxharvester_digitalPath);
		KTo.Append(string);

		for( TInt i = 0; i < 5; i++ )
		    {
            err = iFileMan->Copy(KFrom, KTo);
	        if ( err != KErrNone )
	            {
	            iLog->Log(_L("CopyTestFile returned: %d"), err);
	            User::After( 1000000 );
	            }
	        else
	            {
                break;
	            }
		    }
        }
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::DeleteTestFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::DeleteTestFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::DeleteTestFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
        {
		TBuf<120> KFrom;
		KFrom.Append(Kmpxharvester_digitalPath);
		KFrom.Append(string);

		for( TInt i = 0; i < 5; i++ )
		    {
            err = iFileMan->Delete(KFrom);
	        if ( err != KErrNone )
	            {
	            iLog->Log(_L("DeleteTestFile returned: %d"), err);
	            User::After( 1000000 );
	            }
	        else
	            {
                break;
	            }
		    }

        }
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::VerifyEvents
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::VerifyEvents( CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
    TInt count = iExpectedEvents.Count();
    if ( count > 0 )
	    {
	    for ( TInt i = 0; i < count; i++ )
	    	{
	    	iLog->Log(_L("Cmpxharvestertest::VerifyEvents - Missing: %d"), iExpectedEvents[i]);
	    	}
	    err = KErrGeneral;
	    iExpectedEvents.Reset();
	    }
    return err;
    }
    
// ---------------------------------------------------------------------------
// Cmpxharvestertest::Scan
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::Scan( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxharvestertest::Scan"));
    TRAPD(err, iHarvester->ScanL());
    if ( err != KErrNone )
    	{
    	iLog->Log(_L("ScanL returned: %d"), err);
    	}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::CancelScan
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::CancelScan( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxharvestertest::CancelScan"));
    TRAPD(err, iHarvester->CancelScanL());
    if ( err != KErrNone )
    	{
    	iLog->Log(_L("CancelScanL returned: %d"), err);
    	}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::AddFileByPath
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::AddFileByPath( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::AddFileByPath"));
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

	    TRAP(err, iHarvester->AddFileL(KFile, this))
 		if ( err == KErrNone )
 			{
 			AddExpectedEvent(EEventHandleFileAddCompletedL);
 			}
 		else
 			{
			iLog->Log(_L("AddFileL returned: %d"), err);
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::AddFileByMedia
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
//TInt Cmpxharvestertest::AddFileByMedia( CStifItemParser& aItem )

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RemoveFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RemoveFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::RemoveFile"));
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

	    TRAP(err, iHarvester->RemoveFileL(KFile))
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("RemoveFileL returned: %d"), err);
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RemoveFiles
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RemoveFiles( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::RemoveFiles"));
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		if ( string == KTagAll )
			{
		    TRAP(err, iHarvester->RemoveAllFilesL());
	 		if ( err != KErrNone )
	 			{
				iLog->Log(_L("RemoveAllFilesL returned: %d"), err);
				}
			}
		else
			{
            CDesCArrayFlat* files = new (ELeave)CDesCArrayFlat(1);
            CleanupStack::PushL(files);
			TBuf<120> KFrom;
			KFrom.Append(Kmpxharvester_digitalPath);
			KFrom.Append(string);
            files->AppendL(KFrom);

		    while ( aItem.GetNextString( string ) == KErrNone )
		        {
				TBuf<120> KFrom;
				KFrom.Append(Kmpxharvester_digitalPath);
				KFrom.Append(string);
	            files->AppendL(KFrom);
		        }

			TRAP(err, iHarvester->RemoveFilesL(*files));
		 	if ( err != KErrNone )
		 		{
				iLog->Log(_L("RemoveFilesL returned: %d"), err);
				}
			CleanupStack::PopAndDestroy(files);
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RecreateDatabases
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RecreateDatabases( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxharvestertest::RecreateDatabases"));
    TRAPD(err, iHarvester->RecreateDatabasesL());
    if ( err != KErrNone )
    	{
    	iLog->Log(_L("RecreateDatabasesL returned: %d"), err);
    	}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::DeleteFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::DeleteFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::DeleteFile"));
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

	    TRAP(err, iHarvester->DeleteFileL(KFile, this))
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("DeleteFileL returned: %d"), err);
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::DeleteFiles
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::DeleteFiles( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::DeleteFiles"));
    TInt err = KErrNone;
    TPtrC mode;
    TPtrC string;
    TPtrC string2;
	if ( KErrNone == aItem.GetNextString(mode) )
		{
        CDesCArrayFlat* files = new (ELeave)CDesCArrayFlat(1);
        CleanupStack::PushL(files);
		while ( aItem.GetNextString( string ) == KErrNone )
		    {
			TBuf<120> KFrom;
			KFrom.Append(Kmpxharvester_digitalPath);
			KFrom.Append(string);
	        files->AppendL(KFrom);
		    }

		if ( mode == KTagAsync )
			{
		    TRAP(err, iHarvester->DeleteFilesL(*files, this));
	 		if ( err == KErrNone )
		 		{
		 		AddExpectedEvent(EEventHandleDeleteCompletedL);
		 		}
		 	else
	 			{
				iLog->Log(_L("DeleteFilesL async returned: %d"), err);
				}
			}
		else if ( mode == KTagSync )
			{
            for( TInt i = 0; i < 5; i++ )
                {
                TRAP(err, iHarvester->DeleteFilesL(*files));
                if ( err != KErrNone )
                    {
                    iLog->Log(_L("DeleteFilesL sync returned: %d"), err);
                    User::After( 1000000 );
                    }
                else
                    {
                    break;
                    }
                }
			}
		else
			{
			iLog->Log(_L("Bad parameter on config file"));
			err = KErrBadTestParameter;
			}
		CleanupStack::PopAndDestroy(files);
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::UpdateFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
//TInt Cmpxharvestertest::UpdateFile( CStifItemParser& aItem )

// ---------------------------------------------------------------------------
// Cmpxharvestertest::FindCollectionId
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::FindCollectionId( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::FindCollectionId"));
    TInt err = KErrNone;
    TPtrC string;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

		TInt id = 0;
	    TRAP(err, id = iHarvester->FindCollectionIdL(KFile))
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("FindCollectionIdL returned: %d"), err);
			}
		else
 			{
			iLog->Log(_L("Collection ID: %d"), id);
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RenameFileByMedia
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
//TInt Cmpxharvestertest::RenameFileByMedia( CStifItemParser& aItem )

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RenameFileByPath
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RenameFileByPath( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::RenameFileByPath"));
    TInt err = KErrNone;
    TPtrC oldUri;
    TPtrC newUri;
	TBuf<120> KOld;
	TBuf<120> KNew;
	if ( KErrNone == aItem.GetNextString(oldUri) )
		{
		KOld.Append(Kmpxharvester_digitalPath);
		KOld.Append(oldUri);

		if ( KErrNone == aItem.GetNextString(newUri) )
			{
			KNew.Append(Kmpxharvester_digitalPath);
			KNew.Append(newUri);
			}
		else
			{
			iLog->Log(_L("Bad parameter on config file"));
			err = KErrBadTestParameter;
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

	if ( err == KErrNone )
		{
		TInt id = 0;
	    TRAP(err, id = iHarvester->FindCollectionIdL(KOld))
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("FindCollectionIdL returned: %d"), err);
			}
		else
			{
			TRAP(err, iHarvester->RenameFileL(KOld, KNew, id));
		 	if ( err != KErrNone )
		 		{
				iLog->Log(_L("RenameFileL returned: %d"), err);
				}
			}
		}
    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::ExportPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
//TInt Cmpxharvestertest::ExportPlaylist( CStifItemParser& aItem )

// ---------------------------------------------------------------------------
// Cmpxharvestertest::ImportPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::ImportPlaylist( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::ImportPlaylist"));
    TInt err = KErrNone;
    TPtrC string;
    TPtrC result;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

		if ( KErrNone == aItem.GetNextString(result) )
			{
			if ( result == KTagTrue )
				{
				iResult = ETrue;
				}
			else if ( result == KTagFalse )
				{
				iResult = EFalse;
				}
			else
				{
				iLog->Log(_L("Bad parameter on config file"));
				err = KErrBadTestParameter;
				}
			
			if ( err == KErrNone )
				{
				TRAP(err, iHarvester->ImportPlaylistL(KFile, this))
				if ( err == KErrNone )
					{
					AddExpectedEvent(EEventHandlePlaylistImportCompletedL);
					}
				else
					{
					iLog->Log(_L("ImportPlaylist returned: %d"), err);	
					}
				}
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::ImportFile
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::ImportFile( CStifItemParser& aItem )
    {
	iLog->Log(_L("Cmpxharvestertest::ImportFile"));
    TInt err = KErrNone;
    TPtrC string;
    TPtrC result;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

		if ( KErrNone == aItem.GetNextString(result) )
			{
			if ( result == KTagTrue )
				{
				iResult = ETrue;
				}
			else if ( result == KTagFalse )
				{
				iResult = EFalse;
				}
			else
				{
				iLog->Log(_L("Bad parameter on config file"));
				err = KErrBadTestParameter;
				}			
			
			if ( err == KErrNone )
				{
				TRAP(err, iHarvester->ImportFileL(KFile, this))
				if ( err == KErrNone )
					{
					AddExpectedEvent(EEventHandleFileImportCompletedL);
					}
				else
					{
					iLog->Log(_L("ImportFile returned: %d"), err);
					}

				}
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::RequiredAttributes
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::RequiredAttributes( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::RequiredAttributes"));
    TInt err = KErrNone;
    TPtrC playlistType;
	if ( KErrNone == aItem.GetNextString(playlistType) )
		{
		TMPXPlaylistType type = EMPXPlaylistTypeUnknown;
		if ( playlistType == KTagM3U )
			{
			type = EMPXPlaylistTypeM3U;
			}
		else if ( playlistType == KTagPLA )
			{
			type = EMPXPlaylistTypePLA;
			}
		else if ( playlistType == KTagMPV )
			{
			type = EMPXPlaylistTypeMPV;
			}
		else if ( playlistType == KTagXSPF )
			{
			type = EMPXPlaylistTypeXSPF;
			}
		else if ( playlistType == KTagXML )
			{
			type = EMPXPlaylistTypeXML;
			}
		else
			{
			iLog->Log(_L("Bad parameter on config file"));
			return KErrBadTestParameter;
			}

		RArray<TMPXAttribute> att;
	    TRAP(err, iHarvester->RequiredAttributesL(type, att));
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("RequiredAttributesL returned: %d"), err);
			}
		else
			{
			TInt count = att.Count();
			iLog->Log(_L("Number of attributes: %d"), count );
			for ( TInt i=0; i < count; i++ )
				{
				TMPXAttribute attribute = att[i];
				iLog->Log(_L("AttributeId: %d, ContentId: %d"), attribute.AttributeId(), attribute.ContentId() );
				}
			}
		att.Reset();
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::OptionalAttributes
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::OptionalAttributes( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::OptionalAttributes"));
    TInt err = KErrNone;
    TPtrC playlistType;
	if ( KErrNone == aItem.GetNextString(playlistType) )
		{
		TMPXPlaylistType type = EMPXPlaylistTypeUnknown;
		if ( playlistType == KTagM3U )
			{
			type = EMPXPlaylistTypeM3U;
			}
		else if ( playlistType == KTagPLA )
			{
			type = EMPXPlaylistTypePLA;
			}
		else if ( playlistType == KTagMPV )
			{
			type = EMPXPlaylistTypeMPV;
			}
		else if ( playlistType == KTagXSPF )
			{
			type = EMPXPlaylistTypeXSPF;
			}
		else if ( playlistType == KTagXML )
			{
			type = EMPXPlaylistTypeXML;
			}
		else
			{
			iLog->Log(_L("Bad parameter on config file"));
			return KErrBadTestParameter;
			}

		RArray<TMPXAttribute> att;
	    TRAP(err, iHarvester->OptionalAttributesL(type, att));
 		if ( err != KErrNone )
 			{
			iLog->Log(_L("OptionalAttributesL returned: %d"), err);
			}
		else
			{
			TInt count = att.Count();
			iLog->Log(_L("Number of attributes: %d"), count );
			for ( TInt i=0; i < count; i++ )
				{
				// AK - need to add better checking
				TMPXAttribute attribute = att[i];
				iLog->Log(_L("AttributeId: %d, ContentId: %d"), attribute.AttributeId(), attribute.ContentId() );
				}
			}
		att.Reset();
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::PlaylistFileExtension
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::PlaylistFileExtension( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::PlaylistFileExtension"));
    TInt err = KErrNone;
    TPtrC playlistType;
	if ( KErrNone == aItem.GetNextString(playlistType) )
		{
		TMPXPlaylistType type = EMPXPlaylistTypeUnknown;
		if ( playlistType == KTagM3U )
			{
			type = EMPXPlaylistTypeM3U;
			}
		else if ( playlistType == KTagPLA )
			{
			type = EMPXPlaylistTypePLA;
			}
		else if ( playlistType == KTagMPV )
			{
			type = EMPXPlaylistTypeMPV;
			}
		else if ( playlistType == KTagXSPF )
			{
			type = EMPXPlaylistTypeXSPF;
			}
		else if ( playlistType == KTagXML )
			{
			type = EMPXPlaylistTypeXML;
			}
		else
			{
			iLog->Log(_L("Bad parameter on config file"));
			return KErrBadTestParameter;
			}

	    HBufC* ext = iHarvester->PlaylistFileExtensionLC(type);
		TPtr extptr = ext->Des();
		iLog->Log(_L("Extension: %S"), &extptr );
		err = !( (type == EMPXPlaylistTypeM3U) && (ext->Des() == _L(".m3u")) );
		iLog->Log(_L("Correct extension error: %d"), err );
		CleanupStack::PopAndDestroy(ext);
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }


// ---------------------------------------------------------------------------
// Cmpxharvestertest::IsPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::IsPlaylist( CStifItemParser& aItem )
    {
    iLog->Log(_L("Cmpxharvestertest::IsPlaylist"));
    TInt err = KErrNone;
    TPtrC string;
    TPtrC test;
	if ( KErrNone == aItem.GetNextString(string) )
		{
		TBuf<120> KFile;
		KFile.Append(Kmpxharvester_digitalPath);
		KFile.Append(string);

		if ( KErrNone == aItem.GetNextString(test) )
			{
			TBool playlist = EFalse;
			TRAP(err, playlist = iHarvester->IsPlaylistL(KFile))
			if ( err != KErrNone )
				{
				iLog->Log(_L("IsPlaylist returned: %d"), err);
				}
			else
				{
				if ( ((test == KTagTrue) && playlist ) ||
				     ((test == KTagFalse) && !playlist ) )
				    {
					err = KErrNone;
					}
				else
				    {
					err = KErrGeneral;
					}
				}
			}
		}
	else
		{
		iLog->Log(_L("Bad parameter on config file"));
		err = KErrBadTestParameter;
		}

    return err;
    }

// ---------------------------------------------------------------------------
// Cmpxharvestertest::CheckForSystemEvents
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt Cmpxharvestertest::CheckForSystemEvents( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("Cmpxharvestertest::CheckForSystemEvents"));
    TRAPD(err, iHarvester->CheckForSystemEventsL());
    if ( err != KErrNone )
    	{
    	iLog->Log(_L("CancelScanL returned: %d"), err);
    	}
    return err;
    }

