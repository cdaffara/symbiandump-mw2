/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: creates fetcher plugin dialog
*
*/


// INCLUDES
#include <eikclbd.h>            // for CColumnListBoxData
#include <coecobs.h>            // MCoeControlObserver
#include <AknUtils.h>           // AknTextUtils AknFind
#include <AknIconArray.h>       // for GulArray
#include <AknWaitDialog.h>
#include <textresolver.h>
#include <StringLoader.h>
#include <akninputblock.h>      // CAknInputBlock
#include <eikon.hrh>

#include <aknappui.h>
#include <akntoolbar.h>
#include <eikdialg.h>
#include <eikdialogext.h>
#include <aknselectionlist.h>   // AknSelectionList.cpp
#include <aknsfld.h>            // CAknSearchField
#include <eikcapc.h>            // CEikCaptionedControl

#include <aknconsts.h>          // KAvkonBitmapFile, KAvkonVariatedBitmapsFile
#include <AknsConstants.h>      // icon constants
#include <aknmemorycardui.mbg>  // memory card icon
#include <avkon.mbg>
#include <StringLoader.h>

#include <MMGFetchVerifier.h>   // For VerifySelectionL()

#include <audiofetcher.mbg>
#include <audiofetcherdialog.rsg>
#include "audiofetcherdialog.h"
#include "audiofetcherfilehandler.h"

#include "audiofetcher.hrh"
#include "audiofetcherlog.h"

#include <data_caging_path_literals.hrh> // KDC_APP_RESOURCE_DIR

     
//-----------------------------------------------------------------------------
// C++ default constructor.
//-----------------------------------------------------------------------------
//
inline CAudioFetcherDialog::CAudioFetcherDialog(
            CDesCArray& aSelectedFiles, 
            MMGFetchVerifier* aVerifier,
            const TDesC& aTitle,
            TBool aMultiSelectionEnabled,
            TMediaFileType aMediaType)
        : iSelectedFiles( aSelectedFiles ),
          iVerifier( aVerifier ),
          iTitle( aTitle ),
          iMultiSelectionEnabled(aMultiSelectionEnabled),
          iMediaType(aMediaType)
	{
	WLOG("CAudioFetcherDialog::CAudioFetcherDialog");
	// No implementation required
	}

//-----------------------------------------------------------------------------
// Two-phased constructor.
//-----------------------------------------------------------------------------
//
CAudioFetcherDialog* CAudioFetcherDialog::NewL(CDesCArray& aSelectedFiles, 
               MMGFetchVerifier* aVerifier,  const TDesC& aTitle, TBool aMultiSelectionEnabled,
               TMediaFileType aMediaType)
	{
	WLOG("CAudioFetcherDialog::NewL");
	CAudioFetcherDialog* self=
	    new( ELeave ) CAudioFetcherDialog(
	        aSelectedFiles, aVerifier, aTitle, aMultiSelectionEnabled, aMediaType );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
   	}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
//
CAudioFetcherDialog::~CAudioFetcherDialog()
	{
	WLOG("CAudioFetcherDialog::~CAudioFetcherDialog >");

	delete iStatusPaneHandler;
	delete iListHandler;
	delete iFileHandler;
	delete iListBox;
	delete iEmptyListText;
	delete iIdle;
	delete iInputBlock;
	
	// Restore the Toolbar as it was in the Calling application
	if( iAvkonAppUi->CurrentFixedToolbar() )  // there is Hardware Specific Output for Fixed ToolBar
        {
        iAvkonAppUi->CurrentFixedToolbar()->SetToolbarVisibility( ETrue );
        }

    if ( iAvkonAppUi )
        {
        iAvkonAppUi->RemoveFromStack( this );
        }
	
	WLOG("CAudioFetcherDialog::~CAudioFetcherDialog <");
	}

