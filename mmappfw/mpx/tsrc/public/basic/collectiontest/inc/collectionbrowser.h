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

#ifndef COLLECTIONBROWSER_H
#define COLLECTIONBROWSER_H

// INCLUDES
#include <mpxcollectionobserver.h>
#include "testmenuview.h"

// MACROS

// FORWARD DECLARATIONS
class MMPXCollectionUtility;
class CMPXCollectionPath;
class CStifLogger;
class CTestPlaybackUtility;

// DATA TYPES
_LIT(KTxtCollectionBrowser, "CollectionBrowser");

// CLASS DECLARATION

class CCollectionBrowser : public CTestMenuView,
                         public MMPXCollectionObserver
    {
    public:
        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
//        static CCollectionBrowser* NewL(CConsoleMain* aConsoleMain,
 //                                       CTestBaseView* aParent,
  //                                      const TDesC16& aName,
   //                                     CStifLogger* aLogger,
    //                                    CMPXCollectionPath* aPath = NULL);
                                             
        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
        static CCollectionBrowser* NewLC(CConsoleMain* aConsoleMain,
                                         CTestBaseView* aParent,
                                         const TDesC16& aName,
                                         CStifLogger* aLogger,
                                         CMPXCollectionPath* aPath = NULL);

        /**
        * Destructor of CConsoleMain
        */
        virtual ~CCollectionBrowser();
        
    public:     //From base classes
        /**
        * From CTestBaseView
        * Initialize the view before display view
        */
        void InitializeViewL();
        
        /**
        * From CTestBaseView
        * Cleanup the view before deactivate/destroy view
        */
        void CleanupViewL();
        
        /**
        * From CTestBaseView
        * Handle number key press
        */
        void HandleNumKeyL();
        
        /**
        * From CTestBaseView
        * Handle left key press
        */
        void HandleLeftKeyL();
        
        /**
        * From CTestBaseView
        * Handle right/enter key press
        */
        void HandleRightKeyL();
     
        /**
        *  From MMPXCollectionObserver
        *  Handle extended media properties
        *  @param aMedia media 
        *  @param aError error code    
        */
        void HandleCollectionMediaL(
                        const CMPXMedia& aMedia, 
                        TInt aError);
            
        /**
        *  From MMPXCollectionObserver
        *  Handle collection message
        *  @param aMessage collection message
        */
        void HandleCollectionMessageL(const CMPXMessage& aMsg); //const TMPXCollectionMessage& aMessage);

        /**
        *  From MMPXCollectionObserver
        *  Handles the collection entries being opened. Typically called
        *  when client has Open()'d a folder
        *
        *  @param aEntries collection entries opened
        *  @param aIndex focused entry
        *  @param aComplete ETrue no more entries. EFalse more entries
        *                   expected
        *  @param aError error code   
        */
        void HandleOpenL(const CMPXMedia& aEntries,
                                 TInt aIndex,TBool aComplete,TInt aError);
        
        /**
        *  From MMPXCollectionObserver
        *  Handles the item being opened. Typically called
        *  when client has Open()'d an item. Client typically responds by
        *  'playing' the item via the playlist
        *
        *  @param aPlaylist collection playlist, owner ship is transfered    
        *  @param aError error code   
        */                         
        void HandleOpenL(const CMPXCollectionPlaylist& aPlaylist,TInt aError);            
    
    private:
        /** 
        * C++ default constructor.
        */
        CCollectionBrowser(CConsoleMain* aConsoleMain,
                           CTestBaseView* aParent,
                           const TDesC& aName,
                           CStifLogger* aLogger);

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        */
        void ConstructL(CMPXCollectionPath* aPath);
    
    private:    //data
        MMPXCollectionUtility*  iCollectionUtility;  //owns
        CStifLogger*            iLogger;       //uses
        CMPXCollectionPath*     iCollectionPath;
        CTestPlaybackUtility*   iPlaybackUtility;
        
    };



#endif      //COLLECTIONBROWSER

