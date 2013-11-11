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
* Description:  STIF testing for MPX Collection Playlist
*
*/


// INCLUDE FILES
#include <e32svr.h>
#include <badesca.h>
#include <StifParser.h>
#include <Stiftestinterface.h>

#include <mpxcollectionplaylist.h>
#include <mpxcollectionpath.h>
#include <mpxattribute.h>
#include "mpxcollectionplaylisttest.h"

#include "mpxcollectionobserver.h"//new added
#include "mpxcollectionutility.h"//new 
#include "mpxcollectionplaylistobserver.h"
#include "mpxcollectionopenutility.h"
#include "mpxmediageneraldefs.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Delete
// Delete here all resources allocated and opened from test methods.
// Called from destructor.
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::Delete()
    {
	  if ( iCollectionPlaylist )
		    {
		    delete iCollectionPlaylist;
		    }

      if ( iCollectionPath )
          {
          iCollectionPath->Reset();
          delete iCollectionPath;
          }
	  iExpectedEvents.Close();
    }


// -----------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::AddExpectedEvent
// Add an event to the expected event list
// -----------------------------------------------------------------------------

void CMPXCollectionPlaylistTest::AddExpectedEvent( TExpectedEvents aEvent )
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::AddExpectedEvent: %d"), aEvent);
	  iExpectedEvents.Append(aEvent);
	  }