//-----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// Create the alfscreenbuffer with a generic buffer screen id
//-----------------------------------------------------------------------------
//
void CAudioFetcherDialog::ConstructL()
	{
	WLOG("CAudioFetcherDialog::ConstructL >");
	// always first call the base class
    CAknDialog::ConstructL( R_AUDIOFETCHER_MENUBAR ); //R_MULTISELECT_AUDIOFETCHER_MENUBAR );
    
    // Get the Instance of the toolbar and disable it as it is not required in the fetcher dialog
    if( iAvkonAppUi->CurrentFixedToolbar() )  // there is hardware specific output for fixed toolBar
        {
        iAvkonAppUi->CurrentFixedToolbar()->SetToolbarVisibility( EFalse );
        }
    
	// get previous title so it can be restored
	iStatusPaneHandler = CStatusPaneHandler::NewL( iAvkonAppUi );
	iStatusPaneHandler->StoreOriginalTitleL();
	
	// MdS query handler
    iFileHandler = CAudioFetcherFileHandler::NewL();
    iFileHandler->SetObserver( this );

    // List model
    iListHandler = CAudioFetcherListHandler::NewL();
    iListHandler->SetObserver( this );
    
    WLOG("CAudioFetcherDialog::ConstructL <");
    }
    
//-----------------------------------------------------------------------------
// CAudioFetcherDialog::ProcessCommandL
// This processes the events to the OkToExitL
//-----------------------------------------------------------------------------
//
void CAudioFetcherDialog::ProcessCommandL(TInt aCommandId)
	{
	WLOG("CAudioFetcherDialog::ProcessCommandL");
	switch( aCommandId )
		{
		case EAknSoftkeyCancel :
			{
			TryExitL( aCommandId );
			break;
			}
		case EAknSoftkeySelect :
		case ECmdSelectMarked :
        case EAknSoftkeyOk :
            {
			TryExitL( aCommandId );
			break;
            }
		default :
		    {
            break;
		    }
		}
	}
//-----------------------------------------------------------------------------
// CAudioFetcherDialog::OkToExitL
//-----------------------------------------------------------------------------
//
TBool CAudioFetcherDialog::OkToExitL(TInt aKeycode)
	{
	WLOG("CAudioFetcherDialog::OkToExitL");
    TBool retVal = EFalse;
    
    switch( aKeycode )
        {
        case EAknSoftkeySelect :
            {       
            TInt index = CurrentItemListIndex();
            TFileName fileName;
            
            iFileHandler->GetAttribute( index, CAudioFetcherFileHandler::EAttrFullName, fileName, 0 );
            iSelectedFiles.AppendL( fileName );
            
            // Retreives the uri's of the selected files and verifies if it is a 
            // supported format
            if ( iVerifier && !iVerifier->VerifySelectionL( &iSelectedFiles ) )
                {
                iSelectedFiles.Reset();
                retVal = EFalse;
                }
            else
                {
                retVal = ETrue;                
                }
           break;
            }
        case EAknSoftkeyOk:
            {
            retVal = ETrue;
            }
        case EAknSoftkeyCancel : // exit dialog
        case EAknCmdExit :
            {
            retVal = ETrue;
            break;
            }
        default :
            break;
        }
    return retVal;
	}

//-----------------------------------------------------------------------------
// CAudioFetcherDialog::SizeChanged
//-----------------------------------------------------------------------------
//
void CAudioFetcherDialog::SizeChanged()
	{
	WLOG("CAudioFetcherDialog::SizeChanged");
	// Can be implemented based on the use.						
	CAknDialog::SizeChanged();
	
	TRect parentRect( Rect() );
	if ( iListBox )
	    {
	    iListBox->SetRect( parentRect );
	    }
	}

//-----------------------------------------------------------------------------
// CAudioFetcherDialog::PreLayoutDynInitL
//-----------------------------------------------------------------------------
//
void CAudioFetcherDialog::PreLayoutDynInitL()
    {
    WLOG("CAudioFetcherDialog::PreLayoutDynInitL >");
    
    iListBox = new ( ELeave ) CAknSingleGraphicStyleListBox;
    iListBox->ConstructL( this, EAknListBoxSelectionList );
    
    iListBox->Model()->SetItemTextArray( iListHandler );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );
    
    iListBox->SetListBoxObserver( this );
    iListBox->CreateScrollBarFrameL( ETrue );
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                     CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto );
    
    iListBox->SetListBoxObserver( this );

    SetIconsL();
    
    iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL( ETrue );
    
    if ( iMediaType == EAnyMediaFile )
        {
        iEmptyListText = StringLoader::LoadL( R_AUDIOFETCHER_EMPTY_LIST_TEXT_NO_MEDIA );
        }
    else
        {
        iEmptyListText = StringLoader::LoadL( R_AUDIOFETCHER_EMPTY_LIST_TEXT_NO_AUDIO );        
        }

    
    
    iListBox->View()->SetListEmptyTextL( *iEmptyListText );
    
    iStatusPaneHandler->SetTitleL( iTitle );
    
    CEikButtonGroupContainer& dialogCba = ButtonGroupContainer(); 
    dialogCba.MakeCommandVisible(EAknSoftkeySelect, EFalse);

    WLOG("CAudioFetcherDialog::PreLayoutDynInitL <");
    }

