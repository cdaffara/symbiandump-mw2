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
#include <stiflogger.h>
#include <mpxcollectionpath.h>
#include "collectionbrowser.h"
#include "pathoptionmenu.h"
#include "pathoptionisremote.h"

// MACROS

// DATA TYPES
//Option name - corresponding to each path specific API
_LIT(KTestOptionOpenL, "OpenL");                    //0
_LIT(KTestOptionIsRemote, "IsRemote");              //1


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPathOptionMenu::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPathOptionMenu* CPathOptionMenu::NewL(CConsoleMain* aConsoleMain,
                                             CTestBaseView* aParent,
                                             const TDesC& aName,
                                             CStifLogger* aLogger)
    {
    CPathOptionMenu* self = new (ELeave) CPathOptionMenu(aConsoleMain,
                                                         aParent,
                                                         aName,
                                                         aLogger);
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CPathOptionMenu::~CPathOptionMenu()
    {
    iLogger->Log(_L("+CPathOptionMenu::~CPathOptionMenu"));
    if(iCollectionPath)
        delete iCollectionPath;
    iLogger->Log(_L("-CPathOptionMenu::~CPathOptionMenu"));
    }

// -----------------------------------------------------------------------------
// CPathOptionMenu::SetPathL
// -----------------------------------------------------------------------------
void CPathOptionMenu::SetPathL(CMPXCollectionPath& aPath)
    {
    iCollectionPath = CMPXCollectionPath::NewL(aPath);
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Initialize the view before display view
// -----------------------------------------------------------------------------
void CPathOptionMenu::InitializeViewL()
    {
    iLogger->Log(_L("+CPathOptionMenu::InitializeViewL"));
    //Add option name
    AddItemL(KTestOptionOpenL);             //0
    AddItemL(KTestOptionIsRemote);          //1
    iLogger->Log(_L("-CPathOptionMenu::InitializeViewL"));
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Cleanup the view before deactivate/destroy view
// -----------------------------------------------------------------------------
void CPathOptionMenu::CleanupViewL()
    {
    }
    
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle number key press
// -----------------------------------------------------------------------------
void CPathOptionMenu::HandleNumKeyL()
    {
    TKeyCode key = LastKeyPressed();
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle left key press
// -----------------------------------------------------------------------------
void CPathOptionMenu::HandleLeftKeyL()
    {
    }
        
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle right/enter key press
// -----------------------------------------------------------------------------
void CPathOptionMenu::HandleRightKeyL()
    {
    iLogger->Log(_L("+CPathOptionMenu::HandleRightKeyL"));
    switch( CurrentIndex() )
        {
        case 0:
//            iChild = 
 //               CCollectionBrowser::NewL(iConsoleMain,
  //                                   this,
   //                                  KTxtCollectionBrowser,
    //                                 iLogger,
     //                                iCollectionPath);
            break;
        case 1:
            iChild = 
                CPathOptionIsRemote::NewL(iConsoleMain,
                                     this,
                                     KTxtPathOptionIsRemote,
                                     iLogger,
                                     iCollectionPath);                         
            break;
        }
    NextViewL(iChild);
    iLogger->Log(_L("-CPathOptionMenu::HandleRightKeyL"));
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
CPathOptionMenu::CPathOptionMenu(CConsoleMain* aConsoleMain,
                                       CTestBaseView* aParent,
                                       const TDesC& aName,
                                       CStifLogger* aLogger)
    : CTestMenuView(aConsoleMain, aParent, aName)
    {
    iLogger = aLogger;
    iCollectionPath = NULL;
    }

// -----------------------------------------------------------------------------
// Second phrase constructor
// -----------------------------------------------------------------------------
void CPathOptionMenu::ConstructL()
    {
    iLogger->Log(_L("+CPathOptionMenu::ConstructL"));
    CTestMenuView::ConstructL();
    iLogger->Log(_L("-CPathOptionMenu::ConstructL"));
    }
    
