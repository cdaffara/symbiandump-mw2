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
* Description:  part of testviewframework.
*
*/

#ifndef TESTMENUVIEW_H
#define TESTMENUVIEW_H

//INCLUDES
#include <badesca.h>
#include "testbaseview.h"


// FORWARD DECLARATION
class CConsoleMain;

// CLASS DECLARATION

/**
*  Abstract View class - contains functions which must be derived
*/
class MTestMenuView
    {
    public:
    
        /**
        * Handle number key press
        * NOTE: Must be derived
        */
        virtual void HandleNumKeyL() = 0;
        
        /**
        * Handle left key press
        * NOTE: Must be derived
        */
        virtual void HandleLeftKeyL() = 0;
        
        /**
        * Handle right/enter key press
        * NOTE: Must be derived
        */
        virtual void HandleRightKeyL() = 0;
    };

/**
*  Menu View class - Console base
*/
class CTestMenuView :public CTestBaseView, public MTestMenuView
    {
    public:
        /**
        * Destructor
        */
        IMPORT_C virtual ~CTestMenuView();

        
    public: //Implement abstract function from base classes
        /** 
        * From: CTestBaseView
        * Display view
        */
        IMPORT_C void DisplayViewL();

        /** 
        * From: CTestBaseView
        * Process keypresses on this menu - called by ConsoleMain
        * @param aSelection, key pressed
        * @param aContinue, whether to continue or stop scheduler
        */
        IMPORT_C void SelectL( TKeyCode aSelection, TBool& aContinue );

        /** 
        * From: CTestBaseView
        * Update display from timer - called by ConsoleMain
        */
        IMPORT_C void TimerUpdate();
    

    protected:
    
        /**
        * C++ default constructor
        */
        IMPORT_C CTestMenuView(CConsoleMain* aConsoleMain,
                         CTestBaseView* aParent,
                         const TDesC& aName);

        
        /**
        * NOTE: This function must be call within deriving class ConstructL
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        * @param aConsole, console to display menu
        * @param aParent, parent menu
        * @param aName, name of current menu to be set
        * @param aLastMenu, whether the current has child menu
        */
        IMPORT_C void ConstructL();
                         
    protected:
        /** 
        * Print line
        * @param aPrint, descriptor of message to be printed
        */
        IMPORT_C void Print( const TDesC& aPrint);

        /** 
        * Multi line print if message exceeds one line
        * @param aPrint, descriptor of message to be printed
        */
        IMPORT_C void PrintMulti( const TDesC& aPrint );

        /** 
        * Add item to menu
        * @param aItem, item of current menu
        */
        IMPORT_C void AddItemL ( const TDesC& aItem );
        
        /** 
        * Delete an item from menu
        * @param aIndex, index of item to be deleted
        */
        IMPORT_C void DeleteItem ( TInt aIndex );

        /**
        * Get last key pressed
        * @return code of last key pressed
        */
        IMPORT_C TKeyCode LastKeyPressed();

        /**
        * Determine whether there are items added to iItems
        * @return whether item is empty
        */
        IMPORT_C TBool ItemsCreated();

        /**
        * Get the position that the cursor is point to in the items
        * @return index of current cursor pointing item in iItems
        */
        IMPORT_C TInt CurrentIndex();

        /**
        * Get the position that the cursor is point to on the screen
        * @return index of current cursor pointing item on screen
        */
        IMPORT_C TInt CurrentPosition();

        /**
        * Get the item
        * @return reference of item
        */
        IMPORT_C TPtrC ItemL(TInt aIndex);

        /**
        * Count the number of items in the current menu
        * @return number of items in the current menu
        */
        IMPORT_C TInt ItemCount() const;
        
        /**
        * Reads user input into the start of the descriptor aDes
        * @param aDes, descriptor to store input string
        */
        IMPORT_C void ReadString( TDes& aDes );
        
    protected:  // New functions

        /**
        * Append text before original text.
        */
        void AppendBefore(  TInt aLineNum, TDes& aLine );

    protected:  // Data
        CDesCArrayFlat*         iItems;         // View items

        TInt                    iPosOnScreen;  // Position on display
        TInt                    iFirst;        // First displayed item
        TInt                    iLast;         // Last displayed item
        TInt                    iItemCount;    // Last Item
        TSize                   iSize;         // Display size
        TInt                    iScreenSize;   // "Work area" size
        TInt                    iDirection;    // Scrolling direction
        TInt                    iStart;        // Scrolling position
        TInt                    iPrevPos;      // Previous position in scrolling
        TKeyCode                iKey;          // Last key pressed

    };


#endif  //TESTMENUVIEW_H