//-----------------------------------------------------------------------------
// CAudioFetcherDialog::PostLayoutDynInitL
//-----------------------------------------------------------------------------
//
void CAudioFetcherDialog::PostLayoutDynInitL()
    {
    WLOG("CAudioFetcherDialog::PostLayoutDynInitL");
    }

// ----------------------------------------------------------------------------
// CAudioFetcherDialog::SetIconsL
// ----------------------------------------------------------------------------
//
void CAudioFetcherDialog::SetIconsL()
    {
    WLOG("CAudioFetcherDialog::SetIconsL >");
    
    _LIT( KBitmapFile, "AudioFetcher.mbm" );
    
        
    if ( !iListBox )
        {
        return;
        }
    
    SetIconFileNameL( KBitmapFile, iIconFileName );
    
    CArrayPtr<CGulIcon>* icons = new (ELeave) CAknIconArray(1);
    CleanupStack::PushL( icons );
    
    // NOTE: append order must match TListIcons
    //
    
    // music item icon
    icons->AppendL( IconL( KAknsIIDQgnIndiAiNtMusic, iIconFileName,
            EMbmAudiofetcherQgn_indi_ai_nt_music,
            EMbmAudiofetcherQgn_indi_ai_nt_music_mask ) );

    // tone item icon, default tone icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupAudio, iIconFileName,
            EMbmAudiofetcherQgn_prop_mup_audio,
            EMbmAudiofetcherQgn_prop_mup_audio_mask ) );

    // video item icon
    icons->AppendL( IconL( KAknsIIDQgnPropFmgrFileVideo, iIconFileName,
            EMbmAudiofetcherQgn_prop_fmgr_file_video,
            EMbmAudiofetcherQgn_prop_fmgr_file_video_mask ) );

    // recording item icon
    icons->AppendL( IconL( KAknsIIDQgnPropFmgrFileVoicerec, iIconFileName,
            EMbmAudiofetcherQgn_prop_fmgr_file_voicerec,
            EMbmAudiofetcherQgn_prop_fmgr_file_voicerec_mask ) );

    // artist icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupArtist, iIconFileName,
            EMbmAudiofetcherQgn_prop_mup_artist,
            EMbmAudiofetcherQgn_prop_mup_artist_mask ) );

    // album icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupAlbum, iIconFileName,
            EMbmAudiofetcherQgn_prop_mup_album,
            EMbmAudiofetcherQgn_prop_mup_album_mask ) );

    // genre icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupGenre, iIconFileName,
            EMbmAudiofetcherQgn_prop_mup_genre,
            EMbmAudiofetcherQgn_prop_mup_genre_mask ) );

    // composer icon
    icons->AppendL( IconL( KAknsIIDQgnPropMupComposer, iIconFileName,
            EMbmAudiofetcherQgn_prop_mup_composer,
            EMbmAudiofetcherQgn_prop_mup_composer_mask ) );

    // folder icon (tones, music, videos, recordings, all music, artist,
    // album, genre, composer, artist all)
    icons->AppendL( IconL( KAknsIIDQgnPropFolderSmall, iIconFileName,
            EMbmAudiofetcherQgn_prop_folder_small,
            EMbmAudiofetcherQgn_prop_folder_small_mask ) );

    // unknown folder icon
    icons->AppendL( IconL( KAknsIIDQgnPropMceUnknownRead, iIconFileName,
            EMbmAudiofetcherQgn_prop_mce_unknown_read,
            EMbmAudiofetcherQgn_prop_mce_unknown_read ) );

    // download item icon (null item)
    icons->AppendL( IconL( KAknsIIDQgnPropLinkEmbdSmall, iIconFileName,
            EMbmAudiofetcherQgn_prop_link_embd_small,
            EMbmAudiofetcherQgn_prop_link_embd_small_mask ) );

    // off item icon (null item)
    icons->AppendL( IconL( KAknsIIDQgnPropSmlSyncOff, iIconFileName,
            EMbmAudiofetcherQgn_prop_sml_sync_off,
            EMbmAudiofetcherQgn_prop_sml_sync_off_mask ) );

    // memory card icon
    icons->AppendL( ColorIconL( KAknsIIDQgnIndiMmcAdd, iIconFileName,
            EMbmAudiofetcherQgn_indi_mmc_add,
            EMbmAudiofetcherQgn_indi_mmc_add_mask,
            KAknsIIDQsnIconColors,
            EAknsCIQsnIconColorsCG26 ) );

     // mass storage icon
    icons->AppendL( ColorIconL( KAknsIIDQgnPropLinkEmbdSmall, iIconFileName,
            EMbmAudiofetcherQgn_indi_fmgr_ms_add,
            EMbmAudiofetcherQgn_indi_fmgr_ms_add_mask,
            KAknsIIDQsnIconColors,
            EAknsCIQsnIconColorsCG26 ) );

    // empty icon
    icons->AppendL( IconL( KAknsIIDQgnPropEmpty, KAvkonBitmapFile,
                           EMbmAvkonQgn_prop_empty,
                           EMbmAvkonQgn_prop_empty_mask ) );
    
    // image item icon
    icons->AppendL( IconL( KAknsIIDQgnPropFmgrFileImage, iIconFileName,
            EMbmAudiofetcherQgn_prop_fmgr_file_image,
            EMbmAudiofetcherQgn_prop_fmgr_file_image_mask ) );
    
    // delete old icons
    CArrayPtr<CGulIcon>* arr = iListBox->ItemDrawer()->ColumnData()->IconArray();
    if ( arr )
        {
        arr->ResetAndDestroy();
        delete arr;
        arr = NULL;
        }

    iListBox->ItemDrawer()->ColumnData()->SetIconArray( icons );
    CleanupStack::Pop( icons );
    
    WLOG("CAudioFetcherDialog::SetIconsL <");
    }

