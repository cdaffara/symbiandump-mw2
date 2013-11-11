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
* Description:  console manipulation
*
*/

#ifndef CCONSOLEMAIN_H
#define CCONSOLEMAIN_H

// INCLUDE FILES
#include <aknlists.h>
#include <e32cons.h>
#include <apaflrec.h>
#include <e32base.h>
#include <e32def.h>

// FORWARD DECLARATIONS
class CTestBaseView;
class CScrollerTimer;
class CConsoleReader;

/**
*  console manipulation class 
*
*  @lib TestViewFramework.lib
*/
class CConsoleMain :public CBase
    {
    public:  

        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
        IMPORT_C static CConsoleMain* NewL(CActiveScheduler* aScheduler);

        /**
        * Destructor of CConsoleMain
        */
        virtual ~CConsoleMain();
        
        /**
        * Start
        * @param aView, activate a root view in console
        */
        IMPORT_C void ActivateL( CTestBaseView* aRootView );

        /**
        * Stop ScrollerTimer and ConsoleReader
        */
        IMPORT_C void Stop();

        /**
        * Reads user input into the start of the descriptor aDes
        * @param aDes, descriptor to store input string
        */
        IMPORT_C void ReadString( TDes& aDes );

        

    public: // New functions
		
        /**
        * Return current menu
        * @return reference of current shown menu
        */
        CTestBaseView* CurrentView();
    
        /**
        * Return pointer to console
        * @return reference to console
        */
        CConsoleBase* GetConsole();

        /**
        * Called from timer to perform scrolling
        */
        void TimerUpdate();

        /**
        * Process keystokes.
        */
        void KeyPressedL();

    private:  //Friend class
        friend class CTestBaseView;

    private:

        /** 
        * C++ default constructor.
        */
        CConsoleMain(CActiveScheduler* aScheduler);

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        */
        void ConstructL();
        
        /**
        * Call by CTestBaseView
        * Actiavte the child view
        * @param aChild, a new child view 
        */
        void NextViewL(CTestBaseView* aChild);
        
        /**
        * Call by CTestBaseView
        * Indicate the current is done, go back parent view
        */
        void CurrentViewDoneL();
        
    private:    // Data
        CConsoleBase*       iConsole;       // Pointer to console

        CTestBaseView*      iCurrentView;   // Current view

        CScrollerTimer*     iScroller;      // Scroll text
        
        CConsoleReader*     iReader;        // Console reader
        
        CActiveScheduler*   iScheduler;     // Uses an active scheduler   
    
    	TBool				iSchedulerStarted;
    };

#endif
