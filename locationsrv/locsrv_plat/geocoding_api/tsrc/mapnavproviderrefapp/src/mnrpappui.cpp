/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpAppUi class implementation
*
*/


#include <aknnavi.h>
#include <aknnavide.h>
#include <eikbtgpc.h>

#include <akniconutils.h>
#include <aknselectionlist.h>
#include <gulicon.h>

#include <MnRefProvider.rsg>
#include <MnRefProvider.mbg>
#include "mnrp.hrh"

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>

#include "mnrpappview.h"
#include "mnrpnaviview.h"
#include "mnrpmapview.h"

#include "mnrpmapviewservice.h"
#include "mnrpnavigationservice.h"

#include "mnrpdocument.h"
#include "mnrpengine.h"
#include "mnrpappui.h"
#include "mnrputils.h"
#include "debug.h"

const TInt KDefaultLmIconIndex = 0;
_LIT( KMbmFile, "\\resource\\apps\\MnRefProvider.mbm");

// ============================ LOCAL FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CleanupIconArray( TAny* aArray )
    {
    LOG("MnRefProvider::CleanupIconArray in");
    ( ( CArrayPtrSeg<CGulIcon>* ) aArray)->ResetAndDestroy();
    delete aArray;
    LOG("MnRefProvider::CleanupIconArray out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AddIconL(
    CArrayPtr<CGulIcon>& aIconArray,
    const TDesC& aFileName,
    TInt aIconIndex,
    TInt aMaskIndex )
    {
    LOG("MnRefProvider::AddIconL in");
    CFbsBitmap* icon = NULL;
    CFbsBitmap* mask = NULL;

    AknIconUtils::CreateIconLC( icon, mask, aFileName, aIconIndex, aMaskIndex );
    CGulIcon* gul = CGulIcon::NewL( icon, mask );

    gul->SetBitmapsOwnedExternally( EFalse );
    CleanupStack::PushL( gul );
    aIconArray.AppendL( gul );
    CleanupStack::Pop( 3 ); // gul, icon & mask
    LOG("MnRefProvider::AddIconL out");
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CArrayPtrSeg<CGulIcon>* CreateIconListLC( TInt aGranularity )
    {
    CArrayPtrSeg<CGulIcon>* iconList =
        new (ELeave) CArrayPtrSeg<CGulIcon>( aGranularity );
    CleanupStack::PushL( TCleanupItem( CleanupIconArray, iconList ) );

    AddIconL(
        *iconList,
        KMbmFile,
        EMbmMnrefproviderDefault_lm,
        EMbmMnrefproviderDefault_lm_mask);

    return iconList;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void AddLandmarkL(
    const CPosLandmark& aLandmark,
    CDesCArray& aTextArray,
    CArrayPtr<CGulIcon>& aIconArray )
    {
    TPtrC name;
    aLandmark.GetLandmarkName( name );

    HBufC* printableName = MnrpUtils::PrintableStringLC( name );
    
    TPtrC iconFile;
    TInt iconIndex, maskIndex, iconArrayIndex;

    if ( aLandmark.GetIcon( iconFile, iconIndex, maskIndex ) == KErrNone )
        {
        AddIconL( aIconArray, iconFile, iconIndex, maskIndex );
        iconArrayIndex = aIconArray.Count() - 1;
        }
    else
        {
        iconArrayIndex = KDefaultLmIconIndex;
        }

    _LIT(KListItemFormat, "%d\t%S\t\t");
    const TInt KListItemMinSize = 20;

    HBufC* item = HBufC::NewLC( printableName->Size() + KListItemMinSize );
    item->Des().Format( KListItemFormat, iconArrayIndex, printableName );

    aTextArray.AppendL( *item );

    CleanupStack::PopAndDestroy( item );
    CleanupStack::PopAndDestroy( printableName );
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppUi::CMnrpAppUi( CMnrpEngine& aEngine )
  : iEngine( aEngine )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::ConstructL()
    {
    BaseConstructL( EAknEnableSkin );
    
    // This is empty view, it will be shown by UI framework,
    // when application starts
    iAppView = CMnrpAppView::NewL();
    AddViewL( iAppView );

    if ( !iEikonEnv->StartedAsServerApp() )
        {
        // switch to map view if started normally
        iMapView = CMnrpMapView::NewL( EFalse, iEngine );
        AddViewL( iMapView );
        ActivateLocalViewL( iMapView->Id() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppUi::~CMnrpAppUi()
    {
    delete iNaviPaneLabel;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::HandleCommandL( TInt aCommand )
    {
    switch(aCommand)
        {
        case EAknSoftkeyBack:
        	{
    	    CEikAppServer* server = iEikonEnv->AppServer();
		    if ( server )
		    	{
		        server->NotifyServerExit( EEikCmdExit );
		    	}
		    Exit();	
        	break;
        	}
        	
        case EEikCmdExit:
        case EAknSoftkeyExit:
            {
    	    CEikAppServer* server = iEikonEnv->AppServer();
		    if ( server )
		    	{
		        server->NotifyServerExit( EAknCmdExit ); // this will close client also
		    	}
		    Exit();	
        	break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnrpAppUi::IsChainedMode()
    {
    return ( iParentWg != 0 );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnrpAppUi::IsAnyUserViewActive()
    {
    TVwsViewId viewId;
    if ( ( GetActiveViewId( viewId ) == KErrNotFound ) ||
         ( viewId.iViewUid.iUid != EMnrpMapViewId && 
           viewId.iViewUid.iUid != EMnrpNavigationViewId ) )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CMnrpAppUi::SetNaviPaneTitleL( const TDesC& aPaneTitle )
    {
    if ( !iNaviPane )
        {
        iNaviPane = static_cast<CAknNavigationControlContainer*>(
                StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
        }

    if ( iNaviPaneLabel )
        {
        CAknNavigationDecorator* naviLabel = iNaviPane->CreateNavigationLabelL( aPaneTitle );
        CleanupStack::PushL( naviLabel );

        iNaviPane->ReplaceL( *iNaviPaneLabel, *naviLabel );

        delete iNaviPaneLabel;
        iNaviPaneLabel = naviLabel;

        CleanupStack::Pop( naviLabel );
        }
    else
        {
        iNaviPaneLabel = iNaviPane->CreateNavigationLabelL( aPaneTitle );
        iNaviPane->PushL( *iNaviPaneLabel );
        }
    }    

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CMnrpAppUi::SetRightSoftkeyCommandL( TInt aCommand, const TDesC& aButtonTitle )
    {
    if ( Cba() )
        {
        Cba()->SetCommandL( 1, aCommand, aButtonTitle );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TBool CMnrpAppUi::ProcessCommandParametersL( CApaCommandLine &aCommandLine )
    {
    iParentWg = aCommandLine.ParentWindowGroupID();
    LOG1("parent window group id %d", iParentWg );
    
    return CAknAppUi::ProcessCommandParametersL( aCommandLine );
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void CMnrpAppUi::HandleResourceChangeL( TInt aType )
    {
    CAknAppUi::HandleResourceChangeL( aType );
    
    TRect rect( ClientRect() );
    if ( Cba() )
        {
		Cba()->SetBoundingRect( TRect() );
		Cba()->ReduceRect( rect );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::ShowMapL( CMnrpMapViewService& aService )
    {
    if ( !iMapView )
        {
        iMapView = CMnrpMapView::NewL( IsChainedMode(), iEngine );
        AddViewL( iMapView );
        }
        
    iMapView->SetMapModeL( aService );
    ActivateLocalViewL( iMapView->Id() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::ShowNavigationL( const CPosLandmark& aDestination )
    {
    if ( !iNaviView )
        {
        iNaviView = CMnrpNaviView::NewL( IsChainedMode(), iEngine );
        AddViewL( iNaviView );
        }
        
    iNaviView->SetDestinationL( aDestination );
    ActivateLocalViewL( iNaviView->Id() );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::StartSelectionL( 
    CMnrpMapViewService& aService,  
    MMapSelectionCallback& aCallback )
    {
	SetNaviPaneTitleL( aService.SelectionRequestText() );

    if ( aService.Options() & CMnMapView::EOptionRestrictSelection )
        {
        // select from dialog 
        SelectFromDialogL( aService, aCallback );
        }
    else
        {
        // select from map
        if ( !iMapView )
            {
            iMapView = CMnrpMapView::NewL( IsChainedMode(), iEngine );
            AddViewL( iMapView );
            }

        iMapView->SetSelectionModeL( aService, aCallback );
        ActivateLocalViewL( iMapView->Id() );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::StopSelection()
    {
    __ASSERT_DEBUG( iMapView, Panic( KErrGeneral ) );
    iMapView->StopSelection();

    // Selection is done - close selection view
    TRAP_IGNORE( ActivateLocalViewL( iAppView->Id() ) );
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppUi::SelectFromDialogL(
    CMnrpMapViewService& aService,
    MMapSelectionCallback& aCallback )
    {
    LOG("MnRefProvider::SelectFromDialogL in");

    const TInt KGranularity = 8;

    CDesCArraySeg *textArray = new (ELeave) CDesCArraySeg( KGranularity );
    CleanupStack::PushL( textArray );
    CArrayPtrSeg<CGulIcon>* iconArray = CreateIconListLC( KGranularity );

    TInt linkedLmsOffset = 0;

    CMnrpDocument* doc = ( CMnrpDocument* ) Document();
    CMnrpEngine& engine = doc->Engine();

    // add non-linked landmarks
    LOG("MnRefProvider::DoSelectionDialogL adding non-linked landmarks");
    for ( TInt i = 0; i < aService.LandmarksToShow().Count(); i++)
        {
        const CPosLandmark* lm = aService.LandmarksToShow()[i];
        AddLandmarkL( *lm, *textArray, *iconArray );
        }

    // add linked landmarks
    LOG("MnRefProvider::DoSelectionDialogL adding linked landmarks");

    // create dictionary of linked landmarks
    // in order to ease searching for return values
    // when completing request
    RArray<TInt> dbIndices;
    RArray<TPosLmItemId> lmIds;

    CleanupClosePushL( dbIndices );
    CleanupClosePushL( lmIds );

    linkedLmsOffset = textArray->Count();
    for ( TInt db = 0; db < aService.LandmarksToShowDatabases().Count(); db++ )
        {
        const HBufC* uri = aService.LandmarksToShowDatabases()[db];

        CPosLandmarkDatabase* lmdb = CPosLandmarkDatabase::OpenL( *uri );
        CleanupStack::PushL( lmdb );

        if ( lmdb->IsInitializingNeeded() )
            {
            ExecuteAndDeleteLD( lmdb->InitializeL() );
            }

        for ( TInt i = 0; i < aService.LinkedLandmarksToShow(db).Count(); i++)
            {
            TPosLmItemId id = aService.LinkedLandmarksToShow(db)[i];

            CPosLandmark* lm = lmdb->ReadLandmarkLC( id );
            AddLandmarkL( *lm, *textArray, *iconArray );
            
            dbIndices.AppendL( db );
            lmIds.AppendL( id );

            CleanupStack::PopAndDestroy( lm );
            }

        CleanupStack::PopAndDestroy( lmdb );
        }

    TInt selected = 0;
    CAknSelectionListDialog *dialog =
        CAknSelectionListDialog::NewL( selected, textArray, R_MNREFPROVIDER_MENUBAR );

    dialog->PrepareLC( R_MNREFPROVIDER_SELECTION_DIALOG );

    dialog->SetIconArrayL( iconArray );
    CleanupStack::Pop(); // iconArray cleanup

    LOG1("MnRefProvider::SelectFromDialogL launching dialog with %d items", textArray->Count() );
    TInt result = dialog->RunLD();
    if ( result )
        {
        LOG("MnRefProvider::SelectFromDialogL approved");
        if ( selected < linkedLmsOffset )
            {
            aCallback.HandleSelectionL( selected );
            }
        else
            {
            TInt linkedLmIndex = selected - linkedLmsOffset;
            if ( linkedLmIndex < dbIndices.Count() && 
                 linkedLmIndex < lmIds.Count() )
                {
                aCallback.HandleSelectionL( lmIds[linkedLmIndex], dbIndices[linkedLmIndex] );
                }
            else
                {
                // never happens
                LOG("MnRefProvider::SelectFromDialogL selection out of range, out");
                aCallback.HandleSelectionErrorL( KErrGeneral );
                }
            }
        }
    else
        {
        LOG("MnRefProvider::SelectFromDialogL cancelled, out");
        aCallback.HandleSelectionErrorL( KErrCancel );
        }

    CleanupStack::PopAndDestroy( &lmIds );
    CleanupStack::PopAndDestroy( &dbIndices );

    CleanupStack::PopAndDestroy( textArray );
    LOG("MnRefProvider::SelectFromDialogL out");
}    