// ----------------------------------------------------------------------------
// CAudioFetcherDialog::SetIconFileNameL
//
// ----------------------------------------------------------------------------
//
void CAudioFetcherDialog::SetIconFileNameL( const TDesC& aName, TDes& aFullName )
    {
    WLOG("CAudioFetcherDialog::SetIconFileNameL >");
    _LIT( KDriveZ, "z:" );
    
    aFullName = KNullDesC;
    
    aFullName.Append( KDriveZ );
    aFullName.Append( KDC_APP_RESOURCE_DIR );
    aFullName.Append( aName );
    
    WLOG("CAudioFetcherDialog::SetIconFileNameL <");
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog::IconL
// 
// -----------------------------------------------------------------------------
//
CGulIcon* CAudioFetcherDialog::IconL(TAknsItemID aId, const TDesC& aFileName,
                                   TInt aFileIndex, TInt aFileMaskIndex)
    {
    WLOG("CAudioFetcherDialog::IconL");
    CGulIcon* icon = AknsUtils::CreateGulIconL(AknsUtils::SkinInstance(), aId,
                                aFileName, aFileIndex, aFileMaskIndex);
    return icon;    
    }

// -----------------------------------------------------------------------------
// CMediaFileDialog::ColorIconL
// 
// -----------------------------------------------------------------------------
//
CGulIcon* CAudioFetcherDialog::ColorIconL( const TAknsItemID& aId,
                                           const TDesC& aFileName,
                                           TInt aFilexIndex,
                                           TInt aFileMaskIndex,
                                           const TAknsItemID& aColorId,
                                           TInt aColorIndex )
    {
    WLOG("CAudioFetcherDialog::ColorIconL");
    
    CFbsBitmap* bitmap( NULL );
    CFbsBitmap* mask( NULL );

    if ( aColorId == KAknsIIDNone )
        {
        // do not use theme color, use the default color from the file
        AknsUtils::CreateIconLC( AknsUtils::SkinInstance(), aId,
            bitmap, mask, aFileName, aFilexIndex, aFileMaskIndex );
        }
    else
        {
        // use theme color
        AknsUtils::CreateColorIconLC( AknsUtils::SkinInstance(), aId, aColorId, aColorIndex,
            bitmap, mask, aFileName, aFilexIndex, aFileMaskIndex, KRgbBlack );
        }

    CGulIcon* icon = CGulIcon::NewL( bitmap, mask );
    icon->SetBitmapsOwnedExternally( EFalse );

    // icon now owns the bitmaps, no need to keep on cleanup stack.
    CleanupStack::Pop( 2 ); // mask, bitmap
    
    return icon;
    }


// -----------------------------------------------------------------------------
// CMediaFileDialog::UpdateListBoxL
// -----------------------------------------------------------------------------
//
void CAudioFetcherDialog::UpdateListBoxL()
    {
    WLOG("CAudioFetcherDialog::UpdateListBoxL >");
    
    if ( !iListBox )
        {
        return;
        }

    iListBox->HandleItemAdditionL();
    iListBox->SetCurrentItemIndex( 0 );    
    DrawNow();    
    
    WLOG("CAudioFetcherDialog::UpdateListBoxL <");
    }

//------------------------------------------------------------------------------
// CAudioFetcherDialog::ActivateL
//
// Called by system when dialog is activated.
//------------------------------------------------------------------------------
//
void CAudioFetcherDialog::ActivateL()
    {
    WLOG("CAudioFetcherDialog::ActivateL >");
    CAknDialog::ActivateL();

    // this cannot be in ConstructL which is executed before dialog is launched
    iAvkonAppUi->AddToStackL( this );
    WLOG("CAudioFetcherDialog::ActivateL <");
    }

//-----------------------------------------------------------------------------
// CAudioFetcherDialog::DynInitMenuPaneL
//-----------------------------------------------------------------------------
void CAudioFetcherDialog::DynInitMenuPaneL(
        TInt /*aResourceId*/, CEikMenuPane* /*aMenuPane*/ )
    {
    WLOG("CAudioFetcherDialog::DynInitMenuPaneL");
    // No Implementation
    }


// ---------------------------------------------------------------------------
// CAudioFetcherDialog::CountComponentControls
//
// Gets the number of controls contained in a compound control.
// ---------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::CountComponentControls() const
    {
    return iListBox ? 1 : 0;
    }


// ---------------------------------------------------------------------------
// CAudioFetcherDialog::CountComponentControls
//
// ---------------------------------------------------------------------------
//
CCoeControl* CAudioFetcherDialog::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iListBox;
    }

