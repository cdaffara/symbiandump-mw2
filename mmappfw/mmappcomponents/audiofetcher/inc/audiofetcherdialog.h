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
* Description: 
*
*/

#ifndef AUDIOFETCHERDIALOG_H
#define AUDIOFETCHERDIALOG_H

// INCLUDES
#include <e32base.h>
#include <aknlists.h>
#include <AknDialog.h>
#include <coedef.h>
#include <eikcmobs.h>
#include <MediaFileTypes.hrh>

#include "audiofetcherlisthandler.h"
#include "audiofetcherfilehandler.h"
#include "audiofetcherdialogutils.h"

// FORWARD DECLARATIONS
class MMGFetchVerifier;
class CAknInputBlock;

/**
* CAudioFetcherDialog
*
* CAudioFetcherDialog provides UI for selecting audio files.
*/
NONSHARABLE_CLASS ( CAudioFetcherDialog ) : public CAknDialog,
                                             public MEikListBoxObserver,/*
                                             public MCoeForegroundObserver,*/
                                             public MEikCommandObserver,
                                             public MAudioFetcherListHandlerObserver,
                                             public MAudioFetcherFileHandlerObserver
	{
	 public:  // Constructors and destructor
	     
		/**
		* Two-phased constructor.
		* @param reference to an array where the selected files are set
        * @param selection verifier interface
        * @param the title for the dialog
		*/
        static CAudioFetcherDialog* NewL(
            CDesCArray& aSelectedFiles, 
            MMGFetchVerifier* aVerifier,
            const TDesC& aTitle,
            TBool aMultiSelectionEnabled, 
            TMediaFileType aMediaType );

        /**
        * Destructor.
        */
        virtual ~CAudioFetcherDialog();     

	private: // Constructors
		
	    /**
        * Symbian 2nd phase constructor
		*/
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CAudioFetcherDialog(
            CDesCArray& aSelectedFiles, MMGFetchVerifier* aVerifier, const TDesC& aTitle, 
            TBool aMultiSelectionEnabled, TMediaFileType aMediaType);

	private:
	    
		/**
		* @ref CCoeControl::SizeChanged
		*/
		void SizeChanged();
	   
	    /**
	    *  From the base class.
	    * Handles menu events.
	    * @param  aCommandId Command id.
	    * @return None.
	    */
	    void ProcessCommandL( TInt aCommandId );
	        
        //void HandleGainingForeground() {};
        //void HandleLosingForeground(){};
        
	    /**
        * From MEikListBoxObserver, called by framework.
        * @param aListBox.
        * @param aEventType.
        * @return None
        */
        void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );
        
        /**
        * Initializes the dialog's controls before the dialog is sized and 
        * layed out.
        */
		void PreLayoutDynInitL();

        /**
         * Initializes the dialog's controls after the dialog has been sized 
         * but before it has been activated.
         */
		void PostLayoutDynInitL();
		
		TInt CurrentItemListIndex();
		
		void GetListItemText( TInt aListboxIndex, TDes& aText );

		void SetIconsL();
		
		void SetIconFileNameL( const TDesC& aName, TDes& aFullName );
		
		TInt StorageTypeIcon( TInt aListboxIndex );
		
		TInt MediaTypeIcon( TInt aListboxIndex );
		
		CGulIcon* IconL(TAknsItemID aId, const TDesC& aFileName,
		                TInt aFileIndex, TInt aFileMaskIndex);
		
		CGulIcon* ColorIconL( const TAknsItemID& aId,
		                      const TDesC& aFileName,
		                      TInt aFilexIndex,
		                      TInt aFileMaskIndex,
		                      const TAknsItemID& aColorId,
		                      TInt aColorIndex );
		
		void UpdateListBoxL();
		
        /**
        * From the base class.
        * Called by framework before exiting the dialog.
        * @param aButtonId Button id.
        * @return ETrue to exit\ EFalse to not to exit.
        */
        TBool OkToExitL( TInt aButtonId );

        /**
        * From the base class.
        * Called by framework when dialog is activated.
        * @param None.
        * @return None.
        */
        void ActivateL();
        
        /**
        * From the base class.
        * Called by framework before menu is shown.
        * @param aResourceId Menu resource id.
        * @param aMenuPane Pointer to the menu.
        * @return None.
        */
        void  DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane) ;
        
        /**
        * From the base class.
        * Called by framework for key event handling.
        * @param aKeyEvent.
        * @param aType.
        * @return Return code.
        */
        TKeyResponse OfferKeyEventL(const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/);
        
        /**
        * From base class.
        * Called when UI layout changes. 
        * @param aType.
        * @return None.
        */
        void HandleResourceChange( TInt aType );
        
        virtual CCoeControl* ComponentControl( TInt aIndex ) const;
        virtual TInt CountComponentControls() const;

    private:   // from MAudioFetcherFileHandlerObserver
        
        void HandleFileEventL( TInt aEvent, TInt aError );

        TPtrC ConstructListboxItem( TInt aListboxIndex );
        TInt ListboxItemCount();
        
    private:
        
        /**
         * Query audio files from MdS
         */
        void QueryL();
        TBool HandleListSelectionL();
        static TInt CloseDialog( TAny *aObj );
        void CloseDialogWithDelayL();
        
    private:
	    
	    // listbox
	    CAknSingleGraphicStyleListBox* iListBox;
	    
	   // long buffers for listbox items
	   TBuf<128> iListboxItemText1;
	   TBuf<256> iListboxItemText2;
	        
	    // long buffers for icon file names
	    TFileName iIconFileName;
        
	    // for title and icon handling
	    CStatusPaneHandler* iStatusPaneHandler;
	    TBuf<128> iDialogTitle;
	    
    	// metadata engine wrapper
    	CAudioFetcherFileHandler* iFileHandler;
    	
    	CAudioFetcherListHandler* iListHandler;
    	
        // Selected items array
        // Not Own. Owned by the client
        CDesCArray& iSelectedFiles;
        
        MMGFetchVerifier* iVerifier;
        
        // is metadata databse open
        TBool iDatabaseOpen;

        /// Ref: the title
        const TDesC& iTitle;         
        
        HBufC* iEmptyListText;

        // Holds the status pane layout resource ID
        TInt iCurrentLayoutResId;
        TBool iMultiSelectionEnabled;
        
        // for closing dialog
        CIdle* iIdle;
        CAknInputBlock* iInputBlock;
        
        TMediaFileType iMediaType;
	};

#endif // AUDIOFETCHERDIALOG_H

// End of File

