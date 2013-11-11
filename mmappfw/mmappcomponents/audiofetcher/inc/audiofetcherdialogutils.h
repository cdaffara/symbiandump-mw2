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
* Description: Utils class for Media Fetch plug-in
*
*/


#ifndef AUDIOFETCHERDIALOGUTILS_H
#define AUDIOFETCHERDIALOGUTILS_H


//  INCLUDES

#include <e32base.h>
#include <s32strm.h>  // RReadStream RWriteStream
#include <f32file.h>  // RFs

#include <eikspane.h>   // CEikStatusPane
#include <aknnavi.h>    // CAknNavigationControlContainer
#include <eikimage.h>   // CEikImage



// CONSTANTS

const TInt KBufSize64  = 64;
const TInt KBufSize128 = 128;
const TInt KBufSize256 = 256;
const TInt KBufSize512 = 512;


//	FORWARD DECLARATIONS

class CAknAppUi;
class CRepository;

/**
* MMFActiveCallerObserver
*
* MMFActiveCallerObserver is for observing CActiveCaller.
*/
NONSHARABLE_CLASS (MActiveCallerObserver)
    {
    public:
	    virtual void HandleActiveCallL( TInt aCallId ) = 0;
    };




/**
* CActiveCaller
*
* CActiveCaller is used for generating a call from active scheduler.
* Typical use is to start some operation after a short delay.
*/
NONSHARABLE_CLASS (CActiveCaller) : public CActive
    {
    public:
        static CActiveCaller* NewL( MActiveCallerObserver* aObserver );
        virtual ~CActiveCaller();

    private:
	    CActiveCaller( MActiveCallerObserver* aObserver );
	    void ConstructL();

	public:
		void Start( TInt aCallId, TInt aMilliseconds );
		void Stop();
		void Request();

    private:
	    void RunL();
	    void DoCancel();
        
    private:
		// call that is passed to observer
		TInt iCallId;

		// timer
		RTimer iTimer;

		// observer that gets called
		MActiveCallerObserver* iObserver;
    };



/**
* CNaviPaneHandler
* 
* This class is used for changing navipane title.
*/
NONSHARABLE_CLASS (CNaviPaneHandler) : public CBase
    {
    public:
        /**
        * Destructor.
        */
        virtual ~CNaviPaneHandler();

        /**
        * C++ default constructor.
        */
        CNaviPaneHandler(CEikStatusPane* aStatusPane);

    public:

        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL(const TDesC& aTitle);
        
        /**
        * Store navi pane.
        * @param None.
        * @return None.
        */
        void StoreNavipaneL();

    private:
        // status pane
        CEikStatusPane* iStatusPane;
    
        // navi Pane
        CAknNavigationControlContainer* iNaviPane;
        
        // navi decorator
        CAknNavigationDecorator* iNaviDecorator;
        
        // has Navidecorator been pushed into navi pane
        TBool iNavidecoratorPushed;
        
        // has navi pane been pushed into navigation pane's object stack
        TBool iNavipanePushed;
    };



/**
* CStatusPaneHandler
* 
* This class is used for changing dialog title and image.
*/
NONSHARABLE_CLASS (CStatusPaneHandler) : public CBase
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CStatusPaneHandler* NewL( CAknAppUi* aAppUi );

        /**
        * Destructor.
        */
        virtual ~CStatusPaneHandler();

    private:
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * C++ default constructor.
        */
        CStatusPaneHandler( CAknAppUi* aAppUi );

    public:
        /**
        * Stores original title so it can be restored when dialog closes.
        * @param  None.
        * @return None.
        */
        void StoreOriginalTitleL();

        /**
        * Restores original title.
        * @param  None.
        * @return None.
        */
        void RestoreOriginalTitleL();

        /**
        * Sets dialog title.
        * @param  aText.
        * @return None.
        */
        void SetTitleL( const TDesC& aText );

        /**
        * Sets dialog title.
        * @param  aResourceId.
        * @return None.
        */
        void SetTitleL( TInt aResourceId );

    public:
        /**
        * Sets dialog title.
        * @param aAppUi.
        * @param aText.
        * @return Return code.
        */
        static TBool SetTitleL( CAknAppUi* aAppUi, const TDesC& aText );

        /**
        * Gets dialog title.
        * @param aAppUi.
        * @param aText.
        * @return Return code.
        */
        static TBool GetTitleL( CAknAppUi* aAppUi, TDes& aText );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL( const TDesC& aTitle );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitle( const TDesC& aTitle );
        
        /**
        * Sets navi pane title.
        * @param aTitle.
        * @return None.
        */
        void SetNaviPaneTitleL( TInt aResource );
        
        /**
        * Store navi pane.
        * @param None.
        * @return None.
        */
        void StoreNavipaneL();
        
        static void StrCopy( TDes& aTarget, const TDesC& aSource );

    private:
        // access to app ui
        CAknAppUi* iAppUi;

        // original status pane title
        TBuf<KBufSize256> iOriginalTitle;
        
        // is original status pane title stored
        TBool iOriginalTitleStored;

        // original context pane image
        CEikImage* iOriginalImage;
        
        CNaviPaneHandler* iNaviPaneHandler;
    };




/**
* CDriveUtil
*
* CDriveUtil is used for getting drive information.
*/
NONSHARABLE_CLASS (CDriveUtil) : public CBase
    {
    public:
        static CDriveUtil* NewL();
        virtual ~CDriveUtil();

    private:
        CDriveUtil();
        void ConstructL();

    public:
        TBool IsMemoryCard( const TDesC& aFullPath );
        TBool IsMassStorage( const TDesC& aFullPath );
        TBool IsRom( const TDesC& aFullPath );
        
    private:
        RFs iFsSession;
    };
            
#endif // AUDIOFETCHERDIALOGUTILS_H

// End of File