//-----------------------------------------------------------------------------
// CAudioFetcherDialog::OfferKeyEventL
//-----------------------------------------------------------------------------
TKeyResponse CAudioFetcherDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aType)
    {
    WLOG("CAudioFetcherDialog::OfferKeyEventL");
    TKeyResponse keyResponse( EKeyWasNotConsumed );

	// Escape key was pressed and dialog needs to close
	if ( aKeyEvent.iCode == EKeyEscape && aType == EEventKey )
		{
		TryExitL( EAknSoftkeyCancel );
		keyResponse = EKeyWasConsumed;
		}
	else
		{
		keyResponse = iListBox->OfferKeyEventL( aKeyEvent, aType );
		}
    return keyResponse;
    }

// -----------------------------------------------------------------------------
// CAudioFetcherDialog::HandleListBoxEventL (from MEikListBoxObserver)
// -----------------------------------------------------------------------------
//
void CAudioFetcherDialog::HandleListBoxEventL( CEikListBox* /*aListBox*/,
                                             TListBoxEvent aEventType )
    {
    WLOG("CAudioFetcherDialog::HandleListBoxEventL");
    
    switch ( aEventType )
        {
        case EEventItemSingleClicked:   // fallthrough
        case EEventEnterKeyPressed:
            {
            TBool closeDialog = HandleListSelectionL();
            if ( closeDialog )
                {
                // close after short delay (dialog cannot be closed from this function)
                CloseDialogWithDelayL();
                }
            break;
            }
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CAudioFetcherDialog::CloseDialogWithDelayL
// -----------------------------------------------------------------------------
//
void CAudioFetcherDialog::CloseDialogWithDelayL()
    {    
    delete iIdle;
    iIdle = NULL;
    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    
    if ( iIdle )
        {                    
        iIdle->Start( TCallBack( CloseDialog, this ) );
        }
    delete iInputBlock;
    iInputBlock = NULL;
    iInputBlock = CAknInputBlock::NewLC();
    CleanupStack::Pop( iInputBlock );
    }

// -----------------------------------------------------------------------------
// CAudioFetcherDialog::CloseDialog
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::CloseDialog( TAny *aObj )
    {    
    CAudioFetcherDialog *dlg = (CAudioFetcherDialog*) aObj;
    delete dlg->iInputBlock;
    dlg->iInputBlock = NULL;
    
    TRAP_IGNORE( dlg->TryExitL( EAknSoftkeyOk ) );
    
    return 0;
    }

// -----------------------------------------------------------------------------
// CAudioFetcherDialog::ConstructListboxItem
// -----------------------------------------------------------------------------
//
TPtrC CAudioFetcherDialog::ConstructListboxItem( TInt aListboxIndex )
    {
    WLOG("CAudioFetcherDialog::ConstructListboxItem");
    _LIT( KFormat2, "%d\t%S\t%d" );

    TInt mediaTypeIcon = EEIconEmpty;
    TInt storageTypeIcon = EEIconEmpty;

    GetListItemText( aListboxIndex, iListboxItemText1 );
    mediaTypeIcon = MediaTypeIcon( aListboxIndex );
    storageTypeIcon = StorageTypeIcon( aListboxIndex );
        
    iListboxItemText2.Format( KFormat2, mediaTypeIcon, &iListboxItemText1, storageTypeIcon );
    return iListboxItemText2;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherDialog::StorageTypeIcon
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::StorageTypeIcon( TInt aListboxIndex )
    {
    WLOG("CAudioFetcherDialog::StorageTypeIcon");
    if ( aListboxIndex < 0 )
        {
        return EEIconEmpty;
        }

    TInt storageType = iFileHandler->Attribute( aListboxIndex,
            CAudioFetcherFileHandler::EAttrStorageType, 0 );
    TInt iconId = EEIconEmpty;

    if ( storageType == CAudioFetcherFileHandler::EMemoryCard )
        {
        iconId = EEIconMemoryCard;
        }
	else if ( storageType == CAudioFetcherFileHandler::EMassStorage )
		{
		iconId = EEIconMassStorage;
		}

    return iconId;
    }


// -----------------------------------------------------------------------------
// CAudioFetcherDialog::MediaTypeIcon
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::MediaTypeIcon( TInt aListboxIndex )
    {
    WLOG("CAudioFetcherDialog::MediaTypeIcon");
    if ( aListboxIndex < 0 )
        {
        return EEIconEmpty;
        }

    TInt mediaType = iFileHandler->Attribute( aListboxIndex,
            CAudioFetcherFileHandler::EAttrMediaType, 0 );
    
    TInt iconId = EEIconEmpty;
    
    switch ( mediaType )
        {
        case CAudioFetcherFileHandler::EMediaTypeImage:
            {
            iconId = EEIconImage;
            break;
            }
        case CAudioFetcherFileHandler::EMediaTypeVideo:
            {
            iconId = EEIconVideo;
            break;
            }
        case CAudioFetcherFileHandler::EMediaTypeTone:
            {
            iconId = EEIconTone;
            break;
            }
        case CAudioFetcherFileHandler::EMediaTypeRecording:
            {
            iconId = EEIconRecording;
            break;
            }
        case CAudioFetcherFileHandler::EMediaTypeMusic:
            {
            iconId = EEIconMusic;
            break;
            }
        default:
            break;
        }
    
    return iconId;
    }

// -----------------------------------------------------------------------------
// CAudioFetcherDialog::ListboxItemCount (from MAudioFetcherListHandlerObserver)
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::ListboxItemCount()
    {
    WLOG("CAudioFetcherDialog::ListboxItemCount");
    return iFileHandler->ResultCount();  
    }



// -----------------------------------------------------------------------------
// CAudioFetcherDialog::HandleFileEventL (from MAudioFetcherFileHandlerObserver)
// -----------------------------------------------------------------------------
//
void CAudioFetcherDialog::HandleFileEventL( TInt aEvent, TInt aError )
    {
    WLOG("CAudioFetcherDialog::HandleFileEventL >");
    // handle cancel button
    if ( aEvent == MAudioFetcherFileHandlerObserver::EQueryCanceled )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL == EQueryCanceled");
        UpdateListBoxL();
        return;
        }

    // handle error
    if ( aError != KErrNone )
        {
        WLOG1("### CAudioFetcherDialog::HandleFileEventL (%d) ###", aError );
        return;
        }

    if ( aEvent != MAudioFetcherFileHandlerObserver::EUnknownQueryComplete )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL != EUnknownQueryComplete");  
        }
    
    if ( aEvent == MAudioFetcherFileHandlerObserver::EUnknownQueryComplete )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL == EUnknownQueryComplete");
        }

    if ( aEvent == MAudioFetcherFileHandlerObserver::EQueryComplete )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL == EQueryComplete");
  
        
        if ( iMediaType == EAnyMediaFile )
            {
            // in case of all media files, fetch video files next
            if ( iFileHandler->QueryType() == CAudioFetcherFileHandler::EMediaTypeMusic )
                {
                iFileHandler->QueryVideoL();               
                }
            else if ( iFileHandler->QueryType() == CAudioFetcherFileHandler::EMediaTypeVideo )
                {
                iFileHandler->QueryImageL();
                }
            else
                {
                UpdateListBoxL();
                }
            
            }
        else
            {
            UpdateListBoxL(); 
            }
        }

    if ( aEvent == MAudioFetcherFileHandlerObserver::EMediaFileChanged )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL == EMediaFileChanged");
        }
    
    if ( aEvent == MAudioFetcherFileHandlerObserver::EInitComplete )
        {
        WLOG("CAudioFetcherDialog::HandleFileEventL == EInitComplete");
        iDatabaseOpen = ETrue;
        QueryL();
        }
    
    WLOG("CAudioFetcherDialog::HandleFileEventL <");
    }


