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

#ifndef PATHOPTIONMENU_H
#define PATHOPTIONMENU_H

// INCLUDES
#include <mpxcollectionobserver.h>
#include "testmenuview.h"


// MACROS

// FORWARD DECLARATIONS
class MMPXCollectionUtility;
class CMPXCollectionPath;
class CStifLogger;

// DATA TYPES
_LIT(KTxtPathOptionMenu, "PathOptionMenu");
    
// CLASS DECLARATION
class CPathOptionMenu : public CTestMenuView
    {
    public:
        /**
        * Two-phased constructor
        * @return object of CConsoleMain
        */
        static CPathOptionMenu* NewL(CConsoleMain* aConsoleMain,
                                        CTestBaseView* aParent,
                                        const TDesC& aName,
                                        CStifLogger* aLogger);

        /**
        * Destructor of CConsoleMain
        */
        virtual ~CPathOptionMenu();
        
        /**
        * Set the handling path
        */
        void SetPathL(CMPXCollectionPath& aPath);
        
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
        
        
    private:
        /** 
        * C++ default constructor.
        */
        CPathOptionMenu(CConsoleMain* aConsoleMain,
                           CTestBaseView* aParent,
                           const TDesC& aName,
                           CStifLogger* aLogger);

        /**
        * the second phase constructor ConstructL to safely construct things 
        * that can leave 
        */
        void ConstructL();
    
    private:    //data
        CStifLogger*            iLogger;       //uses
        CMPXCollectionPath*     iCollectionPath;
        

    };

#endif  //PATHOPTIONMENU_H