// -----------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::RemoveExpectedEvent
// Remove the event from the expected event list
// Returns: ETrue: Event found.
//          EFalse: Event not found.
// -----------------------------------------------------------------------------
TBool CMPXCollectionPlaylistTest::RemoveExpectedEvent( TExpectedEvents aEvent )
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::RemoveExpectedEvent: %d"), aEvent);
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
// CMPXCollectionPlaylistTest::BuildCollectionPath
// Build MPX Collection Path
// -----------------------------------------------------------------------------
void CMPXCollectionPlaylistTest::BuildCollectionPathL()
      {
      iLog->Log(_L("CMPXCollectionPlaylistTest::BuildCollectionPath"));
      
      const TMPXItemId KMPXPathLevel1 = 33331;
      const TMPXItemId KMPXPathLevel2 = 33332;
      const TMPXItemId KMPXPathLevel3 = 33333;
      const TMPXItemId KMPXPathItem1 = 44441;
      const TMPXItemId KMPXPathItem2 = 44442;
      const TMPXItemId KMPXPathItem3 = 44443;
      const TMPXItemId KMPXPathItem4 = 44444;
      const TMPXItemId KMPXPathItem5 = 44445;
      RArray<TMPXItemId> itemIdArray;

      // Start from scratch
      if ( iCollectionPath )
          {
          iCollectionPath->Reset();
          delete iCollectionPath;
          }
      // build collection path
      iCollectionPath = CMPXCollectionPath::NewL();
      iCollectionPath->AppendL(KMPXPathLevel1);
      iCollectionPath->AppendL(KMPXPathLevel2);
      iCollectionPath->AppendL(KMPXPathLevel3);
      itemIdArray.Append(KMPXPathItem1);
      itemIdArray.Append(KMPXPathItem2);
      itemIdArray.Append(KMPXPathItem3);
      itemIdArray.Append(KMPXPathItem4);
      itemIdArray.Append(KMPXPathItem5);
      iCollectionPath->AppendL(itemIdArray.Array());
      
      itemIdArray.Close();
      }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::RunMethodL( CStifItemParser& aItem )
    {

    static TStifFunctionInfo const KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "Example", CMPXCollectionPlaylistTest::ExampleL ),
        //ADD NEW ENTRY HERE
        ENTRY( "CreateUtilityNewPlaylist", CMPXCollectionPlaylistTest::CreateUtilityNewPlaylist ),
        ENTRY( "CreateUtilityPlaylistExist", CMPXCollectionPlaylistTest::CreateUtilityPlaylistExist ),
        ENTRY( "CreateUtilityCollectionPath", CMPXCollectionPlaylistTest::CreateUtilityCollectionPath ),
        ENTRY( "CreateUtilityCollectionStream", CMPXCollectionPlaylistTest::CreateUtilityCollectionStream ),
        ENTRY( "CopyContentL", CMPXCollectionPlaylistTest::CopyContentL ),
        ENTRY( "Next", CMPXCollectionPlaylistTest::Next ),
        ENTRY( "Previous", CMPXCollectionPlaylistTest::Previous ),
        ENTRY( "SetToFirst", CMPXCollectionPlaylistTest::SetToFirst ),
        ENTRY( "SetToLast", CMPXCollectionPlaylistTest::SetToLast ),
        ENTRY( "SetToIndex", CMPXCollectionPlaylistTest::SetToIndex ),
        ENTRY( "Count", CMPXCollectionPlaylistTest::Count ),
        ENTRY( "Levels", CMPXCollectionPlaylistTest::Levels ),
        ENTRY( "Index", CMPXCollectionPlaylistTest::Index ),
        ENTRY( "Remote", CMPXCollectionPlaylistTest::Remote ),
        ENTRY( "RepeatMode", CMPXCollectionPlaylistTest::RepeatMode ),
        ENTRY( "Shuffle", CMPXCollectionPlaylistTest::Shuffle ),
        ENTRY( "Path", CMPXCollectionPlaylistTest::Path ),
        ENTRY( "MediaL", CMPXCollectionPlaylistTest::MediaL ),
        ENTRY( "SetRepeatMode", CMPXCollectionPlaylistTest::SetRepeatMode ),
        ENTRY( "SetRepeatEnabled", CMPXCollectionPlaylistTest::SetRepeatEnabled ),
        ENTRY( "SetShuffleL", CMPXCollectionPlaylistTest::SetShuffleL ),
        ENTRY( "SetShuffleEnabledL", CMPXCollectionPlaylistTest::SetShuffleEnabledL ),
        ENTRY( "StreamingL", CMPXCollectionPlaylistTest::StreamingL ),
        ENTRY( "SetObserver", CMPXCollectionPlaylistTest::SetObserver ),
        ENTRY( "EmbeddedPlaylist", CMPXCollectionPlaylistTest::EmbeddedPlaylist ),
        ENTRY( "SetEmbeddedPlaylist", CMPXCollectionPlaylistTest::SetEmbeddedPlaylist ),
        ENTRY( "SetL", CMPXCollectionPlaylistTest::SetL ),
        ENTRY( "CancelRequest", CMPXCollectionPlaylistTest::CancelRequest ),
        ENTRY( "Invalidate", CMPXCollectionPlaylistTest::Invalidate ),
        ENTRY("NextIndex", CMPXCollectionPlaylistTest::NextIndex ),
        ENTRY("PathIndex",CMPXCollectionPlaylistTest::PathIndex ),
        //add function
        ENTRY("CMPXCollectionPlaylistDestructor",CMPXCollectionPlaylistTest::CMPXCollectionPlaylistDestructor ),//Pass debug added 7,10,2008
        ENTRY("CMPXCollectionOpenUtilityCreate",CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityCreate ), //Pass debug added 7,10,2008
        ENTRY( "CMPXCollectionOpenUtilityStartL", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartL ),//Pass added 7,10,2008
        ENTRY( "CMPXCollectionOpenUtilityStop", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStop ),//Pass debug added 7,10,2008 
        ENTRY( "CMPXCollectionOpenUtilityStartUsingPath", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartUsingPath ),//Pass added 7,10,2008
        ENTRY( "CMPXCollectionOpenUtilitySetDelay", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDelay ),//Pass debug  added 7,10,2008
        ENTRY( "CMPXCollectionOpenUtilitySetDirection", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDirection ),//Pass debug added 7,10,2008
        ENTRY( "NewLMMPXCollectionUtility", CMPXCollectionPlaylistTest::NewLMMPXCollectionUtility ),//Pass added by 7,10,2008
        ENTRY( "CMPXCollectionOpenUtilityPathL", CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityPathL ),//Pass debug added 7,10,2008 
        };

    const TInt count = sizeof( KFunctions ) /
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::HandleCollectionMediaL
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::HandleCollectionMediaL( const CMPXMedia& /*aMedia*/, TInt aError )
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::HandleCollectionMediaL: %d"), aError);
	  }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::HandleCollectionMessage
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::HandleCollectionMessage( CMPXMessage* /*aMsg*/, TInt aError )
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::HandleCollectionMessage: %d"), aError);
	  }
	
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::HandleOpenL
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::HandleOpenL( const CMPXMedia& /*aEntries*/, TInt /*aIndex*/, TBool /*aComplete*/, TInt aError )
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::HandleOpenL: %d"), aError);
	  }
	
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::HandleOpenL
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::HandleOpenL( const CMPXCollectionPlaylist& /*aPlaylist*/, TInt aError ) 
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::HandleOpenL: %d"), aError);
	  }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::HandleOpenL
