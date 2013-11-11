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
* Description:  TestView module implementation
*
*/

// INCLUDE FILES
#include <e32base.h>
#include "testBaseview.h"
#include "consolemain.h"

//
//=========================================================================== *
//                    Implementation for Class CTestBaseView                     *
//=========================================================================== *
//

// ---------------------------------------------------------------------------
//  Method: ~CTestBaseView
//  Description: Destructor
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C CTestBaseView::~CTestBaseView()
    {
    if(iChild)
        delete iChild;
    
    }

// ---------------------------------------------------------------------------
//  Method: Name
//  Description: Returns menu name
//  Parameters: None
//  Return Values: TDesC& - View Name
// ---------------------------------------------------------------------------
//
EXPORT_C const TDesC& CTestBaseView::Name( ) const
    {
    return iName;
    }

// ---------------------------------------------------------------------------
//  Method: SetName
//  Description: Sets menu name
//  Parameters: TDesC& aName        :in: Name to be set
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestBaseView::SetName ( const TDesC& aName )
    {
    iName = aName;
    }

// ---------------------------------------------------------------------------
//  Method: SetParent
//  Description: Sets menu parent menu.
//  Parameters: CTestBaseView* aMenu              :in:      Parent View
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestBaseView::SetParent ( CTestBaseView* aView )
    {
    iParent = aView;
    }

// ---------------------------------------------------------------------------
//  Method: Parent
//  Description: Get the parent view
//  Parameters: None
//  Return Values: CTestBaseView*      Parent view
// ---------------------------------------------------------------------------
//
EXPORT_C CTestBaseView* CTestBaseView::Parent()
    {
    return iParent;
    }
    
// ---------------------------------------------------------------------------
//  Method: CTestBaseView
//  Description: Default Constructor
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C CTestBaseView::CTestBaseView(CConsoleMain* aConsoleMain,
                                      CTestBaseView* aParent,
                                      const TDesC& aName)
    {
    iConsole = aConsoleMain->GetConsole();
    iConsoleMain = aConsoleMain;
    iParent = aParent;
    iChild = NULL;
    iName = aName;
    }
    
// ---------------------------------------------------------------------------
//  Method: NextViewL
//  Description: Creating child view
//  Parameters: aChild - a new child view
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestBaseView::NextViewL(CTestBaseView* aChild)
    {
    iConsoleMain->NextViewL(aChild);
    }

// ---------------------------------------------------------------------------
//  Method: NextViewL
//  Description: Creating child view
//  Parameters: aChild - a new child view
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CTestBaseView::CurrentViewDoneL()
    {
    iConsoleMain->CurrentViewDoneL();
    }
    
// ================= OTHER EXPORTED FUNCTIONS =================================

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function
// Returns: KErrNone: No error
// -----------------------------------------------------------------------------
//
#ifndef EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason
    {
    return(KErrNone);
    
    }
#endif
