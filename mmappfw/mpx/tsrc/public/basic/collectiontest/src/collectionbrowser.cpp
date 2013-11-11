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
* Description:  MpxCollectionTest test module.
*
*/

// INCLUDE FILES
#include <mpxcollectionutility.h>
#include <mpxcollectionpath.h>
#include <mpxcollectionplaylist.h>
#include <mpxplaybackframeworkdefs.h>
#include <mpxcollectionmessage.h>
#include <mpxcollectionframeworkdefs.h>
#include <mpxmessagegeneraldefs.h> // KMPXMessageGeneralEvent, KMPXMessageGeneralType
#include <mpxmediageneraldefs.h>
#include <mpxmediaarray.h>
#include <mpxmediacontainerdefs.h>
#include <mpxattribute.h>
#include <stiflogger.h>
#include <testutility.h>
#include <testplaybackutility.h>
#include <testplaybackview.h>
#include "pathoptionmenu.h" 
#include "collectionbrowser.h"

// MACROS

// DATA TYPES

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCollectionBrowser::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
/*
CCollectionBrowser* CCollectionBrowser::NewL(CConsoleMain* aConsoleMain,
                                             CTestBaseView* aParent,
                                             const TDesC16& aName,
                                             CStifLogger* aLogger,
                                             CMPXCollectionPath* aPath)
    {
    CCollectionBrowser* self = CCollectionBrowser::NewL(aConsoleMain,
                                                        aParent,
                                                        aName,
                                                        aLogger);    
    CleanupStack::PopAndDestroy(self);

    return self;
    }
   */ 
// -----------------------------------------------------------------------------
// CCollectionBrowser::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCollectionBrowser* CCollectionBrowser::NewLC(CConsoleMain* aConsoleMain,
                                              CTestBaseView* aParent,
                                              const TDesC16& aName,
                                              CStifLogger* aLogger,
                                              CMPXCollectionPath* aPath)
    {
    CCollectionBrowser* self = new (ELeave) CCollectionBrowser(aConsoleMain,
                                                               aParent,
                                                               aName,
                                                               aLogger);
    
    CleanupStack::PushL( self );
    self->ConstructL(aPath);

    return self;
    }
    
// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CCollectionBrowser::~CCollectionBrowser()
    {
    iLogger->Log(_L("+CCollectionBrowser::~CCollectionBrowser"));
    
    if(iCollectionUtility)   
    	iCollectionUtility->Close();
    if(iCollectionPath)
        delete iCollectionPath;
    if(iPlaybackUtility)
        delete iPlaybackUtility;
    iLogger->Log(_L("-CCollectionBrowser::~CCollectionBrowser"));
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Initialize the view before display view
// -----------------------------------------------------------------------------
void CCollectionBrowser::InitializeViewL()
    {
    iLogger->Log(_L("+CCollectionBrowser::InitializeViewL"));	

/*
    // Resynchronize podcast databases
    TUid collection = KPodcastCollection; 
    collection = iCollectionUtility->
                    Collection().CollectionIDL( collection );
    TRAP_IGNORE( 
        iCollectionUtility->
            Collection().CommandL( EMcCmdCollectionInit, 
                                   collection.iUid ) );
    
    // Resynchronize music collection databases
    collection = KLocalCollection; 
    collection = iCollectionUtility->
                    Collection().CollectionIDL( collection );
    iCollectionUtility->Collection().CommandL( EMcCmdCollectionInit, 
                                               collection.iUid );
*/
    
    // ConstructL() did the Utility->NewL() already.
                                       
    if( iCollectionPath )
        iCollectionUtility->Collection().OpenL(*iCollectionPath);
    else
        iCollectionUtility->Collection().OpenL(); 

    iLogger->Log(_L("-CCollectionBrowser::InitializeViewL"));
    }
    
// -----------------------------------------------------------------------------
// From CTestBaseView
// Cleanup the view before deactivate/destroy view
// -----------------------------------------------------------------------------
void CCollectionBrowser::CleanupViewL()
    {
    if(iPlaybackUtility)
        {
        delete iPlaybackUtility;
        iPlaybackUtility = NULL;      
        }    
    }
    
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle number key press
// -----------------------------------------------------------------------------
void CCollectionBrowser::HandleNumKeyL()
    {
    /*
    TKeyCode key = LastKeyPressed();
    if((int)key == '1')
        {
        CPathOptionMenu* pathOptionMenu = 
                CPathOptionMenu::NewL(iConsoleMain,
                                     this,
                                     KTxtPathOptionMenu,
                                     iLogger);
        CleanupStack::PushL(pathOptionMenu);
        CMPXCollectionPath* path = iCollectionUtility->Collection().PathL();
        CleanupStack::PushL(path);
        path->SelectL(CurrentIndex());   //change to selected path
        pathOptionMenu->SetPathL(*path);
        CleanupStack::PopAndDestroy(path);
        NextViewL(pathOptionMenu);
        CleanupStack::Pop(pathOptionMenu);
        iChild = static_cast<CTestBaseView*>(pathOptionMenu);
        }
    */
    }
    
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle left key press
// -----------------------------------------------------------------------------
void CCollectionBrowser::HandleLeftKeyL()
    {
    iLogger->Log(_L("+CCollectionBrowser::HandleLeftKeyL"));
    CMPXCollectionPath* currPath = iCollectionUtility->Collection().PathL();
    CleanupStack::PushL( currPath );
    if(currPath->Levels() > 1)
        {
        iCollectionUtility->Collection().BackL();
        }
    // otherwise current path is at or below root level, thereofeore, no back call
    CleanupStack::PopAndDestroy( currPath );
    iLogger->Log(_L("-CCollectionBrowser::HandleLeftKeyL"));
    }
        
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle right/enter key press
// -----------------------------------------------------------------------------
void CCollectionBrowser::HandleRightKeyL()
    {
    iLogger->Log(_L("+CCollectionBrowser::HandleRightKeyL"));
    TInt count = ItemCount();
    TInt index = CurrentIndex();
    
    if(count > 0 && index >= 0 && index < count)
        {
        iCollectionUtility->Collection().OpenL( index );
        }
    iLogger->Log(_L("-CCollectionBrowser::HandleRightKeyL"));
    }

// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMediaL
// -----------------------------------------------------------------------------
//
void CCollectionBrowser::HandleCollectionMediaL(
                        const CMPXMedia& aMedia, 
                        TInt aError)
    {
    if(aError == KErrNone)
        {
        // Assume root plugins title will be returned only
        // see CCollectionBrowser::HandleOpenL
        TMPXAttribute titleAttr(KMPXMediaIdGeneral,EMPXMediaGeneralTitle);
        if( aMedia.IsSupported(titleAttr) )
            {
            const TDesC& title = aMedia.ValueText( titleAttr );
 //           TestUtility::TestLog(iLogger, _L("%S\n"), &title);
            AddItemL(title);
            }
        DisplayViewL();
        }
    }
            
// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CCollectionBrowser::HandleCollectionMessageL(const CMPXMessage& aMsg)
    {
    iLogger->Log(_L("+CCollectionBrowser::HandleCollectionMessageL"));
        
    if( aMsg.IsSupported(KMPXMessageGeneralEvent) &&
        aMsg.IsSupported(KMPXMessageGeneralType) ) 
        {
        TMPXCollectionMessage::TEvent event = 
        *aMsg.Value<TMPXCollectionMessage::TEvent>( KMPXMessageGeneralEvent );
        TInt type = *aMsg.Value<TInt>( KMPXMessageGeneralType );
        TInt data = *aMsg.Value<TInt>( KMPXMessageGeneralData );
        TMPXCollectionMessage message(event, type, data);
    
    	if (message.Event() == TMPXCollectionMessage::EPathChanged && 
    	    message.Type() == EMcPathChangedByOpen)
    		{ 
    		// Open new entries
        	iCollectionUtility->Collection().OpenL();
       		}    
       		
  	    iLogger->Log(_L("CCollectionBrowser: Message %d Returned: type=%d data=%d"), 
                                                             message.Event(), 
                                                             message.Type(), 
                                                             message.Data());
    
   		iLogger->Log(_L("-CCollectionBrowser::HandleCollectionMessageL"));
    	}
    }

// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CCollectionBrowser::HandleOpenL(const CMPXMedia& aEntries,
                                     TInt aIndex,TBool aComplete,TInt aError)
    {
    iLogger->Log(_L("+CCollectionBrowser::HandleOpenL"));
    if(!aEntries.IsSupported(TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayContents)) ||
       !aEntries.IsSupported(TMPXAttribute(KMPXMediaIdContainer,EMPXMediaArrayCount)) )
        User::Panic(_L("NOT appropiate array media"), -1);
    
    CMPXMediaArray* media = 
            aEntries.ValueCObjectL<CMPXMediaArray>( TMPXAttribute(KMPXMediaIdContainer, EMPXMediaArrayContents) );
    CleanupStack::PushL( media );
        
    (void)aComplete;
    iItems->Reset();
    if(aError == KErrNone)
        {
        TMPXAttribute uidAttr(KMPXMediaIdGeneral,EMPXMediaGeneralId);
        TMPXAttribute titleAttr(KMPXMediaIdGeneral,EMPXMediaGeneralTitle);
        
        CMPXCollectionPath* currPath = iCollectionUtility->Collection().PathL();
        CleanupStack::PushL( currPath );
        
        for(TInt i=0; i<media->Count(); i++)
            {
            const CMPXMedia* entry = ( *media )[i];
            if(currPath->Levels() == 1)
                {
                // Assume at root level, use MediaL to get plugin name
                CMPXCollectionPath* rootPath = CMPXCollectionPath::NewL();
                CleanupStack::PushL( rootPath );
                TMPXItemId uid = entry->ValueTObjectL<TMPXItemId>( uidAttr );
                rootPath->AppendL( uid );
                RArray<TMPXAttribute> attrs;
                CleanupClosePushL( attrs );
                attrs.Append( titleAttr );
                
                iCollectionUtility->Collection().MediaL(*rootPath, attrs.Array());
                CleanupStack::PopAndDestroy(2, rootPath );  //attrs, rootPath
                }
            else
                {
                // Assume browsing within plugin already
                const TDesC& title = entry->ValueText( titleAttr );
 //               TestUtility::TestLog(iLogger, _L("%S\n"), &title);
                AddItemL(title);
                }
            }
        CleanupStack::PopAndDestroy( currPath );
        }
    iPosOnScreen = aIndex;
    DisplayViewL();
    
    CleanupStack::PopAndDestroy(media);
    iLogger->Log(_L("-CCollectionBrowser::HandleOpenL"));
    }
        
// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CCollectionBrowser::HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError)
    {
    (void)aError;
    CTestPlaybackView* view = CTestPlaybackView::NewL(iConsoleMain,
                                                     this,
                                                     KTxtTestPlaybackView,
                                                     iLogger);
    iPlaybackUtility = CTestPlaybackUtility::NewL(KPbModeNewPlayer,
                                                  view,
                                                  iLogger);
    view->AddObserver(iPlaybackUtility);
    CleanupStack::PushL(view);
    iPlaybackUtility->InitL(aPlaylist);
    CleanupStack::Pop(view);
    iChild = static_cast<CTestBaseView*>(view);
    NextViewL(iChild);
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
CCollectionBrowser::CCollectionBrowser(CConsoleMain* aConsoleMain,
                                       CTestBaseView* aParent,
                                       const TDesC& aName,
                                       CStifLogger* aLogger)
    : CTestMenuView(aConsoleMain, aParent, aName)
    {
    iCollectionUtility = NULL; // set by ConstructL()
    iLogger = aLogger;
    iCollectionPath = NULL;  // set by ConstructL() if path != NULL
    iPlaybackUtility = NULL;    
    }

// -----------------------------------------------------------------------------
// Second phase constructor
// -----------------------------------------------------------------------------
void CCollectionBrowser::ConstructL(CMPXCollectionPath* aPath)
    {    
    iLogger->Log(_L("+CCollectionBrowser::ConstructL"));
    iCollectionUtility = MMPXCollectionUtility::NewL(this);
    if(aPath)
        {
        iCollectionPath = CMPXCollectionPath::NewL(*aPath);
        }
    CTestMenuView::ConstructL();
    iLogger->Log(_L("-CCollectionBrowser::ConstructL"));
    }
    
