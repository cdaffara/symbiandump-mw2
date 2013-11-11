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
#include <mpxcollectionpath.h>
#include <stiflogger.h>
#include <e32cmn.h>
#include "testutility.h"
#include "pathoptionisremote.h"

// DATA TYPES
//Entry properties
_LIT(KTestRemoteTitle, "Title: %S");
_LIT(KTestRemoteState, "Remote: %S");
_LIT(KTestRemoteTrue, "YES");
_LIT(KTestRemoteFalse, "NO");

//CONSTANTS
const TInt KMaxStringSize = 256;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPathOptionIsRemote::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPathOptionIsRemote* CPathOptionIsRemote::NewL(CConsoleMain* aConsoleMain,
                                             CTestBaseView* aParent,
                                             const TDesC& aName,
                                             CStifLogger* aLogger,
                                             CMPXCollectionPath* aPath)
    {
    CPathOptionIsRemote* self = new (ELeave) CPathOptionIsRemote(aConsoleMain,
                                                         aParent,
                                                         aName,
                                                         aLogger);
    
    CleanupStack::PushL( self );
    self->ConstructL(aPath);
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
CPathOptionIsRemote::~CPathOptionIsRemote()
    {
    iLogger->Log(_L("+CPathOptionIsRemote::~CPathOptionIsRemote"));
    iCollectionUtility->Close();
    if(iCollectionPath)
        delete iCollectionPath;
    iLogger->Log(_L("-CPathOptionIsRemote::~CPathOptionIsRemote"));
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Initialize the view before display view
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::InitializeViewL()
    {
    iLogger->Log(_L("+CPathOptionIsRemoteL::InitializeViewL"));
    TBuf<KMaxStringSize> outStr;
    // TODO: uncomment this after Collection().EntryL() is implemented
    /*CMPXCollectionEntry* entry = 
        iCollectionUtility->Collection().EntryL(*iCollectionPath);

    if( entry )
        {
        //TODO: delete entry???
        CleanupStack::PushL(entry);
        //Output title
        outStr.Format(KTestRemoteTitle, &(entry->Title()));
        OutputStringL(outStr);
        CleanupStack::PopAndDestroy(entry);
        }*/
    
    TBool remoteState = 
        iCollectionUtility->Collection().IsRemote(*iCollectionPath);
    if(remoteState)
        {
        outStr.Format(KTestRemoteState, &KTestRemoteTrue);
        }
    else
        {
        outStr.Format(KTestRemoteState, &KTestRemoteFalse);
        }
    OutputStringL(outStr);        
    
    iLogger->Log(_L("-CPathOptionIsRemoteL::InitializeViewL"));
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Cleanup the view before deactivate/destroy view
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::CleanupViewL()
    {
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle number key press
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::HandleNumKeyL()
    {
    TKeyCode key = LastKeyPressed();
    }

// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle left key press
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::HandleLeftKeyL()
    {
    }
        
// -----------------------------------------------------------------------------
// From CTestBaseView
// Handle right/enter key press
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::HandleRightKeyL()
    {
    }

// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMediaL
// -----------------------------------------------------------------------------
//
void CPathOptionIsRemote::HandleCollectionMediaL(
                        const CMPXMedia& /*aMedia*/, 
                        TInt /*aError*/)
    {
    
    }
            
// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CPathOptionIsRemote::HandleCollectionMessageL(const TMPXCollectionMessage& /*aMessage*/)
    {
    
    }

// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CPathOptionIsRemote::HandleOpenL(const CMPXMedia& /*aEntries*/,
                                     TInt /*aIndex*/,TBool /*aComplete*/,TInt /*aError*/)
    {
    
    }
        
// -----------------------------------------------------------------------------
// CCollectionBrowser::HandleCollectionMessageL
// -----------------------------------------------------------------------------
//
void CPathOptionIsRemote::HandleOpenL(const CMPXCollectionPlaylist& /*aPlaylist*/,TInt /*aError*/)
    {
    
    }
    
// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
CPathOptionIsRemote::CPathOptionIsRemote(CConsoleMain* aConsoleMain,
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
void CPathOptionIsRemote::ConstructL(CMPXCollectionPath* aPath)
    {
    iLogger->Log(_L("+CPathOptionIsRemote::ConstructL"));
    iCollectionUtility = MMPXCollectionUtility::NewL( this );
    iCollectionPath = CMPXCollectionPath::NewL(*aPath);
    CTestMenuView::ConstructL();
    iLogger->Log(_L("-CPathOptionIsRemote::ConstructL"));
    }
    
// -----------------------------------------------------------------------------
// Output string
// -----------------------------------------------------------------------------
void CPathOptionIsRemote::OutputStringL(const TDesC& aStr)
    {
 //   TestUtility::TestLog(iLogger, _L("%S\n"), &aStr);
    AddItemL(aStr);
    }
    
