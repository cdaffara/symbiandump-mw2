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
* Description:  HarvesterCollectionMediatorTestBlocks.cpp
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "HarvesterCollectionMediatorTest.h"

#include <mpxmedia.h>
#include <mpxmediageneraldefs.h>//to get KMPXMediaIdGeneral\EMPXSong\TMPXGeneralCategory\KMPXMediaGeneralCategory
#include <mpxmediaaudiodefs.h>//to get KMPXMediaIdAudio
#include <mpxmediamusicdefs.h>//to get KMPXMediaIdMusic 
#include <mpxmediadrmdefs.h>//to get KMPXMediaIdDrm 
#include "mpxcollectionpath.h"
#include <mpxmediamusicdefs.h>
#include <mpxmediageneraldefs.h>
#include <mpxcollectionplugin.hrh>

//#include <mpxcollectionutilityimpl.h>
//#include <mpxcollectionmediator.h>
#include <mpxcollectionutility.h>
#include "HarvesterCollectionMediatorTest.h"
//class CMPXCollectionMediator;

class CMPXMedia;
class MMPXCollection;

_LIT( KTestFileMimeType, "audio/aac" );

class SonOfMMPXCollection:public MMPXCollection
	{
	virtual void OpenL(TMPXOpenMode /*aMode=EMPXOpenDefault*/)
		{
		//iLog->Log(_L("OpenL with a mode is called"));
		};
    virtual void OpenL(TInt/* aIndex*/,TMPXOpenMode /*aMode=EMPXOpenDefault*/){
    //iLog->Log(_L("OpenL with 2 paras is called");
    }; 
    virtual void OpenL(const CMPXCollectionPath& /*aPath*/,
                       TMPXOpenMode /*aMode=EMPXOpenDefault*/){
                       //iLog->Log(_L("OpenL with a path is called");
                       };
    virtual void OpenL(TInt /*aIndex*/,
                       const TArray<TMPXAttribute>&/* aAttrs*/,
                       TMPXOpenMode /*aMode=EMPXOpenDefault*/){
                       //iLog->Log(_L("OpenL with 3 paras is called"));
                       };
    virtual void OpenL(const CMPXCollectionPath& /*aPath*/,
                       const TArray<TMPXAttribute>& /*aAttrs*/,
                       TMPXOpenMode/* aMode=EMPXOpenDefault*/){
                       //iLog->Log(_L("OpenL with 3 paras2 is called"));
                       };  
    virtual void OpenL(const TArray<TUid>& /*aUids*/,
                       TMPXOpenMode/* aMode=EMPXOpenDefault*/){
                       //iLog->Log(_L("OpenL with a mode is called"));
                       };  
    virtual void OpenL(const TUid& /*aUid*/,
                       TMPXOpenMode /*aMode=EMPXOpenDefault*/){};  
    virtual void SetFilterL(CMPXFilter* /*aFilter*/){};
    virtual CMPXFilter* FilterL(){return NULL;};  
    virtual TUid UidL() const{return TUid::Null();};  
    virtual CMPXCollectionPath* PathL(){return NULL;};  
    virtual void BackL(){};  
    virtual TBool IsRemote(const CMPXCollectionPath& /*aPath*/){return NULL;};  
    virtual void CancelRequest(){};
    virtual void AddL(const CMPXMedia& /*aNewMedia*/){}; 
    virtual void RemoveL(const CMPXCollectionPath& /*aPath*/,
                         MMPXCollectionRemoveObserver* /*aObs = NULL*/ ){};
    virtual void RemoveL(const CMPXMedia& /*aMedia*/){}; 
    virtual void SetSyncL(const CMPXMedia&/* aMedia*/){};  
    virtual void SetL(const CMPXMedia& /*aMedia*/){}; 
    virtual void FindAllL(const CMPXSearchCriteria& /*aCriteria*/,
                          const TArray<TMPXAttribute>&/* aAttrs*/,
                          MMPXCollectionFindObserver& /*aObs*/){};   
    virtual CMPXMedia* FindAllL(const CMPXSearchCriteria&/* aCriteria*/,
                                const TArray<TMPXAttribute>& /*aAttrs*/){return NULL;};  
    virtual void MediaL(const CMPXCollectionPath&/* aPath*/,
                        const TArray<TMPXAttribute>& /*aAttrs*/,
                        CMPXAttributeSpecs* /*aSpecs=NULL*/,
                        CMPXFilter* /*aFilter=NULL*/){};
    virtual void NotifyL(TMPXCollectionBroadCastMsg /*aMsg*/, TInt /*aData*/){};
    virtual void CommandL(TMPXCollectionCommand /*aCmd*/, TInt /*aData = 0*/){};
    virtual void CommandL(CMPXCommand&/* aCmd*/){};
    virtual void GetSupportedTypesL(RPointerArray<CMPXCollectionType>& /*aArray*/){};
    virtual void GetSupportedCapabilitiesL(TCollectionCapability& /*aCapability*/){};
    virtual TUid CollectionIDL(TUid& /*aCollection*/){return TUid::Null();}
    virtual void AddSubscriptionL(const CMPXSubscription& /*aSubscription*/){};
    virtual void RemoveSubscriptionL(const CMPXSubscription& /*aSubscription*/){};
    virtual void ClearSubscriptionsL(){};
	};




// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CHarvesterCollectionMediatorTest::Delete() 
    {
    
    }

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CHarvesterCollectionMediatorTest::ExampleL ),
        ENTRY( "CMPXCollectionMediatorNewL", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewL ),
        ENTRY( "CMPXCollectionMediatorNewLC", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLC ),       
        ENTRY( "CMPXCollectionMediatorNewLWithMPXCollection", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLWithMPXCollection ),
        ENTRY( "CMPXCollectionMediatorDestruction", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorDestruction ),
        ENTRY( "CMPXCollectionMediatorAddItemLMedia", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorAddItemLMedia ),
        ENTRY( "CMPXCollectionAddItemLCMPXMediaArray", CHarvesterCollectionMediatorTest::CMPXCollectionAddItemLCMPXMediaArray ),
        ENTRY( "CMPXCollectionSetItemLByCMPXMedia", CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMedia ),
        ENTRY( "CMPXCollectionSetItemLByCMPXMediaArray", CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMediaArray ),        
        ENTRY( "CMPXCollectionSetItemL", CHarvesterCollectionMediatorTest::CMPXCollectionSetItemL ),
        ENTRY( "CMPXCollectionCheckItemL", CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL ),
        ENTRY( "CMPXCollectionMoveItemLwith3paras", CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith3paras ),
        ENTRY( "CMPXCollectionMoveItemLwith4paras", CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith4paras ),
        ENTRY( "CMPXCollectionMediatorWithRealCollection", CHarvesterCollectionMediatorTest::CMPXCollectionMediatorWithRealCollection ), 
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KHarvesterCollectionMediatorTest, "HarvesterCollectionMediatorTest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KHarvesterCollectionMediatorTest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KHarvesterCollectionMediatorTest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//--------debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewL( CStifItemParser&/* aItem*/ )
	{	
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewL testing CMPXCollectionMediator::NewL() begin") );
    TInt err = KErrNone;
    TRAP( err,iMediator = CMPXCollectionMediator::NewL() )
    if( err != KErrNone )
   	   {
   	   iLog->Log( _L("CMPXCollectionMediatorNewL returns a error %d"),err );
   	   }
    else 
	   {
       iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewL testing CMPXCollectionMediator::NewL() End") );
       }
    return err;
 	}

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorDestruction
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//----debug   pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorDestruction( CStifItemParser&/* aItem*/ )
	{
	 TInt err = KErrNone;
     delete iMediator;
     iMediator = NULL;
     if(iCollectionUtility)
    	 {
         iCollectionUtility->Close();
         iCollectionUtility = NULL;
    	 }
	 iLog->Log(_L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorDestruction testing ~CMPXCollectionMediator end err=%d"), err);
	 return err;
 	}



// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLWithMPXCollection
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLWithMPXCollection( CStifItemParser&/* aItem*/ )
	{
	
    iLog->Log(_L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLWithMPXCollection testing CMPXCollectionMediator::NewL() begin"));
    TInt err=KErrNone;
    
    iCollectionUtility = MMPXCollectionUtility::NewL( );
    TRAP( err,iMediator = CMPXCollectionMediator::NewL( iCollectionUtility->Collection() ) );
    if( err != KErrNone )
   	   {
   	   iLog->Log( _L("CMPXCollectionMediatorNewLWithMPXCollection returns a error %d"),err );
   	   }
    else 
	   {
       iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLWithMPXCollection testing CMPXCollectionMediator::NewL() End") );
       }    
    return err;
 	}

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLC
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass---->crash  17,10,2008
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLC( CStifItemParser&/* aItem*/ )
	{
	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLC testing CMPXCollectionMediator::NewLC() begin") );
    TInt err = KErrNone;
    iCollectionUtility = MMPXCollectionUtility::NewL( );
    iMediator = CMPXCollectionMediator::NewLC( iCollectionUtility->Collection() );
    CleanupStack::Pop( iMediator );
    iCollectionUtility->Close();
    iCollectionUtility = NULL;
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorNewLC testing CMPXCollectionMediator::NewLC() End") );
    return err;
 	}


// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorAddItemLMedia
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorAddItemLMedia( CStifItemParser&/* aItem*/ )
	{
	
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorAddItemLMedia testing CMPXCollectionMediator::AddItemL() begin") );
    TInt err=KErrNone;
    RArray<TInt> contentIDs;
    contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic ); 
    contentIDs.AppendL( KMPXMediaIdDrm ); 
    CMPXMedia* media;
    TRAP(err,media=CMPXMedia::NewL(contentIDs.Array()));
    CleanupStack::PushL(media);
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, 
                                                      EMPXSong );
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaMusicGenre, 
                                                          EMPXSong );
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCollectionId, 
                                                          EMPXSong );
    media->ValueText(KMPXMediaGeneralCategory);
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, 
                                                  EMPXItem );
    TRAP(err,media->SetTextValueL( KMPXMediaMusicAlbumTrack, KNullDesC ));
    TRAP(err,media->SetTextValueL( KMPXMediaGeneralMimeType, KTestFileMimeType ));
    //iMediator->AddItemL(media); 
    TRAP( err,iMediator->AddItemL( media ) );
    if( err != KErrNone )
   	   {
   	   iLog->Log( _L("CMPXCollectionMediatorAddItemLMedia returns a error %d"),err );
   	   }
    else 
	   {
       iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorAddItemLMedia testing CMPXCollectionMediator::AddItemL() End") );
       }    
    CleanupStack::PopAndDestroy( media );
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionAddItemLCMPXMediaArray
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionAddItemLCMPXMediaArray( CStifItemParser&/* aItem*/ )
	{
	iLog->Log(_L("CMPXCollectionAddItemLCMPXMediaArray"));
	TInt err = KErrNone;    
    CMPXMediaArray* mediaAry = CMPXMediaArray::NewL();
    CleanupStack::PushL( mediaAry );
    
    // One music and one podcast media
    CMPXMedia* m1 = CMPXMedia::NewL();
    CleanupStack::PushL( m1 );
    m1->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath2);
    m1->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    m1->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    m1->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3A) );
    
    CMPXMedia* m2 = CMPXMedia::NewL();
    CleanupStack::PushL( m2 );
    m2->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath3);
    m2->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    m2->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    m2->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3A) );
    m2->SetTextValueL(KMPXMediaMusicGenre, _L("podcast"));
    
    mediaAry->AppendL(*m1); // copy
    mediaAry->AppendL(*m2); // copy
    
    iLog->Log(_L("CMPXCollectionAddItemLCMPXMediaArray -- adding via mediator"));
    iMediator->AddItemL(*mediaAry);
    
    mediaAry->Reset();
    m1->SetTextValueL(KMPXMediaGeneralTitle, _L("abcdefg") );
    m2->SetTextValueL(KMPXMediaGeneralTitle, _L("abcdefg") );
    
    mediaAry->AppendL(*m1); // copy
    mediaAry->AppendL(*m2); // copy
    iLog->Log(_L("CMPXCollectionAddItemLCMPXMediaArray -- updating via mediator"));
    iMediator->SetItemL(*mediaAry);
    
    CleanupStack::PopAndDestroy(2); // m1, m2
    CleanupStack::PopAndDestroy( mediaAry );
	
    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionAddItemLCMPXMediaArray returns a error %d"),err );
    	}
    else
    	{
    	 iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionAddItemLCMPXMediaArray testing CMPXCollectionMediator::AddItemL() End") );
    	}
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMedia
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMedia( CStifItemParser&/* aItem*/ )
	{	
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMedia testing CMPXCollectionMediator::SetItemL( CMPXMedia*& aMediaProp) begin") );
    TInt err = KErrNone;   
    CMPXMedia* media = CMPXMedia::NewL();
    CleanupStack::PushL( media );
    
    iLog->Log(_L("CMPXCollectionSetItemLByCMPXMedia -- adding item"));
    // Add an item
    media->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath);
    media->SetTextValueL(KMPXMediaMusicArtist,_L("MyArtist"));
    media->SetTextValueL(KMPXMediaMusicAlbum,_L("AnAlbum"));
    media->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    media->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    media->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3A) );
    iMediator->AddItemL(media);
    
    // General update
    iLog->Log(_L("CMPXCollectionSetItemLByCMPXMedia -- updating item"));
    media->SetTextValueL(KMPXMediaGeneralTitle,_L("SomeTitle"));
    iMediator->SetItemL(media);
    
    // Move to podcast, tests FetchAndMoveItemSyncL() in mediator
    iLog->Log(_L("CMPXCollectionSetItemLByCMPXMedia -- moving item"));
    media->SetTextValueL(KMPXMediaMusicGenre, _L("podcast"));
    iMediator->SetItemL(media);
    
    CleanupStack::PopAndDestroy(media);
    
    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionSetItemLByCMPXMedia returns a error %d"),err );
    	}
    else
    	{
    	 iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMedia testing CMPXCollectionMediator::SetItemL( CMPXMedia*& aMediaProp) End") );
    	}    
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMediaArray
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass--->fail 17,10,2008
TInt CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMediaArray( CStifItemParser&/* aItem*/ )
	{
    iLog->Log(_L("CMPXCollectionSetItemL"));
	TInt err = KErrNone;    
    CMPXMediaArray* mediaAry = CMPXMediaArray::NewL();
    CleanupStack::PushL( mediaAry );
    
    // One music and one podcast media
    CMPXMedia* m1 = CMPXMedia::NewL();
    CleanupStack::PushL( m1 );
    m1->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath2);
    m1->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    m1->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    m1->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3A) );
    
    CMPXMedia* m2 = CMPXMedia::NewL();
    CleanupStack::PushL( m2 );
    m2->SetTextValueL(KMPXMediaGeneralUri,KTestItemPath3);
    m2->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    m2->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    m2->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3A) );
    m2->SetTextValueL(KMPXMediaMusicGenre, _L("podcast"));
    
    mediaAry->AppendL(*m1); // copy
    mediaAry->AppendL(*m2); // copy
    
    iLog->Log(_L("CMPXCollectionSetItemL -- adding via mediator"));
    iMediator->AddItemL(*mediaAry);
    
    mediaAry->Reset();
    m1->SetTextValueL(KMPXMediaGeneralTitle, _L("abcdefg") );
    m2->SetTextValueL(KMPXMediaGeneralTitle, _L("abcdefg") );
    
    mediaAry->AppendL(*m1); // copy
    mediaAry->AppendL(*m2); // copy
    iLog->Log(_L("CMPXCollectionSetItemL -- updating via mediator"));
    iMediator->SetItemL(*mediaAry);
    
    CleanupStack::PopAndDestroy(2); // m1, m2
    CleanupStack::PopAndDestroy( mediaAry );
    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionSetItemLByCMPXMediaArray returns a error %d"),err );
    	}
    else
    	{
    	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionSetItemLByCMPXMediaArray testing CMPXCollectionMediator::SetItemL( CMPXMediaArray& aMPArray ) End") );
    	}
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionSetItemL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionSetItemL( CStifItemParser&/* aItem*/ )
	{
	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionSetItemL testing CMPXCollectionMediator::SetItemL(TArray<CMPXMedia*>& aArray,CMPXCollectionPath& aPath,RArray<TInt>& aIndices ) begin") );
    TInt err = KErrNone;
    /*RArray<CMPXMedia*> iArray = RArray<CMPXMedia*>::RArray();
    CMPXMedia* media = CMPXMedia::NewL();
//    CleanupStack::PushL(media);
    iArray.Append( media );
//    CleanupStack::PopAndDestroy(media);
    TArray<CMPXMedia*> iTarray = iArray.Array();
    
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
    TMPXItemId item(33331);
    TRAP( err , iCollectionPath->AppendL(item));
    itemIdArray.Append(KMPXPathItem1);
    TRAP( err , iCollectionPath->AppendL(itemIdArray.Array()));
    
    itemIdArray.Close();
      
            
    CMPXCollectionPath* iPath;
    TRAP( err , iPath = CMPXCollectionPath::NewL(*iCollectionPath));

    RArray<TInt> iIndices = RArray<TInt>::RArray();
    TInt index = 0;
    iIndices.Append( index );    
    iMediator->SetItemL( iTarray,*iPath,iIndices );
//    CleanupStack::PopAndDestroy(iPath);
    iArray.Close();   
  
	delete media;
	media = NULL;
    delete iPath;
	iPath = NULL;   */
    if(err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionSetItemL returns a error %d"),err );
    	}
    else
    	{
    	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionSetItemL testing CMPXCollectionMediator::SetItemL(TArray<CMPXMedia*>& aArray,CMPXCollectionPath& aPath,RArray<TInt>& aIndices ) End") );
    	}
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL( CStifItemParser&/* aItem*/ )
	{
	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL testing CMPXCollectionMediator::CheckItemL( CMPXMedia& aMedia ) begin") );
	RArray<TInt> contentIDs;
	TInt err = KErrNone;
	contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic ); 
    contentIDs.AppendL( KMPXMediaIdDrm ); 
    CMPXMedia* media = CMPXMedia::NewL( contentIDs.Array() );
    CleanupStack::PushL( media );
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, 
                                                      EMPXSong );
    TInt iIndex = media->Index( KMPXMediaGeneralCategory );
    media->ValueText( KMPXMediaGeneralCategory );
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, 
                                                  EMPXItem );
    media->SetTextValueL( KMPXMediaMusicAlbumTrack, KNullDesC );
    media->SetTextValueL( KMPXMediaGeneralMimeType, KTestFileMimeType );  
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL testing CMPXCollectionMediator::CheckItemL( CMPXMedia& aMedia ) begin") );
   
    //iMediator->CheckItemL(*media);
    TRAP( err,iMediator->CheckItemL( *media ) );
    CleanupStack::PopAndDestroy( media );
    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionCheckItemL returns a error %d"),err );
    	}
    else{
    	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionCheckItemL testing CMPXCollectionMediator::SetItemLCheckItemL( CMPXMedia& aMedia ) End") );
    	}
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith3paras
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith3paras( CStifItemParser&/* aItem*/ )
	{
	iLog->Log( _L("Testing CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia, TUid aOldCollectionId,TUid aNewCollectionId ) begin") );		
	TInt err = KErrNone;
	CMPXMedia* m1 = CMPXMedia::NewL();
    CleanupStack::PushL( m1 );
    m1->SetTextValueL(KMPXMediaGeneralUri, KTestItemPath);
    m1->SetTObjectValueL(KMPXMediaGeneralCategory, EMPXSong );
    m1->SetTObjectValueL(KMPXMediaGeneralType, EMPXItem);
    m1->SetTObjectValueL<TUid>(KMPXMediaGeneralCollectionId, TUid::Uid(0x101FFC3C) );
    iLog->Log(_L("CMPXCollectionMoveItemLwith3paras -- adding dummy"));
    iMediator->AddItemL(m1);

    // Next move it synchronously via mediator to the podcasting plugin
    iLog->Log(_L("CMPXCollectionMoveItemLwith3paras -- moving item"));
    iMediator->MoveItemL(m1, TUid::Uid(0x101FFC3C), TUid::Uid(0x101FFC3A));
    CleanupStack::PopAndDestroy(m1);

    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionMoveItemLwith3paras returns a error %d"),err );
    	}
    else
    	{
    	iLog->Log( _L("Testing CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia,TUid aOldCollectionId,TUid aNewCollectionId) End") );
    	}
    return err;
 	}
// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith4paras
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith4paras( CStifItemParser&/* aItem*/ )
	{
	iLog->Log( _L("Testing CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia, TUid aOldCollectionId,TUid aNewCollectionId,MMPXMoveItemObserver* aObs ) begin") );		
		
	RArray<TInt> contentIDs;
	TInt err = KErrNone;
    contentIDs.AppendL( KMPXMediaIdGeneral );
    contentIDs.AppendL( KMPXMediaIdAudio );
    contentIDs.AppendL( KMPXMediaIdMusic ); 
    contentIDs.AppendL( KMPXMediaIdDrm ); 
    CMPXMedia* media;
    TRAP(err,media = CMPXMedia::NewL( contentIDs.Array() ));
    CleanupStack::PushL( media );
    media->SetTObjectValueL<TMPXGeneralCategory>( KMPXMediaGeneralCategory, 
                                                      EMPXSong );
    TInt iIndex = media->Index( KMPXMediaGeneralCategory );
    media->ValueText( KMPXMediaGeneralCategory );
    media->SetTObjectValueL<TMPXGeneralType>( KMPXMediaGeneralType, 
                                                  EMPXItem );
    TRAP(err,media->SetTextValueL( KMPXMediaMusicAlbumTrack, KNullDesC ));
    TRAP(err,media->SetTextValueL( KMPXMediaGeneralMimeType, KTestFileMimeType ));  
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith4paras testing CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia,TUid aOldCollectionId,TUid aNewCollectionId,MMPXMoveItemObserver* aObs) begin") );
    TUid iOldCollectionId = {0};
    TUid iNewCollectionId = {1};
    //iMediator->MoveItemL(media,iOldCollectionId,iNewCollectionId,this);
    TRAP( err,iMediator->MoveItemL( media,iOldCollectionId,iNewCollectionId,this ) );
    CleanupStack::PopAndDestroy( media );
    if( err != KErrNone )
    	{
    	iLog->Log( _L("CMPXCollectionMoveItemLwith4paras returns a error %d"),err );
    	}
    else
    	{
    	iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMoveItemLwith4paras testing CMPXCollectionMediator::MoveItemL( CMPXMedia*& aMedia,TUid aOldCollectionId,TUid aNewCollectionId,MMPXMoveItemObserver* aObs) End") );
    	}
    return err;
 	}



// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::CMPXCollectionMediatorWithRealCollection
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//-----debug pass
TInt CHarvesterCollectionMediatorTest::CMPXCollectionMediatorWithRealCollection( CStifItemParser&/* aItem*/ )
	{
	
    iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorWithRealCollection testing CMPXCollectionMediator::NewL() begin") );
    TInt err = KErrNone;
    SonOfMMPXCollection* son= new SonOfMMPXCollection();
    TRAP( err,iMediator = CMPXCollectionMediator::NewL( *son ) );
    if( err != KErrNone )
   	   {
   	   iLog->Log( _L("CMPXCollectionMediatorWithRealCollection returns a error %d"),err );
   	   }
    else 
	   {
       iLog->Log( _L("CHarvesterCollectionMediatorTest::CMPXCollectionMediatorWithRealCollection testing CMPXCollectionMediator::NewL() End") );
       }    
    //iUtility->Close();
    return err;
 	}

// -----------------------------------------------------------------------------
// CHarvesterCollectionMediatorTest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

/*TInt CHarvesterCollectionMediatorTest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }*/


// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
