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
* Description:  CScrollerTimer class implementation
*
*/

#include "scrollertimer.h"
#include "consolemain.h"

// CONSTANTS
const TInt KScrollPeriod = 300000;		   // Scrolling speed

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: ConstructL

    Description: Second level constructor

    Parameters: CConsoleMain*             :in:      Pointer to main console

    Return Values: None

    Errors/Exceptions: None

    Status: Draft

-------------------------------------------------------------------------------
*/
void CScrollerTimer::ConstructL( CConsoleMain* aMain )
    {

    // Store module information
    iMain = aMain;
	iTimer.CreateLocal();

	CActiveScheduler::Add ( this );

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: CScrollerTimer

    Description: Constructor.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
CScrollerTimer::CScrollerTimer() : CActive (CActive::EPriorityStandard)
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: ~CScrollerTimer

    Description: None

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
CScrollerTimer::~CScrollerTimer( )
    {

	Cancel();
	iTimer.Close();

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: DoCancel

    Description: None

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
void CScrollerTimer::DoCancel( )
    {

	iTimer.Cancel();

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: NewL

    Description: Construct a new CScrollerTimer object

    Parameters: CConsoleMain*             :in:      Pointer to main console               

    Return Values: CScrollerTimer*                         New CScrollerTimer object

    Errors/Exceptions: Leaves if memory allocation or ConstructL leaves

    Status: Draft

-------------------------------------------------------------------------------
*/
CScrollerTimer* CScrollerTimer::NewL( CConsoleMain* aMain )
    {

    CScrollerTimer* self = new ( ELeave ) CScrollerTimer();
    CleanupStack::PushL( self );
    self->ConstructL( aMain );
    CleanupStack::Pop( self );
    return self;

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: RunError

    Description: None

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
TInt CScrollerTimer::RunError( TInt aError)
    {

	return aError;

    }    

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: RunL

    Description: None

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
void CScrollerTimer::RunL( )
    {

	iMain->TimerUpdate();

	// Restart request
	SetActive();
	iTimer.After ( iStatus, KScrollPeriod );

    }

/*
-------------------------------------------------------------------------------

    Class: CScrollerTimer

    Method: StartL

    Description: None

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Draft
    
-------------------------------------------------------------------------------
*/
void CScrollerTimer::Start( )
    {

	SetActive();
	iTimer.After ( iStatus, KScrollPeriod );	

    }