// ---------------------------------------------------------------------------
//
void CMPXCollectionPlaylistTest::HandleCollectionPlaylistChange( TInt aError ) 
	  {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::HandleCollectionPlaylistChange: %d"), aError);
	  }
	
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::ExampleL
// Example test method function.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KMPXCollectionPlaylisttest, "MPXCollectionPlaylistTest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KMPXCollectionPlaylisttest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KMPXCollectionPlaylisttest,
                                KParam, i, &string );
        i++;
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CreateUtilityNewPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CreateUtilityNewPlaylist( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CreateUtilityNewPlaylist"));
    TRAPD(err, iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
   	iLog->Log(_L("CMPXCollectionPlaylist::NewL returned: %d"), err);
    iExpectedEvents.Reset();
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CreateUtilityPlaylistExist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CreateUtilityPlaylistExist( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CreateUtilityPlaylistExist"));
    TInt err = KErrNone;
    CMPXCollectionPlaylist* firstPlaylist = NULL;
    
    // Create brand new playlist first
    TRAP(err, firstPlaylist = CMPXCollectionPlaylist::NewL());
   	iLog->Log(_L("CMPXCollectionPlaylist::NewL (firstPlaylist) returned: %d"), err);
   	
   	if ((err == KErrNone) && firstPlaylist)
   	    {
   	    // Pass existing playlist to create another instance of playlist
   	    TRAP(err, iCollectionPlaylist = CMPXCollectionPlaylist::NewL( *firstPlaylist ));
   	    iLog->Log(_L("CMPXCollectionPlaylist::NewL returned: %d"), err);
   	    }
   	    
   	if (firstPlaylist)
   	    {   	
   	    delete firstPlaylist;
        }
   	
    iExpectedEvents.Reset();
    return err;
    }    

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CreateUtilityCollectionPath
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CreateUtilityCollectionPath( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CreateUtilityCollectionPath"));
    TInt err = KErrNone;

    TRAP(err, BuildCollectionPathL());
   	if ((err == KErrNone) && iCollectionPath)
   	    {
   	    // Pass existing collection path to create playlist
   	    TRAP(err, iCollectionPlaylist = CMPXCollectionPlaylist::NewL( *iCollectionPath ));
   	    iLog->Log(_L("CMPXCollectionPlaylist::NewL returned: %d"), err);
   	    }
   	    
    iExpectedEvents.Reset();
    return err;
    }    

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CreateUtilityCollectionStream
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CreateUtilityCollectionStream( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CreateUtilityCollectionStream"));
    TInt err = KErrNone;
    CMPXCollectionPlaylist* firstPlaylist = NULL;
    
    // Create brand new playlist first
    TRAP(err, firstPlaylist = CMPXCollectionPlaylist::NewL());
    iLog->Log(_L("CMPXCollectionPlaylist::NewL (firstPlaylist) returned: %d"), err);

    if ((err == KErrNone) && firstPlaylist)
        {
        // Pass stream to create another instance of playlist
        CBufBase* buffer( NULL );
        TRAP(err,buffer = CBufFlat::NewL( 200 )); 
        CleanupStack::PushL( buffer );
        RBufWriteStream writeStream( *buffer );
        CleanupClosePushL( writeStream );
        TRAP( err,firstPlaylist->ExternalizeL( writeStream ));
        TRAP( err,writeStream.CommitL());
        buffer->Compress();
        CleanupStack::PopAndDestroy( &writeStream );

        RBufReadStream readStream( *buffer );
        CleanupClosePushL( readStream );     
        TRAP( err,iCollectionPlaylist = CMPXCollectionPlaylist::NewL(readStream));
        iLog->Log(_L("CMPXCollectionPlaylist::NewL returned: %d"), err);
        CleanupStack::PopAndDestroy( &readStream );
        CleanupStack::PopAndDestroy( buffer );
}
    if (firstPlaylist)
   	    {   	
   	    delete firstPlaylist;
        } 
    iExpectedEvents.Reset();
     
    return err;
    }    
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CopyContentL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CopyContentL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CopyContentL"));

    TInt err = KErrNone;
    CMPXCollectionPlaylist* anotherPlaylist = NULL;
    
    // Create another playlist to copy content from
    TRAP(err, anotherPlaylist = CMPXCollectionPlaylist::NewL());
   	iLog->Log(_L("CMPXCollectionPlaylist::NewL (anotherPlaylist) returned: %d"), err);
   	
   	if ((err == KErrNone) && anotherPlaylist)
   	    {
   	    anotherPlaylist->CopyContentL (*iCollectionPlaylist);
   	    if ((iCollectionPlaylist->RepeatMode() == anotherPlaylist->RepeatMode()) &&
            (iCollectionPlaylist->Shuffle() == anotherPlaylist->Shuffle()) &&
            (iCollectionPlaylist->Remote() == anotherPlaylist->Remote()) &&
            (iCollectionPlaylist->Count() == anotherPlaylist->Count()) &&
            (iCollectionPlaylist->Levels() == anotherPlaylist->Levels()) &&
            (iCollectionPlaylist->Index() == anotherPlaylist->Index()) &&
            (iCollectionPlaylist->EmbeddedPlaylist() == anotherPlaylist->EmbeddedPlaylist()))
            {
            err = KErrNone;
            iLog->Log(_L("CMPXCollectionPlaylistTest::CopyContentL copy OK"));
            }
        else
            {
            err = KErrGeneral;
            iLog->Log(_L("CMPXCollectionPlaylistTest::CopyContentL copy FAIL"));
            }
   	    }
   	    
   	if (anotherPlaylist)
   	    {   	
   	    delete anotherPlaylist;
        }    
    
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Next
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Next( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Next"));
    TInt err = KErrNone;
    TInt vRepeatIgnored;
    TBool vRetNext;

	  if ((KErrNone == aItem.GetNextInt(vRepeatIgnored)) &&
	      (vRepeatIgnored >= EFalse) && (vRepeatIgnored <= ETrue))
		    {
		    vRetNext = iCollectionPlaylist->Next (vRepeatIgnored);
		    iLog->Log(_L("CMPXCollectionPlaylist::Next returned: %d"), vRetNext);
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::Next: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Previous
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Previous( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Previous"));
    TInt err = KErrNone;
    TBool vRepeatIgnored;
    TBool vRetNext;

	  if ((KErrNone == aItem.GetNextInt(vRepeatIgnored)) &&
	      (vRepeatIgnored >= EFalse) && (vRepeatIgnored <= ETrue))
		    {
		    vRetNext = iCollectionPlaylist->Previous (vRepeatIgnored);
		    iLog->Log(_L("CMPXCollectionPlaylist::Previous returned: %d"), vRetNext);
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::Previous: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetToFirst
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetToFirst( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetToFirst"));
    TInt err = KErrNone;
    iCollectionPlaylist->SetToFirst();
    iLog->Log(_L("CMPXCollectionPlaylist::SetToFirst OK"));
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetToLast
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetToLast( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetToLast"));
    TInt err = KErrNone;
    iCollectionPlaylist->SetToLast();
    iLog->Log(_L("CMPXCollectionPlaylist::SetToLast OK"));
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetToIndex
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetToIndex( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetToIndex"));
    TInt err = KErrNone;
    TInt vIndex;
	  if ( KErrNone == aItem.GetNextInt(vIndex) )
		    {
		    TInt vCount = iCollectionPlaylist->Count();
		    iLog->Log(_L("CMPXCollectionPlaylist::Count returned: %d"), vCount);
		    if ((vCount > 0) && (vIndex < vCount))
		        {
		        iCollectionPlaylist->SetToIndex (vIndex);
		        iLog->Log(_L("CMPXCollectionPlaylist::SetToIndex OK"));
		        }
		    else
		        {
		        iLog->Log(_L("CMPXCollectionPlaylist::SetToIndex FAIL"));
		        }
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetToIndex: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Count
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Count( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Count"));
    TInt err = KErrNone;
    TInt vCount = iCollectionPlaylist->Count();
		iLog->Log(_L("CMPXCollectionPlaylist::Count returned: %d"), vCount);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Levels
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Levels( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Levels"));
    TInt err = KErrNone;
    TInt vLevels = iCollectionPlaylist->Levels();
		iLog->Log(_L("CMPXCollectionPlaylist::Levels returned: %d"), vLevels);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Index
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Index( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Index"));
    TInt err = KErrNone;
    TInt vIndex = iCollectionPlaylist->Index();
		iLog->Log(_L("CMPXCollectionPlaylist::Index returned: %d"), vIndex);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Remote
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Remote( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Remote"));
    TInt err = KErrNone;
    TBool vIsCollectionStoredInRemoteDevice = iCollectionPlaylist->Remote();
		iLog->Log(_L("CMPXCollectionPlaylist::Remote returned: %d"), vIsCollectionStoredInRemoteDevice);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::RepeatMode
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::RepeatMode( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::RepeatMode"));
    TInt err = KErrNone;
    TInt vRepeatMode = iCollectionPlaylist->RepeatMode();
		iLog->Log(_L("CMPXCollectionPlaylist::RepeatMode returned: %d"), vRepeatMode);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Shuffle
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Shuffle( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Shuffle"));
    TInt err = KErrNone;
    TBool vIsShuffleOn = iCollectionPlaylist->Shuffle();
		iLog->Log(_L("CMPXCollectionPlaylist::Shuffle returned: %d"), vIsShuffleOn);
    return err;    	
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Path
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Path( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Path"));
    TInt err = KErrNone;
//    CMPXCollectionPath& vPath = (CMPXCollectionPath&)iCollectionPlaylist->Path();
    TRAP(err,iCollectionPlaylist->Path());
    if ( err != KErrNone )
            	{
            	iLog->Log(_L("Path returned: %d"), err);
            	}
//    iLog->Log(_L("CMPXCoiCollectionPlaylist->Path()llectionPlaylist::Path returned: %d"), vPath);


    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::MediaL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::MediaL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::MediaL"));
    TInt err = KErrNone;
//    CDesCArrayFlat* aLinkArray = new (ELeave) CDesCArrayFlat(1);
    RArray<TMPXAttribute> attrs;
        CleanupClosePushL( attrs );
//        attrs.Append( KMPXMediaGeneralTitle );
//        TArray<TMPXAttribute>& ary = attrs.Array();
//    TArray<TMPXAttribute>& aAttrs=new (ELeave) TArray<TMPXAttribute> ;
    TRAP(err, iCollectionPlaylist->MediaL(attrs.Array(),*this));
    CleanupStack::PopAndDestroy( &attrs );
    if ( err != KErrNone )
        	{
        	iLog->Log(_L("MediaL returned: %d"), err);
        	}
    return err;    	
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetRepeatMode
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetRepeatMode( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetRepeatMode"));
    TInt err = KErrNone;
    TInt vRepeatMode;
	  if ((KErrNone == aItem.GetNextInt(vRepeatMode)) &&
	      (vRepeatMode >= CMPXCollectionPlaylist::ERepeatOff) && (vRepeatMode <= CMPXCollectionPlaylist::ERepeatAll))
		    {
		    iCollectionPlaylist->SetRepeatMode ((CMPXCollectionPlaylist::TRepeatMode) vRepeatMode);
		    iLog->Log(_L("CMPXCollectionPlaylist::SetRepeatMode OK"));
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetRepeatMode: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetRepeatEnabled
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetRepeatEnabled( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetRepeatEnabled"));
    TInt err = KErrNone;
    TInt vRepeatEnabled;
	  if ((KErrNone == aItem.GetNextInt(vRepeatEnabled)) &&
	      (vRepeatEnabled >= EFalse) && (vRepeatEnabled <= ETrue))
		    {
		    iCollectionPlaylist->SetRepeatEnabled (vRepeatEnabled);
		    iLog->Log(_L("CMPXCollectionPlaylist::SetRepeatEnabled OK"));
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetRepeatEnabled: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;    	
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetShuffleL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetShuffleL( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetShuffleL"));
    TInt err = KErrNone;
    TInt vShuffle;
    TInt vCurrentToTop;
	  if ((KErrNone == aItem.GetNextInt(vShuffle)) &&
	      (vShuffle >= 0) && (vShuffle <=1))
		    {
		    if ((KErrNone == aItem.GetNextInt(vCurrentToTop)) &&
		        (vCurrentToTop >= EFalse) && (vCurrentToTop <= ETrue))
		        {
		        TRAP (err, iCollectionPlaylist->SetShuffleL (vShuffle, vCurrentToTop));
		        if (err != KErrNone)
		            {
		        	  iLog->Log(_L("CMPXCollectionPlaylist::SetShuffleL leave with error: %d"), err);
		            }
		        else
		            {
		            iLog->Log(_L("CMPXCollectionPlaylist::SetShuffleL OK"));
		            }
		        }
		    else
		        {
		        iLog->Log(_L("CMPXCollectionPlaylistTest::SetShuffleL: Bad second parameter on config file"));
		        err = KErrBadTestParameter;
		        }
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetShuffleL: Bad first parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetShuffleEnabledL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetShuffleEnabledL( CStifItemParser& aItem )
    {
	  iLog->Log(_L("CMPXCollectionPlaylistTest::SetShuffleEnabledL"));
    TInt err = KErrNone;
    TInt vShuffleEnabled;
	  if ((KErrNone == aItem.GetNextInt(vShuffleEnabled)) &&
	      (vShuffleEnabled >= EFalse) && (vShuffleEnabled <= ETrue))
		    {
		    TRAP (err, iCollectionPlaylist->SetShuffleEnabledL (vShuffleEnabled));
		    if (err != KErrNone)
		        {
		    	  iLog->Log(_L("CMPXCollectionPlaylist::SetShuffleEnabledL leave with error: %d"), err);
		        }
		    else
		        {
		        iLog->Log(_L("CMPXCollectionPlaylist::SetShuffleEnabledL OK"));
		        }
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetShuffleEnabledL: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::StreamingL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::StreamingL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::StreamingL"));
    TInt err = KErrNone;

    iLog->Log(_L("Start ExternalizeL"));
    CBufBase* buffer = CBufFlat::NewL( 200 );
    CleanupStack::PushL( buffer );
    RBufWriteStream writeStream( *buffer );
    CleanupClosePushL( writeStream );
    iCollectionPlaylist->ExternalizeL( writeStream );
    writeStream.CommitL();
    buffer->Compress();
    CleanupStack::PopAndDestroy( &writeStream );

    iLog->Log(_L("Start InternalizeL"));
    RBufReadStream readStream( *buffer );
    CleanupClosePushL( readStream );                
    CMPXCollectionPlaylist* collPlaylist = CMPXCollectionPlaylist::NewL();
    CleanupStack::PushL( collPlaylist );
    collPlaylist->InternalizeL( readStream );

    CleanupStack::PopAndDestroy( collPlaylist );
    CleanupStack::PopAndDestroy( &readStream );
    CleanupStack::PopAndDestroy( buffer );
return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetObserver
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetObserver( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetObserver"));
    TInt err = KErrNone;
    MMPXCollectionPlaylistObserver* aPlObs(NULL) ;
    MMPXCollectionObserver* aCollectionObs(NULL) ;
    TRAP(err,iCollectionPlaylist->SetObserver(*this,aPlObs,aCollectionObs));
    if ( err != KErrNone )
               	{
               	iLog->Log(_L("SetObserver returned: %d"), err);
               	}
    return err;
    }


// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::EmbeddedPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::EmbeddedPlaylist( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::EmbeddedPlaylist"));
    TInt err = KErrNone;
    TBool vIsEmbeddedPlaylist = iCollectionPlaylist->EmbeddedPlaylist();
		iLog->Log(_L("CMPXCollectionPlaylist::EmbeddedPlaylist returned: %d"), vIsEmbeddedPlaylist);
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetEmbeddedPlaylist
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetEmbeddedPlaylist( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetEmbeddedPlaylist"));
    TInt err = KErrNone;
    TInt vEmbeddedPlaylist;
	  if ((KErrNone == aItem.GetNextInt(vEmbeddedPlaylist)) &&
	      (vEmbeddedPlaylist >= EFalse) && (vEmbeddedPlaylist <= ETrue))
		    {
		    iCollectionPlaylist->SetEmbeddedPlaylist (vEmbeddedPlaylist);
		    iLog->Log(_L("CMPXCollectionPlaylist::SetEmbeddedPlaylist OK"));
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::SetEmbeddedPlaylist: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::SetL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::SetL( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::SetL"));
    TInt err = KErrNone;
    
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    media->SetTObjectValueL<TUid>( KMPXMediaGeneralCollectionId, TUid::Uid(0xabcdef) );
    TRAP(err,iCollectionPlaylist->SetL( *media ));
    CleanupStack::PopAndDestroy( media );

    if ( err != KErrNone )
            	{
            	iLog->Log(_L("SetL returned: %d"), err);
            	}
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CancelRequest
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CancelRequest( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CancelRequest"));
    TInt err = KErrNone;
    iCollectionPlaylist->CancelRequest();
    iLog->Log(_L("CMPXCollectionPlaylist::CancelRequest OK"));
    return err;
    }
    
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::Invalidate
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::Invalidate( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::Invalidate"));
    TInt err = KErrNone;
    iCollectionPlaylist->Invalidate();
    iLog->Log(_L("CMPXCollectionPlaylist::Invalidate OK"));
    return err;
    }
               
// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::NextIndex
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::NextIndex( CStifItemParser& aItem )
    {
        iLog->Log(_L("CMPXCollectionPlaylistTest::NextIndex"));
        TInt err = KErrNone;
        TInt vRepeatIgnored;
        TBool vRetNext;
        TInt aPlaylistIndex;
    	  if ((KErrNone == aItem.GetNextInt(vRepeatIgnored)) &&
    	      (vRepeatIgnored >= EFalse) && (vRepeatIgnored <= ETrue))
    		    {
    		    vRetNext = iCollectionPlaylist->NextIndex(vRepeatIgnored ,aPlaylistIndex);
    		    iLog->Log(_L("CMPXCollectionPlaylist::NextIndex returned: %d"), vRetNext);
    		    }
      	else
    		    {
    		    iLog->Log(_L("CMPXCollectionPlaylistTest::NextIndex: Bad parameter on config file"));
    		    err = KErrBadTestParameter;
    		    }
        return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::PathIndex
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::PathIndex( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::PathIndex"));
    TInt err = KErrNone;
    TInt err1 = KErrNone;
    TInt vRepeatIgnored;
    TBool vRetNext;
    TInt aPlaylistIndex;
	  if ((KErrNone == aItem.GetNextInt(vRepeatIgnored)) &&
	      (vRepeatIgnored >= EFalse) && (vRepeatIgnored <= ETrue))
		    {
		    vRetNext = iCollectionPlaylist->NextIndex(vRepeatIgnored ,aPlaylistIndex);
//		    vRetNext = iCollectionPlaylist->Next(vRepeatIgnored );
		    err1 = iCollectionPlaylist->PathIndex( aPlaylistIndex );
		    iLog->Log(_L("CMPXCollectionPlaylist::NextIndex returned: %d"), vRetNext);
		    iLog->Log(_L("CMPXCollectionPlaylist::PathIndex returned: %d"), err1);
		    		    
		    }
  	else
		    {
		    iLog->Log(_L("CMPXCollectionPlaylistTest::PathIndex: Bad parameter on config file"));
		    err = KErrBadTestParameter;
		    }
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionPlaylistDestructor
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionPlaylistDestructor( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionPlaylistDestructor"));
    TInt err = KErrNone;
    if( iCollectionPlaylist != NULL )
        {
		    delete	 iCollectionPlaylist;
		    iCollectionPlaylist = NULL;
        }
    if( iCollectionPlaylist != NULL )
    	{
    	    iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionPlaylistDestructor err="),err);
    	}
    return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityCreate
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityCreate( CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
    
    MMPXCollectionObserver* aObs(NULL);
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityCreate"));
    TRAP( err , iCollectionOpenUtility=CMPXCollectionOpenUtility::NewL(aObs));
    if( iCollectionOpenUtility != NULL )
    	{
	    delete iCollectionOpenUtility;
	    iCollectionOpenUtility = NULL;
    	}
   	iLog->Log(_L("CMPXCollectionOpenUtility::NewL returned: %d"), err);
    return err;
    }


// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartL( CStifItemParser&  )
    {
       iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartL"));
       TInt err = KErrNone;
       RArray<TMPXAttribute> aAttrs;
       CleanupClosePushL( aAttrs );
       TMPXAttribute attribut = TMPXAttribute();
       aAttrs.AppendL(attribut);
       TInt aChunkSize=0;
       TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
       CMPXCollectionOpenUtility* utility( NULL );
       TRAP( err , utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeDefault));
       TRAP( err , utility->StartL(aAttrs.Array(),aChunkSize));
       delete utility;
       utility = NULL;
       CleanupStack::PopAndDestroy( &aAttrs );
       iLog->Log(_L("CMPXCollectionOpenUtility::CMPXCollectionOpenUtilityStartL err: %d"),err);
       return err;
    }

// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityPathL
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityPathL( CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityPathL"));
    TInt err = KErrNone;
    TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    CMPXCollectionOpenUtility* utility( NULL );
    TRAP( err , utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeDefault));
    
    TRAP( err ,iCollectionPath = utility->PathL());
    
    if ( utility != NULL )
    	{
		    delete utility;
		    utility = NULL;
    	}
    iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilityPathL OK"));
    return err;
    }


// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStop
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStop( CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStop"));
    TInt err = KErrNone;
    TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    CMPXCollectionOpenUtility* utility( NULL );
    TRAP( err , utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeDefault));
    
    TRAP( err , utility->Stop());
    if ( utility != NULL )
    	{
		    delete utility;
		    utility = NULL;
    	}
    iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilityStop return err: %d"),err);
    return err;
    }




// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDelay
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDelay( CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDelay"));
    TInt err = KErrNone;
    TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    CMPXCollectionOpenUtility* utility( NULL );
    TRAP( err , utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeDefault));

    TInt delay = 10;
    TRAP( err , utility->SetDelay(delay));
    if(err != KErrNone)
       {
       iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilitySetDelay does not work right"));
       delete utility;
       utility = NULL;
       return err;
       }
    if ( utility != NULL )
        	{
    		    delete utility;
    		    utility = NULL;
        	}
    iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilitySetDelay OK"));
    return err;
    }



// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartUsingPath
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartUsingPath( CStifItemParser& aItem )
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilityStartUsingPath"));
    TInt err = KErrNone;

    TUint itemId;        
    // read in parameters
    if ( aItem.GetNextInt(itemId) )
       {
       iLog->Log(_L("Missing Parameter: ItemId."));
       return KErrBadTestParameter;
       }
    
         const TMPXItemId KMPXPathItem1 = 44441;
         RArray<TMPXItemId> itemIdArray;

         // Start from scratch
         if ( iCollectionPath )
             {
             iCollectionPath->Reset();
             delete iCollectionPath;
             }
         // build collection path
         TRAP( err , iCollectionPath = CMPXCollectionPath::NewL());
         TMPXItemId item(itemId);
         TRAP( err , iCollectionPath->AppendL(item));
         itemIdArray.Append(KMPXPathItem1);
         TRAP( err , iCollectionPath->AppendL(itemIdArray.Array()));
         
         itemIdArray.Close();
   
         
    CMPXCollectionPath* path( NULL );
    TRAP( err , path = CMPXCollectionPath::NewL(*iCollectionPath));
    
    RArray<TMPXAttribute> aAttrs;   
    aAttrs.Append(KMPXMediaGeneralTitle); 
   	TInt aChunkSize=0;
   	
   	TInt aOffset = 0;
   	CMPXCollectionOpenUtility::TDirection direction = CMPXCollectionOpenUtility::EFetchNormal;
   	TMPXAttribute aKeyAttribute = KMPXMediaNullAttribute ;
   	
   	TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    CMPXCollectionOpenUtility* utility( NULL );
    TRAP( err , utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeIsolated/*KMcModeDefault*/));
    TRAP( err , utility->StartL(*path,aAttrs.Array(),aChunkSize,aOffset,direction,aKeyAttribute));
    delete utility;
    utility = NULL;
    delete path;
    aAttrs.Close();
    iLog->Log(_L("CMPXCollectionOpenUtility::CMPXCollectionOpenUtilityStartUsingPath err: %d"),err);
    return err;
    }


// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDirection
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDirection( CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::CMPXCollectionOpenUtilitySetDirection"));
    TInt err = KErrNone;
    TRAP( err , iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    CMPXCollectionOpenUtility* utility( NULL );
    TRAP( err ,  utility = CMPXCollectionOpenUtility::NewL(iCollectionPlaylist,KMcModeDefault));    
    CMPXCollectionOpenUtility::TDirection direction = CMPXCollectionOpenUtility::EFetchNormal;
    TRAP( err , utility->SetDirection(direction));
    if( err != KErrNone )
       {
       iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilitySetDirection does not work right"));
       delete utility;
       utility = NULL;
       return err;
       }
    delete utility;
    utility = NULL;
    iLog->Log(_L("CMPXCollectionPlaylist::CMPXCollectionOpenUtilitySetDirection return err: %d"),err);
    return err;
    }


// ---------------------------------------------------------------------------
// CMPXCollectionPlaylistTest::NewLMMPXCollectionUtility
// ?implementation_description
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CMPXCollectionPlaylistTest::NewLMMPXCollectionUtility( CStifItemParser& /*aItem*/)
    {
    iLog->Log(_L("CMPXCollectionPlaylistTest::NewLMMPXCollectionUtility"));
    TInt err = KErrNone;
    TRAP( err,iCollectionPlaylist = CMPXCollectionPlaylist::NewL());
    MMPXCollectionUtility* collectionUtility( NULL );
    TRAP( err ,  collectionUtility = MMPXCollectionUtility::NewL(iCollectionPlaylist,KMcModeDefault));
    collectionUtility->Close();
    iLog->Log(_L("CMPXCollectionPlaylist::NewLMMPXCollectionUtility OK"));
    return err;
    }
    