// -----------------------------------------------------------------------------
// CAudioFetcherDialog::GetListItemText
// 
// -----------------------------------------------------------------------------
//
void CAudioFetcherDialog::GetListItemText( TInt aListboxIndex, TDes& aText )
    {
    WLOG("CAudioFetcherDialog::GetListItemText");
    iFileHandler->GetAttribute( aListboxIndex, 
                       CAudioFetcherFileHandler::EAttrSongName, aText, 0 );
    }



// -----------------------------------------------------------------------------
// CAudioFetcherDialog::CurrentItemListIndex
// 
// -----------------------------------------------------------------------------
//
TInt CAudioFetcherDialog::CurrentItemListIndex()
    {
    WLOG("CAudioFetcherDialog::CurrentItemListIndex");
    TInt itemCount = iListBox->Model()->NumberOfItems();
    TInt currentIndex = iListBox->CurrentItemIndex();
     
    if ( itemCount == 0 || currentIndex < 0 )
        {
        return KErrNotFound; // list or find box is empty
        }
    return currentIndex;
    }


//------------------------------------------------------------------------------
// CMediaFileDialog::QueryL
//
//------------------------------------------------------------------------------
//
void CAudioFetcherDialog::QueryL()
    {
    WLOG("CAudioFetcherDialog::QueryL >");
    if ( !iFileHandler->QueryReady() || !iDatabaseOpen )
        {
        WLOG("CAudioFetcherDialog::QueryL: query in progress");
        return;
        }

    iFileHandler->EnableObserverCall( ETrue );
    iFileHandler->SetQueryId( 0 );
    iFileHandler->QueryAudioL();

    WLOG("CAudioFetcherDialog::QueryL <");
    }

