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
* Description:  CConsoleReader class implementation
*
*/

#include "consolereader.h"
#include "consolemain.h"

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: CConsoleReader

    Description: Constructor.

    Parameters: CConsoleMain* aMain: in: Pointer to main console
                CConsoleBase* aConsole: in: Console pointer

    Return Values: None

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
CConsoleReader::CConsoleReader( CConsoleMain* aMain, 
                                CConsoleBase* aConsole ): 
    CActive( EPriorityStandard )
    {
    
    iMain = aMain;
    iConsole = aConsole;
    
    CActiveScheduler::Add( this );

    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: ~CConsoleReader

    Description: Destructor

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
CConsoleReader::~CConsoleReader( )
    {	
    
    Cancel();    
    
    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: ConstructL

    Description: Second level constructor.
    
    Parameters: None

    Return Values: None

    Errors/Exceptions: 

    Status: Draft

-------------------------------------------------------------------------------
*/
void CConsoleReader::ConstructL( )
    {
    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: DoCancel

    Description: Cancel request

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
void CConsoleReader::DoCancel()
    {
    
    iConsole->ReadCancel();
    
    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: NewL

    Description: Construct the console main class

    Parameters: CConsoleMain* aMain: in: Pointer to main console
                CConsoleBase* aConsole: in: Console pointer

    Return Values: CConsoleReader*                    New object

    Errors/Exceptions: Leaves if memory allocation fails or
                       ConstructL leaves.

    Status: Draft

-------------------------------------------------------------------------------
*/
CConsoleReader* CConsoleReader::NewL( CConsoleMain* aMain, 
                                      CConsoleBase* aConsole )
    {

    CConsoleReader* self = 
        new ( ELeave ) CConsoleReader( aMain, aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;

    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: RunL

    Description: Process keyboard events. Print new menu

    Parameters: None

    Return Values: None

    Errors/Exceptions: Leaves if SelectL leaves

    Status: Draft

-------------------------------------------------------------------------------
*/
void CConsoleReader::RunL()
    {

	User::LeaveIfError(iStatus.Int());
    iMain->KeyPressedL();
    
    }

/*
-------------------------------------------------------------------------------

    Class: CConsoleReader

    Method: Start

    Description: Construct menu objects and start the menu handling

    Parameters: None

    Return Values: None

    Errors/Exceptions:

    Status: Draft

-------------------------------------------------------------------------------
*/
void CConsoleReader::Start( )
    {

    // Start to process keyboard events
    iConsole->Read(iStatus);
    SetActive();
    }
