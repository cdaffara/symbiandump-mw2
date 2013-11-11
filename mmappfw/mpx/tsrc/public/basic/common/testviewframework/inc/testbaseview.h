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
* Description:  A console for menu selection
*
*/

#ifndef TESTBASEVIEW_H
#define TESTBASEVIEW_H

// INCLUDE FILES
#include <aknlists.h>
#include <badesca.h>
#include <e32cons.h>

// FORWARD DECLARATION
template <class T> class RRefArray;
class CConsoleMain;

// CLASS DECLARATION

/**
*  Abstract View class - contains functions which must be derived
*
*  @lib TestViewFramework.lib
*/
class MTestBaseView
    {
    public:
    
        /**
        * Initialize the view before display view
        * NOTE: Must be derived
        */
        virtual void InitializeViewL() = 0;
        
        /**
        * Cleanup the child view before deactivate/destroy view
        * NOTE: Must be derived
        */
        virtual void CleanupViewL() = 0;
        
        /**
        * Display this view 
        * NOTE: Must be derived
        */
        virtual void DisplayViewL() = 0;
        
        /** 
        * Process keypresses on this view - called by ConsoleMain
        * @param aSelection, key pressed
        * @param aContinue, whether to continue or stop scheduler
        * NOTE: Must be derived
        */
        virtual void SelectL( TKeyCode aSelection, TBool& aContinue ) = 0;
        
        /** 
        * Update display from timer - called by ConsoleMain
        * NOTE: Must be derived
        */
        virtual void TimerUpdate() = 0;
        
    };


/**
*  View class - Console base
*
*  @lib TestViewFramework.lib
*/
class CTestBaseView :public CBase, public MTestBaseView
    {
    public:
        /**
        * Destructor
        */
        IMPORT_C virtual ~CTestBaseView();

        /** 
        * Return menu name
        * @return descriptor of current menu's name
        */
        IMPORT_C const TDesC& Name() const;

        /** 
        * Set the menu name
        * @param aName, menu name to be set
        */
        IMPORT_C void SetName( const TDesC& aName );

        /** 
        * Set parent menu
        * @param aMenu, parent menu of current menu
        */
        IMPORT_C void SetParent( CTestBaseView* aView );
        
        /**
        * Get the parent view
        * @return reference of parent view
        */
        IMPORT_C CTestBaseView* Parent();
        
        
    protected:
        /**
        * Make sure DisplayViewBase() is call in the beginning
        * of DisplayViewL() call
        */
        void DisplayViewBase()
            { if(iChild) { CleanupViewL(); delete iChild; iChild = NULL; }};
        
        /**
        * C++ default constructor
        */
        IMPORT_C CTestBaseView(CConsoleMain* aConsoleMain,
                               CTestBaseView* aParent,
                               const TDesC& aName);

        /**
        * Signal CConsoleMain that a new child view is created
        */
        IMPORT_C void NextViewL(CTestBaseView* aChild);
        
        /**
        * Signal CConsoleMain that current view is done, go back parent
        */
        IMPORT_C void CurrentViewDoneL();

    protected:  // Data
        CConsoleBase*           iConsole;      // Pointer to console
        CConsoleMain*           iConsoleMain;  // Pointer to console main
        CTestBaseView*          iParent;       // Pointer to parent view
        CTestBaseView*          iChild;         //Pointer to child view
        TName                   iName;         // View name

    };

#endif  //TESTVIEW_H