// ----------------------------------------------------------------------------
// CAudioFetcherDialog::HandleResourceChange
// ----------------------------------------------------------------------------
//
void CAudioFetcherDialog::HandleResourceChange(TInt aType)
    {         
    WLOG("CAudioFetcherDialog::HandleResourceChange");
    if (aType == KAknsMessageSkinChange)
        {
        TRAP_IGNORE( SetIconsL() );
        }
    else if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( 
        AknLayoutUtils::EMainPane, mainPaneRect );
        SetRect( mainPaneRect );
        TRAP_IGNORE( iStatusPaneHandler->SetTitleL( iTitle ) );
        DrawDeferred();
        }
        
    CCoeControl::HandleResourceChange( aType );
    }


// ----------------------------------------------------------------------------
// CAudioFetcherDialog::HandleListSelection
// ----------------------------------------------------------------------------
//
TBool CAudioFetcherDialog::HandleListSelectionL()
    {
    TBool closeDialog = EFalse;
    
    TInt index = CurrentItemListIndex();
    TFileName fileName;
    
    iFileHandler->GetAttribute( index, CAudioFetcherFileHandler::EAttrFullName, fileName, 0 );
    iSelectedFiles.AppendL( fileName );
    
    // Retreives the uri's of the selected files and verifies if it is a 
    // supported format
    if ( iVerifier && !iVerifier->VerifySelectionL( &iSelectedFiles ) )
        {
        iSelectedFiles.Reset();
        closeDialog = EFalse;
        }
    else
        {
        closeDialog = ETrue;                
        }
    return closeDialog;
    }

//  End of File
