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
* Description:  CConsoleMain class implementation
*
*/

// INCLUDES
#include "testbaseview.h"
#include "consolereader.h"
#include "scrollertimer.h"
#include "consolemain.h"

// CONSTANTS
_LIT(KNameTxt,"TEST FRAMEWORK");

// ---------------------------------------------------------------------------
//  Class: CConsoleMain
//  Method: NewL
//  Description: Construct the console main class
//  Parameters: None
//  Return Values: CConsoleMain*                    New object
// ---------------------------------------------------------------------------
//
EXPORT_C CConsoleMain* CConsoleMain::NewL(CActiveScheduler* aScheduler)
    {

    CConsoleMain* self = new ( ELeave ) CConsoleMain(aScheduler);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;

    }

// ---------------------------------------------------------------------------
//  Method: ActivateL
//  Description: Activate a view and start the view handling
//  Parameters: aRootView, a root view of test app
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CConsoleMain::ActivateL( CTestBaseView* aRootView )
    {
    
    iReader = CConsoleReader::NewL( this, iConsole );
	iScroller = CScrollerTimer::NewL ( this );

    // Construct keystroke reader
    iScroller->Start();	

    // Print the main menu
    iCurrentView = aRootView;
    iCurrentView->DisplayViewL();

    // Start to process keyboard events
    iReader->Start();
    
    iSchedulerStarted = TRUE;    
    iScheduler->Start();
    }

// ---------------------------------------------------------------------------
//  Method: Stop
//  Description: Stop refreshing menu and the timer
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CConsoleMain::Stop()
    {
    if(iReader)
    	{
    	iReader->Cancel();
    	delete iReader;
    	iReader = NULL;
    	}

	if(iScroller)
		{
		iScroller->Cancel();
		delete iScroller;
    	iScroller = NULL;
		}

    if(iSchedulerStarted)
    	{
    	iScheduler->Stop();
    	iSchedulerStarted = FALSE;
    	}
    }
    
// ---------------------------------------------------------------------------
//  Method: ReadString
//  Description: Reads user input into the start of the descriptor aDes
//  Parameters: TDes& aDes      :out: a user input string
//  Return Values: None
// ---------------------------------------------------------------------------
//
EXPORT_C void CConsoleMain::ReadString( TDes& aDes )
    {
    TChar inputKey;
    TInt count = 0;

    aDes.Zero();
    for (;;)
        {
        inputKey = (TInt) iConsole->Getch();

        if ( inputKey == EKeyEnter || inputKey == EKeyOK )
            {
            break;
            }

        if(inputKey == EKeyBackspace)
            {
            if (count > 0)
                {
                iConsole->Printf(_L("%C"), (TInt)inputKey);
                aDes.Delete(--count,1);
                }
            }
        else if(inputKey.IsPrint())
            {
            iConsole->Printf(_L("%C"), (TInt)inputKey);
            aDes.Append(inputKey);
            count++;
            }
        }
    }

// ---------------------------------------------------------------------------
//  Method: ~CConsoleMain
//  Description: Destructor
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
CConsoleMain::~CConsoleMain( )
    {	
    delete iReader;
    
	delete iScroller;
	
    delete iConsole;
    }
	
// ---------------------------------------------------------------------------
//  Method: CurrentView
//  Description: Return current View
//  Parameters: None
//  Return Values: CTestBaseView                    iCurrentView
// ---------------------------------------------------------------------------
//
CTestBaseView* CConsoleMain::CurrentView()
    {
    return iCurrentView;
    }

// ---------------------------------------------------------------------------
//  Method: GetConsole
//  Description: Returns the console
//  Parameters: None
//  Return Values: CConsoleBase*                    Console
// ---------------------------------------------------------------------------
//
CConsoleBase* CConsoleMain::GetConsole()
    {
    return iConsole;
    }

// ---------------------------------------------------------------------------
//  Method: TimerUpdate
//  Description: Updates current menu from timer
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
void CConsoleMain::TimerUpdate()
    {
	iCurrentView->TimerUpdate();
    }


// ---------------------------------------------------------------------------
//  Method: KeyPressed
//  Description: Process keyboard events. Print new menu
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
void CConsoleMain::KeyPressedL()
    {

    TBool cont = ETrue;

    // Read the key
    TKeyCode key = iConsole->KeyCode();

    // Let the menu handle the key press
    TRAPD( err, 
        iCurrentView->SelectL( key, cont );
        );
    if( err != KErrNone )
        {
        User::InfoPrint( 
            _L("Processing keystroke failed") );  
        }

    if( iCurrentView )
        {
        iCurrentView->DisplayViewL();
        // For next key event
        iReader->Start();
        }
    }
    
// ---------------------------------------------------------------------------
//  Method: CConsoleMain
//  Description: Constructor.
//  Initialize non-zero member variables.
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
CConsoleMain::CConsoleMain(CActiveScheduler* aScheduler)
    {
    iScheduler = aScheduler;
    iSchedulerStarted = FALSE;
    }

// ---------------------------------------------------------------------------
//  Method: ConstructL
//  Description: Second level constructor.
//  Construct the console
//  Construct module and case containers
//  Retrieve command line parameters
//  Connect to test engine
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
void CConsoleMain::ConstructL( )
    {
    // Construct the console
    iConsole = Console::NewL( KNameTxt,
                             TSize( KConsFullScreen, KConsFullScreen ) );

    iConsole->Printf(_L("\nViewTest Framework\n"));
    }

// ---------------------------------------------------------------------------
//  Method: NextViewL
//  Description: activate child view
//  Parameters: aChild, a child view
//  Return Values: None
// ---------------------------------------------------------------------------
//
void CConsoleMain::NextViewL(CTestBaseView* aChild)
    {
    iCurrentView = aChild;
    
    // Start to process keyboard events
    //<-iReader-StartL() will be call in CConsoleMain::KeyPressedL()
    //  otherwise, Panic because of activiting an activated CActive
    //iReader->StartL();    
    }

// ---------------------------------------------------------------------------
//  Method: CurrentViewDoneL
//  Description: Remove current view and active parent view if any
//  Parameters: None
//  Return Values: None
// ---------------------------------------------------------------------------
//
void CConsoleMain::CurrentViewDoneL()
    {
    //iCurrentView = (iCurrentView) ? iCurrentView->Parent() : NULL;
    if(iCurrentView != NULL)
        {
        Stop();
        }
    }





